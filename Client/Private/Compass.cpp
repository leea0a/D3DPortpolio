#include "stdafx.h"
#include "Compass.h"
#include "GameInstance.h"

CCompass::CCompass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CCompass::CCompass(const CCompass& Prototype)
	: CProp(Prototype)
{
}

HRESULT CCompass::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CCompass::Initialize(void* pArg)
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



	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	Set_PropType(TYPE_COMPASS);

	return S_OK;
}

void CCompass::Priority_Update(_float fTimeDelta)
{
	if (m_bLife == true)
	{
		return;
	}

	__super::Priority_Update(fTimeDelta);
}

_int CCompass::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CCompass::Late_Update(_float fTimeDelta)
{
	if (m_bLife == true)
	{
		return;
	}

	__super::Late_Update(fTimeDelta);
}

HRESULT CCompass::Render()
{
	if (m_bLife == true)
	{
		return S_OK;
	}

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

HRESULT CCompass::Ready_Components(void* pArg)
{
	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);
	
	CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};
	ColliderSphereDesc.fRadius = 3.f;
	ColliderSphereDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z);
	
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderSphereDesc)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Compass"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCompass::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();


	return S_OK;
}

CCompass* CCompass::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCompass* pInstance = new CCompass(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CCompass Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCompass::Clone(void* pArg)
{
	CCompass* pInstance = new CCompass(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CCompass Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCompass::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
}

