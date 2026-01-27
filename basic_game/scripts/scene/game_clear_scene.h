#pragma once
#include "scene.h"
#include "camera.h"
#include "stage_model.h"
#include "result.h"

class GameClearScene : public Scene
{
public:
	GameClearScene();
	~GameClearScene();
	void Initialize(Renderer& renderer, Renderer2D& renderer2d) override;
	void Setup() override;
	void Update() override;
	void Draw() override;
	void Terminate() override;
private:
	Renderer* m_pRenderer = nullptr;
	Renderer2D* m_pRenderer2d = nullptr;
	Camera m_camera;
	StageModel m_stage;
	Result m_result;
	int m_bgm;
};