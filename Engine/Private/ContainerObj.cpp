#include "..\Public\ContainerObj.h"

CContainerObj::CContainerObj(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{
}

CContainerObj::CContainerObj(const CContainerObj & Prototype)
	: CGameObject( Prototype )
{

}

HRESULT CContainerObj::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CContainerObj::Initialize(void * pArg)
{
	CONTAINEROBJ_DESC*	pDesc = static_cast<CONTAINEROBJ_DESC*>(pArg);

	m_PartObjects.resize(pDesc->iNumPartObj);	

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CContainerObj::Priority_Update(_float fTimeDelta)
{
	for (auto& pPartObj : m_PartObjects)
	{
		if(nullptr != pPartObj)
			pPartObj->Priority_Update(fTimeDelta);
	}

}

_int CContainerObj::Update(_float fTimeDelta)
{
	for (auto& pPartObj : m_PartObjects)
	{
		if (nullptr != pPartObj)
			pPartObj->Update(fTimeDelta);
	}

	return OBJ_NOEVENT;
}

void CContainerObj::Late_Update(_float fTimeDelta)
{
	for (auto& pPartObj : m_PartObjects)
	{
		if (nullptr != pPartObj)
			pPartObj->Late_Update(fTimeDelta);
	}
}

HRESULT CContainerObj::Render()
{
	return S_OK;
}

CComponent * CContainerObj::Find_Component(_uint iPartObjectIndex, const _wstring & strPartComponentTag)
{
	if (iPartObjectIndex >= m_PartObjects.size())
		return nullptr;

	return m_PartObjects[iPartObjectIndex]->Find_Component(strPartComponentTag);	
}

void CContainerObj::Free()
{
	__super::Free();

	for (auto& pPartObj : m_PartObjects)
		Safe_Release(pPartObj);

	m_PartObjects.clear();
}
