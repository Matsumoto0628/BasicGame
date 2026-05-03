/**
 * @file renderer_2d.h
 * @brief Direct2D / DirectWrite を使用した 2D レンダラークラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
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

/**
 * @brief Direct2D / DirectWrite を用いてテキストと画像を 2D 描画するクラス
 *
 * D3D11 デバイスと DXGI スワップチェーンを受け取り、
 * D3D11 バックバッファと Direct2D を連携して UI を描画する。
 */
class Renderer2D
{
public:
	/** @brief コンストラクタ */
	Renderer2D();

	/** @brief デストラクタ */
	~Renderer2D();

	/**
	 * @brief Direct2D デバイスと描画コンテキストを初期化する
	 * @param pD3DDevice  D3D11 デバイスポインタ
	 * @param pSwapChain  DXGI スワップチェーンポインタ
	 * @return 成功すれば true
	 */
	bool Initialize(ID3D11Device* pD3DDevice, IDXGISwapChain* pSwapChain);

	/** @brief 全 Direct2D リソースを解放する */
	void Terminate();

	/** @brief 登録済みのテキストと画像をすべて描画する */
	void Draw();

	/**
	 * @brief テキストを描画リストに追加する
	 * @param gameText 追加する GameText ポインタ
	 */
	void AddText(GameText* gameText);

	/** @brief 描画リストのテキストをすべて削除する */
	void ClearTexts();

	/** @brief 描画リストの画像をすべて削除する */
	void ClearImages();

	/**
	 * @brief ファイルから 2D ビットマップを読み込む
	 * @param uri 画像ファイルのパス（ワイド文字列）
	 * @return 読み込んだ ID2D1Bitmap ポインタ（失敗時 nullptr）
	 */
	ID2D1Bitmap* LoadBitmapFromFile(const wchar_t* uri);

	/**
	 * @brief 画像を描画リストに追加する
	 * @param gameImage 追加する GameImage ポインタ
	 */
	void AddImage(GameImage* gameImage);

	/**
	 * @brief 指定インデックスのテキストの表示・非表示を切り替える
	 * @param idx   テキストのインデックス
	 * @param which true で表示、false で非表示
	 */
	void SwitchText(int idx, bool which);

	/**
	 * @brief 指定インデックスの画像の表示・非表示を切り替える
	 * @param idx   画像のインデックス
	 * @param which true で表示、false で非表示
	 */
	void SwitchImage(int idx, bool which);

private:
	/**
	 * @brief テキストを 2D 描画する内部処理
	 * @param text 描画するテキスト
	 * @param x    X 座標
	 * @param y    Y 座標
	 */
	void drawText(const wchar_t* text, float x, float y);

	/**
	 * @brief ビットマップを 2D 描画する内部処理
	 * @param bmp ビットマップポインタ
	 * @param x   X 座標
	 * @param y   Y 座標
	 * @param w   幅
	 * @param h   高さ
	 */
	void drawImage(ID2D1Bitmap* bmp, float x, float y, float w, float h);

	ID2D1Factory1*     m_pD2DFactory = nullptr;       ///< Direct2D ファクトリ
	ID2D1Device*       m_pD2DDevice = nullptr;        ///< Direct2D デバイス
	ID2D1DeviceContext* m_pD2DContext = nullptr;      ///< Direct2D デバイスコンテキスト
	ID2D1Bitmap1*      m_pD2DTargetBitmap = nullptr;  ///< D3D11 バックバッファと共有するターゲットビットマップ

	IDWriteFactory*    m_pDWriteFactory = nullptr;  ///< DirectWrite ファクトリ
	IDWriteTextFormat* m_pTextFormat = nullptr;     ///< テキストフォーマット

	ID2D1SolidColorBrush* m_pBrush = nullptr;  ///< 単色ブラシ

	std::vector<GameText*>  m_pTexts;   ///< テキスト描画リスト
	std::vector<GameImage*> m_pImages;  ///< 画像描画リスト
};
