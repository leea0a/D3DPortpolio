#include "stdafx.h"


#include "Player.h"
#include "GameInstance.h"
#include "Prop.h"
#include "Sound.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CPlayer::Initialize(void* pArg)
{

	PLAYER_DESC pDesc;

	//pDesc.vPosition = pPDesc->vPosition;
	pDesc.fSpeedPerSec		= 12.f;
	m_fOriginSpeed			= pDesc.fSpeedPerSec;
	pDesc.fRotationPerSec	= XMConvertToRadians(90.f);

	// Plorogue Stage 
	//pDesc.vPosition			= { 0.f, 0.45f , 0.f , 1.f };
	
	// Stage1
	pDesc.vPosition			= { 0.f, 0.f, 0.f, 1.f };
	
	pDesc.vScale			= { 0.1f , 0.1f, 0.1f };
	pDesc.iNaviMeshIndex	= 0;

	m_iNaviMeshIndex		= pDesc.iNaviMeshIndex;
	m_PlayerPosY			= pDesc.vPosition.y;
	
	//pDesc.iData = 10;




	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;



	//if (LEVELID::LEVEL_GAMEPLAY == m_pGameInstance->Get_CurrentLevel())
	//{

	//원본
	// Plorogue Stage 
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-36.217, -10.3f, 73.261f, 1.f));

	// Stage1
	if (m_pGameInstance->Get_Level() == 2)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-36.217, -10.3f, 73.261f, 1.f));
	}
	else if (m_pGameInstance->Get_Level() == 3)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-283.f, -6.75f, 63.465f, 1.f));
	}

		

	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(37.049, -3.f, -36.533f, 1.f));
	//}
	
	m_pTransformCom->Scaling(0.1f, 0.1f, 0.1f);

	m_f3OriginExtents = m_pColliderCom[PCOLL_OBB1]->Get_Bounding()->Get_Extents();

	///// 라이타 ///////
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(0.86f, 0.39f, 0.08f, 1.f);
	LightDesc.vAmbient = _float4(0.32f, 0.86f, 0.32f, 1.f);
	LightDesc.vSpecular = _float4(0.86f, 0.39f, 0.08f, 1.f);
	LightDesc.fRange = _float(19.7f);
	LightDesc.vPosition = { 0, 0, 0, 1.f };
	//LightDesc.vPosition = { m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 1.f};
	LightDesc.TurnOn_Off = false;

	if (FAILED(m_pGameInstance->Add_PlayerLight(0, LightDesc)))
		return E_FAIL;



	///// 스포트 라이트 /////
	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_SPOT;
	LightDesc.vDiffuse = _float4(0.9f, 0.9f, 0.9f, 1.f);
	LightDesc.vAmbient = _float4(0.32f, 0.86f, 0.32f, 1.f);
	LightDesc.vSpecular = _float4(0.86f, 0.39f, 0.08f, 1.f);
	LightDesc.fRange = _float(41.7f);

	_vector vCameraPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vCameraLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	LightDesc.vPosition = _float4(XMVectorGetX(vCameraPos), XMVectorGetY(vCameraPos), XMVectorGetZ(vCameraPos), 1.f);
	LightDesc.vDirection = _float4(XMVectorGetX(vCameraLook), XMVectorGetY(vCameraLook), XMVectorGetZ(vCameraLook), 0.f);
	// 백업 //
	//LightDesc.vPosition = { 0.f, 0.f, 0.f, 1.f };
	//LightDesc.vPosition = { CUI_Manager::GetInstance()->Get_Player()->Get_Position().m128_f32[0], CUI_Manager::GetInstance()->Get_Player()->Get_Position().m128_f32[1] + 4.1f  , CUI_Manager::GetInstance()->Get_Player()->Get_Position().m128_f32[2], 1.f };
	// 백업 //

	LightDesc.TurnOn_Off = false;
	LightDesc.fConeAngle = 30.f;
	LightDesc.vDirection = _float4(1.f, 1.f, 0.f, 0.f);
	LightDesc.fAttenuation = 8.f;


	if (FAILED(m_pGameInstance->Add_PlayerLight(1, LightDesc)))
		return E_FAIL;


	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{


	//_vector vPos = m_pNavigationCom->SetUp_OnNavigation(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	//vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + m_PlayerPosY);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);


}

