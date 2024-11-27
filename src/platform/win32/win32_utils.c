#include "platform/win32/win32_utils.h"

#ifdef WIN32

#include <stdlib.h>

wchar_t* win32_create_wide_string_from_uti8_string(const char* utf8_string) {
    int wide_string_character_count = MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, NULL, 0);
    wchar_t* wide_string = malloc(wide_string_character_count * sizeof(wchar_t));
    if (wide_string == NULL) {
        abort();
    }

    MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, wide_string, wide_string_character_count);

    return wide_string;
}
#endif
