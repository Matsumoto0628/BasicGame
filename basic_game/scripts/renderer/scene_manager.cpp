#include "scene_manager.h"
#include "renderer.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize(Renderer& renderer)
{
    m_pRenderer = &renderer;

    m_sampleTriangle.CreateVertexBuffer(renderer);
}

void SceneManager::Terminate()
{
    m_sampleTriangle.DestroyVertexBuffer();
}

void SceneManager::Update()
{
    m_camera.Update();
}

void SceneManager::Draw()
{
    auto viewMatrix = m_camera.GetViewMatrix();
    m_pRenderer->SetupViewTransform(viewMatrix);

    m_sampleTriangle.Draw(*m_pRenderer);
}