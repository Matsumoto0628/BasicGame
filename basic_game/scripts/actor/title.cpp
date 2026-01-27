#include "title.h"
#include "input_manager.h"
#include "scene_manager.h"

Title::Title()
{
}
Title::~Title()
{
}

void Title::Setup()
{
}	

void Title::Update()
{
	if (InputManager::Instance().GetKeyDown(VK_SPACE))
	{
		SceneManager::Instance().Transit(SceneManager::SceneType::Game);
	}
}

void Title::Draw()
{
}

void Title::Terminate()
{
}