#pragma once

struct Triangle 
{
	static constexpr size_t VERTEX_NUM = 3;
	Vertex vertices[VERTEX_NUM];
	ID3D11Buffer* vertexBuffer = nullptr;

	Triangle();
	~Triangle();
	bool CreateVertexBuffer(Renderer& renderer);
	void Draw(Renderer& renderer);
};