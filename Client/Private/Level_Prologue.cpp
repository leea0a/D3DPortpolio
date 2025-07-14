#include "stdafx.h"
#include "Level_Prologue.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"


CLevel_Prologue::CLevel_Prologue(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Prologue::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camara"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_GameObject(TEXT("Layer_GameObject"))))
		return E_FAIL;
	if (FAILED(Ready_Props(TEXT("Layer_Props"))))
		return E_FAIL;
	if (FAILED(Ready_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	if (FAILED(Ready_CollisionProps(TEXT("Layer_CollisionProps"))))
		return E_FAIL;
	if (FAILED(Ready_LevelProps(TEXT("Layer_LevelProps"))))
		return E_FAIL;
	if (FAILED(Ready_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	m_pCamera_POV->Set_Player(m_pPlayer);
	CUI_Manager::GetInstance()->Set_Player(m_pPlayer);
	CUI_Manager::GetInstance()->Set_Camera(m_pCamera_POV);
	m_pSkyBox->Set_Camera(m_pCamera_POV);
	CSoundMgr::GetInstance()->Set_EffectVolume(14.f, SFX_TYPE::SFX_Prologue);
	CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_Prologue, true);

	return S_OK;
}

void CLevel_Prologue::Update(_float fTimeDelta)
{
	

	m_pPlayer->Set_CameraLook(m_pCamera_POV->Get_Transform_For_Player()->Get_State(CTransform::STATE_LOOK));
	m_pPlayer->Set_CameraRight(m_pCamera_POV->Get_Transform_For_Player()->Get_State(CTransform::STATE_RIGHT));
	CUI_Manager::GetInstance()->Update(fTimeDelta);

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_Prologue);
		
		//m_pGameInstance->Clear_UIObjects();//ui는 static에 두면 안됨 아니면 다 static에 박아두고 이거 뺴면됨
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LEVELPROP, false);
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY));
		return; //-> 오픈 레벨하고 키 입력하면 터짐 왜지? 
	}

	if (CUI_Manager::GetInstance()->Get_NextLevelGo() == true)
	{
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY));

	}
}

HRESULT CLevel_Prologue::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("프롤로그입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Prologue::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CGameObject* pGameObject;

	/* POV Cam*/
	CCamera_POV::CAMERA_POV_DESC POVDesc{};
	POVDesc.fSpeedPerSec = 10.f;
	POVDesc.fRotationPerSec = XMConvertToRadians(90.f);
	POVDesc.fMouseSensor = 0.1f;
	POVDesc.fFovy = XMConvertToRadians(60.f);
	POVDesc.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	POVDesc.fNear = 0.1f;
	POVDesc.fFar = 1000.f;
	POVDesc.vEye = _float3(10.f, 10.f, 10.f);
	POVDesc.vAt = _float3(0.f, 0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Camera_POV"), LEVEL_PROLOGUE, strLayerTag, &POVDesc, &pGameObject)))
		return E_FAIL;

	m_pCamera_POV = static_cast<CCamera_POV*>(pGameObject);

	return S_OK;
}

HRESULT CLevel_Prologue::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	CGameObject* pGameObject;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Sky"), LEVEL_PROLOGUE, strLayerTag, nullptr, &pGameObject)))
		return E_FAIL;

	m_pSkyBox = static_cast<CSky*>(pGameObject);

	return S_OK;
}

