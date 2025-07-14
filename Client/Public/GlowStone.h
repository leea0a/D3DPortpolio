#pragma once
#include "Prop.h"
#include "Client_Defines.h"


BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CGlowStone final : public CProp
{
private: 
	explicit		 CGlowStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CGlowStone(const CGlowStone& Prototype);
	virtual			~CGlowStone() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual _int	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT			Create_LightStone();


private:
	HRESULT			Ready_Components(void* pArg);
	HRESULT			Bind_ShaderResources();

public:
	static CGlowStone*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;
};

END
