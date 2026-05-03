/**
 * @file result.h
 * @brief リザルト表示アクタークラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "actor.h"

/**
 * @brief ゲームクリア・ゲームオーバー画面のリザルト表示を担うアクター
 */
class Result : public Actor
{
public:
	/** @brief コンストラクタ */
	Result();

	/** @brief デストラクタ */
	~Result();

	/** @brief 初期化処理 */
	void Setup() override;

	/** @brief 毎フレームの更新処理 */
	void Update() override;

	/** @brief 描画処理 */
	void Draw() override;

	/** @brief 終了処理・リソース解放 */
	void Terminate() override;
};
