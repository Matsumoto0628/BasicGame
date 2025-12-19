#include "application.h"

void Application::Initialize(HINSTANCE hInst)
{
    m_window.Initialize(hInst);
    m_renderer.Initialize(m_window.GetWindowHandle());
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
    m_renderer.Terminate();
    m_window.Terminate();
}

bool Application::gameLoop()
{
    m_renderer.Draw();
    m_renderer.Swap();
    return true;
}