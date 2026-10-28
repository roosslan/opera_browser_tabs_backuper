#ifndef session_parser
#define session_parser

#include "pch.h"
#include <vector>

// —траница, найденна€ в файле сессии Opera
struct page_entry {
    AnsiString url;
    AnsiString title;
};

// „итает файл сессии Opera (autosave.win) и добавл€ет в pages адреса вкладок.
// all_history = true Ч вс€ истори€ навигации каждой вкладки,
// иначе только текуща€ страница вкладки.
void read_opera_session(const AnsiString& file_name, bool all_history, std::vector<page_entry>& pages);

// —троит из заголовка страницы (или из адреса, если заголовка нет)
// допустимое им€ файла Windows без расширени€
AnsiString make_file_name(const page_entry& page);

// ¬озвращает путь dir\name.mht, не совпадающий ни с существующим файлом,
// ни с пут€ми из used. ѕолученный путь добавл€етс€ в used.
AnsiString make_unique_path(const AnsiString& dir, const AnsiString& name, TStrings* used);

#endif
