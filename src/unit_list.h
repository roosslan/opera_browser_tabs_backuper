#ifndef unit_list
#define unit_list

#include "pch.h"
#include <vector>
#include "session_parser.h"

class Tsave_thread;

class Twindow_session_list : public TForm
{
__published:
        TLabel *txt_info;
        TButton *btn_go;
        TButton *btn_sel_all;
        TLabel *txt_count;
        TLabel *txt_result;
        TLabel *txt_size;
        TCheckListBox *checkbox_list_main;
        TProgressBar *progress_bar;

    void __fastcall btn_sel_allClick(TObject *Sender);
    void __fastcall btn_goClick(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);

private:
    // Страницы из сессии; индексы совпадают со строками checkbox_list_main
    std::vector<page_entry> pages_;
    AnsiString out_directory_;
    // Поток сохранения; NULL, если сохранение не идёт
    Tsave_thread* save_thread_;

    void __fastcall save_threadTerminate(TObject *Sender);
    void set_running(bool running);
    void stop_saving();
public:
    __fastcall Twindow_session_list(TComponent* Owner);

    // Заполняет список страниц перед показом окна
    void set_pages(const std::vector<page_entry>& pages, const AnsiString& out_directory);
    // Обновляет индикатор хода сохранения (вызывается в главном потоке)
    void show_progress(int done, int total, __int64 bytes);
};

extern PACKAGE Twindow_session_list *window_session_list;

#endif