_int CPlayer::Update(_float fTimeDelta)
{
	_vector CurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Update_PlayerPos();
	Player_Dead();
	FireUse(fTimeDelta);
	HandLightUse(fTimeDelta);


	
	if (true == m_bHandLightUsePlayer)
	{
		_vector vCameraPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
        _vector vCameraLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
        
		// 마지막으로 추가된 스포트라이트 업데이트
		LIGHT_DESC LightDesc;

		// 카메라 위치와 방향으로 스포트라이트 위치와 방향 업데이트
		LightDesc.vPosition = _float4(XMVectorGetX(vCameraPos), XMVectorGetY(vCameraPos), XMVectorGetZ(vCameraPos), 1.f);

		for (_int i = 0; i < m_pGameInstance->Get_PlayerLightCount(); ++i)
		{
			m_pGameInstance->Update_PlayerPos(i, vCameraPos);
		}
		

		LightDesc.vDirection = _float4(XMVectorGetX(-vCameraLook), XMVectorGetY(-vCameraLook), XMVectorGetZ(-vCameraLook), 0.f);
		m_pGameInstance->Update_PlayerDir(1, LightDesc.vDirection);
    
	}

	if (CUI_Manager::GetInstance()->Get_FinalEnd() != true)
		Player_Key(fTimeDelta);

	if (GetKeyState('P') & 0x8000)
	{
		if (m_bTestNavigation == false)
		{
			m_bTestNavigation = true;
		}
		else if (m_bTestNavigation == true)
		{
			m_bTestNavigation = false;
		}
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
	m_pNavigationCom->SetUp_OnNavigation(m_pTransformCom->Get_State(CTransform::STATE_POSITION)));

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, /*m_CameraView*/ XMVectorSetY(m_CameraView, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_CameraRight);

	for (auto& pCollider : m_pColliderCom)
	{
		pCollider->Update(m_pTransformCom->Get_WorldMatrix());
	}


	_bool bCollision = { false };
	CCollider* pTargetCollider = nullptr;

	if (m_pGameInstance->Get_Level() == 2)
	{
		for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_PROLOGUE, TEXT("Layer_CollisionProps"), TEXT("Com_Collider")); ++i)
		{
			pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_PROLOGUE, TEXT("Layer_CollisionProps"), TEXT("Com_Collider"), i));

			bCollision = m_pColliderCom[PCOLL_OBB1]->Intersect(pTargetCollider);

			if (bCollision == true && m_bLighterUse == false)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);

				for (auto& pCollider : m_pColliderCom)
					pCollider->Update(m_pTransformCom->Get_WorldMatrix());

				
			}
		}

		for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_PROLOGUE, TEXT("Layer_LevelProps"), TEXT("Com_Collider")); ++i)
		{


			pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_PROLOGUE, TEXT("Layer_LevelProps"), TEXT("Com_Collider"), i));

			bCollision = m_pColliderCom[PCOLL_OBB1]->Intersect(pTargetCollider);

			if (bCollision == true)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);

				for (auto& pCollider : m_pColliderCom)
					pCollider->Update(m_pTransformCom->Get_WorldMatrix());


			}
		}

	}
	else if (m_pGameInstance->Get_Level() == 3)
	{
		for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider")); ++i)
		{
			pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CollisionProps"), TEXT("Com_Collider"), i));

			bCollision = m_pColliderCom[PCOLL_OBB1]->Intersect(pTargetCollider);

			if (bCollision == true)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);

				for (auto& pCollider : m_pColliderCom)
					pCollider->Update(m_pTransformCom->Get_WorldMatrix());
			}
		}


		for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_CabinetProps"), TEXT("Com_Collider")); ++i)
		{
			pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CabinetProps"), TEXT("Com_Collider"), i));

			bCollision = m_pColliderCom[PCOLL_OBB1]->Intersect(pTargetCollider);

			if (bCollision == true)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);

				for (auto& pCollider : m_pColliderCom)
					pCollider->Update(m_pTransformCom->Get_WorldMatrix());
			}
		}

	
		for (int i = 0; i < m_pGameInstance->Get_ComponentCount(LEVEL_GAMEPLAY, TEXT("Layer_CandleProps"), TEXT("Com_Collider")); ++i)
		{
			pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CandleProps"), TEXT("Com_Collider"), i));

			bCollision = m_pColliderCom[PCOLL_OBB1]->Intersect(pTargetCollider);

			if (bCollision == true)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);

				for (auto& pCollider : m_pColliderCom)
					pCollider->Update(m_pTransformCom->Get_WorldMatrix());
			}

			
		}

	
		pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_End"), TEXT("Com_Collider")));

		bCollision = m_pColliderCom[PCOLL_OBB1]->Intersect(pTargetCollider);

		if (bCollision == true)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrentPos);

			for (auto& pCollider : m_pColliderCom)
				pCollider->Update(m_pTransformCom->Get_WorldMatrix());
		}

	}

	if (m_bMirriorUSePlayer == true && m_bDeath == false)
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_MIRRORUSE);
		m_pGameInstance->Set_FadeSpeed(0.08f);
		m_pGameInstance->Start_Fade(m_bMirriorUSePlayer);
	}
		

	

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	//if (m_pGameInstance->Get_Level() == 2)
	//{
	//	CUI_Manager::GetInstance()->Get_Inventory()[0]->Set_ItemType(CInventory::ITEM_MIRROR);
	//	CUI_Manager::GetInstance()->Get_Inventory()[0]->Set_iTemCount(1);
	//	CUI_Manager::GetInstance()->Get_Inventory()[0]->Set_HaveItem(true);
	//	CUI_Manager::GetInstance()->Set_ChooseIndex(0);
	//}

	 if(m_pGameInstance->Get_Level() == 3)
	{
		if (m_bTest == false)
		{
			CUI_Manager::GetInstance()->Get_Inventory()[0]->Set_ItemType(CInventory::ITEM_LIGHTER);
			CUI_Manager::GetInstance()->Get_Inventory()[0]->Set_iTemCount(1);
			CUI_Manager::GetInstance()->Get_Inventory()[0]->Set_HaveItem(true);
			CUI_Manager::GetInstance()->Set_ChooseIndex(0);

			m_bTest = true;
		}
	}
	


	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);

	if (CUI_Manager::GetInstance()->Get_PlayerHeartBeat() == true)
	{
	CSound* PlayerHeartBeat = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_HEARTBEAT);

		if (true == PlayerHeartBeat->IsPlaying())
		{
			m_fHeartBeatTime += fTimeDelta;

			if (m_fHeartBeatTime > 9.f)
			{
				CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_HEARTBEAT);
				CUI_Manager::GetInstance()->Set_PlayerHeartBeat(false);
				m_fHeartBeatTime = 0.f;
			}

			return;
			
		}

		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_HEARTBEAT, false);
		m_fHeartBeatTime += fTimeDelta;

		if (m_fHeartBeatTime > 9.f)
		{
			CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_HEARTBEAT);
			CUI_Manager::GetInstance()->Set_PlayerHeartBeat(false);
			m_fHeartBeatTime = 0.f;
		}
		
	}



}

