#include "stdafx.h"
#include "CenterWord.h"


CCenterWord::CCenterWord(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI{ pDevice, pContext }
{
}

CCenterWord::CCenterWord(const CCenterWord& Prototype)
	: CUI{ Prototype }
{
}

HRESULT CCenterWord::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCenterWord::Initialize(void* pArg)
{
	UIOBJDESC* pDesc = static_cast<UIOBJDESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	m_OriginFx = m_fX;
	m_OriginFy = m_fY;

	return S_OK;
}

void CCenterWord::Priority_Update(_float fTimeDelta)
{
}

_int CCenterWord::Update(_float fTimeDelta)
{
	Update_BumpForWord();
	return OBJ_NOEVENT;
}

void CCenterWord::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	switch (CUI_Manager::GetInstance()->Get_BumpType())
	{
	case CUI_Manager::BUMP_PROP:
		wsprintf(m_CenterWord, TEXT("[ F ] Pick it Up"));
		m_iFx = m_OriginFx - 130 ;
		m_iFy = m_OriginFy;
		break;

	case CUI_Manager::BUMP_CANDLE:
		wsprintf(m_CenterWord, TEXT("[ F ] Turn Light On or Off"));
		m_iFx = m_OriginFx - 200;
		m_iFy = m_OriginFy;
		break;

	case CUI_Manager::BUMP_CABINET:
		wsprintf(m_CenterWord, TEXT("[ F ] Go into or out Cabinet"));
		m_iFx = m_OriginFx - 210;
		m_iFy = m_OriginFy - 10;
		break;

	case CUI_Manager::BUMP_DOOR:
		wsprintf(m_CenterWord, TEXT("[ F ] Open or Close Door"));
		m_iFx = m_OriginFx - 190;
		m_iFy = m_OriginFy - 10;
		break;

	case CUI_Manager::BUMP_ENDPROP:
		if (CUI_Manager::GetInstance()->Get_MagatamaCount() != 3)
		{
			wsprintf(m_CenterWord, TEXT("Be Short of %d Magatamas"), 3 - CUI_Manager::GetInstance()->Get_MagatamaCount());
			m_iFx = m_OriginFx - 180;
			m_iFy = m_OriginFy;
		}
		else if (CUI_Manager::GetInstance()->Get_MagatamaCount() == 3)
		{
			wsprintf(m_CenterWord,	TEXT("[ F ] Run Away!"));
			m_iFx = m_OriginFx - 155;
			m_iFy = m_OriginFy - 10;
		}
		
		break;

	case CUI_Manager::BUMP_STOP:
		if (CUI_Manager::GetInstance()->Get_Player()->Get_LighterUse() == false)
		{
			wsprintf(m_CenterWord,	TEXT("too Dark to Pass by"));
			m_iFx = m_OriginFx - 110;
			m_iFy = m_OriginFy - 10;
		}
		else if (CUI_Manager::GetInstance()->Get_Player()->Get_LighterUse() == true)
		{
			wsprintf(m_CenterWord, TEXT(" "));
		}
		break;


	case CUI_Manager::BUMP_LEVEL:
		wsprintf(m_CenterWord,		TEXT("[ F ] E N T E R"));
		m_iFx = m_OriginFx - 110;
		m_iFy = m_OriginFy - 10;
		break;


	case CUI_Manager::BUMP_EMPTY:
		wsprintf(m_CenterWord, TEXT(" "));
		break;
	}
	
}

HRESULT CCenterWord::Render()
{
	__super::Render();
	m_pGameInstance->Render_Font(TEXT("Chiller_28"), m_CenterWord, _float2(m_iFx, m_iFy), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	return S_OK;
}

HRESULT CCenterWord::Ready_Components()
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
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Texture_UI_CenterWord"),
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
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_CenterWord"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	return S_OK;
}

_int CCenterWord::Update_BumpForWord()
{
	if (CUI_Manager::GetInstance()->Get_BumpType() != CUI_Manager::BUMP_EMPTY)
	{
		_bool bCollision = { false };
		CCollider* pTargetCollider = nullptr;

		_bool PropBump = { false };
		_bool CollisionProp = { false };
		_bool CabinetProp = { false };
		_bool CandleProp = { false };
		_bool EndProp = { false };

		if (m_pGameInstance->Get_Level() == 2)
		{
			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_PROLOGUE, TEXT("Layer_Props"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_PROLOGUE, TEXT("Layer_Props"), TEXT("Com_Collider"), i));

				bCollision = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerCollider()->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					PropBump = true;
					return OBJ_NOEVENT;
				}
			}

			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_PROLOGUE, TEXT("Layer_CollisionProps"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_PROLOGUE, TEXT("Layer_CollisionProps"), TEXT("Com_Collider"), i));

				bCollision = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerCollider()->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					CollisionProp = true;
					return OBJ_NOEVENT;
				}
			}

			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_PROLOGUE, TEXT("Layer_CabinetProps"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_PROLOGUE, TEXT("Layer_CabinetProps"), TEXT("Com_Collider"), i));

				bCollision = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerCollider()->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					CabinetProp = true;
					return OBJ_NOEVENT;
				}
			}


			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_PROLOGUE, TEXT("Layer_CandleProps"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_PROLOGUE, TEXT("Layer_CandleProps"), TEXT("Com_Collider"), i));

				bCollision = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerCollider()->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					CandleProp = true;
					return OBJ_NOEVENT;
				}
			}

			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_PROLOGUE, TEXT("Layer_End"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_PROLOGUE, TEXT("Layer_End"), TEXT("Com_Collider"), i));

				bCollision = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerCollider()->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					EndProp = true;
					return OBJ_NOEVENT;
				}
			}
		}
		else if (m_pGameInstance->Get_Level() == 3)
		{
			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_Props"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Props"), TEXT("Com_Collider"), i));

				bCollision = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerCollider()->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					PropBump = true;
					return OBJ_NOEVENT;
				}
			}

			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider"), i));

				bCollision = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerCollider()->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					CollisionProp = true;
					return OBJ_NOEVENT;
				}
			}

			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_CabinetProps"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CabinetProps"), TEXT("Com_Collider"), i));

				bCollision = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerCollider()->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					CabinetProp = true;
					return OBJ_NOEVENT;
				}
			}


			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_CandleProps"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CandleProps"), TEXT("Com_Collider"), i));

				bCollision = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerCollider()->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					CandleProp = true;
					return OBJ_NOEVENT;
				}
			}

			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_End"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_End"), TEXT("Com_Collider"), i));

				bCollision = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerCollider()->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					EndProp = true;
					return OBJ_NOEVENT;
				}
			}
		}

		


		if (PropBump == false && CollisionProp == false && CabinetProp == false && CandleProp == false && EndProp == false)
		{
			CUI_Manager::GetInstance()->Set_BumpType(CUI_Manager::BUMP_EMPTY);
			return OBJ_NOEVENT;
		}

	}
	else
	{
		CUI_Manager::GetInstance()->Set_BumpType(CUI_Manager::BUMP_EMPTY);
		return OBJ_NOEVENT;
	}
}

CCenterWord* CCenterWord::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCenterWord* pInstance = new CCenterWord(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CCenterWord Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCenterWord::Clone(void* pArg)
{
	CCenterWord* pInstance = new CCenterWord(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CCenterWord Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCenterWord::Free()
{

	__super::Free();

}
