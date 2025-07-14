#include "stdafx.h"
#include "MenMonster.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Cabinet.h"
#include "Sound.h"

CMenMonster::CMenMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{
}

CMenMonster::CMenMonster(const CMenMonster& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CMenMonster::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CMenMonster::Initialize(void* pArg)
{
	MONSTER_DESC* Desc = static_cast<MONSTER_DESC*>(pArg);

	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	COLLIDERDESC colDesc{};
	colDesc.vCenter = { 0.f, 0.f, 0.f };
	colDesc.fRadius = 180.f;
	colDesc.vExtents = { 25.f , 50.f, 25.f };
	colDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

	if (FAILED(Ready_Components(&colDesc)))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(0, true);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));

	m_matSpawnPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	return S_OK;
}

void CMenMonster::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CMenMonster::Update(_float fTimeDelta)
{
	for (auto& pColliderCom : m_pColliderMen)
		pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	LightPosition(0, 5.f, 3.f, m_pTransformCom);

	return OBJ_NOEVENT;
}

void CMenMonster::Late_Update(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->Cal_DitanceSoundSize(SFX_TYPE::SFX_MENWAIT, m_pTransformCom);
	CUI_Manager::GetInstance()->Cal_DitanceSoundSize(SFX_TYPE::SFX_MENWALK, m_pTransformCom);

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

	CSoundMgr::GetInstance()->Set_EffectVolume(SoundSize, SFX_TYPE::SFX_MENADDLOOKAT);


	if (fDistance <= 80 && m_fPlaySound <= 0.0f)
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_MENWAIT, false);
		m_fPlaySound = m_confIntervalSound;
	}

	if (fDistance >= 100.f)
	{
		CSound* pRunSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_MENADDLOOKAT);

		if (true == pRunSound->IsPlaying())
		{
			CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_MENADDLOOKAT);

		}
	}



	if (true == CUI_Manager::GetInstance()->Get_SpherUse() || true == CUI_Manager::GetInstance()->Get_FinalEnd())
	{
		m_pModelCom->Play_Animation(0, m_iChanging);
	}
	else
	{

		_vector CurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_bool bCollision = { false };
		CCollider* pTargetCollider = nullptr;

		m_isDynamiteChase = TraceDynamite(fTimeDelta);

		if (m_isDynamiteChase == true)
		{
			m_pModelCom->Play_Animation(fTimeDelta, m_iChanging);

			_vector		vWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			if (true == m_pGameInstance->isIn_Frustum_InWorldSpace(vWorldPos, 2.f))
			{
				m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
			}

			if (m_fPlaySound > 0.0f)
			{
				m_fPlaySound -= fTimeDelta;
			}

			return;
		}
		
		TracePlayer(fTimeDelta);


		if (m_bDoorBump != true)
		{
			for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider")); ++i)
			{
				pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider"), i));

				bCollision = m_pColliderMen[MEN_OBB]->Intersect(pTargetCollider);

				if (bCollision == true)
				{
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);

					for (auto& pCollider : m_pColliderMen)
						pCollider->Update(m_pTransformCom->Get_WorldMatrix());

					/* Door 충돌 추가 관련*/
					m_bDoorBump = true;
				}
			}
		
		}
		Back_DoorBump(fTimeDelta);
		KidnapPlayer(fTimeDelta);
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
	_bool test3 = m_pColliderMen[MEN_SPHERE1]->Intersect(pPlayerTargetCollider);

	if (m_fPlayLookAtSoundTime > 0.0f && false == test3)
	{
		m_fPlayLookAtSoundTime -= fTimeDelta;
	}
	else if (m_fPlayLookAtSoundTime > 0.0f && true == test3)
	{
		m_fPlayLookAtSoundTime = m_confPlayLookAtSoundTime;
	}

	if (m_fWalkSoundTime > 0.0f)
	{
		m_fWalkSoundTime -= fTimeDelta;
	}

}

HRESULT CMenMonster::Render()
{
	__super::Render();

	//for (auto& pColliderCom : m_pColliderMen)
	//	pColliderCom->Render();

	return S_OK;
}

HRESULT CMenMonster::Ready_Components(COLLIDERDESC* pDesc)
{

	CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};

	ColliderSphereDesc.fRadius = pDesc->fRadius;
	ColliderSphereDesc.vCenter = pDesc->vCenter;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere0"), reinterpret_cast<CComponent**>(&m_pColliderMen[MEN_SPHERE0]), &ColliderSphereDesc)))
		return E_FAIL;

	//ColliderSphereDesc.fRadius = pDesc->fRadius + 300.f;

	ColliderSphereDesc.fRadius = pDesc->fRadius + 60.f;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere1"), reinterpret_cast<CComponent**>(&m_pColliderMen[MEN_SPHERE1]), &ColliderSphereDesc)))
		return E_FAIL;





	/* Com_Collider_OBB */
	CBounding_OBB::BOUND_OBB_DESC		ColliderOBBDesc{};

	ColliderOBBDesc.vExtents = pDesc->vExtents;
	ColliderOBBDesc.vCenter = pDesc->vCenter;
	ColliderOBBDesc.vRotation = pDesc->vRotation;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderMen[MEN_OBB]), &ColliderOBBDesc)))
		return E_FAIL;




	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MenMonster"),
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
	LightDesc.fRange = _float(20.f);
	LightDesc.vPosition = { pDesc->vCenter.x, pDesc->vCenter.y , pDesc->vCenter.z, 1.f };
	LightDesc.TurnOn_Off = false;
	
	m_MonsterForLight = 0;

	if (FAILED(m_pGameInstance->Add_MonsterLight(LightDesc, m_MonsterForLight)))
		return E_FAIL;


	__super::Ready_Components(nullptr);

	return S_OK;


}

