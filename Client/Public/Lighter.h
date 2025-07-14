#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CLighter final : public CProp
{
private: 
	explicit		 CLighter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CLighter(const CLighter& Prototype);
	virtual			~CLighter() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual _int	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void	Update_PlayerPos();

private:
	HRESULT			Ready_Components(void* pArg);
	HRESULT			Bind_ShaderResources();
	HRESULT			FireUse(_float fTimeDelta);

public:
	static CLighter*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

};

END
