#include "stdafx.h"
#include "Spider.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Sound.h"

CSpider::CSpider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{pDevice, pContext}
{
}

CSpider::CSpider(const CSpider& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CSpider::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CSpider::Initialize(void* pArg)
{
	MONSTER_DESC* Desc = static_cast<MONSTER_DESC*>(pArg);

	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	COLLIDERDESC colDesc{};
	colDesc.vCenter = { 0.f, 0.f, 0.f };
	colDesc.fRadius = 310.f;
	colDesc.vExtents = { 10.f , 50.f, 25.f };
	colDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

	if (FAILED(Ready_Components(&colDesc)))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(1, true);
	m_iState = SPIDER_STATE::SPIDER_STATE_WALK;

	Set_AnimationStates();

	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));

	

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-283.f, -6.75f, 63.465f, 1.f));

	m_matSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	return S_OK;
}

void CSpider::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CSpider::Update(_float fTimeDelta)
{
	for (auto& pColliderCom : m_pColliderSpider)
		pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	LightPosition(1, 2.5f, 0.f, m_pTransformCom);

	return OBJ_NOEVENT;
	//m_pTransformCom->Get_WorldMatrix();
}

void CSpider::Late_Update(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->Cal_DitanceSoundSize(SFX_TYPE::SFX_SPIDERWAIT, m_pTransformCom);
	
	_vector PlayerPos = CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	_float fDistance = (XMVectorGetX(XMVector3Length(PlayerPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) * 0.5;

	if (fDistance > 100.f)
	{
		fDistance = 100.f;
	}
	else if (fDistance < 0.f)
	{
		fDistance = 0.f;
	}

	_float SoundSize = 80 - fDistance;

	CSoundMgr::GetInstance()->Set_EffectVolume(SoundSize, SFX_TYPE::SFX_SPIDERADDLOOKAT);


	if (fDistance <= 80 && m_fPlaySound <= 0.0f)
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_SPIDERWAIT, false);
		
		m_fPlaySound = m_confIntervalSound;
	}
	
	if (fDistance >= 100.f)
	{
		CSound* pRunSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_SPIDERADDLOOKAT);

		if (true == pRunSound->IsPlaying())
		{
			CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_SPIDERADDLOOKAT);

		}
	}


	_vector CurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_bool bCollision = { false };
	CCollider* pTargetCollider = nullptr;

	if (true == CUI_Manager::GetInstance()->Get_SpherUse() || true == CUI_Manager::GetInstance()->Get_FinalEnd())
	{
		m_pModelCom->Play_Animation(0, m_iChanging);
	}
	else
	{
		m_isDynamiteChase =			TraceDynamite(fTimeDelta);

		if (m_isDynamiteChase == true)
		{
			_vector		vWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			if (true == m_pGameInstance->isIn_Frustum_InWorldSpace(vWorldPos, 2.f))
			{
				m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
			}

			m_pModelCom->Play_Animation(fTimeDelta, m_iChanging);

			if (m_fPlaySound > 0.0f)
			{
				m_fPlaySound -= fTimeDelta;
			}

			return;
		}

		Door_Bump(fTimeDelta);

		if (m_bDoorBump == false)
			Action_Change(fTimeDelta);

		for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider")); ++i)
		{
			pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider"), i));

			bCollision = m_pColliderSpider[SPIDER_OBB]->Intersect(pTargetCollider);

			if (bCollision == true)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);

				for (auto& pCollider : m_pColliderSpider)
					pCollider->Update(m_pTransformCom->Get_WorldMatrix());

				m_bDoorBump = true;
			}
		}
		m_pModelCom->Play_Animation(fTimeDelta, m_iChanging);
	}
	_vector		vWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (true == m_pGameInstance->isIn_Frustum_InWorldSpace(vWorldPos, 2.f))
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}

	if (m_fPlaySound > 0.0f)
	{
		m_fPlaySound -= fTimeDelta;
	}


	CCollider* pPlayerTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player1")));
	_bool test3 = m_pColliderSpider[SPIDER_SPHERE0]->Intersect(pPlayerTargetCollider);

	if (m_fPlayLookAtSoundTime > 0.0f && false == test3)
	{
		m_fPlayLookAtSoundTime -= fTimeDelta;
	}
	else if (m_fPlayLookAtSoundTime > 0.0f && true == test3)
	{
		m_fPlayLookAtSoundTime = m_confPlayLookAtSoundTime;
	}



}

