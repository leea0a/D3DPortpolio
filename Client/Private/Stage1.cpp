#include "stdafx.h"
#include "Stage1.h"
#include "GameInstance.h"

CStage1::CStage1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CStage{pDevice, pContext}
{
}

CStage1::CStage1(const CStage1& Prototype)
	: CStage(Prototype)
{
}

HRESULT CStage1::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CStage1::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));

	return S_OK;
}

void CStage1::Priority_Update(_float fTimeDelta)
{
}

_int CStage1::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CStage1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CStage1::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}
	
	return S_OK;
}

HRESULT CStage1::Ready_Components()
{
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stage1"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CStage1* CStage1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStage1* pInstance = new CStage1(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CStage1 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CStage1::Clone(void* pArg)
{
	CStage1* pInstance = new CStage1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CStage1 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage1::Free()
{
	__super::Free();
}

