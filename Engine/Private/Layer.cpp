#include "..\Public\Layer.h"
#include "GameObject.h"
#include "ContainerObj.h"

CLayer::CLayer()
{
}

CComponent* CLayer::Get_Component(const _wstring& strComponentTag, _uint iObjectIndex)
{
	if (iObjectIndex >= m_GameObjects.size())
		return nullptr;

	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iObjectIndex; i++)
		++iter;

	return (*iter)->Find_Component(strComponentTag);
}

CComponent* CLayer::Get_Component(_uint iPartObjectIndex, const _wstring& strPartComponentTag, _uint iObjectIndex)
{
	if (iObjectIndex >= m_GameObjects.size())
		return nullptr;

	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iObjectIndex; i++)
		++iter;

	return static_cast<CContainerObj*>(*iter)->Find_Component(iPartObjectIndex, strPartComponentTag);
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Priority_Update(fTimeDelta);
	}
		
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
		{

			int iEvent = pGameObject->Update(fTimeDelta);
			if (iEvent == OBJ_DEAD)
			{
				Safe_Release(pGameObject);
			}
		}
	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Update(fTimeDelta);
	}
		
}

void CLayer::Remove_From_Layer(CGameObject* pGameObject)
{

	if (m_GameObjects.empty())
	{
		MSG_BOX("Layer is Empty");
			return;
	}

	auto iter = find_if(m_GameObjects.begin(), m_GameObjects.end(),[&pGameObject](const CGameObject* pNowObject) 
		{
		
		if (pNowObject == pGameObject)
			return true;
		else 
			return false;
		});

	if (iter != m_GameObjects.end())
	{
		
		Safe_Release(*iter);
		m_GameObjects.remove(*iter);

	}
	else {
		MSG_BOX("is not Equal Object this");
		return;
	}

}

CLayer * CLayer::Create()
{
	 return new CLayer();
}


void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}
