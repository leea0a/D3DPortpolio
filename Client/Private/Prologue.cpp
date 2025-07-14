#include "stdafx.h"
#include "Prologue.h"
#include "GameInstance.h"

CPrologue::CPrologue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CStage{pDevice, pContext}
{
}

CPrologue::CPrologue(const CPrologue& Prototype)
	: CStage(Prototype)
{
}

HRESULT CPrologue::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CPrologue::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));

	return S_OK;
}

void CPrologue::Priority_Update(_float fTimeDelta)
{
}

_int CPrologue::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CPrologue::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CPrologue::Render()
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

HRESULT CPrologue::Ready_Components()
{
	if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Model_Prologue_Stage"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPrologue::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CPrologue* CPrologue::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPrologue* pInstance = new CPrologue(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Prologue_Stage Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

CGameObject* CPrologue::Clone(void* pArg)
{
	CPrologue* pInstance = new CPrologue(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Prologue_Stage Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CPrologue::Free()
{
	__super::Free();
}

