#pragma once

#include "Base.h"

BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public:
	_float3 Get_Point(POINT ePoint) const {
		return m_vPoints[ePoint];
	}

	void Set_Neighbor(LINE eLine, CCell* pNeighborCell) { m_iNeighborIndices[eLine] = pNeighborCell->m_iIndex; }

public:
	HRESULT Initialize(const _float3 * pPoints, _int iIndex);
	_bool isIn(_fvector vLocalPos, _int& iNeighborIndex);

	/* �������� Ÿ���� �ݹ���Value */
	/* ��������� Ÿ���� ����Ʈ�δ� �ݹ��̷��۷��� */
	/* Ư���� ��쿡�� �ݹ��� ������ */
	_bool Compare_Points(const _float3& SourPoint, const _float3& DestPoint);
	_float Compute_Height(_fvector vPosition);

	void AdjustPoints(TRIANGLE_VERTICES vTriangle);

	_vector Get_Normal() { return XMLoadFloat3(&m_Normal); }
	void    Set_Normal();
	_vector Get_Line_To_Slide(_fvector vObjPos);
	_bool Check_Near_To_Points(_fvector vObjPos);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif

private:
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };

private:
	_int							m_iIndex = { -1 };	
	_float3							m_vPoints[POINT_END] = {};
	_int							m_iNeighborIndices[LINE_END] = { -1, -1, -1 };

	_float3							m_Normal = {};

#ifdef _DEBUG
private:
	class CVIBuffer_Cell*			m_pVIBuffer = { nullptr };
#endif

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex);
	virtual void Free() override;
};

END