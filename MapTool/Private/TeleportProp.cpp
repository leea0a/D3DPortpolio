#include "stdafx.h"
#include "TeleportProp.h"
//#include "UI_Manager.h"
#include "GameInstance.h"

CTeleportProp::CTeleportProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CTeleportProp::CTeleportProp(const CTeleportProp& Prototype)
	: CProp(Prototype)
{
}

HRESULT CTeleportProp::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CTeleportProp::Initialize(void* pArg)
{
	PROP_DESC pDesc;
	pDesc.fSpeedPerSec = 10.f;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = { -19.79f, -2.8f, -72.5f , 1.f };
	pDesc.vScale = { 0.03f , 0.03f, 0.03f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = 8;



	

	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-19.79f, -2.8f, -72.5f, 1.f));
	Set_PropType(TYPE_TELEPORT);

	return S_OK;
}

void CTeleportProp::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);


}

_int CTeleportProp::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CTeleportProp::Late_Update(_float fTimeDelta)
{
	//__super::Late_Update(fTimeDelta);
	
	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));

	_bool bInterSect = m_pColliderCom->Intersect(pTargetCollider);


	//if (true == bInterSect && true == CUI_Manager::GetInstance()->Get_Player()->Get_LighterUse() )
	//{
	//	CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(-18.79f, -2.8f, -99.5986f, 1.f));
	//	CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(196);
	//}
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this); 
	


}

HRESULT CTeleportProp::Render()
{

	m_pColliderCom->Render();
	
	return S_OK;
}

HRESULT CTeleportProp::Ready_Components(void* pArg)
{
	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);


	//CBounding_AABB::BOUND_AABB_DESC		ColliderAABBDesc{};
	//ColliderAABBDesc.vExtents = _float3{0.8f, 10.f, 1.5f};
	//ColliderAABBDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z);
	//
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderAABBDesc)))
	//	return E_FAIL;

	/* Com_Shader */
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;

	///* Com_Model  for Anim*/
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Lighter"),
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CTeleportProp::Bind_ShaderResources()
{
	//__super::Bind_ShaderResources();
	//
	return S_OK;
}

CTeleportProp* CTeleportProp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTeleportProp* pInstance = new CTeleportProp(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CTeleportProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CTeleportProp::Clone(void* pArg)
{
	CTeleportProp* pInstance = new CTeleportProp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CTeleportProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTeleportProp::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
}

