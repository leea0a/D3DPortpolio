#include "..\Public\GameInstance.h"

#include "Frustum.h"
#include "Shadow.h"
#include "Height.h"
#include "Picking.h"
#include "Renderer.h"
#include "PipeLine.h"
#include "Input_Device.h"
#include "Font_Manager.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Target_Manager.h"
#include "Prototype_Manager.h"
#include "Light_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
	
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iViewportWidth, EngineDesc.iViewportHeight, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/*m_pInput_Device = CInput_Device::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;*/
	
	
	m_pLightManager = CLight_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pLightManager)
		return E_FAIL;

	m_pKeyManager = CKey_Manager::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	if (nullptr == m_pKeyManager)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pTarget_Manager = CTarget_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pFontManager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFontManager)
		return E_FAIL;

	m_pPicking = CPicking::Create(*ppDevice, *ppContext, EngineDesc.hWnd);
	if (nullptr == m_pPicking)
		return E_FAIL;

	m_pHeight = CHeight::Create(*ppDevice, *ppContext);
	if (nullptr == m_pHeight)
		return E_FAIL;

	m_pShadow = CShadow::Create(*ppDevice, *ppContext);
	if (nullptr == m_pShadow)
		return E_FAIL;

	m_pFrustum = CFrustum::Create();
	if (nullptr == m_pFrustum)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	//m_pInput_Device->Update_InputDev();
	//m_pUIManager->Update_UIs();
	m_pKeyManager->Update_KeyManager();

	m_pPicking->Update();
	m_pHeight->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);

	m_pPipeLine->Update();
	m_pFrustum->Update();

	m_pObject_Manager->Update(fTimeDelta);
	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pLevel_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Render_Begin(const _float4& vClearColor)
{
	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Draw()
{
	m_pRenderer->Draw_RenderObject();

	m_pLevel_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::Render_End()
{
	return m_pGraphic_Device->Present();
}

void CGameInstance::Clear(_int iLevelID)
{
	/* 특정 레벨용 객체들을 지운다. */

 	m_pObject_Manager->Clear(iLevelID);

	m_pPrototype_Manager->Clear(iLevelID);



}



_float CGameInstance::Compute_Random_Normal()
{
	return (_float)rand()/RAND_MAX;
}

_float CGameInstance::Compute_Random(_float fMin, _float fMax)
{
	return fMin+ (fMax-fMin)*Compute_Random_Normal();
}

KEY_STATE CGameInstance::Get_DIKeyState(KEY eKey)
{
	return m_pKeyManager->Get_DIKeySatate(eKey);
}

KEY_STATE CGameInstance::Get_DIMouseState(MOUSE eMouse)
{
	return m_pKeyManager->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSE_MOVE eMouseState)
{
	return m_pKeyManager->Get_DIMouseMove(eMouseState);
}


_float CGameInstance::Get_TimeDelta(const _wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Update_TimeDelta(const _wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return;

	return m_pTimer_Manager->Update_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObject(eRenderGroup, pRenderObject);
}

HRESULT CGameInstance::Add_DebugComponent(CComponent* pDebugCom)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_DebugComponent(pDebugCom);
}

HRESULT CGameInstance::Set_WhiteBlackShader(_bool Use)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Set_WhiteBlackShader(Use);
}

HRESULT CGameInstance::Start_Fade(_bool _use)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Start_Fade(_use);
}

HRESULT CGameInstance::Start_EndFade(_bool _use)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Start_EndFade(_use);
}

HRESULT CGameInstance::Stop_Fade() 
{ 
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Stop_Fade();
}
HRESULT CGameInstance::Set_FadeSpeed(_float fSpeed) 
{ 
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Set_FadeSpeed(fSpeed);
}
_float	CGameInstance::Get_FadeAmount() 
{ 
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Get_FadeAmount();
}

HRESULT CGameInstance::Set_GlitchShader(_bool _use)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Set_GlitchShader(_use);
}


HRESULT CGameInstance::Open_Level(_int iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

_int CGameInstance::Get_CurrentLevel()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Get_CurrnetLevelID();
}

_int CGameInstance::Get_PreLevelID()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Get_PreLevelID();
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring & strPrototypeTag, CBase * pPrototype)
{
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CBase * CGameInstance::Clone_Prototype(Engine::PROTOTYPE eType, _uint iLevelIndex, const _wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pPrototype_Manager)
		return nullptr;

	return m_pPrototype_Manager->Clone_Prototype(eType, iLevelIndex, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring & strPrototypeTag, _uint iLevelIndex, const _wstring & strLayerTag, void * pArg, CGameObject** ppout)
{
	
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_ToLayer(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg, ppout);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iObjectIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iObjectIndex);
}

