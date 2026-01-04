#pragma once
#include <assimp/Importer.hpp>
#include "material.h"

class Renderer;
class Mesh;
class aiNode;
class aiMaterial;

class Model
{
public:
    Model();
    ~Model();
    bool Setup(Renderer& renderer, const char* filePath);
    void Terminate();
    void Draw();
    void ProcessNode(aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransform);
    void SetPosition(const DirectX::XMFLOAT3& pos);
    void SetRotation(const DirectX::XMFLOAT3& rot);
    void SetScale(const DirectX::XMFLOAT3& scale);
    virtual void Update() = 0;

private:
    DirectX::XMMATRIX getModelTransform() const;

private:
    Mesh* m_meshes = nullptr;
    unsigned int m_meshNum = 0;

    DirectX::XMFLOAT3 m_position{ 0,0,0 };
    DirectX::XMFLOAT3 m_rotation{ 0,0,0 };
    DirectX::XMFLOAT3 m_scale{ 1,1,1 };

    DirectX::XMFLOAT4 m_modelTransform;

protected:
    virtual void initializeMaterialSet(int idx, aiMaterial* mat) = 0;

    Renderer* m_pRenderer = nullptr;
	MaterialSet m_materialSets[3];
};