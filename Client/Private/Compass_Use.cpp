#include "stdafx.h"
#include "Compass_Use.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CCompass_Use::CCompass_Use(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CCompass_Use::CCompass_Use(const CCompass_Use& Prototype)
	: CProp(Prototype)
{
}

HRESULT CCompass_Use::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CCompass_Use::Initialize(void* pArg)
{
	

	PROP_DESC pDesc;



	pDesc.fSpeedPerSec = 1.f;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = {0.f, 0.f, 0.f, 1.f};
	pDesc.vScale = { 0.001f , 0.001f, 0.001f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;


	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	Set_PropType(TYPE_COMPASS);

	m_pTransformCom->Scaling(0.001f, 0.001f, 0.001f);

	return S_OK;
}

void CCompass_Use::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CCompass_Use::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CCompass_Use::Late_Update(_float fTimeDelta)
{
	_vector CameraPos = CUI_Manager::GetInstance()->Get_Camera()->Get_Position();
	_vector Look = CUI_Manager::GetInstance()->Get_Camera()->Get_Look();
	_vector Right = CUI_Manager::GetInstance()->Get_Camera()->Get_Right();
	_vector Up = CUI_Manager::GetInstance()->Get_Camera()->Get_Up();
	_vector offset = Right - Up * 0.14f + Look * 1.5;
	_vector NewPos = CameraPos + offset;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, NewPos);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, CUI_Manager::GetInstance()->Get_Camera()->Get_Transform_For_Player()->Get_State(CTransform::STATE_RIGHT) * 0.04f);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, CUI_Manager::GetInstance()->Get_Camera()->Get_Transform_For_Player()->Get_State(CTransform::STATE_LOOK) * 0.04f);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, CUI_Manager::GetInstance()->Get_Camera()->Get_Transform_For_Player()->Get_State(CTransform::STATE_UP) * 0.04f);


	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	
}

HRESULT CCompass_Use::Render()
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

		//
		// m_pColliderCom->Render();
	}


	return S_OK;
}

HRESULT CCompass_Use::Ready_Components(void* pArg)
{
	//PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);
	//
	//CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};
	//ColliderSphereDesc.fRadius = 3.f;
	//ColliderSphereDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z);
	//
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderSphereDesc)))
	//	return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Compass"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCompass_Use::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CCompass_Use* CCompass_Use::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCompass_Use* pInstance = new CCompass_Use(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CCompass_Use Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCompass_Use::Clone(void* pArg)
{
	CCompass_Use* pInstance = new CCompass_Use(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CCompass_Use Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCompass_Use::Free()
{
	__super::Free();
	//Safe_Release(m_pColliderCom);
}

