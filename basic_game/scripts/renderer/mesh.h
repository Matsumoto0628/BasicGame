#pragma once

struct aiMesh;
struct Vertex;
class Renderer;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool Setup(Renderer& renderer, aiMesh* pMeshData);
    void Terminate();
    void Draw(Renderer& renderer);

private:
    bool createVertexBuffer(Renderer& renderer);
    bool createIndexBuffer(Renderer& renderer);
    void destroyVertexBuffer();
    void destroyIndexBuffer();

private:
    // データの解釈ワーク
    Vertex* m_vertices = nullptr;
    unsigned int* m_indices = nullptr;
    unsigned int m_vertexNum = 0;
    unsigned int m_indexNum = 0;

    // バッファリソース
    ID3D11Buffer* m_vertexBuffer = nullptr;
    ID3D11Buffer* m_indexBuffer = nullptr;
};