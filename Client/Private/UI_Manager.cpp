#pragma once
#include "stdafx.h"
#include "UI_Manager.h"
#include "InnerInventory.h"
#include "GameInstance.h"
#include "Inventory.h"
#include "HandLighter_Battery_Count.h"
#include "ChooseBox.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
}

void CUI_Manager::Update(_float fTimeDelta)
{
	if (m_bEnd == true && m_bEEnd == false && m_bEEEnd == false)
	{
		_float BGMSound = CSoundMgr::GetInstance()->GetBGMVolume();

		CSoundMgr::GetInstance()->SetBGMVolume(BGMSound - 0.01f);

		if (CSoundMgr::GetInstance()->GetBGMVolume() <= 1.f)
		{
			m_bEEnd = true;
		}
	}
	else if (m_bEEnd == true)
	{
		CSoundMgr::GetInstance()->EndBGM();
		CSoundMgr::GetInstance()->SetBGMVolume(100.f);
		CSoundMgr::GetInstance()->StartBGM(BGM_TYPE::BGM_ENDING);
		m_bEEnd = false;
		m_bEEEnd = true;
	}

	
	if (m_bEEEnd == true)
	{
		CGameInstance::GetInstance()->Start_EndFade(true);
	}


	if (MOUSE_DOWN(MOUSE::DIM_RB))
	{
		Use_Item();
	}
}

void CUI_Manager::Delete_Player()
{
	Safe_Release(m_pPlayer);
	m_pPlayer = nullptr;
}

void CUI_Manager::Set_InventoryObj(CInnerInventory* InventoryObj)
{
	m_vecInventory.push_back(InventoryObj);
	//Safe_AddRef(InventoryObj);
}

void CUI_Manager::Delete_Inventory()
{
	for (auto iter = m_vecInventory.begin(); iter != m_vecInventory.end(); ++iter)
	{
		Safe_Release(*iter);
	}

	m_vecInventory.clear();
}

void CUI_Manager::Cal_DitanceSoundSize(SFX_TYPE _Type, CTransform* _transForm)
{
	_vector PlayerPos = CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	_float fDistance = (XMVectorGetX(XMVector3Length(PlayerPos - _transForm->Get_State(CTransform::STATE_POSITION)))) * 2;

	if (fDistance > 80.f)
	{
		fDistance = 80.f;
	}
	else if (fDistance < 0.f)
	{
		fDistance = 0.f;
	}

	_float SoundSize = 80 - fDistance;

	CSoundMgr::GetInstance()->Set_EffectVolume(SoundSize, _Type);

}

void CUI_Manager::Update_ChooseIndex()
{
	if (m_vecInventory[m_iChooseIndex]->Get_iTemCount() <= 0)
	{
		for (int i = m_iChooseIndex; i < m_vecInventory.size() - 1; ++i)
		{
			m_vecInventory[i]->Set_ItemType(m_vecInventory[i + 1]->Get_ItemType());
			m_vecInventory[i]->Set_iTemCount(m_vecInventory[i + 1]->Get_iTemCount());
			m_vecInventory[i]->Set_HaveItem(true);

			if (m_vecInventory[i + 1]->Get_ItemType() == CInventory::ITEM_EMPTY)
				break;
		}

		if (m_vecInventory[0]->Get_ItemType() == CInventory::ITEM_EMPTY)
		{
			CUI_Manager::GetInstance()->Set_ChooseIndex(-1);
		}
		else if (m_vecInventory[m_iChooseIndex]->Get_ItemType() == CInventory::ITEM_EMPTY)
		{
			CUI_Manager::GetInstance()->Set_ChooseIndex(m_iChooseIndex - 1);
			m_pChooseBox->isMove();
		}
	}
}

