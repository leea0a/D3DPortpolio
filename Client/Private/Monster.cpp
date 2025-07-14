#include "stdafx.h"
#include "Monster.h"
#include "GameInstance.h"
#include "UI_Manager.h"


CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CMonster::CMonster(const CMonster& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CMonster::Initialize(void* pArg)
{
	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);

	if (nullptr != pDesc)
	{
		m_Desc.iIndex = pDesc->iIndex;
		m_Desc.vScale = pDesc->vScale;
		m_Desc.vPosition = pDesc->vPosition;
		m_Desc.fAngle = pDesc->fAngle;
		m_Desc.iNaviMeshIndex = pDesc->iNaviMeshIndex;
		m_iNaviMeshIndex = pDesc->iNaviMeshIndex;
	}

	if (FAILED(__super::Initialize(pArg)))
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

void CMonster::Priority_Update(_float fTimeDelta)
{
}

_int CMonster::Update(_float fTimeDelta)
{

	

	m_pModelCom->Play_Animation(fTimeDelta, m_iChanging);

	for (auto& pColliderCom : m_pColliderCom)
		pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CMonster::Late_Update(_float fTimeDelta)
{
	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider")));

	for (auto& pCollider : m_pColliderCom)
		pCollider->Intersect(pTargetCollider);

	_vector		vWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	if (true == m_pGameInstance->isIn_Frustum_InWorldSpace(vWorldPos, 2.f))
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}
}

HRESULT CMonster::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	//for (auto& pCollider : m_pColliderCom)
	//	pCollider->Render();

	return S_OK;
}

void CMonster::Monster_Turn(_float fTimeDelta)
{
	_vector PlayerPosition;
	_vector SpawnPosition;

	if (CUI_Manager::GetInstance()->Get_Player()->Get_PlayerHide() == false)
	{
		PlayerPosition = m_pPlayer->Get_Position();
		m_pTransformCom->LookAt(PlayerPosition);
	}
	else if (CUI_Manager::GetInstance()->Get_Player()->Get_PlayerHide() == true)
	{
		SpawnPosition = m_matSpawnPos;
		m_pTransformCom->LookAt(m_matSpawnPos);
	}


	m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
}

void CMonster::Monster_Turn_Back(_float fTimeDelta)
{

	_vector PlayerPosition;
	_vector PlayerPosConverse;
	_vector SpawnPosition;

	if (CUI_Manager::GetInstance()->Get_Player()->Get_PlayerHide() == false)
	{
		PlayerPosition = m_pPlayer->Get_Position();
		m_pTransformCom->LookAt(PlayerPosition);
		m_bHidePlayer = true;
	}
	else if (CUI_Manager::GetInstance()->Get_Player()->Get_PlayerHide() == true)
	{
		if (m_bHidePlayer == false)
		{
			_vector InverseLook = -1 * (m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, InverseLook);
			m_bHidePlayer = true;
		}
	}

	Distance Player = { m_pPlayer->Get_Position().m128_f32[0], m_pPlayer->Get_Position().m128_f32[2] };
	Distance Monster = { m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2] };

	if (20 < sqrt(pow(Monster.x - Player.x, 2) + pow(Monster.z - Player.z, 2)))
	{
		m_pTransformCom->LookAt(m_matSpawnPos);
	}

	m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
}

_vector CMonster::ToLookDirection(const _vector& Rotation)
{
	float x = XMVectorGetX(Rotation);
	float y = XMVectorGetY(Rotation);
	float z = XMVectorGetZ(Rotation);
	float w = XMVectorGetW(Rotation);

	_vector LookDirection = XMVectorSet(2.0f * (x * z + w * y), 2.0f * (y * z - w * x), 1.0f - 2.0f * (x * x + y * y), 0.0f);

	return XMVector3Normalize(LookDirection);
}

void CMonster::LightPosition(_int index, _float Look, _float Height, CTransform* transform)
{
	_vector vecMonsterPos = transform->Get_State(CTransform::STATE_POSITION);
	_vector vecMonsterLook = transform->Get_State(CTransform::STATE_LOOK);

	vecMonsterLook = XMVector3Normalize(vecMonsterLook);

	_vector vecLightPos = vecMonsterPos + (vecMonsterLook * Look);

	LIGHT_DESC LightDesc{};

	LightDesc.vPosition = { XMVectorGetX(vecLightPos), XMVectorGetY(vecLightPos) + Height, XMVectorGetZ(vecLightPos), 1.f };

	_vector FinalPos = XMVectorSet(LightDesc.vPosition.x, LightDesc.vPosition.y, LightDesc.vPosition.z, LightDesc.vPosition.w);
	m_pGameInstance->Update_MonsterPos(index, FinalPos);
	
}

HRESULT CMonster::Ready_Components(COLLIDERDESC* pDesc)
{
	//CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};
	//
	//
	//ColliderSphereDesc.fRadius = pDesc->fRadius;
	//ColliderSphereDesc.vCenter = pDesc->vCenter;
	//
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
	//	TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLM_SPHERE]), &ColliderSphereDesc)))
	//	return E_FAIL;
	//
	//ColliderSphereDesc.fRadius = pDesc->fRadius + 200.f;
	//
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
	//	TEXT("Com_Collider_Sphere2"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLM_SPHERE2]), &ColliderSphereDesc)))
	//	return E_FAIL;
	//
	///* Com_Collider_OBB */
	//CBounding_OBB::BOUND_OBB_DESC		ColliderOBBDesc{};
	//
	//ColliderOBBDesc.vExtents = pDesc->vExtents;
	//ColliderOBBDesc.vCenter = pDesc->vCenter;
	//ColliderOBBDesc.vRotation = pDesc->vRotation;
	//
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
	//	TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLM_OBB]), &ColliderOBBDesc)))
	//	return E_FAIL;




	CNavigation::NAVIGATION_DESC		NaviDesc{};
	//NaviDesc.iStartIndex = m_iNaviMeshIndex;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonster::Bind_ShaderResources()
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

void CMonster::Set_Player(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
	Safe_AddRef(m_pPlayer);
}


CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster* pInstance = new CMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster01 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pPlayer);

	for (auto& pColliderCom : m_pColliderCom)
		Safe_Release(pColliderCom);
}

