
#include "Sound.h"


IMPLEMENT_SINGLETON(CSoundMgr)


CSoundMgr::CSoundMgr()
{
}




void CSoundMgr::StartBGM(BGM_TYPE _eLevelType, _bool _Loop)
{
	if (nullptr != m_CurBGM)
		m_CurBGM->Stop();
	m_bSampleMode = false;
	m_CurBGM = m_arrpBGM[(UINT)_eLevelType];
	float fStart = 0.f;

	if (nullptr != m_CurBGM)
		m_CurBGM->PlayToBGM(fStart, _Loop);
}

void CSoundMgr::EndBGM()
{
	if (nullptr != m_CurBGM)
		m_CurBGM->Stop();
}

void CSoundMgr::SetBGMVolume(float _fVolume)
{
	m_CurBGM->SetVolume(_fVolume); 
}

float CSoundMgr::GetBGMVolume()
{
	return (float)m_CurBGM->GetVolume();
}

void CSoundMgr::StopEffectPlay(SFX_TYPE _eType)
{
	m_arrEffectSnd[(UINT)_eType]->Stop();
}

void CSoundMgr::Set_EffectVolume(float _fVolume, SFX_TYPE _eType)
{
	m_arrEffectSnd[(UINT)_eType]->SetVolume(_fVolume);
}

void CSoundMgr::StartEffectPlay(SFX_TYPE _eType, bool _bRepeat)
{
	m_arrEffectSnd[(UINT)_eType]->Stop(false);
	m_arrEffectSnd[(UINT)_eType]->Play(_bRepeat);
}

CSound* CSoundMgr::Get_EffectSound(SFX_TYPE _eType)
{
	return m_arrEffectSnd[(UINT)_eType];
}


