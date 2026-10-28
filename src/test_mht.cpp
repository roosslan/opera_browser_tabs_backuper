#include <vcl.h>
#pragma hdrstop

#include <memory>
#include <vector>
#include "test_mht.h"
#include "unit_list.h"
#include "session_parser.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

Twindow_ami *window_ami;

__fastcall Twindow_ami::Twindow_ami(TComponent* Owner) : TForm(Owner) {
}

void __fastcall Twindow_ami::FormCreate(TObject *Sender) {

    txt_about->Caption = "Rasa (absoluteopenbsd@ya.ru) | blddt 30 09 06 20 10";

    const AnsiString default_directory = "C:\\Program Files\\Opera\\profile\\sessions";
    combobox_opera_directory->Items->Add(default_directory);
    combobox_opera_directory->Items->Add(ExtractFileDir(ParamStr(0)));
    combobox_opera_directory->Text = default_directory;

    std::auto_ptr<TRegistry> reg(new TRegistry());
    reg->RootKey = HKEY_CURRENT_USER;
    if (reg->OpenKeyReadOnly("Software\\Opera Software") && reg->ValueExists("Last CommandLine")) {
        const AnsiString session_directory =
            ExtractFileDir(reg->ReadString("Last CommandLine")) + "\\profile\\sessions";
        combobox_opera_directory->Text = session_directory;
        combobox_opera_directory->Items->Add(session_directory);
    }

    edit_out_directory->Text = ExtractFileDir(ParamStr(0)) + "\\mht";
}

void __fastcall Twindow_ami::btn_sel_directoryClick(TObject *Sender) {

    AnsiString directory = combobox_opera_directory->Text;
    if (SelectDirectory("Укажите директорию", "", directory))
        combobox_opera_directory->Text = directory;
}

void __fastcall Twindow_ami::FormKeyPress(TObject *Sender, char &Key) {

    if (Key == VK_ESCAPE)
        Close();
}

void __fastcall Twindow_ami::btn_analyseClick(TObject *Sender) {

    const AnsiString session_file =
        IncludeTrailingBackslash(combobox_opera_directory->Text.Trim()) + "autosave.win";
    if (!FileExists(session_file)) {
        MessageDlg("Файл сессии не найден:\n" + session_file, mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    const AnsiString out_directory = ExcludeTrailingBackslash(edit_out_directory->Text.Trim());
    if (out_directory.IsEmpty()) {
        MessageDlg("Не указана директория для сохранения файлов", mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }
    if (!DirectoryExists(out_directory) && !ForceDirectories(out_directory)) {
        MessageDlg("Не удалось создать директорию:\n" + out_directory, mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    std::vector<page_entry> pages;
    read_opera_session(session_file, checkbox_get_all_history->Checked, pages);

    window_session_list->set_pages(pages, out_directory);
    window_session_list->ShowModal();
}

void __fastcall Twindow_ami::btn_sel_out_directoryClick(TObject *Sender) {
    AnsiString out_dir = edit_out_directory->Text;
    if (SelectDirectory("Укажите директорию", "", out_dir))
        edit_out_directory->Text = out_dir;
}

void __fastcall Twindow_ami::menu_greetingsClick(TObject *Sender) {
    ShowMessage("Спасибо красавице Microsoft Keyboard Elite");
}
