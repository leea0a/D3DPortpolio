#include "stdafx.h"
#include "RunngerGauge.h"
#include "UI_Manager.h"


CRunnerGauge::CRunnerGauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI {pDevice, pContext}
{
}

CRunnerGauge::CRunnerGauge(const CRunnerGauge& Prototype)
	: CUI { Prototype }
{
}

HRESULT CRunnerGauge::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRunnerGauge::Initialize(void* pArg)
{
	UIOBJDESC* pDesc = static_cast<UIOBJDESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CRunnerGauge::Priority_Update(_float fTimeDelta)
{
}

_int CRunnerGauge::Update(_float fTimeDelta)
{
	
	return OBJ_NOEVENT;
}

void CRunnerGauge::Late_Update(_float fTimeDelta)
{
	_float fPlayerRuntime = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerRunTime() * 5;

	if (fPlayerRuntime < 0)
	{
		fPlayerRuntime = 0;
	}

	if (CUI_Manager::GetInstance()->Get_Player()->Get_PlayerRunTime() <= 0)
	{
		fPlayerRuntime = 0.001f;
	}

	m_pTransformCom->Scaling(fPlayerRuntime, 15.f, 15.f);

	if (CUI_Manager::GetInstance()->Get_Player()->Get_PlayerRunTime() < 100.f)
	{
		__super::Late_Update(fTimeDelta);
	}	

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CRunnerGauge::Render()
{
	//__super::Render();
	
	_float fRunnerGague = (CUI_Manager::GetInstance()->Get_Player()->Get_PlayerRunTime());
	fRunnerGague /= 100;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Bind_RawValue("g_DisappearGuage", &fRunnerGague, sizeof(_float));

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CRunnerGauge::Ready_Components()
{
	if (m_pGameInstance->Get_Level() == 2)
	{
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxPosTex"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		/* Com_VIBuffer */
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_VIBuffer_Rect"),
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Texture_UI_RunnerGauge"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	

 	else if (m_pGameInstance->Get_Level() == 3)
	{
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;
	
		/* Com_VIBuffer */
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
			return E_FAIL;
	
		/* Com_Texture */
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_RunnerGauge"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}



	return S_OK;
}

CRunnerGauge* CRunnerGauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRunnerGauge* pInstance = new CRunnerGauge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CRunnerGauge Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CRunnerGauge::Clone(void* pArg)
{
	CRunnerGauge* pInstance = new CRunnerGauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CRunnerGauge Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CRunnerGauge::Free()
{
	
	__super::Free();
	
}
