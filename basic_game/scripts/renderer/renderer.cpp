#include "renderer.h"
#include "cb_view.h"

Renderer::Renderer()
    :m_nearClipDist(0.1f)
	, m_farClipDist(1000.f)
	, m_fov(DirectX::XMConvertToRadians(30.f))
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

    CompileShader(L"scripts/shader/vertex_shader.hlsl", L"scripts/shader/pixel_shader.hlsl", DefaultShader);

	m_renderParam.Initialize(*this);

	setupProjectionTransform();

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

    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    // OMにブレンドステートオブジェクトを設定
    FLOAT BlendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
    m_pImmediateContext->OMSetBlendState(m_pBlendState, BlendFactor, 0xffffffff);

    m_pImmediateContext->OMSetDepthStencilState(m_pDepthState, 0);

    // 青でクリア
    float color[] = { 0.f, 0.f, 1.f, 0.f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);    

    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    
    m_pImmediateContext->IASetInputLayout(DefaultShader.pInputLayout);
    m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->VSSetShader(DefaultShader.pVertexShader, nullptr, 0);
    m_pImmediateContext->PSSetShader(DefaultShader.pPixelShader, nullptr, 0);
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

    // 深度ステンシルバッファをつくる
    // DepthStencil Texture 作成
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = m_screenWidth;
    depthDesc.Height = m_screenHeight;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* depthTexture = nullptr;
    hr = m_pD3DDevice->CreateTexture2D(&depthDesc, nullptr, &depthTexture);
    if (FAILED(hr)) return false;

    // DepthStencilView 作成
    hr = m_pD3DDevice->CreateDepthStencilView(
        depthTexture,
        nullptr,
        &m_pDepthStencilView
    );
    DX_SAFE_RELEASE(depthTexture);

    if (FAILED(hr)) return false;

    // ビューポートの設定
    m_viewPort[0].TopLeftX = 0.0f;    // ビューポート領域の左上X座標。
    m_viewPort[0].TopLeftY = 0.0f;    // ビューポート領域の左上Y座標。
    m_viewPort[0].Width = static_cast<float>(m_screenWidth);  // ビューポート領域の幅
    m_viewPort[0].Height = static_cast<float>(m_screenHeight);  // ビューポート領域の高さ
    m_viewPort[0].MinDepth = 0.0f; // ビューポート領域の深度値の最小値
    m_viewPort[0].MaxDepth = 1.0f; // ビューポート領域の深度値の最大値
    m_pImmediateContext->RSSetViewports(1, &m_viewPort[0]);

    // RenderTarget0へのAlphaブレンド描画設定
    D3D11_BLEND_DESC BlendState;
    ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
    BlendState.AlphaToCoverageEnable = FALSE;
    BlendState.IndependentBlendEnable = FALSE;
    BlendState.RenderTarget[0].BlendEnable = TRUE;
    BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = m_pD3DDevice->CreateBlendState(&BlendState, &m_pBlendState);
    if (FAILED(hr)) {
        //TRACE(L"InitDirect3D g_pD3DDevice->CreateBlendState", hr);
        return false;
    }

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

	DX_SAFE_RELEASE(m_pBlendState);
}

bool Renderer::CompileShader(const WCHAR* vsPath, const WCHAR* psPath, Shader& outShader)
{
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* errBlob = nullptr;
    auto pDevice = GetDevice();

    // シェーダーコンパイル
    auto hr = D3DCompileFromFile(
        vsPath,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "vs_4_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &vsBlob,
        &errBlob
    );
    if (FAILED(hr)) return false;

    // 頂点シェーダ作成(シェーダオブジェクト作成)
    ID3D11VertexShader* pVertexShader = nullptr;
    hr = pDevice->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &pVertexShader
    );
    if (FAILED(hr)) return false;

    // 入力レイアウトオブジェクト作成
    ID3D11InputLayout* pInputLayout = nullptr;
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    hr = pDevice->CreateInputLayout(
        layout,
        _countof(layout),
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &pInputLayout
    );
    if (FAILED(hr)) return false;

    // ピクセルシェーダー作成
    ID3DBlob* psBlob = nullptr;
    hr = D3DCompileFromFile(
        psPath,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "ps_4_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &psBlob,
        &errBlob
    );

    ID3D11PixelShader* pPixelShader = nullptr;
    hr = pDevice->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &pPixelShader
    );
    if (FAILED(hr)) return false;

    outShader.pVertexShader = pVertexShader;
    outShader.pPixelShader = pPixelShader;
    outShader.pInputLayout = pInputLayout;

    return true;
}

/*
 *    透視投影行列を設定
 */
bool Renderer::setupProjectionTransform()
{
    DirectX::XMMATRIX mat = DirectX::XMMatrixPerspectiveFovLH(
        m_fov,
        static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight),   // アスペクト比
        m_nearClipDist,
        m_farClipDist);
    mat = XMMatrixTranspose(mat);

    auto cb = GetRenderParam().CbProjectionSet;
    XMStoreFloat4x4(&cb.Data.Projection, mat);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    auto pDeviceContext = GetDeviceContext();
    // CBufferにひもづくハードウェアリソースマップ取得（ロックして取得）
    HRESULT hr = pDeviceContext->Map(
        cb.pBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource);
    if (FAILED(hr)) {
        return false;
    }
    CopyMemory(mappedResource.pData, &cb.Data, sizeof(cb.Data));
    // マップ解除
    pDeviceContext->Unmap(cb.pBuffer, 0);

    // VSにProjectionMatrixをセット(ここで1度セットして以後不変)
    pDeviceContext->VSSetConstantBuffers(2, 1, &cb.pBuffer);

    return true;
}

bool Renderer::SetupViewTransform(const DirectX::XMMATRIX& viewMat)
{
    auto cb = GetRenderParam().CbViewSet;
    XMStoreFloat4x4(&cb.Data.View, XMMatrixTranspose(viewMat));

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    auto pDeviceContext = GetDeviceContext();
    // CBufferにひもづくハードウェアリソースマップ取得（ロックして取得）
    HRESULT hr = pDeviceContext->Map(
        cb.pBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource);
    if (FAILED(hr)) {
        //DXTRACE_ERR(L"DrawSceneGraph failed", hr);
        return false;
    }
    CopyMemory(mappedResource.pData, &cb.Data, sizeof(cb.Data));
    // マップ解除
    pDeviceContext->Unmap(cb.pBuffer, 0);

    // VSにViewMatrixをセット
    pDeviceContext->VSSetConstantBuffers(1, 1, &cb.pBuffer);

    return true;
}