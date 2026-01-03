#pragma once
#include <assimp/Importer.hpp>

class Renderer;
class Mesh;
class aiNode;

class Model
{
public:
    Model();
    ~Model();
    bool Setup(Renderer& renderer, const char* filePath);
    void Terminate();
    void Draw(Renderer& renderer);
    void Rotate(Renderer& renderer, float rot);
    void ProcessNode(aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransform);

private:
    Mesh* m_meshes = nullptr;
    unsigned int m_meshNum = 0;

    DirectX::XMFLOAT3 m_position{ 0,0,0 };
    DirectX::XMFLOAT3 m_rotation{ 0,0,0 };
    DirectX::XMFLOAT3 m_scale{ 1,1,1 };

    DirectX::XMFLOAT4 m_modelTransform;
};