#pragma once
#include "UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CVIBuffer_Collider;
END

class CCenterWord final : public CUI
{

protected:
	explicit CCenterWord(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCenterWord(const CCenterWord& Prototype);
	virtual ~CCenterWord() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual _int			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

protected:
	virtual HRESULT			Ready_Components() override;

private:
	_int					Update_BumpForWord();

public:
	static CCenterWord*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;

private:
	_tchar						m_CenterWord[MAX_PATH] = {};
	_int						m_iFx = { 0 };
	_int						m_iFy = { 0 };

	_int						m_OriginFx = { 0 };
	_int						m_OriginFy = { 0 };


};

