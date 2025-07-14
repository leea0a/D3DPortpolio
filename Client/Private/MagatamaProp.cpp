#include "stdafx.h"
#include "MagatamaProp.h"
#include "GameInstance.h"

CMagatamaProp::CMagatamaProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CMagatamaProp::CMagatamaProp(const CMagatamaProp& Prototype)
	: CProp(Prototype)
{
}

HRESULT CMagatamaProp::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CMagatamaProp::Initialize(void* pArg)
{
	PROP_DESC* pSetDesc = static_cast<PROP_DESC*>(pArg);

	PROP_DESC pDesc;



	pDesc.fSpeedPerSec = pSetDesc->fSpeedPerSec;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = {0.f, 0.f, 0.f, 1.f};
	pDesc.vScale = { 0.03f , 0.03f, 0.03f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = pSetDesc->iPropNo;


	m_iMagatamaNo = pSetDesc->iPropNo;


	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	Set_PropType(TYPE_MAGATAMA);
 	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pSetDesc->vPosition.x, pSetDesc->vPosition.y, pSetDesc->vPosition.z, 1.f));
	m_pTransformCom->Scaling(0.1f, 0.1f, 0.1f);

	m_prePosY = pSetDesc->vPosition.y;
	m_MaxPosY = m_prePosY + 0.7f;
	m_MinPosY = m_prePosY - 0.7f;


	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(0.423f, 0.956f, 0.458f, 1.f);
	LightDesc.vAmbient = _float4(0.423f, 0.956f, 0.458f, 1.f);
	LightDesc.vSpecular = _float4(0.423f, 0.956f, 0.458f, 1.f);
	LightDesc.fRange = _float(7.f);
	LightDesc.vPosition = { 0.f, 0.f, 0.f , 1.f};
	LightDesc.TurnOn_Off = true;


	if (FAILED(m_pGameInstance->Add_MagatamaLight(LightDesc, m_iMagatamaNo)))
		return E_FAIL;

	return S_OK;
}

void CMagatamaProp::Priority_Update(_float fTimeDelta)
{
	if (m_bLife == true)
	{
		return;
	}
	__super::Priority_Update(fTimeDelta);
}

_int CMagatamaProp::Update(_float fTimeDelta)
{
	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta / 1.5f);

	if (m_bUpDown == false)
	{
		_float MoveY = m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1] + fTimeDelta / 3;
		
		//MoveY += pow(m_MaxPosY , 300);

		if (MoveY >= m_MaxPosY)
		{
			m_bUpDown = true;
			return OBJ_NOEVENT;
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], MoveY, m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 1.f));

	}
	else if (m_bUpDown == true)
	{
		if (m_bPut == true)
		{
			m_pGameInstance->Delete_MagamaLight(m_iMagatamaNo);
		}


		_float MoveY = m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1] - fTimeDelta / 3;
		//MoveY -= pow(m_MaxPosY , 300);
		if (MoveY <= m_MinPosY)
		{
			m_bUpDown = false;
			return OBJ_NOEVENT;
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], MoveY, m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 1.f));
	}

	if (m_bPut == false)
	{
		m_pGameInstance->Update_MagatamaPos(m_iMagatamaNo, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}
	
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CMagatamaProp::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);
}

HRESULT CMagatamaProp::Render()
{
	if (m_bChooseObj == false)
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

		//m_pColliderCom->Render();
	}


	return S_OK;
}

HRESULT CMagatamaProp::Ready_Components(void* pArg)
{
	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);

	CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};
	ColliderSphereDesc.fRadius = 23.f;
	ColliderSphereDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z);

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderSphereDesc)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Magatama"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMagatamaProp::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}


CMagatamaProp* CMagatamaProp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMagatamaProp* pInstance = new CMagatamaProp(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CMagatamaProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMagatamaProp::Clone(void* pArg)
{
	CMagatamaProp* pInstance = new CMagatamaProp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CMagatamaProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMagatamaProp::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
}

