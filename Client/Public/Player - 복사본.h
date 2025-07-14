#pragma once

#include "GameObject.h"
#include "Client_Defines.h"
#include "Engine_Macro.h"
//#include "Camera_POV.h"


BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
class CVIBuffer_Collider;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	//enum COLLIDER { COLL_AABB, COLL_OBB, COLL_SPHERE, COLL_END };

public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		_float4		vPosition;
		_float3		vScale;
		_float		fAngle;

	}PLAYER_DESC;

private:
	explicit CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


	CTransform*				Get_Transform() { return m_pTransformCom; }

public:
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	CCollider*				m_pColliderCom = {nullptr};

private:
	PLAYER_DESC				m_Desc;
	_vector					m_CameraView;
	_vector					m_CameraRight;

	_float4x4				m_WorldMatrix = {};
	_bool					m_bHide = { false };
	_vector					m_prePos;
	


public:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();
	_vector Get_Position();
	_vector Set_CameraLook(_vector CameraView) { return m_CameraView = CameraView; }
	_vector Set_CameraRight(_vector CameraView) { return m_CameraRight = CameraView; }
	_bool	Get_PlayerHide() { return m_bHide; }
	void	Set_PlayerHide(_bool bHide) { m_bHide = bHide; }
	void	Set_PrePos(_vector prePos) { m_prePos = prePos; }
	_vector Get_PrePos() { return m_prePos; }

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;
};

END
