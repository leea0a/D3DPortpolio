#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CDoor01 final : public CProp
{
public:
	typedef struct
	{
		_float3 vCenter = { 0, 0, 0 };
		_float3 vExtents = { 0, 0, 0 };
		_float3 vRotation = { 0.f, 0.f ,0.f };

	}COLLIDERDESC;

	enum HINGEDDOOR {COLLIDER_OBB0, COLLIDER_OBB1, COLLIDER_END};

private: 
	explicit		 CDoor01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CDoor01(const CDoor01& Prototype);
	virtual			~CDoor01() = default;


public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

protected:
	void					Set_InterAction(_float fTimeDelta) override;
	void					Open(_vector OriginPos, _float fTimeDelta);
	void					Close(_vector OriginPos,  _float fTimeDelta);
	_float					Get_PosX(_vector Pos) { return Pos.m128_f32[0]; }


private:
	HRESULT					Ready_Components(void* pArg);
	HRESULT					Bind_ShaderResources();

public:
	static CDoor01*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

private:
	_bool					m_bOpen = { false };
	CCollider*				m_pColliderDoor[COLLIDER_END] = { nullptr, nullptr };
	_bool					m_bOrigin = { false };
	_vector					m_OriginPos;
	_vector					m_ChangePos;



	_float3					m_DoorScale;
	_vector					m_OriginRight;
	_float					m_fCurrentAngle = {9};

};

END
