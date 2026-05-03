/**
 * @file vertex.h
 * @brief 頂点データ構造体の定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

/**
 * @brief 3D 頂点の座標・カラー・法線・UV を保持する構造体
 *
 * 頂点バッファのレイアウトに対応し、シェーダーの入力レイアウトと一致させる。
 */
struct Vertex
{
	DirectX::XMFLOAT3 Position = {};  ///< ワールド座標（XYZ）
	DirectX::XMFLOAT4 Color = {};     ///< 頂点カラー（RGBA）
	DirectX::XMFLOAT3 Normal = {};    ///< 法線ベクトル
	DirectX::XMFLOAT2 TexCoord = {};  ///< UV テクスチャ座標

	/** @brief デフォルトコンストラクタ */
	Vertex() {}

	/**
	 * @brief XYZ 座標を指定するコンストラクタ
	 * @param x X 座標
	 * @param y Y 座標
	 * @param z Z 座標
	 */
	Vertex(float x, float y, float z) {
		Position.x = x;
		Position.y = y;
		Position.z = z;
	}
};
