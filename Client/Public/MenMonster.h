#pragma once
#include "Monster.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CMenMonster final : public CMonster
{
public:
	enum MEN { MEN_OBB, MEN_SPHERE0, MEN_SPHERE1, MEN_END };

private:
	explicit		 CMenMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CMenMonster(const CMenMonster& Prototype);
	virtual			~CMenMonster() = default;


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

private:
	void			KidnapPlayer(_float fTimeDelta);
	_bool			TraceDynamite(_float fTimeDelta);
	void			TracePlayer(_float fTimeDelta);
	void			Back_DoorBump(_float fTimeDelta);

public:
	static CMenMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void			Free()override;

private:
	CCollider* m_pColliderMen[MEN_END] = { nullptr, nullptr, nullptr };
	_bool		m_bFindPlayer = { false };
	_bool		m_bChasePlayer = { false };
	_bool		m_bReturing = { false };

	_float		m_fHideCurrentTime = { 0.f };
	

	/* Door Bump ฐüทร*/
	_float m_fDoorBumpTime = { 0.0f };
	_float m_fDoorBumpBackTime = { 0.0f };


	_float		m_fPlaySound = { 0.0f };
	_float const m_confIntervalSound = { 1.f };


	_float		m_fPlayLookAtSoundTime = { 0.0f };
	_float const m_confPlayLookAtSoundTime = { 15.f };

	_float		m_fWalkSoundTime = { 0.0f };
	_float const m_fconWalkSoundTime = { 5.8f };


};

END