HRESULT CSoundMgr::Initialize(HWND hWnd)
{
	if (FAILED(DirectSoundCreate8(NULL, &m_pSound, NULL)))
	{
		MessageBox(NULL, L"�������̽���������", L"SYSTEM ERROR", MB_OK);
	}

	// ���� ����̽�?�������� ����.
	if (FAILED(m_pSound->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE))) // Flag�� ����
	{
		MessageBox(NULL, L"�������̽� �������� ����", L"SYSTEM ERROR", MB_OK);
	}

	//////////////////////////////////////////////////////////// SOUND 추가 //////////////////////////////////////////////////////////////

	/* BGM */
	m_arrpBGM[(UINT)BGM_TYPE::BGM_LOBBY]					= new CSound(L"../Bin/Resources/Audio/BGM/Lobby_BGM.wav");
	m_arrpBGM[(UINT)BGM_TYPE::BGM_GAMEPLAY]					= new CSound(L"../Bin/Resources/Audio/BGM/GamePlay_BGM.wav");
	m_arrpBGM[(UINT)BGM_TYPE::BGM_ENDING]					= new CSound(L"../Bin/Resources/Audio/BGM/Ending_BGM.wav");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* SFX */
	/* BGM + SFX*/
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_Prologue]			= new CSound(L"../Bin/Resources/Audio/SE/Lobby_ESE.wav");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Trans PROP*/
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_TELEPORTPROP]		= new CSound(L"../Bin/Resources/Audio/SE/SE_TeleportProps.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_LEVELPROP]			= new CSound(L"../Bin/Resources/Audio/SE/SE_LevelProp.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_ENDPROPFAIL]			= new CSound(L"../Bin/Resources/Audio/SE/SE_EndProp_Fail.wav");


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* PLAYER 이동*/
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_WALK]				= new CSound(L"../Bin/Resources/Audio/SE/WoodWalk1.wav");
	Set_EffectVolume(30.f, SFX_TYPE::SFX_WALK);
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_RUN]					= new CSound(L"../Bin/Resources/Audio/SE/WoodDash1.wav");
	Set_EffectVolume(30.f, SFX_TYPE::SFX_RUN);
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_DOWNWALK]			= new CSound(L"../Bin/Resources/Audio/SE/WoodWalk4.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_BREATH]				= new CSound(L"../Bin/Resources/Audio/SE/SE_RunBreath.wav");
	Set_EffectVolume(60.f, SFX_TYPE::SFX_BREATH);
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_SIT]					= new CSound(L"../Bin/Resources/Audio/SE/SE_StandUp.wav");
	
	/* PLAYER 데드 */
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_PLAYERDEAD]			= new CSound(L"../Bin/Resources/Audio/SE/SE_Death.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_REVIVE]				= new CSound(L"../Bin/Resources/Audio/Gingle/Revive.wav");
	Set_EffectVolume(80.f, SFX_TYPE::SFX_REVIVE);

	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_HEARTBEAT]			= new CSound(L"../Bin/Resources/Audio/SE/SE_HeartBeat.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_BREATHEND]			= new CSound(L"../Bin/Resources/Audio/SE/SE_RunBrethEnd.wav");
	Set_EffectVolume(100.f, SFX_TYPE::SFX_BREATHEND);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* PROP 획득*/
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_PROP1]				= new CSound(L"../Bin/Resources/Audio/SE/Prop1.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_PROP2]				= new CSound(L"../Bin/Resources/Audio/SE/Prop2.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_PROPMAGATAMA]		= new CSound(L"../Bin/Resources/Audio/SE/Prop_Magatama.wav");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* PROP 사용*/
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_LIGHTERON]			= new CSound(L"../Bin/Resources/Audio/SE/Zippolighter.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_LIGHTEROFF]			= new CSound(L"../Bin/Resources/Audio/SE/Zippolighter_close.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_HANDLIGHT]			= new CSound(L"../Bin/Resources/Audio/SE/SE_HandLightSwich.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_SPHERE]				= new CSound(L"../Bin/Resources/Audio/SE/SE_Sphere.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_LIGHTSTONEPUT]		= new CSound(L"../Bin/Resources/Audio/SE/SE_LightStonePut.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_DYNAMITEWAIT]		= new CSound(L"../Bin/Resources/Audio/SE/SE_Dynamite_Not_Bomb.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_DYNAMITEBOMB]		= new CSound(L"../Bin/Resources/Audio/SE/SE_Dynamite.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_MIRRORUSE]			= new CSound(L"../Bin/Resources/Audio/SE/SE_MirrorUse.wav");
	Set_EffectVolume(20.f, SFX_TYPE::SFX_MIRRORUSE);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* DOOR 및 캐비넷 사용*/
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_DOOROPEN1]			= new CSound(L"../Bin/Resources/Audio/SE/SE_DoorOldOpen.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_DOORCLOSE]			= new CSound(L"../Bin/Resources/Audio/SE/SE_DoorClose.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_DOORSLIDEOPEN]		= new CSound(L"../Bin/Resources/Audio/SE/SE_DoorSlide.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_CABBINET]			= new CSound(L"../Bin/Resources/Audio/SE/SE_Cabinet.wav");


	/* 캔들 키고 끄기*/
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_FIREON]				= new CSound(L"../Bin/Resources/Audio/SE/SE_FireOn.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_FIREOFF]				= new CSound(L"../Bin/Resources/Audio/SE/SE_FireOff.wav");


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* 몬스터 관련 */

	/* 대기 */
	// CryingGirl
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_CRYWAIT]				= new CSound(L"../Bin/Resources/Audio/SE/SE_dokoniiruno.wav");

	// Spider
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_SPIDERWAIT]			= new CSound(L"../Bin/Resources/Audio/SE/SE_SpiderWait.wav");

	//Men
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_MENWAIT]				= new CSound(L"../Bin/Resources/Audio/SE/SE_MenMonsterWait.wav");

	/* 이동 */
	//CryingGirl
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_CRYWALK]				= new CSound(L"../Bin/Resources/Audio/SE/WoodWalk2.wav");
	
	// Men
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_MENWALK]				= new CSound(L"../Bin/Resources/Audio/SE/SE_MenMonsterWalk.wav");



	/* 마주쳤을 때 */
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_CRYLOOKAT]			= new CSound(L"../Bin/Resources/Audio/SE/SE_CryingGirl_LookAt.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_SPIDERLOOKAT]		= new CSound(L"../Bin/Resources/Audio/SE/SE_LookAtSpider.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_MENLOOKAT]			= new CSound(L"../Bin/Resources/Audio/SE/SE_Men_LookAt.wav");

	/* 마주쳤을 때 추가 BGM*/
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_CRYADDLOOKAT]		= new CSound(L"../Bin/Resources/Audio/AddBGM/CryingGirl_LookAt.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_SPIDERADDLOOKAT]		= new CSound(L"../Bin/Resources/Audio/AddBGM/Men_LookAt.wav");
	m_arrEffectSnd[(UINT)SFX_TYPE::SFX_MENADDLOOKAT]		= new CSound(L"../Bin/Resources/Audio/AddBGM/Spider_LookAt.wav");


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	return S_OK; 
}

void CSoundMgr::Update()
{
}

void CSoundMgr::Free()
{
	EndBGM();

	for (UINT i = 0; i < (UINT)BGM_TYPE::BGM_END; ++i)
	{
		if (nullptr != m_arrpBGM[i])
		{
			delete m_arrpBGM[i];
			m_arrpBGM[i] = nullptr;
		}
	}

	for (UINT i = 0; i < (UINT)SFX_TYPE::SFX_END; ++i)
	{
		if (nullptr != m_arrEffectSnd[i])
		{
			StopEffectPlay(SFX_TYPE(i));
			delete m_arrEffectSnd[i];
			m_arrEffectSnd[i] = nullptr;
		}
	}
}