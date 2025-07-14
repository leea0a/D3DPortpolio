#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"

/*For GameLogo*/
#include "BackGround.h"

/*for GamePlay*/
#include "Camera_Free.h"
#include "Camera_POV.h"
#include "Terrain.h"
#include "Player.h"
#include "CubeCollider.h"

/*for Monster*/
#include "CryingGirl.h"
#include "Spider.h"
#include "MenMonster.h"

/*for Stage*/
#include "Prologue.h"
#include "Stage1.h"

/*for UI*/
#include "Inventory.h"
#include "InnerInventory.h"
#include "ChooseBox.h"
#include "MiddlePoint.h"
#include "RunngerGauge.h"
#include "HandLighter_Battery.h"
#include "HandLighter_Battery_Count.h"
#include "Magatama.h"
#include "CenterWord.h"
#include "Dead.h"

/*For Prop*/
#include "Prop.h"

/*For. ItemProps*/
#include "Lighter.h"
#include "Mirror.h"
#include "HandLighter.h"
//#include "Compass.h"
//#include "Kagura.h"
#include "GlowStone.h"
#include "Key.h"
#include "Spher.h"
#include "Dynamite.h"
#include "Dynamite_Use.h"
#include "LightStone.h"
#include "MagatamaProp.h"


/*For. InterActionProps*/
#include "Door00.h"
#include "Door01.h"
#include "Cabinet.h"
#include "Candle.h"
#include "Candle_Fire.h"
#include "EndProp.h"

/* For. TeleportProp*/
#include "TeleportProp.h"

/* For. LevelProp*/
#include "LevelProp.h"

/* For. StopProp*/
#include "StopProp.h"

/* POV_Prop*/
#include "Compass_Use.h"
#include "Compass_Plate.h"

#include "Mirror_Use.h"

/*For. Sky*/
#include "Sky.h"


/* For. Effect*/
#include "Particle_Spark.h"
#include "Particle_SparkLv2.h"

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);

}

//typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);
_uint APIENTRY LoadingMain(void* pArg)
{	
	CLoader*	pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_Critical_Section);

	/* 내 코드를 읽어나갈수 있는 하나의 스레드를 생성한다. */
	/* 생성한 스레드가 호출해야할 진입점함수를 지정해준다. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);

	if (0 == m_hThread)
		return E_FAIL;



	return S_OK;
}

HRESULT CLoader::Loading()
{
	/*For.LOGO*/



	EnterCriticalSection(&m_Critical_Section);

	CoInitializeEx( nullptr, 0);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_Level_Logo();
		break;

	case LEVEL_PROLOGUE:
		hr = Loading_Level_Prologue();
		break;

	case LEVEL_GAMEPLAY:
		hr = Loading_Level_GamePlay();
		break;

	case LEVEL_GAMEPLAY_LV2:
		hr = Loading_Level_GamePlay_Lv2();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}

#ifdef _DEBUG

void CLoader::Show_Debug()
{
	SetWindowText(g_hWnd, TEXT("Shadow Corridor"));
}

#endif

