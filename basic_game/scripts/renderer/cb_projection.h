/**
 * @file cb_projection.h
 * @brief プロジェクション変換用定数バッファ構造体の定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

/**
 * @brief シェーダーに渡すプロジェクション変換行列データ
 */
struct CbProjection
{
	DirectX::XMFLOAT4X4 Projection;  ///< プロジェクション変換行列
};

/**
 * @brief プロジェクション定数バッファのデータと GPU バッファをまとめた構造体
 */
struct CbProjectionSet
{
	CbProjection  Data;              ///< CPU 側のバッファデータ
	ID3D11Buffer* pBuffer = nullptr; ///< GPU 定数バッファ
};
