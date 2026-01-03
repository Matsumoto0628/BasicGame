#pragma once
#include "material.h"

struct aiMesh;
struct Vertex;
class Renderer;
class aiMaterial;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool Setup(Renderer& renderer, aiMesh* pMeshData, aiMaterial* mat);
    void Terminate();
    void Draw(Renderer& renderer);
    void SetLocalTransform(const DirectX::XMMATRIX& mtx);
    const DirectX::XMFLOAT4X4& GetLocalTransform() const { return m_localTransform; }

private:
    bool createVertexBuffer(Renderer& renderer);
    bool createIndexBuffer(Renderer& renderer);
    void destroyVertexBuffer();
    void destroyIndexBuffer();
	bool createMaterialBuffer(Renderer& renderer);
    void setMaterial(Renderer& renderer);

private:
    // データの解釈ワーク
    Vertex* m_vertices = nullptr;
    unsigned int* m_indices = nullptr;
    unsigned int m_vertexNum = 0;
    unsigned int m_indexNum = 0;

    // バッファリソース
    ID3D11Buffer* m_vertexBuffer = nullptr;
    ID3D11Buffer* m_indexBuffer = nullptr;

	MaterialSet m_materialSet;

    DirectX::XMFLOAT4X4 m_localTransform;
};