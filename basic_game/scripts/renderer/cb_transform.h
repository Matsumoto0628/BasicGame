/**
 * @file cb_transform.h
 * @brief ワールド変換用定数バッファ構造体の定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

/**
 * @brief シェーダーに渡すワールド変換行列データ
 */
struct CbTransform
{
	DirectX::XMFLOAT4X4 Transform;  ///< ワールド変換行列
};

/**
 * @brief ワールド変換定数バッファのデータと GPU バッファをまとめた構造体
 */
struct CbTransformSet
{
	CbTransform   Data;              ///< CPU 側のバッファデータ
	ID3D11Buffer* pBuffer = nullptr; ///< GPU 定数バッファ
};
