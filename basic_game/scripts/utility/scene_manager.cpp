#include "scene_manager.h"
#include "renderer.h"
#include "renderer_2d.h"
#include "scene.h"
#include "game_scene.h"
#include "title_scene.h"
#include "game_over_scene.h"
#include "game_clear_scene.h"

class ID2DI1Bitmap;

SceneManager& SceneManager::Instance()
{
	static SceneManager instance;
	return instance;
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize(Renderer& renderer, Renderer2D& renderer2d)
{
	m_pRenderer = &renderer;
	m_pRenderer2d = &renderer2d;
}

void SceneManager::Terminate()
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene->Terminate();
		delete m_pCurrentScene;
		m_pCurrentScene = nullptr;
	}
}

void SceneManager::Update()
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene->Update();
	}

	if (m_isTransit) 
	{
		m_isTransit = false;
		transit();
	}
}

void SceneManager::Draw()
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene->Draw();
	}
}

void SceneManager::Transit(SceneType sceneType)
{
	m_isTransit = true;
	m_currentSceneType = sceneType;
}

void SceneManager::transit() 
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene->Terminate();
		delete m_pCurrentScene;
		m_pCurrentScene = nullptr;
	}

	switch (m_currentSceneType)
	{
	case SceneManager::Game:
		m_pCurrentScene = new GameScene();
		break;
	case SceneManager::Title:
		m_pCurrentScene = new TitleScene();
		break;
	case SceneManager::GameOver:
		m_pCurrentScene = new GameOverScene();
		break;
	case SceneManager::GameClear:
		m_pCurrentScene = new GameClearScene();
		break;
	default:
		break;
	}

	m_pCurrentScene->Initialize(*m_pRenderer, *m_pRenderer2d);
	m_pCurrentScene->Setup();
}