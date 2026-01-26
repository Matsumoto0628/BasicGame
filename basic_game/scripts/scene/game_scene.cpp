#include "game_scene.h"
#include "renderer.h"
#include "renderer_2d.h"
#include "euler_converter.h"
#include "game_text.h"
#include "game_image.h"

class ID2DI1Bitmap;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(Renderer& renderer, Renderer2D& renderer2d)
{
	m_pRenderer = &renderer;
	m_pRenderer2d = &renderer2d;

	m_player.Initialize(renderer, renderer2d, &m_camera, &m_weapon);
	m_weapon.Initialize(renderer);
	for (int i = 0; i < ENEMY_MAX; ++i)
	{
		m_enemies[i].Initialize(renderer, &m_player);
	}

	const char* STAGE_PATH = "models/field/field.obj";
	m_stage.Setup(renderer, STAGE_PATH);
}

void GameScene::Setup()
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

	m_pRenderer2d->AddText(new GameText(L"Escでカーソル表示、ゲームに戻る", 10.f, 10.f, DirectX::XMFLOAT4(1.f, 1.f, 0.f, 1.f)));
	m_pRenderer2d->AddText(new GameText(L"左クリックで攻撃", 10.f, 30.f, DirectX::XMFLOAT4(1.f, 1.f, 0.f, 1.f)));
	m_pRenderer2d->AddText(new GameText(L"WASDで移動", 10.f, 50.f, DirectX::XMFLOAT4(1.f, 1.f, 0.f, 1.f)));
	m_pRenderer2d->AddImage(new GameImage(m_pRenderer2d->LoadBitmapFromFile(L"images/red.png"), 0.f, 0.f, 960.f, 540.0f));
	ID2D1Bitmap* heart = m_pRenderer2d->LoadBitmapFromFile(L"images/heart.png");
	float scale = 50.f;
	float ofstX = 20.f;
	float ofstY = 475.f;
	for (int i = 0; i < 5; ++i)
	{
		m_pRenderer2d->AddImage(new GameImage(heart, ofstX + i * scale, ofstY, scale, scale));
	}
	m_pRenderer2d->SwitchImage(0, false);
}

void GameScene::Terminate()
{
	m_stage.Terminate();
	m_weapon.Terminate();
	m_player.Terminate();
	for (int i = 0; i < ENEMY_MAX; ++i)
	{
		m_enemies[i].Terminate();
	}
}

void GameScene::Update()
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

void GameScene::Draw()
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