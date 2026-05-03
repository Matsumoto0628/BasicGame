/**
 * @file application.h
 * @brief アプリケーション全体を管理するクラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include "window.h"
#include "renderer.h"
#include "renderer_2d.h"

/**
 * @brief アプリケーションのエントリポイントとなるクラス
 *
 * ウィンドウ・レンダラーのライフサイクルを管理し、
 * メインループを駆動する。
 */
class Application
{
public:
	/**
	 * @brief ウィンドウとレンダラーを生成して初期化する
	 * @param hInst アプリケーションのインスタンスハンドル
	 */
	void Initialize(HINSTANCE hInst);

	/** @brief シーンマネージャーを介してシーンを初期化する */
	void Setup();

	/** @brief ウィンドウが閉じられるまでメインループを実行する */
	void Loop();

	/** @brief 全リソースを解放して終了する */
	void Terminate();

private:
	/** @brief 1フレーム分の更新・描画を行う。終了要求が来たら false を返す */
	bool gameLoop();

private:
	Window     m_window;      ///< ウィンドウ管理
	Renderer   m_renderer;    ///< 3Dレンダラー
	Renderer2D m_renderer2d;  ///< 2Dレンダラー（UI・テキスト）
};
