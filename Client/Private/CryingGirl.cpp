#include "stdafx.h"
#include "CryingGirl.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Sound.h"

CryingGirl::CryingGirl(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{
}

CryingGirl::CryingGirl(const CryingGirl& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CryingGirl::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CryingGirl::Initialize(void* pArg)
{
	MONSTER_DESC* Desc = static_cast<MONSTER_DESC*>(pArg);

	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	COLLIDERDESC colDesc{};
	colDesc.vCenter = { 0.f, 0.f, 0.f };
	colDesc.fRadius = 280.f;
	colDesc.vExtents = { 10.f , 50.f, 10.f };
	colDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	
	if (FAILED(Ready_Components(&colDesc)))
		return E_FAIL;

	
	m_pModelCom->SetUp_Animation(0, true);
	m_iState = CRY_STATE::CRY_STATE_CRY;

	Set_AnimationStates();

	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	
	m_matSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_bChasePlayer = false;
	return S_OK;
}

void CryingGirl::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	//Action_Change(fTimeDelta);
}

_int CryingGirl::Update(_float fTimeDelta)
{
	for (auto& pColliderCom : m_pColliderCrying)
		pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	LightPosition(2, 1.1f, 3.8f, m_pTransformCom);




	return OBJ_NOEVENT;
	//m_pTransformCom->Get_WorldMatrix();
}

void CryingGirl::Late_Update(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->Cal_DitanceSoundSize(SFX_TYPE::SFX_CRYLOOKAT, m_pTransformCom);
	CUI_Manager::GetInstance()->Cal_DitanceSoundSize(SFX_TYPE::SFX_CRYWALK, m_pTransformCom);
	
	
	_vector PlayerPos = CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	_float fDistance = (XMVectorGetX(XMVector3Length(PlayerPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) * 0.5;

	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player1")));
	_bool test3 = m_pColliderCrying[CRY_SPHERE2]->Intersect(pTargetCollider);

	if (fDistance > 100.f)
	{
		fDistance = 100.f;
	}
	else if (fDistance < 0.f)
	{
		fDistance = 0.f;
	}

	_float SoundSize = 80 - fDistance;

	CSoundMgr::GetInstance()->Set_EffectVolume(SoundSize, SFX_TYPE::SFX_CRYADDLOOKAT);


	if (fDistance >= 100.f)
	{
		CSound* pRunSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_CRYADDLOOKAT);

		if (true == pRunSound->IsPlaying())
		{
			CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_CRYADDLOOKAT);
			
		}
	}

	_vector CurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_bool bCollision = { false };
	pTargetCollider = nullptr;

	if (true == CUI_Manager::GetInstance()->Get_SpherUse() || true == CUI_Manager::GetInstance()->Get_FinalEnd())
	{
		m_pModelCom->Play_Animation(0, m_iChanging);
	}
	else
	{
		m_isDynamiteChase = TraceDynamite(fTimeDelta);

		if (m_isDynamiteChase == true)
		{
			m_pModelCom->Play_Animation(fTimeDelta, m_iChanging);

			_vector		vWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			if (true == m_pGameInstance->isIn_Frustum_InWorldSpace(vWorldPos, 2.f))
			{
				m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
			}

			if (m_fLookAtInterval > 0.0f && false == test3)
			{
				m_fLookAtInterval -= fTimeDelta;
			}
			else if (m_fLookAtInterval > 0.0f && true == test3)
			{
				m_fLookAtInterval = m_confLookatIntervalTime;
			}


			return;
		}

		DoorBump(fTimeDelta);
	
		if (m_bDoorBump == false)
			Action_Change(fTimeDelta);

		if (m_bDoorBump == false)
		{
			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider"), i));

				bCollision = m_pColliderCrying[CRY_OBB]->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);

					for (auto& pCollider : m_pColliderCrying)
						pCollider->Update(m_pTransformCom->Get_WorldMatrix());

					m_bDoorBump = true;
				}
			}
		}
		

		

		m_pModelCom->Play_Animation(fTimeDelta, m_iChanging);



		if (m_fLookAtInterval > 0.0f && false == test3)
		{
			m_fLookAtInterval -= fTimeDelta;
		}
		else if (m_fLookAtInterval > 0.0f && true == test3)
		{
			m_fLookAtInterval = m_confLookatIntervalTime;
		}

		if (m_fWalkInterval > 0.0f)
		{
			m_fWalkInterval -= fTimeDelta;
		}
	}



	_vector		vWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (true == m_pGameInstance->isIn_Frustum_InWorldSpace(vWorldPos, 2.f))
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}
}

