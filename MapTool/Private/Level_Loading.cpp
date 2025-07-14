#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "Loader.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_GamePlay.h"

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
	if (true == m_pLoader->isFinished() &&
		GetKeyState(VK_SPACE) & 0x8000)
	{
		
		CLevel*			pNewLevel = { nullptr };

		switch (m_eNextLevelID)
		{
		case LEVEL_LOGO:
			pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_GAMEPLAY:
			pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
			break;
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
