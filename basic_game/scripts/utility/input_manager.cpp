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
}