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
    if (GetKeyState(VK_LEFT) & 0x80){
        m_eyePos.y += 0.03f;
    }
    // 0.03は適当なので環境に合わせて変えてOK
    if (m_eyePos.z < -1000.f) {
        m_eyePos.z = 0.f;
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