HRESULT CPlayer::Render()
{
	
#ifdef _DEBUG 

	//for (auto& pCollider : m_pColliderCom)
	//{
	//	pCollider->Render();
	//}
	//
	//m_pNavigationCom->Render();
	//if (m_bTestNavigation == true)
	//{
	//	
	//}
	

#endif 



	return S_OK;
}


void CPlayer::Cal_PlayerRunSound(_float fTimeDelta)
{
	CSound* pWalkSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_WALK);

	if (true == pWalkSound->IsPlaying())
	{
		CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_WALK);
	}

	CSound* pRunSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_RUN);
	CSound* pBreathSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_BREATH);
	CSound* pBreathEnd = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_BREATHEND);

	if (false == pRunSound->IsPlaying())
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_RUN, false);
		
		return;
	}

	if (false == pBreathSound->IsPlaying())
	{
		if (true == pBreathEnd->IsPlaying())
		{
			CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_BREATHEND);
		}

		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_BREATH, false);
	}

	if (m_fSoundTimer <= 0.0f)
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_RUN, false);
		
		m_fSoundTimer = m_confSoundInterval;
	}

	if (m_fBreathSoundTimer <= 0.0f)
	{
		if (true == pBreathEnd->IsPlaying())
		{
			CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_BREATHEND);
		}

		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_BREATH, false);
		m_fBreathSoundTimer = m_confBreathSoundInterval;
	}

}

