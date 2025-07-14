#include "stdafx.h"
#include "MiddlePoint.h"


CMiddlePoint::CMiddlePoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI {pDevice, pContext}
{
}

CMiddlePoint::CMiddlePoint(const CMiddlePoint& Prototype)
	: CUI { Prototype }
{
}

HRESULT CMiddlePoint::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMiddlePoint::Initialize(void* pArg)
{
	UIOBJDESC* pDesc = static_cast<UIOBJDESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CMiddlePoint::Priority_Update(_float fTimeDelta)
{
}

_int CMiddlePoint::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CMiddlePoint::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMiddlePoint::Render()
{
	__super::Render();
	
	return S_OK;
}

HRESULT CMiddlePoint::Ready_Components()
{
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MiddlePoint"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CMiddlePoint* CMiddlePoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMiddlePoint* pInstance = new CMiddlePoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CMiddlePoint Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMiddlePoint::Clone(void* pArg)
{
	CMiddlePoint* pInstance = new CMiddlePoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CMiddlePoint Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiddlePoint::Free()
{
	
	__super::Free();
	
}
