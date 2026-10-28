#ifndef save_mht
#define save_mht

#include "pch.h"

// Загружает страницу url и сохраняет её в MHT-файл dest_file_name через CDO.Message.
// COM в вызывающем потоке должен быть инициализирован (CoInitialize).
// При ошибке возвращает false и помещает текст ошибки в error_message.
bool __fastcall save2mht(const AnsiString& url, const AnsiString& dest_file_name, AnsiString& error_message);

#endif
