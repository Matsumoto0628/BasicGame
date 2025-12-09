#pragma once
#include "window.h"

class Application
{
public:
    void Initialize(HINSTANCE hInst);
    void Loop();
    void Terminate();

private:
    // アプリのメインループ
    bool gameLoop();

private:
    Window  m_window;
};