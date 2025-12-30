#include "mesh.h"
#include "renderer.h"
#include <assimp/mesh.h>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	Terminate();
}

bool Mesh::Setup(Renderer& renderer, aiMesh* pMeshData)
{
	// 頂点データ取得
	m_vertexNum = pMeshData->mNumVertices;
	m_vertices = new Vertex[m_vertexNum];
	for (unsigned int vertexIdx = 0; vertexIdx < m_vertexNum; ++vertexIdx) {
		auto& pos = pMeshData->mVertices[vertexIdx];
		m_vertices[vertexIdx].Position = DirectX::XMFLOAT3(pos.x, pos.y, pos.z);
		constexpr float COLOR = 0.5f;
		m_vertices[vertexIdx].Color = DirectX::XMFLOAT4(COLOR, COLOR, COLOR, 1.f);
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

	pDeviceContext->DrawIndexed(m_indexNum, 0, 0);
}