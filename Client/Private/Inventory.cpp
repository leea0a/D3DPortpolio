#include "stdafx.h"
#include "Inventory.h"
#include "InnerInventory.h"

CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI {pDevice, pContext}
{
}

CInventory::CInventory(const CInventory& Prototype)
	: CUI { Prototype }
{
}

HRESULT CInventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	UIOBJDESC* pDesc = static_cast<UIOBJDESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
}

_int CInventory::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CInventory::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CInventory::Render(_int iTemType)
{
	__super::Render((int)m_eItemType);
	
	return S_OK;
}

HRESULT CInventory::Ready_Components()
{
	return S_OK;
}

CInventory* CInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInventory* pInstance = new CInventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CInventory Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CInventory Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInventory::Free()
{
	__super::Free();
}



