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

	//�T�C�Y�̎擾
	DWORD GetSize() { return m_dwSize; }

	// Wave�t�@�C���̃t�H�[�}�b�g�擾
	WAVEFORMATEX* GetFormat() { return m_pwfx; };

	// �I�[�v��
	HRESULT Open(LPWSTR strFileName, WAVEFORMATEX* pwfx);

	// �}���`���f�B�A�֐��œǂݍ���
	HRESULT ReadMMIO();

	// �t�@�C�����Z�b�g
	HRESULT ResetFile();

	// �t�@�C������ǂݍ���
	HRESULT Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead);

	// �t�@�C�������
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

	// ������
	BOOL Init();

	// ���
	void Release();

	// �T�E���h���
	void ReleaseSound(int no);

	// �t�@�C���ǂݍ���
	int LoadSound(LPWSTR szFilename, int loop_num);

	// �T�E���h�Đ�
	void PlaySounds(int no);

	// �T�E���h��~
	void StopSound(int no);
};

//static CAudioBase* const pAudio = &CAudioBase::GetInstance();
extern CAudioBase audio;