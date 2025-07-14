#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CMirror final : public CProp
{
private: 
	explicit		 CMirror(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CMirror(const CMirror& Prototype);
	virtual			~CMirror() = default;


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

private:
	void			Use_Mirror(_float fTimeDelta);
	void			Player_Revive();



public:
	static CMirror*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

};

END
