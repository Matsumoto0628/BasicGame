#include "scene_manager.h"
#include "renderer.h"
#include "input_manager.h"

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
    const char* MODEL_PATH = "models/box.fbx";
    m_sampleModel.Setup(renderer, MODEL_PATH);
}

void SceneManager::Terminate()
{
    m_sampleTriangle.DestroyVertexBuffer();
    m_sampleModel.Terminate();
}

void SceneManager::Update()
{
    m_camera.Update();
    InputManager::Instance().Update();
}

void SceneManager::Draw()
{
    auto viewMatrix = m_camera.GetViewMatrix();
    m_pRenderer->SetupViewTransform(viewMatrix);

    m_sampleTriangle.Draw(*m_pRenderer);
    m_sampleModel.Draw(*m_pRenderer);
}