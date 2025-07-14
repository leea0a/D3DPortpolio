#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CDoor00 final : public CProp
{
public:
	enum COLLIDERDOOR {COLLIDER_AABB0, COLLIDER_AABB1, COLLIDER_END};

private: 
	explicit		 CDoor00(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CDoor00(const CDoor00& Prototype);
	virtual			~CDoor00() = default;


public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual _int			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

protected:
	void					Set_InterAction(_float fTimeDelta) override;
	void					Open(_vector OriginPos);
	void					Close(_vector OriginPos);
	_float					Get_PosX(_vector Pos) { return Pos.m128_f32[0]; }
	_float					Get_PosZ(_vector Pos) { return Pos.m128_f32[2]; }


private:
	HRESULT					Ready_Components(void* pArg);
	HRESULT					Bind_ShaderResources();

public:
	static CDoor00*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

private:
	_bool					m_bOpen = { false };
	CCollider*				m_pColliderDoor[COLLIDER_END] = { nullptr, nullptr };
	_bool					m_bOrigin = { false };
	_vector					m_OriginPos;
	_vector					m_MaxPos;


};

END
