/**
 * @file sound_manager.h
 * @brief XAudio2 を使用したサウンド管理クラスの定義
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#define _WIN32_DCOM
#include <mmsystem.h>
#include <xaudio2.h>
#include <string>

/** @brief 管理できる WAV サウンドの最大数 */
#define MAX_WAV 100

/**
 * @brief XAudio2 を使用して WAV ファイルの読み込み・再生・停止を管理するシングルトンクラス
 */
class SoundManager
{
public:
	/**
	 * @brief シングルトンインスタンスを取得する
	 * @return SoundManager の唯一のインスタンス
	 */
	static SoundManager& Instance();

	IXAudio2*              m_pXAudio2;                  ///< XAudio2 エンジン
	IXAudio2MasteringVoice* m_pMasteringVoice;          ///< マスタリングボイス
	IXAudio2SourceVoice*   m_pSourceVoice[MAX_WAV];     ///< サウンドごとのソースボイス
	BYTE*  m_pWavBuffer[MAX_WAV];                        ///< WAV 波形データバッファ（フォーマット情報を含まない生波形）
	DWORD  m_dwWavSize[MAX_WAV];                         ///< 各 WAV バッファのサイズ（バイト）
	int    m_iSoundIndex[10];                            ///< 読み込み済みサウンドの番号一覧
	int    m_iSoundCurrentIndex;                         ///< 現在の再生インデックス

	/** @brief コンストラクタ */
	SoundManager();

	/** @brief デストラクタ */
	~SoundManager();

	/**
	 * @brief XAudio2 エンジンとマスタリングボイスを初期化する
	 * @return 成功すれば S_OK
	 */
	HRESULT Initialize();

	/**
	 * @brief WAV ファイルを読み込んでソースボイスを生成する
	 * @param szFileName 読み込む WAV ファイルのパス（ワイド文字列）
	 * @return 割り当てたサウンドインデックス（PlaySound / StopSound に渡す）
	 */
	int LoadSound(const std::wstring& szFileName);

	/**
	 * @brief 指定インデックスのサウンドを再生する
	 * @param iSoundIndex LoadSound で取得したサウンドインデックス
	 * @param boLoop      ループ再生する場合は true
	 */
	void PlaySound(int iSoundIndex, bool boLoop);

	/**
	 * @brief 指定インデックスのサウンドを停止する
	 * @param iSoundIndex LoadSound で取得したサウンドインデックス
	 */
	void StopSound(int iSoundIndex);
};
