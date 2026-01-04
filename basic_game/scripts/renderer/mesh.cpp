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

bool Mesh::Setup(Renderer& renderer, aiMesh* pMeshData, MaterialSet& mat)
{
	m_pRenderer = &renderer;
	m_pMaterialSet = &mat;
	
	createMaterialBuffer();
	setMaterial();

	// 頂点データ取得
	m_vertexNum = pMeshData->mNumVertices;
	m_vertices = new Vertex[m_vertexNum];

	for (unsigned int vertexIdx = 0; vertexIdx < m_vertexNum; ++vertexIdx) {
		// 位置、法線
		auto& v = pMeshData->mVertices[vertexIdx];
		auto& n = pMeshData->mNormals[vertexIdx];
		m_vertices[vertexIdx].Position = { v.x, v.y, v.z };
		m_vertices[vertexIdx].Normal = { n.x, n.y, n.z };

		// テクスチャ
		if (pMeshData->HasTextureCoords(0))
		{
			auto& uv = pMeshData->mTextureCoords[0][vertexIdx];
			m_vertices[vertexIdx].TexCoord = { uv.x, uv.y };
		}
		else
		{
			m_vertices[vertexIdx].TexCoord = { 0.0f, 0.0f };
		}

	}
	if (createVertexBuffer() == false) {
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
	if (createIndexBuffer() == false) {
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

bool Mesh::createVertexBuffer()
{
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexNum;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubData;
	vertexSubData.pSysMem = m_vertices;

	auto hr = m_pRenderer->GetDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexSubData,
		&m_vertexBuffer
	);

	if (FAILED(hr)) return false;

	return true;
}

bool Mesh::createIndexBuffer()
{
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = m_indexNum * 4;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubData;
	indexSubData.pSysMem = m_indices;

	auto hr = m_pRenderer->GetDevice()->CreateBuffer(
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

void Mesh::Draw()
{
	auto pDeviceContext = m_pRenderer->GetDeviceContext();

	UINT strides[1] = { sizeof(Vertex) };
	UINT offsets[1] = { 0 };
	pDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, strides, offsets);
	pDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pMaterialSet->pBuffer);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pRenderer->GetLightSet().pBuffer);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pMaterialSet->DiffuseTex);

	pDeviceContext->IASetInputLayout(m_pMaterialSet->pShader->pInputLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->VSSetShader(m_pMaterialSet->pShader->pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pMaterialSet->pShader->pPixelShader, nullptr, 0);

	pDeviceContext->DrawIndexed(m_indexNum, 0, 0);
}

void Mesh::SetLocalTransform(const DirectX::XMMATRIX& mtx)
{
	DirectX::XMStoreFloat4x4(&m_localTransform, mtx);
}

bool Mesh::createMaterialBuffer()
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(Material);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	return SUCCEEDED(
		m_pRenderer->GetDevice()->CreateBuffer(&desc, nullptr, &m_pMaterialSet->pBuffer)
	);
}

void Mesh::setMaterial() 
{
	auto pDeviceContext = m_pRenderer->GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE mapped;
	pDeviceContext->Map(m_pMaterialSet->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	CopyMemory(mapped.pData, &m_pMaterialSet->Data, sizeof(Material));
	pDeviceContext->Unmap(m_pMaterialSet->pBuffer, 0);
}