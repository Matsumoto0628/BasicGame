#include "mesh.h"
#include "renderer.h"
#include <assimp/mesh.h>
#include <assimp/material.h>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	Terminate();
}

bool Mesh::Setup(Renderer& renderer, aiMesh* pMeshData, aiMaterial* mat)
{
	// ライトの設定
	m_light.Data.LightDir = DirectX::XMFLOAT4(5.f, -5.f, 0.f, 1.f);
	m_light.Data.LightColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_light.Data.EyePos = DirectX::XMFLOAT4(10.f, 10.f, -10.f, 1.f);

	bool result = createLightBuffer(renderer);
	updateLight(renderer);

	// マテリアルの Diffuse Color を取得
	aiColor4D diffuse, specular;
	float shininess = 0.0f;

	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular);
	aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);

	m_material.Data.Diffuse =
		DirectX::XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	m_material.Data.Specular =
		DirectX::XMFLOAT4(1, 1, 1, 1);//本来はspecular.r
	m_material.Data.Shininess = 10;//本来はshininess

	createMaterialBuffer(renderer);

	// 頂点データ取得
	m_vertexNum = pMeshData->mNumVertices;
	m_vertices = new Vertex[m_vertexNum];

	for (unsigned int vertexIdx = 0; vertexIdx < m_vertexNum; ++vertexIdx) {
		auto& v = pMeshData->mVertices[vertexIdx];
		auto& n = pMeshData->mNormals[vertexIdx];

		m_vertices[vertexIdx].Position = { v.x, v.y, v.z };
		m_vertices[vertexIdx].Normal = { n.x, n.y, n.z };
	}
	if (createVertexBuffer(renderer) == false) {
		return false;
	}

	// 頂点インデックスデータ取得（TriangleList前提）
	m_indexNum = pMeshData->mNumFaces * 3;
	m_indices = new unsigned int[m_indexNum];
	for (unsigned int faceIdx = 0; faceIdx < pMeshData->mNumFaces; ++faceIdx) {
		auto& face = pMeshData->mFaces[faceIdx];
		assert(face.mNumIndices == 3);
		for (unsigned int idx = 0; idx < 3; ++idx) {
			m_indices[faceIdx * 3 + idx] = face.mIndices[idx];
		}
	}
	if (createIndexBuffer(renderer) == false) {
		return false;
	}

	return true;
}

void Mesh::Terminate()
{
	destroyVertexBuffer();
	destroyIndexBuffer();
	SAFE_DELETE_ARRAY(m_vertices);
	m_vertexNum = 0;
	SAFE_DELETE_ARRAY(m_indices);
	m_indexNum = 0;
}

bool Mesh::createVertexBuffer(Renderer& renderer)
{
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexNum;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubData;
	vertexSubData.pSysMem = m_vertices;

	auto hr = renderer.GetDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexSubData,
		&m_vertexBuffer
	);

	if (FAILED(hr)) return false;

	return true;
}

bool Mesh::createIndexBuffer(Renderer& renderer)
{
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = m_indexNum * 4;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubData;
	indexSubData.pSysMem = m_indices;

	auto hr = renderer.GetDevice()->CreateBuffer(
		&indexBufferDesc,
		&indexSubData,
		&m_indexBuffer
	);

	if (FAILED(hr)) return false;

	return true;
}

void Mesh::destroyVertexBuffer()
{
	DX_SAFE_RELEASE(m_vertexBuffer);
}

void Mesh::destroyIndexBuffer()
{
	DX_SAFE_RELEASE(m_indexBuffer);
}

void Mesh::Draw(Renderer& renderer)
{
	//setupTransform(renderer);

	auto pDeviceContext = renderer.GetDeviceContext();

	UINT strides[1] = { sizeof(Vertex) };
	UINT offsets[1] = { 0 };
	pDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, strides, offsets);
	pDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Material CBuffer 更新
	D3D11_MAPPED_SUBRESOURCE mapped;
	pDeviceContext->Map(m_materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	CopyMemory(mapped.pData, &m_material, sizeof(Material));
	pDeviceContext->Unmap(m_materialBuffer, 0);

	// PixelShader にバインド
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_materialBuffer);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_light.pBuffer);

	pDeviceContext->DrawIndexed(m_indexNum, 0, 0);
}

bool Mesh::createMaterialBuffer(Renderer& renderer)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(Material);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	return SUCCEEDED(
		renderer.GetDevice()->CreateBuffer(&desc, nullptr, &m_materialBuffer)
	);
}

bool Mesh::createLightBuffer(Renderer& renderer)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(LightData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	return SUCCEEDED(
		renderer.GetDevice()->CreateBuffer(&desc, nullptr, &m_light.pBuffer)
	);
}

void Mesh::updateLight(Renderer& renderer)
{
	auto ctx = renderer.GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE mapped;
	ctx->Map(m_light.pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &m_light.Data, sizeof(LightData));
	ctx->Unmap(m_light.pBuffer, 0);
}

void Mesh::SetLocalTransform(const DirectX::XMMATRIX& mtx)
{
	DirectX::XMStoreFloat4x4(&m_localTransform, mtx);
}