#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "CubeCollider.h"
#include "Terrain.h"
#include "Level_Loading.h"
#include "Prop.h"
#include "Monster.h"
#include "Cabinet.h"
#include "Candle.h"
#include "HandLighter_Battery_Count.h"



vector<CInnerInventory*>* CProp::m_pInventory = nullptr;

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	//if (FAILED(Ready_Lights()))
	//	return E_FAIL;
	if (FAILED(Ready_ShadowCamera()))
		return E_FAIL;
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_GameObject(TEXT("Layer_GameObject"))))
		return E_FAIL;
	if (FAILED(Ready_Prop(TEXT("Layer_Props"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camara"))))
		return E_FAIL;
	if (FAILED(Ready_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;
	if (FAILED(Ready_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	if (FAILED(Ready_UseProps(TEXT("Layer_UseProps"))))
		return E_FAIL;
	if (FAILED(Ready_UI(TEXT("Layer_UI"))))
		return E_FAIL;
	if (FAILED(Ready_EndProp(TEXT("Layer_End"))))
		return E_FAIL;

	CGameObject* GameObject = { nullptr };
	m_pCamera_POV->Set_Player(m_pPlayer);
	m_pCryingGirl->Set_Player(m_pPlayer);
	m_pSpider->Set_Player(m_pPlayer);
	m_pMenMonster->Set_Player(m_pPlayer);
	CUI_Manager::GetInstance()->Set_Player(m_pPlayer); 
	CUI_Manager::GetInstance()->Set_Camera(m_pCamera_POV);
	m_pSkyBox->Set_Camera(m_pCamera_POV);

	CSoundMgr::GetInstance()->SetBGMVolume(50.f);
	CSoundMgr::GetInstance()->StartBGM(BGM_TYPE::BGM_GAMEPLAY , true);

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	m_pPlayer->Set_CameraLook(m_pCamera_POV->Get_Transform_For_Player()->Get_State(CTransform::STATE_LOOK));
	m_pPlayer->Set_CameraRight(m_pCamera_POV->Get_Transform_For_Player()->Get_State(CTransform::STATE_RIGHT));

	CUI_Manager::GetInstance()->Update(fTimeDelta);

	//if (true == CUI_Manager::GetInstance()->Get_NextLevelGo() || KEY_UP(KEY::ENTER))
	//{
	//	//m_pGameInstance->Clear_UIObjects();//ui는 static에 두면 안됨 아니면 다 static에 박아두고 이거 뺴면됨
	//	m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY_LV2));
	//	//CUI_Manager::GetInstance()->Set_NextLevelLevelOpen(false);
	//	return; //-> 오픈 레벨하고 키 입력하면 터짐 왜지? 
	//}

 	if (MOUSE_DOWN(MOUSE::DIM_LB) ) 
	{
		// 인터렉션하게
	}

	Update_For_ChromaticShader(fTimeDelta);


	_int iCheckCount = { 0 };
	_float fDis[3] = {};

	fDis[0] = XMVectorGetX(XMVector3Length(m_pPlayer->Get_Position() - m_pCryingGirl->Get_Transform()->Get_State(CTransform::STATE_POSITION)));
	fDis[1] = XMVectorGetX(XMVector3Length(m_pPlayer->Get_Position() - m_pMenMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION)));
	fDis[2] = XMVectorGetX(XMVector3Length(m_pPlayer->Get_Position() - m_pSpider->Get_Transform()->Get_State(CTransform::STATE_POSITION)));

	for (_int i = 0; i < 3; ++i)
	{
		if (fDis[i] < 35)
		{
			++iCheckCount;

			break;
		}
	}

	if (iCheckCount != 0)
	{
		CUI_Manager::GetInstance()->Set_PlayerHeartBeat(true);
	}

}

HRESULT CLevel_GamePlay::Render()
{
#ifdef _DEBUG

	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

#endif



	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring & strLayerTag)
{
	CGameObject* pGameObject;

	/* POV Cam*/
	CCamera_POV::CAMERA_POV_DESC POVDesc{};
	POVDesc.fSpeedPerSec	= 10.f;
	POVDesc.fRotationPerSec = XMConvertToRadians(90.f);
	POVDesc.fMouseSensor	= 0.1f;
	POVDesc.fFovy			= XMConvertToRadians(60.f);
	POVDesc.fAspect			= static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	POVDesc.fNear			= 0.1f;
	POVDesc.fFar			= 1000.f;
	POVDesc.vEye			= _float3(10.f, 10.f, 10.f);
	POVDesc.vAt				= _float3(0.f, 0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_POV"), LEVEL_GAMEPLAY, strLayerTag, &POVDesc, &pGameObject)))
		return E_FAIL;

	m_pCamera_POV = static_cast<CCamera_POV*>(pGameObject);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_ShadowCamera()
{
	m_pGameInstance->SetUp_Shadow_TransformMatrix(CShadow::D3DTS_VIEW, XMMatrixLookAtLH(XMVectorSet(-10.f, 50.f, -10.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	m_pGameInstance->SetUp_Shadow_TransformMatrix(CShadow::D3DTS_PROJ, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.f), static_cast<_float>(g_iWinSizeX) / g_iWinSizeY, 0.1f, 1000.f));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring & strLayerTag)
{
	CGameObject* pGameObject;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, strLayerTag, nullptr, &pGameObject)))
		return E_FAIL;

	m_pSkyBox = static_cast<CSky*>(pGameObject);
	if (nullptr != m_pTerrain)
	{
		m_pPos = m_pTerrain->Get_Vertices();
		m_VerticesX = m_pTerrain->Get_VerticesX();
		m_VerticesZ = m_pTerrain->Get_VerticesZ();
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_GameObject(const _wstring& strLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Prologue_Stage"), LEVEL_GAMEPLAY, strLayerTag)))
	//	return E_FAIL;


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Stage1"), LEVEL_GAMEPLAY, strLayerTag)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};
	
	ZeroMemory(&LightDesc, sizeof LightDesc);
	
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTOINAL;
	LightDesc.vDirection = _float4(-1.f, -1.f, -1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.TurnOn_Off = true;
	
	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Prop(const _wstring& strLayerTag)
{
	Read_Props_Desc_From_File();

	for (int i = 1; i <= M_SEMI_STAGE; ++i) 
	{
		for (int j = 1; j <= M_PROP_TYPES; ++j) 
		{
			int ivecSize = m_vecPropDescs[i][j].size();
			_tchar NowPropProtoTag[MAX_PATH] = L"";

			for (int k = 0; k < ivecSize; ++k)
			{
				switch (j)
				{
				case 0:
				{
					_tchar szBasePropTag[MAX_PATH] = TEXT("Prototype_GameObject_Lighter");
					wsprintf(NowPropProtoTag, szBasePropTag, j);


					if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, NowPropProtoTag, LEVEL_GAMEPLAY, strLayerTag, &m_vecPropDescs[i][j][k])))
						return E_FAIL;
				}
				break;

				case 1:
				{
					_tchar szBasePropTag[MAX_PATH] = TEXT("Prototype_GameObject_Mirror");
					wsprintf(NowPropProtoTag, szBasePropTag, j);


					if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, NowPropProtoTag, LEVEL_GAMEPLAY, strLayerTag, &m_vecPropDescs[i][j][k])))
						return E_FAIL;
				}
				break;

				case 2:
				{
					_tchar szBasePropTag[MAX_PATH] = TEXT("Prototype_GameObject_HandLighter");
					wsprintf(NowPropProtoTag, szBasePropTag, j);


					if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, NowPropProtoTag, LEVEL_GAMEPLAY, strLayerTag, &m_vecPropDescs[i][j][k])))
						return E_FAIL;
				}
				break;

				case 3:
				{
					_tchar szBasePropTag[MAX_PATH] = TEXT("Prototype_GameObject_GlowStone");
					wsprintf(NowPropProtoTag, szBasePropTag, j);


					if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, NowPropProtoTag, LEVEL_GAMEPLAY, strLayerTag, &m_vecPropDescs[i][j][k])))
						return E_FAIL;
				}
				break;

				case 4:
				{
					_tchar szBasePropTag[MAX_PATH] = TEXT("Prototype_GameObject_Key");
					wsprintf(NowPropProtoTag, szBasePropTag, j);


					if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, NowPropProtoTag, LEVEL_GAMEPLAY, strLayerTag, &m_vecPropDescs[i][j][k])))
						return E_FAIL;
				}
				break;

				case 5:
				{
					_tchar szBasePropTag[MAX_PATH] = TEXT("Prototype_GameObject_Spher");
					wsprintf(NowPropProtoTag, szBasePropTag, j);


					if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, NowPropProtoTag, LEVEL_GAMEPLAY, strLayerTag, &m_vecPropDescs[i][j][k])))
						return E_FAIL;
				}
				break;

				case 6:
				{
					_tchar szBasePropTag[MAX_PATH] = TEXT("Prototype_GameObject_Dynamite");
					wsprintf(NowPropProtoTag, szBasePropTag, j);


					if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, NowPropProtoTag, LEVEL_GAMEPLAY, strLayerTag, &m_vecPropDescs[i][j][k])))
						return E_FAIL;

				}
				break;

				case 7:
				{
					_tchar szBasePropTag[MAX_PATH] = TEXT("Prototype_GameObject_Door00");
					wsprintf(NowPropProtoTag, szBasePropTag, j);

					if (FAILED(Ready_CollisionProps(TEXT("Layer_CollisionProps"), &m_vecPropDescs[i][j][k], j)))
						return E_FAIL;
				}
				break;

				case 8:
				{
					_tchar szBasePropTag[MAX_PATH] = TEXT("Prototype_GameObject_Door01");
					wsprintf(NowPropProtoTag, szBasePropTag, j);

					CProp* pProp = nullptr;

					if (FAILED(Ready_CollisionProps(TEXT("Layer_CollisionProps"), &m_vecPropDescs[i][j][k], j)))
						return E_FAIL;
				}
				break;

				case 9:
				{
					_tchar szBasePropTag[MAX_PATH] = TEXT("Prototype_GameObject_Cabinet");
					wsprintf(NowPropProtoTag, szBasePropTag, j);

					if (FAILED(Ready_CabinetProps(TEXT("Layer_CabinetProps"), &m_vecPropDescs[i][j][k])))
						return E_FAIL;
				}
				break;

				case 10:
				{
					_tchar szBasePropTag[MAX_PATH] = TEXT("Prototype_GameObject_Candle");
					wsprintf(NowPropProtoTag, szBasePropTag, j);
					if (FAILED(Ready_CandleProps(TEXT("Layer_CandleProps"), &m_vecPropDescs[i][j][k])))
						return E_FAIL;
				}
				break;
				}
			}
		}
	}

	CProp::PROP_DESC Desc;

	Desc.vPosition = _float4(-103.47f, -1.75f, -98.66f, 1.f);
	Desc.iPropNo = 0;
	Desc.vScale = _float3(0.01f, 0.01f, 0.01f);
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.fSpeedPerSec = 1.f;
	Desc.fAngle = 90.f;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MagatamaProp"), LEVEL_GAMEPLAY, strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vPosition = _float4(62.46, -1.75f, 66.64, 1.f);
	Desc.iPropNo = 1;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MagatamaProp"), LEVEL_GAMEPLAY, strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vPosition = _float4(-80.15f, -1.75f, 159.11f, 1.f);
	Desc.iPropNo = 2;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MagatamaProp"), LEVEL_GAMEPLAY, strLayerTag, &Desc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Monster(const _wstring& strLayerTag)
{
	CMonster::MONSTER_DESC Desc{};
	CGameObject* pGameObject = { nullptr };
	Desc.fAngle = 0.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.fSpeedPerSec = 14.f;
	Desc.iData = 10;
	Desc.MonsterNo = 0;
	Desc.vPosition = _float4{ -103.861f, -6.75f, 19.081f, 1.f };
	Desc.vScale = _float3{ 0.1f, 0.1f, 0.1f };
	Desc.iIndex = 0;
	Desc.iNaviMeshIndex = 924;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_CryingGirl"), LEVEL_GAMEPLAY, strLayerTag, &Desc, &pGameObject)))
		return E_FAIL;
	m_pCryingGirl = static_cast<CryingGirl*>(pGameObject);
	
	Desc.MonsterNo = 1;
	Desc.vPosition = _float4{ 33.284f, -6.75f, -137.067f, 1.f };
	Desc.fSpeedPerSec = 15.f;
	Desc.iIndex = 1;
	Desc.iNaviMeshIndex = 298;
	
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_Spider"), LEVEL_GAMEPLAY, strLayerTag, &Desc, &pGameObject)))
		return E_FAIL;
	m_pSpider = static_cast<CSpider*>(pGameObject);
	
	Desc.MonsterNo = 2;
	Desc.fSpeedPerSec = 12.f;
	//Desc.fSpeedPerSec = 4.2f;
	Desc.vPosition = _float4{ 113.186f, -6.75f, 115.832f, 1.f };
	Desc.iIndex = 2;
	Desc.iNaviMeshIndex = 753;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_MenMonster"), LEVEL_GAMEPLAY, strLayerTag, &Desc, &pGameObject)))
		return E_FAIL;
	m_pMenMonster = static_cast<CMenMonster*>(pGameObject);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Player(const _wstring& strLayerTag)
{	
	CGameObject* pGameObject = { nullptr };
	

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, strLayerTag, nullptr, &pGameObject)))
		return E_FAIL;

	m_pPlayer = static_cast<CPlayer*>(pGameObject);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_UI(const _wstring& strLayerTag)
{
	CGameObject*				pGameObject;
	CUIObject::UIOBJECT_DESC	pDesc{};
	pDesc.fX		= 7;
	pDesc.fY		= 15;
	pDesc.fSizeX	= 48;
	pDesc.fSizeY	= 48;

	Set_InventoryUI(strLayerTag);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ChooseBox"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	m_pChooseBox = static_cast<CChooseBox*>(pGameObject);
	CUI_Manager::GetInstance()->Set_ChooseBox(m_pChooseBox);

	pDesc.fX		= g_iWinSizeX / 2;
	pDesc.fY		= g_iWinSizeY / 2;
	pDesc.fSizeX	= 10;
	pDesc.fSizeY	= 10;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MiddlePoint"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	m_pMiddlePoint = static_cast<CMiddlePoint*>(pGameObject);

	pDesc.fX = g_iWinSizeX / 2;
	pDesc.fY = g_iWinSizeY - g_iWinSizeY / 4;
	pDesc.fSizeX = 15;
	pDesc.fSizeY = 15;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_RunnerGauge"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, nullptr)))
		return E_FAIL;

	pDesc.fX = g_iWinSizeX - g_iWinSizeX / 9;
	pDesc.fY = g_iWinSizeY - g_iWinSizeY / 8;
	pDesc.fSizeX = 80;
	pDesc.fSizeY = 80;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HandLighter_Battery"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, nullptr)))
		return E_FAIL;
	
	pDesc.fX = g_iWinSizeX - g_iWinSizeX / 10.5;
	pDesc.fY = g_iWinSizeY - g_iWinSizeY / 10;
	pDesc.fSizeX = 40;
	pDesc.fSizeY = 15;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HandLighter_Battery_Count"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;

	CHandLighter_Battery_Count* pBatteryCount = static_cast<CHandLighter_Battery_Count*>(pGameObject);
	CUI_Manager::GetInstance()->BatteryCount(pBatteryCount);


	pDesc.fX = g_iWinSizeX - g_iWinSizeX / 10.5;
	pDesc.fY = g_iWinSizeY - g_iWinSizeY / 13;
	pDesc.fSizeX = 40;
	pDesc.fSizeY = 15;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HandLighter_Battery_Count"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;

	pBatteryCount = static_cast<CHandLighter_Battery_Count*>(pGameObject);
	CUI_Manager::GetInstance()->BatteryCount(pBatteryCount);

	pDesc.fX = g_iWinSizeX - g_iWinSizeX / 10.5;
	pDesc.fY = g_iWinSizeY - g_iWinSizeY / 18.8;
	pDesc.fSizeX = 40;
	pDesc.fSizeY = 15;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HandLighter_Battery_Count"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;

	pBatteryCount = static_cast<CHandLighter_Battery_Count*>(pGameObject);
	CUI_Manager::GetInstance()->BatteryCount(pBatteryCount);
	

	pDesc.fX = 15; /*g_iWinSizeX  - g_iWinSizeX / 50  ;*/
	pDesc.fY = g_iWinSizeY - g_iWinSizeY / 11;
	pDesc.fSizeX = 80;
	pDesc.fSizeY = 40;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Magatama_UI"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, nullptr)))
		return E_FAIL;
	

	pDesc.fX = g_iWinSizeX / 2;
	pDesc.fY = g_iWinSizeY / 2;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CenterWord_UI"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, nullptr)))
		return E_FAIL;

	pDesc.fX = 0;
	pDesc.fY = 0;
	pDesc.fSizeX = g_iWinSizeX;
	pDesc.fSizeY = g_iWinSizeY;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Dead_UI"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, nullptr)))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_CollisionProps(const _wstring& strLayerTag, CProp::PROP_DESC* _Desc, _int _Index)
{
	CGameObject* pGameObejct;
	

	/*for. InterActionProps*/

	switch (_Index)
	{
	case 7:
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Door00"), LEVEL_GAMEPLAY, strLayerTag, _Desc)))
			return E_FAIL;
	}
	break;

	case 8:
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Door01"), LEVEL_GAMEPLAY, strLayerTag, _Desc)))
			return E_FAIL;
	}
	break;
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_CabinetProps(const _wstring& strLayerTag, CProp::PROP_DESC* _Desc)
{
	CGameObject* pGameObejct;
	CCabinet* pCabinet;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Cabinet"), LEVEL_GAMEPLAY, strLayerTag, _Desc, &pGameObejct)))
		return E_FAIL;

	pCabinet = static_cast<CCabinet*>(pGameObejct);
	pCabinet->Set_Camera(m_pCamera_POV);

	Safe_Release(pGameObejct);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_CandleProps(const _wstring& strLayerTag, CProp::PROP_DESC* _Desc)
{
	CGameObject* pGameObejct;
	CCandle* pCabinet;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Candle"), LEVEL_GAMEPLAY, strLayerTag, _Desc, &pGameObejct)))
		return E_FAIL;

	pCabinet = static_cast<CCandle*>(pGameObejct);

	/* 라이트쪽 추가해야한다.*/

	Safe_Release(pGameObejct);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_UseProps(const _wstring& strLayerTag)
{

	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Compass_Use"), LEVEL_GAMEPLAY, strLayerTag, nullptr, nullptr)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Compass_Plate"), LEVEL_GAMEPLAY, strLayerTag, nullptr, nullptr)))
	//	return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mirror_Use"), LEVEL_GAMEPLAY, strLayerTag, nullptr, nullptr)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_EndProp(const _wstring& strLayerTag)
{
	CProp::PROP_DESC Desc;

	Desc.vPosition = _float4{287.25f, -6.f,  46.96, 1.f};
	Desc.vScale = _float3{ 0.03f, 0.03f, 0.03f };
	Desc.iPropNo = 0;
	Desc.fAngle = XMConvertToRadians(270.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_EndProp"), LEVEL_GAMEPLAY, strLayerTag, &Desc, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Update_For_ChromaticShader(_float fTimeDelta)
{
	_int iCheckCount = { 0 };
	_float fDis[3] = {};
	
	fDis[0] = XMVectorGetX(XMVector3Length(m_pPlayer->Get_Position() - m_pCryingGirl->Get_Transform()->Get_State(CTransform::STATE_POSITION)));
	fDis[1] = XMVectorGetX(XMVector3Length(m_pPlayer->Get_Position() - m_pMenMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION)));
	fDis[2] = XMVectorGetX(XMVector3Length(m_pPlayer->Get_Position() - m_pSpider->Get_Transform()->Get_State(CTransform::STATE_POSITION)));

	for (_int i = 0; i < 3 ; ++i)
	{
		if (fDis[i] < 28)
		{
			++iCheckCount;

			break;
		}
	}

	if (iCheckCount != 0 && m_fChromaticCoolTime <= 10.f)
	{
		m_pGameInstance->Set_GlitchShader(true);
		m_fChromaticTime += fTimeDelta;

		if (m_fChromaticTime >= 1.3f)
		{
			m_pGameInstance->Set_GlitchShader(false);
			m_fChromaticCoolTime += fTimeDelta;
		}
	}
	else
	{
		m_pGameInstance->Set_GlitchShader(false);
		m_fChromaticTime = 0.f;
		m_fChromaticCoolTime += fTimeDelta;
	}
	
	if (m_fChromaticCoolTime > 10.f)
	{
		m_fChromaticCoolTime = 0.f;
		m_fChromaticTime = 0.f;
	}

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);
	CUI_Manager::GetInstance()->Delete_Inventory();
	CUI_Manager::GetInstance()->Delete_Camera();
	CUI_Manager::GetInstance()->Delete_Player();
	

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	for (auto iter = m_vecCubeCollider.begin(); iter != m_vecCubeCollider.end(); ++iter)
	{
		Safe_Release(*iter);
	}

	//Safe_Delete_Array(m_pPos);
	Safe_Release(m_pTerrain);
	Safe_Release(m_pCamera_POV);
	Safe_Release(m_pInventory);
	Safe_Release(m_pPlayer);
	Safe_Release(m_pChooseBox);
	Safe_Release(m_pLigher);
	Safe_Release(m_pMiddlePoint);
	Safe_Release(m_pCryingGirl);
	Safe_Release(m_pSpider);
	Safe_Release(m_pMenMonster);
	Safe_Release(m_pSkyBox);
	

	__super::Free();
}


