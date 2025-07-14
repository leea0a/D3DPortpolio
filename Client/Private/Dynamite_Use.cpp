#include "stdafx.h"
#include "Dynamite_Use.h"
#include "GameInstance.h"
#include "Sound.h"


CDynamite_Use::CDynamite_Use(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CDynamite_Use::CDynamite_Use(const CDynamite_Use& Prototype)
	: CProp(Prototype)
{
}

HRESULT CDynamite_Use::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CDynamite_Use::Initialize(void* pArg)
{
	PROP_DESC* pSetDesc = static_cast<PROP_DESC*>(pArg);

	PROP_DESC pDesc;



	pDesc.fSpeedPerSec = pSetDesc->fSpeedPerSec;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = pSetDesc->iPropNo;
	pDesc.fSpeedPerSec = 20.f;
	pDesc.vScale = pSetDesc->vScale;

	m_f4OriginPos = pSetDesc->vPosition;
	m_fCalPos = { m_f4OriginPos.x, m_f4OriginPos.y, m_f4OriginPos.z };
	m_fChangeExtents = 100.f;



	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	_vector Pos = XMVectorSet(pSetDesc->vPosition.x, pSetDesc->vPosition.y, pSetDesc->vPosition.z, 1.f);

	m_pTransformCom->Scaling(0.05f, 0.05f, 0.05f);
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.0f, 0.f, 0.f), XMConvertToRadians(90.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos);
	
	
	m_pTransformCom->Set_Speed(1.5f);


	Set_PropType(TYPE_DYNAMITE_THROW);
	
	m_XMoveSpeed = XMVectorAdd(XMVectorScale(m_fLook, m_fSpeed * m_fCosAngle), XMVectorScale(m_fRight, m_fSpeed * m_fSinAngle));
	m_YMoveSpeed = m_fSpeed * m_fSinAngle - m_fGravity * m_fTime;


	m_pTransformCom->Set_State(CTransform::STATE_LOOK, CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_LOOK) * 0.05f);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_RIGHT) * 0.05f);
	m_pTransformCom->Set_State(CTransform::STATE_UP, CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_UP) * 0.1f);


	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, CUI_Manager::GetInstance()->Get_Camera()->Get_Look() * 0.05f);
	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, CUI_Manager::GetInstance()->Get_Camera()->Get_Right() * 0.05f);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, CUI_Manager::GetInstance()->Get_Camera()->Get_Up() * 0.05f);
	
	m_iDynamiteRightCount = CUI_Manager::GetInstance()->Get_DynamiteCount();
	Ready_Layer_SparkEffect(TEXT("Layer_Spark"));

	return S_OK;
}

