#pragma once
#include "Monster.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END


enum SPIDER_ANIM
{
	SPIDER_ANIM_SPLINT,
	SPIDER_ANIM_WALK,
	SOIDER_ANIM_END
};

enum SPIDER_STATE
{
	SPIDER_STATE_SPLINT		= 0x00000001 ,
	SPIDER_STATE_WALK		= 0x00000002
	
};

#define CONDITION_SPLINT	(SPIDER_STATE::SPIDER_STATE_SPLINT & m_iState)
#define CONDITION_WALK		(SPIDER_STATE::SPIDER_STATE_WALK & m_iState)

#define CONDITION_LOOP		(CONDITION_WALK || CONDITION_SPLINT)


BEGIN(Client)
class CSpider final : public CMonster
{
public:
	enum CRYINGGIRL { SPIDER_OBB, SPIDER_SPHERE0, CRY_END };


private: 
	explicit		 CSpider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CSpider(const CSpider& Prototype);
	virtual			~CSpider() = default;


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

	_bool			TraceDynamite(_float fTimeDelta);
	void			Action_Change(_float fTimeDelta);
	void			Action_Walk(_float fTimeDelta);
	void			Action_Walk_Back(_float fTimeDelta);
	void			Action_Splint(_float fTimeDelta);

	void			Door_Bump(_float fTimeDelta);


public:
	static CSpider*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

private:
	CCollider*	m_pColliderSpider[CRY_END] = { nullptr, nullptr };
	_bool		m_bFindPlayer = { false };
	_bool		m_bChasePlayer = { false };
	_bool		m_bReturing = { false };

	_float		m_fHideCurrentTime = { 0.f };
	_uint		m_iState = { 0 };

	_float		m_fDoorBumpBackTime = { 0.f };


	_float		m_fPlaySound = { 0.0f };
	_float const m_confIntervalSound = { 3.3f };

	_float		m_fPlayLookAtSoundTime = { 0.0f };
	_float const m_confPlayLookAtSoundTime = { 15.f };
	

	//MONSTER_DESC* 

};

END
