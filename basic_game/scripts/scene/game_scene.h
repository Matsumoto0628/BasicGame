/**
 * @file game_scene.h
 * @brief ゲームメインシーンクラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "scene.h"
#include "camera.h"
#include "stage_model.h"
#include "weapon.h"
#include "player.h"
#include "enemy.h"

/**
 * @brief ゲームのメインプレイを管理するシーンクラス
 *
 * プレイヤー・武器・複数の敵・ステージを保持し、
 * 当たり判定・スコア・シーン遷移を制御する。
 */
class GameScene : public Scene
{
public:
	/** @brief コンストラクタ */
	GameScene();

	/** @brief デストラクタ */
	~GameScene();

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
	Weapon      m_weapon;                ///< プレイヤーの武器

	Player m_player;  ///< プレイヤーキャラクター

	static constexpr int ENEMY_MAX = 12;  ///< 同時に存在できる敵の最大数
	Enemy m_enemies[ENEMY_MAX];           ///< 敵キャラクター配列

	int m_bgm;    ///< BGM のサウンドインデックス
	int m_slash;  ///< スラッシュ SE のサウンドインデックス
	int m_hit;    ///< ヒット SE のサウンドインデックス
};
