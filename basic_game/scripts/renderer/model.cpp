#include "model.h"
#include "mesh.h"
#include "renderer.h"
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
    // load処理
    Assimp::Importer importer;
    unsigned int flag = aiProcess_Triangulate;
    auto pScene = importer.ReadFile(filePath, flag);

    if (pScene == nullptr) return false;

    m_meshNum = pScene->mNumMeshes;
    if (m_meshNum > 0) {
        m_meshes = new Mesh[m_meshNum];
        for (unsigned int meshIdx = 0; meshIdx < pScene->mNumMeshes; ++meshIdx) {

            auto pMeshData = pScene->mMeshes[meshIdx];

            // ここで Material を取得
            auto mat = pScene->mMaterials[pMeshData->mMaterialIndex];

            // Mesh に Mesh + Material を渡す
            if (m_meshes[meshIdx].Setup(renderer, pMeshData, mat) == false) {
                return false;
            }
        }
    }

    ProcessNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());

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

void Model::Draw(Renderer& renderer)
{
    auto modelWorld = DirectX::XMMatrixIdentity();
    for (unsigned int meshIdx = 0; meshIdx < m_meshNum; ++meshIdx) {
        auto cb = renderer.GetRenderParam().CbTransformSet;
        DirectX::XMMATRIX meshLocal = DirectX::XMLoadFloat4x4(&m_meshes[meshIdx].GetLocalTransform());
        DirectX::XMMATRIX world = meshLocal * modelWorld;

        DirectX::XMStoreFloat4x4(&cb.Data.Transform, DirectX::XMMatrixTranspose(world));
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        // CBufferにひもづくハードウェアリソースマップ取得（ロックして取得）
        auto pDeviceContext = renderer.GetDeviceContext();
        // Mapでロックして安全にデータ書き込み
        HRESULT hr = pDeviceContext->Map(
            cb.pBuffer,
            0,
            D3D11_MAP_WRITE_DISCARD,
            0,
            &mappedResource);
        if (FAILED(hr)) {
            //DXTRACE_ERR(L"DrawSceneGraph failed", hr);
            return;
        }
        CopyMemory(mappedResource.pData, &cb.Data, sizeof(cb.Data));
        // マップ解除
        pDeviceContext->Unmap(cb.pBuffer, 0);
        pDeviceContext->VSSetConstantBuffers(0, 1, &cb.pBuffer);

        m_meshes[meshIdx].Draw(renderer);
    }
}

void Model::Rotate(Renderer& renderer, float rot)
{
    auto cb = renderer.GetRenderParam().CbTransformSet;

    float scale = 0.1f;
    float angle = DirectX::XMConvertToRadians(rot); // 回転角（度→ラジアン）

    auto mtx = DirectX::XMMatrixScaling(scale, scale, scale) 
        * DirectX::XMMatrixRotationX(angle);

    DirectX::XMStoreFloat4x4(&cb.Data.Transform, DirectX::XMMatrixTranspose(mtx));
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    // CBufferにひもづくハードウェアリソースマップ取得（ロックして取得）
    auto pDeviceContext = renderer.GetDeviceContext();
    // Mapでロックして安全にデータ書き込み
    HRESULT hr = pDeviceContext->Map(
        cb.pBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource);
    if (FAILED(hr)) {
        //DXTRACE_ERR(L"DrawSceneGraph failed", hr);
        return;
    }
    CopyMemory(mappedResource.pData, &cb.Data, sizeof(cb.Data));
    // マップ解除
    pDeviceContext->Unmap(cb.pBuffer, 0);
    pDeviceContext->VSSetConstantBuffers(0, 1, &cb.pBuffer);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransform)
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
        ProcessNode(node->mChildren[i], scene, globalTransform);
    }
}