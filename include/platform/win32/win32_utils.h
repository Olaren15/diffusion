#ifndef DIFFUSION_PLATFORM_WIN32_UTILS_H
#define DIFFUSION_PLATFORM_WIN32_UTILS_H

#ifdef WIN32

#include "platform/win32/win32_types.h"

wchar_t* win32_create_wide_string_from_uti8_string(const char* utf8_string);

#endif
#endif // DIFFUSION_PLATFORM_WIN32_UTILS_H
