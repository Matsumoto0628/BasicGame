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

    const char* MODEL_PATH = "models/tea.glb";
    m_sampleModel.Setup(renderer, MODEL_PATH);
}

void SceneManager::Terminate()
{
    m_sampleModel.Terminate();
}

void SceneManager::Update()
{
    static float rot = 0.0f;
    rot += 0.1f;
    m_camera.Update();
	m_sampleModel.Rotate(*m_pRenderer, rot);
    InputManager::Instance().Update();
}

void SceneManager::Draw()
{
    auto viewMatrix = m_camera.GetViewMatrix();
    m_pRenderer->SetupViewTransform(viewMatrix);
    m_sampleModel.Draw(*m_pRenderer);
}