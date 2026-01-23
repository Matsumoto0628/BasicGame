#pragma once
#include "material.h"

struct aiMesh;
struct Vertex;
class Renderer;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool Setup(Renderer& renderer, aiMesh* pMeshData, MaterialSet& mat);
    void Terminate();
    void Draw();
    void SetLocalTransform(const DirectX::XMMATRIX& mtx);
    const DirectX::XMFLOAT4X4& GetLocalTransform() const { return m_localTransform; }

private:
    bool createVertexBuffer();
    bool createIndexBuffer();
    void destroyVertexBuffer();
    void destroyIndexBuffer();
	bool createMaterialBuffer();
    void setMaterial();

    Renderer* m_pRenderer;

    // データの解釈ワーク
    Vertex* m_vertices = nullptr;
    unsigned int* m_indices = nullptr;
    unsigned int m_vertexNum = 0;
    unsigned int m_indexNum = 0;

    // バッファリソース
    ID3D11Buffer* m_vertexBuffer = nullptr;
    ID3D11Buffer* m_indexBuffer = nullptr;

    DirectX::XMFLOAT4X4 m_localTransform;

    MaterialSet* m_pMaterialSet;
};