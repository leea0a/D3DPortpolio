#pragma once

#include "Component.h"



/* ��ü�� �������� �浹ü�� �ǹ��Ѵ�. */
/* Sphere, AABB, OBB */

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum COLLIDERTYPE { TYPE_SPHERE, TYPE_AABB, TYPE_OBB, TYPE_END };
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(COLLIDERTYPE eColliderType);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_fmatrix WorldMatrix);
	virtual HRESULT Render();

public:
	_bool Intersect(const CCollider* pTargetCollider);

public:
	class CBounding* Get_Bounding() { return m_pBounding; }


private:
	COLLIDERTYPE								m_eType = { TYPE_END };
	class CBounding*							m_pBounding = { nullptr };

#ifdef _DEBUG
	PrimitiveBatch<VertexPositionColor>*		m_pBatch = { nullptr };
	BasicEffect*								m_pEffect = { nullptr };
	ID3D11InputLayout*							m_pInputLayout = { nullptr };
#endif

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDERTYPE eColliderType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END