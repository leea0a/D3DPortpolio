#pragma once
#include "UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CVIBuffer_Collider;
END

class CMagatama final : public CUI
{

protected:
	explicit CMagatama(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMagatama(const CMagatama& Prototype);
	virtual ~CMagatama() = default;

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
	static CMagatama*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;

public:
	_tchar						m_MagatamaWord[MAX_PATH] = {};
	_uint						m_MagatamaCount = { 0 };

};

