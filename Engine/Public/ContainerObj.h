#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CContainerObj abstract : public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		_uint			iNumPartObj;
	}CONTAINEROBJ_DESC;

protected:
	CContainerObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CContainerObj(const CContainerObj& Prototype);
	virtual ~CContainerObj() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual _int Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	class CComponent* Find_Component(_uint iPartObjectIndex, const _wstring& strPartComponentTag);

	/* 이 객체를 구성하기위한 부품(몸, 헤드, 무기, 이펙트, 사운드) 객체들 */
protected:
	vector<CGameObject*>			m_PartObjects;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END