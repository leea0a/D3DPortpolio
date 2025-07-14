#include "..\Public\PartObj.h"

CPartObj::CPartObj(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{

}

CPartObj::CPartObj(const CPartObj & Prototype) 
	: CGameObject{ Prototype }
{

}

HRESULT CPartObj::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPartObj::Initialize(void * pArg)
{
	PARTOBJ_DESC*		pDesc = static_cast<PARTOBJ_DESC*>(pArg);

	m_pParentMatrix = pDesc->pParentMatrix;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CPartObj::Priority_Update(_float fTimeDelta)
{
}

_int CPartObj::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CPartObj::Late_Update(_float fTimeDelta)
{
}

HRESULT CPartObj::Render()
{
	return S_OK;
}

void CPartObj::Free()
{
	__super::Free();


}
