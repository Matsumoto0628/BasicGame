#include "renderer.h"

Renderer::Renderer()
{
    m_pFeatureLevels[0] = D3D_FEATURE_LEVEL_11_1;
    m_pFeatureLevels[1] = D3D_FEATURE_LEVEL_11_0;
    m_pFeatureLevels[2] = D3D_FEATURE_LEVEL_10_1;
    m_pFeatureLevels[3] = D3D_FEATURE_LEVEL_10_0;
}

Renderer::~Renderer()
{

}

bool Renderer::Initialize(HWND hWindow)
{
    // Windowに合わせてスクリーンサイズ初期化
    RECT rc;
    GetClientRect(hWindow, &rc);
    m_screenWidth = rc.right - rc.left;
    m_screenHeight = rc.bottom - rc.top;

    initDeviceAndSwapChain(hWindow);

    initBackBuffer();

    return true;
}

bool Renderer::initDeviceAndSwapChain(HWND hWindow)
{
    // デバイスとスワップ・チェイン作成
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = m_backBufferNum;        // バックバッファの数
    sd.BufferDesc.Width = m_screenWidth;    // バックバッファの幅
    sd.BufferDesc.Height = m_screenHeight;    // バックバッファの高さ
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // フォーマット
    sd.BufferDesc.RefreshRate.Numerator = 60;    // リフレッシュレート（分母）
    sd.BufferDesc.RefreshRate.Denominator = 1;    // リフレッシュレート（分子）
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // バックバッファの使用法
    sd.OutputWindow = hWindow;        // 関連付けるウィンドウ
    sd.SampleDesc.Count = 1;            // マルチサンプル（アンチエイリアス）の数
    sd.SampleDesc.Quality = 0;            // マルチサンプル（アンチエイリアス）のクオリティ
    sd.Windowed = TRUE;        // ウィンドウモード（TRUEがウィンドウモード）
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;        // モード自動切り替え

#if defined(DEBUG) || defined(_DEBUG)
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
    UINT createDeviceFlags = 0;
#endif

    const D3D_DRIVER_TYPE DriverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    HRESULT hr;
    for (auto type : DriverTypes) {
        // ハードウェアデバイスを作成
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr, type, nullptr, createDeviceFlags,
            m_pFeatureLevels, FEATURE_LEVELS_NUM, D3D11_SDK_VERSION, &sd,
            &m_pSwapChain, &m_pD3DDevice, &m_featureLevelsSupported, &m_pImmediateContext);
        if (SUCCEEDED(hr)) {
            break;
        }
    }
    if (FAILED(hr)) return false;

    return true;
}

void Renderer::Draw()
{
    if (!m_pImmediateContext || !m_pRenderTargetView) return;

    // 青でクリア
    float color[] = { 0.f, 0.f, 1.f, 0.f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);
}

void Renderer::Swap()
{
    // バックバッファの表示（画面をすぐに更新）
    HRESULT hr = m_pSwapChain->Present(0, 0);
    if (FAILED(hr)) {
        //TRACE("Swap failed(%0x08x)n", hr);
        return;
    }
}

bool Renderer::initBackBuffer()
{
    HRESULT hr;

    // スワップ・チェインから最初のバック・バッファを取得する
    ID3D11Texture2D* pBackBuffer;  // バッファのアクセスに使うインターフェイス
    hr = m_pSwapChain->GetBuffer(
        0,                         // バック・バッファの番号
        __uuidof(ID3D11Texture2D), // バッファにアクセスするインターフェイス
        (LPVOID*)&pBackBuffer);    // バッファを受け取る変数
    if (FAILED(hr)) {
        //TRACE("InitBackBuffer g_pSwapChain->GetBuffer(%0x08x)n", hr);  // 失敗
        return false;
    }

    // バック・バッファの情報
    D3D11_TEXTURE2D_DESC descBackBuffer;
    pBackBuffer->GetDesc(&descBackBuffer);

    // バック・バッファの描画ターゲット・ビューを作る
    hr = m_pD3DDevice->CreateRenderTargetView(
        pBackBuffer,           // ビューでアクセスするリソース
        nullptr,               // 描画ターゲット・ビューの定義
        &m_pRenderTargetView); // 描画ターゲット・ビューを受け取る変数
    DX_SAFE_RELEASE(pBackBuffer);  // 以降、バック・バッファは直接使わないので解放
    if (FAILED(hr)) {
        //TRACE("InitBackBuffer g_pD3DDevice->CreateRenderTargetView(%0x08x)n", hr);  // 失敗
        return false;
    }

    // ビューポートの設定
    m_viewPort[0].TopLeftX = 0.0f;    // ビューポート領域の左上X座標。
    m_viewPort[0].TopLeftY = 0.0f;    // ビューポート領域の左上Y座標。
    m_viewPort[0].Width = static_cast<float>(m_screenWidth);  // ビューポート領域の幅
    m_viewPort[0].Height = static_cast<float>(m_screenHeight);  // ビューポート領域の高さ
    m_viewPort[0].MinDepth = 0.0f; // ビューポート領域の深度値の最小値
    m_viewPort[0].MaxDepth = 1.0f; // ビューポート領域の深度値の最大値
    m_pImmediateContext->RSSetViewports(1, &m_viewPort[0]);

    return true;
}

void Renderer::Terminate()
{
    // デバイス・ステートのクリア
    if (m_pImmediateContext) m_pImmediateContext->ClearState();

    // スワップ チェインをウインドウ モードにする
    if (m_pSwapChain) m_pSwapChain->SetFullscreenState(FALSE, nullptr);

    // 取得したインターフェイスの開放
    DX_SAFE_RELEASE(m_pRenderTargetView);
    DX_SAFE_RELEASE(m_pSwapChain);

    DX_SAFE_RELEASE(m_pImmediateContext);
    DX_SAFE_RELEASE(m_pD3DDevice);
}