#include "stdafx.h"
#include "Mirror_Use.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CMirror_Use::CMirror_Use(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CMirror_Use::CMirror_Use(const CMirror_Use& Prototype)
	: CProp(Prototype)
{
}

HRESULT CMirror_Use::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CMirror_Use::Initialize(void* pArg)
{
	PROP_DESC pDesc;

	pDesc.fSpeedPerSec = 1.f;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = {0.f, 0.f, 0.f, 1.f};
	pDesc.vScale = { 0.001f , 0.001f, 0.001f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;


	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	Set_PropType(TYPE_COMPASS);

	m_pTransformCom->Scaling(0.001f, 0.001f, 0.001f);

	return S_OK;
}
 
void CMirror_Use::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CMirror_Use::Update(_float fTimeDelta)
{
	if (CUI_Manager::GetInstance()->Get_ChooseIndex() != -1)
	{
		if (CUI_Manager::GetInstance()->Get_Inventory()[CUI_Manager::GetInstance()->Get_ChooseIndex()]->Get_ItemType() == CInventory::ITEM_MIRROR)
		{
			_vector CameraPos = CUI_Manager::GetInstance()->Get_Camera()->Get_Position();
			_vector Look = CUI_Manager::GetInstance()->Get_Camera()->Get_Look();
			_vector Right = CUI_Manager::GetInstance()->Get_Camera()->Get_Right();
			_vector Up = CUI_Manager::GetInstance()->Get_Camera()->Get_Up();
			_vector offset = Right - Up * 0.14f + Look * 1.5;
			_vector NewPos = CameraPos + offset;

			m_pTransformCom->Set_State(CTransform::STATE_RIGHT, CUI_Manager::GetInstance()->Get_Camera()->Get_Transform_For_Player()->Get_State(CTransform::STATE_RIGHT) * 0.03f);
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, -(CUI_Manager::GetInstance()->Get_Camera()->Get_Transform_For_Player()->Get_State(CTransform::STATE_LOOK)) * -0.04f);
			m_pTransformCom->Set_State(CTransform::STATE_UP, CUI_Manager::GetInstance()->Get_Camera()->Get_Transform_For_Player()->Get_State(CTransform::STATE_UP) * 0.04f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, NewPos);
		}
	}	


	//if (CUI_Manager::GetInstance()->Get_Player()->Get_MirrorUsePlayer() == true)
	//{
	//	m_pGameInstance->Render_Fade(fTimeDelta);
	//
	//	m_fUseTime += fTimeDelta;
	//
	//	if (m_fUseTime >= 2.f)
	//	{
	//		CUI_Manager::GetInstance()->Get_Player()->Set_MirrorUsePlayer(false);
	//	}
	//}


	return OBJ_NOEVENT;
}

void CMirror_Use::Late_Update(_float fTimeDelta)
{
	if (CUI_Manager::GetInstance()->Get_ChooseIndex() != -1)
	{
		if (CUI_Manager::GetInstance()->Get_Inventory()[CUI_Manager::GetInstance()->Get_ChooseIndex()]->Get_ItemType() == CInventory::ITEM_MIRROR)
		{
			m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
		}
	}
}

HRESULT CMirror_Use::Render()
{
	if (m_bChooseObj == false)
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
	}


	return S_OK;
}

HRESULT CMirror_Use::Ready_Components(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Mirror"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMirror_Use::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CMirror_Use* CMirror_Use::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMirror_Use* pInstance = new CMirror_Use(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CMirror_Use Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMirror_Use::Clone(void* pArg)
{
	CMirror_Use* pInstance = new CMirror_Use(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CMirror_Use Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMirror_Use::Free()
{
	__super::Free();
	//Safe_Release(m_pColliderCom);
}

