/**
 * @file weapon_model.h
 * @brief 武器用 3D モデルクラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "model.h"

/**
 * @brief 武器の 3D モデルクラス
 *
 * A・B・C の 3 種類のマテリアルセットで武器の各部位を表現する。
 */
class WeaponModel : public Model
{
public:
	/** @brief コンストラクタ */
	WeaponModel();

	/** @brief デストラクタ */
	~WeaponModel();

private:
	/** @brief マテリアルセット A（刃部分など）を設定する */
	void setupMaterialSetA(aiMaterial* mat);
	/** @brief マテリアルセット B（柄部分など）を設定する */
	void setupMaterialSetB(aiMaterial* mat);
	/** @brief マテリアルセット C（装飾部分など）を設定する */
	void setupMaterialSetC(aiMaterial* mat);

protected:
	/**
	 * @brief インデックス idx のマテリアルを初期化する
	 * @param idx メッシュインデックス
	 * @param mat Assimp のマテリアルデータ
	 */
	void initializeMaterialSet(int idx, aiMaterial* mat) override;
};
