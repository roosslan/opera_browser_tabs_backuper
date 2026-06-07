#ifndef unit_list
#define unit_list

#include "pch.h"

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

	void __fastcall btn_sel_allClick(TObject *Sender);
    void __fastcall btn_goClick(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);

private:
    // Private declarations
public:
    // Public declarations
    __fastcall Twindow_session_list(TComponent* Owner);
    AnsiString __fastcall get_title_tag(const AnsiString& file_name);
};

extern PACKAGE Twindow_session_list *window_session_list;

#endif