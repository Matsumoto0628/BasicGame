#include "euler_converter.h"

DirectX::XMFLOAT4 EulerToQuaternion(const DirectX::XMFLOAT3& eulerAngles)
{
    DirectX::XMVECTOR q =
        DirectX::XMQuaternionRotationRollPitchYaw(
            DirectX::XMConvertToRadians(eulerAngles.x),
			DirectX::XMConvertToRadians(eulerAngles.y),
			DirectX::XMConvertToRadians(eulerAngles.z)
        );

    DirectX::XMFLOAT4 quat;
    DirectX::XMStoreFloat4(&quat, q);
    return quat;
}