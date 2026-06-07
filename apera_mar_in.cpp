//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("test_mht.cpp", window_ami);
USEFORM("unit_list.cpp", window_session_list);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
        try  {
                 Application->Initialize();
                 Application->CreateForm(__classid(Twindow_ami), &window_ami);
                 Application->CreateForm(__classid(Twindow_session_list), &window_session_list);
                 Application->Run();
        }
        catch (Exception &exception) {
                 Application->ShowException(&exception);
        }
        catch (...) {
                 try {
                         throw Exception("");
                 }
                 catch (Exception &exception) {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