HRESULT CLoader::Loading_Level_Logo()
{

	m_pGameInstance->Set_CurrentLevel(1);
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/*For Mouse*/
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Mouse"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/UI/Mouse.png"), 1))))
	//	return E_FAIL;

	/*For.LOGO*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_StartLogo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Icon/213.png"), 1))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("사운드를 로딩중입니다."));
	

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));
	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;	

	lstrcpy(m_szLoadingText, TEXT("모델(을)를 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	

	lstrcpy(m_szLoadingText, TEXT("객체원형(을)를 로딩중입니다."));
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_Mouse"),
	//	CMouse::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Level_Prologue()
{
	m_pGameInstance->Set_CurrentLevel(2);
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Texture_SkyBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/SkyBox/SkyBox.dds"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Texture_UI_Item"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/UI_0%d.png"), 9))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Texture_UI_ChooseBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/ChooseBox.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Texture_UI_RunnerGauge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/RunnerGauge.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Texture_UI_CenterWord"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/NotAlpha.png"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX::Elements, VTXCUBETEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/*For Cube Collider*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxCubeCol"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCubeCol.hlsl"), VTXCUBECOL::Elements, VTXCUBECOL::iNumElements))))
		return E_FAIL;

	/*For AnimModel*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxRectInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxRectInstance.hlsl"), VTXRECTINSTANCE::Elements, VTXRECTINSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxPointInstance.hlsl"), VTXPOINTINSTANCE::Elements, VTXPOINTINSTANCE::iNumElements))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("콜라이더(을)를 로딩중입니다."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델(을)를 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	_matrix PreTransformMatrix = XMMatrixIdentity();

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Default/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	/*For Orototype_Component_VIBuffer_Cube*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/*for.Prototype_Component_VIBuffer_Collider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_VIBuffer_Collider"),
		CVIBuffer_Collider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For Prototype_Component_VIBuffer_Flat*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_VIBuffer_Flat"),
		CVIBuffer_Flat::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("맵을 로딩중입니다."));


	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Model_Prologue_Stage"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/Stage/Stage1/Stage1.fbx", PreTransformMatrix,
			nullptr, false))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("프랍을 로딩중입니다."));

	_matrix PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.f));

	/* for.itemProp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Model_Lighter"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Zippo.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	 PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(270.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Model_LevelProp"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/MaturiKagami.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("네비게이션을 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_Component_Navigation"),		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Data/Navigation_Clinet_Prologue.dat")))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("객체원형(을)를 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/*For. Prototype_GameObject_CubeCollider*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_CubeCollider"), CCubeCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Camera_Free"), CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Camera_POV"), CCamera_POV::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Stage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Prologue_Stage"), CPrologue::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Player*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Sky"), CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Inventory"), CInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_InnerInventory0"), CInnerInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_InnerInventory1"), CInnerInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_CenterWord_UI"), CCenterWord::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_ChooseBox"), CChooseBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MiddlePoint"), CMiddlePoint::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_RunnerGauge"), CRunnerGauge::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_Lighter"), CLighter::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For. TeleportProp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_TeleportProp"), CTeleportProp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For. LevelProp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_LevelProp"), CLevelProp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. StopProp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_PROLOGUE, TEXT("Prototype_GameObject_StopProp"), CStopProp::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;
	return S_OK;
}








HRESULT CLoader::Loading_Level_GamePlay()
{
	//CUI_Manager::GetInstance()->Delete_Navigation();
	m_pGameInstance->Delete_PlayerAllLight();

	m_pGameInstance->Set_CurrentLevel(3);
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Default/Textures/Terrain/Tile0.dds"), 1))))
		return E_FAIL;


  	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SkyBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/SkyBox/SkyBox.dds"), 1))))
		return E_FAIL;

	/*for.Prototype_Component_Texture_UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Item"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/UI_0%d.png"), 9))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ChooseBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/ChooseBox.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MiddlePoint"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Reticle_Dot.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_RunnerGauge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/RunnerGauge.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HandLighter_Battery"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Icon_Battery.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HandLighter_BatteryCount"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BatteryCount.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Magatama"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Icon_Magatama_IsHave.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_CenterWord"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/NotAlpha.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Dead"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Icon/Dead_1.png"), 1))))
		return E_FAIL;

	/* For Effect*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_Smoke"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/T_Smoke_Tiled_D.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_Spark"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Spark_Core_2.png"), 1))))
		return E_FAIL;




	lstrcpy(m_szLoadingText, TEXT("사운드를 로딩중입니다."));

		

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX::Elements, VTXCUBETEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/*For Cube Collider*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeCol"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCubeCol.hlsl"), VTXCUBECOL::Elements, VTXCUBECOL::iNumElements))))
		return E_FAIL;

	/*For AnimModel*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxRectInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxRectInstance.hlsl"), VTXRECTINSTANCE::Elements, VTXRECTINSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxPointInstance.hlsl"), VTXPOINTINSTANCE::Elements, VTXPOINTINSTANCE::iNumElements))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("콜라이더(을)를 로딩중입니다."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델(을)를 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//_matrix PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(270.f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LevelProp"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/MaturiKagami.fbx", PropPreTransformMatrix,
	//		nullptr, true))))
	//	return E_FAIL;
	
	_matrix PreTransformMatrix = XMMatrixIdentity();
	
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Default/Textures/Terrain/Height.bmp")))))
	//	return E_FAIL;

	/*For Orototype_Component_VIBuffer_Cube*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/*for.Prototype_Component_VIBuffer_Collider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Collider"),
		CVIBuffer_Collider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For Prototype_Component_VIBuffer_Flat*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Flat"),
		CVIBuffer_Flat::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/*For Monster Mesh*/
	//PreTransformMatrix = XMMatrixScaling(0.0f, 0.01f, 0.01f);
	 PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f);

	/* NON ANIM 예시*/
	//CModel::Make_Binary_NonAnim(CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/Stage/Stage1/Stage1.fbx", TEXT("../Bin/resources/Models/Data/Stage/Stage1.dat"), PreTransformMatrix);
	//CModel::Make_Binary_NonAnim(CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/Stage/Stage2/Stage2.fbx", TEXT("../Bin/resources/Models/Data/Stage/Stage2.dat"), PreTransformMatrix);

	/* ANIM 예시*/
	//CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/CryingGirl/CryingGirl.fbx", TEXT("../Bin/resources/Models/Data/Anim/CryingGirl.dat"), PreTransformMatrix);
	//CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/KimonoMonster/KimonoMonster.fbx", TEXT("../Bin/resources/Models/Data/Anim/KimonoMonster.dat"), PreTransformMatrix);
	//CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/MenMonster/MenMonster.fbx", TEXT("../Bin/resources/Models/Data/Anim/MenMonster.dat"), PreTransformMatrix);
	//CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/Spider/Spider.fbx", TEXT("../Bin/resources/Models/Data/Anim/Spider.dat"), PreTransformMatrix);



	/* Effect */
	/* Spark*/

	CVIBuffer_Particle_Instance::PARTICLE_DESC			ExplosionDesc{};
	ExplosionDesc.iNumInstances = 3;
	ExplosionDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ExplosionDesc.vRange = _float3(0.01f, 0.01f, 0.01f);
	ExplosionDesc.vSpeed = _float2(4.f, 9.f);
	ExplosionDesc.vLifeTime = _float2(0.1f, 0.5f);
	ExplosionDesc.vSize = _float2(0.1f, 0.3f);
	//ExplosionDesc.vSize = _float2(0.2f, 0.75f);
	ExplosionDesc.vPivot = _float3(0.0f, -1.f, 0.f);
	ExplosionDesc.isLoop = true;
	ExplosionDesc.isShow = true;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Particle_Explosion_Lv1"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, ExplosionDesc))))
		return E_FAIL;






	//CVIBuffer_Particle_Instance::PARTICLE_DESC			ExplosionDesc{};
	ExplosionDesc.iNumInstances = 3;
	ExplosionDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ExplosionDesc.vRange = _float3(0.0f, -0.5f, 0.0f);
	ExplosionDesc.vSpeed = _float2(4.f, 9.f);
	ExplosionDesc.vLifeTime = _float2(0.1f, 0.5f);
	ExplosionDesc.vSize = _float2(0.5f, 0.7f);
	//ExplosionDesc.vSize = _float2(0.2f, 0.75f);
	ExplosionDesc.vPivot = _float3(0.0f, -1.f, -1.f);
	ExplosionDesc.isLoop = true;
	ExplosionDesc.isShow = false;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Particle_Explosion_Lv2"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, ExplosionDesc))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("몬스터를 로딩중입니다."));

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CryingGirl"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/CryingGirl/CryingGirl.fbx", PreTransformMatrix,
	//	TEXT("../Bin/resources/Models/Data/Anim/CryingGirl.dat"), true))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_KimonoMonster"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/KimonoMonster/KimonoMonster.fbx", PreTransformMatrix,
	//		TEXT("../Bin/resources/Models/Data/Anim/KimonoMonster.dat"), true))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MenMonster"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/MenMonster/MenMonster.fbx", PreTransformMatrix,
	//		TEXT("../Bin/resources/Models/Data/Anim/MenMonster.dat"), true))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Spider"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/Spider/Spider.fbx", PreTransformMatrix,
	//		TEXT("../Bin/resources/Models/Data/Anim/Spider.dat"), true))))
	//	return E_FAIL;

	

	PreTransformMatrix = XMMatrixScaling(0.4f, 0.4f, 0.4f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CryingGirl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/CryingGirl/CryingGirl.fbx", PreTransformMatrix,
			nullptr, false))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_KimonoMonster"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/KimonoMonster/KimonoMonster.fbx", PreTransformMatrix,
			nullptr, false))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.45f, 0.45f, 0.45f) * XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixTranslation(18.f, 0.f, 0.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MenMonster"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/MenMonster/MenMonster.fbx", PreTransformMatrix,
			nullptr, false))))
		return E_FAIL;


	PreTransformMatrix = XMMatrixIdentity();

	PreTransformMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationZ(XMConvertToRadians(90.f)) * XMMatrixTranslation(-25.f, 0.f, -110.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Spider"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM, "../Bin/Resources/Models/Anim/Monster/Spider/Spider.fbx", PreTransformMatrix,
			nullptr, false))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);



	lstrcpy(m_szLoadingText, TEXT("맵을 로딩중입니다."));


	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Prologue_Stage"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/Stage/Stage1/Stage1.fbx", PreTransformMatrix,
	//	nullptr, false))))
	//	return E_FAIL;


	PreTransformMatrix = XMMatrixScaling(0.028f, 0.028f, 0.028f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stage1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/Stage/Stage2/Stage2.fbx", PreTransformMatrix,
			nullptr, false))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("프랍을 로딩중입니다."));

	_matrix PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.f));

	/* for.itemProp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Lighter"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Zippo.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Mirror"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/HandMirror.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_HandLighter"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/HandLight.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixRotationX(XMConvertToRadians(270.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Magatama"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Magatama.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MagatamaBase"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/MagatamaBase.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;


	/* Compass */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Compass"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Compass.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Compass_Plate"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Compas_Plate.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	//
	/* Kagura */ // Anim 
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Kagura"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Zippo.fbx", PropPreTransformMatrix,
	//		nullptr, true))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_GlowStone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/SmallStonePochi.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Key"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Key.fbx", PropPreTransformMatrix,
	//		nullptr, true))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TimeSpire"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/TimeSpire.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dynamite"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Dynamite.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LightStone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/SmallStone.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_DynamiteThrow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Dynamite.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;


	PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.f));
	/* for.InteractionProp*/
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BigBox"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/BigBox.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Cabinet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Rocker.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Candle"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Syokudai.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Candle_Fire"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Candle_Fire.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Door_00"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Architecture/WoodDoor_00.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Door_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Architecture/WoodDoor_01.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;
	PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Door_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Architecture/WoodDoor_02.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(270.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LevelProp"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/MaturiKagami.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;







	


	/* NON ANIM 예시*/
 	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster01"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Sunoko.fbx", PreTransformMatrix
	//		, nullptr, false
	//	))))
	PropPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	lstrcpy(m_szLoadingText, TEXT("가구를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("네비게이션을 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
 	
	
	//Prologue Stage
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Data/Navigation_Clinet_Prologue.dat")))))
	//	return E_FAIL;
	
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Data/Navigation_Clinet_Stage01.dat")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("객체원형(을)를 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),			CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/*For. Prototype_GameObject_CubeCollider*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CubeCollider"),	CCubeCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_POV"),		CCamera_POV::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/*For.Stage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Prologue_Stage"),	CPrologue::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Stage1"),			CStage1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Player*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),			CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Monster*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_CryingGirl"),	CryingGirl::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_Spider"),		CSpider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_MenMonster"),	CMenMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"),					CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Inventory"),		CInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory0"), CInnerInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory1"), CInnerInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory2"), CInnerInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory3"), CInnerInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory4"), CInnerInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory5"), CInnerInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory6"), CInnerInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory7"), CInnerInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_InnerInventory8"), CInnerInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ChooseBox"),		CChooseBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MiddlePoint"),		CMiddlePoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_RunnerGauge"),		CRunnerGauge::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HandLighter_Battery"),	CHandLighter_Battery::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HandLighter_Battery_Count"), CHandLighter_Battery_Count::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Magatama_UI"),		CMagatama::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CenterWord_UI"),	CCenterWord::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Dead_UI"),			CDead::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/*For. ItemProps*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Lighter"),			CLighter::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mirror"),			CMirror::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HandLighter"),		CHandLighter::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Compass"),		CCompass::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Kagura"),		CKagura::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_GlowStone"),		CGlowStone::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Key"),				CKey::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Spher"),			CSpher::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Dynamite"),		CDynamite::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LightStone"),		CLightStone::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DynamicThrow"),	CDynamite_Use::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MagatamaProp"),	CMagatamaProp::Create(m_pDevice, m_pContext))))
		return E_FAIL;





	/*For. InterActionProps*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Door00"),			CDoor00::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Door01"),			CDoor01::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Cabinet"),			CCabinet::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_EndProp"),			CEndProp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Candle"),			CCandle::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Candle_Fire"),		CCandle_Fire::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/*For. TeleportProp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TeleportProp"),	CTeleportProp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For. LevelProp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LevelProp"),		CLevelProp::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/*For. POV PROP*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Compass_Use"),		CCompass_Use::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Compass_Plate"),	CCompass_Plate::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mirror_Use"),		CMirror_Use::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* Effect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_ExplosionLv1"), CParticle_Spark::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_ExplosionLv2"), CParticle_SparkLv2::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));



	//m_pGameInstance->Delete_PlayerAllLight();

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Level_GamePlay_Lv2()
{

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX::Elements, VTXCUBETEX::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

	/*For AnimModel*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	
	/*For Cube Collider*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_Component_Shader_VtxCubeCol"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCubeCol.hlsl"), VTXCUBECOL::Elements, VTXCUBECOL::iNumElements))))
		return E_FAIL;
	


	_matrix PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);



	lstrcpy(m_szLoadingText, TEXT("모델를 로딩중입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_Component_Model_Stage1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/Stage/Stage2/Stage2_Props.fbx", PreTransformMatrix,
			nullptr, true))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("원형 객체를 로딩중입니다."));
	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For MOnster01*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_GameObject_Stage1"),
		CStage1::Create(m_pDevice, m_pContext))))



	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;
	

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);



}
