/**
 * @file game_text.h
 * @brief 2D テキスト描画用データ構造体の定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include <string>

/**
 * @brief 2D テキストの内容・位置・色・表示状態を保持する構造体
 */
struct GameText
{
	std::wstring text;       ///< 描画するテキスト文字列
	float x;                 ///< 描画 X 座標（スクリーン空間）
	float y;                 ///< 描画 Y 座標（スクリーン空間）
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);  ///< テキストカラー（RGBA）
	bool visible = true;     ///< 表示フラグ

	/** @brief デフォルトコンストラクタ */
	GameText() {}

	/**
	 * @brief テキスト・位置・色を指定するコンストラクタ
	 * @param t  テキスト文字列
	 * @param px X 座標
	 * @param py Y 座標
	 * @param c  テキストカラー（RGBA）
	 */
	GameText(const std::wstring& t, float px, float py, DirectX::XMFLOAT4 c)
		: text(t), x(px), y(py), color(c) {
	}
};
