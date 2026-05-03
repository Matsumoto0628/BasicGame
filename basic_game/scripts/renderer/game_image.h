/**
 * @file game_image.h
 * @brief 2D 画像描画用データ構造体の定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include <string>
#include <d2d1_1.h>

/**
 * @brief 2D ビットマップの描画位置・サイズ・表示状態を保持する構造体
 */
struct GameImage
{
	ID2D1Bitmap* pBmp;  ///< 描画するビットマップ
	float x;            ///< 描画 X 座標（スクリーン空間）
	float y;            ///< 描画 Y 座標（スクリーン空間）
	float w;            ///< 描画幅
	float h;            ///< 描画高さ
	bool visible = true; ///< 表示フラグ

	/**
	 * @brief 全パラメータを指定するコンストラクタ
	 * @param b  ビットマップポインタ
	 * @param px X 座標
	 * @param py Y 座標
	 * @param pw 幅
	 * @param ph 高さ
	 */
	GameImage(ID2D1Bitmap* b, float px, float py, float pw, float ph)
		: pBmp(b), x(px), y(py), w(pw), h(ph) {
	}
};
