#include "application.h"
#include "input_manager.h"
#include "scene_manager.h"

void Application::Initialize(HINSTANCE hInst)
{
    m_window.Initialize(hInst);
    m_renderer.Initialize(m_window.GetWindowHandle());
    m_renderer2d.Initialize(m_renderer.GetDevice(), m_renderer.GetSwapChain());
    SceneManager::Instance().Initialize(m_renderer, m_renderer2d);
}

void Application::Setup()
{
    InputManager::Instance().SetHwnd(m_window.GetWindowHandle());
    InputManager::Instance().SetCursorLock(true);
    SceneManager::Instance().Transit(SceneManager::SceneType::Title);
}

void Application::Loop()
{
    while (true) {
        bool result = m_window.MessageLoop();
        if (result == false) break;

        result = gameLoop();
        if (result == false) break;
    }
}

void Application::Terminate()
{
    SceneManager::Instance().Terminate();
    m_renderer.Terminate();
    m_window.Terminate();
}

bool Application::gameLoop()
{
    SceneManager::Instance().Update();
    m_renderer.Draw();
    SceneManager::Instance().Draw();
    m_renderer2d.Draw();
    m_renderer.Swap();
	InputManager::Instance().Update();

    Sleep(10);
    return true;
}