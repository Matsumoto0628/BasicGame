/**
 * @file title.h
 * @brief タイトル画面アクタークラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "actor.h"

/**
 * @brief タイトル画面の表示・入力待ちを担うアクター
 */
class Title : public Actor
{
public:
	/** @brief コンストラクタ */
	Title();

	/** @brief デストラクタ */
	~Title();

	/** @brief 初期化処理 */
	void Setup() override;

	/** @brief 毎フレームの更新処理 */
	void Update() override;

	/** @brief 描画処理 */
	void Draw() override;

	/** @brief 終了処理・リソース解放 */
	void Terminate() override;
};
