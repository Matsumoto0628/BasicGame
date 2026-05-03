/**
 * @file render_param.h
 * @brief レンダリングに必要な定数バッファをまとめた構造体の定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "cb_transform.h"
#include "cb_projection.h"
#include "cb_view.h"

class Renderer;

/**
 * @brief ワールド・プロジェクション・ビューの各定数バッファセットを集約する構造体
 *
 * Renderer が初期化し、各メッシュ描画時に参照する。
 */
struct RenderParam
{
public:
	CbTransformSet  CbTransformSet;   ///< ワールド変換定数バッファセット
	CbProjectionSet CbProjectionSet;  ///< プロジェクション定数バッファセット
	CbViewSet       CbViewSet;        ///< ビュー変換定数バッファセット

	/**
	 * @brief 各定数バッファを GPU 上に生成する
	 * @param renderer 3Dレンダラーの参照
	 * @return 成功すれば true
	 */
	bool Initialize(Renderer& renderer);

	/**
	 * @brief 各定数バッファを解放する
	 * @param renderer 3Dレンダラーの参照
	 */
	void Terminate(Renderer& renderer);

private:
	/**
	 * @brief 定数バッファを実際に生成する内部処理
	 * @param renderer 3Dレンダラーの参照
	 * @return 成功すれば true
	 */
	bool initConstantBuffer(Renderer& renderer);
};
