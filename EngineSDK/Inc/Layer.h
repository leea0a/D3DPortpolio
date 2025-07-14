#pragma once

#include "Base.h"

/* 객체들을 레이어 별로 묶어서 다수 보관한다. */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	class CComponent* Get_Component(const _wstring& strComponentTag, _uint iObjectIndex = 0);
	class CComponent* Get_Component(_uint iPartObjectIndex, const _wstring& strPartComponentTag, _uint iObjectIndex = 0);

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	list<class CGameObject*> Get_GameObject() { return m_GameObjects; }
	
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Remove_From_Layer(CGameObject* pGameObject);
	
private:
	list<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END