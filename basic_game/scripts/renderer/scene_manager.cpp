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

	m_player.Initialize(renderer, &m_camera, &m_weapon);
	m_weapon.Initialize(renderer);
	for (int i = 0; i < ENEMY_MAX; ++i) 
	{
		m_enemies[i].Initialize(renderer, &m_player);
	}

	const char* STAGE_PATH = "models/field/field.obj";
	m_stage.Setup(renderer, STAGE_PATH);
}

void SceneManager::Setup()
{
	m_player.Setup();
	m_weapon.Setup();

	float minY = 0.3f;
	float maxY = 2.0f;
	const float radius = 5.0f;     // 円の半径
	const DirectX::XMFLOAT3 center = { 0.f, 0.5f, 0.f };  // 円の中心
	for (int i = 0; i < ENEMY_MAX; ++i)
	{
		m_enemies[i].Setup();
		if (i % 2 == 0)
			m_enemies[i].SetMoveType(MoveType::Basic);
		else
			m_enemies[i].SetMoveType(MoveType::Curve);

		float angle = DirectX::XM_2PI * i / ENEMY_MAX;  // ラジアン

		float x = center.x + cosf(angle) * radius;
		float z = center.z + sinf(angle) * radius;
		float r = (float)rand() / RAND_MAX;        // 0〜1
		float y = minY + r * (maxY - minY);       // 指定範囲へ変換

		m_enemies[i].SetPosition({ x, y, z });
	}

	m_stage.SetPosition({ 0.f, 0.f, 0.f });
	m_stage.SetScale({ 0.05f, 0.05f, 0.05f });
}

void SceneManager::Terminate()
{
	m_stage.Terminate();
	m_weapon.Terminate();
	m_player.Terminate();
	for (int i = 0; i < ENEMY_MAX; ++i)
	{
		m_enemies[i].Terminate();
	}
}

void SceneManager::Update()
{
	m_pRenderer->SetEyePosLight(m_camera.GetEyePos());
	m_player.Update();
    m_camera.Update();
	m_weapon.Update();

	for (int i = 0; i < ENEMY_MAX; ++i)
	{
		m_enemies[i].Update();

		if (m_weapon.GetCollider().Intersects(m_enemies[i].GetCollider()))
		{
			m_enemies[i].TakeDamage(10);
			m_enemies[i].CancelAttack();
		}

		if (m_player.GetCollider().Intersects(m_enemies[i].GetCollider()))
		{
			m_player.TakeDamage(10);
			m_enemies[i].CancelAttack();
		}
	}

	static float timer = 0.f;
	timer += 0.017f;
	static int attackNum = 0;
	for (int i = 0; i < ENEMY_MAX; ++i)
	{
		if (attackNum >= ENEMY_MAX)
		{
			attackNum = 0;
		}
		if (m_enemies[attackNum].IsDead())
		{
			attackNum++;
			continue;
		}
		if (timer >= 3.f)
		{
			timer = 0.f;
			m_enemies[attackNum].Attack();
			attackNum++;
		}
	}
}

void SceneManager::Draw()
{
    auto viewMatrix = m_camera.GetViewMatrix();
    m_pRenderer->SetupViewTransform(viewMatrix);
	m_player.Draw();
	m_stage.Draw();
	m_weapon.Draw();
	for (int i = 0; i < ENEMY_MAX; ++i)
	{
		m_enemies[i].Draw();
	}
}