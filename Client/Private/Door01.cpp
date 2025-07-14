#include "stdafx.h"
#include "Door01.h"
#include "GameInstance.h"

CDoor01::CDoor01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp{ pDevice, pContext }
{
}

CDoor01::CDoor01(const CDoor01& Prototype)
	: CProp (Prototype)
{
}

HRESULT CDoor01::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDoor01::Initialize(void* pArg)
{

	PROP_DESC* pSetDesc = static_cast<PROP_DESC*>(pArg);

	PROP_DESC pDesc;



	pDesc.fSpeedPerSec = 10.f;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = {0.f, 0.f, 0.f, 1.f};
	pDesc.vScale = { 3.5f , 3.5f, 3.5f };
	pDesc.fAngle = pDesc.fAngle;
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

void CDoor01::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CDoor01::Update(_float fTimeDelta)
{
	if (m_bChooseObj == true)
		Set_InterAction(fTimeDelta);

	for (auto& pCollider : m_pColliderDoor)
		pCollider->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CDoor01::Late_Update(_float fTimeDelta)
{
	//__super::Late_Update(fTimeDelta);

	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));
	
	if (m_bChooseObj == false)
	{
		for (auto& pCollider : m_pColliderDoor)
		{
			if (true == m_pColliderDoor[COLLIDER_OBB1]->Intersect(pTargetCollider))
			{
				CUI_Manager::GetInstance()->Set_BumpType(CUI_Manager::BUMP_DOOR);

				if (KEY_DOWN(KEY::F))
				{
					m_bChooseObj = true;

					if (m_bOpen == false)
					{
						CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_DOOROPEN1);
					}
					else if (m_bOpen == true)
					{
						
					}

					InterAction(fTimeDelta);
				}
			}
		}
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}
	else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CDoor01::Render()
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

void CDoor01::Set_InterAction(_float fTimeDelta)
{
	if (false == m_bOpen)
	{
		Open(fTimeDelta);
		
	}
	else if (true == m_bOpen)
	{
		Close(fTimeDelta);
		
	}
}

void CDoor01::Open(_float fTimeDelta)
{
	if (m_fCurrentAngle <= 90.f)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(1.f));
		m_fCurrentAngle += 1.45f;
		
	}
	else
	{
		m_fCurrentAngle = 0;
		
		m_bOpen = true;
		m_bChooseObj = false;
	}
	
}

void CDoor01::Close(_float fTimeDelta)
{
	if (m_fCurrentAngle <= 90.f)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, -1.f, 0.f, 0.f), XMConvertToRadians(1.f));
		m_fCurrentAngle += 1.45;

	}
	else
	{
		m_fCurrentAngle = 0;
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_DOORCLOSE);
		m_bOpen = false;
		m_bChooseObj = false;
	}
}

HRESULT CDoor01::Ready_Components(void* pArg)
{
	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);

	CBounding_OBB::BOUND_OBB_DESC ColliderOBB{};

	ColliderOBB.vCenter = _float3{ pDesc->vPosition.x + 0.5f, pDesc->vPosition.y, pDesc->vPosition.z };
	ColliderOBB.vExtents = { 0.6, 2.f, 0.1f };
	ColliderOBB.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_OBB0]), &ColliderOBB)))
		return E_FAIL;

	ColliderOBB.vExtents = { 0.8f, 2.f, 0.6f };

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider1"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_OBB1]), &ColliderOBB)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Door_02"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoor01::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();
	return S_OK;
}

CDoor01* CDoor01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDoor01* pInstance = new CDoor01(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CDoor01 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CDoor01::Clone(void* pArg)
{
	CDoor01* pInstance = new CDoor01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CDoor01 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoor01::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderDoor)
		Safe_Release(pColliderCom);
}
