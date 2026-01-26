#include "sphere.h"
#include "renderer.h"

Sphere::Sphere(float radius, int slice, int stack)
    : m_radius(radius), m_slice(slice), m_stack(stack)
{
    for (int i = 0; i <= m_stack; ++i)
    {
        float v = float(i) / m_stack;
        float phi = DirectX::XM_PI * v;

        for (int j = 0; j <= m_slice; ++j)
        {
            float u = float(j) / m_slice;
            float theta = DirectX::XM_2PI * u;

            float x = m_radius * sinf(phi) * cosf(theta);
            float y = m_radius * cosf(phi);
            float z = m_radius * sinf(phi) * sinf(theta);

            Vertex vtx;
            vtx.Position = { x, y, z };
            m_vertices.push_back(vtx);
        }
    }
}

Sphere::~Sphere()
{
    DestroyVertexBuffer();
}

void Sphere::Initialize(Renderer& renderer)
{
	m_pRenderer = &renderer;
	createVertexBuffer(renderer);
	createMaterialBuffer(renderer);
	setupMaterial(renderer);
}

bool Sphere::createVertexBuffer(Renderer& renderer)
{
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = UINT(sizeof(Vertex) * m_vertices.size());
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sub = {};
    sub.pSysMem = m_vertices.data();

    HRESULT hr = renderer.GetDevice()->CreateBuffer(
        &desc,
        &sub,
        &m_vertexBuffer
    );

    return SUCCEEDED(hr);
}

void Sphere::DestroyVertexBuffer()
{
    DX_SAFE_RELEASE(m_vertexBuffer);
}

void Sphere::Draw(Renderer& renderer)
{
    setupTransform(renderer);

    auto ctx = renderer.GetDeviceContext();
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    ctx->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    ctx->PSSetConstantBuffers(0, 1, &m_materialSet.pBuffer);
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    ctx->VSSetShader(renderer.SimpleShader.pVertexShader, nullptr, 0);
    ctx->PSSetShader(renderer.SimpleShader.pPixelShader, nullptr, 0);

    ctx->Draw(UINT(m_vertices.size()), 0);
}

void Sphere::setupTransform(Renderer& renderer)
{
    auto ctx = renderer.GetDeviceContext();
    auto cb = renderer.GetRenderParam().CbTransformSet;

    DirectX::XMMATRIX scale =
        DirectX::XMMatrixScaling(m_radius, m_radius, m_radius);

    DirectX::XMMATRIX trans =
        DirectX::XMMatrixTranslation(
            m_position.x, m_position.y, m_position.z);

    DirectX::XMMATRIX mtx = scale * trans;

    DirectX::XMStoreFloat4x4(
        &cb.Data.Transform,
        DirectX::XMMatrixTranspose(mtx));

    D3D11_MAPPED_SUBRESOURCE mapped{};

    if (SUCCEEDED(ctx->Map(cb.pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
    {
        CopyMemory(mapped.pData, &cb.Data, sizeof(cb.Data));
        ctx->Unmap(cb.pBuffer, 0);
    }

    ctx->VSSetConstantBuffers(0, 1, &cb.pBuffer);
}

void Sphere::setupMaterial(Renderer& renderer)
{
    auto ctx = renderer.GetDeviceContext();
    auto cb = renderer.GetRenderParam().CbTransformSet;
    D3D11_MAPPED_SUBRESOURCE mapped{};

    m_materialSet.Data.Diffuse = m_color;

    if (SUCCEEDED(ctx->Map(
        m_materialSet.pBuffer, 0,
        D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
    {
        CopyMemory(mapped.pData,
            &m_materialSet.Data,
            sizeof(Material));
        ctx->Unmap(m_materialSet.pBuffer, 0);
    }

    ctx->PSSetConstantBuffers(0, 1, &m_materialSet.pBuffer);
}

bool Sphere::createMaterialBuffer(Renderer &renderer)
{
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = sizeof(Material);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    return SUCCEEDED(
        renderer.GetDevice()->CreateBuffer(&desc, nullptr, &m_materialSet.pBuffer)
    );
}

void Sphere::ChangeColor(DirectX::XMFLOAT4 color)
{
	m_color = color;
	setupMaterial(*m_pRenderer);
}