HRESULT CryingGirl::Render()
{
	__super::Render();

	//for (auto& pColliderCom : m_pColliderCrying)
	//	pColliderCom->Render();

	return S_OK;
}

HRESULT CryingGirl::Ready_Components(COLLIDERDESC* pDesc)
{

	CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};

	ColliderSphereDesc.fRadius = pDesc->fRadius;
	ColliderSphereDesc.vCenter = pDesc->vCenter;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere0"), reinterpret_cast<CComponent**>(&m_pColliderCrying[CRY_SPHERE0]), &ColliderSphereDesc)))
		return E_FAIL;

	ColliderSphereDesc.fRadius = pDesc->fRadius + 20.f;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere1"), reinterpret_cast<CComponent**>(&m_pColliderCrying[CRY_SPHERE1]), &ColliderSphereDesc)))
		return E_FAIL;

	ColliderSphereDesc.fRadius = pDesc->fRadius + 100.f;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere2"), reinterpret_cast<CComponent**>(&m_pColliderCrying[CRY_SPHERE2]), &ColliderSphereDesc)))
		return E_FAIL;



	/* Com_Collider_OBB */
	CBounding_OBB::BOUND_OBB_DESC		ColliderOBBDesc{};

	ColliderOBBDesc.vExtents = pDesc->vExtents;
	ColliderOBBDesc.vCenter = pDesc->vCenter;
	ColliderOBBDesc.vRotation = pDesc->vRotation;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCrying[CRY_OBB]), &ColliderOBBDesc)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CryingGirl"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iStartIndex = m_iNaviMeshIndex;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(0.9f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vSpecular = _float4(0.85f, 0.26f, 0.33f, 1.f);
	LightDesc.fRange = _float(10.f);
	LightDesc.vPosition = { pDesc->vCenter.x, pDesc->vCenter.y , pDesc->vCenter.z, 1.f };
	LightDesc.TurnOn_Off = false;
	
	m_MonsterForLight = 2;

	if (FAILED(m_pGameInstance->Add_MonsterLight(LightDesc, m_MonsterForLight)))
		return E_FAIL;

	__super::Ready_Components(nullptr);

	return S_OK;


}

HRESULT CryingGirl::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

HRESULT CryingGirl::Set_AnimationStates()
{
	m_vecAnimStates.resize(CRY_ANIM::CRY_ANIM_END);

	m_vecAnimStates[CRY_ANIM::CRY_ANIM_CRY]			= 0;
	m_vecAnimStates[CRY_ANIM::CRY_ANIM_AROUND]		= 1;
	m_vecAnimStates[CRY_ANIM::CRY_ANIM_SPLINT]		= 2;
	m_vecAnimStates[CRY_ANIM::CRY_ANIM_WALK]		= 3;

	return S_OK;
}

void CryingGirl::Pri_Update_Cancle_LoopState()
{
	if (COND_CRY)
		m_iState ^= CRY_STATE_CRY;

	if (COND_AROUND)
		m_iState ^= CRY_STATE_AROUND;

	if (COND_SPLINT)
		m_iState ^= CRY_STATE_SPLINT;

	if (COND_WALK)
		m_iState ^= CRY_STATE_WALK;
}

_bool CryingGirl::TraceDynamite(_float fTimeDelta)
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

			bCollision = m_pColliderCrying[CRY_SPHERE0]->Intersect(pTargetCollider);
			ObbCollision = m_pColliderCrying[CRY_OBB]->Intersect(pTargetCollider);

			if (bCollision == true)
			{
				_float3 DynamitePos = pTargetCollider->Get_Bounding()->Get_Pos();
				_vector Dynamitevec = XMVectorSet(DynamitePos.x, m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1], DynamitePos.z, 1.f);

				if (m_pModelCom->Get_Current_AnimIndex() == m_vecAnimStates[CRY_ANIM_SPLINT])
				{
					m_iState |= CRY_ANIM::CRY_ANIM_SPLINT;

					m_pTransformCom->Set_Speed(14.f);
					Pri_Update_Cancle_LoopState();

					m_pTransformCom->LookAt(Dynamitevec);

					if (ObbCollision == false)
						m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

					return true;
				}
				else
				{
					m_pModelCom->Move_NextAnim(m_vecAnimStates[CRY_ANIM_SPLINT], 0.f, true);
					Pri_Update_Cancle_LoopState();
					m_iState = CRY_ANIM::CRY_ANIM_SPLINT;
					m_pTransformCom->Set_Speed(14.f);

					m_pTransformCom->LookAt(Dynamitevec);

					if (ObbCollision == false)
						m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
				}
				return true;
			}
		}
	}

	return false;
}

