#include "camera.h"

Camera::Camera()
    : m_eyePos(0, 0, -5)
    , m_focutPos(0, 0, 0)
{
}

Camera::~Camera()
{

}

void Camera::Update()
{
    // 0.03は適当なので環境に合わせて変えてOK
    m_eyePos.y += 0.03f;
    if (m_eyePos.y > 10.f) {
        m_eyePos.y = 0.f;
    }
}

/*
 *    View変換行列取得
 */
DirectX::XMMATRIX Camera::GetViewMatrix() const
{
    // ビュー変換行列を求める
    DirectX::XMMATRIX viewMat = DirectX::XMMatrixLookAtLH(
        DirectX::XMLoadFloat3(&m_eyePos), DirectX::XMLoadFloat3(&m_focutPos), DirectX::XMLoadFloat3(&m_up));
    return viewMat;
}