#pragma once
#include "vertex.h"

class Renderer;

struct Triangle 
{
public:
	static constexpr size_t VERTEX_NUM = 3;
	Vertex Vertices[VERTEX_NUM];
	ID3D11Buffer* VertexBuffer = nullptr;

public:
	Triangle();
	~Triangle();
	bool CreateVertexBuffer(Renderer& renderer);
	void Draw(Renderer& renderer);

private:
	void setupTransform(Renderer& renderer);
};