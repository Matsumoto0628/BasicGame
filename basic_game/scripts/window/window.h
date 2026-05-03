/**
 * @file window.h
 * @brief Win32 ウィンドウ管理クラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#include <windows.h>

/**
 * @brief Win32 ウィンドウの生成・メッセージループ・リサイズを管理するクラス
 */
class Window
{
public:
	/** @brief コンストラクタ */
	Window();

	/** @brief デストラクタ */
	virtual ~Window();

	/**
	 * @brief ウィンドウクラスを登録してウィンドウを生成する
	 * @param hInst アプリケーションのインスタンスハンドル
	 * @return 成功すれば true
	 */
	bool Initialize(HINSTANCE hInst);

	/**
	 * @brief ウィンドウを破棄してリソースを解放する
	 * @return 成功すれば true
	 */
	bool Terminate();

	/**
	 * @brief ウィンドウメッセージを処理する
	 * @return WM_QUIT を受信したら false、継続中は true
	 */
	bool MessageLoop();

	/**
	 * @brief ウィンドウプロシージャ（Win32 コールバック）
	 * @param hWnd   対象ウィンドウハンドル
	 * @param msg    メッセージID
	 * @param wParam 追加パラメータ W
	 * @param lParam 追加パラメータ L
	 * @return メッセージ処理結果
	 */
	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief ウィンドウサイズ変更時の処理
	 * @param width  新しいクライアント幅
	 * @param height 新しいクライアント高さ
	 */
	void OnResize(UINT width, UINT height);

	/**
	 * @brief ウィンドウハンドルを取得する
	 * @return HWND
	 */
	HWND GetWindowHandle() { return m_hWindow; }

private:
	WNDCLASS m_wc;             ///< ウィンドウクラス情報
	HWND     m_hWindow = nullptr;  ///< ウィンドウハンドル
	SIZE     m_sizeWindow;     ///< ウィンドウサイズ
	MSG      m_msg = {};       ///< メッセージ構造体
};
