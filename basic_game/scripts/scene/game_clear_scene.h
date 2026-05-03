/**
 * @file game_clear_scene.h
 * @brief ゲームクリアシーンクラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "scene.h"
#include "camera.h"
#include "stage_model.h"
#include "result.h"

/**
 * @brief ゲームクリア画面を管理するシーンクラス
 *
 * ステージ背景とリザルト UI を表示し、タイトルへの遷移入力を待つ。
 */
class GameClearScene : public Scene
{
public:
	/** @brief コンストラクタ */
	GameClearScene();

	/** @brief デストラクタ */
	~GameClearScene();

	/**
	 * @brief レンダラーを受け取ってシーンを初期化する
	 * @param renderer   3Dレンダラーの参照
	 * @param renderer2d 2Dレンダラーの参照
	 */
	void Initialize(Renderer& renderer, Renderer2D& renderer2d) override;

	/** @brief シーン開始時の初期化処理 */
	void Setup() override;

	/** @brief 毎フレームの更新処理 */
	void Update() override;

	/** @brief 描画処理 */
	void Draw() override;

	/** @brief 終了処理・リソース解放 */
	void Terminate() override;

private:
	Renderer*   m_pRenderer = nullptr;   ///< 3Dレンダラーへのポインタ
	Renderer2D* m_pRenderer2d = nullptr; ///< 2Dレンダラーへのポインタ
	Camera      m_camera;                ///< シーンカメラ
	StageModel  m_stage;                 ///< ステージ背景モデル
	Result      m_result;                ///< リザルト表示アクター
	int         m_bgm;                   ///< BGM のサウンドインデックス
};
