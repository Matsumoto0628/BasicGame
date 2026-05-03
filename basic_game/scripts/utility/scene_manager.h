/**
 * @file scene_manager.h
 * @brief シーン管理クラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

class Renderer;
class Renderer2D;
class Scene;

/**
 * @brief ゲームシーンのライフサイクルと遷移を管理するシングルトンクラス
 *
 * フレーム末尾で Transit() が要求されたシーンに切り替える。
 * シーンの生成・破棄・更新・描画を一元管理する。
 */
class SceneManager
{
public:
	/**
	 * @brief 管理対象のシーン種別を表す列挙型
	 */
	enum SceneType
	{
		Game,      ///< ゲームメインシーン
		Title,     ///< タイトルシーン
		GameOver,  ///< ゲームオーバーシーン
		GameClear  ///< ゲームクリアシーン
	};

	/** @brief コンストラクタ */
	SceneManager();

	/** @brief デストラクタ */
	~SceneManager();

	/**
	 * @brief レンダラーを受け取って初期シーンを生成・初期化する
	 * @param renderer   3Dレンダラーの参照
	 * @param renderer2d 2Dレンダラーの参照
	 */
	void Initialize(Renderer& renderer, Renderer2D& renderer2d);

	/** @brief 現在のシーンを終了してリソースを解放する */
	void Terminate();

	/** @brief 現在のシーンを更新し、遷移要求があれば切り替える */
	void Update();

	/** @brief 現在のシーンを描画する */
	void Draw();

	/**
	 * @brief 次フレームに遷移するシーンを予約する
	 * @param sceneType 遷移先のシーン種別
	 */
	void Transit(SceneType sceneType);

	/**
	 * @brief シングルトンインスタンスを取得する
	 * @return SceneManager の唯一のインスタンス
	 */
	static SceneManager& Instance();

private:
	/** @brief フレーム末尾で遷移を実行する内部処理 */
	void transit();

	Renderer*   m_pRenderer = nullptr;                           ///< 3Dレンダラーへのポインタ
	Renderer2D* m_pRenderer2d = nullptr;                         ///< 2Dレンダラーへのポインタ
	Scene*      m_pCurrentScene = nullptr;                       ///< 現在のシーンポインタ
	SceneType   m_currentSceneType = SceneType::Title;           ///< 現在のシーン種別
	bool        m_isTransit = false;                             ///< 遷移要求フラグ
};
