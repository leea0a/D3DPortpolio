#pragma once
#include "Monster.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

enum CRY_ANIM
{
	CRY_ANIM_CRY,
	CRY_ANIM_AROUND,
	CRY_ANIM_SPLINT,
	CRY_ANIM_WALK,
	CRY_ANIM_END
};

enum CRY_STATE
{
	CRY_STATE_CRY		= 0x00000000,
	CRY_STATE_AROUND	= 0x00000001,
	CRY_STATE_SPLINT	= 0x00000002,
	CRY_STATE_WALK		= 0x00000004
	

};

#define COND_CRY		(CRY_STATE::CRY_STATE_CRY		& m_iState)
#define COND_AROUND		(CRY_STATE::CRY_STATE_AROUND	& m_iState)
#define COND_SPLINT		(CRY_STATE::CRY_STATE_SPLINT	& m_iState)
#define COND_WALK		(CRY_STATE::CRY_STATE_WALK		& m_iState)


#define COND_LOOP		(COND_CRY || COND_AROUND || COND_WALK || COND_SPLINT)



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
	virtual _int	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
private:
	HRESULT			Ready_Components(COLLIDERDESC* pDesc) override;
	HRESULT			Bind_ShaderResources();
	HRESULT			Set_AnimationStates();

private:
	void			Pri_Update_Cancle_LoopState();

private:
	_bool			TraceDynamite(_float fTimeDelta);
	void			Action_Change(_float fTimeDelta);
	void			Action_Crying(_float fTimeDelta);
	void			Action_Around(_float fTimeDelta);
	void			Action_Walk(_float fTimeDelta);
	void			Action_Splint(_float fTimeDelta);

	void			DoorBump(_float fTimeDelta);


public:
	static CryingGirl*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

private:
	CCollider*				m_pColliderCrying[CRY_END] = { nullptr, nullptr, nullptr, nullptr };
	_bool					m_bFindPlayer = { false };
	_bool					m_bChasePlayer = { false };
	_bool					m_bReturing = { false };

	_float					m_fHideCurrentTime = { 0.f };

	_uint					m_iState = { (_int)CRY_STATE_CRY };

	_float					m_fDoorBumpBackTime = { 0.f };

	_float					m_fLookAtInterval = { 0.f };
	_float const			m_confLookatIntervalTime = { 15.f };
	_bool					m_bLookAtSoundOut = { false };


	_float					m_fWalkInterval = { 0.f };
	_float const			m_confWalkInterval = { 0.8f };
	
	//MONSTER_DESC* 

};

class CCryingGirl
{
};

END
