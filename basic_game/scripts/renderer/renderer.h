#pragma once

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Initialize(HWND hWindow);

    void Terminate();

    void Draw();

    void Swap();

    ID3D11Device* GetDevice() { return m_pD3DDevice; }
    ID3D11DeviceContext* GetDeviceContext() { return m_pImmediateContext; }

private:
    bool initDeviceAndSwapChain(HWND hWindow);
    bool initBackBuffer();

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

};