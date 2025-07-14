#include "stdafx.h"
#include "HandLighter_Battery.h"
#include "UI_Manager.h"
#include "InnerInventory.h"


CHandLighter_Battery::CHandLighter_Battery(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI {pDevice, pContext}
{
}

CHandLighter_Battery::CHandLighter_Battery(const CHandLighter_Battery& Prototype)
	: CUI { Prototype }
{
}

HRESULT CHandLighter_Battery::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHandLighter_Battery::Initialize(void* pArg)
{
	UIOBJDESC* pDesc = static_cast<UIOBJDESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CHandLighter_Battery::Priority_Update(_float fTimeDelta)
{
}

_int CHandLighter_Battery::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CHandLighter_Battery::Late_Update(_float fTimeDelta)
{
	if (CUI_Manager::GetInstance()->Get_ChooseIndex() != -1)
	{
		if (CUI_Manager::GetInstance()->Get_Inventory()[CUI_Manager::GetInstance()->Get_ChooseIndex()]->Get_ItemType() == CInventory::ITEM_HANDLIGHTER)
		__super::Late_Update(fTimeDelta);
	}
	
	

	
}

HRESULT CHandLighter_Battery::Render()
{
	if (CUI_Manager::GetInstance()->Get_ChooseIndex() != -1)
	{
		if (CUI_Manager::GetInstance()->Get_Inventory()[CUI_Manager::GetInstance()->Get_ChooseIndex()]->Get_ItemType() == CInventory::ITEM_HANDLIGHTER)
			__super::Render();
	}
	return S_OK;
}

HRESULT CHandLighter_Battery::Ready_Components()
{
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HandLighter_Battery"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CHandLighter_Battery* CHandLighter_Battery::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHandLighter_Battery* pInstance = new CHandLighter_Battery(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CHandLighter_Battery Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CHandLighter_Battery::Clone(void* pArg)
{
	CHandLighter_Battery* pInstance = new CHandLighter_Battery(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CHandLighter_Battery Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHandLighter_Battery::Free()
{
	
	__super::Free();
	
}
