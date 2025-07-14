#include "stdafx.h"
#include "Dead.h"


CDead::CDead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI{ pDevice, pContext }
{
}

CDead::CDead(const CDead& Prototype)
	: CUI{ Prototype }
{
}

HRESULT CDead::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDead::Initialize(void* pArg)
{
	UIOBJDESC* pDesc = static_cast<UIOBJDESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	//m_OriginFx = m_fX;
	//m_OriginFy = m_fY;
	//wsprintf(m_DeadWord, TEXT("Put it Space"));


	return S_OK;
}

void CDead::Priority_Update(_float fTimeDelta)
{
}

_int CDead::Update(_float fTimeDelta)
{

	return OBJ_NOEVENT;
}

void CDead::Late_Update(_float fTimeDelta)
{
	//__super::Late_Update(fTimeDelta);

	if (CUI_Manager::GetInstance()->Get_Player()->Get_PlayerDeath() == true)
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
		
	}
	
}

HRESULT CDead::Render()
{
	__super::Render();

	if (CUI_Manager::GetInstance()->Get_Player()->Get_PlayerDeath() == true)
	{
		m_pGameInstance->Render_Font(TEXT("Cormorant_36_Sharp"), TEXT("Push Enter Key"), _float2(850, 630), XMVectorSet(1.f, 1.f, 1.f, 1.f));
	}
	

	return S_OK;
}

HRESULT CDead::Ready_Components()
{

	
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Dead"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	
	return S_OK;
}

void CDead::Dead_Output()
{
	
}



CDead* CDead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDead* pInstance = new CDead(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CDead Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CDead::Clone(void* pArg)
{
	CDead* pInstance = new CDead(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CDead Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDead::Free()
{

	__super::Free();

}
