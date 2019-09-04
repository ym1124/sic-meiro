#include "..\Header\Sound.h"

CAudioBase audio;

//-----------------------------------------------------------------------------
// 生成
CAudioWave::CAudioWave()
{
	m_pwfx = NULL;
	m_hmmio = NULL;
	//m_pResourceBuffer = NULL;
	m_dwSize = 0;
}
//-----------------------------------------------------------------------------
// 解放
CAudioWave::~CAudioWave()
{
	Release();
}
//-----------------------------------------------------------------------------
// オープン
HRESULT CAudioWave::Open(LPWSTR strFileName, WAVEFORMATEX* pwfx)
{
	HRESULT hr;

	if (strFileName == NULL) {
		return FALSE;
	}
	if (m_pwfx) {
		delete[] m_pwfx;
		m_pwfx = NULL;
	}
	//SAFE_DELETE_ARRAY(m_pwfx);

	// ファイルオープン
	m_hmmio = mmioOpen(strFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (FAILED(hr = ReadMMIO())) {
		mmioClose(m_hmmio, 0);
		return FALSE;
	}

	if (FAILED(ResetFile())) {
		return FALSE;
	}

	// ファイルサイズ保持
	m_dwSize = m_ck.cksize;

	return TRUE;
}
//-----------------------------------------------------------------------------
// マルチメディア関数で読み込み
HRESULT CAudioWave::ReadMMIO()
{
	MMCKINFO ckIn;
	PCMWAVEFORMAT pcmWaveFormat;

	m_pwfx = NULL;

	if ((0 != mmioDescend(m_hmmio, &m_ckRiff, NULL, 0))) {
		return E_FAIL;
	}

	// Waveファイルかどうか
	if ((m_ckRiff.ckid != FOURCC_RIFF) || (m_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E'))) {
		return E_FAIL;
	}

	// FMTチャンク検索
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (0 != mmioDescend(m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK)) {
		return E_FAIL;
	}

	// pcmWaveFormat取得
	if (mmioRead(m_hmmio, (HPSTR)&pcmWaveFormat, sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat)) {
		return E_FAIL;
	}

	// 余分なBYTEがある？
	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM) {
		m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
		if (NULL == m_pwfx) {
			return E_FAIL;
		}

		// waveformatexコピー
		memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		m_pwfx->cbSize = 0;
	}
	else {
		// 余分なBYTE読み込み
		WORD cbExtraBytes = 0L;
		if (mmioRead(m_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD)) {
			return E_FAIL;
		}

		m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX) + cbExtraBytes];
		if (NULL == m_pwfx) {
			return E_FAIL;
		}

		// waveformatexコピー
		memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		m_pwfx->cbSize = cbExtraBytes;

		// 余分なBYTE読み込み
		if (mmioRead(m_hmmio, (CHAR*)(((BYTE*)&(m_pwfx->cbSize)) + sizeof(WORD)), cbExtraBytes) != cbExtraBytes) {
			if (m_pwfx) {
				delete m_pwfx;
				m_pwfx = NULL;
			}
			//SAFE_DELETE(m_pwfx);
			return E_FAIL;
		}
	}

	// FMTチャンクから出る
	if (0 != mmioAscend(m_hmmio, &ckIn, 0)) {
		if (m_pwfx) {
			delete m_pwfx;
			m_pwfx = NULL;
		}
		//SAFE_DELETE(m_pwfx);
		return E_FAIL;
	}

	return S_OK;
}
//-----------------------------------------------------------------------------
// ファイルリセット
HRESULT CAudioWave::ResetFile()
{
	if (m_hmmio == NULL) {
		return CO_E_NOTINITIALIZED;
	}

	// データ検索
	if (-1 == mmioSeek(m_hmmio, m_ckRiff.dwDataOffset + sizeof(FOURCC), SEEK_SET)) {
		return E_FAIL;
	}

	// データチャンク検索
	m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (0 != mmioDescend(m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK)) {
		return E_FAIL;
	}

	return S_OK;
}
//-----------------------------------------------------------------------------
// ファイルから読み込み
HRESULT CAudioWave::Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead)
{
	MMIOINFO mmioinfoIn;

	if (m_hmmio == NULL) {
		return CO_E_NOTINITIALIZED;
	}
	if (pBuffer == NULL || pdwSizeRead == NULL) {
		return E_INVALIDARG;
	}

	*pdwSizeRead = 0;

	if (0 != mmioGetInfo(m_hmmio, &mmioinfoIn, 0)) {
		return E_FAIL;
	}

	UINT cbDataIn = dwSizeToRead;
	if (cbDataIn > m_ck.cksize) {
		cbDataIn = m_ck.cksize;
	}

	m_ck.cksize -= cbDataIn;

	for (DWORD cT = 0; cT < cbDataIn; cT++) {
		// BYTEコピー
		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead) {
			if (0 != mmioAdvance(m_hmmio, &mmioinfoIn, MMIO_READ)) {
				return E_FAIL;
			}

			if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead) {
				return E_FAIL;
			}
		}

		// コピー
		*((BYTE*)pBuffer + cT) = *((BYTE*)mmioinfoIn.pchNext);
		mmioinfoIn.pchNext++;
	}

	if (0 != mmioSetInfo(m_hmmio, &mmioinfoIn, 0)) {
		return E_FAIL;
	}

	*pdwSizeRead = cbDataIn;

	return S_OK;
}
//-----------------------------------------------------------------------------
// ファイルを閉じる
HRESULT CAudioWave::Close()
{
	mmioClose(m_hmmio, 0);
	m_hmmio = NULL;
	//if (!m_pResourceBuffer) {
	//	delete[] m_pResourceBuffer;
	//	m_pResourceBuffer = NULL;
	//}
	////SAFE_DELETE_ARRAY(m_pResourceBuffer);

	return S_OK;
}

