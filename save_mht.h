#ifndef save_mht
#define save_mht

#include "pch.h"
#include "CDO_TLB.h"
#include "ADODB_TLB.h"

// This procedure can be used as long as the threading model has not
// been set to Multithreaded. If you try, you will get an
// "Interface not supported" error.
//
// But if you have already set COM to multithreaded through the
// CoInitializeEx function then use the other function (SaveToMHT_InCOThread)
void __fastcall save_to_mht(const AnsiString& url, const AnsiString& dest_file_name);

// Main function to save URL to MHT file
// Returns true on success, false on failure
// AShowErrorMessage controls whether to show error dialog
bool __fastcall save2mht(const AnsiString& url, const AnsiString& dest_file_name, bool show_error_message = false);

// This should be used when you have already set the threading model to multithreaded
// The difference in this function is that it runs the 1st function in
// a separate thread with CoInitialize(nil) called. This allows you to still
// call the SaveToMHT when you have previously set COM to multithreaded.
// (Its still blocking, so the function will only return when it is finished)
//
// Beware, it is possible to get a "security" error when downloading from a
// secure https website. The only workaround I am aware of is to remove the
// "s" and just use http.
void __fastcall save_to_mht_in_cothread(const AnsiString& url, const AnsiString& dest_file_name);

#endif