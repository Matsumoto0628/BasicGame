#include "application.h"

void Application::Initialize(HINSTANCE hInst)
{
    m_window.Initialize(hInst);
    m_renderer.Initialize(m_window.GetWindowHandle());
    m_sceneManager.Initialize(m_renderer);
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
    m_sceneManager.Terminate();
    m_renderer.Terminate();
    m_window.Terminate();
}

bool Application::gameLoop()
{
    m_sceneManager.Update();
    m_renderer.Draw();
    m_sceneManager.Draw();
    m_renderer.Swap();

    Sleep(10);
    return true;
}