void CPlayer::Cal_PlayerWalkSound(_float ftimeDelta)
{
	CSound* pRunSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_RUN);

	if (true == pRunSound->IsPlaying())
	{
		CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_RUN);
		CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_BREATH);
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_BREATHEND, false);
	}


	CSound* pWalkSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_WALK);
	if (false == pWalkSound->IsPlaying())
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_WALK, false);
	}

	if (m_fWalkSoundTimer <= 0.0f)
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_WALK, false);
		m_fWalkSoundTimer = m_confWalkSoundInterval;
	}

	
}

void CPlayer::cal_PlayerDownWalkSound(_float fTimeDelta)
{
	CSound* pRunSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_RUN);

	if (true == pRunSound->IsPlaying())
	{
		CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_RUN);
		CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_BREATH);
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_BREATHEND, false);
	}

	CSound* pWalkSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_WALK);
	if (false == pWalkSound->IsPlaying())
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_WALK, false);
	}



	if (false == pWalkSound->IsPlaying())
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_DOWNWALK, false);
	}

	if (m_fSoundTimer <= 0.0f)
	{
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_DOWNWALK, false);
		m_fSoundTimer = m_confWalkSoundInterval;
	}


}

HRESULT CPlayer::Ready_Components(void* pArg)
{
	if (m_pGameInstance->Get_Level() == 2)
	{
		PLAYER_DESC* pDesc = static_cast<PLAYER_DESC*>(pArg);

		CBounding_OBB::BOUND_OBB_DESC		ColliderOBBDesc{};
		ColliderOBBDesc.vExtents = _float3(1.2f, 100.f, 1.2f);
		ColliderOBBDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y /* + 10.f*/, pDesc->vPosition.z);
		ColliderOBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_Collider_Player1"), reinterpret_cast<CComponent**>(&m_pColliderCom[PCOLL_OBB1]), &ColliderOBBDesc)))
			return E_FAIL;

		ColliderOBBDesc.vExtents = _float3(10.0f, 100.f, 10.0f);

		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_Collider_Player2"), reinterpret_cast<CComponent**>(&m_pColliderCom[PCOLL_OBB2]), &ColliderOBBDesc)))
			return E_FAIL;


		CNavigation::NAVIGATION_DESC		NaviDesc{};
		NaviDesc.iStartIndex = m_iNaviMeshIndex;
		if (FAILED(Add_Component(LEVEL_PROLOGUE, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
			return E_FAIL;



		CUI_Manager::GetInstance()->Set_Navigation(m_pNavigationCom);

	}
	else if (m_pGameInstance->Get_Level() == 3)
	{
		PLAYER_DESC* pDesc = static_cast<PLAYER_DESC*>(pArg);

		CBounding_OBB::BOUND_OBB_DESC		ColliderOBBDesc{};
		ColliderOBBDesc.vExtents = _float3(1.2f, 100.f, 1.2f);
		ColliderOBBDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y /* + 10.f*/, pDesc->vPosition.z);
		ColliderOBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_Collider_Player1"), reinterpret_cast<CComponent**>(&m_pColliderCom[PCOLL_OBB1]), &ColliderOBBDesc)))
			return E_FAIL;

		ColliderOBBDesc.vExtents = _float3(10.0f, 100.f, 10.0f);

		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_Collider_Player2"), reinterpret_cast<CComponent**>(&m_pColliderCom[PCOLL_OBB2]), &ColliderOBBDesc)))
			return E_FAIL;


		CNavigation::NAVIGATION_DESC		NaviDesc{};
		NaviDesc.iStartIndex = m_iNaviMeshIndex;
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
			return E_FAIL;
	}

	

	return S_OK;
}

HRESULT CPlayer::Bind_ShaderResources()
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

_vector CPlayer::Get_Position()
{
 	return m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
}

void CPlayer::Player_Key(_float fTimeDelta)
{
	_float3 ChangeExtents = {};

	if (m_bLighterUse == false && m_bHandLighterUse == false)
	{
		ChangeExtents = { 1.5f, 30.f, 1.5f };
	}
	else if (m_bLighterUse == true && m_bHandLighterUse == false)
	{
		ChangeExtents = { 2.0f, 30.f, 2.0f };
	}
	else if (m_bLighterUse == false && m_bHandLighterUse == true)
	{
		ChangeExtents = { 3.0f, 30.f, 3.0f };
	}

	if (GetKeyState('W') & 0x8000 || GetKeyState('A') & 0x8000 || GetKeyState('S') & 0x8000 || GetKeyState('D') & 0x8000)
	{
		if (GetKeyState('W') & 0x8000)
		{
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				if (m_fRunTime > 0.f && m_bRunPossible == true)
				{
					Set_PlayerSpeed(20.f);
					m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
					m_bRunPossible = true;
					m_fRunTime -= 0.21f;

					Cal_PlayerRunSound(fTimeDelta);
				}
				else if (m_bRunPossible == false && m_fRunTime >= 50.f)
				{

					Set_PlayerSpeed(20.f);
					m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
					m_bRunPossible = true;
					m_fRunTime -= 0.21f;

					Cal_PlayerRunSound(fTimeDelta);
				}
				else if (m_fRunTime < 50.f && m_bRunPossible == false)
				{
					//WALK

					m_fRunTime += 0.2f;
			
					if (m_pTransformCom->Get_Speed() == 18.f)
					{
						m_pTransformCom->Set_Speed(m_fOriginSpeed);
						m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
					}

					Cal_PlayerWalkSound(fTimeDelta);
				}
				else if (m_fRunTime <= 0.f)
				{	// WALK

					m_bRunPossible = false;
			
					if (m_pTransformCom->Get_Speed() == 18.f)
					{
						m_pTransformCom->Set_Speed(m_fOriginSpeed);
						m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
					}

					Cal_PlayerWalkSound(fTimeDelta);
				}
			}
			else if (GetKeyState(VK_CONTROL) & 0x8000)
			{

				m_bSit = true;
				Set_PlayerSpeed(8.f);
				m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
				ChangeExtents = { 0.8f, 30.f, 0.8f };
				m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
			
				if (m_fRunTime < 100)
				{
					m_fRunTime += 0.2f;
			
					if (m_fRunTime > 100)
						m_fRunTime = 100;
				}

				cal_PlayerDownWalkSound(fTimeDelta);
			}
			else
			{

				Set_PlayerSpeed(m_fOriginSpeed);
				m_pTransformCom->Set_Speed(m_fOriginSpeed);
				m_bSit = false;
				if (m_bLighterUse == false && m_bHandLighterUse == false)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
				}
				else if (m_bLighterUse == true && m_bHandLighterUse == false)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
				}
				else if (m_bLighterUse == false && m_bHandLighterUse == true)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
				}
			
				if (m_fRunTime <= 100.f)
				{
					m_fRunTime += 0.2f;
				}
				if (m_fRunTime > 100.f)
				{
					m_fRunTime = 100.f;
				}

				Cal_PlayerWalkSound(fTimeDelta);
				
			}
			
			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		}

		if (GetKeyState('S') & 0x8000)
		{
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				if (m_fRunTime > 0.f && m_bRunPossible == true)
				{
					Set_PlayerSpeed(20.f);
					m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
					m_bRunPossible = true;
					m_fRunTime -= 0.21f;
					Cal_PlayerRunSound(fTimeDelta);
				}
				else if (m_bRunPossible == false && m_fRunTime >= 50.f)
				{
					Set_PlayerSpeed(20.f);
					m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
					m_bRunPossible = true;
					m_fRunTime -= 0.21f;
					Cal_PlayerRunSound(fTimeDelta);
				}
				else if (m_fRunTime < 50.f && m_bRunPossible == false)
				{
					m_fRunTime += 0.2f;

					if (m_pTransformCom->Get_Speed() == 18.f)
					{
						m_pTransformCom->Set_Speed(m_fOriginSpeed);
						m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
					}
					Cal_PlayerWalkSound(fTimeDelta);
				}
				else if (m_fRunTime <= 0.f)
				{
					m_bRunPossible = false;

					if (m_pTransformCom->Get_Speed() == 18.f)
					{
						m_pTransformCom->Set_Speed(m_fOriginSpeed);
						m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
					}
					Cal_PlayerWalkSound(fTimeDelta);
				}
			}
			else if (GetKeyState(VK_CONTROL) & 0x8000)
			{
				m_bSit = true;
				Set_PlayerSpeed(8.f);
				m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
				ChangeExtents = { 0.8f, 30.f, 0.8f };
				m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);

				if (m_fRunTime < 100)
				{
					m_fRunTime += 0.2f;

					if (m_fRunTime > 100)
						m_fRunTime = 100;
				}
				cal_PlayerDownWalkSound(fTimeDelta);
			}
			else
			{
				Set_PlayerSpeed(m_fOriginSpeed);
				m_pTransformCom->Set_Speed(m_fOriginSpeed);
				m_bSit = false;
				if (m_bLighterUse == false && m_bHandLighterUse == false)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
				}
				else if (m_bLighterUse == true && m_bHandLighterUse == false)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
				}
				else if (m_bLighterUse == false && m_bHandLighterUse == true)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
				}

				if (m_fRunTime <= 100.f)
				{
					m_fRunTime += 0.2f;
				}
				if (m_fRunTime > 100.f)
				{
					m_fRunTime = 100.f;
				}
				Cal_PlayerWalkSound(fTimeDelta);
			}
			m_pTransformCom->Go_Backward(fTimeDelta, m_pNavigationCom);
		}

		if (GetKeyState('A') & 0x8000)
		{
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				if (m_fRunTime > 0.f && m_bRunPossible == true)
				{
					Set_PlayerSpeed(20.f);
					m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
					m_bRunPossible = true;
					m_fRunTime -= 0.21f;
					Cal_PlayerRunSound(fTimeDelta);
				}
				else if (m_bRunPossible == false && m_fRunTime >= 50.f)
				{
					Set_PlayerSpeed(20.f);
					m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
					m_bRunPossible = true;
					m_fRunTime -= 0.21f;
					Cal_PlayerRunSound(fTimeDelta);
				}
				else if (m_fRunTime < 50.f && m_bRunPossible == false)
				{
					m_fRunTime += 0.2f;

					if (m_pTransformCom->Get_Speed() == 18.f)
					{
						m_pTransformCom->Set_Speed(m_fOriginSpeed);
						m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
					}
					Cal_PlayerWalkSound(fTimeDelta);
				}
				else if (m_fRunTime <= 0.f)
				{
					m_bRunPossible = false;

					if (m_pTransformCom->Get_Speed() == 18.f)
					{
						m_pTransformCom->Set_Speed(m_fOriginSpeed);
						m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
					}
					Cal_PlayerWalkSound(fTimeDelta);
				}
			}
			else if (GetKeyState(VK_CONTROL) & 0x8000)
			{
				m_bSit = true;
				Set_PlayerSpeed(8.f);
				m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
				ChangeExtents = { 0.8f, 30.f, 0.8f };
				m_pColliderCom[PCOLL_OBB1]->Get_Bounding()->Set_Extents(ChangeExtents);

				if (m_fRunTime < 100)
				{
					m_fRunTime += 0.2f;

					if (m_fRunTime > 100)
						m_fRunTime = 100;
				}
				cal_PlayerDownWalkSound(fTimeDelta);
			}
			else
			{
				Set_PlayerSpeed(m_fOriginSpeed);
				m_pTransformCom->Set_Speed(m_fOriginSpeed);
				m_bSit = false;
				if (m_bLighterUse == false && m_bHandLighterUse == false)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
				}
				else if (m_bLighterUse == true && m_bHandLighterUse == false)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
				}
				else if (m_bLighterUse == false && m_bHandLighterUse == true)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
				}

				if (m_fRunTime <= 100.f)
				{
					m_fRunTime += 0.2f;
				}
				if (m_fRunTime > 100.f)
				{
					m_fRunTime = 100.f;
				}
				Cal_PlayerWalkSound(fTimeDelta);
			}

			m_pTransformCom->Go_Left(fTimeDelta, m_pNavigationCom);
		}

		if (GetKeyState('D') & 0x8000)
		{
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				if (m_fRunTime > 0.f && m_bRunPossible == true)
				{
					Set_PlayerSpeed(20.f);
					m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
					m_bRunPossible = true;
					m_fRunTime -= 0.21f;
					Cal_PlayerRunSound(fTimeDelta);
				}
				else if (m_bRunPossible == false && m_fRunTime >= 50.f)
				{
					Set_PlayerSpeed(20.f);
					m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
					m_bRunPossible = true;
					m_fRunTime -= 0.21f;
					Cal_PlayerRunSound(fTimeDelta);
				}
				else if (m_fRunTime < 50.f && m_bRunPossible == false)
				{
					m_fRunTime += 0.f;

					if (m_pTransformCom->Get_Speed() == 18.f)
					{
						m_pTransformCom->Set_Speed(m_fOriginSpeed);
						m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
					}
					Cal_PlayerWalkSound(fTimeDelta);
				}
				else if (m_fRunTime <= 0.f)
				{
					m_bRunPossible = false;

					if (m_pTransformCom->Get_Speed() == 18.f)
					{
						m_pTransformCom->Set_Speed(m_fOriginSpeed);
						m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
					}
					Cal_PlayerWalkSound(fTimeDelta);
				}
			}
			else if (GetKeyState(VK_CONTROL) & 0x8000)
			{
				m_bSit = true;
				Set_PlayerSpeed(8.f);
				m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
				ChangeExtents = { 0.8f, 30.f, 0.8f };
				m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);

				if (m_fRunTime < 100)
				{
					m_fRunTime += 0.2f;

					if (m_fRunTime > 100)
						m_fRunTime = 100;
				}
				cal_PlayerDownWalkSound(fTimeDelta);
			}
			else
			{
				Set_PlayerSpeed(m_fOriginSpeed);
				m_pTransformCom->Set_Speed(m_fOriginSpeed);
				m_bSit = false;
				if (m_bLighterUse == false && m_bHandLighterUse == false)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
				}
				else if (m_bLighterUse == true && m_bHandLighterUse == false)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
				}
				else if (m_bLighterUse == false && m_bHandLighterUse == true)
				{
					m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
				}

				if (m_fRunTime <= 100.f)
				{
					m_fRunTime += 0.2f;
				}
				if (m_fRunTime > 100.f)
				{
					m_fRunTime = 100.f;
				}
				Cal_PlayerWalkSound(fTimeDelta);
			}

			m_pTransformCom->Go_Right(fTimeDelta, m_pNavigationCom);
		}

		return;
	}
	else if(GetKeyState(VK_CONTROL) & 0x8000)
	{
		if (m_bSit == false)
		{
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_SIT);
		}


		m_bSit = true;
		Set_PlayerSpeed(8.f);
		m_pTransformCom->Set_Speed(m_Desc.fSpeedPerSec);
		ChangeExtents = { 0.8f, 30.f, 0.8f };
		m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);

		if (m_fRunTime < 100)
		{
			m_fRunTime += 0.2f;

			if (m_fRunTime > 100)
				m_fRunTime = 100;
		}

	}
	else 
	{
		if (m_bLighterUse == false && m_bHandLighterUse == false)
		{
			m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(m_f3OriginExtents);
		}
		else if (m_bLighterUse == true && m_bHandLighterUse == false)
		{
			m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
		}
		else if (m_bLighterUse == false && m_bHandLighterUse == true)
		{
			m_pColliderCom[PCOLL_OBB2]->Get_Bounding()->Set_Extents(ChangeExtents);
		}
		if (m_fRunTime < 100)
		{
			m_fRunTime += 0.2f;

			if (m_fRunTime > 100)
				m_fRunTime = 100;
		}

		CSound* pRunSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_RUN);
		CSound* pWalkSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_WALK);
		if (true == pRunSound->IsPlaying())
		{
			CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_RUN);
			CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_BREATH);
			
		}

		if (true == pWalkSound->IsPlaying())
		{
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_WALK, false);
		}

		if (m_bSit == true)
		{
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_SIT);
		}

		m_bSit = false;
	}

	if (m_fSoundTimer > 0.0f)
	{
		m_fSoundTimer -= fTimeDelta;
	}

	if (m_fWalkSoundTimer > 0.0f)
	{
		m_fWalkSoundTimer -= fTimeDelta;
	}

	if (m_fBreathSoundTimer > 0.0f)
	{
		m_fBreathSoundTimer -= fTimeDelta;
	}

}

