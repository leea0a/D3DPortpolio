#include "..\Public\Cell.h"

#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CCell::Initialize(const _float3 * pPoints, _int iIndex)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	m_iIndex = iIndex;

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

	return S_OK;
}
_bool CCell::isIn(_fvector vLocalPos, _int& iNeighborIndex)
{
	for (size_t i = 0; i < LINE_END; i++)
	{
		_vector		vLine = XMLoadFloat3(&m_vPoints[(i + 1) % POINT_END]) - XMLoadFloat3(&m_vPoints[i]);
		_vector		vNormal = XMVectorSet(vLine.m128_f32[2] * -1.f, 0.f, vLine.m128_f32[0], 0.f);		
		_vector		vDir = vLocalPos - XMLoadFloat3(&m_vPoints[i]);

		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vNormal), XMVector3Normalize(vDir))))
		{
			iNeighborIndex = m_iNeighborIndices[i];
			return false;
		}
	}

	return true;
}

_bool CCell::Compare_Points(const _float3 & SourPoint, const _float3 & DestPoint)
{
	/*XMVectorEqual();
	XMVector3Equal();*/

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&SourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&DestPoint)))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&DestPoint)))
			return true;		
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&SourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&DestPoint)))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&DestPoint)))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&SourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&DestPoint)))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&DestPoint)))
			return true;
	}

	return false;
}

_float CCell::Compute_Height(_fvector vPosition)
{
	_vector		vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&m_vPoints[POINT_C]));	

	/* ax + by + cz + d = 0*/

	/* y = (-ax - cz - d) / b */

	return (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition) - XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition) - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);	
}

void CCell::AdjustPoints(TRIANGLE_VERTICES vTriangle)
{
	memcpy(m_vPoints, &vTriangle, sizeof(_float3) * POINT_END);
}

void CCell::Set_Normal()
{
	_vector vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&m_vPoints[POINT_C]));
	m_Normal.x = XMVectorGetX(vPlane);
	m_Normal.y = XMVectorGetY(vPlane);
	m_Normal.z = XMVectorGetZ(vPlane);
}

_vector CCell::Get_Line_To_Slide(_fvector vObjPos)
{
	_vector vCenter = (XMLoadFloat3(&m_vPoints[POINT_A]) + XMLoadFloat3(&m_vPoints[POINT_B]) + XMLoadFloat3(&m_vPoints[POINT_C])) / 3.f;
	_float3 vCenterFloat3;
	XMStoreFloat3(&vCenterFloat3, vCenter);

	/*�ﰢ���� �ﰢ���� �߽� �� �������� ������ 3���� �ﰢ������ ������. (������ ���ؼ� � ���п� �����̵� ���͸� �������� ���Ѵ�. */
	_float3 vTriangles[3][3];
	vTriangles[0][0] = m_vPoints[POINT_A];
	vTriangles[0][1] = m_vPoints[POINT_B];
	vTriangles[0][2] = vCenterFloat3;

	vTriangles[1][0] = m_vPoints[POINT_B];
	vTriangles[1][1] = m_vPoints[POINT_C];
	vTriangles[1][2] = vCenterFloat3;

	vTriangles[2][0] = m_vPoints[POINT_C];
	vTriangles[2][1] = m_vPoints[POINT_A];
	vTriangles[2][2] = vCenterFloat3;

	for (int i = 0; i < 3; ++i)
	{

		/*if (-1 != m_iNeighborIndices[i])
			continue;*/

		_bool bIsIn = true;
		_vector vLine = {};

		for (int j = 0; j < 3; ++j)
		{

			vLine = XMLoadFloat3(&vTriangles[i][(j + 1) % 3]) - XMLoadFloat3(&vTriangles[i][j]);
			//(���� ����)(x,0,z)-> (-z,0,x)(��� ���)
			_vector vNormal = XMVectorSet(vLine.m128_f32[2] * -1.f, 0.f, vLine.m128_f32[0], 0.f);
			//(�ð���� ������ �븻 dot ����ġ- ������ ������)
			_vector vDir = vObjPos - XMLoadFloat3(&vTriangles[i][j]); //����ġ���� �������� ����
			//�ϳ��� ����� �ȿ� ���°Ŵ�
			// (���� ������ �������� �ܺο� �־�� ����� ����) 
			if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vNormal), XMVector3Normalize(vDir))))
			{
				bIsIn = false;
			}

		}
		if (true == bIsIn)
		{		//������ �����ϴ� �ﰢ���� ��ȯ
			vLine = XMVector3Normalize(XMLoadFloat3(&vTriangles[i][1]) - XMLoadFloat3(&vTriangles[i][0]));
			return vLine;
		}
	}


	return _vector();
}

_bool CCell::Check_Near_To_Points(_fvector vObjPos)
{
	_float fDist = 0.f;
	for (int i = 0; i < 3; ++i)
	{
		_vector vDistVec = (XMVectorSetW(vObjPos, 0.f) - XMLoadFloat3(&m_vPoints[i]));

		if (0.1f > (fDist = XMVectorGetX(XMVector3Length(vDistVec))))
			return true;
	}

	return false;
}

#ifdef _DEBUG

HRESULT CCell::Render()
{
	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();

	return S_OK;
}
#endif
CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _float3 * pPoints, _int iIndex)
{
	CCell*	pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCell::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
