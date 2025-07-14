#pragma once
#include "Inventory.h"

class CInnerInventory final : public CInventory
{
private:
	explicit CInnerInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CInnerInventory(const CInnerInventory& Prototype);
	virtual ~CInnerInventory() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual _int			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

protected:
	virtual HRESULT			Ready_Components() override;

public:
	static CInventory*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;

private:
	_tchar						m_ItemCountChar[MAX_PATH] = {};
	ITEM						m_PreItemType = { ITEM_EMPTY };
};