void CPlayer::Player_Dead()
{

	CSound* DeadthSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_REVIVE);

	if (m_bDeath == true)
	{
		if (m_fPlayerReviveSound != 80.f)
		{
			m_fPlayerReviveSound = 80.f;
			CSoundMgr::GetInstance()->Set_EffectVolume(m_fPlayerReviveSound, SFX_TYPE::SFX_REVIVE);
		}
			

		if (CUI_Manager::GetInstance()->Get_PlayerRevive() == true)
		{
			if (KEY_UP(KEY::ENTER))
			{
				m_bDeath = false;

			}
		}
	}


	if (m_bDeath == false && true == DeadthSound->IsPlaying())
	{
		m_fPlayerReviveSound -= 0.5f;

		CSoundMgr::GetInstance()->Set_EffectVolume(m_fPlayerReviveSound, SFX_TYPE::SFX_REVIVE);
	}


}



HRESULT CPlayer::FireUse(_float fTimeDelta)
{
	if (CUI_Manager::GetInstance()->Get_Player() == nullptr)
		return E_FAIL;

	if (true == CUI_Manager::GetInstance()->Get_Player()->Get_LighterUse() && false == m_bLighterUsePlayer && m_bLightCool == false)
	{
		m_fLighterOnTime += fTimeDelta;
		m_fLightCoolTime += fTimeDelta;

		if (m_fLighterOnTime > 0.87f)
		{
			m_pGameInstance->PlayerLight_TurnOnOff(0, true);
			m_bLighterUsePlayer = true;
			m_bLightCool = true;
		}

	}
	else if (false == CUI_Manager::GetInstance()->Get_Player()->Get_LighterUse() && m_bLightCool == true)
	{
		m_pGameInstance->PlayerLight_TurnOnOff(0, false);
		m_bLighterUsePlayer = false;
		m_fLighterOnTime = 0.0f;
		m_fLightCoolTime = 0.0f;
		m_bLightCool = false;
	}


	return S_OK;
}

