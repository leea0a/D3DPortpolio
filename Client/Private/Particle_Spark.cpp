#include "stdafx.h"
#include "Particle_Spark.h"

#include "GameInstance.h"
#include "Player.h"
#include "UI_Manager.h"


CParticle_Spark::CParticle_Spark(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{
}

CParticle_Spark::CParticle_Spark(const CParticle_Spark & Prototype)
	: CGameObject ( Prototype )
{
}

HRESULT CParticle_Spark::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Spark::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-283.f, -6.75f, 63.465f, 1.f));
	

	return S_OK;
}

void CParticle_Spark::Priority_Update(_float fTimeDelta)
{
	
}

_int CParticle_Spark::Update(_float fTimeDelta)
{	
	// m_pVIBufferCom->Drop(fTimeDelta);
	//m_pVIBufferCom->Spark(fTimeDelta);
	m_pVIBufferCom->Spark(fTimeDelta);
	
	
	return OBJ_NOEVENT;
}

void CParticle_Spark::Late_Update(_float fTimeDelta)
{	

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONLIGHT, this);
}

HRESULT CParticle_Spark::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	
	m_pShaderCom->Begin(1);
	m_pVIBufferCom->Bind_BufferDesc();
	m_pVIBufferCom->Render();
	
	return S_OK;
}



HRESULT CParticle_Spark::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Particle_Explosion_Lv1"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_Spark"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Spark::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	//if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
	//	return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", /*CUI_Manager::GetInstance()->Get_Camera()->Get_f4Pos()*/m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CParticle_Spark * CParticle_Spark::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CParticle_Spark*	pInstance = new CParticle_Spark(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Spark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CParticle_Spark::Clone(void * pArg)
{
	CParticle_Spark*	pInstance = new CParticle_Spark(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Spark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Spark::Free()
{
	__super::Free();
	
	Safe_Release(m_pShaderCom);	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);


	

	//Safe_Release(m_pVIBufferCom);
	
}
