/**
 * @file enemy.h
 * @brief 敵キャラクタークラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "actor.h"
#include "../model/box_model.h"
#include "collider.h"

class Player;

/**
 * @brief 敵の移動パターンを表す列挙型
 */
enum MoveType
{
	Curve,  ///< 曲線的な移動
	Basic,  ///< 直線的な基本移動
};

/**
 * @brief 敵キャラクタークラス
 *
 * プレイヤーを追跡し、攻撃・ノックバック・死亡などの状態を管理する。
 * 移動パターンは MoveType で切り替え可能。
 */
class Enemy : public Actor
{
public:
	/** @brief コンストラクタ */
	Enemy();

	/** @brief デストラクタ */
	~Enemy();

	/**
	 * @brief レンダラーとプレイヤーポインタを受け取って初期化する
	 * @param renderer 3Dレンダラーの参照
	 * @param pPlayer プレイヤーへのポインタ
	 */
	void Initialize(Renderer &renderer, Player* pPlayer);

	/** @brief シーン開始時の初期化処理 */
	void Setup() override;

	/** @brief 毎フレームの更新処理 */
	void Update() override;

	/** @brief 描画処理 */
	void Draw() override;

	/** @brief 終了処理・リソース解放 */
	void Terminate() override;

	/**
	 * @brief 当たり判定コライダーを取得する
	 * @return コライダーへの定数参照
	 */
	const Collider& GetCollider() const { return m_collider; }

	/**
	 * @brief ダメージを受ける
	 * @param amount ダメージ量
	 */
	void TakeDamage(int amount);

	/**
	 * @brief ワールド座標を設定する
	 * @param pos 設定する座標
	 */
	void SetPosition(const DirectX::XMFLOAT3& pos) { m_position = pos; }

	/**
	 * @brief 移動パターンを設定する
	 * @param type 移動タイプ
	 */
	void SetMoveType(MoveType type) { m_moveType = type; }

	/** @brief 攻撃アクションを開始する */
	void Attack();

	/**
	 * @brief 死亡しているか判定する
	 * @return 死亡していれば true
	 */
	bool IsDead() const { return m_isDead; }

	/** @brief 進行中の攻撃をキャンセルする */
	void CancelAttack();

private:
	/** @brief 移動処理 */
	void move();
	/** @brief プレイヤーへの向き計算 */
	void look();
	/** @brief 移動軸ベクトルを更新する */
	void calcMoveAxis();
	/** @brief ボディのワールド座標を更新する */
	void calcBoxPos();
	/** @brief ボディの回転を更新する */
	void calcBoxRot();
	/**
	 * @brief 二つの角度を球面線形補間する
	 * @param a 開始角度（ラジアン）
	 * @param b 終了角度（ラジアン）
	 * @param t 補間係数 [0, 1]
	 * @return 補間後の角度
	 */
	float lerpAngle(float a, float b, float t);
	/** @brief ノックバック処理 */
	void knockback();
	/** @brief カーブ移動時の向き計算 */
	void curveLook();
	/** @brief 直線移動時の向き計算 */
	void basicLook();
	/** @brief ランダムに目標座標を設定する */
	void setTargetPosRandom();

	BoxModel m_boxBody;        ///< 胴体ボックスモデル
	BoxModel m_boxRight;       ///< 右腕ボックスモデル
	BoxModel m_boxLeft;        ///< 左腕ボックスモデル
	Player* m_pPlayer = nullptr;      ///< プレイヤーへのポインタ
	Collider m_collider;       ///< 当たり判定コライダー
	Renderer* m_pRenderer = nullptr;  ///< レンダラーへのポインタ

	DirectX::XMFLOAT3 m_position = { 0, 0, -5.f };  ///< ワールド座標
	DirectX::XMFLOAT4 m_rotation = { 0, 0, 0, 0 };  ///< 回転クォータニオン

	DirectX::XMFLOAT3 m_forward = { 0, 0, 1 };  ///< 前方ベクトル
	DirectX::XMFLOAT3 m_right = { 1, 0, 0 };    ///< 右方向ベクトル

	float m_yaw;   ///< ヨー角（Y軸回転）
	float m_pitch; ///< ピッチ角（X軸回転）

	int m_health = 30;  ///< 残りHP

	bool m_isHit = false;           ///< ヒット中フラグ
	bool m_isKnockback = false;     ///< ノックバック中フラグ
	float m_hitTimer = 0.f;         ///< ヒット継続タイマー
	float m_knockbackTimer = 0.f;   ///< ノックバック継続タイマー
	static constexpr float HIT_DURATION = 0.25f;       ///< ヒット演出の継続時間（秒）
	static constexpr float KNOCKBACK_DURATION = 0.5f;  ///< ノックバックの継続時間（秒）

	float m_wingTimer = 0.f;  ///< 翼アニメーション用タイマー
	float m_waveTimer = 0.f;  ///< 波形アニメーション用タイマー

	MoveType m_moveType = MoveType::Basic;  ///< 現在の移動パターン

	float m_sideYawOffset = 0.f;   ///< 横方向のヨーオフセット
	float m_dirChangeTimer = 0.f;  ///< 方向転換タイマー

	DirectX::XMFLOAT3 m_targetPos{ 0,0,0 };  ///< 目標座標

	float m_targetTimer = 0.f;     ///< 目標座標タイマー
	float m_targetDuration = 3.f;  ///< 目標座標の保持時間（秒）

	bool m_isAttack = false;      ///< 攻撃中フラグ
	float m_attackTimer = 0.f;    ///< 攻撃継続タイマー

	bool m_isDead = false;        ///< 死亡フラグ
	float m_deadTimer = 0.f;      ///< 死亡演出タイマー
};
