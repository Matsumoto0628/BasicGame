#pragma once
#include "camera.h"
#include "triangle.h"
#include "model.h"

class Renderer;

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void Initialize(Renderer& renderer);
    void Terminate();

    void Update();
    void Draw();
private:
    Renderer* m_pRenderer = nullptr;
    Camera m_camera;
	Model m_sampleModel;
};