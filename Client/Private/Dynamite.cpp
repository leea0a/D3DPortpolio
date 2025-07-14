#include "stdafx.h"
#include "Dynamite.h"
#include "GameInstance.h"
#include "Dynamite_Use.h"

CDynamite::CDynamite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CDynamite::CDynamite(const CDynamite& Prototype)
	: CProp(Prototype)
{
}

HRESULT CDynamite::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CDynamite::Initialize(void* pArg)
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

	Set_PropType(TYPE_DYNAMITE);

	return S_OK;
}

void CDynamite::Priority_Update(_float fTimeDelta)
{
	if (m_bLife == true)
	{
		return;
	}

	__super::Priority_Update(fTimeDelta);
}

_int CDynamite::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);


	

	return OBJ_NOEVENT;
}

void CDynamite::Late_Update(_float fTimeDelta)
{

	if (m_bLife == false)
	{
		__super::Late_Update(fTimeDelta);
	}
		

	if (CUI_Manager::GetInstance()->Get_DynamiteCool() == true)
	{
		CUI_Manager::GetInstance()->Set_DynamiteCoolF(CUI_Manager::GetInstance()->Get_DynamiteCoolF() + fTimeDelta);

		if (CUI_Manager::GetInstance()->Get_DynamiteCoolF() >= 2.f)
		{
			CUI_Manager::GetInstance()->Set_DynamiteCool(false);
		}
	}

	else if (CUI_Manager::GetInstance()->Get_DynamiteCool() == false)
	{
		if (CUI_Manager::GetInstance()->Get_ChooseIndex() != -1)
		{
			if (CUI_Manager::GetInstance()->Get_Inventory()[CUI_Manager::GetInstance()->Get_ChooseIndex()]->Get_ItemType() == CInventory::ITEM_DYNAMITE && CUI_Manager::GetInstance()->Get_DynamiteUse() == true)
			{
				CUI_Manager::GetInstance()->Get_Inventory()[CUI_Manager::GetInstance()->Get_ChooseIndex()]->Set_iTemCount(CUI_Manager::GetInstance()->Get_Inventory()[CUI_Manager::GetInstance()->Get_ChooseIndex()]->Get_iTemCount() - 1);
				Create_Dynamite(fTimeDelta);
				CUI_Manager::GetInstance()->Update_ChooseIndex();
			}
		}
		else if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
		{
			if (CUI_Manager::GetInstance()->Get_GlowStoneUse() == true)
			{
				Create_Dynamite(fTimeDelta);
			}
		}
	}

	
}

HRESULT CDynamite::Render()
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

HRESULT CDynamite::Create_Dynamite(_float fTimeDelta)
{
	CGameObject* pGameObject = { nullptr };
	PROP_DESC		Desc;
	Desc.vPosition.x = CUI_Manager::GetInstance()->Get_Player()->Get_Position().m128_f32[0];
	Desc.vPosition.y = CUI_Manager::GetInstance()->Get_Player()->Get_Position().m128_f32[1] + 4.7f;
	Desc.vPosition.z = CUI_Manager::GetInstance()->Get_Player()->Get_Position().m128_f32[2];
	Desc.vScale = { 10.f, 10.f, 10.f };
	Desc.fAngle = 0.f;

	const _wstring strLayerTag = TEXT("Layer_DynamiteThrow");

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DynamicThrow"), LEVEL_GAMEPLAY, strLayerTag, &Desc)))
		return E_FAIL;

	CUI_Manager::GetInstance()->Set_DynamiteSoundCount(1);

	
	if (CUI_Manager::GetInstance()->Get_FirstDynamite() == false && CUI_Manager::GetInstance()->Get_FirstMade() == false)
	{
		CUI_Manager::GetInstance()->Set_FirstDynamite(true);
		CUI_Manager::GetInstance()->Set_FirstMade(true);
	}
	


	CUI_Manager::GetInstance()->Set_DynamiteUse(false);
	CUI_Manager::GetInstance()->Set_DynamiteCool(true);
	CUI_Manager::GetInstance()->Set_DynamiteCoolF(0.f);


	return S_OK;
}

HRESULT CDynamite::Ready_Components(void* pArg)
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
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dynamite"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamite::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CDynamite* CDynamite::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDynamite* pInstance = new CDynamite(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CDynamite Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CDynamite::Clone(void* pArg)
{
	CDynamite* pInstance = new CDynamite(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CDynamite Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDynamite::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
}

