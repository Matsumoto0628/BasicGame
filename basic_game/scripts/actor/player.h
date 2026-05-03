/**
 * @file player.h
 * @brief プレイヤークラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "actor.h"
#include "collider.h"

class Camera;
class Weapon;
class Renderer;
class Renderer2D;

/**
 * @brief プレイヤーキャラクタークラス
 *
 * カメラ・武器・コライダーを持ち、入力に応じた移動・視点操作・ノックバックを管理する。
 */
class Player : public Actor
{
public:
	/** @brief コンストラクタ */
	Player();

	/** @brief デストラクタ */
	~Player();

	/**
	 * @brief 必要な依存オブジェクトを受け取って初期化する
	 * @param renderer    3Dレンダラーの参照
	 * @param renderer2d  2Dレンダラーの参照
	 * @param pCamera     カメラへのポインタ
	 * @param pWeapon     武器へのポインタ
	 */
	void Initialize(Renderer& renderer, Renderer2D& renderer2d, Camera* pCamera, Weapon* pWeapon);

	/** @brief シーン開始時の初期化処理 */
	void Setup() override;

	/** @brief 毎フレームの更新処理 */
	void Update() override;

	/** @brief 描画処理 */
	void Draw() override;

	/** @brief 終了処理・リソース解放 */
	void Terminate() override;

	/**
	 * @brief ワールド座標を取得する
	 * @return 現在の座標
	 */
	DirectX::XMFLOAT3 GetPosition() { return m_position; }

	/**
	 * @brief ダメージを受ける
	 * @param amount ダメージ量
	 */
	void TakeDamage(int amount);

	/**
	 * @brief 当たり判定コライダーを取得する
	 * @return コライダーへの定数参照
	 */
	const Collider& GetCollider() const { return m_collider; }

private:
	/** @brief 移動処理 */
	void move();
	/** @brief マウス入力による視点操作 */
	void look();
	/** @brief 移動軸ベクトルを更新する */
	void calcMoveAxis();
	/** @brief 武器のワールド座標を更新する */
	void calcWeaponPos();
	/** @brief 武器の回転を更新する */
	void calcWeaponRot();
	/** @brief ノックバック処理 */
	void knockback();

	Camera* m_pCamera = nullptr;      ///< カメラへのポインタ
	Weapon* m_pWeapon = nullptr;      ///< 武器へのポインタ
	Collider m_collider;              ///< 当たり判定コライダー
	Renderer2D* m_pRenderer2d = nullptr;  ///< 2Dレンダラーへのポインタ

	DirectX::XMFLOAT3 m_position = { 0, 0, -5.f };  ///< ワールド座標
	DirectX::XMFLOAT4 m_rotation = { 0, 0, 0, 0 };  ///< 回転クォータニオン

	DirectX::XMFLOAT3 m_forward = { 0, 0, 1 };  ///< 前方ベクトル
	DirectX::XMFLOAT3 m_right = { 1, 0, 0 };    ///< 右方向ベクトル

	float m_yaw;   ///< ヨー角（Y軸回転）
	float m_pitch; ///< ピッチ角（X軸回転）

	float m_health = 50;  ///< 残りHP

	bool m_isKnockback = false;    ///< ノックバック中フラグ
	float m_knockbackTimer = 0.f;  ///< ノックバック継続タイマー
	static constexpr float KNOCKBACK_DURATION = 0.25f;  ///< ノックバックの継続時間（秒）

	bool m_isHit = false;      ///< ヒット中フラグ
	float m_hitTimer = 0.f;    ///< ヒット演出タイマー
	static constexpr float HIT_DURATION = 2.f;  ///< ヒット演出の継続時間（秒）
};
