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

private:
	bool m_prevKeys[KEY_MAX];
	bool m_currentKeys[KEY_MAX];
};