#pragma once
#define _WIN32_DCOM
#include <mmsystem.h>
#include <xaudio2.h>
#include <string>

#define MAX_WAV 100 //WAVサウンド最大数

class SoundManager
{
public:
	static SoundManager& Instance();
	IXAudio2* m_pXAudio2;
	IXAudio2MasteringVoice* m_pMasteringVoice;
	IXAudio2SourceVoice* m_pSourceVoice[MAX_WAV];
	BYTE* m_pWavBuffer[MAX_WAV];//波形データ（フォーマット等を含まない、純粋に波形データのみ）
	DWORD m_dwWavSize[MAX_WAV];//波形データのサイズ
	int m_iSoundIndex[10];//今回読み込む音の番号
	int m_iSoundCurrentIndex;//音の現在の再生番号

	SoundManager();
	~SoundManager();
	HRESULT Initialize();
	int LoadSound(const std::wstring& szFileName);
	void PlaySound(int iSoundIndex, bool boLoop);
	void StopSound(int iSoundIndex);
};