HRESULT CLevel_GamePlay::Set_InventoryUI(const _wstring& strLayerTag)
{
	CGameObject* pGameObject = { nullptr };

	CUIObject::UIOBJECT_DESC		pDesc{};
	pDesc.fX = 10;
	pDesc.fY = 20;
	pDesc.fSizeX = 40;
	pDesc.fSizeY = 40;

	CInnerInventory* Inventory0 = { nullptr };
	CInnerInventory* Inventory1 = { nullptr };
	CInnerInventory* Inventory2 = { nullptr };
	CInnerInventory* Inventory3 = { nullptr };
	CInnerInventory* Inventory4 = { nullptr };
	CInnerInventory* Inventory5 = { nullptr };
	CInnerInventory* Inventory6 = { nullptr };
	CInnerInventory* Inventory7 = { nullptr };
	CInnerInventory* Inventory8 = { nullptr };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Inventory"), LEVEL_GAMEPLAY, strLayerTag, nullptr, &pGameObject)))
		return E_FAIL;
	m_pInventory = static_cast<CInventory*>(pGameObject);


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory0"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	Inventory0 = static_cast<CInnerInventory*>(pGameObject);
	
	pDesc.fX = 60;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory1"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	Inventory1 = static_cast<CInnerInventory*>(pGameObject);

	pDesc.fX = 110;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory2"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	Inventory2 = static_cast<CInnerInventory*>(pGameObject);
	
	pDesc.fX = 160;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory3"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	Inventory3 = static_cast<CInnerInventory*>(pGameObject);
	
	pDesc.fX = 210;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory4"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	Inventory4 = static_cast<CInnerInventory*>(pGameObject);

	pDesc.fX = 260;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory5"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	Inventory5 = static_cast<CInnerInventory*>(pGameObject);
	
	pDesc.fX = 310;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory6"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	Inventory6 = static_cast<CInnerInventory*>(pGameObject);
	
	pDesc.fX = 360;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory7"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	Inventory7 = static_cast<CInnerInventory*>(pGameObject);
	
	pDesc.fX = 410;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory8"), LEVEL_GAMEPLAY, strLayerTag, &pDesc, &pGameObject)))
		return E_FAIL;
	Inventory8 = static_cast<CInnerInventory*>(pGameObject);

	CUI_Manager::GetInstance()->Set_InventoryObj(Inventory0);
	CUI_Manager::GetInstance()->Set_InventoryObj(Inventory1);
	CUI_Manager::GetInstance()->Set_InventoryObj(Inventory2);
	CUI_Manager::GetInstance()->Set_InventoryObj(Inventory3);
	CUI_Manager::GetInstance()->Set_InventoryObj(Inventory4);
	CUI_Manager::GetInstance()->Set_InventoryObj(Inventory5);
	CUI_Manager::GetInstance()->Set_InventoryObj(Inventory6);
	CUI_Manager::GetInstance()->Set_InventoryObj(Inventory7);
	CUI_Manager::GetInstance()->Set_InventoryObj(Inventory8);

	Safe_Release(Inventory0);
	Safe_Release(Inventory1);
	Safe_Release(Inventory2);
	Safe_Release(Inventory3);
	Safe_Release(Inventory4);
	Safe_Release(Inventory5);
	Safe_Release(Inventory6);
	Safe_Release(Inventory7);
	Safe_Release(Inventory8);

	return S_OK;
}

