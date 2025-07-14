#pragma once

#include "Client_Defines.h"
#include "Camera.h"
#include "Player.h"

BEGIN(Client)

class CCamera_POV final : public CCamera
{
public:
	typedef struct : public CCamera::CAMERA_DESC
	{
		_float4			vPosition = {};
		_float			fMouseSensor = {};
	}CAMERA_POV_DESC;

private:
	CCamera_POV(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_POV(const CCamera_POV& Prototype);
	virtual ~CCamera_POV() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual _int Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Position(_vector Position);
	void Set_Look(_vector Look);
	void Set_Player(CPlayer* pPlayer);
	CTransform* Get_Transform_For_Player() { return m_pTransformCom; }

	_vector Get_Position() { return m_pTransformCom->Get_State(CTransform::STATE_POSITION); }
	_float3 Get_Position_float3() 
	{ 
		return _float3(m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0],
			m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1],
			m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2]);
	}

	_float3 Get_Look_float3()
	{
		return _float3(m_pTransformCom->Get_State(CTransform::STATE_LOOK).m128_f32[0],
			m_pTransformCom->Get_State(CTransform::STATE_LOOK).m128_f32[1],
			m_pTransformCom->Get_State(CTransform::STATE_LOOK).m128_f32[2]);
	}

	_float3 Get_Up_float3()
	{
		return _float3(m_pTransformCom->Get_State(CTransform::STATE_UP).m128_f32[0],
			m_pTransformCom->Get_State(CTransform::STATE_UP).m128_f32[1],
			m_pTransformCom->Get_State(CTransform::STATE_UP).m128_f32[2]);
	}

	_vector Get_Look() { return m_pTransformCom->Get_State(CTransform::STATE_LOOK); }
	_vector Get_Up() { return m_pTransformCom->Get_State(CTransform::STATE_UP); }
	_vector Get_Right() { return m_pTransformCom->Get_State(CTransform::STATE_RIGHT); }

	_float4 Get_f4Look() { return { 
		m_pTransformCom->Get_WorldMatrix().r[2].m128_f32[0], 
		m_pTransformCom->Get_WorldMatrix().r[2].m128_f32[1], 
		m_pTransformCom->Get_WorldMatrix().r[2].m128_f32[2], 
		0.f }; }

	_float4 Get_f4Up() { return { 
		m_pTransformCom->Get_WorldMatrix().r[1].m128_f32[0], 
	m_pTransformCom->Get_WorldMatrix().r[1].m128_f32[1], 
	m_pTransformCom->Get_WorldMatrix().r[1].m128_f32[2], 
	0.f}; }

	_float4 Get_f4Right() { return { 
		m_pTransformCom->Get_WorldMatrix().r[0].m128_f32[0],
	m_pTransformCom->Get_WorldMatrix().r[0].m128_f32[1], 
	m_pTransformCom->Get_WorldMatrix().r[0].m128_f32[2], 
	0.f}; }

	_float4 Get_f4Position() {
		return {
m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0],
		m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1],
		m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2] , 1.f };
	}
	
	_float4* Get_f4Pos() { return m_pf4Position; }


private:
	void Fix_Mouse();
	void Free_Mouse(_float fTimeDelta);

private:
	_float			m_fMouseSensor = {};
	_bool			m_bFix = { true };
	_float4			m_Position = {};
	CPlayer*		m_pPlayer = { nullptr };


	_float3			m_fPosition = {};
	_float4*		m_pf4Position = { nullptr };
	_float4			m_f4Position = {};
	_float3			m_fLook = {};
	_float3			m_fUp = {};

public:
	static CCamera_POV* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
	
};


END