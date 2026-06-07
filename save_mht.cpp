#include <vcl.h>
#pragma hdrstop

#include "save_mht.h"


#pragma package(smart_init)

/*
 * ----------------------------------------------------------------------------
 * save2mht - Simplified version using OleVariant
 * ----------------------------------------------------------------------------
 */
bool __fastcall save2mht(const AnsiString& url, const AnsiString& dest_file_name, bool show_error_message) {
    bool res = false;
    const AnsiString file_name = ChangeFileExt(dest_file_name, ".mht");
    DeleteFile(file_name.c_str());

    try {
        Variant vconfig = CreateOleObject("CDO.Configuration");
        Variant vmsg = CreateOleObject("CDO.Message");

        vmsg.OlePropertySet("Configuration", vconfig);
        vmsg.OleProcedure("CreateMHTMLBody", url);
        vmsg.OlePropertyGet("GetStream").OleProcedure("SaveToFile", file_name);

        res = true;
    }
    catch (const Exception &e) {
        if (show_error_message)
            MessageDlg(e.Message, mtError, TMsgDlgButtons() << mbOK, 0);
        res = false;
    }

    return res;
}

/*
 * ----------------------------------------------------------------------------
 * save_to_mht - Using typed interfaces (IMessage, IConfiguration, _Stream)
 * ----------------------------------------------------------------------------
 */
void __fastcall save_to_mht(const AnsiString& url, const AnsiString& dest_file_name) {
    IMessage* msg = CoMessage::Create();
    IConfiguration* conf = CoConfiguration::Create();
    msg->Configuration = conf;

    const WideString w_url = url;
    msg->CreateMHTMLBody(w_url, cdoSuppressNone, L"", L"");
    _Stream* ss = msg->GetStream();

    const WideString wdest_file_name = dest_file_name;
    ss->SaveToFile(wdest_file_name, adSaveCreateOverWrite);
}

/*
 * ----------------------------------------------------------------------------
 * Thread class for multithreaded COM initialization
 * ----------------------------------------------------------------------------
 */
class TCOMInitNullThread : public TThread {
    AnsiString url_;
    AnsiString fname;
    Exception* ex;
protected:
    virtual void __fastcall Execute() {
        CoInitialize(NULL);
        try {
            save_to_mht(url_, fname);
        }
        catch (const Exception &e) {
                ShowMessage("Error:\n" + e.Message);
        }
        CoUninitialize();
    }

public:
    __fastcall TCOMInitNullThread(const AnsiString url, const AnsiString file_name)
        : TThread(true), url_(url), fname(file_name), ex(NULL) {
    }

    __fastcall ~TCOMInitNullThread() {
        delete ex;
    }

    void __fastcall CheckException() {
        if (ex)
            throw ex;
    }
};

/*
 * ----------------------------------------------------------------------------
 * save_to_mht_in_cothread - Runs save_to_mht in a thread with COM initialized
 * ----------------------------------------------------------------------------
 */
void __fastcall save_to_mht_in_cothread(const AnsiString& url, const AnsiString& dest_file_name) {
    TCOMInitNullThread *th = new TCOMInitNullThread(url, dest_file_name);
    try {
        th->Resume();
        th->WaitFor();
        th->CheckException();
    }
    __finally {
        delete th;
    }
}