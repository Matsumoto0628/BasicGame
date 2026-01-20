#include "scene_manager.h"
#include "renderer.h"
#include "euler_converter.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize(Renderer& renderer)
{
	m_pRenderer = &renderer;

	m_player.Initialize(&m_camera, &m_weapon);

	const char* STAGE_PATH = "models/field/field.obj";
	m_stage.Setup(renderer, STAGE_PATH);
	m_stage.SetPosition({ 0.f, 0.f, 0.f });
	m_stage.SetScale({ 0.05f, 0.05f, 0.05f });

	const char* WEAPON_PATH = "models/weapon/weapon.obj";
	m_weapon.Setup(renderer, WEAPON_PATH);
	m_weapon.SetPivot({ 0, -0.1f, 0 });
    m_weapon.SetScale({ 0.1f, 0.1f, 0.1f });
}

void SceneManager::Terminate()
{
	m_stage.Terminate();
	m_weapon.Terminate();
}

void SceneManager::Update()
{
	m_pRenderer->SetEyePosLight(m_camera.GetEyePos());
	m_player.Update();
    m_camera.Update();
	m_stage.Update();
	m_weapon.Update();
}

void SceneManager::Draw()
{
    auto viewMatrix = m_camera.GetViewMatrix();
    m_pRenderer->SetupViewTransform(viewMatrix);
	m_stage.Draw();
	m_weapon.Draw();
}