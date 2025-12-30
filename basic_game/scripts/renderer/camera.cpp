#include "camera.h"
#include "input_manager.h"

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
    if (InputManager::Instance().GetKeyUp(VK_UP)) {
        m_eyePos.y += 0.03f;
    }
    if (InputManager::Instance().GetKeyUp(VK_DOWN)) {
        m_eyePos.y -= 0.03f;
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