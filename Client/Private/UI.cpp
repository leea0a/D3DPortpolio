#include "stdafx.h"
#include "UI.h"
#include "GameInstance.h"

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject {pDevice, pContext}
{
}

CUI::CUI(const CUI& Prototype)
    : CUIObject (Prototype)
{
}

HRESULT CUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    if (FAILED(Ready_Components()))
        return E_FAIL;


    return S_OK;
}

void CUI::Priority_Update(_float fTimeDelta)
{
}

_int CUI::Update(_float fTimeDelta)
{
    return OBJ_NOEVENT;
}

void CUI::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}



HRESULT CUI::Render(_int iTemType)
{
    if(FAILED(Bind_ShaderResources(iTemType)))
        return E_FAIL;


    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_BufferDesc();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI::Ready_Components()
{
    if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_StartLogo"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI::Bind_ShaderResources(_int iTemType)
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", iTemType)))
        return E_FAIL;

    return S_OK;
}


void CUI::Free()
{
    __super::Free();

   Safe_Release(m_pShaderCom);
   Safe_Release(m_pTextureCom);
   Safe_Release(m_pVIBufferCom);
}
