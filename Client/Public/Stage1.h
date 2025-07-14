#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Stage.h"


BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CStage1 final : public CStage
{
private: 
	explicit		 CStage1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CStage1(const CStage1& Prototype);
	virtual			~CStage1() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual _int	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Components();
	HRESULT			Bind_ShaderResources();

public:
	static CStage1*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

};

END
