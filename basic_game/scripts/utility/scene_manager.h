#pragma once

class Renderer;
class Renderer2D;
class Scene;

class SceneManager
{
public:
	enum SceneType
	{
		Game,
		Title,
		GameOver,
		GameClear
	};
    SceneManager();
    ~SceneManager();
    void Initialize(Renderer& renderer, Renderer2D& renderer2d);
    void Terminate();
    void Update();
    void Draw();
	void Transit(SceneType sceneType);
	static SceneManager& Instance();

private:
	void transit();

    Renderer* m_pRenderer = nullptr;
	Renderer2D* m_pRenderer2d = nullptr;
	Scene* m_pCurrentScene = nullptr;
	SceneType m_currentSceneType = SceneType::Title;
	bool m_isTransit = false;
};