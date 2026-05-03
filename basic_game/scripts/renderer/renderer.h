/**
 * @file renderer.h
 * @brief Direct3D 11 レンダラークラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "triangle.h"
#include "shader.h"
#include "render_param.h"
#include "light.h"
#include "renderer_2d.h"

/**
 * @brief Direct3D 11 デバイスとスワップチェーンを管理するレンダラークラス
 *
 * デバイスの初期化・シェーダーコンパイル・ビュー変換設定・ライト管理を行い、
 * 各メッシュが参照する RenderParam を提供する。
 */
class Renderer
{
public:
	/** @brief コンストラクタ */
	Renderer();

	/** @brief デストラクタ */
	~Renderer();

	/**
	 * @brief Direct3D デバイス・スワップチェーン・バックバッファを初期化する
	 * @param hWindow 描画先ウィンドウハンドル
	 * @return 成功すれば true
	 */
	bool Initialize(HWND hWindow);

	/** @brief 全 Direct3D リソースを解放する */
	void Terminate();

	/** @brief バックバッファをクリアして描画を開始する */
	void Draw();

	/** @brief バックバッファを画面に表示する */
	void Swap();

	/**
	 * @brief 頂点シェーダーとピクセルシェーダーをコンパイルする
	 * @param vsPath    頂点シェーダーファイルのパス
	 * @param psPath    ピクセルシェーダーファイルのパス
	 * @param outShader コンパイル結果を受け取る Shader 構造体
	 * @return 成功すれば true
	 */
	bool CompileShader(const WCHAR* vsPath, const WCHAR* psPath, Shader& outShader);

	/**
	 * @brief D3D デバイスポインタを取得する
	 * @return ID3D11Device ポインタ
	 */
	ID3D11Device* GetDevice() { return m_pD3DDevice; }

	/**
	 * @brief D3D デバイスコンテキストポインタを取得する
	 * @return ID3D11DeviceContext ポインタ
	 */
	ID3D11DeviceContext* GetDeviceContext() { return m_pImmediateContext; }

	/**
	 * @brief 定数バッファセットを取得する
	 * @return RenderParam の値コピー
	 */
	RenderParam GetRenderParam() { return m_renderParam; }

	/**
	 * @brief ビュー変換行列を定数バッファに設定する
	 * @param viewMat ビュー変換行列
	 * @return 成功すれば true
	 */
	bool SetupViewTransform(const DirectX::XMMATRIX& viewMat);

	/**
	 * @brief ライトセットを取得する
	 * @return LightSet への定数参照
	 */
	const LightSet& GetLightSet() const { return m_lightSet; }

	/**
	 * @brief ライトの視点座標を更新する
	 * @param eyePos 視点座標（XMFLOAT4）
	 */
	void SetEyePosLight(DirectX::XMFLOAT4 eyePos);

	/**
	 * @brief スワップチェーンポインタを取得する
	 * @return IDXGISwapChain ポインタ
	 */
	IDXGISwapChain* GetSwapChain() { return m_pSwapChain; }

	Shader TextureSpecularShader;  ///< テクスチャ＋スペキュラシェーダー
	Shader TextureShader;          ///< テクスチャのみシェーダー
	Shader SimpleShader;           ///< 頂点カラーのみシェーダー
	Shader SpecularShader;         ///< スペキュラのみシェーダー

private:
	/** @brief D3D デバイスとスワップチェーンを生成する */
	bool initDeviceAndSwapChain(HWND hWindow);
	/** @brief バックバッファのレンダーターゲットビューを生成する */
	bool initBackBuffer();
	/** @brief プロジェクション変換を設定する */
	bool setupProjectionTransform();
	/** @brief ライト定数バッファを生成する */
	bool createLightBuffer();
	/** @brief ライトデータを定数バッファに転送する */
	void setLight();
	/** @brief サンプラーステートを生成する */
	bool createSamplerState();
	/** @brief 全ビルトインシェーダーをコンパイルする */
	void compileShaders();

	static const UINT FEATURE_LEVELS_NUM = 4;                         ///< 試行するフィーチャーレベルの数
	D3D_FEATURE_LEVEL m_pFeatureLevels[FEATURE_LEVELS_NUM] = {};      ///< 試行するフィーチャーレベル配列
	D3D_FEATURE_LEVEL m_featureLevelsSupported;                       ///< 実際にサポートされたフィーチャーレベル

	ID3D11Device*        m_pD3DDevice = nullptr;        ///< D3D デバイス
	ID3D11DeviceContext* m_pImmediateContext = nullptr; ///< D3D デバイスコンテキスト
	IDXGISwapChain*      m_pSwapChain = nullptr;        ///< スワップチェーン

	ID3D11RenderTargetView* m_pRenderTargetView = nullptr; ///< バックバッファのレンダーターゲットビュー
	D3D11_VIEWPORT m_viewPort[1];                          ///< ビューポート設定

	UINT m_backBufferNum = 3;   ///< バックバッファ枚数
	UINT m_screenWidth = 0;     ///< スクリーン幅（ピクセル）
	UINT m_screenHeight = 0;    ///< スクリーン高さ（ピクセル）

	ID3D11BlendState* m_pBlendState = nullptr;  ///< ブレンドステート

	RenderParam m_renderParam;  ///< 定数バッファセット

	float m_nearClipDist = 0.f;  ///< ニアクリップ距離
	float m_farClipDist = 0.f;   ///< ファークリップ距離
	float m_fov = 0.f;           ///< 視野角（ラジアン）

	ID3D11DepthStencilView*  m_pDepthStencilView = nullptr; ///< 深度ステンシルビュー
	ID3D11DepthStencilState* m_pDepthState = nullptr;       ///< 深度ステンシルステート

	LightSet m_lightSet;  ///< ライトデータセット

	ID3D11SamplerState* m_samplerState = nullptr;  ///< テクスチャサンプラーステート
};
