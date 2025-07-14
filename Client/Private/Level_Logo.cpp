#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"


CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;	
	//if (FAILED(Ready_Layer_UI(TEXT("LAYER_UI"))))
	//	return E_FAIL;

	

	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{


	if (KEY_DOWN(KEY::DOWN) && m_pGameInstance->Get_CurrentLevel() == 2)
	{
		++m_iChooseUI;

		if (m_iChooseUI == 3)
		{
			m_iChooseUI = 0;
		}
	}
	else if (KEY_UP(KEY::UP))
	{
		--m_iChooseUI;

		if (m_iChooseUI == -1)
		{
			m_iChooseUI = 3;
		}
	}

	
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		//m_pGameInstance->Clear_UIObjects();//ui는 static에 두면 안됨 아니면 다 static에 박아두고 이거 뺴면됨
		
		
		//백업
		//m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY));
		//return; //-> 오픈 레벨하고 키 입력하면 터짐 왜지? 



		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_PROLOGUE));
		return;

	}

	

}

HRESULT CLevel_Logo::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	CBackGround::BACKGROUND_DESC		Desc{};
	Desc.iData = 10;
	Desc.fSpeedPerSec = 5.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		LEVEL_LOGO, strLayerTag, &Desc)))
		return E_FAIL;

	CSoundMgr::GetInstance()->StartBGM(BGM_TYPE::BGM_LOBBY);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UI(const _wstring strLayerTag)
{
	
	//CUIObject* pUIObject = { nullptr };
	//CUIObject::UIOBJECT_DESC UI_Desc = {};
	
	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
