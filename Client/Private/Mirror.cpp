#include "stdafx.h"
#include "Mirror.h"
#include "GameInstance.h"

CMirror::CMirror(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CMirror::CMirror(const CMirror& Prototype)
	: CProp(Prototype)
{
}

HRESULT CMirror::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CMirror::Initialize(void* pArg)
{
	PROP_DESC* pSetDesc = static_cast<PROP_DESC*>(pArg);

	PROP_DESC pDesc;



	pDesc.fSpeedPerSec = pSetDesc->fSpeedPerSec;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = pSetDesc->vPosition;
	pDesc.vScale = { 0.03f , 0.03f, 0.03f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = pSetDesc->iPropNo;





	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	Set_PropType(TYPE_MIRROR);

	return S_OK;
}

void CMirror::Priority_Update(_float fTimeDelta)
{
	if (m_bLife == true)
	{
		return;
	}
	__super::Priority_Update(fTimeDelta);
}

_int CMirror::Update(_float fTimeDelta)
{
	Use_Mirror(fTimeDelta);
	Player_Revive();
	return OBJ_NOEVENT;
}

void CMirror::Late_Update(_float fTimeDelta)
{
	if (m_bLife == true)
	{
		return;
	}

	__super::Late_Update(fTimeDelta);
}

HRESULT CMirror::Render()
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

		//m_pColliderCom->Render();
	}


	return S_OK;
}

HRESULT CMirror::Ready_Components(void* pArg)
{
	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);

	CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};
	ColliderSphereDesc.fRadius = 3.f;
	ColliderSphereDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z);

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderSphereDesc)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Mirror"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMirror::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

void CMirror::Use_Mirror(_float fTimeDelta)
{

	if (true == CUI_Manager::GetInstance()->Get_Player()->Get_MirrorUsePlayer())
	{
		_int Rand = rand() % 30;

		Rand /= 5;

		switch (Rand)
		{
		case 0:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(270);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(-23.6f, -6.75f, -184.135f, 1.f));
		}
		break;

		case 1:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(404);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(149.565f, -6.75f, -147.398f, 1.f));
		}
		break;

		case 2:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(584);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(-93.794f, -6.75f, 165.240f, 1.f));
		}
		break;

		case 3:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(765);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(113.631f, -6.75f, 38.308f, 1.f));
		}
		break;

		case 4:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(824);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(63.694f, -6.750f, -4.776f, 1.f));
		}
		break;

		case 5:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(879);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(-23.837f, -6.750f, -40.081f, 1.f));
		}
		break;
		}

		CUI_Manager::GetInstance()->Set_MirrorUse(false);
		CUI_Manager::GetInstance()->Get_Player()->Set_MirrorUsePlayer(false);
	}

	if (true == CUI_Manager::GetInstance()->Get_Player()->Get_Unhitable())
	{
		_float fUnhitableTime = CUI_Manager::GetInstance()->Get_Player()->Get_UnhitableTime();

		fUnhitableTime += fTimeDelta;

		CUI_Manager::GetInstance()->Get_Player()->Set_UnhitableTime(fUnhitableTime);

		if (fUnhitableTime > 7.f)
		{
			CUI_Manager::GetInstance()->Get_Player()->Set_Unhitable(false);
		}
	}
}

void CMirror::Player_Revive()
{
	if (CUI_Manager::GetInstance()->Get_PlayerRevive() == true)
	{
		return;
	}


	if (CUI_Manager::GetInstance()->Get_PlayerRevive() == false && CUI_Manager::GetInstance()->Get_Player()->Get_PlayerDeath() == true)
	{
		_int Rand = rand() % 30;

		Rand /= 5;

		switch (Rand)
		{
		case 0:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(270);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(-23.6f, -6.75f, -184.135f, 1.f));
		}
		break;

		case 1:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(404);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(149.565f, -6.75f, -147.398f, 1.f));
		}
		break;

		case 2:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(584);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(-93.794f, -6.75f, 165.240f, 1.f));
		}
		break;

		case 3:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(765);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(113.631f, -6.75f, 38.308f, 1.f));
		}
		break;

		case 4:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(824);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(63.694f, -6.750f, -4.776f, 1.f));
		}
		break;

		case 5:
		{
			CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Set_Index(879);
			CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(-23.837f, -6.750f, -40.081f, 1.f));
		}
		break;
		}





		CUI_Manager::GetInstance()->Set_PlayerRevive(true);


	}

	
}

CMirror* CMirror::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMirror* pInstance = new CMirror(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CMirror Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMirror::Clone(void* pArg)
{
	CMirror* pInstance = new CMirror(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CMirror Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMirror::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
}