HRESULT CSpider::Render()
{
	__super::Render();

	//for (auto& pColliderCom : m_pColliderSpider)
	//	pColliderCom->Render();

	return S_OK;
}

HRESULT CSpider::Ready_Components(COLLIDERDESC* pDesc)
{
	
	CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};

	ColliderSphereDesc.fRadius = pDesc->fRadius;
	ColliderSphereDesc.vCenter = pDesc->vCenter;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere0"), reinterpret_cast<CComponent**>(&m_pColliderSpider[SPIDER_SPHERE0]), &ColliderSphereDesc)))
		return E_FAIL;



	/* Com_Collider_OBB */
	CBounding_OBB::BOUND_OBB_DESC		ColliderOBBDesc{};

	ColliderOBBDesc.vExtents = pDesc->vExtents;
	ColliderOBBDesc.vCenter = pDesc->vCenter;
	ColliderOBBDesc.vRotation = pDesc->vRotation;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderSpider[SPIDER_OBB]), &ColliderOBBDesc)))
		return E_FAIL;




	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	 
	/* Com_Model for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Spider"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iStartIndex = m_iNaviMeshIndex;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.fRange = _float(12.f);
	LightDesc.vPosition = { pDesc->vCenter.x, pDesc->vCenter.y , pDesc->vCenter.z, 1.f };
	LightDesc.TurnOn_Off = false;

	m_MonsterForLight = 1;
	if (FAILED(m_pGameInstance->Add_MonsterLight(LightDesc, m_MonsterForLight)))
		return E_FAIL;

	return S_OK;


}

HRESULT CSpider::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

HRESULT CSpider::Set_AnimationStates()
{
	m_vecAnimStates.resize(SPIDER_ANIM::SOIDER_ANIM_END);
	m_vecAnimStates[SPIDER_ANIM::SPIDER_ANIM_SPLINT] = 0;
	m_vecAnimStates[SPIDER_ANIM::SPIDER_ANIM_WALK] = 1;
	

	return S_OK;
}

void CSpider::Pri_Update_Cancle_LoopState()
{
	if (CONDITION_WALK)
		m_iState ^= SPIDER_STATE_WALK;
	if (CONDITION_SPLINT)
		m_iState ^= SPIDER_STATE_SPLINT;
}

_bool CSpider::TraceDynamite(_float fTimeDelta)
{
	CCollider* pTargetCollider = nullptr;
	_bool ObbCollision = { false };
	_bool bCollision = { false };

	for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_DynamiteThrow"), TEXT("Com_Collider1")); ++i)
	{
		pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_DynamiteThrow"), TEXT("Com_Collider1"), i));
		{
			if (pTargetCollider == nullptr || pTargetCollider->Get_Bounding()->Get_Radius() <= 1.f)
			{
				continue;
			}

			bCollision = m_pColliderSpider[SPIDER_SPHERE0]->Intersect(pTargetCollider);
			ObbCollision = m_pColliderSpider[SPIDER_OBB]->Intersect(pTargetCollider);

			if (bCollision == true)
			{
				_float3 DynamitePos = pTargetCollider->Get_Bounding()->Get_Pos();
				_vector Dynamitevec = XMVectorSet(DynamitePos.x, m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1], DynamitePos.z, 1.f);
				m_pGameInstance->MonsterLight_TurnOnOff(1, true);

				if (m_pModelCom->Get_Current_AnimIndex() == 0)
				{
					//m_pModelCom->Move_NextAnim(m_vecAnimStates[SPIDER_ANIM_SPLINT], 0.f, true);
					Pri_Update_Cancle_LoopState();
					m_iState = SPIDER_STATE::SPIDER_STATE_SPLINT;
					m_pTransformCom->Set_Speed(10.f);

					m_pTransformCom->LookAt(Dynamitevec);

					if (ObbCollision == false)
						m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

					return true;
				}
				else
				{
					m_pModelCom->Move_NextAnim(m_vecAnimStates[SPIDER_ANIM_SPLINT], 0.f, true);
					Pri_Update_Cancle_LoopState();
					m_iState = SPIDER_STATE::SPIDER_STATE_SPLINT;

					m_pTransformCom->LookAt(Dynamitevec);

					if (ObbCollision == false)
						m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
				}
				return true;
			}
			else
				m_pGameInstance->MonsterLight_TurnOnOff(1, false);
		}
	}

	return false;
}

void CSpider::Action_Change(_float fTimeDelta)
{
	m_PlayerPosition = m_pPlayer->Get_Position();
	_bool PlayerHide = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerHide();

	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player1")));

	for (auto& pCollider : m_pColliderSpider)
	{
		if (true == m_pColliderSpider[SPIDER_SPHERE0]->Intersect(pTargetCollider) && PlayerHide == false)
		{
			Action_Splint(fTimeDelta);

			m_bHidePlayer = false;
			m_pGameInstance->MonsterLight_TurnOnOff(1, true);
			CUI_Manager::GetInstance()->Set_PlayerHeartBeat(true);
		}
		else if (true == m_pColliderSpider[SPIDER_SPHERE0]->Intersect(pTargetCollider) &&  PlayerHide == true)
		{
			
			Action_Walk_Back(fTimeDelta);
			
			
		}
		else
			m_pGameInstance->MonsterLight_TurnOnOff(1, false);
		//else if (false == m_pColliderSpider[SPIDER_SPHERE0]->Intersect(pTargetCollider) && PlayerHide == false)
		//{
		//	
		//	Action_Walk(fTimeDelta);
		//	
		//}
	}


	///////////////////////////////////////////////////////////////////////



	if (true == m_pColliderSpider[SPIDER_OBB]->Intersect(pTargetCollider))
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PLAYERDEAD, false);
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_REVIVE, false);

		CUI_Manager::GetInstance()->Get_Player()->Set_PlayerDeath(true);
		CUI_Manager::GetInstance()->Set_PlayerRevive(false);
	}

}

void CSpider::Action_Walk(_float fTimeDelta)
{
	
	if (m_pModelCom->Get_Current_AnimIndex() == 1)
	{
		m_iState |= SPIDER_ANIM::SPIDER_ANIM_WALK;
		__super::Monster_Turn(fTimeDelta);
		m_pTransformCom->Set_Speed(10.f);
		Pri_Update_Cancle_LoopState();

		if (m_fHideCurrentTime < 30.f)
		{
			m_fHideCurrentTime += fTimeDelta;
		}

		if (m_bReturing == true)
		{
			if (m_fHideCurrentTime > 20.f)
			{
				m_fHideCurrentTime = 0.0f;
			}
		}

		return;
	}
	//m_pModelCom->SetUp_Animation(1, true);

	m_pModelCom->Move_NextAnim(1/*m_vecAnimStates[SPIDER_ANIM_WALK]*/, 0.f, true);
	__super::Monster_Turn(fTimeDelta);
	m_pTransformCom->Set_Speed(12.f);

	if (m_fHideCurrentTime < 30.f)
	{
		m_fHideCurrentTime += fTimeDelta;
	}

	if (m_bReturing == true)
	{
		if (m_fHideCurrentTime > 20.f)
		{
			m_fHideCurrentTime = 0.0f;
		}
	}
}

