#pragma once
#include "scene.h"
#include "camera.h"
#include "stage_model.h"
#include "title.h"

class TitleScene : public Scene
{
public:
	TitleScene();
	~TitleScene();
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
	Title m_title;
	int m_bgm;
};