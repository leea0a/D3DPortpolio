#include "stdafx.h"
#include "..\Public\Sky.h"

#include "GameInstance.h"

CSky::CSky(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CSky::CSky(const CSky & Prototype)
	: CGameObject( Prototype )
{
}

HRESULT CSky::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSky::Initialize(void * pArg)
{	
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSky::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

_int CSky::Update(_float fTimeDelta)
{	
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(m_pGameInstance->Get_CamPosition()));
	return OBJ_NOEVENT;
	
}

void CSky::Late_Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pCamera->Get_Transform_For_Player()->Get_State(CTransform::STATE_POSITION));
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CSky::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSky::Ready_Components()
{


	if (m_pGameInstance->Get_Level() == 2)
	{
		/* Com_Shader */
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxCube"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		/* Com_VIBuffer */
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_VIBuffer_Cube"),
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Texture_SkyBox"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	else if (m_pGameInstance->Get_Level() == 3)
	{
		/* Com_Shader */
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		/* Com_VIBuffer */
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SkyBox"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CSky::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;



	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CSky * CSky::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSky*	pInstance = new CSky(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSky");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSky::Clone(void * pArg)
{
	CSky*	pInstance = new CSky(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSky");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSky::Free()
{
	__super::Free();
	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pCamera);
}