void CSpider::Action_Walk_Back(_float fTimeDelta)
{
	if (m_pModelCom->Get_Current_AnimIndex() == 1)
	{
		m_iState |= SPIDER_ANIM::SPIDER_ANIM_WALK;
			__super::Monster_Turn_Back(fTimeDelta);
		m_pTransformCom->Set_Speed(10.f);

		if (m_fHideCurrentTime < 30.f)
		{
			m_fHideCurrentTime += fTimeDelta;
		}

		if (m_bReturing == true)
		{
			if (m_fHideCurrentTime > 20.f)
			{
				m_fHideCurrentTime = 0.0f;
			}
		}

		return;
	}

	m_pModelCom->Move_NextAnim(m_vecAnimStates[SPIDER_ANIM_WALK], 0.f, true);
	Pri_Update_Cancle_LoopState();
	m_iState = SPIDER_STATE::SPIDER_STATE_WALK;

	if (CONDITION_LOOP)
	{
		m_iState = SPIDER_STATE::SPIDER_STATE_WALK;
	}
	

	//1m_pModelCom->SetUp_Animation(1, true);

	__super::Monster_Turn_Back(fTimeDelta);
	m_pTransformCom->Set_Speed(10.f);
}

void CSpider::Action_Splint(_float fTimeDelta)
{
	if (m_fPlayLookAtSoundTime <= 0.0f)
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_SPIDERLOOKAT, false);
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_SPIDERADDLOOKAT, false);
		m_fPlayLookAtSoundTime = m_confPlayLookAtSoundTime;
	}


	if (m_pModelCom->Get_Current_AnimIndex() == 0)
	{
		Pri_Update_Cancle_LoopState();
		__super::Monster_Turn(fTimeDelta);
		m_pTransformCom->Set_Speed(12.f);
		CUI_Manager::GetInstance()->Set_PlayerHeartBeat(true);
		//Pri_Update_Cancle_LoopState();

		return;
	}

	m_pModelCom->Move_NextAnim(m_vecAnimStates[SPIDER_ANIM_SPLINT], 0.f, true);
	Pri_Update_Cancle_LoopState();
	m_iState = SPIDER_STATE::SPIDER_STATE_SPLINT;

	//m_pModelCom->Move_NextAnim(1, 0.2f, true);
	__super::Monster_Turn(fTimeDelta);
	CUI_Manager::GetInstance()->Set_PlayerHeartBeat(true);
	m_pTransformCom->Set_Speed(12.f);
}

