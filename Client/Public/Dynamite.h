#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CDynamite final : public CProp
{
private: 
	explicit		 CDynamite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CDynamite(const CDynamite& Prototype);
	virtual			~CDynamite() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual _int	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	HRESULT			Create_Dynamite(_float fTimeDelta);

private:
	HRESULT			Ready_Components(void* pArg);
	HRESULT			Bind_ShaderResources();

public:
	static CDynamite*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

private:
	_float					m_fCooltime = { 0.0f };
	_float					m_bCool = { false };

	_bool					m_bFirstMade = { false };
	_float					m_bDynamiteSound = { 0.0f };

};

END
