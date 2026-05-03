/**
 * @file sphere.h
 * @brief 球体ポリゴンクラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include <vector>
#include "vertex.h"
#include "material.h"

class Renderer;

/**
 * @brief スライス・スタック分割による球体ポリゴンを生成・描画するクラス
 *
 * 主にコライダーのデバッグ描画に使用する。
 */
class Sphere
{
public:
	/**
	 * @brief 球体パラメータを指定するコンストラクタ
	 * @param radius 球の半径
	 * @param slice  経度方向の分割数
	 * @param stack  緯度方向の分割数
	 */
	Sphere(float radius, int slice, int stack);

	/** @brief デストラクタ */
	~Sphere();

	/** @brief 頂点バッファを解放する */
	void DestroyVertexBuffer();

	/**
	 * @brief 球体を描画する
	 * @param renderer 3Dレンダラーの参照
	 */
	void Draw(Renderer& renderer);

	/**
	 * @brief 球体の中心座標を設定する
	 * @param pos ワールド座標
	 */
	void SetPosition(const DirectX::XMFLOAT3& pos) { m_position = pos; }

	/**
	 * @brief 球体の半径を設定する
	 * @param r 半径
	 */
	void SetRadius(float r) { m_radius = r; }

	/**
	 * @brief 頂点バッファと初期マテリアルを生成する
	 * @param renderer 3Dレンダラーの参照
	 */
	void Initialize(Renderer& renderer);

	/**
	 * @brief 球体の表示色を変更する
	 * @param color 新しい色（RGBA）
	 */
	void ChangeColor(DirectX::XMFLOAT4 color);

private:
	/** @brief ワールド変換を定数バッファに設定する */
	void setupTransform(Renderer& renderer);
	/** @brief マテリアルを定数バッファに設定する */
	void setupMaterial(Renderer& renderer);
	/** @brief マテリアル定数バッファを生成する */
	bool createMaterialBuffer(Renderer& renderer);
	/** @brief 頂点バッファを生成する */
	bool createVertexBuffer(Renderer& renderer);

	std::vector<Vertex> m_vertices;          ///< 頂点データ
	ID3D11Buffer* m_vertexBuffer = nullptr;  ///< 頂点バッファ

	float m_radius;  ///< 球の半径
	int m_slice;     ///< 経度分割数
	int m_stack;     ///< 緯度分割数

	DirectX::XMFLOAT3 m_position = { 0.f, 0.f, 0.f };  ///< ワールド座標
	MaterialSet m_materialSet;                           ///< マテリアルセット
	DirectX::XMFLOAT4 m_color{ 1.f, 1.f, 0.f, 1.f };   ///< 表示色（デフォルト黄色）

	Renderer* m_pRenderer = nullptr;  ///< レンダラーへのポインタ
};