HRESULT CMenMonster::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

void CMenMonster::KidnapPlayer(_float fTimeDelta)
{
	_bool PlayerHide = m_pPlayer->Get_PlayerHide();
	m_PlayerPosition = m_pPlayer->Get_Position();
	CCollider* pPlayerCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player1")));
	CCollider* pCabinetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CabinetProps"), TEXT("Com_Collider")));

	for (auto& pCollider : m_pColliderMen)
	{
		if (pCabinetCollider != nullptr)
		{
			pCollider->Intersect(pCabinetCollider);

			if (true == m_pColliderMen[MEN_SPHERE0]->Intersect(pCabinetCollider) && true == PlayerHide)
			{
				CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, CUI_Manager::GetInstance()->Get_Player()->Get_PrePos());
				CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_CABBINET);
				m_pPlayer->Set_PlayerHide(false);
			}
		}
	}
}

_bool CMenMonster::TraceDynamite(_float fTimeDelta)
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

			bCollision = m_pColliderMen[MEN_SPHERE1]->Intersect(pTargetCollider);
			ObbCollision = m_pColliderMen[MEN_OBB]->Intersect(pTargetCollider);

			if (bCollision == true)
			{
				_float3 DynamitePos = pTargetCollider->Get_Bounding()->Get_Pos();
				_vector Dynamitevec = XMVectorSet(DynamitePos.x, m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1], DynamitePos.z, 1.f);
				m_pTransformCom->LookAt(Dynamitevec);
				m_pGameInstance->MonsterLight_TurnOnOff(m_MonsterForLight, true);

				if (ObbCollision == false)
					m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

				return true;
			}
			else
				m_pGameInstance->MonsterLight_TurnOnOff(m_MonsterForLight, false);
		}
	}

	return false;
}

void CMenMonster::TracePlayer(_float fTimeDelta)
{
	m_PlayerPosition = m_pPlayer->Get_Position();
	CCollider* pPlayerCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player1")));


	for (auto& pCollider : m_pColliderMen)
	{
		pCollider->Intersect(pPlayerCollider);

		if (true == m_pColliderMen[MEN_SPHERE1]->Intersect(pPlayerCollider))
		{
			m_pGameInstance->MonsterLight_TurnOnOff(m_MonsterForLight, true);
			CUI_Manager::GetInstance()->Set_PlayerHeartBeat(true);
			_vector PlayerPosition;
			_vector SpawnPosition;

			PlayerPosition = m_pPlayer->Get_Position();
			m_pTransformCom->LookAt(PlayerPosition);

			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

			if (m_fPlayLookAtSoundTime <= 0.0f)
			{
				CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_MENLOOKAT, false);
				CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_MENADDLOOKAT, false);
				
				m_fPlayLookAtSoundTime = m_confPlayLookAtSoundTime;
			}

			if (m_fWalkSoundTime <= 0.0f)
			{
				CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_MENWALK, false);
				m_fWalkSoundTime = m_fconWalkSoundTime;
			}
			

		}
		else
		{
			m_pGameInstance->MonsterLight_TurnOnOff(m_MonsterForLight, false);
			CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_MENWALK);
			m_fWalkSoundTime = 0.0f;

		}
			
	}


	///////////////////////////////////////////////////////////////////////



	if (true == m_pColliderMen[MEN_OBB]->Intersect(pPlayerCollider))
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_PLAYERDEAD, false);
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_REVIVE, false);

		CUI_Manager::GetInstance()->Get_Player()->Set_PlayerDeath(true);
		CUI_Manager::GetInstance()->Set_PlayerRevive(false);
	}

}

void CMenMonster::Back_DoorBump(_float fTimeDelta)
{
	m_PlayerPosition = m_pPlayer->Get_Position();
	_vector PlayerPosition;
	CCollider* pPlayerCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player1")));



	// 다이너마이트와 충돌하면 그쪽으로 보내버린다.


	if (m_bDoorBump == false)
	{
		if (m_pTransformCom->Get_Speed() >= 12.f)
		{
			m_pTransformCom->Set_Speed(10.f);
		}

		return;
	}
		

	if (true == m_pColliderMen[MEN_SPHERE1]->Intersect(pPlayerCollider))
	{

		if (m_pTransformCom->Get_Speed() >= 12.f)
		{
			m_pTransformCom->Set_Speed(10.f);
		}

		// 플레이어와 충돌하면 시간을 원복 시킨다.
		m_fDoorBumpTime = 0.f;
		m_fDoorBumpBackTime = 0.f;
		TracePlayer(fTimeDelta);
		return;
	}

	// 플레이어와 충돌하지 않고 문하고만 충돌하고 있으면 시간을 증가시킨다.
	if (m_bDoorBump == true && m_fDoorBumpTime < 4.f)
	{
		m_fDoorBumpTime += fTimeDelta;
		return;
	}

	if (m_fDoorBumpTime >= 4.f)
	{
		
		if (m_fDoorBumpBackTime < 9.f)
		{
			m_fDoorBumpBackTime += fTimeDelta;

			m_pTransformCom->LookAt(m_matSpawnPos);

			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
			m_pTransformCom->Set_Speed(12.f);
		}
		else if (m_fDoorBumpBackTime >= 9.f)
		{
			m_bDoorBump = false;
			m_fDoorBumpBackTime = 0;
		}
	}
}



CMenMonster* CMenMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMenMonster* pInstance = new CMenMonster(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CMenMonster Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMenMonster::Clone(void* pArg)
{
	CMenMonster* pInstance = new CMenMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CMenMonster Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMenMonster::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	for (auto& pColliderCom : m_pColliderMen)
		Safe_Release(pColliderCom);
}

