#include "model.h"
#include "mesh.h"
#include "renderer.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model()
{

}

Model::~Model()
{
    SAFE_DELETE_ARRAY(m_meshes);
}

bool Model::Setup(Renderer& renderer, const char* filePath)
{
	m_pRenderer = &renderer;

    // load処理
    Assimp::Importer importer;
    unsigned int flag = 
        aiProcess_Triangulate |
        aiProcess_FlipUVs;
    const aiScene* pScene = importer.ReadFile(filePath, flag);

    if (pScene == nullptr) return false;

    m_meshNum = pScene->mNumMeshes;
    if (m_meshNum > 0) {
        m_meshes = new Mesh[m_meshNum];
        for (unsigned int meshIdx = 0; meshIdx < pScene->mNumMeshes; ++meshIdx) {

            auto pMeshData = pScene->mMeshes[meshIdx];

            // ここで Material を取得
            auto mat = pScene->mMaterials[pMeshData->mMaterialIndex];
            initializeMaterialSet(meshIdx, mat);

            // Mesh に Mesh + Material を渡す
            if (m_meshes[meshIdx].Setup(*m_pRenderer, pMeshData, m_materialSets[meshIdx]) == false) {
                return false;
            }
        }
    }

    ProcessNode(pScene->mRootNode, DirectX::XMMatrixIdentity());

    return true;
}

void Model::Terminate()
{
    for (unsigned int meshIdx = 0; meshIdx < m_meshNum; ++meshIdx) {
        m_meshes[meshIdx].Terminate();
    }
    SAFE_DELETE_ARRAY(m_meshes);
    m_meshNum = 0;
}

void Model::Draw()
{
    // SRTを取得
    auto modelWorld = getModelTransform();

    // メッシュごとに描画
    for (unsigned int meshIdx = 0; meshIdx < m_meshNum; ++meshIdx) {
        auto cb = m_pRenderer->GetRenderParam().CbTransformSet;
        DirectX::XMMATRIX meshLocal = DirectX::XMLoadFloat4x4(&m_meshes[meshIdx].GetLocalTransform());
        DirectX::XMMATRIX world = meshLocal * modelWorld;

        DirectX::XMStoreFloat4x4(&cb.Data.Transform, DirectX::XMMatrixTranspose(world));

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        // CBufferにひもづくハードウェアリソースマップ取得（ロックして取得）
        auto pDeviceContext = m_pRenderer->GetDeviceContext();
        // Mapでロックして安全にデータ書き込み
        HRESULT hr = pDeviceContext->Map(cb.pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if (FAILED(hr)) {
            //DXTRACE_ERR(L"DrawSceneGraph failed", hr);
            return;
        }
        CopyMemory(mappedResource.pData, &cb.Data, sizeof(cb.Data));
        // マップ解除
        pDeviceContext->Unmap(cb.pBuffer, 0);

        pDeviceContext->VSSetConstantBuffers(0, 1, &cb.pBuffer);

        m_meshes[meshIdx].Draw();
    }
}

void Model::ProcessNode(aiNode* node, const DirectX::XMMATRIX& parentTransform)
{
    DirectX::XMMATRIX nodeTransform = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&node->mTransformation));

    nodeTransform = DirectX::XMMatrixTranspose(nodeTransform);
    DirectX::XMMATRIX globalTransform = nodeTransform * parentTransform;

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        unsigned int meshIndex = node->mMeshes[i];
        m_meshes[meshIndex].SetLocalTransform(globalTransform);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], globalTransform);
    }
}

DirectX::XMMATRIX Model::getModelTransform() const
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
    DirectX::XMMATRIX R =
        DirectX::XMMatrixRotationQuaternion(
            DirectX::XMLoadFloat4(&m_rotation)
		);
    DirectX::XMMATRIX Rp =
        DirectX::XMMatrixRotationQuaternion(
            DirectX::XMLoadFloat4(&m_pivotRotation)
        );
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	DirectX::XMMATRIX Tp = DirectX::XMMatrixTranslation(-m_pivot.x, -m_pivot.y, -m_pivot.z);
	DirectX::XMMATRIX TpInv = DirectX::XMMatrixTranslation(m_pivot.x, m_pivot.y, m_pivot.z);

    return S * Tp * Rp * TpInv * R * T;
}

void Model::SetPosition(const DirectX::XMFLOAT3& pos)
{
    m_position = pos;
}

void Model::SetRotation(const DirectX::XMFLOAT4& rot)
{
    m_rotation = rot;
}

void Model::SetScale(const DirectX::XMFLOAT3& scale)
{
    m_scale = scale;
}

void Model::SetPivot(const DirectX::XMFLOAT3& pivot)
{
	m_pivot = pivot;
}

void Model::SetPivotRotation(const DirectX::XMFLOAT4& rot)
{
    m_pivotRotation = rot;
}

void Model::ChangeMaterial() 
{
    if (m_meshNum > 0) {
        for (unsigned int meshIdx = 0; meshIdx < m_meshNum; ++meshIdx) {
            m_meshes[meshIdx].ChangeMaterial(m_materialSets[meshIdx]);
        }
    }
}