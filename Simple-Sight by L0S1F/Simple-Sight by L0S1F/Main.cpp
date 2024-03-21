#include <Windows.h>
#include <iostream>

bool crosshairVisible = true;
HWND hwnd;
HWND hwndText;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (crosshairVisible) {
            int centerX = 15;
            int centerY = 15;
            int radius = 5;
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
            SelectObject(hdc, hBrush);
            Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
            DeleteObject(hBrush);
        }
        EndPaint(hwnd, &ps);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT CALLBACK WindowTextProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        SetTextColor(hdc, RGB(0, 255, 0));
        SetBkMode(hdc, TRANSPARENT);
        HFONT hFont = CreateFont(16, 0, 5, 5, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SelectObject(hdc, hFont);
        DrawText(hdc, L"Приціл by L0S1F", -1, &rect, DT_RIGHT | DT_TOP); 
        DeleteObject(hFont);
        EndPaint(hwnd, &ps);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MessageBox(NULL, L"Зараз ви використовуєте безкоштовну версію прицілу від L0S1F.\nНатисніть ОК, щоб продовжити.", L"Приціл від L0S1F", MB_OK);
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"CrosshairWindowClass";
    RegisterClass(&wc);


    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    hwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
        wc.lpszClassName,
        NULL,
        WS_POPUP,
        (screenWidth - 28) / 2, (screenHeight - 28) / 2, 28, 28,
        NULL,
        NULL,
        hInstance,
        NULL);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    wc.lpfnWndProc = WindowTextProc;
    wc.lpszClassName = L"TextWindowClass";
    RegisterClass(&wc);

    hwndText = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
        wc.lpszClassName,
        NULL,
        WS_POPUP,
        screenWidth - 150, 5, 143, 30,
        NULL,
        NULL,
        hInstance,
        NULL);

    SetLayeredWindowAttributes(hwndText, RGB(0, 0, 0), 0, LWA_COLORKEY);

    ShowWindow(hwndText, SW_SHOW);
    UpdateWindow(hwndText);

    MSG msg;
    while (true) {
        if (GetAsyncKeyState(VK_INSERT) & 0x2D) { 
            crosshairVisible = !crosshairVisible; 
            if (crosshairVisible) {
                SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                ShowWindow(hwnd, SW_SHOW);
            }
            else {
                ShowWindow(hwnd, SW_HIDE);
            }
        }

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Sleep(10);
    }

    return 0;
}
