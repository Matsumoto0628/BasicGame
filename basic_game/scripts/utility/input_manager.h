/**
 * @file input_manager.h
 * @brief キーボード・マウス入力管理クラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

/** @brief 管理するキーコードの最大数 */
#define KEY_MAX 256

/**
 * @brief キーボードとマウスの入力状態を管理するシングルトンクラス
 *
 * 毎フレーム Update() を呼ぶことで押下・離し・押し続けの状態を更新する。
 * マウスカーソルのロック機能も提供する。
 */
class InputManager
{
public:
	/**
	 * @brief シングルトンインスタンスを取得する
	 * @return InputManager の唯一のインスタンス
	 */
	static InputManager& Instance();

	/** @brief コンストラクタ */
	InputManager();

	/** @brief デストラクタ */
	~InputManager();

	/**
	 * @brief キーが押下された瞬間かどうかを返す
	 * @param key 仮想キーコード
	 * @return 押下された瞬間であれば true
	 */
	bool GetKeyDown(int key);

	/**
	 * @brief キーが離された瞬間かどうかを返す
	 * @param key 仮想キーコード
	 * @return 離された瞬間であれば true
	 */
	bool GetKeyUp(int key);

	/**
	 * @brief キーが押し続けられているかどうかを返す
	 * @param key 仮想キーコード
	 * @return 押下中であれば true
	 */
	bool GetKey(int key);

	/** @brief 毎フレームの入力状態を更新する */
	void Update();

	/**
	 * @brief 前フレームからのマウス移動量を取得する
	 * @return マウスの移動量（POINT 型）
	 */
	POINT GetMouseDelta() const { return m_mouseDelta; }

	/**
	 * @brief ウィンドウハンドルを設定する（マウスロック時に使用）
	 * @param hwnd ウィンドウハンドル
	 */
	void SetHwnd(HWND hwnd) { m_hwnd = hwnd; }

	/**
	 * @brief マウスカーソルのロック状態を切り替える
	 * @param lock true でロック（カーソルを中心に固定）、false で解除
	 */
	void SetCursorLock(bool lock);

private:
	bool m_prevKeys[KEY_MAX];     ///< 前フレームのキー状態
	bool m_currentKeys[KEY_MAX];  ///< 現フレームのキー状態
	POINT m_mouseDelta{};         ///< 前フレームからのマウス移動量
	HWND m_hwnd;                  ///< ウィンドウハンドル
	bool m_mouseLoocked = false;  ///< マウスロック中フラグ
};
