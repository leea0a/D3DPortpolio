#pragma once
#include "Prop.h"
#include "Client_Defines.h"
#include "Particle_Spark.h"
#include "Particle_SparkLv2.h"


BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CDynamite_Use final : public CProp
{

private:
	explicit		 CDynamite_Use(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CDynamite_Use(const CDynamite_Use& Prototype);
	virtual			~CDynamite_Use() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual _int	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Layer_SparkEffect(const _wstring& strLayerTag);
	HRESULT			Ready_Components(void* pArg);
	HRESULT			Bind_ShaderResources();
	void			Parabola(_float fTimeDelta);
	void			Parabola_2(_float fTimeDelta);

public:
	static CDynamite_Use* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void			Free()override;

private:
	_float4							m_f4OriginPos{};
	_float3							m_fCalPos{};
	_float							m_fTime = { 0.3f };
	_vector m_fUp, m_fLook, m_fRight;
	CNavigation* m_pNavigationCom = { nullptr };

	//class CVIBUffer

	class CParticle_Spark* m_pDynamiteSparkLv1 = { nullptr };
	class CParticle_SparkLv2* m_pDynamiteSparkLv2 = { nullptr };

	_float							m_fOriginalExtents	= {};
	_float							m_fChangeExtents	= {};

	_bool							m_bMade = { false };



	_float m_fSpeed = { 1.0f };
	_float m_fGravity = { 9.8f };
	_float m_fCosAngle = { cos(XMConvertToRadians(45.f)) };
	_float m_fSinAngle = { sin(XMConvertToRadians(45.f)) };

	_vector m_XMoveSpeed{};
	_float m_YMoveSpeed;



	_bool							m_bFlight	= { true };	// 날라가는 중이에요
	_bool							m_bArrive	= { false };	// 도착했어요
	
	_bool							m_bWait		= { false };	// 도착 후 대기해요
	_float							m_fWaitTime = { 0.0f };

	_bool							m_bBang		= { false };	// 터지고 있어요
	_float							m_fBangTime = { 0.0f };
	
	_bool							m_bDelete	= { false };	// 다 터져서 사라져야해요

	_bool							m_bMaxHeight = { false };
	_bool							m_bPara2 = { false };
	_uint							m_iDynamiteRightCount = { 0 };

	_float							m_fDynamiteLightTime = { 0.f };
	_float							m_fRandomLight = { 0.f };


	_bool							m_bPlayWait = { false };
	_bool							m_bPlayBomb = { false };
	_float							m_bSoundBomb = { false };
	_float							m_fPlayerBombCooltime = { 0.0f };
	_float							m_fBombIntervalTime = { 0.0f };

};

END
