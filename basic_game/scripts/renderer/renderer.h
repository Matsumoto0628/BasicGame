#pragma once
#include "triangle.h"
#include "shader.h"
#include "render_param.h"
#include "light.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();
    bool Initialize(HWND hWindow);
    void Terminate();
    void Draw();
    void Swap();
    bool CompileShader(const WCHAR* vsPath, const WCHAR* psPath, Shader& outShader);
    ID3D11Device* GetDevice() { return m_pD3DDevice; }
    ID3D11DeviceContext* GetDeviceContext() { return m_pImmediateContext; }
    RenderParam GetRenderParam() { return m_renderParam; }
	bool SetupViewTransform(const DirectX::XMMATRIX& viewMat);
    const LightSet& GetLightSet() const { return m_lightSet; }

public:
    Shader DefaultShader;

private:
    bool initDeviceAndSwapChain(HWND hWindow);
    bool initBackBuffer();
    bool setupProjectionTransform();
    bool createLightBuffer();
    void setLight();
	bool createSamplerState();

private:
    //! 機能レベルの配列
    static const UINT   FEATURE_LEVELS_NUM = 4;
    D3D_FEATURE_LEVEL m_pFeatureLevels[FEATURE_LEVELS_NUM] = {};
    //! デバイス作成時に返される機能レベル
    D3D_FEATURE_LEVEL m_featureLevelsSupported;

    //! デバイス
    ID3D11Device* m_pD3DDevice = nullptr;
    //! デバイスコンテキスト
    ID3D11DeviceContext* m_pImmediateContext = nullptr;
    //! スワップ・チェイン
    IDXGISwapChain* m_pSwapChain = nullptr;

    //! 描画ターゲット・ビュー
    ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
    //! ビューポート
    D3D11_VIEWPORT m_viewPort[1];

    UINT    m_backBufferNum = 3;
    UINT    m_screenWidth = 0;
    UINT    m_screenHeight = 0;

	ID3D11BlendState* m_pBlendState = nullptr;

	RenderParam m_renderParam;

    float m_nearClipDist = 0.f;
    float m_farClipDist = 0.f;
    float m_fov = 0.f;

    ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
    ID3D11DepthStencilState* m_pDepthState = nullptr;

    LightSet m_lightSet;

    ID3D11SamplerState* m_samplerState = nullptr;
};