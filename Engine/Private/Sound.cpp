//#include "stdafx.h"
#include "Sound.h"

CSound::CSound()
{
}

CSound::CSound(const wstring& _strName)
	: m_pSoundBuffer(nullptr)
	, m_tBuffInfo{}
	, m_iVolume(0)
{
	Load(_strName);
}

CSound::~CSound()
{
	if (nullptr != m_pSoundBuffer)
		m_pSoundBuffer->Release();
}

void CSound::Free()
{
}

int CSound::Load(const wstring& _strPath)
{

	if (nullptr == CSoundMgr::GetInstance()->GetSoundDevice())
		int a = 1;
	// 확장자 이름 구별하기
	wchar_t szExt[10] = { 0 };
	_wsplitpath_s(_strPath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 10);

	if (!wcscmp(szExt, L".wav")) // WAV 파일 로드
	{
		if (false == LoadWaveSound(_strPath))
			int a = 1;
	}

	return S_OK;
}

bool CSound::LoadWaveSound(const wstring& _strPath)
{
	HMMIO	hFile; // File Handle

	wstring strFilePath = _strPath;

	//CreateFile
	hFile = mmioOpen((wchar_t*)strFilePath.c_str(), NULL, MMIO_READ);//wave파일을 연다.

	if (nullptr == hFile)
	{
		MessageBox(NULL, L"사운드 리소스 경로에 파일 없음", L"사운드 로딩 실패", MB_OK);
		return false;
	}// Raw DirectSound는 mp3 압축 오디오를 재생할 수 없습니다. .........

	//Chunk 청크 구조체, 문자열로 색인을 인식해서 WaveFormat 및 버퍼선언정보를 읽어온다.
	MMCKINFO	pParent;
	memset(&pParent, 0, sizeof(pParent));
	pParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmioDescend(hFile, &pParent, NULL, MMIO_FINDRIFF);

	MMCKINFO	pChild;
	memset(&pChild, 0, sizeof(pChild));
	pChild.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	WAVEFORMATEX	wft;
	memset(&wft, 0, sizeof(wft));
	mmioRead(hFile, (char*)&wft, sizeof(wft));

	mmioAscend(hFile, &pChild, 0);
	pChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);



	memset(&m_tBuffInfo, 0, sizeof(DSBUFFERDESC));
	m_tBuffInfo.dwBufferBytes = pChild.cksize;
	m_tBuffInfo.dwSize = sizeof(DSBUFFERDESC);
	m_tBuffInfo.dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLVOLUME;
	m_tBuffInfo.lpwfxFormat = &wft;
	HRESULT res = CSoundMgr::GetInstance()->GetSoundDevice()->CreateSoundBuffer(&m_tBuffInfo, &m_pSoundBuffer, NULL);


	if (FAILED(CSoundMgr::GetInstance()->GetSoundDevice()->CreateSoundBuffer(&m_tBuffInfo, &m_pSoundBuffer, NULL)))
	{
		MessageBox(NULL, L"사운드버퍼생성실패", L"", MB_OK);
		return false;
	}

	void* pWrite1 = NULL;
	void* pWrite2 = NULL;
	DWORD dwlength1, dwlength2;

	m_pSoundBuffer->Lock(0, pChild.cksize, &pWrite1, &dwlength1
		, &pWrite2, &dwlength2, 0L);

	if (pWrite1 != nullptr)
		mmioRead(hFile, (char*)pWrite1, dwlength1);
	if (pWrite2 != nullptr)
		mmioRead(hFile, (char*)pWrite2, dwlength2);

	m_pSoundBuffer->Unlock(pWrite1, dwlength1, pWrite2, dwlength2);

	mmioClose(hFile, 0);

	// 초기 음량 절반으로 설정
	SetVolume(50.f);

	return true;
}


void CSound::Play(bool _bLoop)
{
	// Play 함수의 1번째 2번째 인자는 0 으로 이미 예약되어있다.
	// 3번째 변수는 사운드를 반복재생 할 것인지 아닌지를 결정한다.
	m_pSoundBuffer->SetCurrentPosition(0);
	m_pSoundBuffer->Stop();
	if (_bLoop)
		m_pSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	else
		m_pSoundBuffer->Play(0, 0, 0);
}

void CSound::PlayToBGM(float _StartPosition, bool _bLoop)
{
	//MGR(CSoundMgr)->RegisterToBGM(this);
	SetPosition(_StartPosition);
	//m_pSoundBuffer->SetCurrentPosition(_StartPosition);
	// Play 함수의 1번째 2번째 인자는 0 으로 이미 예약되어있다.
	// 3번째 변수는 사운드를 반복재생 할 것인지 아닌지를 결정한다.
	if (_bLoop)
		m_pSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	else
		m_pSoundBuffer->Play(0, 0, 0);
}

void CSound::PlayToSampleBGM()
{
	m_pSoundBuffer->Play(0, 0, 0);
}

void CSound::Stop(bool _bReset)
{


	if (nullptr != m_pSoundBuffer)
	{
		m_pSoundBuffer->Stop();
	}

	if (_bReset)
		m_pSoundBuffer->SetCurrentPosition(0);
}


void CSound::SetVolume(float _fVolume)
{
	if (this != nullptr)
	{
		m_fVolume = (int)_fVolume;
		m_iVolume = GetDecibel(_fVolume);
		m_pSoundBuffer->SetVolume(m_iVolume);
	}
}

void CSound::SetPosition(float _fPosition)
{
	Stop(true);

	DWORD dwBytes = (DWORD)((_fPosition / 100.f) * (float)m_tBuffInfo.dwBufferBytes);
	HRESULT res = m_pSoundBuffer->SetCurrentPosition(dwBytes);

}

bool CSound::IsPlaying()
{
	DWORD dwStatus = 0;
	m_pSoundBuffer->GetStatus(&dwStatus);
	return (dwStatus & DSBSTATUS_PLAYING);
}

int CSound::GetDecibel(float _fVolume)
{
	if (_fVolume > 100.f)
		_fVolume = 100.f;
	else if (_fVolume <= 0.f)
		_fVolume = 0.00001f;

	// 1 ~ 100 사이값을 데시벨 단위로 변경
	int iVolume = (LONG)(-2000.0 * log10(100.f / _fVolume));

	if (iVolume < -10000)
		iVolume = -10000;
	return  iVolume;
}
