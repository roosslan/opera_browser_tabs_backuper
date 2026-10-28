#include <vcl.h>
#pragma hdrstop

#include "save_mht.h"

#pragma package(smart_init)

namespace {
// ADODB.SaveOptionsEnum: перезаписать существующий файл
const int ad_save_create_over_write = 2;
}

bool __fastcall save2mht(const AnsiString& url, const AnsiString& dest_file_name, AnsiString& error_message) {
    error_message = "";
    try {
        Variant vconfig = CreateOleObject("CDO.Configuration");
        Variant vmsg = CreateOleObject("CDO.Message");

        // Строки передаются как WideString (BSTR): AnsiString попадает в Variant
        // как varString, и CDO отвечает ошибкой "Неверный тип переменной"
        vmsg.OlePropertySet("Configuration", vconfig);
        vmsg.OleProcedure("CreateMHTMLBody", WideString(url));
        vmsg.OleFunction("GetStream").OleProcedure("SaveToFile", WideString(dest_file_name), ad_save_create_over_write);
        return true;
    }
    catch (const Exception &e) {
        error_message = e.Message;
    }
    return false;
}
