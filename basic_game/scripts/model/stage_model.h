/**
 * @file stage_model.h
 * @brief ステージ用 3D モデルクラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "model.h"

/**
 * @brief ゲームステージ（背景・地形）の 3D モデルクラス
 *
 * A・B・C の 3 種類のマテリアルセットに分けてステージの各部位を表現する。
 */
class StageModel : public Model
{
public:
	/** @brief コンストラクタ */
	StageModel();

	/** @brief デストラクタ */
	~StageModel();

private:
	/** @brief マテリアルセット A（地面など）を設定する */
	void setupMaterialSetA(aiMaterial* mat);
	/** @brief マテリアルセット B（壁など）を設定する */
	void setupMaterialSetB(aiMaterial* mat);
	/** @brief マテリアルセット C（装飾など）を設定する */
	void setupMaterialSetC(aiMaterial* mat);

protected:
	/**
	 * @brief インデックス idx のマテリアルを初期化する
	 * @param idx メッシュインデックス
	 * @param mat Assimp のマテリアルデータ
	 */
	void initializeMaterialSet(int idx, aiMaterial* mat) override;
};
