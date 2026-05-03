/**
 * @file actor.h
 * @brief アクターの基底クラス定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

/**
 * @brief ゲーム内アクター（プレイヤー・敵など）の純粋仮想基底クラス
 *
 * すべてのアクターはこのクラスを継承し、各ライフサイクルメソッドを実装する。
 */
class Actor
{
public:
	virtual ~Actor() = default;

	/** @brief 初期化処理（シーン開始時に呼ぶ） */
	virtual void Setup() = 0;

	/** @brief 毎フレームの更新処理 */
	virtual void Update() = 0;

	/** @brief 毎フレームの描画処理 */
	virtual void Draw() = 0;

	/** @brief 終了処理・リソース解放 */
	virtual void Terminate() = 0;
};
