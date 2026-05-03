/**
 * @file mesh.h
 * @brief メッシュクラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "material.h"

struct aiMesh;
struct Vertex;
class Renderer;

/**
 * @brief 単一メッシュのジオメトリデータと GPU バッファを管理するクラス
 *
 * 頂点バッファ・インデックスバッファ・マテリアルバッファを保持し、
 * ローカル変換行列を適用して描画する。
 */
class Mesh
{
public:
	/** @brief コンストラクタ */
	Mesh();

	/** @brief デストラクタ */
	~Mesh();

	/**
	 * @brief Assimp のメッシュデータから GPU バッファを構築する
	 * @param renderer   3Dレンダラーの参照
	 * @param pMeshData  Assimp のメッシュデータ
	 * @param mat        適用するマテリアルセット
	 * @return 成功すれば true
	 */
	bool Setup(Renderer& renderer, aiMesh* pMeshData, MaterialSet& mat);

	/** @brief GPU バッファを解放する */
	void Terminate();

	/** @brief メッシュを描画する */
	void Draw();

	/**
	 * @brief ローカル変換行列を設定する
	 * @param mtx ローカル変換行列
	 */
	void SetLocalTransform(const DirectX::XMMATRIX& mtx);

	/**
	 * @brief ローカル変換行列を取得する
	 * @return ローカル変換行列（XMFLOAT4X4）
	 */
	const DirectX::XMFLOAT4X4& GetLocalTransform() const { return m_localTransform; }

	/**
	 * @brief マテリアルを切り替える
	 * @param mat 新しいマテリアルセット
	 */
	void ChangeMaterial(MaterialSet& mat);

private:
	/** @brief 頂点バッファを生成する */
	bool createVertexBuffer();
	/** @brief インデックスバッファを生成する */
	bool createIndexBuffer();
	/** @brief 頂点バッファを解放する */
	void destroyVertexBuffer();
	/** @brief インデックスバッファを解放する */
	void destroyIndexBuffer();
	/** @brief マテリアル定数バッファを生成する */
	bool createMaterialBuffer();
	/** @brief マテリアルを定数バッファにセットする */
	void setMaterial();

	Renderer* m_pRenderer;  ///< レンダラーへのポインタ

	Vertex* m_vertices = nullptr;      ///< 頂点データ配列（解析用ワーク）
	unsigned int* m_indices = nullptr; ///< インデックスデータ配列（解析用ワーク）
	unsigned int m_vertexNum = 0;      ///< 頂点数
	unsigned int m_indexNum = 0;       ///< インデックス数

	ID3D11Buffer* m_vertexBuffer = nullptr; ///< 頂点バッファ
	ID3D11Buffer* m_indexBuffer = nullptr;  ///< インデックスバッファ

	DirectX::XMFLOAT4X4 m_localTransform;  ///< ノード階層から計算したローカル変換行列

	MaterialSet* m_pMaterialSet;  ///< 使用中のマテリアルセット
};
