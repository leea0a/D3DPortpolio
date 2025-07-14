#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "Loader.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_Prologue.h"
#include "Level_GamePlay.h"
#include "Level_GamePlay_Lv2.h"
#include "BackGround.h"

CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;
	


	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);

	if (nullptr == m_pLoader)
		return E_FAIL;


	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{

		

	if (true == m_pLoader->isFinished() && GetKeyState(VK_SPACE) & 0x8000)
	{
		
		
		CLevel*			pNewLevel = { nullptr };

		switch (m_eNextLevelID)
		{
		case LEVEL_LOGO:
			pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			break;

		case LEVEL_PROLOGUE:
			CSoundMgr::GetInstance()->SetBGMVolume(10.f);
 			pNewLevel = CLevel_Prologue::Create(m_pDevice, m_pContext);
			break;

		case LEVEL_GAMEPLAY:
			CSoundMgr::GetInstance()->EndBGM();
			//CSoundMgr::GetInstance()->StopEffectPlay();
			pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
			break;

		//case LEVEL_GAMEPLAY_LV2:
		//	pNewLevel = CLevel_GamePlay_Lv2::Create(m_pDevice, m_pContext);
		//	break;

		}

		if (nullptr == pNewLevel)
			return;

		if (FAILED(m_pGameInstance->Open_Level(m_eNextLevelID, pNewLevel)))
			return;
	}
}

HRESULT CLevel_Loading::Render()
{

#ifdef _DEBUG
	m_pLoader->Show_Debug();
#endif
	
	int b = m_pGameInstance->Get_CurrentLevel();
	int c = m_pGameInstance->Get_PreLevelID();
	int a = 0;


	if (m_pGameInstance->Get_PreLevelID() == 2)
	{

		if (true == m_pLoader->isFinished())
		{
			m_pGameInstance->Render_Font(TEXT("Cormorant_36_Sharp"), TEXT("Push Enter Key"), _float2(850, 630), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		}
		else
			m_pGameInstance->Render_Font(TEXT("Cormorant_36_Sharp"), TEXT("Loading"), _float2(860, 630), XMVectorSet(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_pGameInstance->Get_PreLevelID() == 3)
	{
		m_pGameInstance->Render_Font(TEXT("Cormorant_36_Sharp"), TEXT("- Shadow Corridor -"), _float2(45, 100), XMVectorSet(1.f, 1.f, 1.f, 1.f));

		m_pGameInstance->Render_Font(TEXT("Cormorant_26_Sharp"), TEXT("The Moment I touched the mirror, a sinister darkness engulfed the area around me."), _float2(45, 200), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Font(TEXT("Cormorant_26_Sharp"), TEXT("The alley I was in but a moment before was nowhere to be seen."), _float2(45, 240), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Font(TEXT("Cormorant_26_Sharp"), TEXT("As darkness enveloped everything in sight, maze-like corridors stretched out before me."), _float2(45, 280), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Font(TEXT("Cormorant_26_Sharp"), TEXT(" "), _float2(45, 320), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Font(TEXT("Cormorant_26_Sharp"), TEXT(" \" I need to get out of here... \" "), _float2(45, 360), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Font(TEXT("Cormorant_26_Sharp"), TEXT("The intrepid attitude I so innocently felt just an instant ago was gone,"), _float2(45, 400), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Font(TEXT("Cormorant_26_Sharp"), TEXT("replaced with a growing sense of anxiety and dread"), _float2(45, 440), XMVectorSet(1.f, 1.f, 1.f, 1.f));



		if (true == m_pLoader->isFinished())
		{
			m_pGameInstance->Render_Font(TEXT("Cormorant_36_Sharp"), TEXT("Push Enter Key"), _float2(850, 630), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		}
		else
			m_pGameInstance->Render_Font(TEXT("Cormorant_36_Sharp"), TEXT("Loading"), _float2(950, 630), XMVectorSet(1.f, 1.f, 1.f, 1.f));
	}


	

	m_pGameInstance->Render_End();

	return S_OK;
}




CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);

}
