#include "stdafx.h"
#include "StopProp.h"
#include "UI_Manager.h"
#include "GameInstance.h"

CStopProp::CStopProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CStopProp::CStopProp(const CStopProp& Prototype)
	: CProp(Prototype)
{
}

HRESULT CStopProp::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CStopProp::Initialize(void* pArg)
{
	PROP_DESC pDesc;
	pDesc.fSpeedPerSec = 10.f;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	//pDesc.vPosition = { 0.f, 0.f, 0.f, 1.f };
	pDesc.vPosition = { 35.293f, -2.874f, -53.729f, 1.f };
	pDesc.vScale = { 0.03f , 0.03f, 0.03f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = 0;


	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(35.293f, -2.874f, -53.729f, 1.f));
	Set_PropType(TYPE_STOP);

	return S_OK;
}

void CStopProp::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CStopProp::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CStopProp::Late_Update(_float fTimeDelta)
{
	//__super::Late_Update(fTimeDelta);
	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_PROLOGUE, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));

	if (m_bChooseObj == false)
	{
		if (true == m_pColliderCom_Stop[COLL_STOPAABB1]->Intersect(pTargetCollider))
		{
			CUI_Manager::GetInstance()->Set_BumpType(CUI_Manager::BUMP_STOP);

			//m_vecPreInsidePosition = CUI_Manager::GetInstance()->Get_Player()->Get_Position();
			//
			//if (KEY_UP(KEY::F))
			//{
			//	m_bChooseObj = true;
			//	//m_bCabinetIn = true;
			//	InterAction(fTimeDelta);
			//
			//}
		}
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}
	//else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR)
	//	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	//
	//
	//
	//for (auto& pCollider : m_pColliderDoor)
	//	pCollider->Update(m_pTransformCom->Get_WorldMatrix());
}

HRESULT CStopProp::Render()
{

	//m_pColliderCom->Render();
	
	return S_OK;
}

HRESULT CStopProp::Ready_Components(void* pArg)
{
	if (m_pGameInstance->Get_Level() == 2)
	{
		PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);


		CBounding_AABB::BOUND_AABB_DESC		ColliderAABBDesc{};
		ColliderAABBDesc.vExtents = _float3{ 8.f, 100.f, 8.f };
		ColliderAABBDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z);

		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Collider_AABB"),
			TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom_Stop[COLL_STOPAABB0]), &ColliderAABBDesc)))
			return E_FAIL;


		ColliderAABBDesc.vExtents = _float3{ 10.f, 100.f, 10.f }; 
		ColliderAABBDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z);

		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Collider_AABB"),
			TEXT("Com_Collider1"), reinterpret_cast<CComponent**>(&m_pColliderCom_Stop[COLL_STOPAABB1]), &ColliderAABBDesc)))
			return E_FAIL;


	}
	else if (m_pGameInstance->Get_Level() == 3)
	{
		PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);


		CBounding_AABB::BOUND_AABB_DESC		ColliderAABBDesc{};
		ColliderAABBDesc.vExtents = _float3{ 0.8f, 10.f, 1.5f };
		ColliderAABBDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z);

		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
			TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderAABBDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStopProp::Bind_ShaderResources()
{
	return S_OK;
}

CStopProp* CStopProp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStopProp* pInstance = new CStopProp(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CStopProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CStopProp::Clone(void* pArg)
{
	CStopProp* pInstance = new CStopProp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CStopProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStopProp::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);

	for (auto& pCollider : m_pColliderCom_Stop)
		Safe_Release(pCollider);
}

