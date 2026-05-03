/**
 * @file cb_view.h
 * @brief ビュー変換用定数バッファ構造体の定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

/**
 * @brief シェーダーに渡すビュー変換行列データ
 */
struct CbView
{
	DirectX::XMFLOAT4X4 View;  ///< ビュー変換行列
};

/**
 * @brief ビュー変換定数バッファのデータと GPU バッファをまとめた構造体
 */
struct CbViewSet
{
	CbView        Data;              ///< CPU 側のバッファデータ
	ID3D11Buffer* pBuffer = nullptr; ///< GPU 定数バッファ
};
