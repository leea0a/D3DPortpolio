#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CMagatamaProp final : public CProp
{
private: 
	explicit		 CMagatamaProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CMagatamaProp(const CMagatamaProp& Prototype);
	virtual			~CMagatamaProp() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual _int	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool			Get_MagatamaPut() { return m_bPut; }
	void			Set_MagatamaPut(_bool Putit) { m_bPut = Putit; }

private:
	HRESULT			Ready_Components(void* pArg);
	HRESULT			Bind_ShaderResources();

public:
	static CMagatamaProp*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;


private:
	_int				m_iMagatamaNo = { 0 };
	_float				m_prePosY = { 0.f };
	_float				m_MaxPosY = { 0.f };
	_float				m_MinPosY = { 0.f };
	_bool				m_bUpDown = { false };

	_bool				m_bPut = { false };
};

END
