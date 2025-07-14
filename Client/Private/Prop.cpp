#include "stdafx.h"
#include "Prop.h"
#include "GameInstance.h"
#include "LightStone.h"
#include "Dynamite_Use.h"
#include "MagatamaProp.h"

static vector<CInnerInventory*>* m_pInventory = { nullptr };


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
		m_Desc.fRotationPerSec = pDesc->fRotationPerSec;
	}

	if (FAILED(__super::Initialize(&m_Desc)))
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
	CCollider* pTargetCollider = { nullptr };

	if (m_pGameInstance->Get_Level() == 2)
	{
		pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_PROLOGUE, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));
	}
	else if (m_pGameInstance->Get_Level() == 3)
	{
		pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));
	}

	if (m_bChooseObj == false)
	{
		_bool bInterSect = m_pColliderCom->Intersect(pTargetCollider);
		
	
		if (bInterSect == true)
		{
			CUI_Manager::GetInstance()->Set_BumpType(CUI_Manager::BUMP_PROP);
			_float4 Pos = { m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 0.f };

			if (KEY_DOWN(KEY::F))
			{
				m_bChooseObj = true;

				InterAction(fTimeDelta);

			}
		}
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}
	else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CProp::Render()
{
	return S_OK;
}

void CProp::InterAction(_float fTimedelta)
{
	CInnerInventory* pInnerInventory = { nullptr };

	/* m_vecInventory 만큼 돌린다.*/
	for (_int i = 0; i < CUI_Manager::GetInstance()->Get_Inventory().size(); ++i)
	{
 		if (CUI_Manager::GetInstance()->Get_Inventory()[i]->Get_ItemType() == CInventory::ITEM_HANDLIGHTER || CInventory::ITEM_GLOWSTONE || CInventory::ITEM_SPHER || CInventory::ITEM_DYNAMITE || CInventory::ITEM_LIGHTSTONE)
		{
			switch (m_ePropType)
			{
			case CInventory::ITEM_HANDLIGHTER:
			{
				for (size_t i = 0; i < CUI_Manager::GetInstance()->Get_Inventory().size(); ++i)
				{
					if (CUI_Manager::GetInstance()->Get_Inventory()[i]->Get_ItemType() == CInventory::ITEM_HANDLIGHTER)
					{
						if (CUI_Manager::GetInstance()->Get_HandLightTime() < 300)
						{
							CUI_Manager::GetInstance()->Set_HandLightTime(300);
							m_bLife = true;
							CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP1, false);
							return;
						}
						else if (CUI_Manager::GetInstance()->Get_HandLightTime() >= 300)
						{
							CUI_Manager::GetInstance()->Set_HandLightTime(300);
							CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP1, false);
							m_bLife = true;
							return;
						}
					}
				}
			}
			break;

			case CInventory::ITEM_GLOWSTONE:
			{
				_int PlusCount;
				PlusCount = rand() % 3;
				if (PlusCount == 0)
					PlusCount = 1;

				for (size_t i = 0; i < CUI_Manager::GetInstance()->Get_Inventory().size(); ++i)
				{
					if (CUI_Manager::GetInstance()->Get_Inventory()[i]->Get_ItemType() == CInventory::ITEM_GLOWSTONE)
					{
						pInnerInventory = CUI_Manager::GetInstance()->Get_Inventory()[i];
						pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + PlusCount);
						CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP2, false);
						m_bLife = true;
						return;
					}
				}
			}
			break;

			case CInventory::ITEM_LIGHTSTONE:
			{
				for (size_t i = 0; i < CUI_Manager::GetInstance()->Get_Inventory().size(); ++i)
				{
					if (CUI_Manager::GetInstance()->Get_Inventory()[i]->Get_ItemType() == CInventory::ITEM_GLOWSTONE)
					{
						pInnerInventory = CUI_Manager::GetInstance()->Get_Inventory()[i];
						pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
						CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP2, false);
						m_bLife = true;
						
						//CLightStone* LightStone = static_cast<CLightStone*>(this);
						//
						//LightStone->Delete_Light();
						//
						//Safe_Release(LightStone);

						return;
					}
				}
				
			}
			break;

			case CInventory::ITEM_SPHER:
			{
				for (size_t i = 0; i < CUI_Manager::GetInstance()->Get_Inventory().size(); ++i)
				{
					if (CUI_Manager::GetInstance()->Get_Inventory()[i]->Get_ItemType() == CInventory::ITEM_SPHER)
					{
						pInnerInventory = CUI_Manager::GetInstance()->Get_Inventory()[i];
						pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
						CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP1, false);
						m_bLife = true;
						return;
					}
				}
			}
			break;

			case CInventory::ITEM_DYNAMITE:
			{
				for (size_t i = 0; i < CUI_Manager::GetInstance()->Get_Inventory().size(); ++i)
				{
					if (CUI_Manager::GetInstance()->Get_Inventory()[i]->Get_ItemType() == CInventory::ITEM_DYNAMITE)
					{
						_int PlusCount = rand() % 3;
						if (PlusCount == 0)
						{
							PlusCount = 1;
						}
						pInnerInventory = CUI_Manager::GetInstance()->Get_Inventory()[i];
						pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + PlusCount);
						CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP1, false);
						m_bLife = true;
						return;
						
					}
				}
			}
			break;

			}

			if (CUI_Manager::GetInstance()->Get_Inventory()[i]->Get_ItemType() == CInventory::ITEM_EMPTY)
			{
				pInnerInventory = CUI_Manager::GetInstance()->Get_Inventory()[i];

				switch (m_ePropType)
				{
				case TYPE_LIGHTER:
				{
					pInnerInventory->Set_ItemType(CInventory::ITEM_LIGHTER);
					pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP1, false);

					if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
					{
						CUI_Manager::GetInstance()->Set_ChooseIndex(0);
						m_bLife = true;
						return;
					}

					m_bLife = true;
					return;
				}
				break;

				case TYPE_MIRROR:
				{
					pInnerInventory->Set_ItemType(CInventory::ITEM_MIRROR);
					pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP1, false);

					if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
					{
						CUI_Manager::GetInstance()->Set_ChooseIndex(0);
						m_bLife = true;
						return;
					}

					m_bLife = true;
					return;
				}
				break;

				case TYPE_HANDLIGHTER:
				{
					pInnerInventory->Set_ItemType(CInventory::ITEM_HANDLIGHTER);
					pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP1, false);
					if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
					{

						CUI_Manager::GetInstance()->Set_ChooseIndex(0);
						m_bLife = true;
						return;
					}

					m_bLife = true;
					return;
				}
				break;

				case TYPE_COMPASS:
				{
					pInnerInventory->Set_ItemType(CInventory::ITEM_COMPASS);
					pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP1, false);

					if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
					{
						CUI_Manager::GetInstance()->Set_ChooseIndex(0);
					}

					m_bLife = true;
					return;
				}
				break;

				case TYPE_KAGURA:
				{
					pInnerInventory->Set_ItemType(CInventory::ITEM_KAGURA);
					pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);

					if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
					{
						CUI_Manager::GetInstance()->Set_ChooseIndex(0);
					}

					m_bLife = true;
					return;
				}
				break;

				case TYPE_GLOWSTONE:
				{
					pInnerInventory->Set_ItemType(CInventory::ITEM_GLOWSTONE);
					pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP2, false);

					if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
					{
						CUI_Manager::GetInstance()->Set_ChooseIndex(0);
					}

					m_bLife = true;
					return;
				}
				break;

				case TYPE_LIGHTSTONE:
				{
					pInnerInventory->Set_ItemType(CInventory::ITEM_GLOWSTONE);
					pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP2, false);

					if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
					{
						CUI_Manager::GetInstance()->Set_ChooseIndex(0);

					}

					m_bLife = true;
					return;
				}
				break;

				case TYPE_KEY:
				{
					pInnerInventory->Set_ItemType(CInventory::ITEM_KEY);
					pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);

					if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
					{
						CUI_Manager::GetInstance()->Set_ChooseIndex(0);
					}


					m_bLife = true;
					return;
				}
				break;

				case TYPE_SPHER:
				{
					pInnerInventory->Set_ItemType(CInventory::ITEM_SPHER);
					pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + 1);
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP1, false);

					if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
					{
						CUI_Manager::GetInstance()->Set_ChooseIndex(0);
					}


					m_bLife = true;
					return;
				}
				break;

				case TYPE_DYNAMITE:
				{
					pInnerInventory->Set_ItemType(CInventory::ITEM_DYNAMITE);
					_uint Count = rand() % 3;

					if (Count == 0)
					{
						Count = 1;
					}

					pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() + Count);
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROP1, false);

					if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
					{
						CUI_Manager::GetInstance()->Set_ChooseIndex(0);

					}

					m_bLife = true;
					return;
				}
				break;

				case TYPE_CANDLE:
				{
					m_bCandleOn = true;
					return;
				}
				break;

				case TYPE_CABINET:
				{
					m_bCabinetIn = true;
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_CABBINET, false);
					return;
				}
				break;

				case TYPE_BIGBOX:
				{
					//m_bCabinetIn = true;
				}
				break;

				case TYPE_DOOR:
				{
					m_bDoorOpenClose = true;
					return;
				}
				break;

				case TYPE_MAGATAMA:
				{
					CUI_Manager::GetInstance()->Set_MagatamaCount(CUI_Manager::GetInstance()->Get_MagatamaCount() + 1);
					dynamic_cast<CMagatamaProp*>(this)->Set_MagatamaPut(true);
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PROPMAGATAMA, false);
					return;
				}
				break;

				case TYPE_LEVEL:
				{
					CUI_Manager::GetInstance()->Set_NextLevelGo(true);

					return;
				}
				break;

				case TYPE_ENDPROP:
				{
					CUI_Manager::GetInstance()->Set_FinalEnd(true);
				}
				break;
				}

			}

		}
	}
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

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
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

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

}


