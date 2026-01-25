#pragma once

class Sphere;
class Renderer;

class Collider
{
public:
    Collider();
    ~Collider();
    bool Intersects(const Collider& other) const;
    void SetPosition(const DirectX::XMFLOAT3& pos) { m_position = pos; }
    void SetRadius(float r) { m_radius = r; }
    void Initialize(Renderer& renderer);
    void Draw();
    void Update();
	void SetActive(bool isActive) { m_isActive = isActive; }

private:
    DirectX::XMFLOAT3 m_position{ 0,0,0 };
    float m_radius = 1.0f;
	Sphere* m_pSphere = nullptr;
	Renderer* m_pRenderer = nullptr;
    bool m_isActive = false;
};
