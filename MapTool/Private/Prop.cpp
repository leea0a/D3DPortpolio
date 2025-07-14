#include "stdafx.h"
#include "Prop.h"
#include "GameInstance.h"

//static vector<CInnerInventory*>* m_pInventory = { nullptr };

CProp::CProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CProp::CProp(const CProp& Prototype)
	:CGameObject(Prototype)
	, m_pColliderCom{ Prototype.m_pColliderCom }
{
}

HRESULT CProp::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CProp::Initialize(void* pArg)
{
	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);

	if (nullptr != pDesc) 
	{
		m_Desc.iIndex = pDesc->iIndex;
		m_Desc.vScale = pDesc->vScale;
		m_Desc.vPosition = pDesc->vPosition;
		m_Desc.fAngle = pDesc->fAngle;
	}
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr == pDesc && nullptr != m_pTransformCom)
	{
		m_pTransformCom->Scaling(1, 1, 1);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	}
	
	if (nullptr != pDesc)
	{
		_float4 Axis = { 0.f,1.f,0.f,0.f };
		m_pTransformCom->Scaling(m_Desc.vScale.x, m_Desc.vScale.y, m_Desc.vScale.z);
		m_pTransformCom->Rotation(XMLoadFloat4(&Axis), XMConvertToRadians(m_Desc.fAngle));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_Desc.vPosition));
	}

	return S_OK;
}

void CProp::Priority_Update(_float fTimeDelta)
{
}

_int CProp::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CProp::Late_Update(_float fTimeDelta)
{
	//CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));
	//
	//if (m_bChooseObj == false)
	//{
	//	_bool bInterSect = m_pColliderCom->Intersect(pTargetCollider);
	//
	//
	//	if (bInterSect == true)
	//	{
	//		if (KEY_DOWN(KEY::F))
	//		{
	//			m_bChooseObj = true;
	//
	//			InterAction(fTimeDelta);
	//		}
	//	}
	//
	//	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	//}
	//else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CProp::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
		//	return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}


	if (FAILED(Bind_ShaderResources_For_Collider()))
		return E_FAIL;

	{

		m_pShader_For_ColliderCom->Bind_RawValue("g_ColorR", &m_arrColiderColor[0], sizeof(_float));
		m_pShader_For_ColliderCom->Bind_RawValue("g_ColorG", &m_arrColiderColor[1], sizeof(_float));
		m_pShader_For_ColliderCom->Bind_RawValue("g_ColorB", &m_arrColiderColor[2], sizeof(_float));
	}
	m_pShader_For_ColliderCom->Begin(0);
	m_pVIBuffer_ColliderCom->Bind_BufferDesc();
	m_pContext->RSSetState(m_CullMode);
	m_pVIBuffer_ColliderCom->Render();
	m_pContext->RSSetState(m_NoneCullMode);
	return S_OK;
}

