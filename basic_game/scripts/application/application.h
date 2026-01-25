#pragma once
#include "window.h"
#include "renderer.h"
#include "scene_manager.h"

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
    SceneManager m_sceneManager;
};