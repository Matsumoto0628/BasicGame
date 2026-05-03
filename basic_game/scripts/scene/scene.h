/**
 * @file scene.h
 * @brief シーンの純粋仮想基底クラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

class Renderer;
class Renderer2D;

/**
 * @brief ゲームシーンの共通インターフェース
 *
 * タイトル・ゲーム・ゲームオーバー・ゲームクリアなど
 * 各シーンはこのクラスを継承して実装する。
 */
class Scene
{
public:
	virtual ~Scene() = default;

	/**
	 * @brief レンダラーを受け取ってシーンを初期化する
	 * @param renderer   3Dレンダラーの参照
	 * @param renderer2d 2Dレンダラーの参照
	 */
	virtual void Initialize(Renderer& renderer, Renderer2D& renderer2d) = 0;

	/** @brief シーン開始時の初期化処理 */
	virtual void Setup() = 0;

	/** @brief 毎フレームの更新処理 */
	virtual void Update() = 0;

	/** @brief 毎フレームの描画処理 */
	virtual void Draw() = 0;

	/** @brief 終了処理・リソース解放 */
	virtual void Terminate() = 0;
};