void CryingGirl::Action_Change(_float fTimeDelta)
{
	m_PlayerPosition = m_pPlayer->Get_Position();
	_bool PlayerHide = CUI_Manager::GetInstance()->Get_Player()->Get_PlayerHide();
	CUI_Manager::GetInstance()->Cal_DitanceSoundSize(SFX_TYPE::SFX_CRYWAIT, m_pTransformCom);
	
	_vector PlayerPos = CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	_float fDistance = (XMVectorGetX(XMVector3Length(PlayerPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION))));

	if (fDistance >= 40 && fDistance <= 60)
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_CRYWAIT, false);

	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player1")));

	_bool test1 = m_pColliderCrying[CRY_SPHERE0]->Intersect(pTargetCollider);
	_bool test2 = m_pColliderCrying[CRY_SPHERE1]->Intersect(pTargetCollider);
	_bool test3 = m_pColliderCrying[CRY_SPHERE2]->Intersect(pTargetCollider);

	int a = 0;

	//for (auto& pCollider : m_pColliderCrying)
	//{
		if (true == m_pColliderCrying[CRY_SPHERE0]->Intersect(pTargetCollider) && 
			true == m_pColliderCrying[CRY_SPHERE1]->Intersect(pTargetCollider) &&
			true == m_pColliderCrying[CRY_SPHERE2]->Intersect(pTargetCollider) &&
			PlayerHide == false)
		{
			Action_Splint(fTimeDelta);
			
		}
		else if (true == m_pColliderCrying[CRY_SPHERE0]->Intersect(pTargetCollider) &&
			true == m_pColliderCrying[CRY_SPHERE1]->Intersect(pTargetCollider) &&
			true == m_pColliderCrying[CRY_SPHERE2]->Intersect(pTargetCollider) && 
			PlayerHide == true)
		{
			Action_Around(fTimeDelta);
			m_fHideCurrentTime += fTimeDelta;
			if (m_fHideCurrentTime > 10.f)
			{
				Action_Walk(fTimeDelta);
			}
		}
		else if (true == m_pColliderCrying[CRY_SPHERE0]->Intersect(pTargetCollider) &&
			true == m_pColliderCrying[CRY_SPHERE1]->Intersect(pTargetCollider) &&
			PlayerHide == false)
		{
			Action_Walk(fTimeDelta);

			if (m_bHidePlayer == true)
			{
				Action_Splint(fTimeDelta);
			}
		}
		else if (true == m_pColliderCrying[CRY_SPHERE0]->Intersect(pTargetCollider) &&
			true == m_pColliderCrying[CRY_SPHERE1]->Intersect(pTargetCollider) &&
			PlayerHide == true)
		{
			Action_Walk(fTimeDelta); // 이거 문제일 가능 성이 높음
			m_fHideCurrentTime = 0.f;
		}
		else if (true == m_pColliderCrying[CRY_SPHERE1]->Intersect(pTargetCollider) && PlayerHide == true)
		{
			Action_Walk(fTimeDelta);
			m_fHideCurrentTime = 0.f;
		}
		else if (true == m_pColliderCrying[CRY_SPHERE1]->Intersect(pTargetCollider) && 
			true == m_pColliderCrying[CRY_SPHERE2]->Intersect(pTargetCollider) &&
			PlayerHide == true)
		{
			Action_Walk(fTimeDelta);
			m_fHideCurrentTime = 0.f;
		}
		else if (true == m_pColliderCrying[CRY_SPHERE1]->Intersect(pTargetCollider) &&
			true == m_pColliderCrying[CRY_SPHERE2]->Intersect(pTargetCollider) &&
			PlayerHide == false)
		{
			if (m_bChasePlayer == true)
			{
				Action_Splint(fTimeDelta);
				return;
			}
			Action_Walk(fTimeDelta);
			m_fHideCurrentTime = 0.f;
		}
		else if (true == m_pColliderCrying[CRY_SPHERE2]->Intersect(pTargetCollider) && PlayerHide == false)
		{
			if (m_bChasePlayer == true)
			{
				Action_Around(fTimeDelta);
				return;
			}
			Action_Around(fTimeDelta);
			m_fHideCurrentTime = 0.f;
		}
		else if (true == m_pColliderCrying[CRY_SPHERE2]->Intersect(pTargetCollider) && PlayerHide == true && m_bChasePlayer == true)
		{
			Action_Walk(fTimeDelta);
			m_fHideCurrentTime = 0.f;
		}
		else if (true == m_pColliderCrying[CRY_SPHERE2]->Intersect(pTargetCollider) && PlayerHide == true)
		{
			Action_Crying(fTimeDelta);
			m_fHideCurrentTime = 0.f;
		}
		else if (true == m_pColliderCrying[CRY_SPHERE2]->Intersect(pTargetCollider) && m_bChasePlayer == true)
		{
			Action_Walk(fTimeDelta);
		}
		
		else if (false == m_pColliderCrying[CRY_SPHERE2]->Intersect(pTargetCollider))
		{
			//if (m_bChasePlayer == true)
			//{
			//	//Action_Crying(fTimeDelta);
			//	Action_Around(fTimeDelta);
			//	return;
			//}
			Action_Crying(fTimeDelta);
			m_fHideCurrentTime = 0.f;
		}


		///////////////////////////////////////////////////////////////////////



		if (true == m_pColliderCrying[CRY_OBB]->Intersect(pTargetCollider))
		{
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PLAYERDEAD, false);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_REVIVE, false);
			
			CUI_Manager::GetInstance()->Get_Player()->Set_PlayerDeath(true);
			CUI_Manager::GetInstance()->Set_PlayerRevive(false);
		}



	//}
}

