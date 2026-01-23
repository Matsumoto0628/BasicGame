#include "collider.h"
#include "sphere.h"

Collider::Collider()
{
}

Collider::~Collider()
{
}

bool Collider::Intersects(const Collider& other) const
{
    DirectX::XMVECTOR a = DirectX::XMLoadFloat3(&m_position);
    DirectX::XMVECTOR b = DirectX::XMLoadFloat3(&other.m_position);

    float distSq;
    DirectX::XMStoreFloat(&distSq,
        DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(a, b)));

    float r = m_radius + other.m_radius;
    return distSq <= r * r;
}

void Collider::Initialize(Renderer& renderer)
{
	m_pRenderer = &renderer;
	m_pSphere = new Sphere(m_radius, 16, 16);
	m_pSphere->SetPosition(m_position);
	m_pSphere->Initialize(renderer);
}

void Collider::Draw() 
{
    m_pSphere->Draw(*m_pRenderer);
}

void Collider::Update()
{
	m_pSphere->SetPosition(m_position);
	m_pSphere->SetRadius(m_radius);
}