HRESULT CPlayer::HandLightUse(_float fTimeDelta)
{
	if (CUI_Manager::GetInstance()->Get_Player() == nullptr)
		return E_FAIL;

	if (CUI_Manager::GetInstance()->Get_HandLightTime() <= 0)
	{
		if (m_bHandLightUsePlayer == true)
		{
			m_pGameInstance->PlayerLight_TurnOnOff(1, false);
			m_bHandLightUsePlayer = false;
		}


		return S_OK;
	}
	
	if (true == CUI_Manager::GetInstance()->Get_Player()->Get_HandLighterUse() && false == m_bHandLightUsePlayer)
	{
		m_pGameInstance->PlayerLight_TurnOnOff(1, true);
	 
		m_bHandLightUsePlayer = true;
	
	}
	else if (false == CUI_Manager::GetInstance()->Get_Player()->Get_HandLighterUse())
	{
		m_pGameInstance->PlayerLight_TurnOnOff(1, false);
		m_bHandLightUsePlayer = false;
	}


	return S_OK;
}

void CPlayer::Update_PlayerPos()
{
	if (m_pGameInstance->Get_PlayerLightCount() >= 1)
	{
		for (_int i = 0; i < m_pGameInstance->Get_PlayerLightCount(); ++i)
		{
			m_pGameInstance->Update_PlayerPos(i, CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION));
		}
	}
}


CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Player Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Player Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Free()
{
	//m_pNavigationCom->Delete_Cell();

	Safe_Release(m_pNavigationCom);

	for (auto& pCollider : m_pColliderCom)
	{
		Safe_Release(pCollider);
	}



	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	__super::Free();


	




}

