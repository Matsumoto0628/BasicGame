#include "application.h"

void Application::Initialize(HINSTANCE hInst)
{
    m_window.Initialize(hInst);
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
    m_window.Terminate();
}

bool Application::gameLoop()
{
    return true;
}