/**
 * @file camera.h
 * @brief 3D カメラクラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

/**
 * @brief ビュー行列と方向ベクトルを管理するカメラクラス
 *
 * 位置と回転（ヨー・ピッチ）からビュー行列と前方・右・上ベクトルを計算する。
 */
class Camera
{
public:
	/** @brief コンストラクタ */
	Camera();

	/** @brief デストラクタ */
	~Camera();

	/** @brief 毎フレームの軸ベクトルを更新する */
	void Update();

	/**
	 * @brief ビュー変換行列を取得する
	 * @return ビュー行列
	 */
	DirectX::XMMATRIX GetViewMatrix() const;

	/**
	 * @brief 視点座標を XMFLOAT4 形式で取得する
	 * @return 視点座標（w=1）
	 */
	DirectX::XMFLOAT4 GetEyePos() const;

	/**
	 * @brief カメラのワールド座標を設定する
	 * @param pos 座標
	 */
	void SetPosition(const DirectX::XMFLOAT3& pos);

	/**
	 * @brief カメラの回転を設定する（pitch, yaw, roll の順）
	 * @param rot 回転クォータニオン
	 */
	void SetRotation(const DirectX::XMFLOAT4& rot);

	/** @brief 前方ベクトルを取得する */
	DirectX::XMFLOAT3 GetForward() const { return m_forward; }

	/** @brief 右方向ベクトルを取得する */
	DirectX::XMFLOAT3 GetRight() const { return m_right; }

	/** @brief 上方向ベクトルを取得する */
	DirectX::XMFLOAT3 GetUp() const { return m_up; }

	/** @brief 現在の回転クォータニオンを取得する */
	DirectX::XMFLOAT4 GetRotation() const { return m_rotation; }

private:
	/** @brief 回転から前方・右・上ベクトルを再計算する */
	void calcAxis();

	DirectX::XMFLOAT3 m_position = { 0, 0, -5.f };  ///< ワールド座標
	DirectX::XMFLOAT4 m_rotation = { 0, 0, 0, 0 };  ///< 回転クォータニオン

	DirectX::XMFLOAT3 m_forward = { 0, 0, 1 };  ///< 前方ベクトル
	DirectX::XMFLOAT3 m_up = { 0, 1, 0 };       ///< 上方向ベクトル
	DirectX::XMFLOAT3 m_right = { 1, 0, 0 };    ///< 右方向ベクトル
};