void CDynamite_Use::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CDynamite_Use::Update(_float fTimeDelta)
{

	if (m_bFlight == true)
	{
		Parabola(fTimeDelta);
		if (m_bPlayWait == false)
		{
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_DYNAMITEWAIT, true);
			m_bPlayWait = true;
		}
		

		_vector PlayerPos = CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION);
		_float fDistance = (XMVectorGetX(XMVector3Length(PlayerPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) * 3;

		if (fDistance > 80.f)
		{
			fDistance = 80.f;
		}
		else if (fDistance < 0.f)
		{
			fDistance = 0.f;
		}

		_float SoundSize = 80 - fDistance;

		CSoundMgr::GetInstance()->Set_EffectVolume(SoundSize, SFX_TYPE::SFX_DYNAMITEWAIT);


		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= -6.0f)
		{
			m_bFlight = false;
			m_bArrive = true;
		}
	}
	else if (m_bArrive == true)
	{

		m_bArrive = false;
		m_bWait = true;
		
		
	}
	else if (m_bWait == true)
	{
		if (m_fWaitTime <= 5.f)
		{
			m_fWaitTime += fTimeDelta;
			

			_vector PlayerPos = CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION);
			_float fDistance = (XMVectorGetX(XMVector3Length(PlayerPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) * 3;

			if (fDistance > 80.f)
			{
				fDistance = 80.f;
			}
			else if (fDistance < 0.f)
			{
				fDistance = 0.f;
			}

			_float SoundSize = 80 - fDistance;

			CSoundMgr::GetInstance()->Set_EffectVolume(SoundSize, SFX_TYPE::SFX_DYNAMITEWAIT);

			return OBJ_NOEVENT;
		}

		/* 대기 완료 후*/
		m_bWait = false;
		m_bBang = true;
	}
	else if (m_bBang == true)
	{
		

		



		//CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_DYNAMITEBOMB, false);
		m_pDynamiteSparkLv1->Set_Loop(false);
		m_pDynamiteSparkLv1->Set_Show(false);

		if (m_pDynamiteSparkLv2->Get_Show() == false)
		{
			m_pDynamiteSparkLv2->Set_Show(true);
			m_pDynamiteSparkLv2->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0],
				m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 1.f));

			LIGHT_DESC			LightDesc{};

			ZeroMemory(&LightDesc, sizeof LightDesc);

			LightDesc.eType = LIGHT_DESC::TYPE_POINT;
			LightDesc.vDiffuse = _float4(0.86f, 0.39f, 0.08f, 1.f);
			LightDesc.vAmbient = _float4(0.32f, 0.86f, 0.32f, 1.f);
			LightDesc.vSpecular = _float4(0.86f, 0.39f, 0.08f, 1.f);
			LightDesc.fRange = _float(19.7f);
			LightDesc.vPosition = _float4(m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 1.f);
			LightDesc.TurnOn_Off = false;

			if (FAILED(m_pGameInstance->Add_DynamiteLight(LightDesc, m_iDynamiteRightCount)))
				return E_FAIL;

			m_pGameInstance->DynamiteLight_TurnOnOff(m_iDynamiteRightCount, true);
			m_fDynamiteLightTime += fTimeDelta;
			m_fRandomLight = m_pGameInstance->Compute_Random(0.1, 0.35);


			


				


			CUI_Manager::GetInstance()->Set_DynamiteCount(CUI_Manager::GetInstance()->Get_DynamiteCount() + 1);


		}
		
		m_pColliderCom->Get_Bounding()->Set_Radius(280.f);

		if (m_bPlayBomb == false)
		{
			CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_DYNAMITEWAIT);
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_DYNAMITEBOMB, true);
			m_bPlayBomb = true;
		}


		_vector PlayerPos = CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION);
		_float fDistance = (XMVectorGetX(XMVector3Length(PlayerPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) * 2;

		if (fDistance > 80.f)
		{
			fDistance = 80.f;
		}
		else if (fDistance < 0.f)
		{
			fDistance = 0.f;
		}

		_float SoundSize = 80 - fDistance;

		CSoundMgr::GetInstance()->Set_EffectVolume(SoundSize, SFX_TYPE::SFX_DYNAMITEBOMB);

		
		CSound* pBombSound = CSoundMgr::GetInstance()->Get_EffectSound(SFX_TYPE::SFX_DYNAMITEBOMB);
		if (false == pBombSound->IsPlaying())
		{
			CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_DYNAMITEBOMB, true);
		}
		


		if (m_fDynamiteLightTime <= m_fRandomLight)
		{

			m_pGameInstance->DynamiteLight_TurnOnOff(m_iDynamiteRightCount, true);
			m_fDynamiteLightTime += fTimeDelta;
		}
		else if (m_fDynamiteLightTime > m_fRandomLight)
		{
			m_pGameInstance->DynamiteLight_TurnOnOff(m_iDynamiteRightCount, false);
			m_fDynamiteLightTime = 0.f;
			m_fRandomLight = m_pGameInstance->Compute_Random(0.1, 0.35);
		}

		if (m_bMade == false)
		{
			m_bMade = true;
		}
				
		if (m_fBangTime <= 15.f)
		{
			
			m_fBangTime += fTimeDelta;
			m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
			return OBJ_NOEVENT;
			
		}
		m_bBang = false;
		m_bDelete = true;


	}
	else if (m_bDelete == true)
	{
		/*
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DynamicThrow"), LEVEL_GAMEPLAY, strLayerTag, &Desc)))
		return E_FAIL;
		
		*/



		
		m_bPlayBomb = false;
		m_pColliderCom->Get_Bounding()->Set_Radius(0.1f);
		m_pDynamiteSparkLv2->Set_Loop(false);
		m_pDynamiteSparkLv2->Set_Show(false);
		m_pGameInstance->DynamiteLight_TurnOnOff(m_iDynamiteRightCount, false);

		if (m_bSoundBomb == false)
		{
			CUI_Manager::GetInstance()->Set_DynamiteSoundCount(-1);
			m_bSoundBomb = true;
		}
		
	}

	if (m_bMade == true)
	{
		m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
	}
	
	if (CUI_Manager::GetInstance()->Get_DynamiteSoundCount() <= 0)
	{
		
		CSoundMgr::GetInstance()->StopEffectPlay(SFX_TYPE::SFX_DYNAMITEBOMB);
	}

	int a = CUI_Manager::GetInstance()->Get_DynamiteSoundCount();
	int b = 0;
	return OBJ_NOEVENT;
}

