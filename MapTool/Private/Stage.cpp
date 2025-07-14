
#include "stdafx.h"

#include "Stage.h"
#include "GameInstance.h"


CStage::CStage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CStage::CStage(const CStage& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CStage::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CStage::Initialize(void* pArg)
{
	 if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	//오브젝트에서 트랜 스폼 생성
	if (nullptr == m_pTransformCom)
	{
		MSG_BOX("failed to create in Terrain Base transform com");
		return E_FAIL;
	}
	TERRAIN_BASE_DESC* pDesc = static_cast<TERRAIN_BASE_DESC*>(pArg);

	if (nullptr != pDesc)
	{
		m_Desc.vAxis = pDesc->vAxis;
		m_Desc.vPosition = pDesc->vPosition;
		m_Desc.vScale = pDesc->vScale;
		m_Desc.fRotation = pDesc->fRotation;
		m_Desc.fRotation = pDesc->fSpeedPerSec;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_Desc.vPosition));
		m_pTransformCom->Scaling(m_Desc.vScale.x, m_Desc.vScale.y, m_Desc.vScale.z);
		m_pTransformCom->Rotation(XMLoadFloat4(&m_Desc.vAxis), m_Desc.fRotation);
	}
	else
	{
		m_Desc.vAxis = { 0.f,1.f,0.f,0.f };
		m_Desc.vPosition = { 0.f,0.f,0.f,1.f };
		m_Desc.vScale = { 1.f,1.f,1.f };
		m_Desc.fRotation = 0.f;
		m_Desc.fRotationPerSec = 1;
		m_Desc.fRotationPerSec = 1.f;
	}

	return S_OK;

	return S_OK;
}

void CStage::Priority_Update(_float fTimeDelta)
{
}

_int CStage::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CStage::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CStage::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;
		
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	m_pShader_For_ColliderCom->Begin(0);
	m_pVIBuffer_ColliderCom->Bind_BufferDesc();
	m_pContext->RSSetState(m_CullMode);
	m_pVIBuffer_ColliderCom->Render();
	m_pContext->RSSetState(m_NoneCullMode);
	
#ifdef _DEBUG
	m_pNavigationCom->Render();
#endif

	return S_OK;
}

void CStage::Update_Transform_By_Desc(TERRAIN_BASE_DESC Desc)
{
	_float3  vScale = Desc.vScale;
	m_pTransformCom->Scaling(vScale.x, vScale.y, vScale.z);
	_vector vPosition = XMLoadFloat4(&Desc.vPosition);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	_float fRot = Desc.fRotation;
	_vector vAxis = XMVector3Normalize(XMLoadFloat4(&Desc.vAxis));
	m_pTransformCom->Rotation(vAxis, fRot);
	m_Desc = Desc;

	return;
}

void CStage::Update_Transform_Go_Straight(const _float fTimeDelta)
{
	//if (fTimeDelta >= 0.1f)
	//	int a = 0;
	//m_pTransformCom->Go_Straight(fTimeDelta);
	//XMStoreFloat4(&m_Desc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

}

void CStage::Update_Transform_Go_Backward(const _float fTimeDelta)
{
}


void CStage::Add_Triangle_To_Navigation(TRIANGLE_VERTICES Triangle)
{
	m_pNavigationCom->Add_Cells(Triangle);
}

void CStage::AdjustCurTriangle(TRIANGLE_VERTICES Triangle)
{
	m_pNavigationCom->Edit_CurCell(Triangle);
}

void CStage::Pop_Triangle()
{
	m_pNavigationCom->Pop_Cell();
}

//void CStage::Update_Transform_By_Desc(MONSTER_DESC Desc)
//{
//	m_Desc = Desc;
//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&Desc.vPosition));
//	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), Desc.fAngle);
//	m_pTransformCom->Scaling(Desc.vScale.x, Desc.vScale.y, Desc.vScale.z);
//	BoundingBox Box;
//	Box.Center = { 0.f,0.f,0.f };
//	Box.Extents = { 0.5f,0.5f,0.5f };
//	Box.Transform(Box, m_pTransformCom->Get_WorldMatrix());
//	m_Box = Box;
//	
//	return;
//}
//
//void CStage::Update_Transform_Go_Straight(const _float fTimeDelta)
//{
//	if (fTimeDelta > 5.f)
//		int a = 0;
//	m_pTransformCom->Go_Straight(fTimeDelta);
//	XMStoreFloat4(&m_Desc.vPosition,(m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
//
//	BoundingBox Box;
//	Box.Center = { 0.f,0.f,0.f };
//	Box.Extents = { 0.5f,0.5f,0.5f };
//	Box.Transform(Box, m_pTransformCom->Get_WorldMatrix());
//	m_Box = Box;
//}
//
//void CStage::Update_Transform_Go_Backward(const _float fTimeDelta)
//{
//	m_pTransformCom->Go_Backward(fTimeDelta);
//}
//
HRESULT CStage::Ready_Components()
{
	/* Com_Shader */
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;

	/* Com_Model */
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster1"),
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;

	return S_OK;

}

HRESULT CStage::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage::Bind_ShaderResources_For_Collider()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader_For_ColliderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShader_For_ColliderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	
	if (FAILED(m_pShader_For_ColliderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CStage* CStage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStage* pInstance = new CStage(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CStage::Clone(void* pArg)
{
	CStage* pInstance = new CStage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Stage Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_CullMode);
	Safe_Release(m_NoneCullMode);
	Safe_Release(m_pTransformCom_ForCollider);
	Safe_Release(m_pShader_For_ColliderCom);
	Safe_Release(m_pVIBuffer_ColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}

