﻿#include "platform/window.h"

#ifdef WIN32

#define WIN32_LEAN__AND_MEAN

#include "event.h"
#include "project.h"

#include <stdbool.h>
#include <stdint.h>
#include <Windows.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

struct platform_window_s {
    HINSTANCE instance;
    HWND window;
};

typedef HRESULT (*DwmSetWindowAttribute_fn)(HWND, DWORD, LPCVOID, DWORD);

LRESULT CALLBACK window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

static event_t latest_event;

bool window_create(const char* title, uint32_t width, uint32_t height, window_t* window) {
    HINSTANCE instance;
    if (!GetModuleHandleEx(0, NULL, &instance)) {
        return false;
    }

    HICON icon = LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    if (icon == NULL) {
        return false;
    }

    HCURSOR cursor = LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    if (cursor == NULL) {
        return false;
    }

    WNDCLASSEX window_class = {
      .cbSize = sizeof(WNDCLASSEX),
      .style = 0,
      .lpfnWndProc = window_callback,
      .cbClsExtra = 0,
      .cbWndExtra = 0,
      .hInstance = instance,
      .hIcon = icon,
      .hCursor = cursor,
      .hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH),
      .lpszMenuName = NULL,
      .lpszClassName = PROJECT_NAME,
      .hIconSm = icon,
    };
    if (RegisterClassEx(&window_class) == 0) {
        return false;
    }

    HWND window_handle = CreateWindow(
      PROJECT_NAME,
      title,
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT,
      SW_SHOWNORMAL,
      (int)width,
      (int)height,
      NULL,
      NULL,
      instance,
      NULL
    );
    if (window_handle == NULL) {
        return false;
    }

    HINSTANCE dwmapi_lib = LoadLibrary("dwmapi.dll");
    if (dwmapi_lib != NULL) {
        DwmSetWindowAttribute_fn DwmSetWindowAttribute_function = (DwmSetWindowAttribute_fn
        )GetProcAddress(dwmapi_lib, "DwmSetWindowAttribute");
        if (DwmSetWindowAttribute_function != NULL) {
            BOOL allow_immersive_dark_mode = TRUE;
            DwmSetWindowAttribute_function(
              window_handle, DWMWA_USE_IMMERSIVE_DARK_MODE, &allow_immersive_dark_mode, sizeof(allow_immersive_dark_mode)
            );
        }

        FreeLibrary(dwmapi_lib);
    }

    platform_window_t* platform_window = malloc(sizeof(platform_window_t));
    platform_window->instance = instance;
    platform_window->window = window_handle;
    window->platform_window = platform_window;

    return true;
}

void window_destroy(window_t* self) {
    DestroyWindow(self->platform_window->window);
    FreeLibrary(self->platform_window->instance);
    free(self->platform_window);
}

bool window_poll_event(const window_t* self, event_t* event) {
    MSG message;
    BOOL message_received = PeekMessage(&message, self->platform_window->window, 0, 0, PM_REMOVE);

    TranslateMessage(&message);
    DispatchMessage(&message);

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
