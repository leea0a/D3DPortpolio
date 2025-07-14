#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
/*For GameLogo*/
#include "BackGround.h"
/*for GamePlay*/
#include "Camera_Free.h"
#include "Terrain.h"
#include "CubeCollider.h"
//#include "Terrain_Stage1.h"
#include "Prologue.h"
#include "Stage1.h"

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


/*For. InterActionProps*/
#include "Door00.h"
#include "Door01.h"
#include "Cabinet.h"
#include "Candle.h"

/* For. TeleportProp*/
#include "TeleportProp.h"

/* For. LevelProp*/
#include "LevelProp.h"

#include "Flat_Virtual_Terrain.h"
#include <time.h>

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
	EnterCriticalSection(&m_Critical_Section);

	CoInitializeEx( nullptr, 0);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:

 		hr = Loading_Level_Logo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_Level_GamePlay();
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
	SetWindowText(g_hWnd, m_szLoadingText);
}

#endif

HRESULT CLoader::Loading_Level_Logo()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For.Prototype_Component_Texture_Logo */
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
	

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Level_GamePlay()
{
	

	_matrix PreTransformMatrix = XMMatrixIdentity();
	

	/* 바이너리화 만들기! */

//CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster01/Monster01.fbx", TEXT("../Bin/resources/Models/Monster/Monster01/Monster01.dat"), PreTransformMatrix);
/*	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster02/Monster02.fbx", TEXT("../Bin/resources/Models/Monster/Monster02/Monster02_Anim.dat"), PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster04/Monster04.fbx", TEXT("../Bin/resources/Models/Monster/Monster04/Monster04_Anim.dat"),PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster05/Monster05.fbx", TEXT("../Bin/resources/Models/Monster/Monster05/Monster05_Anim.dat"), PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster06/Monster06.fbx", TEXT("../Bin/resources/Models/Monster/Monster06/Monster06_Anim.dat"), PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster07/Monster07.fbx", TEXT("../Bin/resources/Models/Monster/Monster07/Monster07_Anim.dat"), PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster08/Monster08.fbx", TEXT("../Bin/resources/Models/Monster/Monster08/Monster08_Anim.dat"), PreTransformMatrix);
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster09/Monster09.fbx", TEXT("../Bin/resources/Models/Monster/Monster09/Monster09_Anim.dat"), PreTransformMatrix);
    CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/1302_Right_aaok_LivingGun/Anim.fbx", TEXT("../Bin/resources/Models/1302_Right_ok_LivingGun/Anim.dat"), PreTransformMatrix);	
	CModel::Make_Binary_Anim(CModel::TYPE::TYPE_ANIM, "../Bin/resources/Models/Monster/Monster03/Monster03.fbx", TEXT("../Bin/resources/Models/Monster/Monster03/Monster03_Anim.dat"),PreTransformMatrix);
	*/
	//PreTransformMatrix = XMMatrixIdentity();
	//PreTransformMatrix = XMMatrixTranslation((_float)286.182 ,(_float)-17.432,(_float)85.044) * XMMatrixRotationY(XMConvertToRadians(180.f))* XMMatrixRotationX((0.051f));
	//CModel::Make_Binary_NonAnim(CModel::TYPE::TYPE_NONANIM, "../Bin/resources/Models/Map/Map01_Stage01/Map01_Stage01.fbx", TEXT("../Bin/resources/Models/Map/Map01_Stage01/Map01_Stage01.dat"), PreTransformMatrix);
	 lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
/* For.Prototype_Component_Texture_Terrain */
	 if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		 CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Default/Textures/Terrain/Tile0.dds"), 1))))
		 return E_FAIL;


	 if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SkyBox"),
		 CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/SkyBox/SkyBox.dds"), 1))))
		 return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("사운드를 로딩중입니다."));
		

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
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

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
	//	return E_FAIL;

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

	lstrcpy(m_szLoadingText, TEXT("네비게이션원형(을)를 로딩중입니다."));
	/* For.Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, nullptr))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델(을)를 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */





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

	/* Compass */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Compass"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Zippo.fbx", PropPreTransformMatrix,
	//		nullptr, true))))
	//	return E_FAIL;
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Key"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Key.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TimeSpire"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/TimeSpire.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dynamite"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Dynamite.fbx", PropPreTransformMatrix,
			nullptr, true))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Candle"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnim/Props/fbx/Syokudai.fbx", PropPreTransformMatrix,
	//		nullptr, true))))
	//	return E_FAIL;






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


PreTransformMatrix = XMMatrixIdentity();

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Default/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	clock_t start = clock();
	
	/*For Prologue Stage*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Prologue_Stage"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/Stage/Stage1/Stage1.fbx", PreTransformMatrix,
			nullptr, false))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.028f, 0.028f, 0.028f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stage1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, "../Bin/Resources/Models/Stage/Stage2/Stage2.fbx", PreTransformMatrix,
			nullptr, false))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	

	clock_t end = clock();
 
	double a = (double)(end - start) / CLOCKS_PER_SEC;

	lstrcpy(m_szLoadingText, TEXT("객체원형(을)를 로딩중입니다."));

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*For. Prototype_GameObject_CubeCollider*/
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CubeCollider"),
		CCubeCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
//	


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain_Prologue"),		CPrologue::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Stage1"),					CStage1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Flat_Virtual_Terrain"),	CFlat_Virtual_Terrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For. ItemProps*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Lighter"), CLighter::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mirror"), CMirror::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HandLighter"), CHandLighter::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mirror"), CCompass::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mirror"), CKagura::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_GlowStone"), CGlowStone::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Key"), CKey::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Spher"), CSpher::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Dynamite"), CDynamite::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For. InterActionProps*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Door00"), CDoor00::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Door01"), CDoor01::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Cabinet"), CCabinet::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Candle"), CCandle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For. TeleportProp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TeleportProp"), CTeleportProp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For. LevelProp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LevelProp"), CLevelProp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

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
