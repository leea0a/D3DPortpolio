#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_AABB final : public CBounding
{
public:
	typedef struct : public CBounding::BOUND_DESC
	{
		_float3		vExtents;
	}BOUND_AABB_DESC;
private:
	CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_AABB() = default;

public:
	const BoundingBox* Get_Desc() const {
		return m_pBoundDesc;
	}

	virtual _float3 Get_Extents() { return m_pOriginalBoundDesc->Extents; }
	virtual void Set_Extents(_float3 _Extents) { m_pOriginalBoundDesc->Extents = _Extents; } 

	virtual void	Set_Radius(_float	_Radius) {  };
	virtual _float3	Get_Pos() { return m_pOriginalBoundDesc->Center; }
	virtual _float Get_Radius() { return 0.0f; }
	

public:
	virtual HRESULT Initialize(const BOUND_DESC* pBoundDesc);
	virtual void Update(_fmatrix WorldMatrix) override;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
	virtual _bool Intersect(CCollider::COLLIDERTYPE eType, CBounding* pTargetBounding) override;

private:
	BoundingBox*					m_pOriginalBoundDesc = {};
	BoundingBox*					m_pBoundDesc = {};

private:
	_bool Intersect_to_AABB(CBounding_AABB* pTarget);

public:
	static CBounding_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUND_DESC* pBoundDesc);
	virtual void Free() override;
};
 
END