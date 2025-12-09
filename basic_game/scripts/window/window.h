#include <windows.h>

class Window
{
public:
    Window();
    virtual ~Window();

    bool Initialize(HINSTANCE hInst);

    bool Terminate();

    bool MessageLoop();

    static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void OnResize(UINT width, UINT height);

    HWND    GetWindowHandle() { return m_hWindow; }

private:
    WNDCLASS    m_wc;
    HWND        m_hWindow = nullptr;
    SIZE        m_sizeWindow;
    MSG         m_msg = {};
};