void CUI_Manager::Use_Item()
{

	if (m_iChooseIndex == -1)
		return;

	CInnerInventory* pInnerInventory = m_vecInventory[m_iChooseIndex];
	
	_uint CItemType = { 0 };

	CItemType = pInnerInventory->Get_ItemType();

	//if (MOUSE_DOWN(MOUSE::DIM_RB))
	//{
		switch ((CInventory::ITEM)CItemType)
		{
		case CInventory::ITEM_LIGHTER:
		{
			if (false == m_pPlayer->Get_LighterUse())
			{
				m_pPlayer->Set_LighterUse(true);
				All_DontUse(CItemType);
				CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LIGHTERON, false);

			}
			else if (true == m_pPlayer->Get_LighterUse())
			{
				m_pPlayer->Set_LighterUse(false);
				CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LIGHTEROFF, false);
			}
		}
		break;

		case CInventory::ITEM_MIRROR:
		{
			if (m_pPlayer->Get_MirrorUsePlayer() == false)
			{
				m_pPlayer->Set_MirrorUsePlayer(true);
			}
			//	if (0.f != m_pPlayer->Get_UnhitableTime())
			//	{
			//		m_pPlayer->Set_UnhitableTime(0.f);
			//		m_pPlayer->Set_Unhitable(true);
			//
			//	}
			//	else
			//	{
			//		m_pPlayer->Set_Unhitable(true);
			//		All_DontUse(CItemType);
			//	}
			//
			//}	
			//else if (m_MirrorUse == true)
			//{
			//	m_MirrorUse = false;
			//}
		}
		break;

		case CInventory::ITEM_HANDLIGHTER:
		{
			if (false == m_pPlayer->Get_HandLighterUse())
			{
				m_pPlayer->Set_HandLighterUse(true);
				All_DontUse(CItemType);
				CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
			}
			else if (true == m_pPlayer->Get_HandLighterUse())
			{
				m_pPlayer->Set_HandLighterUse(false);
				CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
			}
		}
		break;

		case CInventory::ITEM_COMPASS:
		{
			All_DontUse(CItemType);
		}
		break;

		case CInventory::ITEM_KAGURA:
		{
			All_DontUse(CItemType);
		}
		break;

		case CInventory::ITEM_GLOWSTONE:
		{
			m_GlowStoneUse = true;
			All_DontUse(CItemType);
		}
		break;

		case CInventory::ITEM_KEY:
		{
			All_DontUse(CItemType);
		}
		break;

		case CInventory::ITEM_SPHER:
		{
			if (m_bSphereUse == true)
			{
				break;
			}

			pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() - 1);

			m_bSphereUse = true;
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_SPHERE, false);
			All_DontUse(CItemType);
		}
		break;

		case CInventory::ITEM_DYNAMITE:
		{
			//pInnerInventory->Set_iTemCount(pInnerInventory->Get_iTemCount() - 1);

			m_bDynamiteUse = true;
			All_DontUse(CItemType);
		}
		break;
		}

		if (pInnerInventory->Get_iTemCount() <= 0)
		{
			for (int i = m_iChooseIndex; i < m_vecInventory.size() - 1; ++i)
			{
				m_vecInventory[i]->Set_ItemType(m_vecInventory[i + 1]->Get_ItemType());
				m_vecInventory[i]->Set_iTemCount(m_vecInventory[i + 1]->Get_iTemCount());
				m_vecInventory[i]->Set_HaveItem(true);

				if (m_vecInventory[i + 1]->Get_ItemType() == CInventory::ITEM_EMPTY)
					break;
			}

			if (m_vecInventory[0]->Get_ItemType() == CInventory::ITEM_EMPTY)
			{
				CUI_Manager::GetInstance()->Set_ChooseIndex(-1);
			}
			else if (m_vecInventory[m_iChooseIndex]->Get_ItemType() == CInventory::ITEM_EMPTY)
			{
				CUI_Manager::GetInstance()->Set_ChooseIndex(m_iChooseIndex - 1);
				m_pChooseBox->isMove();
			}
		}
}

