#include "..\Public\Bounding_Sphere.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"

CBounding_Sphere::CBounding_Sphere(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBounding { pDevice, pContext }
{
}

HRESULT CBounding_Sphere::Initialize(const BOUND_DESC* pBoundDesc)
{
	const BOUND_SPHERE_DESC*		pDesc = static_cast<const BOUND_SPHERE_DESC*>(pBoundDesc);

	m_pOriginalBoundDesc = new BoundingSphere(pDesc->vCenter, pDesc->fRadius);
	m_pBoundDesc = new BoundingSphere(*m_pOriginalBoundDesc);

	return S_OK;
}

void CBounding_Sphere::Update(_fmatrix WorldMatrix)
{
	
	m_pOriginalBoundDesc->Transform(*m_pBoundDesc, WorldMatrix);
}

void CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundDesc, m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));
}

_bool CBounding_Sphere::Intersect(CCollider::COLLIDERTYPE eType, CBounding * pTargetBounding)
{
	m_isColl = false;

	switch (eType)
	{
	case CCollider::TYPE_AABB:
		m_isColl = m_pBoundDesc->Intersects(*static_cast<CBounding_AABB*>(pTargetBounding)->Get_Desc());
		break;

	case CCollider::TYPE_OBB:
		m_isColl = m_pBoundDesc->Intersects(*static_cast<CBounding_OBB*>(pTargetBounding)->Get_Desc());
		break;

	case CCollider::TYPE_SPHERE:
		m_isColl = m_pBoundDesc->Intersects(*static_cast<CBounding_Sphere*>(pTargetBounding)->Get_Desc());
		break;
	}

	return m_isColl;
}

CBounding_Sphere * CBounding_Sphere::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const BOUND_DESC * pBoundDesc)
{
	CBounding_Sphere*	pInstance = new CBounding_Sphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pBoundDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_Sphere");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBounding_Sphere::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalBoundDesc);
	Safe_Delete(m_pBoundDesc);
}
