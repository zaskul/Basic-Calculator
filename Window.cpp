#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include "Window.h"

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window()
    : m_hInstance(GetModuleHandle(nullptr))
{
    const wchar_t* CLASS_NAME = L"Window Class";

    WNDCLASS wndCLass = {};
    wndCLass.lpszClassName = CLASS_NAME;
    wndCLass.hInstance = m_hInstance;
    wndCLass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wndCLass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndCLass.lpfnWndProc = WindowProc;

    RegisterClass(&wndCLass);

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    int width = 320;
    int height = 680;

    RECT rect;
    rect.left = 250;
    rect.top = 250;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    m_hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Title",
        style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );

    ShowWindow(m_hWnd, SW_SHOW);
}

Window::~Window()
{
    const wchar_t* CLASS_NAME = L"Window Class";
    
    UnregisterClass(CLASS_NAME, m_hInstance);
}

bool Window::ProcessMessages()
{
    MSG msg = {};

    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}