_int CGameInstance::Get_ComponentCount(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Get_ComponentCount(iLevelIndex, strLayerTag, strComponentTag);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, _uint iPartObjectIndex, const _wstring& strPartComponentTag, _uint iObjectIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, iPartObjectIndex, strPartComponentTag, iObjectIndex);
}



HRESULT CGameInstance::Remove_Object(_uint iLevelID, const _wstring& strLayerTag, CGameObject* pRemoveObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pObject_Manager->Remove_Object(iLevelID, strLayerTag, pRemoveObject);

	return S_OK;
}

HRESULT CGameInstance::Get_Object(_uint iLevelIndex, const _wstring& strLayerTag, list<class CGameObject*> ppOut)
{
	return m_pObject_Manager->Get_Object(iLevelIndex, strLayerTag, ppOut);
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	return m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState)
{

	if (nullptr == m_pPipeLine)
		return _float4x4();
	
	return m_pPipeLine->Get_TransformFloat4x4(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4_Inverse(eState);
}

_matrix CGameInstance::Get_TransformMatrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix_Inverse(eState);
}

_matrix CGameInstance::Get_TransformInversedMatrix(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformInversedMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformInversedFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformInversedFloat4x4(eState);
}

const _float4* CGameInstance::Get_CamPosition() const
{
	if(nullptr == m_pPipeLine)
		return nullptr;

	m_pPipeLine->Get_CamPosition();
}

HRESULT CGameInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Add_Light(LightDesc);
}


HRESULT CGameInstance::Delete_Light()
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Delete_Light();
}

HRESULT CGameInstance::Add_PlayerLight(_int _index, const LIGHT_DESC& LightDesc)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Add_PlayerLight(_index, LightDesc);

}

HRESULT CGameInstance::Add_CandleLight(const LIGHT_DESC& LightDesc, _int _index)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Add_CandleLight(LightDesc, _index);

}

HRESULT CGameInstance::Add_MonsterLight(LIGHT_DESC LightDesc, _int _index)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Add_MonsterLight(LightDesc, _index);
}

HRESULT CGameInstance::Update_MonsterPos(_int _index, _vector Pos)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Update_MonsterPos(_index, Pos);
}

HRESULT CGameInstance::Update_PlayerPos(_int _index, _vector Pos)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Update_PlayerPos(_index, Pos);
}

HRESULT CGameInstance::Delete_PlayerLight(_int _index)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Delete_PlayerLight(_index);

}

HRESULT CGameInstance::PlayerLight_TurnOnOff(_int _index, _bool OnOff)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->PlayerLight_TurnOnOff(_index, OnOff);
}

HRESULT CGameInstance::Update_PlayerDir(_int _index, _float4 Dir)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Update_PlayerDir(_index, Dir);
}

HRESULT CGameInstance::Delete_PlayerAllLight()
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Delete_PlayerAllLight();
}

HRESULT CGameInstance::Delete_CandleLight(_int CandleIndex)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Delete_CandleLight(CandleIndex);
}

HRESULT CGameInstance::CandeLight_TurnOnOff(_int CandleIndex, _bool OnOff)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->CandeLight_TurnOnOff(CandleIndex, OnOff);
}

HRESULT CGameInstance::Delete_MonsterLight(_int _index)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Delete_MonsterLight(_index);
}

HRESULT CGameInstance::MonsterLight_TurnOnOff(_int _index, _bool OnOff)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->MonsterLight_TurnOnOff(_index, OnOff);
}



HRESULT CGameInstance::Update_StoneLight(_int _index, _vector Pos)
{

	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Update_StoneLight(_index, Pos);
}

HRESULT CGameInstance::Add_StoneLight(LIGHT_DESC LightDesc, _uint _index)
{

	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Add_StoneLight(LightDesc, _index);
}

HRESULT CGameInstance::Delete_SToneLight(_int _index)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Delete_SToneLight(_index);
}

HRESULT CGameInstance::StoneLight_TurnOnOff(_int _index, _bool OnOff)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->StoneLight_TurnOnOff(_index, OnOff);
}

HRESULT CGameInstance::Add_MagatamaLight(LIGHT_DESC LightDesc, _int _index)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Add_MagatamaLight(LightDesc, _index);
}

HRESULT CGameInstance::Update_MagatamaPos(_int _index, _vector Pos)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Update_MagatamaPos(_index, Pos);
}

HRESULT CGameInstance::Delete_MagamaLight(_int _index)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Delete_MagamaLight(_index);
}

HRESULT CGameInstance::MagatamaLight_TurnOnOff(_int _index, _bool OnOff)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->MagatamaLight_TurnOnOff(_index, OnOff);
}

