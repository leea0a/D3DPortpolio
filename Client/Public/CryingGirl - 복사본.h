#pragma once
#include "Monster.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CryingGirl final : public CMonster
{
public:
	enum CRYINGGIRL { CRY_OBB, CRY_SPHERE0, CRY_SPHERE1, CRY_SPHERE2, CRY_END };

private: 
	explicit		 CryingGirl(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CryingGirl(const CryingGirl& Prototype);
	virtual			~CryingGirl() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual void	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
private:
	HRESULT			Ready_Components(COLLIDERDESC* pDesc) override;
	HRESULT			Bind_ShaderResources();

private:
	void			Action_Change(_float fTimeDelta);
	void			Action_Crying(_float fTimeDelta);
	void			Action_Around(_float fTimeDelta);
	void			Action_Walk(_float fTimeDelta);
	void			Action_Splint(_float fTimeDelta);


public:
	static CryingGirl*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

private:
	CCollider*	m_pColliderCrying[CRY_END] = { nullptr, nullptr, nullptr, nullptr };
	_bool		m_bFindPlayer = { false };
	_bool		m_bChasePlayer = { false };
	_bool		m_bReturing = { false };

	_float		m_fHideCurrentTime = { 0.f };
	
	
	//MONSTER_DESC* 

};

END
