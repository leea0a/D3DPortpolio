#include "stdafx.h"
#include "Lighter.h"
#include "GameInstance.h"

CLighter::CLighter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CLighter::CLighter(const CLighter& Prototype)
	: CProp(Prototype)
{
}

HRESULT CLighter::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CLighter::Initialize(void* pArg)
{
	PROP_DESC pDesc;
	pDesc.fSpeedPerSec = 10.f;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = { 20.370f, -7.347f, 19.051f , 1.f };
	pDesc.vScale = { 0.03f , 0.03f, 0.03f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = 0;
	

	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	Set_PropType(TYPE_LIGHTER);

	return S_OK;
}

void CLighter::Priority_Update(_float fTimeDelta)
{
	if (m_bLife == true)
	{
		return;
	}

	__super::Priority_Update(fTimeDelta);
}

_int CLighter::Update(_float fTimeDelta)
{
	//Update_PlayerPos();
	//FireUse(fTimeDelta);


	return OBJ_NOEVENT;
}

void CLighter::Late_Update(_float fTimeDelta)
{
	if (m_bLife == true)
	{
		return;
	}
	__super::Late_Update(fTimeDelta);
}

HRESULT CLighter::Render()
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

void CLighter::Update_PlayerPos()
{
	//m_pGameInstance->Update_PlayerPos(CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION));
}

HRESULT CLighter::Ready_Components(void* pArg)
{

	if (m_pGameInstance->Get_Level() == 2)
	{
		PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);

		CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};
		ColliderSphereDesc.fRadius = 3.f;
		ColliderSphereDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z);

		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Collider_Sphere"),
			TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderSphereDesc)))
			return E_FAIL;

		/* Com_Shader */
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxMesh"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		/* Com_Model  for Anim*/
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Model_Lighter"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;

	}
	else if (m_pGameInstance->Get_Level() == 3)
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

		/* Com_Model  for Anim*/
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Lighter"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;

	}






	return S_OK;
}

HRESULT CLighter::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

HRESULT CLighter::FireUse(_float fTimeDelta)
{
	//if (CUI_Manager::GetInstance()->Get_Player() == nullptr)
	//	return E_FAIL;
	//
	//if (true == CUI_Manager::GetInstance()->Get_Player()->Get_LighterUse())
	//{
	//	LIGHT_DESC			LightDesc{};
	//
	//	ZeroMemory(&LightDesc, sizeof LightDesc);
	//
	//	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	//	LightDesc.vAmbient = _float4(1.f, 0.f, 0.f, 1.f);
	//	LightDesc.vSpecular = _float4(1.f, 0.f, 0.f, 1.f);
	//	LightDesc.fRange = _float(20.f);
	//	LightDesc.vPosition = { CUI_Manager::GetInstance()->Get_Player()->Get_Position().m128_f32[0], CUI_Manager::GetInstance()->Get_Player()->Get_Position().m128_f32[1] + 4.1f  , CUI_Manager::GetInstance()->Get_Player()->Get_Position().m128_f32[2], 1.f};
	//
	//
	//	if(FAILED(m_pGameInstance->Add_PlayerLight(LightDesc)))
	//		return E_FAIL;
	//
	//}
	//else if (false == CUI_Manager::GetInstance()->Get_Player()->Get_LighterUse())
	//{
	//	//m_pGameInstance->Delete_PlayerLight();
	//}


	return S_OK;
}

CLighter* CLighter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLighter* pInstance = new CLighter(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CLighter Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CLighter::Clone(void* pArg)
{
	CLighter* pInstance = new CLighter(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CLighter Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLighter::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
}

