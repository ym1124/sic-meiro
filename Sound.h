#pragma once

#include <windows.h>

#pragma comment ( lib, "winmm.lib" )
#pragma comment ( lib, "xaudio2.lib" )

#include <mmsystem.h>
#include <xaudio2.h>

#define MAX_SOUND 32

class CAudioWave
{
private:
	
public:
	HMMIO m_hmmio;
	WAVEFORMATEX* m_pwfx;
	DWORD m_dwSize;
	MMCKINFO m_ckRiff;
	MMCKINFO m_ck;

	CAudioWave();
	~CAudioWave();

	//サイズの取得
	DWORD GetSize() { return m_dwSize; }

	// Waveファイルのフォーマット取得
	WAVEFORMATEX* GetFormat() { return m_pwfx; };

	// オープン
	HRESULT Open(LPWSTR strFileName, WAVEFORMATEX* pwfx);

	// マルチメディア関数で読み込み
	HRESULT ReadMMIO();

	// ファイルリセット
	HRESULT ResetFile();

	// ファイルから読み込み
	HRESULT Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead);

	// ファイルを閉じる
	HRESULT Close();

	void Release();
};

class CAudioBase
{
private:

public:
	IXAudio2 *pXAudio2;
	IXAudio2MasteringVoice *pMasteringVoice;

	IXAudio2SourceVoice *pSourceVoice[MAX_SOUND];
	XAUDIO2_BUFFER* pXAudioBuffer[MAX_SOUND];
	BYTE* pbWaveData[MAX_SOUND];

	CAudioBase();
	~CAudioBase();

	//static CAudioBase GetInstance()
	//{
	//	CAudioBase instance;
	//	return instance;
	//}

	// 初期化
	BOOL Init();

	// 解放
	void Release();

	// サウンド解放
	void ReleaseSound(int no);

	// ファイル読み込み
	int LoadSound(LPWSTR szFilename, int loop_num);

	// サウンド再生
	void PlaySounds(int no);

	// サウンド停止
	void StopSound(int no);
};

//static CAudioBase* const pAudio = &CAudioBase::GetInstance();
extern CAudioBase audio;