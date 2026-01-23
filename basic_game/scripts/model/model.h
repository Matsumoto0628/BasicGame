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
    void ProcessNode(aiNode* node, const DirectX::XMMATRIX& parentTransform);
    void SetPosition(const DirectX::XMFLOAT3& pos);
    void SetRotation(const DirectX::XMFLOAT4& rot);
    void SetScale(const DirectX::XMFLOAT3& scale);
    void SetPivot(const DirectX::XMFLOAT3& pivot);
	void SetPivotRotation(const DirectX::XMFLOAT4& rot);

private:
    DirectX::XMMATRIX getModelTransform() const;

    Mesh* m_meshes = nullptr;
    unsigned int m_meshNum = 0;

    DirectX::XMFLOAT3 m_position{ 0,0,0 };
    DirectX::XMFLOAT4 m_rotation{ 0,0,0,0 };
    DirectX::XMFLOAT3 m_scale{ 1,1,1 };

    DirectX::XMFLOAT3 m_pivot = { 0,0,0 };
    DirectX::XMFLOAT4 m_pivotRotation = { 0,0,0,0 };

protected:
    virtual void initializeMaterialSet(int idx, aiMaterial* mat) = 0;

    Renderer* m_pRenderer = nullptr;
	MaterialSet m_materialSets[10];
	const aiScene* m_pScene = nullptr;
};