void CDynamite_Use::Late_Update(_float fTimeDelta)
{
	if (m_bDelete == false)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CDynamite_Use::Render()
{
	if (m_bDelete == false)
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

			//if (m_bMade == true)
			//{
			//	m_pColliderCom->Render();
			//}
		}
	}

	return S_OK;
}

HRESULT CDynamite_Use::Ready_Layer_SparkEffect(const _wstring& strLayerTag)
{
	CGameObject* pGameObject = { nullptr };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_ExplosionLv1"), LEVEL_GAMEPLAY, strLayerTag, nullptr, &pGameObject)))
		return E_FAIL;

	m_pDynamiteSparkLv1 = dynamic_cast<CParticle_Spark*>(pGameObject);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_ExplosionLv2"), LEVEL_GAMEPLAY, strLayerTag, nullptr, &pGameObject)))
		return E_FAIL;

	m_pDynamiteSparkLv2 = dynamic_cast<CParticle_SparkLv2*>(pGameObject);
	


	return S_OK;
}

HRESULT CDynamite_Use::Ready_Components(void* pArg)
{
	CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};
	ColliderSphereDesc.fRadius = 0.01f;
	ColliderSphereDesc.vCenter = _float3(0.f, 0.f, 0.f);/*_float3(
		m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0],
		m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1],
		m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2]);*/
	m_fOriginalExtents = ColliderSphereDesc.fRadius;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider1"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderSphereDesc)))
		return E_FAIL;


	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iStartIndex = CUI_Manager::GetInstance()->Get_Player()->Get_Navigation()->Get_Index();
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_DynamiteThrow"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamite_Use::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

void CDynamite_Use::Parabola(_float fTimeDelta)
{
	_float fGravity = 9.8f;
	 m_f4OriginPos.y += m_f4OriginPos.y * fTimeDelta - 0.5f * fGravity * fTimeDelta * fTimeDelta;
	 m_pTransformCom->Go_Straight(fTimeDelta * 3.f, m_pNavigationCom);
	//m_pTransformCom->Go_Straight_for_Camera(fTimeDelta * 3.f);
	
	_vector vecPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//m_pDynamiteSparkLv1->Get_SparkViBuffer(0)->Render();
	vecPos.m128_f32[1] = m_f4OriginPos.y;

	if (vecPos.m128_f32[1] <= -6.0f)
	{
		vecPos.m128_f32[1] = -6.0f;
	}
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPos);
	m_pDynamiteSparkLv1->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], 
		m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 1.f));
}

void CDynamite_Use::Parabola_2(_float fTimeDelta)
{
	_float fGravity = 9.8f;

	if (m_bMaxHeight == false)
	{
		
		m_f4OriginPos.y += /*m_f4OriginPos.y **/ fTimeDelta + 3.f * fGravity * fTimeDelta * fTimeDelta;

		m_pTransformCom->Go_Straight(fTimeDelta * 3.f, m_pNavigationCom);
		//m_pTransformCom->Go_Straight_for_Camera(fTimeDelta * 1.2f);

		_vector vecPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		vecPos.m128_f32[1] = m_f4OriginPos.y;

		if (vecPos.m128_f32[1] >= -5.7f)
		{
			vecPos.m128_f32[1] = -5.7f;
			m_bMaxHeight = true;
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPos);
	}
	else if (m_bMaxHeight == true)
	{
		m_f4OriginPos.y += m_f4OriginPos.y *  fTimeDelta - 0.000001f * fGravity * fTimeDelta * fTimeDelta;

		m_pTransformCom->Go_Straight_for_Camera(fTimeDelta * 1.9);

		_vector vecPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		vecPos.m128_f32[1] = m_f4OriginPos.y;

		if (vecPos.m128_f32[1] <= -6.0f)
		{
			vecPos.m128_f32[1] = -6.0f;
			m_bPara2 = true;
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPos);
	}

	
}

CDynamite_Use* CDynamite_Use::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDynamite_Use* pInstance = new CDynamite_Use(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CDynamite_Use Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CDynamite_Use::Clone(void* pArg)
{
	CDynamite_Use* pInstance = new CDynamite_Use(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CDynamite_Use Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDynamite_Use::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pDynamiteSparkLv1);
	Safe_Release(m_pDynamiteSparkLv2);

}

