#include "platform/window.h"

#ifdef WIN32

#include "event.h"
#include "platform/win32/win32_types.h"
#include "platform/win32/win32_utils.h"
#include "project.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

struct platform_window_s {
    HINSTANCE instance;
    HWND window;
    wchar_t* window_class_name_w;
    wchar_t* window_title_w;
};

typedef HRESULT (*DwmSetWindowAttribute_fn)(HWND, DWORD, LPCVOID, DWORD);

LRESULT CALLBACK window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

static event_t latest_event;

bool window_create(const char* title, int width, int height, window_t* window) {
    wchar_t* window_title_w = win32_create_wide_string_from_uti8_string(title);
    wchar_t* window_class_name_w = win32_create_wide_string_from_uti8_string(PROJECT_NAME);

    HINSTANCE instance;
    if (!GetModuleHandleExW(0, NULL, &instance)) {
        return false;
    }

    HICON icon = LoadImageW(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    if (icon == NULL) {
        return false;
    }

    HCURSOR cursor = LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    if (cursor == NULL) {
        return false;
    }

    WNDCLASSEXW window_class = {
      .cbSize = sizeof(WNDCLASSEXW),
      .style = 0,
      .lpfnWndProc = window_callback,
      .cbClsExtra = 0,
      .cbWndExtra = 0,
      .hInstance = instance,
      .hIcon = icon,
      .hCursor = cursor,
      .hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH),
      .lpszMenuName = NULL,
      .lpszClassName = window_class_name_w,
      .hIconSm = icon,
    };
    if (RegisterClassEx(&window_class) == 0) {
        return false;
    }

    int primary_screen_width = GetSystemMetrics(SM_CXSCREEN);
    int primary_screen_height = GetSystemMetrics(SM_CYSCREEN);
    int position_x = (primary_screen_width / 2) - (width / 2);
    int position_y = (primary_screen_height / 2) - (height / 2);

    HWND window_handle = CreateWindowExW(
      0,
      window_class_name_w,
      window_title_w,
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      position_x,
      position_y,
      width,
      height,
      NULL,
      NULL,
      instance,
      NULL
    );
    if (window_handle == NULL) {
        return false;
    }

    HINSTANCE dwmapi_lib = LoadLibraryW(L"dwmapi.dll");
    if (dwmapi_lib != NULL) {
        DwmSetWindowAttribute_fn DwmSetWindowAttribute_function = (DwmSetWindowAttribute_fn)(void (*)(void)
        )GetProcAddress(dwmapi_lib, "DwmSetWindowAttribute");
        if (DwmSetWindowAttribute_function != NULL) {
            BOOL allow_immersive_dark_mode = TRUE;
            DwmSetWindowAttribute_function(
              window_handle, DWMWA_USE_IMMERSIVE_DARK_MODE, &allow_immersive_dark_mode, sizeof(allow_immersive_dark_mode)
            );
        }

        FreeLibrary(dwmapi_lib);
    }

    ShowWindow(window_handle, SW_SHOWNORMAL);

    platform_window_t* platform_window = malloc(sizeof(platform_window_t));
    if (platform_window == NULL) {
        return false;
    }

    platform_window->instance = instance;
    platform_window->window = window_handle;
    platform_window->window_class_name_w = window_class_name_w;
    platform_window->window_title_w = window_title_w;
    window->platform_window = platform_window;

    return true;
}

void window_destroy(window_t* self) {
    DestroyWindow(self->platform_window->window);
    FreeLibrary(self->platform_window->instance);
    free(self->platform_window->window_title_w);
    free(self->platform_window->window_class_name_w);
    free(self->platform_window);
}

bool window_poll_event(const window_t* self, event_t* event) {
    MSG message;
    BOOL message_received = PeekMessageW(&message, self->platform_window->window, 0, 0, PM_REMOVE);

    TranslateMessage(&message);
    DispatchMessageW(&message);

    *event = latest_event;
    return message_received;
}

LRESULT CALLBACK window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
    latest_event.type = EVENT_TYPE_UNKNOWN;

    if (message == WM_CLOSE) {
        latest_event.type = EVENT_TYPE_QUIT;
        return 0;
    }

    return DefWindowProc(window, message, w_param, l_param);
}

#endif
