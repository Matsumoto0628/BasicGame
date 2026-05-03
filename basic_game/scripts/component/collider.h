/**
 * @file collider.h
 * @brief 球体コライダーコンポーネントの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

class Sphere;
class Renderer;

/**
 * @brief 球体ベースの当たり判定コンポーネント
 *
 * 中心座標と半径で球体を定義し、他のコライダーとの交差判定を行う。
 * デバッグ描画にも対応する。
 */
class Collider
{
public:
	/** @brief コンストラクタ */
	Collider();

	/** @brief デストラクタ */
	~Collider();

	/**
	 * @brief 別のコライダーと交差しているか判定する
	 * @param other 比較対象のコライダー
	 * @return 交差していれば true
	 */
	bool Intersects(const Collider& other) const;

	/**
	 * @brief コライダーの中心座標を設定する
	 * @param pos ワールド座標
	 */
	void SetPosition(const DirectX::XMFLOAT3& pos) { m_position = pos; }

	/**
	 * @brief コライダーの半径を設定する
	 * @param r 半径
	 */
	void SetRadius(float r) { m_radius = r; }

	/**
	 * @brief レンダラーを受け取ってデバッグ描画用球体を初期化する
	 * @param renderer 3Dレンダラーの参照
	 */
	void Initialize(Renderer& renderer);

	/** @brief デバッグ用の球体を描画する */
	void Draw();

	/** @brief 毎フレームの状態更新 */
	void Update();

	/**
	 * @brief コライダーの有効・無効を切り替える
	 * @param isActive true で有効化、false で無効化
	 */
	void SetActive(bool isActive) { m_isActive = isActive; }

	/**
	 * @brief コライダーが有効かどうかを返す
	 * @return 有効であれば true
	 */
	const bool GetActive() const { return m_isActive; }

private:
	DirectX::XMFLOAT3 m_position{ 0,0,0 };  ///< コライダーの中心座標
	float m_radius = 1.0f;                   ///< コライダーの半径
	Sphere* m_pSphere = nullptr;             ///< デバッグ描画用球体
	Renderer* m_pRenderer = nullptr;         ///< レンダラーへのポインタ
	bool m_isActive = false;                 ///< 有効フラグ
};
