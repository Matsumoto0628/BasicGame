#include "input_manager.h"

InputManager& InputManager::Instance()
{
	static InputManager instance;
	return instance;
}

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

bool InputManager::GetKeyDown(int key)
{
	return !m_prevKeys[key] && m_currentKeys[key];
}

bool InputManager::GetKey(int key)
{
	return m_currentKeys[key];
}

bool InputManager::GetKeyUp(int key)
{
	return m_prevKeys[key] && !m_currentKeys[key];
}

void InputManager::Update()
{
	for (int i = 0; i < KEY_MAX; i++) {
		m_prevKeys[i] = m_currentKeys[i];
		m_currentKeys[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
	}

	if (GetKeyDown(VK_ESCAPE))
	{
		SetCursorLock(!m_mouseLoocked);
	}

    if (m_mouseLoocked) 
    {
        // ウィンドウ中央
        RECT rect;
        GetClientRect(m_hwnd, &rect);

        POINT center{
            (rect.right - rect.left) / 2,
            (rect.bottom - rect.top) / 2
        };
        ClientToScreen(m_hwnd, &center);

        // 現在位置
        POINT mouse;
        GetCursorPos(&mouse);

        // 中央との差分だけを見る
        m_mouseDelta.x = mouse.x - center.x;
        m_mouseDelta.y = mouse.y - center.y;

        // 必ず最後に中央へ戻す
        SetCursorPos(center.x, center.y);
    }
}

void InputManager::SetCursorLock(bool lock)
{
    m_mouseLoocked = lock;
    if (m_mouseLoocked)
    {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        ClientToScreen(m_hwnd, (POINT*)&rect.left);
        ClientToScreen(m_hwnd, (POINT*)&rect.right);

        ClipCursor(&rect);
        while (ShowCursor(FALSE) >= 0);
    }
    else
    {
        ClipCursor(nullptr);
        while (ShowCursor(TRUE) < 0);
    }
}