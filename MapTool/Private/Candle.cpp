#include "stdafx.h"
#include "Candle.h"
#include "GameInstance.h"

CCandle::CCandle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CCandle::CCandle(const CCandle& Prototype)
	: CProp(Prototype)
{
}

HRESULT CCandle::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CCandle::Initialize(void* pArg)
{

	PROP_DESC* pSetDesc = static_cast<PROP_DESC*>(pArg);

	PROP_DESC pDesc;



	pDesc.fSpeedPerSec = pSetDesc->fSpeedPerSec;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = pSetDesc->vPosition;
	pDesc.vScale = { 0.03f , 0.03f, 0.03f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = pSetDesc->iPropNo;

	Set_PropType(TYPE_CANDLE);

	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(nullptr)))
		return E_FAIL;

	Set_PropType(TYPE_CANDLE);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pSetDesc->vPosition.x, pSetDesc->vPosition.y, pSetDesc->vPosition.z, 1.f));

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 0.45f, -1.f, 1.f));
	//m_pTransformCom->Scaling(0.03f, 0.03f, 0.03f);

	return S_OK;
}

void CCandle::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CCandle::Update(_float fTimeDelta)
{
	//if (m_bChooseObj == true)
	//	Set_InterAction(fTimeDelta);

	return OBJ_NOEVENT;
	
}

void CCandle::Late_Update(_float fTimeDelta)
{
	//__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

}

HRESULT CCandle::Render()
{
	__super::Render();

	return S_OK;
}

void CCandle::Set_InterAction(_float fTimeDelta)
{
	//if (false == m_bCabinetIn)
	//	return;
	//
	//
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

void CCandle::Open(_vector OriginPos)
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

void CCandle::Close(_vector OriginPos)
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

HRESULT CCandle::Ready_Components(void* pArg)
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
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Candle"),
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

HRESULT CCandle::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CCandle* CCandle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCandle* pInstance = new CCandle(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CCandle Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCandle::Clone(void* pArg)
{
	CCandle* pInstance = new CCandle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CCandle Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCandle::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderDoor)
		Safe_Release(pColliderCom);

	//Safe_Release(m_pCameraPOV);
}

