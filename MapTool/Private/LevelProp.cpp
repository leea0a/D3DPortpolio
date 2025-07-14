#include "stdafx.h"
#include "LevelProp.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CLevelProp::CLevelProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CLevelProp::CLevelProp(const CLevelProp& Prototype)
	: CProp(Prototype)
{
}

HRESULT CLevelProp::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CLevelProp::Initialize(void* pArg)
{
	PROP_DESC pDesc;
	pDesc.fSpeedPerSec = 10.f;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = { 0.f, 0.f , 0.f , 1.f };
	pDesc.vScale = { 1.f, 1.f, 1.f };//{ 0.033f , 0.033f, 0.033f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = 9;

	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	Set_PropType(TYPE_CABINET);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(63.486, -2.606, -99.566, 1.f));
	m_pTransformCom->Scaling(0.03f, 0.03f, 0.03f);

	return S_OK;
}

void CLevelProp::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CLevelProp::Update(_float fTimeDelta)
{
	if (m_bChooseObj == true)
		Set_InterAction(fTimeDelta);

	return OBJ_NOEVENT;

	
}

void CLevelProp::Late_Update(_float fTimeDelta)
{
	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));

	//if (m_bChooseObj == false)
	//{

	if (m_bUse == true)
	{
		return;
	}

	if (true == m_pColliderDoor[COLLIDER_AABB1]->Intersect(pTargetCollider))
	{
		//if (KEY_UP(KEY::F) && false == m_bUse)
		//{
		//	CUI_Manager::GetInstance()->Set_NextLevelGo(true);
		//}
		
	}
	else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR && m_bUse == false)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);


	//for (auto& pCollider : m_pColliderDoor)
	//{
	//	//pCollider->Intersect(pTargetCollider);
	//
	//	if (KEY_UP(KEY::F) && m_bUse)
	//	{
	//		m_bUse = true;
	//		if (true == m_pColliderDoor[COLLIDER_AABB1]->Intersect(pTargetCollider))
	//		{
	//
	//			m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY_LV2));
	//
	//		}
	//	}
	//	
	//}
	
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

		for (auto& pCollider : m_pColliderDoor)
			pCollider->Update(m_pTransformCom->Get_WorldMatrix());







}

HRESULT CLevelProp::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
	
	for (auto& pCollider : m_pColliderDoor)
		pCollider->Render();

	return S_OK;
}

void CLevelProp::Set_InterAction(_float fTimeDelta)
{
	if (false == m_bCabinetIn)
		return;


	//if (false == m_bHide && true == m_bCabinetIn)
	//{
	//	CUI_Manager::GetInstance()->Get_Player()->Set_PrePos(CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION));
	//	CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	//	m_bHide = true;
	//	CUI_Manager::GetInstance()->Get_Player()->Set_PlayerHide(m_bHide);
	//	
	//	m_bChooseObj = false;
	//}
	//else if (true == m_bHide)
	//{
	//	CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, CUI_Manager::GetInstance()->Get_Player()->Get_PrePos());
	//	m_bHide = false;
	//
	//	CUI_Manager::GetInstance()->Get_Player()->Set_PlayerHide(m_bHide);
	//	m_bCabinetIn = false;
	//	m_bChooseObj = false;
	//}
}

void CLevelProp::Open(_vector OriginPos)
{
	_vector CurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (Get_PosX(CurrentPos) > Get_PosX(m_MaxPos))
	{
		CurrentPos += XMVector3Normalize(vRight) * - 0.8f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
	}
	else
	{
		m_bOpen = true;
		m_bChooseObj = false;
	}

}

void CLevelProp::Close(_vector OriginPos)
{
	_vector CurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (Get_PosX(CurrentPos) < Get_PosX(m_OriginPos))
	{
		CurrentPos += XMVector3Normalize(vRight) * 0.8f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
	}
	else
	{
		m_bOpen = false;
		m_bChooseObj = false;
	}

}

HRESULT CLevelProp::Ready_Components(void* pArg)
{
	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);


	//CBounding_AABB::BOUND_AABB_DESC ColliderAABB{};
	//
	//ColliderAABB.vCenter = { 0.f, 120.f, 0.f };
	//	//_float3{ pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z };
	//ColliderAABB.vExtents = { 40.f, 100.f, 40.f };
	//
	//
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB0]), &ColliderAABB)))
	//	return E_FAIL;
	//
	//
	//ColliderAABB.vExtents = { 50.f, 100.f, 50.f };
	//
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
	//	TEXT("Com_Collider1"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB1]), &ColliderAABB)))
	//	return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LevelProp"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevelProp::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CLevelProp* CLevelProp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevelProp* pInstance = new CLevelProp(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CLevelProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CLevelProp::Clone(void* pArg)
{
	CLevelProp* pInstance = new CLevelProp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CLevelProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLevelProp::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderDoor)
		Safe_Release(pColliderCom);

	//Safe_Release(m_pCameraPOV);
}

