/**
 * @file triangle.h
 * @brief 三角形ポリゴン構造体の定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "vertex.h"

class Renderer;

/**
 * @brief 3 頂点の三角形ポリゴンを管理する構造体
 *
 * 主にレンダラーの動作確認・デバッグ用途に使用する。
 */
struct Triangle
{
public:
	static constexpr size_t VERTEX_NUM = 3;  ///< 頂点数（常に 3）
	Vertex Vertices[VERTEX_NUM];             ///< 頂点データ配列
	ID3D11Buffer* VertexBuffer = nullptr;    ///< 頂点バッファ

	/** @brief コンストラクタ */
	Triangle();

	/** @brief デストラクタ */
	~Triangle();

	/**
	 * @brief 頂点バッファを GPU 上に生成する
	 * @param renderer 3Dレンダラーの参照
	 * @return 成功すれば true
	 */
	bool CreateVertexBuffer(Renderer& renderer);

	/** @brief 頂点バッファを解放する */
	void DestroyVertexBuffer();

	/**
	 * @brief 三角形を描画する
	 * @param renderer 3Dレンダラーの参照
	 */
	void Draw(Renderer& renderer);

private:
	/** @brief ワールド変換を定数バッファに設定する */
	void setupTransform(Renderer& renderer);
};
