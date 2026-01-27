#include "result.h"
#include "input_manager.h"
#include "scene_manager.h"

Result::Result()
{
}
Result::~Result()
{
}

void Result::Setup()
{
}

void Result::Update()
{
	if (InputManager::Instance().GetKeyDown(VK_SPACE))
	{
		SceneManager::Instance().Transit(SceneManager::SceneType::Title);
	}
}

void Result::Draw()
{
}

void Result::Terminate()
{
}