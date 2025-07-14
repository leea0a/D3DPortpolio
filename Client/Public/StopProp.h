#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CStopProp final : public CProp
{
private:
	enum STOPCOLLIDER { COLL_STOPAABB0, COLL_STOPAABB1, COLL_STOPEND };

private: 
	explicit		 CStopProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CStopProp(const CStopProp& Prototype);
	virtual			~CStopProp() = default;


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
	static CStopProp*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

private:
	_vector		m_vTeleportPos;
	CCollider* m_pColliderCom_Stop[COLL_STOPEND] = { nullptr, nullptr };
	
};

END