void CProp::InterAction(_float fTimedelta)
{
	//CInnerInventory* pInnerInventory = { nullptr };
	//
	//for (_int i = 0; i < CUI_Manager::GetInstance()->Get_Inventory().size(); ++i)
	//{
	//	if (CUI_Manager::GetInstance()->Get_Inventory()[i]->Get_ItemType() == CInventory::ITEM_EMPTY)
	//	{
	//		pInnerInventory = CUI_Manager::GetInstance()->Get_Inventory()[i];
	//		break;
	//	}
	//}
	//
	//switch (m_ePropType)
	//{
	//case TYPE_LIGHTER:
	//{
	//	pInnerInventory->Set_ItemType(CInventory::ITEM_LIGHTER);
	//	pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
	//
	//	if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
	//	{
	//		CUI_Manager::GetInstance()->Set_ChooseIndex(0);
	//	}
	//}
	//
	//break;
	//
	//case TYPE_MIRROR:
	//{
	//	pInnerInventory->Set_ItemType(CInventory::ITEM_MIRROR);
	//	pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
	//
	//	if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
	//	{
	//		CUI_Manager::GetInstance()->Set_ChooseIndex(0);
	//	}
	//}
	//break;
	//
	//case TYPE_HANDLIGHTER:
	//{
	//	pInnerInventory->Set_ItemType(CInventory::ITEM_HANDLIGHTER);
	//	pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
	//	if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
	//	{
	//
	//		CUI_Manager::GetInstance()->Set_ChooseIndex(0);
	//	}
	//}
	//break;
	//
	//case TYPE_COMPASS:
	//{
	//	pInnerInventory->Set_ItemType(CInventory::ITEM_COMPASS);
	//	pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
	//
	//	if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
	//	{
	//		CUI_Manager::GetInstance()->Set_ChooseIndex(0);
	//	}
	//}
	//break;
	//
	//case TYPE_KAGURA:
	//{
	//	pInnerInventory->Set_ItemType(CInventory::ITEM_KAGURA);
	//	pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
	//
	//	if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
	//	{
	//		CUI_Manager::GetInstance()->Set_ChooseIndex(0);
	//	}
	//}
	//break;
	//
	//case TYPE_GLOWSTONE:
	//{
	//	pInnerInventory->Set_ItemType(CInventory::ITEM_GLOWSTONE);
	//	pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
	//
	//	if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
	//	{
	//		CUI_Manager::GetInstance()->Set_ChooseIndex(0);
	//	}
	//}
	//break;
	//
	//case TYPE_KEY:
	//{
	//	pInnerInventory->Set_ItemType(CInventory::ITEM_KEY);
	//	pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
	//
	//	if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
	//	{
	//		CUI_Manager::GetInstance()->Set_ChooseIndex(0);
	//	}
	//}
	//break;
	//
	//case TYPE_SPHER:
	//{
	//	pInnerInventory->Set_ItemType(CInventory::ITEM_SPHER);
	//	pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
	//
	//	if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
	//	{
	//		CUI_Manager::GetInstance()->Set_ChooseIndex(0);
	//	}
	//}
	//break;
	//
	//case TYPE_DYNAMITE:
	//{
	//	pInnerInventory->Set_ItemType(CInventory::ITEM_DYNAMITE);
	//	pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + rand() % 4);
	//
	//	if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
	//	{
	//		CUI_Manager::GetInstance()->Set_ChooseIndex(0);
	//	}
	//}
	//break;
	//
	//case TYPE_CANDLE:
	//{
	//
	//}
	//break;
	//
	//case TYPE_CABINET:
	//{
	//	m_bCabinetIn = true;
	//}
	//break;
	//
	//case TYPE_BIGBOX:
	//{
	//	//m_bCabinetIn = true;
	//}
	//break;
	//
	//case TYPE_DOOR:
	//{
	//	m_bDoorOpenClose = true;
	//}
	//break;
	//}
}

void CProp::Update_Transform_By_Desc(PROP_DESC Desc)
{
	m_Desc = Desc;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&Desc.vPosition));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), Desc.fAngle);
	m_pTransformCom->Scaling(Desc.vScale.x, Desc.vScale.y, Desc.vScale.z);
	BoundingBox Box;
	Box.Center = { 0.f,0.f,0.f };
	Box.Extents = { 0.5f,0.5f,0.5f };
	Box.Transform(Box, m_pTransformCom->Get_WorldMatrix());
	m_Box = Box;

	return;
}

void CProp::Update_Transform_Go_Straight(const _float fTimeDelta)
{
	//if (fTimeDelta > 5.f)
	//	int a = 0;
	////m_pTransformCom->Go_Straight(fTimeDelta, );
	//XMStoreFloat4(&m_Desc.vPosition, (m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
	//
	//BoundingBox Box;
	//Box.Center = { 0.f,0.f,0.f };
	//Box.Extents = { 0.5f,0.5f,0.5f };
	//Box.Transform(Box, m_pTransformCom->Get_WorldMatrix());
	//m_Box = Box;
}

void CProp::Update_Transform_Go_Backward(const _float fTimeDelta)
{
	//if (fTimeDelta > 5.f)
	//	int a = 0;
	//m_pTransformCom->Go_Straight(fTimeDelta);
	//XMStoreFloat4(&m_Desc.vPosition, (m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
	//
	//BoundingBox Box;
	//Box.Center = { 0.f,0.f,0.f };
	//Box.Extents = { 0.5f,0.5f,0.5f };
	//Box.Transform(Box, m_pTransformCom->Get_WorldMatrix());
	//m_Box = Box;
}


HRESULT CProp::Ready_Components()
{
	return S_OK;
}

HRESULT CProp::Bind_ShaderResources()
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

HRESULT CProp::Bind_ShaderResources_For_Collider()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader_For_ColliderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShader_For_ColliderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader_For_ColliderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}


CProp* CProp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CProp* pInstance = new CProp(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CProp::Clone(void* pArg)
{
	CProp* pInstance = new CProp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CProp Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CProp::Free()
{
	__super::Free();

	Safe_Release(m_CullMode);
	Safe_Release(m_NoneCullMode);
	Safe_Release(m_pShader_For_ColliderCom);
	Safe_Release(m_pVIBuffer_ColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

}