void CryingGirl::Action_Crying(_float fTimeDelta)
{



	CSound* DokoniSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_CRYWAIT);

	if (false == DokoniSound->IsPlaying())
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_CRYWAIT, false);
	}

	if (m_pModelCom->Get_Current_AnimIndex() == m_vecAnimStates[CRY_ANIM_CRY])
	{
		m_iState |= CRY_ANIM::CRY_ANIM_CRY;
		
		//__super::Monster_Turn(fTimeDelta);
		m_pTransformCom->Set_Speed(2.5f);
		Pri_Update_Cancle_LoopState();
		m_pGameInstance->MonsterLight_TurnOnOff(2, false);

		return;
	}

	//Pri_Update_Cancle_LoopState();
	m_pModelCom->Move_NextAnim(m_vecAnimStates[CRY_ANIM_CRY], 0.f, true);
	Pri_Update_Cancle_LoopState();
	m_pGameInstance->MonsterLight_TurnOnOff(2, false);
	m_iState = CRY_ANIM::CRY_ANIM_CRY;
	m_pTransformCom->Set_Speed(2.5f);

}

void CryingGirl::Action_Around(_float fTimeDelta)
{
	if (m_pModelCom->Get_Current_AnimIndex() == m_vecAnimStates[CRY_ANIM_AROUND])
	{
		m_iState |= CRY_ANIM::CRY_ANIM_AROUND;

		//__super::Monster_Turn(fTimeDelta);
		m_pTransformCom->Set_Speed(2.f);
		Pri_Update_Cancle_LoopState();
		m_pGameInstance->MonsterLight_TurnOnOff(2, false);
	

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

	m_pModelCom->Move_NextAnim(m_vecAnimStates[CRY_ANIM_AROUND], 0.f, true);
	Pri_Update_Cancle_LoopState();

	m_iState = CRY_ANIM::CRY_ANIM_AROUND;
	m_pTransformCom->Set_Speed(2.5f);
}

void CryingGirl::Action_Walk(_float fTimeDelta)
{
	if (m_pModelCom->Get_Current_AnimIndex() == m_vecAnimStates[CRY_ANIM_WALK])
	{
		m_iState |= CRY_ANIM::CRY_ANIM_WALK;

		__super::Monster_Turn(fTimeDelta);
		m_pTransformCom->Set_Speed(0.3f);
		Pri_Update_Cancle_LoopState();
		m_pGameInstance->MonsterLight_TurnOnOff(2, true);
		return;
	}

	m_pModelCom->Move_NextAnim(m_vecAnimStates[CRY_ANIM_WALK], 0.f, true);
	Pri_Update_Cancle_LoopState();
	__super::Monster_Turn(fTimeDelta);
	m_iState = CRY_ANIM::CRY_ANIM_WALK;
	m_pTransformCom->Set_Speed(0.3f);
	m_pGameInstance->MonsterLight_TurnOnOff(2, true);
}

void CryingGirl::Action_Splint(_float fTimeDelta)
{
	if (m_fLookAtInterval <= 0.0f)
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_CRYLOOKAT, false);
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_CRYADDLOOKAT, false);
		m_fLookAtInterval = m_confLookatIntervalTime;
	}

	if (m_fWalkInterval <= 0.0f)
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_CRYWALK, false);
		m_fWalkInterval = m_confWalkInterval;
	}


	if (m_pModelCom->Get_Current_AnimIndex() == m_vecAnimStates[CRY_ANIM_SPLINT])
	{
		m_iState |= CRY_ANIM::CRY_ANIM_SPLINT;

		__super::Monster_Turn(fTimeDelta);
		m_pTransformCom->Set_Speed(14.f);
		Pri_Update_Cancle_LoopState();
		m_pGameInstance->MonsterLight_TurnOnOff(2, true);
		CUI_Manager::GetInstance()->Set_PlayerHeartBeat(true);
		return;
	}

	m_pModelCom->Move_NextAnim(m_vecAnimStates[CRY_ANIM_SPLINT], 0.f, true);
	Pri_Update_Cancle_LoopState();
	__super::Monster_Turn(fTimeDelta);
	m_iState = CRY_ANIM::CRY_ANIM_SPLINT;
	m_pTransformCom->Set_Speed(14.f);
	m_pGameInstance->MonsterLight_TurnOnOff(2, true);
	CUI_Manager::GetInstance()->Set_PlayerHeartBeat(true);


}