HRESULT CGameInstance::Add_DynamiteLight(LIGHT_DESC LightDesc, _int _index)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Add_DynamiteLight(LightDesc, _index);
}

HRESULT CGameInstance::DynamiteLight_TurnOnOff(_int _index, _bool OnOff)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->DynamiteLight_TurnOnOff(_index, OnOff);
}


const LIGHT_DESC* CGameInstance::Get_LightDesc(_uint iIndex) const
{
	if (nullptr == m_pLightManager)
		return nullptr;

	return m_pLightManager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{

	return m_pLightManager->Render(pShader, pVIBuffer);
}

_int CGameInstance::Get_PlayerLightCount()
{
	return m_pLightManager->Get_PlayerLightCount();
}

_int CGameInstance::Get_StoneLightCount()
{
	return m_pLightManager->Get_StoneLightCount();
}

HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{

	return m_pFontManager->Add_Font(strFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin)
{
	return m_pFontManager->Render_Font(strFontTag, pText, vPosition, vColor, fRotation, vOrigin);
}

HRESULT CGameInstance::Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iWidth, iHeight, ePixelFormat, vClearColor);
}

HRESULT CGameInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}

HRESULT CGameInstance::Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV, _bool isClear)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag, pDSV, isClear);
}

HRESULT CGameInstance::End_MRT()
{
	return m_pTarget_Manager->End_MRT();
}

HRESULT CGameInstance::Bind_RT_ShaderResource(CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Bind_ShaderResource(pShader, pConstantName, strTargetTag);
}

HRESULT CGameInstance::Copy_RT_Resource(const _wstring& strTargetTag, ID3D11Texture2D* pDest)
{
	return m_pTarget_Manager->Copy_RT_Resource(strTargetTag, pDest);
}



HRESULT CGameInstance::Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_Debug(strTargetTag, fX, fY, fSizeX, fSizeY);
}

HRESULT CGameInstance::Render_RT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render_Debug(strMRTTag, pShader, pVIBuffer);
}

_bool CGameInstance::isPicked(_float4* pWorldPos)
{
	return m_pPicking->isPicked(pWorldPos);
}

const _float4x4* CGameInstance::Get_Height_Transform(CHeight::D3DTRANSFORMSTATE eState)
{
	return m_pHeight->Get_Height_Transform(eState);
}

void CGameInstance::SetUp_Height_TransformMatrix(CHeight::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	m_pHeight->SetUp_TransformMatrix(eState, TransformMatrix);
}

_float CGameInstance::Compute_Height(_fvector vWorldPos)
{
	return m_pHeight->Compute_Height(vWorldPos);
}

const _float4x4* CGameInstance::Get_Shadow_Transform(CShadow::D3DTRANSFORMSTATE eState)
{

	return m_pShadow->Get_Shadow_Transform(eState);
}

void CGameInstance::SetUp_Shadow_TransformMatrix(CShadow::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	m_pShadow->SetUp_TransformMatrix(eState, TransformMatrix);
}

void CGameInstance::Transform_Frustum_ToLocalSpace(_fmatrix WorldMatrix)
{
	m_pFrustum->Transform_ToLocalSpace(WorldMatrix);
}

_bool CGameInstance::isIn_Frustum_InWorldSpace(_fvector vWorldPos, _float fRange)
{
	return m_pFrustum->isIn_InWorldSpace(vWorldPos, fRange);
}

_bool CGameInstance::isIn_Frustum_InLocalSpace(_fvector vLocalPos, _float fRange)
{
	return m_pFrustum->isIn_InLocalSpace(vLocalPos, fRange);
}






void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->Free();

	CGameInstance::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	__super::Free();

	//Safe_Release(m_pInput_Device);
	//Safe_Release(m_pFrustum);
	//Safe_Release(m_pShadow);
	//Safe_Release(m_pHeight);
	//Safe_Release(m_pPicking);
	//Safe_Release(m_pTarget_Manager);
	//Safe_Release(m_pFontManager);
	//Safe_Release(m_pLightManager);
	//Safe_Release(m_pKeyManager);
	//Safe_Release(m_pPipeLine);
	//Safe_Release(m_pRenderer);
	//Safe_Release(m_pObject_Manager);
 	//Safe_Release(m_pPrototype_Manager);
	//Safe_Release(m_pLevel_Manager);
	//Safe_Release(m_pTimer_Manager);
	//Safe_Release(m_pGraphic_Device);



	Safe_Release(m_pFrustum);
	Safe_Release(m_pShadow);
	Safe_Release(m_pHeight);
	Safe_Release(m_pPicking);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pFontManager);
	Safe_Release(m_pLightManager);
	Safe_Release(m_pKeyManager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);
}
