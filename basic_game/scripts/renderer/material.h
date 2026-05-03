/**
 * @file material.h
 * @brief マテリアル用データ構造体の定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

struct Shader;

/**
 * @brief シェーダーに渡すマテリアルパラメータ（拡散色・鏡面色・光沢度）
 */
struct Material
{
	DirectX::XMFLOAT4 Diffuse;  ///< 拡散反射色（RGBA）
	DirectX::XMFLOAT4 Specular; ///< 鏡面反射色（RGBA）
	float Shininess;             ///< 光沢係数
	float padding[3];            ///< 16バイトアライメント用パディング
};

/**
 * @brief マテリアルのデータ・テクスチャ・シェーダー・GPU バッファをまとめた構造体
 */
struct MaterialSet
{
	Material Data;                              ///< マテリアルパラメータ
	ID3D11ShaderResourceView* DiffuseTex = nullptr; ///< ディフューズテクスチャ
	Shader* pShader;                            ///< 使用するシェーダー
	ID3D11Buffer* pBuffer = nullptr;            ///< GPU 定数バッファ
};
