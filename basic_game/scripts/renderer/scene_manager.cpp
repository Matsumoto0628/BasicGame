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
	m_weapon.Initialize(renderer);
	m_enemy.Initialize(renderer, &m_player);

	const char* STAGE_PATH = "models/field/field.obj";
	m_stage.Setup(renderer, STAGE_PATH);
}

void SceneManager::Setup()
{
	m_player.Setup();
	m_weapon.Setup();
	m_enemy.Setup();

	m_stage.SetPosition({ 0.f, 0.f, 0.f });
	m_stage.SetScale({ 0.05f, 0.05f, 0.05f });
}

void SceneManager::Terminate()
{
	m_stage.Terminate();
	m_weapon.Terminate();
	m_player.Terminate();
	m_enemy.Terminate();
}

void SceneManager::Update()
{
	m_pRenderer->SetEyePosLight(m_camera.GetEyePos());
	m_player.Update();
    m_camera.Update();
	m_weapon.Update();
	m_enemy.Update();

	if (m_weapon.GetCollider().Intersects(m_enemy.GetCollider())) 
	{
		m_enemy.TakeDamage(10);
	}
}

void SceneManager::Draw()
{
    auto viewMatrix = m_camera.GetViewMatrix();
    m_pRenderer->SetupViewTransform(viewMatrix);
	m_player.Draw();
	m_stage.Draw();
	m_weapon.Draw();
	m_enemy.Draw();
}