#pragma once

class Renderer;
class Renderer2D;

class Scene
{
public:
	virtual ~Scene() = default;
	virtual void Initialize(Renderer& renderer, Renderer2D& renderer2d) = 0;
	virtual void Setup() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Terminate() = 0;
};