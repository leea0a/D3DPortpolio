#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_OBB final : public CBounding
{
public:
	typedef struct : public CBounding::BOUND_DESC
	{
		_float3		vExtents;
		_float3		vRotation;
	}BOUND_OBB_DESC;

	typedef struct
	{
		_float3		vCenter;
		_float3		vExtentDir[3];
		_float3		vAlignDir[3];
	}OBBDESC;

private:
	CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_OBB() = default;

public:
	const BoundingOrientedBox* Get_Desc() const {
		return m_pBoundDesc;
	}

public:
	virtual HRESULT Initialize(const BOUND_DESC* pBoundDesc);
	virtual void Update(_fmatrix WorldMatrix) override;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
	virtual _bool Intersect(CCollider::COLLIDERTYPE eType, CBounding* pTargetBounding) override;

	_float3 Get_OBB_Extents() { return m_pBoundDesc->Extents; }
	void	Set_OBB_Extents(_float3 _Extents) { m_pBoundDesc->Extents = _Extents; }

	virtual _float3 Get_Extents() { return m_pBoundDesc->Extents; }
	virtual void Set_Extents(_float3 _Extents) {
		m_pOriginalBoundDesc->Extents = _Extents;
	}

	virtual void	Set_Radius(_float	_Radius) {};
	virtual _float3	Get_Pos() { return m_pOriginalBoundDesc->Center; }
	virtual _float Get_Radius() { return 0.0f; }



private:
	BoundingOrientedBox*					m_pOriginalBoundDesc = {};
	BoundingOrientedBox*					m_pBoundDesc = {};

private:
	_bool Intersect_to_OBB(CBounding_OBB* pTarget);
	OBBDESC Compute_OBB();

public:
	static CBounding_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUND_DESC* pBoundDesc);
	virtual void Free() override;
};

END