void CUI_Manager::All_DontUse(_uint UseItemType)
{
	switch ((CInventory::ITEM)UseItemType)
	{
	case CInventory::ITEM_LIGHTER:
	{
		if (true == m_pPlayer->Get_HandLighterUse())
		{
			m_pPlayer->Set_HandLighterUse(false);
		}

		if (true == m_pPlayer->Get_HandLighterUse())
		{
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
			m_pPlayer->Set_HandLighterUse(false);
			
		}

		if (true == Get_MirrorUse())
		{
			Set_MirrorUse(false);
		}

		if (true == Get_CompassUse())
		{
			Set_CompassUse(false);
		}

		if (true == Get_KaguraUse())
		{
			Set_KaguraUse(false);
		}
	}
	break;

	case CInventory::ITEM_MIRROR:
	{
		if (true == m_pPlayer->Get_LighterUse())
		{
			m_pPlayer->Set_LighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LIGHTEROFF, false);
		}

		if (true == m_pPlayer->Get_HandLighterUse())
		{
			m_pPlayer->Set_HandLighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
		}

		if (true == Get_CompassUse())
		{
			Set_CompassUse(false);
		}

		if (true == Get_KaguraUse())
		{
			Set_KaguraUse(false);
		}
	}
	break;

	case CInventory::ITEM_HANDLIGHTER:
	{
		if (true == m_pPlayer->Get_LighterUse())
		{
			m_pPlayer->Set_LighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LIGHTEROFF, false);
		}



		if (true == Get_MirrorUse())
		{
			Set_MirrorUse(false);
		}

		if (true == Get_CompassUse())
		{
			Set_CompassUse(false);
		}

		if (true == Get_KaguraUse())
		{
			Set_KaguraUse(false);
		}
	}
	break;

	case CInventory::ITEM_COMPASS:
	{
		if (true == m_pPlayer->Get_LighterUse())
		{
			m_pPlayer->Set_LighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LIGHTEROFF, false);
		}

		if (true == m_pPlayer->Get_HandLighterUse())
		{
			m_pPlayer->Set_HandLighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
		}

		if (true == Get_MirrorUse())
		{
			Set_MirrorUse(false);
		}

		if (true == Get_CompassUse())
		{
			Set_CompassUse(false);
		}

		if (true == Get_KaguraUse())
		{
			Set_KaguraUse(false);
		}
	}
	break;

	case CInventory::ITEM_KAGURA:
	{
		if (true == m_pPlayer->Get_LighterUse())
		{
			m_pPlayer->Set_LighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LIGHTEROFF, false);
		}

		if (true == m_pPlayer->Get_HandLighterUse())
		{
			m_pPlayer->Set_HandLighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
		}

		if (true == Get_MirrorUse())
		{
			Set_MirrorUse(false);
		}

		if (true == Get_CompassUse())
		{
			Set_CompassUse(false);
		}
	}
	break;

	case CInventory::ITEM_GLOWSTONE:
	{
		if (true == m_pPlayer->Get_LighterUse())
		{
			m_pPlayer->Set_LighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LIGHTEROFF, false);
		}

		if (true == m_pPlayer->Get_HandLighterUse())
		{
			m_pPlayer->Set_HandLighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
		}

		if (true == Get_MirrorUse())
		{
			Set_MirrorUse(false);
		}

		if (true == Get_CompassUse())
		{
			Set_CompassUse(false);
		}

		if (true == Get_KaguraUse())
		{
			Set_KaguraUse(false);
		}
	}
	break;

	case CInventory::ITEM_KEY:
	{
		if (true == m_pPlayer->Get_LighterUse())
		{
			m_pPlayer->Set_LighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LIGHTEROFF, false);
		}

		if (true == m_pPlayer->Get_HandLighterUse())
		{
			m_pPlayer->Set_HandLighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
		}

		if (true == Get_MirrorUse())
		{
			Set_MirrorUse(false);
		}

		if (true == Get_CompassUse())
		{
			Set_CompassUse(false);
		}

		if (true == Get_KaguraUse())
		{
			Set_KaguraUse(false);
		}
	}
	break;

	case CInventory::ITEM_SPHER:
	{
		if (true == m_pPlayer->Get_LighterUse())
		{
			m_pPlayer->Set_LighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LIGHTEROFF, false);
		}

		if (true == m_pPlayer->Get_HandLighterUse())
		{
			m_pPlayer->Set_HandLighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
		}

		if (true == Get_MirrorUse())
		{
			Set_MirrorUse(false);
		}

		if (true == Get_CompassUse())
		{
			Set_CompassUse(false);
		}

		if (true == Get_KaguraUse())
		{
			Set_KaguraUse(false);
		}
	}
	break;

	case CInventory::ITEM_DYNAMITE:
	{
		if (true == m_pPlayer->Get_LighterUse())
		{
			m_pPlayer->Set_LighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LIGHTEROFF, false);
		}

		if (true == m_pPlayer->Get_HandLighterUse())
		{
			m_pPlayer->Set_HandLighterUse(false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HANDLIGHT, false);
		}

		if (true == Get_MirrorUse())
		{
			Set_MirrorUse(false);
		}

		if (true == Get_CompassUse())
		{
			Set_CompassUse(false);
		}

		if (true == Get_KaguraUse())
		{
			Set_KaguraUse(false);
		}
	}
	break;
	}

}

void CUI_Manager::Delete_Navigation()
{
	m_pNavigation->Free();
}

void CUI_Manager::Free()
{
	//Safe_Release(m_pChooseBox);
	Safe_Release(m_pPlayer);
	Safe_Release(m_pCamera);

	for (auto iter = m_vecBatteryCount.begin(); iter != m_vecBatteryCount.end(); ++iter)
	{
		Safe_Release(*iter);
		
	}



	//for (auto iter = m_vecBatteryCount.begin(); iter != m_vecBatteryCount.end(); ++iter)
	//{
	//	Safe_Release(*iter);				  %
	//}

	for (auto iter = m_vecInventory.begin(); iter != m_vecInventory.end(); ++iter)
	{
		Safe_Release(*iter);
	}

	m_vecInventory.clear();

	m_pNavigation->Delete_Cell();
	Safe_Release(m_pNavigation);
}
