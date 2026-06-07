#ifndef test_mht
#define test_mht

#include "pch.h"

class Twindow_ami : public TForm
{
__published:
        TButton *btn_sel_directory;
        TLabel *txt_top_info;
        TLabel *txt_pos0;
        TLabel *txt_info0;
        TLabel *txt_footer_info;
        TLabel *txt_info1;
        TLabel *txt_pos1;
        TLabel *txt_pos2;
        TLabel *txt_info3;
        TButton *btn_analyse;
        TComboBox *combobox_opera_directory;
        TLabel *txt_about;
        TCheckBox *checkbox_get_all_history;
        TLabel *txt_pos3;
        TLabel *txt_info2;
        TEdit *edit_out_directory;
        TButton *btn_sel_out_directory;
        TCheckBox *checkbox_use_wand;
        TPopupMenu *context_menu;
        TMenuItem *menu_greetings;
    
    void __fastcall menu_greetingsClick(TObject *Sender);
    void __fastcall btn_sel_out_directoryClick(TObject *Sender);
    void __fastcall btn_analyseClick(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall btn_sel_directoryClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btn_call_using_threadClick(TObject *Sender);
    
private:
    // Private declarations
public:
    // Public declarations
    __fastcall Twindow_ami(TComponent* Owner);
};

extern PACKAGE Twindow_ami *window_ami;

#endif