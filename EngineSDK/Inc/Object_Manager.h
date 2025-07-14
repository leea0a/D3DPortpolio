#pragma once

/* ���̾� : ���� ������ ���� �������� ������ ������ �׷�. */
/* ���̾���� �������� �����Ѵ�. */
#include "Base.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iObjectIndex = 0);
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, _uint iPartObjectIndex, const _wstring& strPartComponentTag, _uint iObjectIndex = 0);
	_int Get_ComponentCount(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag);

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr ,class CGameObject** ppOut = nullptr);
	HRESULT	Remove_Object(_uint iLevelID, const _wstring& strLayerTag, CGameObject* pRemoveObject);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

	HRESULT Get_Object(_uint iLevelIndex, const _wstring& strLayerTag, list<class CGameObject*> ppOut);

private:
	_uint										m_iNumLevels = { 0 };
	map<const _wstring, class CLayer*>*			m_pLayers = { nullptr };
	typedef map<const _wstring, class CLayer*>	LAYERS;

	class CGameInstance*						m_pGameInstance = { nullptr };

	

private:
	class CLayer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END