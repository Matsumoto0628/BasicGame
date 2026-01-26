#include "renderer_2d.h"
#include <d2d1_1.h>
#include <dwrite.h>
#include "game_text.h"
#include "game_image.h"
#include <wincodec.h>

Renderer2D::Renderer2D() {}
Renderer2D::~Renderer2D() { Terminate(); }

bool Renderer2D::Initialize(ID3D11Device* pD3DDevice, IDXGISwapChain* pSwapChain)
{
    HRESULT hr;

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    if (FAILED(hr)) return false;

    IDXGIDevice* pDxgiDevice = nullptr;
    pD3DDevice->QueryInterface(IID_PPV_ARGS(&pDxgiDevice));

    m_pD2DFactory->CreateDevice(pDxgiDevice, &m_pD2DDevice);
    m_pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pD2DContext);
    pDxgiDevice->Release();

    IDXGISurface* pBackBuffer = nullptr;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

    D2D1_BITMAP_PROPERTIES1 props =
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                D2D1_ALPHA_MODE_PREMULTIPLIED));

    m_pD2DContext->CreateBitmapFromDxgiSurface(pBackBuffer, &props, &m_pD2DTargetBitmap);
    m_pD2DContext->SetTarget(m_pD2DTargetBitmap);
    pBackBuffer->Release();

    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_pDWriteFactory));

    m_pDWriteFactory->CreateTextFormat(
        L"Segoe UI",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        16.0f,
        L"ja-jp",
        &m_pTextFormat);

    m_pD2DContext->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White),
        &m_pBrush);

    return true;
}

void Renderer2D::Draw()
{
    m_pD2DContext->BeginDraw();

    for (auto& image : m_pImages)
    {
		if (!image->visible) continue;
        drawImage(image->pBmp, image->x, image->y, image->w, image->h);
    }

    for (auto& text : m_pTexts) 
    {
        if (!text->visible) continue;
        m_pBrush->SetColor(D2D1::ColorF(text->color.x, text->color.y, text->color.z, text->color.w));
        drawText(text->text.c_str(), text->x, text->y);
    }

    m_pD2DContext->EndDraw();
}

void Renderer2D::drawText(const wchar_t* text, float x, float y)
{
    D2D1_RECT_F rect = D2D1::RectF(x, y, x + 800, y + 200);

    m_pD2DContext->DrawTextW(
        text,
        (UINT32)wcslen(text),
        m_pTextFormat,
        rect,
        m_pBrush);
}

void Renderer2D::Terminate()
{
    for (auto p : m_pTexts)
    {
        delete p;
    }
    m_pTexts.clear();

    for (auto p : m_pImages)
    {
        delete p;
    }
    m_pImages.clear();

    if (m_pBrush) { m_pBrush->Release(); m_pBrush = nullptr; }
    if (m_pTextFormat) { m_pTextFormat->Release(); m_pTextFormat = nullptr; }
    if (m_pDWriteFactory) { m_pDWriteFactory->Release(); m_pDWriteFactory = nullptr; }
    if (m_pD2DTargetBitmap) { m_pD2DTargetBitmap->Release(); m_pD2DTargetBitmap = nullptr; }
    if (m_pD2DContext) { m_pD2DContext->Release(); m_pD2DContext = nullptr; }
    if (m_pD2DDevice) { m_pD2DDevice->Release(); m_pD2DDevice = nullptr; }
    if (m_pD2DFactory) { m_pD2DFactory->Release(); m_pD2DFactory = nullptr; }
}

void Renderer2D::AddText(GameText* pGameText)
{
	m_pTexts.push_back(pGameText);
}

void Renderer2D::ClearTexts()
{
    for (auto p : m_pTexts)
    {
        delete p;
    }
    m_pTexts.clear();
}

void Renderer2D::AddImage(GameImage* pGameImage)
{
    m_pImages.push_back(pGameImage);
}

ID2D1Bitmap* Renderer2D::LoadBitmapFromFile(const wchar_t* uri)
{
    IWICImagingFactory* pWICFactory = nullptr;
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pFrame = nullptr;
    IWICFormatConverter* pConverter = nullptr;
    ID2D1Bitmap* pBitmap = nullptr;

    CoInitialize(nullptr);

    CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pWICFactory));

    pWICFactory->CreateDecoderFromFilename(
        uri,
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder);

    pDecoder->GetFrame(0, &pFrame);

    pWICFactory->CreateFormatConverter(&pConverter);

    pConverter->Initialize(
        pFrame,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.f,
        WICBitmapPaletteTypeMedianCut);

    m_pD2DContext->CreateBitmapFromWicBitmap(
        pConverter,
        nullptr,
        &pBitmap);

    pConverter->Release();
    pFrame->Release();
    pDecoder->Release();
    pWICFactory->Release();

    return pBitmap;
}

void Renderer2D::drawImage(ID2D1Bitmap* bmp, float x, float y, float w, float h)
{
    D2D1_RECT_F dest = D2D1::RectF(x, y, x + w, y + h);
    m_pD2DContext->DrawBitmap(bmp, dest);
}

void Renderer2D::SwitchText(int idx, bool which)
{
	if (idx < 0 || idx >= (int)m_pTexts.size()) return;
	m_pTexts[idx]->visible = which;
}

void Renderer2D::SwitchImage(int idx, bool which)
{
	if (idx < 0 || idx >= (int)m_pImages.size()) return;
	m_pImages[idx]->visible = which;
}