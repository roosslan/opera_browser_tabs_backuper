#include <vcl.h>
#pragma hdrstop

#include <memory>
#include <string.h>
#include "session_parser.h"

#pragma package(smart_init)

namespace {

const AnsiString history_url_suffix = "history url";
const AnsiString history_title_suffix = "history title";

// Максимальная длина имени файла (без расширения)
const int max_file_name_length = 100;

// Добавляет страницу в список, если адрес ведёт на http/https
void add_page(std::vector<page_entry>& pages, const AnsiString& url, const AnsiString& title) {
    if (url.SubString(1, 4).LowerCase() != "http")
        return;
    page_entry page;
    page.url = url;
    page.title = title;
    pages.push_back(page);
}

// Проверяет, совпадает ли имя с зарезервированным именем устройства Windows
bool is_reserved_name(const AnsiString& name) {
    static const char* reserved[] = {
        "CON", "PRN", "AUX", "NUL",
        "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
        "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
    };
    AnsiString base = name.UpperCase();
    const int dot = base.Pos(".");
    if (dot > 0)
        base = base.SubString(1, dot - 1);
    base = base.TrimRight();
    for (unsigned i = 0; i < sizeof(reserved) / sizeof(reserved[0]); ++i)
        if (base == reserved[i])
            return true;
    return false;
}

} // namespace

void read_opera_session(const AnsiString& file_name, bool all_history, std::vector<page_entry>& pages) {
    // TMemIniFile читает файл целиком, поэтому, в отличие от TIniFile,
    // не обрезает значения длиннее 2047 символов
    std::auto_ptr<TMemIniFile> ini(new TMemIniFile(file_name));
    std::auto_ptr<TStringList> sections(new TStringList());
    ini->ReadSections(sections.get());

    for (int s = 0; s < sections->Count; ++s) {
        // История вкладки хранится в секции "<N>history url", где N — номер окна.
        // Номера окон идут не обязательно подряд, поэтому перебираются все секции файла
        const AnsiString sect = sections->Strings[s];
        const int suffix_pos = sect.Length() - history_url_suffix.Length() + 1;
        if (suffix_pos < 2 ||
            sect.SubString(suffix_pos, history_url_suffix.Length()).LowerCase() != history_url_suffix)
            continue;
        const AnsiString window = sect.SubString(1, suffix_pos - 1);
        if (StrToIntDef(window, -1) < 0)
            continue;

        const int count = ini->ReadInteger(sect, "Count", 0);
        if (count <= 0)
            continue;

        int first = 0;
        int last = count - 1;
        if (!all_history) {
            // "current history" в секции окна — номер текущей страницы вкладки, считая с 1.
            // Если ключа нет или значение вне диапазона, берётся последняя страница
            int current = ini->ReadInteger(window, "current history", count);
            if (current < 1 || current > count)
                current = count;
            first = last = current - 1;
        }

        const AnsiString title_sect = window + history_title_suffix;
        for (int i = first; i <= last; ++i) {
            const AnsiString key = IntToStr(i);
            add_page(pages, ini->ReadString(sect, key, ""),
                     Utf8ToAnsi(ini->ReadString(title_sect, key, "")));
        }
    }
}

AnsiString make_file_name(const page_entry& page) {
    AnsiString source = page.title.Trim();
    if (source.IsEmpty()) {
        // Заголовка нет — имя строится из адреса без схемы
        source = page.url;
        const int scheme_end = source.Pos("://");
        if (scheme_end > 0)
            source = source.SubString(scheme_end + 3, source.Length());
    }

    // Символы, недопустимые в именах файлов Windows, заменяются на "_".
    // Кириллица и прочие символы сохраняются
    AnsiString res;
    for (int i = 1; i <= source.Length(); ++i) {
        const char ch = source[i];
        if ((unsigned char)ch < 32 || strchr("\\/:*?\"<>|", ch) != NULL)
            res += '_';
        else
            res += ch;
    }

    if (res.Length() > max_file_name_length)
        res = res.SubString(1, max_file_name_length);
    res = res.Trim();
    // Windows не допускает точки и пробелы в конце имени
    while (res.Length() > 0 && (res[res.Length()] == '.' || res[res.Length()] == ' '))
        res.SetLength(res.Length() - 1);

    if (res.IsEmpty())
        res = "page";
    if (is_reserved_name(res))
        res = "_" + res;
    return res;
}

AnsiString make_unique_path(const AnsiString& dir, const AnsiString& name, TStrings* used) {
    const AnsiString base = IncludeTrailingBackslash(dir) + name;
    AnsiString path = base + ".mht";
    for (int n = 2; FileExists(path) || used->IndexOf(path) >= 0; ++n)
        path = base + " (" + IntToStr(n) + ").mht";
    used->Add(path);
    return path;
}
