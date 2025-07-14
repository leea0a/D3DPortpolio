#pragma once
#include "UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CVIBuffer_Collider;
END

class CChooseBox final : public CUI
{

protected:
	explicit CChooseBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CChooseBox(const CChooseBox& Prototype);
	virtual ~CChooseBox() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual _int			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

public:
	void isMove();

protected:
	virtual HRESULT			Ready_Components() override;
	void					For_Render_ChooseBox();

public:
	static CChooseBox*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;

private:
	_int					m_iCurrentIndex = { -1 };

};

