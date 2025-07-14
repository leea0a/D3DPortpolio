#include "stdafx.h"
#include "HandLighter_Battery_Count.h"
#include "UI_Manager.h"
#include "InnerInventory.h"


CHandLighter_Battery_Count::CHandLighter_Battery_Count(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI {pDevice, pContext}
{
}

CHandLighter_Battery_Count::CHandLighter_Battery_Count(const CHandLighter_Battery_Count& Prototype)
	: CUI { Prototype }
{
}

HRESULT CHandLighter_Battery_Count::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	//CUI_Manager::GetInstance()->Get_BatteryCount().push_back(this);

	return S_OK;
}

HRESULT CHandLighter_Battery_Count::Initialize(void* pArg)
{
	UIOBJDESC* pDesc = static_cast<UIOBJDESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	

	return S_OK;
}

void CHandLighter_Battery_Count::Priority_Update(_float fTimeDelta)
{
}

_int CHandLighter_Battery_Count::Update(_float fTimeDelta)
{
	if (CUI_Manager::GetInstance()->Get_Player()->Get_HandLighterUse() == true)
	{
		CUI_Manager::GetInstance()->Set_HandLightTime(CUI_Manager::GetInstance()->Get_HandLightTime() - 0.1f);

		if (CUI_Manager::GetInstance()->Get_HandLightTime() <= 0.f)
		{
			CUI_Manager::GetInstance()->Set_HandLightTime(0.f);
			CUI_Manager::GetInstance()->Get_Player()->Set_HandLighterUse(false);
		}
	}
	
	return OBJ_NOEVENT;
}

void CHandLighter_Battery_Count::Late_Update(_float fTimeDelta)
{
	if (CUI_Manager::GetInstance()->Get_ChooseIndex() != -1)
	{
	if (CUI_Manager::GetInstance()->Get_Inventory()[CUI_Manager::GetInstance()->Get_ChooseIndex()]->Get_ItemType() == CInventory::ITEM_HANDLIGHTER)
	{

	

	
		if (CUI_Manager::GetInstance()->Get_HandLightTime() > 200.f &&
			CUI_Manager::GetInstance()->Get_HandLightTime() <= 300.f)
		{
			for (_int i = 0; i < CUI_Manager::GetInstance()->Get_BatteryCount().size(); ++i)
			{
				CUI_Manager::GetInstance()->Get_BatteryCount()[i]->__super::Late_Update(fTimeDelta);
			}
		}
		else if (CUI_Manager::GetInstance()->Get_HandLightTime() > 100.f &&
			CUI_Manager::GetInstance()->Get_HandLightTime() <= 200.f)
		{
			for (_int i = 1; i < CUI_Manager::GetInstance()->Get_BatteryCount().size(); ++i)
			{
				CUI_Manager::GetInstance()->Get_BatteryCount()[i]->__super::Late_Update(fTimeDelta);
			}
		}
		else if (CUI_Manager::GetInstance()->Get_HandLightTime() > 0.f &&
			CUI_Manager::GetInstance()->Get_HandLightTime() <= 100.f)
		{
			CUI_Manager::GetInstance()->Get_BatteryCount()[2]->__super::Late_Update(fTimeDelta);
		}
	}
	}
}

HRESULT CHandLighter_Battery_Count::Render()
{
	if (CUI_Manager::GetInstance()->Get_ChooseIndex() != -1)
	{
	if (CUI_Manager::GetInstance()->Get_Inventory()[CUI_Manager::GetInstance()->Get_ChooseIndex()]->Get_ItemType() == CInventory::ITEM_HANDLIGHTER)
	{
	
	
		if (CUI_Manager::GetInstance()->Get_HandLightTime() > 200.f &&
			CUI_Manager::GetInstance()->Get_HandLightTime() <= 300.f)
		{
			for (_int i = 0; i < CUI_Manager::GetInstance()->Get_BatteryCount().size(); ++i)
			{
				CUI_Manager::GetInstance()->Get_BatteryCount()[i]->__super::Render();
			}
		}
		else if (CUI_Manager::GetInstance()->Get_HandLightTime() > 100.f &&
			CUI_Manager::GetInstance()->Get_HandLightTime() <= 200.f)
		{
			for (_int i = 1; i < CUI_Manager::GetInstance()->Get_BatteryCount().size(); ++i)
			{
				CUI_Manager::GetInstance()->Get_BatteryCount()[i]->__super::Render();
			}
		}
		else if (CUI_Manager::GetInstance()->Get_HandLightTime() > 0.f &&
			CUI_Manager::GetInstance()->Get_HandLightTime() <= 100.f)
		{

				CUI_Manager::GetInstance()->Get_BatteryCount()[2]->__super::Render();
			
		}
	}
	}
	return S_OK;
}

HRESULT CHandLighter_Battery_Count::Ready_Components()
{
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HandLighter_BatteryCount"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CHandLighter_Battery_Count* CHandLighter_Battery_Count::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHandLighter_Battery_Count* pInstance = new CHandLighter_Battery_Count(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CHandLighter_Battery_Count Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CHandLighter_Battery_Count::Clone(void* pArg)
{
	CHandLighter_Battery_Count* pInstance = new CHandLighter_Battery_Count(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CHandLighter_Battery_Count Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHandLighter_Battery_Count::Free()
{
	
	__super::Free();
	
}
