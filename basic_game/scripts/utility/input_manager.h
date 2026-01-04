#pragma once
#define KEY_MAX 256

class InputManager {
public:
	static InputManager& Instance();
	InputManager();
	~InputManager();
	bool GetKeyDown(int key);
	bool GetKeyUp(int key);
	bool GetKey(int key);
	void Update();
	POINT GetMouseDelta() const { return m_mouseDelta; }
	void SetHwnd(HWND hwnd) { m_hwnd = hwnd; }
	void SetCursorLock(bool lock);

private:
	bool m_prevKeys[KEY_MAX];
	bool m_currentKeys[KEY_MAX];
	POINT m_mouseDelta{};
	HWND m_hwnd;
	bool m_mouseLoocked = false;
};