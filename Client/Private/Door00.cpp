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

	pDesc.fSpeedPerSec = pSetDesc->fSpeedPerSec;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = {0.f, 0.f, 0.f, 1.f};
	pDesc.vScale = { 3.5f , 3.5f, 3.5f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = pSetDesc->iPropNo;

	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	Set_PropType(TYPE_DOOR);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pSetDesc->vPosition.x, pSetDesc->vPosition.y, pSetDesc->vPosition.z, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), pSetDesc->fAngle);
	m_pTransformCom->Scaling(pSetDesc->vScale.x, pSetDesc->vScale.y, pSetDesc->vScale.z);

	return S_OK;
}

void CDoor00::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CDoor00::Update(_float fTimeDelta)
{
	if (m_bChooseObj == true)
		Set_InterAction(fTimeDelta);

	for (auto& pCollider : m_pColliderDoor)
	{
		pCollider->Update(m_pTransformCom->Get_WorldMatrix());
	}

	return OBJ_NOEVENT;
}

void CDoor00::Late_Update(_float fTimeDelta)
{
	//__super::Late_Update(fTimeDelta);

	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));
	
	if (m_bChooseObj == false)
	{
		for (auto& pCollider : m_pColliderDoor)
		{
			if (true == m_pColliderDoor[COLLIDER_AABB1]->Intersect(pTargetCollider))
			{
				CUI_Manager::GetInstance()->Set_BumpType(CUI_Manager::BUMP_DOOR);

				if (KEY_DOWN(KEY::F))
				{
					m_bChooseObj = true;
					InterAction(fTimeDelta);
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_DOORSLIDEOPEN);
				}
			}
		}
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}
	else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CDoor00::Render()
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
	
	//for (auto& pCollider : m_pColliderDoor)
	//	pCollider->Render();

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


	if ((floor(CurrentPos.m128_f32[0] * 10) / 10 == floor(m_MaxPos.m128_f32[0] * 10) / 10
		&& floor(CurrentPos.m128_f32[2] * 10) / 10 == floor(m_MaxPos.m128_f32[2] * 10) / 10))
	{
		m_bOpen = true;
		m_bChooseObj = false;
	}

	else if (floor(CurrentPos.m128_f32[0] * 10) / 10 == floor(m_MaxPos.m128_f32[0] * 10) / 10)
	{
		if (Get_PosZ(CurrentPos) < Get_PosZ(m_MaxPos))
		{
			CurrentPos += XMVector3Normalize(vRight) * -0.05f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
		}
		else if (Get_PosZ(CurrentPos) > Get_PosZ(m_MaxPos))
		{
			CurrentPos -= XMVector3Normalize(vRight) * 0.05f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
		}
	}
	else if (floor(CurrentPos.m128_f32[2] * 10) / 10 == floor(m_MaxPos.m128_f32[2] * 10) / 10)
	{
		if (Get_PosX(CurrentPos) > Get_PosX(m_MaxPos))
		{
			CurrentPos += XMVector3Normalize(vRight) * -0.05f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
		}
		else if (Get_PosX(CurrentPos) < Get_PosX(m_MaxPos))
		{
			CurrentPos -= XMVector3Normalize(vRight) * 0.05f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
		}
	}
}

void CDoor00::Close(_vector OriginPos)
{
	_vector CurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if ((floor(CurrentPos.m128_f32[0] * 10) / 10 == floor(m_OriginPos.m128_f32[0] * 10) / 10
		&& floor(CurrentPos.m128_f32[2] * 10) / 10 == floor(m_OriginPos.m128_f32[2] * 10) / 10))
	{
		m_bOpen = false;
		m_bChooseObj = false;
	}

	else if (floor(CurrentPos.m128_f32[0] * 10) / 10 == floor(m_OriginPos.m128_f32[0] * 10) / 10)
	{
		if (Get_PosZ(CurrentPos) < Get_PosZ(m_OriginPos))
		{
			CurrentPos -= XMVector3Normalize(vRight) * -0.05f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
		}
		else if (Get_PosZ(CurrentPos) > Get_PosZ(m_OriginPos))
		{
			CurrentPos += XMVector3Normalize(vRight) * 0.05f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
		}
	}
	else if (floor(CurrentPos.m128_f32[2] * 10) / 10 == floor(m_OriginPos.m128_f32[2] * 10) / 10)
	{
		if (Get_PosX(CurrentPos) > Get_PosX(m_OriginPos))
		{
			CurrentPos -= XMVector3Normalize(vRight) * -0.05f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
		}
		else if (Get_PosX(CurrentPos) < Get_PosX(m_OriginPos))
		{
			CurrentPos += XMVector3Normalize(vRight) * 0.05f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);
		}
	}
}

HRESULT CDoor00::Ready_Components(void* pArg)
{
	//PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);
	//
	//CBounding_AABB::BOUND_AABB_DESC ColliderAABB{};
	//
	//
	//
	//ColliderAABB.vCenter	= _float3{ pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z };
	//ColliderAABB.vExtents = { 0.6, 2.f, 0.1f };
	//
	////ColliderAABB.vCenter = { 0.f, 120.f, 0.f };
	////ColliderAABB.vExtents = { 0.6f, 2.f, 0.1f }; // 테스트용
	///* Com_Collider */
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB0]), &ColliderAABB)))
	//	return E_FAIL;
	//
	//ColliderAABB.vExtents = { 0.8f, 2.f, 0.6f };
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
	//	TEXT("Com_Collider1"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB1]), &ColliderAABB)))
	//	return E_FAIL;



	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);

	CBounding_OBB::BOUND_OBB_DESC ColliderOBB{};

	ColliderOBB.vCenter = _float3{ pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z };
	ColliderOBB.vExtents = { 0.6, 2.f, 0.1f };
	ColliderOBB.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));


	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB0]), &ColliderOBB)))
		return E_FAIL;
	
	ColliderOBB.vExtents = { 0.8f, 2.f, 0.6f };
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider1"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB1]), &ColliderOBB)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Door_00"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
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