HRESULT CLevel_GamePlay::Read_Props_Desc_From_File()
{

	for (int i = 1; i <= M_SEMI_STAGE; ++i)
	{
		HANDLE hFile = CreateFile(m_DescPathes[i], GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			MSG_BOX("Failed To Read Prop Desc Checkk Pathes Again");
			return E_FAIL;
		}

		_ulong dwByte = { 0 };
		_uint iNumPropTypes = { 0 };
		ReadFile(hFile, &iNumPropTypes, sizeof(_uint), &dwByte, nullptr);
		vector<_uint>m_NumPropsPerTypes;

		m_NumPropsPerTypes.resize(iNumPropTypes + 1);

		for (int j = 1; j <= iNumPropTypes; ++j)
			ReadFile(hFile, &m_NumPropsPerTypes[j], sizeof(_uint), &dwByte, nullptr);

		for (int j = 1; j <= iNumPropTypes; ++j)
		{
			for (int k = 0; k < m_NumPropsPerTypes[j]; ++k)
			{
				CProp::PROP_DESC desc = {};

				ReadFile(hFile, &desc.vScale, sizeof(_float3), &dwByte, nullptr);
				ReadFile(hFile, &desc.fAngle, sizeof(_float), &dwByte, nullptr);
				ReadFile(hFile, &desc.vPosition, sizeof(_float4), &dwByte, nullptr);
				ReadFile(hFile, &desc.iNaviIndex, sizeof(_int), &dwByte, nullptr);
				desc.fRotationPerSec = XMConvertToRadians(90.f);
				desc.fSpeedPerSec = 10.f;
				m_vecPropDescs[i][j].push_back(desc);
			}
		}
		CloseHandle(hFile);
	}

	return S_OK;
}