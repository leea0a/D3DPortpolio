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
	pDesc.vPosition = { 63.486f, -2.606f, -99.566f , 1.f };
	pDesc.vScale = { 1.f, 1.f, 1.f };//{ 0.033f , 0.033f, 0.033f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = 0;

	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	 

	Set_PropType(TYPE_LEVEL);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(63.486f, -2.606f, -99.566f, 1.f));
	m_pTransformCom->Scaling(0.03f, 0.03f, 0.03f);

	for (auto& pCollider : m_pColliderDoor)
		pCollider->Update(m_pTransformCom->Get_WorldMatrix());

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
	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_PROLOGUE, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));

	if (m_bChooseObj == false)
	{
		for (auto& pCollider : m_pColliderDoor)
		{
			//pCollider->Intersect(pTargetCollider);

			if (true == m_pColliderDoor[COLLIDER_AABB1]->Intersect(pTargetCollider))
			{
				CUI_Manager::GetInstance()->Set_BumpType(CUI_Manager::BUMP_LEVEL);

				m_vecPreInsidePosition = CUI_Manager::GetInstance()->Get_Player()->Get_Position();

				if (KEY_UP(KEY::F))
				{
					m_bChooseObj = true;
					//m_bCabinetIn = true;
					InterAction(fTimeDelta);
					CUI_Manager::GetInstance()->Set_BumpType(CUI_Manager::BUMP_EMPTY);

				}
			}
		}
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}
	else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);



	//for (auto& pCollider : m_pColliderDoor)
	//	pCollider->Update(m_pTransformCom->Get_WorldMatrix());



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
	
	//for (auto& pCollider : m_pColliderDoor)
	//	pCollider->Render();

	return S_OK;
}

void CLevelProp::Set_InterAction(_float fTimeDelta)
{

}

HRESULT CLevelProp::Ready_Components(void* pArg)
{
	if (m_pGameInstance->Get_Level() == 2)
	{
		PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);


		CBounding_AABB::BOUND_AABB_DESC ColliderAABB{};

		ColliderAABB.vCenter = { 0.f, 120.f, 0.f };
		//_float3{ pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z };
		ColliderAABB.vExtents = { 40.f, 100.f, 40.f };


		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Collider_AABB"),
			TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB0]), &ColliderAABB)))
			return E_FAIL;


		ColliderAABB.vExtents = { 50.f, 100.f, 50.f };

		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Collider_AABB"),
			TEXT("Com_Collider1"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB1]), &ColliderAABB)))
			return E_FAIL;

		/* Com_Shader */
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxMesh"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		/* Com_Model  for Anim*/
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Model_LevelProp"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
	}
	else if (m_pGameInstance->Get_Level() == 3)
	{
		PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);


		CBounding_AABB::BOUND_AABB_DESC ColliderAABB{};

		ColliderAABB.vCenter = { 0.f, 120.f, 0.f };
		//_float3{ pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z };
		ColliderAABB.vExtents = { 40.f, 100.f, 40.f };


		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
			TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB0]), &ColliderAABB)))
			return E_FAIL;


		ColliderAABB.vExtents = { 50.f, 100.f, 50.f };

		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
			TEXT("Com_Collider1"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB1]), &ColliderAABB)))
			return E_FAIL;

		/* Com_Shader */
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		/* Com_Model  for Anim*/
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LevelProp"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
	}

	

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

	Safe_Release(m_pCameraPOV);
}

