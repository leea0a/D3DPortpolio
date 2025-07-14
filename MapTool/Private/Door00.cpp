#include "stdafx.h"
#include "Door00.h"
#include "GameInstance.h"

CDoor00::CDoor00(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CDoor00::CDoor00(const CDoor00& Prototype)
	: CProp(Prototype)
{
}

HRESULT CDoor00::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CDoor00::Initialize(void* pArg)
{
	PROP_DESC* pSetDesc = static_cast<PROP_DESC*>(pArg);

	PROP_DESC pDesc;



	pDesc.fSpeedPerSec = 10.f;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = pSetDesc->vPosition;
	pDesc.vScale = { 3.8f , 4.0f, 3.5f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = pSetDesc->iPropNo;



	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(nullptr)))
		return E_FAIL;

	Set_PropType(TYPE_DOOR);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pSetDesc->vPosition.x, pSetDesc->vPosition.y, pSetDesc->vPosition.z, 1.f));

	return S_OK;
}

void CDoor00::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CDoor00::Update(_float fTimeDelta)
{
	//if (m_bChooseObj == true)
	//	Set_InterAction(fTimeDelta);
	//
	//for (auto& pCollider : m_pColliderDoor)
	//	pCollider->Update(m_pTransformCom->Get_WorldMatrix());
	return OBJ_NOEVENT;
	
}

void CDoor00::Late_Update(_float fTimeDelta)
{
	//CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));
	//
	//if (m_bChooseObj == false)
	//{
	//	for (auto& pCollider : m_pColliderDoor)
	//	{
	//		if (true == m_pColliderDoor[COLLIDER_AABB1]->Intersect(pTargetCollider))
	//		{
	//			if (KEY_DOWN(KEY::F))
	//			{
	//				m_bChooseObj = true;
	//				InterAction(fTimeDelta);
	//			}
	//		}
	//	}
	//	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	//}
	//else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CDoor00::Render()
{
	__super::Render();

	return S_OK;
}

void CDoor00::Set_InterAction(_float fTimeDelta)
{
	if (m_bOrigin == false)
	{
		m_OriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

		vPos += XMVector3Normalize(vRight) * -4.f;
		m_MaxPos = vPos;
		m_bOrigin = true;
	}

	if (true == m_bOrigin && false == m_bOpen)
	{
		Open(m_OriginPos);
	}
	else if (true == m_bOpen)
	{
		Close(m_OriginPos);
	}
}

void CDoor00::Open(_vector OriginPos)
{
	_vector CurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (Get_PosX(CurrentPos) > Get_PosX(m_MaxPos))
	{
		CurrentPos += XMVector3Normalize(vRight) * - 0.2f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
	}
	else
	{
		m_bOpen = true;
		m_bChooseObj = false;
	}

}

void CDoor00::Close(_vector OriginPos)
{
	_vector CurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (Get_PosX(CurrentPos) < Get_PosX(m_OriginPos))
	{
		CurrentPos += XMVector3Normalize(vRight) * 0.2f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
	}
	else
	{
		m_bOpen = false;
		m_bChooseObj = false;
	}
}

HRESULT CDoor00::Ready_Components(void* pArg)
{
	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);

	//CBounding_AABB::BOUND_AABB_DESC ColliderAABB{};
	//ColliderAABB.vCenter	= _float3{ pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z };
	//ColliderAABB.vExtents = { 0.6, 2.f, 0.1f };
	//
	//
	//
	//CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};
	//ColliderSphereDesc.fRadius = 4.f;
	//ColliderSphereDesc.vCenter = _float3(pDesc->vPosition.x, 3.f, pDesc->vPosition.z);
	//
	///* Com_Collider */
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB0]), &ColliderAABB)))
	//	return E_FAIL;

	//ColliderAABB.vExtents = { 0.8f, 2.f, 0.6f };
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
	//	TEXT("Com_Collider1"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB1]), &ColliderAABB)))
	//	return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Door_00"),
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

HRESULT CDoor00::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();
	return S_OK;
}

CDoor00* CDoor00::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDoor00* pInstance = new CDoor00(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CDoor00 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CDoor00::Clone(void* pArg)
{
	CDoor00* pInstance = new CDoor00(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CDoor00 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoor00::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderDoor)
		Safe_Release(pColliderCom);
}

