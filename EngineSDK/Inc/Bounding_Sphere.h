#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_Sphere final : public CBounding
{
public:
	typedef struct : public CBounding::BOUND_DESC
	{
		_float		fRadius;
	}BOUND_SPHERE_DESC;

private:
	CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_Sphere() = default;

public:
	const BoundingSphere* Get_Desc() const {
		return m_pBoundDesc;
	}
public:
	virtual HRESULT Initialize(const BOUND_DESC* pBoundDesc);
	virtual void Update(_fmatrix WorldMatrix) override;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
	virtual _bool Intersect(CCollider::COLLIDERTYPE eType, CBounding* pTargetBounding) override;

	/* GARA */
	virtual _float3 Get_Extents() { return _float3(0.f, 0.f, 0.f); }
	virtual _float Get_Radius() { return m_pOriginalBoundDesc->Radius; }


	virtual void Set_Extents(_float3 _Extents) {};
	virtual void Set_Radius(_float _Radius) {
		m_pOriginalBoundDesc->Radius = _Radius;
	}
	virtual _float3	Get_Pos() { return m_pBoundDesc->Center; }


private:
	BoundingSphere*					m_pOriginalBoundDesc = {};
	BoundingSphere*					m_pBoundDesc = {};

public:
	static CBounding_Sphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUND_DESC* pBoundDesc);
	virtual void Free() override;
};

END