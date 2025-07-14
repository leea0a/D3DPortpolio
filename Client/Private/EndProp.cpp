#include "stdafx.h"
#include "EndProp.h"
#include "GameInstance.h"

CEndProp::CEndProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CEndProp::CEndProp(const CEndProp& Prototype)
	: CProp(Prototype)
{
}

HRESULT CEndProp::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CEndProp::Initialize(void* pArg)
{
	PROP_DESC* pSetDesc = static_cast<PROP_DESC*>(pArg);

	PROP_DESC pDesc;



	pDesc.fSpeedPerSec = 1.f;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = { 0.f, 0.f, 0.f, 1.f };
	pDesc.vScale = { 0.03f , 0.03f, 0.03f };
	pDesc.fAngle = pDesc.fAngle;
	pDesc.iData = 10;
	pDesc.iPropNo = pSetDesc->iPropNo;



	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	Set_PropType(TYPE_ENDPROP);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pSetDesc->vPosition.x, pSetDesc->vPosition.y, pSetDesc->vPosition.z, 1.f));
	m_pTransformCom->Scaling(0.05f, 0.05f, 0.05f);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), pSetDesc->fAngle);

	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(0.853f, 0.853, 0.853, 1.f);
	LightDesc.vAmbient = _float4(0.853f, 0.853, 0.853, 1.f);
	LightDesc.vSpecular = _float4(0.853f, 0.853, 0.853, 1.f);
	LightDesc.fRange = _float(7.f);
	LightDesc.vPosition = { m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1] + 1.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 1.f};
	LightDesc.TurnOn_Off = true;


	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;


	return S_OK;
}

void CEndProp::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CEndProp::Update(_float fTimeDelta)
{
	if (m_bChooseObj == true)
		Set_InterAction(fTimeDelta);

	return OBJ_NOEVENT;
}

void CEndProp::Late_Update(_float fTimeDelta)
{
	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));

	if (m_bChooseObj == false)
	{
		for (auto& pCollider : m_pColliderDoor)
		{
			//pCollider->Intersect(pTargetCollider);

			if (true == m_pColliderDoor[COLLIDER_AABB1]->Intersect(pTargetCollider))
			{
				CUI_Manager::GetInstance()->Set_BumpType(CUI_Manager::BUMP_ENDPROP);

				m_vecPreInsidePosition = CUI_Manager::GetInstance()->Get_Player()->Get_Position();

				if (KEY_UP(KEY::F))
				{
					if (CUI_Manager::GetInstance()->Get_MagatamaCount() == 3)
					{
						m_bChooseObj = true;
						//m_bCabinetIn = true;
						CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_TELEPORTPROP, false);
						InterAction(fTimeDelta);
						

						CUI_Manager::GetInstance()->Set_End(true);
					}
					else
					{
						CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_ENDPROPFAIL, false);
					}
					
					
				}
			}
		}
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}
	else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);



	for (auto& pCollider : m_pColliderDoor)
		pCollider->Update(m_pTransformCom->Get_WorldMatrix());

}

HRESULT CEndProp::Render()
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

void CEndProp::Set_InterAction(_float fTimeDelta)
{

}



HRESULT CEndProp::Ready_Components(void* pArg)
{
	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);


	CBounding_AABB::BOUND_AABB_DESC ColliderAABB{};

	ColliderAABB.vCenter = { 0.f, 90.f, 0.f };
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
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MagatamaBase"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEndProp::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CEndProp* CEndProp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEndProp* pInstance = new CEndProp(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CEndProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CEndProp::Clone(void* pArg)
{
	CEndProp* pInstance = new CEndProp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CEndProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEndProp::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderDoor)
		Safe_Release(pColliderCom);

	Safe_Release(m_pCameraPOV);
}

