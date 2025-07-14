#pragma once

#include "Component.h"

/* �ٸ� ������Ʈ�ʹ� �� �ٸ��� ��ü ���� �� ������ Ʈ������ ������Ʈ�� ������ �ְ� �Ұ��̴�. */
/* ��ü�� ���� ���¸� ǥ���Ѵ�.(m_WorldMatrix) */
/* ��ü�� ���� ���� ��ȯ�� ����� �����Ѵ�. ������ ����, ȸ���Ѵ�. �ٶ󺻴�.  */
/* ��������� �������� ������ �� �ֵ��� ���̴��� ��������� ������ ����� �����Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

	typedef struct
	{
		_float		fSpeedPerSec{};
		_float		fRotationPerSec{};

	}TRANSFORM_DESC;
private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState], vState);
	}

	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_matrix Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}
	_matrix Get_WorldMatrix_Inverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

	void Set_Speed(_float _speed) { m_fSpeedPerSec = _speed; }
	_float Get_Speed() { return m_fSpeedPerSec; }

	_float3 Compute_Scaled();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void Scaling(_float fX, _float fY, _float fZ);
	_bool Go_Straight(_float fTimeDelta, class CNavigation* pNavigation);
	void Go_Backward(_float fTimeDelta, class CNavigation* pNavigation);
	void Go_Left(_float fTimeDelta, class CNavigation* pNavigation);
	void Go_Right(_float fTimeDelta, class CNavigation* pNavigation);


	void Go_Straight_for_Camera(_float fTimeDelta);
	void Go_Backward_for_Camera(_float fTimeDelta);
	void Go_Left_for_Camera(_float fTimeDelta);
	void Go_Right_for_Camera(_float fTimeDelta);

	void LookAt(_fvector vAt);

	/* ���� ȸ���� �������� �߰��� ������ �ӵ��� ȸ���Ѵ�. */
	void Turn(_fvector vAxis, _float fTimeDelta);
	/* �׵���¸� �������� ������ ������ ȸ���Ѵ�. */
	void Rotation(_fvector vAxis, _float fRadian);
	_float	Get_RotationPerSec() { return m_fRotationPerSec; }
	void	Set_RotationPerSec(_float Angle) { m_fRotationPerSec = Angle; }

	_vector Get_SlidingFromLookVector(_fvector vLook, class CNavigation* pNavigation);

public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);


private:
	/* rowmajor*/
	_float4x4				m_WorldMatrix = {};

	_float					m_fSpeedPerSec = {};
	_float					m_fRotationPerSec = {};

	_float3		m_PrevLine = {};
	_uint		m_iPrevIndex = {};
	_int		m_iPrevSign = { 0 };

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END