/**
 * @file box_model.h
 * @brief ボックス形状モデルクラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "model.h"

/**
 * @brief ボックス（直方体）形状の 3D モデルクラス
 *
 * 色・シェーダーの動的変更に対応し、敵キャラクターの胴体・腕などに使用する。
 */
class BoxModel : public Model
{
public:
	/** @brief コンストラクタ */
	BoxModel();

	/** @brief デストラクタ */
	~BoxModel();

	/**
	 * @brief マテリアルのディフューズカラーを変更する
	 * @param color 新しい色（RGBA）
	 */
	void ChangeColor(DirectX::XMFLOAT4 color);

	/**
	 * @brief 使用するシェーダーを切り替える
	 * @param shader 新しいシェーダーへのポインタ
	 */
	void ChangeShader(Shader* shader);

private:
	/** @brief マテリアルセットを初期値で設定する */
	void setupMaterialSet(aiMaterial* mat);

protected:
	/**
	 * @brief インデックス idx のマテリアルを初期化する
	 * @param idx メッシュインデックス
	 * @param mat Assimp のマテリアルデータ
	 */
	void initializeMaterialSet(int idx, aiMaterial* mat) override;
};
