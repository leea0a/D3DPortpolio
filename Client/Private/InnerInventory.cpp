#include "stdafx.h"
#include "InnerInventory.h"

CInnerInventory::CInnerInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInventory { pDevice, pContext }
{
}

CInnerInventory::CInnerInventory(const CInnerInventory& Prototype)
	: CInventory ( Prototype )
{
}

HRESULT CInnerInventory::Initialize_Prototype()
{
	Ready_Components();
	return S_OK;
}

HRESULT CInnerInventory::Initialize(void* pArg)
{
	UIOBJDESC* pDesc = static_cast<UIOBJDESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	//if (FAILED(Ready_Components()))
	//	return E_FAIL;

	m_PreItemType = m_eItemType;

	return S_OK;
}

void CInnerInventory::Priority_Update(_float fTimeDelta)
{
}

_int CInnerInventory::Update(_float fTimeDelta)
{
	if (m_eItemType != ITEM_EMPTY && m_iItemCount > 0)
	{
		__super::Late_Update(fTimeDelta);
		//wsprintf(m_ItemCountChar, TEXT("x%d"), m_iItemCount);

		switch (m_eItemType)
		{
		case ITEM_LIGHTER:
			wsprintf(m_ItemCountChar, TEXT(" "));
			break;

		case ITEM_MIRROR:
			wsprintf(m_ItemCountChar, TEXT(" "));
			break;

		case ITEM_HANDLIGHTER:
			wsprintf(m_ItemCountChar, TEXT(" "));
			break;

		case ITEM_COMPASS:
			wsprintf(m_ItemCountChar, TEXT(" "));
			break;

		case ITEM_GLOWSTONE:
			wsprintf(m_ItemCountChar, TEXT("x%d"), m_iItemCount);
			break;

		case ITEM_SPHER:
			wsprintf(m_ItemCountChar, TEXT("x%d"), m_iItemCount);
			break;

		case ITEM_DYNAMITE:
			wsprintf(m_ItemCountChar, TEXT("x%d"), m_iItemCount);
			break;
		}

	}

	return OBJ_NOEVENT;
}

void CInnerInventory::Late_Update(_float fTimeDelta)
{	
}

HRESULT CInnerInventory::Render()
{
	__super::Render((int)m_eItemType);
	m_pGameInstance->Render_Font(TEXT("Chiller_16"), m_ItemCountChar, _float2(m_fX + 3, m_fY + 21), XMVectorSet(1.f, 1.f, 1.f, 1.f)); // µ¸¿ò / 10 Size
	return S_OK;
}

HRESULT CInnerInventory::Ready_Components()
{
	if (m_pGameInstance->Get_Level() == 2)
	{
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxPosTex"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		/* Com_VIBuffer */
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_VIBuffer_Rect"),
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Texture_UI_Item"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	else if (m_pGameInstance->Get_Level() == 3)
	{
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		/* Com_VIBuffer */
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Item"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	

	return S_OK;
}

CInventory* CInnerInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInnerInventory* pInstance = new CInnerInventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CInnerInventory Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CInnerInventory::Clone(void* pArg)
{
	CInnerInventory* pInstance = new CInnerInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CInnerInventory Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInnerInventory::Free()
{
	__super::Free();
	//Safe_Delete(m_pShaderCom);
	//Safe_Delete(m_pTextureCom);
	//Safe_Delete(m_pVIBufferCom);
}