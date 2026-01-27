#pragma once
#include <vector>
#include <string>

class ID2D1Factory1;
class ID2D1Device;
class ID2D1DeviceContext;
class ID2D1Bitmap;
class ID2D1Bitmap1;
class IDWriteFactory;
class IDWriteTextFormat;
class ID2D1SolidColorBrush;
class GameText;
class GameImage;

class Renderer2D
{
public:
    Renderer2D();
    ~Renderer2D();
    bool Initialize(ID3D11Device* pD3DDevice, IDXGISwapChain* pSwapChain);
    void Terminate();
    void Draw();
    void AddText(GameText* gameText);
    void ClearTexts();
    void ClearImages();
    ID2D1Bitmap* LoadBitmapFromFile(const wchar_t* uri);
    void AddImage(GameImage* gameImage);
    void SwitchText(int idx, bool which);
    void SwitchImage(int idx, bool which);

private:
    void drawText(const wchar_t* text, float x, float y);
    void drawImage(ID2D1Bitmap* bmp, float x, float y, float w, float h);

    // Direct2D
    ID2D1Factory1* m_pD2DFactory = nullptr;
    ID2D1Device* m_pD2DDevice = nullptr;
    ID2D1DeviceContext* m_pD2DContext = nullptr;
    ID2D1Bitmap1* m_pD2DTargetBitmap = nullptr;

    // DirectWrite
    IDWriteFactory* m_pDWriteFactory = nullptr;
    IDWriteTextFormat* m_pTextFormat = nullptr;

    // Brush
    ID2D1SolidColorBrush* m_pBrush = nullptr;

	std::vector<GameText*> m_pTexts;
	std::vector<GameImage*> m_pImages;
};