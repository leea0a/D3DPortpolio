#pragma once

#include "GameObject.h"
#include "Client_Defines.h"
#include "Engine_Macro.h"
//#include "Camera_POV.h"


BEGIN(Engine)
class CNavigation;
class CCollider;
class CShader;
class CModel;
class CVIBuffer_Collider;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	enum PLAERCOLLIDER { PCOLL_OBB1, PCOLL_OBB2, PCOLL_END };

public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		_float4		vPosition;
		_float3		vScale;
		_float		fAngle;
		_uint		iNaviMeshIndex;

	}PLAYER_DESC;

private:
	explicit CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual _int Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


	CTransform* Get_Transform() { return m_pTransformCom; }

public:
	CShader*		m_pShaderCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };
	CCollider*		m_pColliderCom[PCOLL_END] = {nullptr, nullptr};
	CNavigation*	m_pNavigationCom = { nullptr };

private:
	PLAYER_DESC				m_Desc;
	_vector					m_CameraView;
	_vector					m_CameraRight;

	_float4x4				m_WorldMatrix = {};
	_bool					m_bHide = { false };
	_vector					m_prePos;

	_vector					m_vCurrentPos = {};

	_uint					m_iNaviMeshIndex{};

	_float					m_PlayerPosY = { 0.f };
	
	// 플레이어 Extents 변경
	_bool					m_bLighterUse = { false };
	_bool					m_bHandLighterUse = { false };
	_bool					m_bSit = { false };


	// 플레이어 달리기 스피드
	_float					m_fOriginSpeed = { 0.f };
	_float3					m_f3OriginExtents = {};


	// 플레이어 스테미나
	_float					m_fRunTime = { 100.f };
	_bool					m_bRunPossible = { true };


	// 플레이어 무적 상태
	_bool					m_bUnhitable = { false };
	_float					m_fUnhitableTime = { 0.f };

	/* 플레이어 라이트 사용 상태*/
	_bool					m_bLighterUsePlayer = { false };
	_float					m_fLighterOnTime = { 0.0f };
	_float					m_fLightCoolTime = { 0.0f };
	_bool					m_bLightCool = { false };

	/* 플레이어 랜턴 사용 상태*/
	_bool					m_bHandLightUsePlayer = { false };

	_bool					m_bTest = { false };
	_bool					m_bTestNavigation = { false };

	/* 거울 사용 상태*/
	_bool					m_bMirriorUSePlayer = { false };

	/* SOUND 용*/
	_bool						m_bMoving = { false };
	_float						m_fSoundTimer = { 0.0f };
	const _float				m_confSoundInterval = { 3.f };

	_float						m_fWalkSoundTimer = { 0.0f };
	const _float				m_confWalkSoundInterval = { 2.5f };

	_float						m_fBreathSoundTimer = { 0.0f };
	const _float				m_confBreathSoundInterval = { 3.5f };


	/* DEATH */
	_bool						m_bDeath = { false };
	_float						m_fPlayerReviveSound = { 80.f };


	_float						m_fHeartBeatTime = { 0.0f };







private:
	void			Set_PlayerSpeed(_float fSpeed) { m_Desc.fSpeedPerSec = fSpeed; }
	void			Cal_PlayerRunSound(_float fTimeDelta);
	void			Cal_PlayerWalkSound(_float ftimeDelta);
	void			cal_PlayerDownWalkSound(_float fTimeDelta);

public:
	HRESULT			Ready_Components(void* pArg);
	HRESULT			Bind_ShaderResources();
	_vector			Get_Position();
	_vector			Set_CameraLook(_vector CameraView) { return m_CameraView = CameraView; }
	_vector			Set_CameraRight(_vector CameraView) { return m_CameraRight = CameraView; }
	_bool			Get_PlayerHide() { return m_bHide; }
	void			Set_PlayerHide(_bool bHide) { m_bHide = bHide; }
	_bool			Get_PlayerSit() { return m_bSit; }
	void			Set_PlayerSit(_bool bSit) { m_bSit = bSit; }

	void			Set_PrePos(_vector prePos) { m_prePos = prePos; }
	_vector			Get_PrePos() { return m_prePos; }
	_bool			Get_LighterUse() { return m_bLighterUse; }
	void			Set_LighterUse(_bool LighterUse) { m_bLighterUse = LighterUse; }
	_bool			Get_HandLighterUse() { return 
		m_bHandLighterUse; }
	void			Set_HandLighterUse(_bool HandLighterUse) { m_bHandLighterUse = HandLighterUse; }


	void			Set_Unhitable(_bool _Invincible) { m_bUnhitable = _Invincible; }
	_bool			Get_Unhitable() { return m_bUnhitable; }

	void			Set_UnhitableTime(_float _UnHitableTime) { m_fUnhitableTime = _UnHitableTime; }
	_float			Get_UnhitableTime() { return m_fUnhitableTime; }

	void			Set_MirrorUsePlayer(_bool Use) { 
		m_bMirriorUSePlayer = Use; 
	}
	_bool			Get_MirrorUsePlayer() { return m_bMirriorUSePlayer; }
	


	CNavigation*	Get_Navigation() { return m_pNavigationCom; }
	_float			Get_PlayerRunTime() { return m_fRunTime; }

	CCollider*		Get_PlayerCollider()		{ return m_pColliderCom[PCOLL_OBB1]; }

	_bool			Get_PlayerDeath() { return m_bDeath; }
	void			Set_PlayerDeath(_bool Life) { m_bDeath = Life; }

private:
	void			Player_Key(_float fTimeDelta);
	void			Player_Dead();

	HRESULT FireUse(_float fTimeDelta);
	HRESULT HandLightUse(_float fTimeDelta);

	void Update_PlayerPos();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;
};

END
