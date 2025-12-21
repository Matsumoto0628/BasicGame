#pragma once
#include "vertex.h"

class Renderer;

struct Triangle 
{
	static constexpr size_t VERTEX_NUM = 3;
	Vertex Vertices[VERTEX_NUM];
	ID3D11Buffer* VertexBuffer = nullptr;

	Triangle();
	~Triangle();
	bool CreateVertexBuffer(Renderer& renderer);
	void Draw(Renderer& renderer);
};