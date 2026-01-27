#include "title_scene.h"
#include "renderer.h"
#include "renderer_2d.h"
#include "game_text.h"
#include "game_image.h"
#include "euler_converter.h"
#include "sound_manager.h"

class ID2DI1Bitmap;

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize(Renderer& renderer, Renderer2D& renderer2d)
{
	m_pRenderer = &renderer;
	m_pRenderer2d = &renderer2d;

	const char* STAGE_PATH = "models/field/field.obj";
	m_stage.Setup(renderer, STAGE_PATH);
}

void TitleScene::Setup()
{
	m_stage.SetPosition({ 0.f, 0.f, 0.f });
	m_stage.SetScale({ 0.05f, 0.05f, 0.05f });

	m_pRenderer2d->AddText(new GameText(L"Escでカーソル表示、ゲームに戻る", 0.f, 10.f, DirectX::XMFLOAT4(1.f, 1.f, 0.f, 1.f)));
	m_pRenderer2d->AddText(new GameText(L"Spaceでゲームスタート", 400.f, 400.f, DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f)));
	m_pRenderer2d->AddImage(new GameImage(m_pRenderer2d->LoadBitmapFromFile(L"images/title.png"), 0.f, 0.f, 960.f, 540.0f));

	m_bgm = SoundManager::Instance().LoadSound(L"sounds/heiannoyoi.wav");
	SoundManager::Instance().PlaySoundW(m_bgm, true);
}

void TitleScene::Terminate()
{
	m_stage.Terminate();
	m_pRenderer2d->ClearTexts();
	m_pRenderer2d->ClearImages();
	SoundManager::Instance().StopSound(m_bgm);
}

void TitleScene::Update()
{
	m_pRenderer->SetEyePosLight(m_camera.GetEyePos());
	m_camera.Update();
	m_title.Update();

	static float timer = 0.f;
	timer += 0.017f;
	m_camera.SetRotation(EulerToQuaternion({ 0.f, timer * 2.f, 0.f }));
}

void TitleScene::Draw()
{
	auto viewMatrix = m_camera.GetViewMatrix();
	m_pRenderer->SetupViewTransform(viewMatrix);
	m_stage.Draw();
}