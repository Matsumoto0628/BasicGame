#pragma once
#include <vector>
#include "vertex.h"
#include "material.h"

class Renderer;

class Sphere
{
public:
    Sphere(float radius, int slice, int stack);
    ~Sphere();
    void DestroyVertexBuffer();
    void Draw(Renderer& renderer);
    void SetPosition(const DirectX::XMFLOAT3& pos) { m_position = pos; }
    void SetRadius(float r) { m_radius = r; }
	void Initialize(Renderer& renderer);

private:
    void setupTransform(Renderer& renderer);
	void setupMaterial(Renderer& renderer);
	bool createMaterialBuffer(Renderer& renderer);
    bool createVertexBuffer(Renderer& renderer);

private:
    std::vector<Vertex> m_vertices;
    ID3D11Buffer* m_vertexBuffer = nullptr;

    float m_radius;
    int m_slice;
    int m_stack;

    DirectX::XMFLOAT3 m_position = { 0.f, 0.f, 0.f };
	MaterialSet m_materialSet;
};
