#include <vcl.h>
#pragma hdrstop

#include <memory>
#include <objbase.h>
#include "unit_list.h"
#include "save_mht.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

Twindow_session_list *window_session_list;

namespace {

// Страница, которую нужно сохранить, и путь к файлу для неё
struct save_task {
    AnsiString url;
    AnsiString path;
};

// Сколько ошибок перечислять в итоговом сообщении
const int max_errors_shown = 15;

__int64 get_file_size(const AnsiString& path) {
    TSearchRec sr;
    if (FindFirst(path, faAnyFile, sr) != 0)
        return 0;
    const __int64 size = sr.Size;
    FindClose(sr);
    return size;
}

AnsiString format_size(__int64 bytes) {
    if (bytes < 1024 * 1024)
        return FormatFloat("0.0", bytes / 1024.0) + " КБ";
    return FormatFloat("0.0", bytes / (1024.0 * 1024.0)) + " МБ";
}

} // namespace

// Фоновое сохранение страниц, чтобы окно не зависало на время загрузки
class Tsave_thread : public TThread {
    Twindow_session_list* owner_;
    std::vector<save_task> tasks_;
    int done_;
    __int64 bytes_;

    // Вызывается через Synchronize в главном потоке
    void __fastcall report_progress() {
        owner_->show_progress(done_, total(), bytes_);
    }

protected:
    virtual void __fastcall Execute() {
        CoInitialize(NULL);
        try {
            for (unsigned i = 0; i < tasks_.size() && !Terminated; ++i) {
                AnsiString error_message;
                if (save2mht(tasks_[i].url, tasks_[i].path, error_message)) {
                    ++saved;
                    bytes_ += get_file_size(tasks_[i].path);
                }
                else
                    errors->Add(tasks_[i].url + " — " + error_message);
                done_ = i + 1;
                Synchronize(report_progress);
            }
        }
        __finally {
            CoUninitialize();
        }
    }

public:
    TStringList* errors;
    int saved;

    __fastcall Tsave_thread(Twindow_session_list* owner, const std::vector<save_task>& tasks)
        : TThread(true), owner_(owner), tasks_(tasks), done_(0), bytes_(0),
          errors(new TStringList()), saved(0) {
        FreeOnTerminate = true;
    }

    __fastcall ~Tsave_thread() {
        delete errors;
    }

    int total() const {
        return tasks_.size();
    }

    bool stopped() const {
        return Terminated && done_ < total();
    }
};

__fastcall Twindow_session_list::Twindow_session_list(TComponent* Owner)
    : TForm(Owner), save_thread_(NULL) {
}

void Twindow_session_list::set_pages(const std::vector<page_entry>& pages, const AnsiString& out_directory) {
    pages_ = pages;
    out_directory_ = out_directory;

    checkbox_list_main->Items->BeginUpdate();
    try {
        checkbox_list_main->Clear();
        for (unsigned i = 0; i < pages_.size(); ++i) {
            const page_entry& page = pages_[i];
            checkbox_list_main->Items->Add(page.title.IsEmpty() ? page.url : page.title + "  —  " + page.url);
            checkbox_list_main->Checked[i] = true;
        }
    }
    __finally {
        checkbox_list_main->Items->EndUpdate();
    }

    txt_count->Caption = "Количество адресов: " + IntToStr(checkbox_list_main->Count);
    txt_result->Caption = "";
    txt_size->Caption = "";
    progress_bar->Position = 0;
}

void Twindow_session_list::show_progress(int done, int total, __int64 bytes) {
    progress_bar->Position = 0;
    progress_bar->Max = total;
    progress_bar->Position = done;
    txt_result->Caption = "Обработано " + IntToStr(done) + " из " + IntToStr(total);
    txt_size->Caption = "Общий объём: " + format_size(bytes);
}

void Twindow_session_list::set_running(bool running) {
    checkbox_list_main->Enabled = !running;
    btn_sel_all->Enabled = !running;
    btn_go->Caption = running ? "Стоп" : "Старт";
    btn_go->Enabled = true;
}

void Twindow_session_list::stop_saving() {
    save_thread_->Terminate();
    btn_go->Enabled = false;
    txt_result->Caption = "Остановка после текущей страницы...";
}

void __fastcall Twindow_session_list::btn_goClick(TObject *Sender) {

    if (save_thread_ != NULL) {
        stop_saving();
        return;
    }

    std::vector<save_task> tasks;
    std::auto_ptr<TStringList> used_paths(new TStringList());
    for (int i = 0; i < checkbox_list_main->Count; ++i) {
        if (!checkbox_list_main->Checked[i])
            continue;
        save_task task;
        task.url = pages_[i].url;
        task.path = make_unique_path(out_directory_, make_file_name(pages_[i]), used_paths.get());
        tasks.push_back(task);
    }

    if (tasks.empty()) {
        MessageDlg("Не отмечено ни одной страницы", mtInformation, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    save_thread_ = new Tsave_thread(this, tasks);
    save_thread_->OnTerminate = save_threadTerminate;
    set_running(true);
    show_progress(0, tasks.size(), 0);
    save_thread_->Resume();
}

void __fastcall Twindow_session_list::save_threadTerminate(TObject *Sender) {

    // Поток удаляется сам (FreeOnTerminate) сразу после этого обработчика
    Tsave_thread* thread = static_cast<Tsave_thread*>(Sender);
    save_thread_ = NULL;
    set_running(false);

    AnsiString message = "Сохранено файлов: " + IntToStr(thread->saved) + " из " + IntToStr(thread->total());
    if (thread->stopped())
        message += "\nСохранение прервано.";

    TStringList* errors = thread->errors;
    if (errors->Count > 0) {
        message += AnsiString("\n\nОшибки (") + IntToStr(errors->Count) + "):";
        for (int i = 0; i < errors->Count && i < max_errors_shown; ++i)
            message += AnsiString("\n") + errors->Strings[i];
        if (errors->Count > max_errors_shown)
            message += "\n...";
    }

    MessageDlg(message, errors->Count > 0 ? mtWarning : mtInformation, TMsgDlgButtons() << mbOK, 0);
}

void __fastcall Twindow_session_list::FormKeyPress(TObject *Sender, char &Key) {
    if (Key == VK_ESCAPE)
        Close();
}

void __fastcall Twindow_session_list::FormCloseQuery(TObject *Sender, bool &CanClose) {
    // Пока идёт сохранение, окно не закрывается: попытка закрытия останавливает поток
    CanClose = (save_thread_ == NULL);
    if (!CanClose && btn_go->Enabled)
        stop_saving();
}

void __fastcall Twindow_session_list::btn_sel_allClick(TObject *Sender) {
    // Если отмечено всё — снимает все отметки, иначе отмечает все строки
    bool all_checked = true;
    for (int i = 0; i < checkbox_list_main->Count && all_checked; ++i)
        all_checked = checkbox_list_main->Checked[i];

    for (int i = 0; i < checkbox_list_main->Count; ++i)
        checkbox_list_main->Checked[i] = !all_checked;
}
