/**
 * @file shader.h
 * @brief シェーダーリソースをまとめた構造体の定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

/**
 * @brief 頂点シェーダー・ピクセルシェーダー・入力レイアウトを保持する構造体
 *
 * Renderer::CompileShader で生成され、MaterialSet から参照される。
 * デストラクタで全 COM オブジェクトを自動解放する。
 */
struct Shader
{
	/** @brief コンストラクタ */
	Shader() {}

	/** @brief デストラクタ（全 COM リソースを解放する） */
	~Shader() {
		DX_SAFE_RELEASE(pVertexShader);
		DX_SAFE_RELEASE(pPixelShader);
		DX_SAFE_RELEASE(pInputLayout);
	}

	ID3D11VertexShader* pVertexShader = nullptr;  ///< 頂点シェーダー
	ID3D11PixelShader*  pPixelShader = nullptr;   ///< ピクセルシェーダー
	ID3D11InputLayout*  pInputLayout = nullptr;   ///< 入力レイアウト
};
