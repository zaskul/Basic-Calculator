#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include "Window.h"

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

// The above is needed in order to have UNICODE working

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

const int GRID_SIZE = 4;
const int BUTTON_SIZE = 120;
int result = 0;
int currentValue = 0;

DWORD button_style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
DWORD text_block_style = WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE;
HFONT CreateFontWithSize(int size)
{
    LOGFONT logFont = {};
    logFont.lfHeight = size;
    logFont.lfWeight = FW_NORMAL;
    logFont.lfCharSet = DEFAULT_CHARSET;
    logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logFont.lfQuality = DEFAULT_QUALITY;
    logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    wcscpy(logFont.lfFaceName, L"Arial");

    return CreateFontIndirect(&logFont);
}

// int to LPCWSTR conversion

// LPCWSTR intToLPCWSTR(int value)
// {
//     std::string strValue = std::to_string(value);
//     std::wstring wstrValue(strValue.begin(), strValue.end());
//     LPCWSTR lpcwstrValue = wstrValue.c_str();
//     std::cout << lpcwstrValue << std::endl;
//     return lpcwstrValue;
// }

// string to wstring conversion

std::wstring StringToLPCWSTR(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), &wstr[0], size_needed);
    return wstr;
}

// wstring to LPCWSTR conversion

LPCWSTR wstringToLPCWSTR(std::wstring value)
{
    return value.c_str();
}

// LPCWSTR to int conversion

int LPCWSTRToInt(wchar_t value)
{
     // Convert the wchar_t to a char
    char ch = static_cast<char>(value);

    // Convert the char to an integer
    int intValue = ch - '0';

    return intValue;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND g_buttons[GRID_SIZE][GRID_SIZE];
    static HWND wideButton;
    static HWND equationBox;

    LPCWSTR buttonLabels[GRID_SIZE][GRID_SIZE] = {
        {L"/", L"*", L"√", L"C"},
        {L"7", L"8", L"9", L"-"},
        {L"4", L"5", L"6", L"+"},
        {L"1", L"2", L"3", L"0"}
    };

    switch (uMsg)
    {
    case WM_CREATE:
    {

        // Generate font for the equation box
        HFONT hFont = CreateFontWithSize(100);

        equationBox = CreateWindow(
            L"STATIC",
            L"",
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

        int equationBoxID = GetDlgCtrlID(equationBox);

        SendMessage(equationBox, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Generate font for the grid cells
        hFont = CreateFontWithSize(25);

        for (int x = 0; x < GRID_SIZE; ++x)
        {
            for (int y = 0; y < GRID_SIZE; ++y)
            {
                int xPos = x * BUTTON_SIZE;
                int yPos = y * BUTTON_SIZE + BUTTON_SIZE;
                g_buttons[x][y] = CreateWindow(
                    L"BUTTON",
                    buttonLabels[y][x],
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
                SendMessage(g_buttons[x][y], WM_SETFONT, (WPARAM)hFont, TRUE);
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
        SendMessage(wideButton, WM_SETFONT, (WPARAM)hFont, TRUE);
    }
        break;
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == BN_CLICKED)
        {
            HWND buttonClicked = (HWND)lParam;
            int buttonID = GetDlgCtrlID(buttonClicked);

            // Buffer to store the new text
            const wchar_t* newText = nullptr;

            // Buffer to store the button label later
            wchar_t buttonLabel[16];

            for (int x = 0; x < GRID_SIZE; ++x)
            {
                for (int y = 0; y < GRID_SIZE; ++y)
                {
                    if (buttonClicked == g_buttons[x][y])
                    {
                        // Get the clicked button's label
                        GetWindowText(buttonClicked, buttonLabel, 16);
                        // std::wstring message = L"Button Clicked: " + std::wstring(buttonLabel);
                        // MessageBox(hWnd, message.c_str(), L"Button Clicked", MB_OK | MB_ICONINFORMATION);
                        newText = buttonLabel;
                        break;
                    }
                }
            }

            if (buttonClicked == wideButton)
            {
                newText = L"=";
            }

            if (newText)
            {
                if (*newText == 43)
                {
                    std::cout << "Action:" << "addition" << std::endl;
                    result += currentValue;
                }
                else if (*newText == 45)
                {
                    std::cout << "Action:" << "subtraction" << std::endl;
                    result -= currentValue;
                }
                else if (*newText == 42)
                {
                    std::cout << "Action:" << "multiplication" << std::endl;
                    result *= currentValue;
                }
                else if (*newText == 47)
                {
                    std::cout << "Action:" << "division" << std::endl;
                    result /= currentValue;
                }
                else if (*newText == 8730)
                {
                    std::cout << "Action:" << "square root" << std::endl;
                    result = sqrt(result);
                }
                else if (*newText == 67)
                {
                    std::cout << "Action:" << "CLEAR" << std::endl;
                    result = 0;
                }
                else
                {
                    currentValue = LPCWSTRToInt(*newText);
                    std::cout << "Number: " << currentValue << std::endl;
                }
                
                SetWindowText(equationBox, wstringToLPCWSTR(StringToLPCWSTR(std::to_string(result))));
            }
        }
    }
        break;
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


