#include "window.h"
#include <tchar.h>

Window::Window()
    : m_sizeWindow()
{
    m_sizeWindow.cx = 960;
    m_sizeWindow.cy = 540;
}

Window::~Window()
{

}

bool Window::Initialize(HINSTANCE hInst)
{
    // ウインドウ クラスの登録
    m_wc.style = CS_HREDRAW | CS_VREDRAW;
    m_wc.lpfnWndProc = (WNDPROC)Window::MainWndProc;
    m_wc.cbClsExtra = 0;
    m_wc.cbWndExtra = 0;
    m_wc.hInstance = hInst;
    m_wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
    m_wc.hCursor = LoadCursor(hInst, IDC_ARROW);
    m_wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    m_wc.lpszMenuName = nullptr;
    m_wc.lpszClassName = _T("ShowWindow");
    if (!RegisterClass(&m_wc)) {
        return false;
    }

    // メイン ウインドウ作成
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.right = m_sizeWindow.cx;
    rect.bottom = m_sizeWindow.cy;
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWindow = CreateWindow(m_wc.lpszClassName, _T("ShowWindow"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr, hInst, nullptr);
    if (m_hWindow == nullptr) {
        return false;
    }

    // ウインドウ表示
    ShowWindow(m_hWindow, SW_SHOWNORMAL);
    UpdateWindow(m_hWindow);

    return true;

}

bool Window::Terminate()
{
    DestroyWindow(GetWindowHandle());

    // ウインドウ クラスの登録解除
    UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
    m_hWindow = nullptr;
    return true;
}

bool Window::MessageLoop()
{
    // Windowsメッセージループ処理
    if (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&m_msg); DispatchMessage(&m_msg);
    }
    // アプリケーションが終わるときにmessageがWM_QUITになる 
    if (m_msg.message == WM_QUIT) {
        return false;
    }
    return true;
}

/*static*/ LRESULT CALLBACK Window::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_DESTROY:
        // アプリ終了
        PostQuitMessage(0);
        return 0;
    }
    // デフォルト処理（必須）
    return DefWindowProc(hWnd, msg, wParam, lParam);
}