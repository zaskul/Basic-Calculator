#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include "Window.h"

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif
// The above is needed in order to have UNICODE working

const int GRID_SIZE = 4;
const int BUTTON_SIZE = 120;


DWORD button_style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
DWORD text_block_style = WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND g_buttons[GRID_SIZE][GRID_SIZE];
    static HWND wideButton;
    static HWND textBlock;
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CREATE:
    {
        textBlock = CreateWindow(
            L"STATIC",
            L"EQUATION",
            text_block_style,
            0,
            0,
            GRID_SIZE * BUTTON_SIZE,
            BUTTON_SIZE,
            hWnd,
            NULL,
            NULL,
            NULL  
        );

        for (int x = 0; x < GRID_SIZE; ++x)
        {
            for (int y = 0; y < GRID_SIZE; ++y)
            {
                int xPos = x * BUTTON_SIZE;
                int yPos = y * BUTTON_SIZE + BUTTON_SIZE;
                g_buttons[x][y] = CreateWindow(
                    L"BUTTON",
                    L"",
                    button_style,
                    xPos,
                    yPos,
                    BUTTON_SIZE,
                    BUTTON_SIZE,
                    hWnd,
                    NULL,
                    NULL,
                    NULL
                );
            }
        }
        wideButton = CreateWindow(
            L"BUTTON",
            L"=",
            button_style,
            0,
            GRID_SIZE * BUTTON_SIZE + BUTTON_SIZE,
            GRID_SIZE * BUTTON_SIZE,
            BUTTON_SIZE,
            hWnd,
            NULL,
            NULL,
            NULL
            
        );
    }
    // Paint a line grid
    // case WM_PAINT:
    //     {
    //     PAINTSTRUCT paintStruct;
    //     HDC hdc = BeginPaint(hWnd, &paintStruct);

    //     // Draw vertical lines
    //     for (int x = 0; x < GRID_SIZE; ++x)
    //     {
    //         int xPos = x * (paintStruct.rcPaint.right - paintStruct.rcPaint.left) / GRID_SIZE;
    //         MoveToEx(hdc, xPos, 0, NULL);
    //         LineTo(hdc, xPos, paintStruct.rcPaint.bottom);
    //     }

    //     // Draw horizontal lines
    //     for (int y = 0; y < GRID_SIZE; ++y)
    //     {
    //         int yPos = y * (paintStruct.rcPaint.bottom - paintStruct.rcPaint.top) / GRID_SIZE;
    //         MoveToEx(hdc, 0, yPos, NULL);
    //         LineTo(hdc, paintStruct.rcPaint.right, yPos);
    //     }

    //     EndPaint(hWnd, &paintStruct);
    //     }
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

    int width = 480;
    int height = 720;

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