void CAudioWave::Release()
{
	Close();

	if (m_pwfx) {
		delete[] m_pwfx;
		m_pwfx = NULL;
	}
	//SAFE_DELETE_ARRAY(m_pwfx);
}

//--------------------------------------------------------------------------------------
// 生成
CAudioBase::CAudioBase()
{
	for (int i = 0; i < MAX_SOUND; i++) {
		pSourceVoice[i] = NULL;
		pXAudioBuffer[i] = NULL;
		pbWaveData[i] = NULL;
	}

	pXAudio2 = NULL;
	pMasteringVoice = NULL;
}
//-----------------------------------------------------------------------------
// 解放
CAudioBase::~CAudioBase()
{
	Release();

	// マスターボイス解放
	if (pMasteringVoice) {
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = NULL;
	}

	// XAudio解放
	if (pXAudio2) {
		pXAudio2->Release();
		pXAudio2 = NULL;
	}
	//SAFE_RELEASE(pXAudio2);

	// COM解放
	CoUninitialize();
}
//--------------------------------------------------------------------------------------
// 初期化
BOOL CAudioBase::Init()
{
	// COM初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// デバッグモードの場合
	UINT32 flags = 0;
	//#ifdef _DEBUG
	//	flags |= XAUDIO2_DEBUG_ENGINE;
	//#endif

		// XAudio2生成
	if (FAILED(XAudio2Create(&pXAudio2, flags))) {
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイス生成
	if (FAILED(pXAudio2->CreateMasteringVoice(&pMasteringVoice))) {
		if (pXAudio2) {
			pXAudio2->Release();
			pXAudio2 = NULL;
		}
		//SAFE_RELEASE(pXAudio2);
		CoUninitialize();
		return E_FAIL;
	}

	return S_OK;
}
//-----------------------------------------------------------------------------
// 解放
void CAudioBase::Release()
{
	// ソースボイス解放
	for (int i = 0; i < MAX_SOUND; i++) {
		ReleaseSound(i);
	}
}
//-----------------------------------------------------------------------------
// サウンド解放
void CAudioBase::ReleaseSound(int no)
{
	StopSound(no);

	if (pbWaveData[no]) {
		delete pbWaveData[no];
		pbWaveData[no] = NULL;
	}
	//SAFE_DELETE(pbWaveData[no]);

	if (pXAudioBuffer[no]) {
		delete pXAudioBuffer[no];
		pXAudioBuffer[no] = NULL;
	}
	//SAFE_DELETE(pXAudioBuffer[no]);

	if (pSourceVoice[no] != NULL) {
		pSourceVoice[no]->DestroyVoice();
		pSourceVoice[no] = NULL;
	}
}
//--------------------------------------------------------------------------------------
// ファイル読み込み
int CAudioBase::LoadSound(LPWSTR szFilename, int loop_num)
{
	int i;

	// 空いているWaveがあるか検索
	for (i = 0; i < MAX_SOUND; i++) {
		if (pSourceVoice[i] == NULL) {
			break;
		}
	}
	if (i >= MAX_SOUND) {
		return -1;
	}

	// Waveファイル読み込み
	CAudioWave wav;
	if (FAILED(wav.Open(szFilename, NULL))) {
		return -1;
	}

	// Waveファイルのフォーマット取得
	WAVEFORMATEX* pwfx = wav.GetFormat();

	// サイズ計算
	DWORD cbWaveSize = wav.GetSize();

	// メモリにデータを乗せる
	pbWaveData[i] = new BYTE[cbWaveSize];
	if (FAILED(wav.Read(pbWaveData[i], cbWaveSize, &cbWaveSize))) {
		if (pbWaveData[i]) {
			delete[] pbWaveData[i];
			pbWaveData[i] = NULL;
		}
		//SAFE_DELETE_ARRAY(pbWaveData[i]);
		return -1;
	}

	// ソースボイス生成
	if (FAILED(pXAudio2->CreateSourceVoice(&pSourceVoice[i], pwfx))) {
		if (pbWaveData[i]) {
			delete[] pbWaveData[i];
			pbWaveData[i] = NULL;
		}
		//SAFE_DELETE_ARRAY(pbWaveData[i]);
		return -1;
	}

	if (loop_num != 0) {
		loop_num = XAUDIO2_LOOP_INFINITE;
	}

	// オーディオバッファ設定
	pXAudioBuffer[i] = new XAUDIO2_BUFFER;
	pXAudioBuffer[i]->Flags = XAUDIO2_END_OF_STREAM;
	pXAudioBuffer[i]->AudioBytes = cbWaveSize;
	pXAudioBuffer[i]->pAudioData = pbWaveData[i];
	pXAudioBuffer[i]->PlayBegin = 0;
	pXAudioBuffer[i]->PlayLength = 0;
	pXAudioBuffer[i]->LoopBegin = 0;
	pXAudioBuffer[i]->LoopLength = 0;
	pXAudioBuffer[i]->LoopCount = loop_num;
	if (FAILED(pSourceVoice[i]->SubmitSourceBuffer(pXAudioBuffer[i]))) {
		pSourceVoice[i]->DestroyVoice();

		if (pbWaveData[i]) {
			delete[] pbWaveData[i];
			pbWaveData[i] = NULL;
		}
		//SAFE_DELETE_ARRAY(pbWaveData[i]);
		return -1;
	}

	wav.Release();

	return i;
}
//--------------------------------------------------------------------------------------
// サウンド再生
void CAudioBase::PlaySounds(int no)
{
	if (no == -1 || pSourceVoice[no] == NULL) {
		return;
	}
	//StopSound(no);
	pSourceVoice[no]->FlushSourceBuffers();
	pSourceVoice[no]->SubmitSourceBuffer(pXAudioBuffer[no]);
	HRESULT hr = pSourceVoice[no]->Start();
}
//--------------------------------------------------------------------------------------
// サウンド停止
void CAudioBase::StopSound(int no)
{
	if (no == -1 || pSourceVoice[no] == NULL) {
		return;
	}

	pSourceVoice[no]->Stop();
}