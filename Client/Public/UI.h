#pragma once
#include "UIObject.h"
#include "UI_Manager.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI : public Engine::CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{

	}UIOBJDESC;


public:
	explicit CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI(const CUI& Prototype);
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual _int Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_int iTemType = 0);


protected:
	virtual HRESULT Ready_Components() = 0;
	HRESULT Bind_ShaderResources(_int iTemType = 0);


public:
	//virtual HRESULT Add_UIObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& LayerTag, 
	//									const _wstring strUIObjectTag, _uint iLevelIndex, void* pArg = nullptr, CUI** ppOut = nullptr);
	//
	//virtual CUIObject* Find_UIObject(const _wstring& strUIObjectTag);
	

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
	CShader*			m_pShaderCom	= { nullptr };
	CTexture*			m_pTextureCom	= { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom	= { nullptr };

};

END