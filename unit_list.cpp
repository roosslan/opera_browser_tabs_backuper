#include <vcl.h>
#pragma hdrstop

#include "unit_list.h"
#include "test_mht.h"
#include "save_mht.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

Twindow_session_list *window_session_list;

__fastcall Twindow_session_list::Twindow_session_list(TComponent* Owner) : TForm(Owner) {
}

void __fastcall Twindow_session_list::btn_goClick(TObject *Sender) {

    for (int i = 0; i < checkbox_list_main->Count; ++i) {
        const AnsiString str = checkbox_list_main->Items->Strings[i];

        /* note double backslash in C++ string literal */
        int ipos = str.Pos("{/MAR|_777_|IN\\}");
        const AnsiString full_url = str.SubString(1, ipos - 1); // extract clean URL

        /* Title (future filename on disk) */
        int len = str.Length();
        const AnsiString mht_file_name = str.SubString(ipos + 16, len - full_url.Length());

        /* Call the MHT saving function */
        save2mht(full_url, window_ami->edit_out_directory->Text + "\\" + mht_file_name + ".mht");
        // Alternative: save_to_mht(full_url, window_ami->EditResullFiles->Text + "\\" + mht_file_name + ".mht");

        txt_result->Caption = "Сохранено " + IntToStr(i + 1);
        /* force immediate update */
        txt_result->Update();
    }
}

void __fastcall Twindow_session_list::FormKeyPress(TObject *Sender, char &Key) {
    if (Key == VK_ESCAPE)
        Close();
}

AnsiString __fastcall Twindow_session_list::get_title_tag(const AnsiString& file_name) {
    const TStringList *html = new TStringList();
    html->LoadFromFile(file_name);
    AnsiString result = "No title found";
    for (int i = 0; i < html->Count; ++i) {
        int p = html->Strings[i].Pos("<title>");
        if (p > 0) {
            int p2 = html->Strings[i].Pos("</title>");
            if (p2 > p)
                result = html->Strings[i].SubString(p + 7, p2 - p - 7);
            break;
        }
    }
    delete html;
    return result;
}

/*
    TRegExpr *re = new TRegExpr();
    AnsiString result;

    try
    {
        re->Expression = "^(?im).*?<title>(.*?)</title>.*$";
        if (re->Exec(html->Text))
            result = re->Match[1];
        else
            result = "No title found";
    }
    __finally
    {
        delete re;
        delete html;
    }
*/

void __fastcall Twindow_session_list::btn_sel_allClick(TObject *Sender) {
    for (int i = 0; i < checkbox_list_main->Count; ++i)
        checkbox_list_main->Checked[i] = !checkbox_list_main->Checked[i];
}