#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"

#include "Level_Loading.h"
#include "BackGround.h"


CMainApp::CMainApp()
	: m_pGameInstance	{ CGameInstance::GetInstance() }
	, m_pUIManager		{ CUI_Manager::GetInstance() }
	, m_pSoundMgr		{ CSoundMgr::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pUIManager);
	Safe_AddRef(m_pSoundMgr);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC			EngineDesc{};

	EngineDesc.hInstance = g_hInst;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.isWindowed = true;
	EngineDesc.iNumLevels = LEVEL_END;
	EngineDesc.iViewportWidth = g_iWinSizeX;
	EngineDesc.iViewportHeight = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;
	if (FAILED(Ready_First()))
		return E_FAIL;

	m_pSoundMgr->Initialize(g_hWnd);

	//if (FAILED(Ready_Prototype_Component_For_Static()))
	//	return E_FAIL;

	//if (FAILED(SetUp_StartLevel(LEVEL_LOGO)))
	//	return E_FAIL;

	if (FAILED(SetUp_StartLevel(LEVEL_LOGO)))
		return E_FAIL;



	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	if (nullptr != m_pGameInstance)
		m_pGameInstance->Update_Engine(fTimeDelta);

#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Render_Begin()))
		return E_FAIL;

	m_pGameInstance->Draw();

#ifdef _DEBUG
	++m_iNumDraw;

	//if (m_fTimeAcc >= 1.f)
	//{
	//	wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumDraw);
	//
	//	m_iNumDraw = 0;
	//	m_fTimeAcc = 0.f;
	//}
	//
	//m_pGameInstance->Render_Font(TEXT("Ehedna"), m_szFPS, _float2(0.f, 5.f), XMVectorSet(1.f, 0.f, 0.f, 1.f)); // µ¸¿ò / 10 Size
#endif
	/*MakeSpriteFont "³Ø½¼lv1°íµñ Bold" /FontSize:10 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 147ex.spritefont */

	


	if (FAILED(m_pGameInstance->Render_End()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::SetUp_StartLevel(LEVELID eLevelID)
{

	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING,
		CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID))))
		return E_FAIL;


	
	return S_OK;
}

HRESULT CMainApp::Ready_First()
{
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Ehedna"), TEXT("../Bin/Resources/Font/Ehedna.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Chiller_16"), TEXT("../Bin/Resources/Font/Chiller_16.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Chiller_28"), TEXT("../Bin/Resources/Font/Chiller_28.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Chiller_40"), TEXT("../Bin/Resources/Font/Chiller_40.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Batang_20"), TEXT("../Bin/Resources/Font/Batang_20.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Batang_24"), TEXT("../Bin/Resources/Font/Batang_24.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Cormorant_30"), TEXT("../Bin/Resources/Font/Cormorant_30.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Cormorant_24"), TEXT("../Bin/Resources/Font/Cormorant_24.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Cormorant_36"), TEXT("../Bin/Resources/Font/Cormorant_36.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Cormorant_36_Sharp"), TEXT("../Bin/Resources/Font/Cormorant_36_Sharp.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Cormorant_26_Sharp"), TEXT("../Bin/Resources/Font/Cormorant_26_Sharp.spritefont"))))
		return E_FAIL;





	if (FAILED(m_pGameInstance->Add_Font(TEXT("TestFont_24"), TEXT("../Bin/Resources/Font/Maruburi_24.spritefont"))))
		return E_FAIL;



	//ID3D11Texture2D* pTexture2D = { nullptr };
	//
	//D3D11_TEXTURE2D_DESC	TextureDesc{};
	//
	//TextureDesc.Width = 256;
	//TextureDesc.Height = 256;
	//TextureDesc.MipLevels = 1;
	//TextureDesc.ArraySize = 1;
	//TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//
	//TextureDesc.SampleDesc.Quality = 0;
	//TextureDesc.SampleDesc.Count = 1;
	//
	//TextureDesc.Usage = D3D11_USAGE_STAGING;
	//TextureDesc.BindFlags = 0;
	//TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	//TextureDesc.MiscFlags = 0;
	//
	//_uint* pPixel = new _uint[256 * 256];
	//ZeroMemory(pPixel, sizeof(_uint) * 256 * 256);
	//
	//for (size_t i = 0; i < 256; i++)
	//{
	//	for (size_t j = 0; j < 256; j++)
	//	{
	//		_uint		iIndex = i * 256 + j;
	//
	//		pPixel[iIndex] = 0xffffffff;
	//	}
	//}
	//
	///* a b g r */
	//pPixel[0] = 0xff0000ff;
	//
	//D3D11_SUBRESOURCE_DATA		InitialData{};
	//
	//InitialData.pSysMem = pPixel;
	//InitialData.SysMemPitch = 256 * 4;
	//
	//if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &InitialData, &pTexture2D)))
	//	return E_FAIL;
	//
	//D3D11_MAPPED_SUBRESOURCE		Subresource{};
	//
	//m_pContext->Map(pTexture2D, 0, D3D11_MAP_READ_WRITE, 0, &Subresource);
	//
	//for (size_t i = 0; i < 256; i++)
	//{
	//	for (size_t j = 0; j < 256; j++)
	//	{
	//		_uint		iIndex = i * 256 + j;
	//
	//		if (j < 128)
	//			static_cast<_uint*>(Subresource.pData)[iIndex] = 0xffffffff;
	//		else
	//			static_cast<_uint*>(Subresource.pData)[iIndex] = 0xff000000;
	//
	//	}
	//}
	//m_pContext->Unmap(pTexture2D, 0);
	//
	//if (FAILED(SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../Bin/Resources/Textures/Terrain/Mask.dds"))))
	//	return E_FAIL;
	//
	//Safe_Release(pTexture2D);
	//
	//Safe_Delete_Array(pPixel);



	return S_OK;
}



CMainApp * CMainApp::Create()
{	
	CMainApp*	pInstance = new CMainApp();	

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{

	/* ºÎ¸ð ¸â¹ö¸¦ Á¤¸®ÇÑ´Ù. */
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pUIManager);
	Safe_Release(m_pSoundMgr);

	CUI_Manager::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	CGameInstance::Release_Engine();
}