void CSpider::Door_Bump(_float fTimeDelta)
{
	_bool bCollision = { false };
	_vector CurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player1")));
	_bool		bBumpPlayer = m_pColliderSpider[SPIDER_SPHERE0]->Intersect(pTargetCollider);

	if (m_bDoorBump == false)
	{
		if (m_pTransformCom->Get_Speed() >= 12.0f)
		{
			m_pTransformCom->Set_Speed(10.0f);
		}
		return;
	}

	if (m_bDoorBump == true && bBumpPlayer == true)
	{
		// 플레이어를 따라간다. 그런데 문과 충돌하면 그자리 유지한다.

		if (m_pTransformCom->Get_Speed() >= 12.0f)
		{
			m_pTransformCom->Set_Speed(10.0f);
		}

		m_pGameInstance->MonsterLight_TurnOnOff(1, true);

		__super::Monster_Turn(fTimeDelta);

		for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider")); ++i)
		{
			pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider"), i));

			bCollision = m_pColliderSpider[SPIDER_OBB]->Intersect(pTargetCollider);

			if (bCollision == true)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);

				for (auto& pCollider : m_pColliderSpider)
					pCollider->Update(m_pTransformCom->Get_WorldMatrix());

				return;
			}
		}
	}
	// 문과 충돌을 하고 플레이어와 충돌을 안했을 경우
	else if (m_bDoorBump == true && bBumpPlayer == false && m_fDoorBumpBackTime < 8.f)
	{
		// 바로 원래자리로 보내버린다.

		m_pGameInstance->MonsterLight_TurnOnOff(1, false);
		m_pTransformCom->LookAt(m_matSpawnPos);
		m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		m_pTransformCom->Set_Speed(10.0f);
		m_fDoorBumpBackTime += fTimeDelta;
	}
	else if (m_bDoorBump == true && m_fDoorBumpBackTime >= 8.f)
	{
		m_fDoorBumpBackTime = 0.f;
		m_bDoorBump = false;
		return;
	}

}

CSpider* CSpider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpider* pInstance = new CSpider(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CSpider Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CSpider::Clone(void* pArg)
{
	CSpider* pInstance = new CSpider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CSpider Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSpider::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	for (auto& pColliderCom : m_pColliderSpider)
		Safe_Release(pColliderCom);
}

