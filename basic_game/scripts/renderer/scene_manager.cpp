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

    const char* STAGE_PATH = "models/field/field.obj";
    m_stage.Setup(renderer, STAGE_PATH);
    m_stage.SetPosition({0.f, 0.f, 0.f });
    m_stage.SetScale({ 0.05f, 0.05f, 0.05f });
}

void SceneManager::Terminate()
{
	m_stage.Terminate();
}

void SceneManager::Update()
{
    m_camera.Update();
	m_stage.Update();
}

void SceneManager::Draw()
{
    auto viewMatrix = m_camera.GetViewMatrix();
    m_pRenderer->SetupViewTransform(viewMatrix);
	m_stage.Draw();
}