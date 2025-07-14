#pragma once

#include "Engine_Defines.h"
#include "Base.h"

#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

BEGIN(Engine)

enum class BGM_TYPE
{
	BGM_LOBBY,
	BGM_GAMEPLAY,
	BGM_ENDING,
	BGM_END
};

enum class SFX_TYPE
{
	/* BGM*/
	SFX_Prologue,


	/* PROP ���ͷ���*/
	SFX_TELEPORTPROP,
	SFX_LEVELPROP,
	SFX_ENDPROPFAIL,

	/* �� ���ͷ���*/
	SFX_DOOROPEN0,
	SFX_DOOROPEN1,
	SFX_DOORCLOSE,
	SFX_DOORSLIDEOPEN,

	/* ĳ��� ����*/
	SFX_CABBINET,

	/* �к� ���� */
	SFX_FIREON,
	SFX_FIREOFF,

	/* �÷��̾� */
	/* �̵� ����*/
	SFX_WALK,
	SFX_RUN,
	SFX_DOWNWALK,
	SFX_BREATH,
	SFX_BREATHEND,
	SFX_SIT,

	/* ���� */
	SFX_PLAYERDEAD,
	SFX_REVIVE,
	SFX_HEARTBEAT,

	/* ��� ����*/
	SFX_LIGHTERON,
	SFX_LIGHTEROFF,
	SFX_HANDLIGHT,
	SFX_SPHERE,
	SFX_LIGHTSTONEPUT,
	SFX_DYNAMITEWAIT,
	SFX_DYNAMITEBOMB,
	SFX_MIRRORUSE,

	/* ���� ȹ�� ����*/
	SFX_PROP1,
	SFX_PROP2,
	SFX_PROPMAGATAMA,

	/* ���� ����*/
	
	/* ��� */
	SFX_CRYWAIT,
	SFX_SPIDERWAIT,
	SFX_MENWAIT,


	/* �̵� */
	SFX_CRYWALK,
	SFX_MENWALK,
	
	/* �������� ��*/
	SFX_CRYLOOKAT,
	SFX_SPIDERLOOKAT,
	SFX_MENLOOKAT,

	/* �������� �� �߰� BGM */
	SFX_CRYADDLOOKAT,
	SFX_SPIDERADDLOOKAT,
	SFX_MENADDLOOKAT,

	


	SFX_END
};

class CSound;

class ENGINE_DLL CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr);

private :
	CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	LPDIRECTSOUND8	GetSoundDevice() { return m_pSound; }
	void			StartBGM(BGM_TYPE _eLevelType, _bool _Loop = false);
	void			EndBGM();
	void			SetBGMVolume(float _fVolume);
	float			GetBGMVolume();
	void			StartEffectPlay(SFX_TYPE _eType, bool _bRepeat = false);
	CSound*			Get_EffectSound(SFX_TYPE _eType);
	void			StopEffectPlay(SFX_TYPE _eType);
	void			Set_EffectVolume(float _fVolume, SFX_TYPE _eType);
	//CSound*			Get_EffectSound(SFX_TYPE _eType) { return m_arrEffectSnd[(UINT)_eType]; }
	HRESULT			Initialize(HWND hWnd);
	void			Update();

private:
	LPDIRECTSOUND8	m_pSound								= { nullptr };	
	CSound*			m_bgm[(UINT)BGM_TYPE::BGM_END]			= { nullptr };
	CSound*			m_arrpBGM[(UINT)BGM_TYPE::BGM_END]		= { nullptr };
	CSound*			m_arrEffectSnd[(UINT)SFX_TYPE::SFX_END] = { nullptr };
	CSound*			m_CurBGM								= { nullptr };
	bool			m_bSampleMode							= { false };
	float			m_fAccSampleTime						= { 0.f };
	float			m_fStartPos;



	// CBase��(��) ���� ��ӵ�?
	void Free() override;
};

END