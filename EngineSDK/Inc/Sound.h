#pragma once
#include "Base.h"
#include "Engine_Defines.h"
#include "SoundMgr.h"

BEGIN(Engine)

class ENGINE_DLL CSound : public CBase
{
public:
	explicit CSound();
	explicit CSound(const wstring& _strName);
	virtual ~CSound();



private:
	LPDIRECTSOUNDBUFFER		m_pSoundBuffer = { nullptr };
	DSBUFFERDESC			m_tBuffInfo = {};
	int						m_iVolume = { 0 };
	int						m_fVolume;

	wstring m_strKey; 
	wstring m_strRelativePath; 

public:
	int Load(const wstring& _strPath);

	void Play(bool _bLoop = false);

	void PlayToBGM(float _StartPosition, bool _bLoop = false);
	void PlayToSampleBGM();

	void Stop(bool _bReset = false);

	void SetVolume(float _fVolume);
	int GetVolume() { return m_fVolume; };

	void SetPosition(float _fPosition); 

	bool CSound::IsPlaying();

	wstring GetKey() { return m_strKey; }
	wstring GetPath() { return m_strRelativePath; }
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetPath(const wstring& _strPath) { m_strRelativePath = _strPath; }

private:
	bool LoadWaveSound(const wstring& _strPath);
	int GetDecibel(float _fVolume);
	void Free();
};

END