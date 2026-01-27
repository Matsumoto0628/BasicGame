#pragma once
#include "window.h"
#include "renderer.h"
#include "renderer_2d.h"

class Application
{
public:
    void Initialize(HINSTANCE hInst);
    void Setup();
    void Loop();
    void Terminate();

private:
    // アプリのメインループ
    bool gameLoop();

private:
    Window  m_window;
    Renderer m_renderer;
	Renderer2D m_renderer2d;
};