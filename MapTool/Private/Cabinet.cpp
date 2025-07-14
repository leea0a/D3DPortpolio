#include "stdafx.h"
#include "Cabinet.h"
#include "GameInstance.h"

CCabinet::CCabinet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CCabinet::CCabinet(const CCabinet& Prototype)
	: CProp(Prototype)
{
}

HRESULT CCabinet::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CCabinet::Initialize(void* pArg)
{
	PROP_DESC* pSetDesc = static_cast<PROP_DESC*>(pArg);

	PROP_DESC pDesc;



	pDesc.fSpeedPerSec = pSetDesc->fSpeedPerSec;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = pSetDesc->vPosition;
	pDesc.vScale = { 0.03f, 0.03f, 0.03f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = pSetDesc->iPropNo;



	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(nullptr)))
		return E_FAIL;

	Set_PropType(TYPE_CABINET);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pSetDesc->vPosition.x, pSetDesc->vPosition.y, pSetDesc->vPosition.z, 1.f));

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, -10.f, 1.f));
	/*m_pTransformCom->Scaling(0.03f, 0.03f, 0.03f);*/

	return S_OK;
}

void CCabinet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CCabinet::Update(_float fTimeDelta)
{
	//if (m_bChooseObj == true)
	//	Set_InterAction(fTimeDelta);
	return OBJ_NOEVENT;

	
}

void CCabinet::Late_Update(_float fTimeDelta)
{
	//CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));
	//
	//if (m_bChooseObj == false)
	//{
	//	for (auto& pCollider : m_pColliderDoor)
	//	{
	//		//pCollider->Intersect(pTargetCollider);
	//
	//		if (true == m_pColliderDoor[COLLIDER_AABB1]->Intersect(pTargetCollider))
	//		{
	//			//m_vecPreInsidePosition = CUI_Manager::GetInstance()->Get_Player()->Get_Position();
	//			//
	//			//if (KEY_UP(KEY::F))
	//			//{
	//			//	m_bChooseObj = true;
	//			//	//m_bCabinetIn = true;
	//			//	InterAction(fTimeDelta);
	//			//	
	//			//}
	//		}
	//	}
	//	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	//}
	//else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);



	//for (auto& pCollider : m_pColliderDoor)
	//	pCollider->Update(m_pTransformCom->Get_WorldMatrix());

}

HRESULT CCabinet::Render()
{
	
	__super::Render();

	return S_OK;
}

void CCabinet::Set_InterAction(_float fTimeDelta)
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

void CCabinet::Open(_vector OriginPos)
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

void CCabinet::Close(_vector OriginPos)
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

HRESULT CCabinet::Ready_Components(void* pArg)
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
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Cabinet"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/*Com_Shader_For_Collider*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeCol"),
		TEXT("Com_Shader_For_Collider"), reinterpret_cast<CComponent**>(&m_pShader_For_ColliderCom))))
		return E_FAIL;
	/*Com_VIBuffer_Collider*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Collider"),
		TEXT("Com_VIBuffer_Collider"), reinterpret_cast<CComponent**>(&m_pVIBuffer_ColliderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCabinet::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CCabinet* CCabinet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCabinet* pInstance = new CCabinet(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CCabinet Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCabinet::Clone(void* pArg)
{
	CCabinet* pInstance = new CCabinet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CCabinet Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCabinet::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderDoor)
		Safe_Release(pColliderCom);

	//Safe_Release(m_pCameraPOV);
}