HRESULT CLevel_Prologue::Ready_Layer_GameObject(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Prologue_Stage"), LEVEL_PROLOGUE, strLayerTag)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Prologue::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_SPOT;
	LightDesc.vDiffuse = _float4(3.92f, 2.32f, 0.6f, 1.f);
	LightDesc.vAmbient = _float4(0.32f, 0.86f, 0.32f, 1.f);
	LightDesc.vSpecular = _float4(0.86f, 0.39f, 0.08f, 1.f);
	LightDesc.fRange = _float(8.7f);
	LightDesc.vPosition = _float4(20.414f, -5.25f, 19.667f, 1.f);
	LightDesc.vDirection = _float4(0.f, 1.f, 0.f, 0.f);
	LightDesc.TurnOn_Off = true;
	LightDesc.fConeAngle = 30.f;
	LightDesc.fAttenuation = 1.f;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_SPOT;
	LightDesc.vDiffuse = _float4(3.f, 3.f, 3.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.fRange = _float(25.f);
	LightDesc.vPosition = _float4(-11.467f, 0.628f, 22.040f, 1.f);
	LightDesc.vDirection = _float4(0.f, 1.f, 0.f, 0.f);
	LightDesc.TurnOn_Off = true;
	LightDesc.fConeAngle = 60.f;
	LightDesc.fAttenuation = 2.f;
	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_SPOT;
	LightDesc.vDiffuse = _float4(3.f, 3.f, 3.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.fRange = _float(25.f);
	LightDesc.vPosition = _float4(-37.182f, 0.552f, -2.087f, 1.f);
	LightDesc.vDirection = _float4(0.f, 1.f, 0.f, 0.f);
	LightDesc.TurnOn_Off = true;
	LightDesc.fConeAngle = 60.f;
	LightDesc.fAttenuation = 2.f;
	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(2.0f, 2.f, 2.f, 1.f);
	LightDesc.vAmbient = _float4(0.25f, 0.25f, 0.25f, 1.f);
	LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.fRange = _float(21.f);
	LightDesc.vPosition = _float4(3.40f, -8.f, -6.04f, 1.f);
	LightDesc.TurnOn_Off = true;
	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(2.0f, 2.f, 2.f, 1.f);
	LightDesc.vAmbient = _float4(0.25f, 0.25f, 0.25f, 1.f);
	LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.fRange = _float(21.f);
	LightDesc.vPosition = _float4(-41.65f, -1.39f, 45.71f, 1.f);
	LightDesc.TurnOn_Off = true;
	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.0f, 0.95f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.25f, 0.25f, 0.25f, 1.f);
	LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.fRange = _float(80.f);
	//LightDesc.fRange = _float(100.f);
	LightDesc.vPosition = _float4(18.19f, 3.03f, 42.11f, 1.f);
	LightDesc.TurnOn_Off = true;
	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Prologue::Ready_UI(const _wstring& strLayerTag)
{
	CGameObject* pGameObject = { nullptr };
	CUIObject::UIOBJECT_DESC		pDesc{};
	pDesc.fX = 10;
	pDesc.fY = 20;
	pDesc.fSizeX = 40;
	pDesc.fSizeY = 40;

	CInnerInventory* Inventory0 = { nullptr };
	CInnerInventory* Inventory1 = { nullptr };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Inventory"), LEVEL_PROLOGUE, strLayerTag, nullptr, &pGameObject)))
		return E_FAIL;
	m_pInventory = static_cast<CInventory*>(pGameObject);


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_InnerInventory0"), LEVEL_PROLOGUE, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;

	Inventory0 = static_cast<CInnerInventory*>(pGameObject);
	CUI_Manager::GetInstance()->Set_InventoryObj(Inventory0);



	pDesc.fX = 60;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_InnerInventory1"), LEVEL_PROLOGUE, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	Inventory1 = static_cast<CInnerInventory*>(pGameObject);
	CUI_Manager::GetInstance()->Set_InventoryObj(Inventory1);



	pDesc.fX = 7;
	pDesc.fY = 15;
	pDesc.fSizeX = 48;
	pDesc.fSizeY = 48;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_ChooseBox"), LEVEL_PROLOGUE, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	m_pChooseBox = static_cast<CChooseBox*>(pGameObject);
	CUI_Manager::GetInstance()->Set_ChooseBox(m_pChooseBox);

	pDesc.fX = g_iWinSizeX / 2;
	pDesc.fY = g_iWinSizeY / 2;
	pDesc.fSizeX = 10;
	pDesc.fSizeY = 10;

	pDesc.fX = g_iWinSizeX / 2;
	pDesc.fY = g_iWinSizeY / 2;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_CenterWord_UI"), LEVEL_PROLOGUE, strLayerTag, &pDesc, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Prologue::Ready_Player(const _wstring& strLayerTag)
{
	CGameObject* pGameObject = { nullptr };


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Player"), LEVEL_PROLOGUE, strLayerTag, nullptr, &pGameObject)))
		return E_FAIL;

	m_pPlayer = static_cast<CPlayer*>(pGameObject);

	return S_OK;
}

HRESULT CLevel_Prologue::Ready_Props(const _wstring& strLayerTag)
{
	CProp::PROP_DESC Desc;

	Desc.vPosition = _float4{ 20.414f, -7.565f, 19.667f, 1.f };
	Desc.vScale = _float3{ 0.03f, 0.03f, 0.03f };
	Desc.iPropNo = 0;
	Desc.fAngle = XMConvertToRadians(270.f);


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Lighter"), LEVEL_PROLOGUE, strLayerTag, &Desc, nullptr)))
		return E_FAIL;


	Desc.vPosition = _float4{ 19.817f, -2.8f, -72.658f, 1.f };
	Desc.vScale = _float3{ 0.03f, 0.03f, 0.03f };
	Desc.iPropNo = 0;
	Desc.fAngle = XMConvertToRadians(270.f);


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_TeleportProp"), LEVEL_PROLOGUE, strLayerTag, &Desc, nullptr)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Prologue::Ready_CollisionProps(const _wstring& strLayerTag)
{
	CProp::PROP_DESC Desc;

	//Desc.vPosition = _float4{ 29.911f, -2.80f, -69.582f, 1.f };

	Desc.vPosition = _float4{ 35.293f, -2.874f, -53.729f, 1.f };
	Desc.vScale = _float3{ 0.03f, 0.03f, 0.03f };
	Desc.iPropNo = 0;
	Desc.fAngle = XMConvertToRadians(270.f);


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_StopProp"), LEVEL_PROLOGUE, strLayerTag, &Desc, nullptr)))
		return E_FAIL;



	//Desc.vPosition = _float4{ 63.486, -2.606, -99.566, 1.f };
	//Desc.vScale = _float3{ 0.03f, 0.03f, 0.03f };
	//Desc.iPropNo = 0;
	//Desc.fAngle = XMConvertToRadians(270.f);
	//
	//
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_LevelProp"), LEVEL_PROLOGUE, strLayerTag, &Desc, nullptr)))
	//	return E_FAIL;
	//
	

	return S_OK;
}

HRESULT CLevel_Prologue::Ready_LevelProps(const _wstring& strLayerTag)
{
	CProp::PROP_DESC Desc;

	Desc.vPosition = _float4{ 19.817f, -2.8f, -72.658f, 1.f };
	Desc.vScale = _float3{ 0.03f, 0.03f, 0.03f };
	Desc.iPropNo = 0;
	Desc.fAngle = XMConvertToRadians(270.f);


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_LevelProp"), LEVEL_PROLOGUE, strLayerTag, &Desc, nullptr)))
		return E_FAIL;

	return S_OK;
}


CLevel_Prologue * CLevel_Prologue::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{

	CLevel_Prologue*	pInstance = new CLevel_Prologue(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Prologue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Prologue::Free()
{
	Safe_Release(m_pPlayer);
	Safe_Release(m_pCamera_POV);
	Safe_Release(m_pInventory);
	Safe_Release(m_pChooseBox);
	Safe_Release(m_pLigher);
	Safe_Release(m_pSkyBox);

	//CUI_Manager::GetInstance()->Delete_Navigation();
	m_pGameInstance->Delete_Light();
	

	__super::Free();


}
