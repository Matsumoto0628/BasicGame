/**
 * @file light.h
 * @brief ライティング用定数バッファ構造体の定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

/**
 * @brief シェーダーに渡すライトデータ（方向・色・視点座標）
 */
struct Light
{
	DirectX::XMFLOAT4 LightDir;   ///< ライトの方向ベクトル
	DirectX::XMFLOAT4 LightColor; ///< ライトの色（RGBA）
	DirectX::XMFLOAT4 EyePos;     ///< 視点（カメラ）のワールド座標
};

/**
 * @brief ライト定数バッファのデータと GPU バッファをまとめた構造体
 */
struct LightSet
{
	Light         Data;              ///< CPU 側のライトデータ
	ID3D11Buffer* pBuffer = nullptr; ///< GPU 定数バッファ
};