void CryingGirl::DoorBump(_float fTimeDelta)
{
	_bool bCollision = { false };
	_vector CurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CCollider*	pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player1")));
	_bool		bBumpPlayer = m_pColliderCrying[CRY_SPHERE0]->Intersect(pTargetCollider);

	if (m_bDoorBump == false)
	{
		//if (m_pTransformCom->Get_Speed() >= 4.0f)
		//{
		//	m_pTransformCom->Set_Speed(3.0f);
		//}
		return;
	}

	// 문과 충돌을 하고 플레이어와 충돌했을 경우
	if (m_bDoorBump == true && bBumpPlayer == true)
	{
		// 플레이어를 따라간다. 그런데 문과 충돌하면 그자리 유지한다.

		if (m_pTransformCom->Get_Speed() >= 4.0f)
		{
			m_pTransformCom->Set_Speed(3.0f);
		}

		if (m_pGameInstance->MonsterLight_TurnOnOff(2, true))
			return;

		__super::Monster_Turn(fTimeDelta);

		for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider")); ++i)
		{
			pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider"), i));

			bCollision = m_pColliderCrying[CRY_OBB]->Intersect(pTargetCollider);

			if (bCollision == true)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);

				for (auto& pCollider : m_pColliderCrying)
					pCollider->Update(m_pTransformCom->Get_WorldMatrix());

				return;
			}
		}




	}
	// 문과 충돌을 하고 플레이어와 충돌을 안했을 경우
	else if (m_bDoorBump == true && bBumpPlayer == false && m_fDoorBumpBackTime < 8.f)
	{
		// 바로 원래자리로 보내버린다.

		m_pGameInstance->MonsterLight_TurnOnOff(2, false);

		m_pTransformCom->LookAt(m_matSpawnPos);
		m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		m_pTransformCom->Set_Speed(7.f);
		m_fDoorBumpBackTime += fTimeDelta;
	}
	else if (m_bDoorBump == true && m_fDoorBumpBackTime >= 8.f)
	{
		m_fDoorBumpBackTime = 0.f;
		m_bDoorBump = false;
		return;
	}


	
}

CryingGirl* CryingGirl::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CryingGirl* pInstance = new CryingGirl(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CryingGirl Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CryingGirl::Clone(void* pArg)
{
	CryingGirl* pInstance = new CryingGirl(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CryingGirl Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CryingGirl::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	for (auto& pColliderCom : m_pColliderCrying)
		Safe_Release(pColliderCom);
}

