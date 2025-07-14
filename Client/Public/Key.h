#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CKey final : public CProp
{
private: 
	explicit		 CKey(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CKey(const CKey& Prototype);
	virtual			~CKey() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual _int	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Components(void* pArg);
	HRESULT			Bind_ShaderResources();

public:
	static CKey*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

};

END
