#pragma once
#include "UI.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CShader;
class CModel;
class CVIBuffer_Collider;
END

class CInventory : public CUI
{
public:
	enum ITEM {ITEM_LIGHTER, ITEM_MIRROR, ITEM_HANDLIGHTER, ITEM_COMPASS, ITEM_KAGURA, ITEM_GLOWSTONE,  
					ITEM_KEY, ITEM_SPHER, ITEM_DYNAMITE, ITEM_LIGHTSTONE, ITEM_EMPTY, ITEM_END};

protected:
	explicit CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CInventory(const CInventory& Prototype);
	virtual ~CInventory() = default;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual _int					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render(_int iTemType) override;

protected:
	virtual HRESULT					Ready_Components() override;

public:
	static CInventory*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;

public:
	void							Set_ItemType(ITEM _item)	{m_eItemType = _item; }
	ITEM							Get_ItemType() { 
		return m_eItemType;
	}
	_int							Get_iTemCount() { return m_iItemCount; }
	void							Set_iTemCount(_int ItemCount) { m_iItemCount = ItemCount; }

	void							Set_HaveItem(_bool _HaveItem) { m_bHaveItem = _HaveItem; }
	_bool							Get_HaveItem() { return m_bHaveItem; }
	

protected:
	_bool					m_bHaveItem = { false };
	ITEM					m_eItemType = { ITEM_EMPTY };
	_int					m_iItemCount = { 0 };


};

