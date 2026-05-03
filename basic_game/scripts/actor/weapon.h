/**
 * @file weapon.h
 * @brief 武器アクタークラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "actor.h"
#include "../model/weapon_model.h"
#include "collider.h"

class Renderer;

/**
 * @brief プレイヤーが装備する武器を管理するアクター
 *
 * スラッシュアニメーション・コライダーの有効化・トランスフォーム設定を行う。
 */
class Weapon : public Actor
{
public:
	/** @brief コンストラクタ */
	Weapon();

	/** @brief デストラクタ */
	~Weapon();

	/**
	 * @brief レンダラーを受け取って初期化する
	 * @param renderer 3Dレンダラーの参照
	 */
	void Initialize(Renderer& renderer);

	/** @brief シーン開始時の初期化処理 */
	void Setup() override;

	/** @brief 毎フレームの更新処理（スラッシュアニメーション進行） */
	void Update() override;

	/** @brief 描画処理 */
	void Draw() override;

	/** @brief 終了処理・リソース解放 */
	void Terminate() override;

	/** @brief スラッシュアニメーションを開始する */
	void Slash();

	/**
	 * @brief 武器のワールド座標を設定する
	 * @param pos 設定する座標
	 */
	void SetPosition(const DirectX::XMFLOAT3& pos);

	/**
	 * @brief 武器の回転を設定する
	 * @param rot 回転クォータニオン
	 */
	void SetRotation(const DirectX::XMFLOAT4& rot);

	/**
	 * @brief 武器のスケールを設定する
	 * @param scale スケール値
	 */
	void SetScale(const DirectX::XMFLOAT3& scale);

	/**
	 * @brief 回転のピボット座標を設定する
	 * @param pivot ピボット座標
	 */
	void SetPivot(const DirectX::XMFLOAT3& pivot);

	/**
	 * @brief ピボットを中心とした回転を設定する
	 * @param rot 回転クォータニオン
	 */
	void SetPivotRotation(const DirectX::XMFLOAT4& rot);

	/**
	 * @brief 当たり判定コライダーを取得する
	 * @return コライダーへの定数参照
	 */
	const Collider& const GetCollider() { return m_collider; }

	/**
	 * @brief 攻撃判定の基準座標を設定する
	 * @param pos 攻撃判定座標
	 */
	void SetAttackPos(const DirectX::XMFLOAT3& pos);

private:
	bool m_isAnimation = false;                           ///< スラッシュアニメーション再生中フラグ
	float m_animationTime = 0.f;                          ///< アニメーション経過時間
	static constexpr float ANIMATION_DURATION = 0.5f;    ///< スラッシュアニメーションの総時間（秒）

	WeaponModel m_weaponModel;              ///< 武器の3Dモデル
	Collider m_collider;                    ///< 攻撃判定コライダー
	Renderer* m_pRenderer = nullptr;        ///< レンダラーへのポインタ

	DirectX::XMFLOAT3 m_attackPos{ 0,0,0 };  ///< 攻撃判定の基準座標
};
