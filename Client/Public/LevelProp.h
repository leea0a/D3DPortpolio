#pragma once
#include "Prop.h"
#include "Client_Defines.h"
#include "Camera_POV.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CLevelProp final : public CProp
{
public:
	enum COLLIDERDOOR {COLLIDER_AABB0, COLLIDER_AABB1, COLLIDER_END};

private: 
	explicit		 CLevelProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CLevelProp(const CLevelProp& Prototype);
	virtual			~CLevelProp() = default;


public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual _int			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;
	void					Set_Camera(CCamera_POV* _Camera) { m_pCameraPOV = _Camera; Safe_AddRef(m_pCameraPOV); }

protected:
	void					Set_InterAction(_float fTimeDelta) override;
	_float					Get_PosX(_vector Pos) { return Pos.m128_f32[0]; }
				


private:
	HRESULT					Ready_Components(void* pArg);
	HRESULT					Bind_ShaderResources();

public:
	static CLevelProp*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

private:
	_bool					m_bOpen = { false };
	CCollider*				m_pColliderDoor[COLLIDER_END] = { nullptr, nullptr };
	_bool					m_bOrigin = { false };
	_vector					m_OriginPos;
	_vector					m_MaxPos;

	_bool					m_bHide = { false };
	_vector					m_vecPreInsidePosition;

	CCamera_POV*			m_pCameraPOV = { nullptr };

	_bool					m_bUse = { false };

};

END
