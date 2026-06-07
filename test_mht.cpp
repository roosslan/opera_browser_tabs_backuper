#include <vcl.h>
#pragma hdrstop

#include "test_mht.h"
#include "unit_list.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

Twindow_ami *window_ami;
AnsiString session_directory;
TIniFile *ini_file;

__fastcall Twindow_ami::Twindow_ami(TComponent* Owner) : TForm(Owner) {
}

void __fastcall Twindow_ami::btn_call_using_threadClick(TObject *Sender) {
    // save_to_mht_in_cothread("http://www.eggcentric.com/", "C:\\test.mht");
    // ShowMessage("Done");
}

void __fastcall Twindow_ami::FormCreate(TObject *Sender) {

    txt_about->Caption = "Rasa (absoluteopenbsd@ya.ru) | blddt 30 09 06 20 10";

    session_directory = "C:\\Program Files\\Opera\\profile\\sessions";
    combobox_opera_directory->Items->Add(session_directory);
    combobox_opera_directory->Items->Add(ExtractFileDir(ParamStr(0)));

    TRegistry *reg = new TRegistry();
    reg->RootKey = HKEY_CURRENT_USER;
    if (reg->KeyExists("Software\\Opera Software"))
    {
        reg->OpenKeyReadOnly("Software\\Opera Software");
        if (reg->ValueExists("Last CommandLine"))
        {
            session_directory = ExtractFileDir(reg->ReadString("Last CommandLine")) + "\\profile\\sessions";
            combobox_opera_directory->Text = session_directory;
            combobox_opera_directory->Items->Add(session_directory);
        }
    }
    delete reg;
}

void __fastcall Twindow_ami::btn_sel_directoryClick(TObject *Sender) {

    if (SelectDirectory("Укажите директорию", "", session_directory))
        combobox_opera_directory->Text = session_directory;
}

void __fastcall Twindow_ami::FormKeyPress(TObject *Sender, char &Key) {

    if (Key == VK_ESCAPE)
        Close();
}

/* Helper function: get last URL index in session section */
AnsiString get_last_url(const AnsiString &sect) {

    int ret = ini_file->ReadInteger(sect, "Count", 1);
    return IntToStr(ret - 1);
}

/* Helper function: remove characters not allowed in Windows filenames */
AnsiString remove_non_windows_symbols(const AnsiString& path) {

    AnsiString res = "";
    for (int i = 1; i <= path.Length(); ++i)
    {
        char ch = path[i];
        if ((ch >= 'A' && ch <= 'Z') ||
            (ch >= 'a' && ch <= 'z') ||
            (ch >= '0' && ch <= '9') ||
            ch == '_')
        {
            res += ch;
        }
    }
    return res;
}

void __fastcall Twindow_ami::btn_analyseClick(TObject *Sender) {

    window_session_list->checkbox_list_main->Clear();

    ini_file = new TIniFile(combobox_opera_directory->Text + "\\autosave.win");
    int i_counter = 1;

    AnsiString url, title;
    TGUID mht_guid;
    do {
        i_counter++;

        if (checkbox_get_all_history->Checked) {
            int maxCount = ini_file->ReadInteger(IntToStr(i_counter) + "history url", "Count", 1);
            for (int i = 0; i <= maxCount; ++i) {
                if (CreateGUID(mht_guid) == 0)
                title = remove_non_windows_symbols(
                    Utf8ToAnsi(ini_file->ReadString(IntToStr(i_counter) + "history title",
                                                    IntToStr(i), "1"))) + "__" + Sysutils::GUIDToString(mht_guid);

                url = ini_file->ReadString(IntToStr(i_counter) + "history url",
                                                      IntToStr(i), "1") + "{/MAR|_777_|IN\\}" + title;

                if (url.SubString(1, 4) == "http")
                    window_session_list->checkbox_list_main->Items->Add(url);
            }
        }

        if (!checkbox_get_all_history->Checked)
        {
            if (CreateGUID(mht_guid) == 0)
            title = remove_non_windows_symbols(
                Utf8ToAnsi(ini_file->ReadString(IntToStr(i_counter) + "history title",
                                                get_last_url(IntToStr(i_counter) + "history url"),
                                                "1"))) + "__" + Sysutils::GUIDToString(mht_guid);

            url = ini_file->ReadString(IntToStr(i_counter) + "history url",
                                                  get_last_url(IntToStr(i_counter) + "history url"),
                                                  "1") + "{/MAR|_777_|IN\\}" + title;

            if (url.SubString(1, 4) == "http")
                window_session_list->checkbox_list_main->Items->Add(url);
        }

    } while (ini_file->SectionExists(IntToStr(i_counter) + "history url"));

    delete ini_file;

    window_session_list->txt_count->Caption = "Количество адресов: " + IntToStr(window_session_list->checkbox_list_main->Count);
    window_session_list->ShowModal();
}

void __fastcall Twindow_ami::btn_sel_out_directoryClick(TObject *Sender) {
    AnsiString out_dir;
    if (SelectDirectory("Укажите директорию", "", out_dir))
        edit_out_directory->Text = out_dir;
}

void __fastcall Twindow_ami::menu_greetingsClick(TObject *Sender) {
    ShowMessage("Спасибо красавице Microsoft Keyboard Elite");
}