/**
 * @file model.h
 * @brief 3Dモデルの基底クラス定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "material.h"

class Renderer;
class Mesh;
class aiNode;
class aiMaterial;

/**
 * @brief Assimp で読み込んだ 3D モデルを管理する基底クラス
 *
 * ノード階層をトラバースしてメッシュを構築し、
 * ワールドトランスフォーム（位置・回転・スケール・ピボット）を提供する。
 * マテリアルの初期化はサブクラスが実装する。
 */
class Model
{
public:
	/** @brief コンストラクタ */
	Model();

	/** @brief デストラクタ */
	~Model();

	/**
	 * @brief モデルファイルを読み込んでメッシュを構築する
	 * @param renderer  3Dレンダラーの参照
	 * @param filePath  モデルファイルのパス
	 * @return 成功すれば true
	 */
	bool Setup(Renderer& renderer, const char* filePath);

	/** @brief メッシュリソースを解放する */
	void Terminate();

	/** @brief 全メッシュを描画する */
	void Draw();

	/**
	 * @brief Assimp のノード階層を再帰的に処理してメッシュを構築する
	 * @param node            処理対象のノード
	 * @param parentTransform 親ノードのワールド変換行列
	 */
	void ProcessNode(aiNode* node, const DirectX::XMMATRIX& parentTransform);

	/**
	 * @brief モデルのワールド座標を設定する
	 * @param pos 座標
	 */
	void SetPosition(const DirectX::XMFLOAT3& pos);

	/**
	 * @brief モデルの回転を設定する
	 * @param rot 回転クォータニオン
	 */
	void SetRotation(const DirectX::XMFLOAT4& rot);

	/**
	 * @brief モデルのスケールを設定する
	 * @param scale スケール値
	 */
	void SetScale(const DirectX::XMFLOAT3& scale);

	/**
	 * @brief 回転のピボット座標を設定する
	 * @param pivot ピボット座標
	 */
	void SetPivot(const DirectX::XMFLOAT3& pivot);

	/**
	 * @brief ピボットを中心とした追加回転を設定する
	 * @param rot 回転クォータニオン
	 */
	void SetPivotRotation(const DirectX::XMFLOAT4& rot);

	/** @brief マテリアルを全メッシュに再適用する */
	void ChangeMaterial();

private:
	/**
	 * @brief 位置・回転・スケール・ピボットを合成したモデル変換行列を返す
	 * @return ワールド変換行列
	 */
	DirectX::XMMATRIX getModelTransform() const;

	Mesh* m_meshes = nullptr;    ///< メッシュ配列
	unsigned int m_meshNum = 0;  ///< メッシュ数

	DirectX::XMFLOAT3 m_position{ 0,0,0 };   ///< ワールド座標
	DirectX::XMFLOAT4 m_rotation{ 0,0,0,0 }; ///< 回転クォータニオン
	DirectX::XMFLOAT3 m_scale{ 1,1,1 };       ///< スケール

	DirectX::XMFLOAT3 m_pivot = { 0,0,0 };         ///< ピボット座標
	DirectX::XMFLOAT4 m_pivotRotation = { 0,0,0,0 }; ///< ピボット回転

protected:
	/**
	 * @brief インデックス idx のメッシュにマテリアルを設定する（サブクラスで実装）
	 * @param idx メッシュインデックス
	 * @param mat Assimp のマテリアルデータ
	 */
	virtual void initializeMaterialSet(int idx, aiMaterial* mat) = 0;

	Renderer* m_pRenderer = nullptr;       ///< レンダラーへのポインタ
	MaterialSet m_materialSets[10];        ///< マテリアルセット配列（最大10）
};
