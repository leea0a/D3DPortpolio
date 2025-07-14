#include "stdafx.h"
#include "Magatama.h"
#include "UI_Manager.h"


CMagatama::CMagatama(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI {pDevice, pContext}
{
}

CMagatama::CMagatama(const CMagatama& Prototype)
	: CUI { Prototype }
{
}

HRESULT CMagatama::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMagatama::Initialize(void* pArg)
{
	UIOBJDESC* pDesc = static_cast<UIOBJDESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CMagatama::Priority_Update(_float fTimeDelta)
{
}

_int CMagatama::Update(_float fTimeDelta)
{
	m_MagatamaCount = CUI_Manager::GetInstance()->Get_MagatamaCount();

	return OBJ_NOEVENT;
}

void CMagatama::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	wsprintf(m_MagatamaWord, TEXT("%d / 3"), m_MagatamaCount);
}

HRESULT CMagatama::Render()
{
	m_pGameInstance->Render_Font(TEXT("Chiller_28"), m_MagatamaWord, _float2(m_fX + 18, m_fY - 20), XMVectorSet(1.f, 1.f, 1.f, 1.f));
	__super::Render();
	
	return S_OK;
}

HRESULT CMagatama::Ready_Components()
{
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Magatama"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CMagatama* CMagatama::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMagatama* pInstance = new CMagatama(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CMagatama Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMagatama::Clone(void* pArg)
{
	CMagatama* pInstance = new CMagatama(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CMagatama Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMagatama::Free()
{
	
	__super::Free();
	
}
