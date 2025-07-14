#include "stdafx.h"
#include "ChooseBox.h"
#include "GameInstance.h"
#include "InnerInventory.h"


CChooseBox::CChooseBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI {pDevice, pContext}
{
}

CChooseBox::CChooseBox(const CChooseBox& Prototype)
	: CUI { Prototype }
{
}

HRESULT CChooseBox::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CChooseBox::Initialize(void* pArg)
{
	UIOBJDESC* pDesc = static_cast<UIOBJDESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CChooseBox::Priority_Update(_float fTimeDelta)
{
}

_int CChooseBox::Update(_float fTimeDelta)
{
	if (KEY_DOWN(KEY::Q))
	{
		_int CurrentIndex = CUI_Manager::GetInstance()->Get_ChooseIndex();

		if (CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
		{
			return OBJ_NOEVENT;
		}

		for (_int i = CurrentIndex; i > 0; --i)
		{
			if (i < 0)
			{
				return OBJ_NOEVENT;
			}

			if (CUI_Manager::GetInstance()->Get_Inventory()[i - 1]->Get_ItemType() != CInventory::ITEM_EMPTY)
			{
				CUI_Manager::GetInstance()->Set_ChooseIndex(i - 1);
				isMove();

				if (CUI_Manager::GetInstance()->Get_Inventory()[CurrentIndex]->Get_ItemType() == CInventory::ITEM::ITEM_HANDLIGHTER && CUI_Manager::GetInstance()->Get_Player()->Get_HandLighterUse() == true)
				{
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
				}

				return OBJ_NOEVENT;
			}
		}
		if (CUI_Manager::GetInstance()->Get_ChooseIndex() != -1)
		{
			CUI_Manager::GetInstance()->All_DontUse(CUI_Manager::GetInstance()->Get_Inventory()[CUI_Manager::GetInstance()->Get_ChooseIndex()]->Get_ItemType());


			
		}
	}

	if (KEY_DOWN(KEY::E))
	{
		_int CurrentIndex = CUI_Manager::GetInstance()->Get_ChooseIndex();

		if (CUI_Manager::GetInstance()->Get_ChooseIndex() == 9 || CUI_Manager::GetInstance()->Get_ChooseIndex() == -1)
		{
			return OBJ_NOEVENT;
		}

		for (_int i = CurrentIndex; i < 9; ++i)
		{
			if (i >= 8)
			{
				return OBJ_NOEVENT;
			}

			if (CUI_Manager::GetInstance()->Get_Inventory()[i + 1]->Get_ItemType() != CInventory::ITEM_EMPTY)
			{
				CUI_Manager::GetInstance()->Set_ChooseIndex(i + 1);
				isMove();

				if (CUI_Manager::GetInstance()->Get_Inventory()[CurrentIndex]->Get_ItemType() == CInventory::ITEM::ITEM_HANDLIGHTER && CUI_Manager::GetInstance()->Get_Player()->Get_HandLighterUse() == true)
				{
					CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
				}

				return OBJ_NOEVENT;
			}
		}
	}
	if (CUI_Manager::GetInstance()->Get_ChooseIndex() != -1)
	{
		CUI_Manager::GetInstance()->All_DontUse(CUI_Manager::GetInstance()->Get_Inventory()[CUI_Manager::GetInstance()->Get_ChooseIndex()]->Get_ItemType());
	}

	return OBJ_NOEVENT;
}

void CChooseBox::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CChooseBox::Render()
{
	if (CUI_Manager::GetInstance()->Get_ChooseIndex() != -1)
	{
		__super::Render();
	}

	return S_OK;
}

void CChooseBox::isMove()
{
	switch (CUI_Manager::GetInstance()->Get_ChooseIndex())
	{
	case 0:
		m_fX = 30.f;
		break;

	case 1:
		m_fX = 80.f;
		break;

	case 2:
		m_fX = 130.f;
		break;

	case 3:
		m_fX = 180.f;
		break;

	case 4:
		m_fX = 230.f;
		break;
	
	case 5:
		m_fX = 280.f;
		break;
	
	case 6:
		m_fX = 330.f;
		break;
		
	case 7:
		m_fX = 380.f;
		break;

	case 8:
		m_fX = 430.f;
		break;

	}

	For_Render_ChooseBox();
}

HRESULT CChooseBox::Ready_Components()
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
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Texture_UI_ChooseBox"),
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
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ChooseBox"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}


	return S_OK;
}

void CChooseBox::For_Render_ChooseBox()
{
	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - ViewportDesc.Width * 0.5f, -m_fY + ViewportDesc.Height * 0.5f, 0.f, 1.f));

}

CChooseBox* CChooseBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CChooseBox* pInstance = new CChooseBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CChooseBox Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CChooseBox::Clone(void* pArg)
{
	CChooseBox* pInstance = new CChooseBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CChooseBox Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CChooseBox::Free()
{
	
	__super::Free();
	
}
