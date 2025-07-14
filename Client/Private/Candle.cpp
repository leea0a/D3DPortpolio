#include "stdafx.h"
#include "Candle.h"
#include "GameInstance.h"

CCandle::CCandle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CCandle::CCandle(const CCandle& Prototype)
	: CProp(Prototype)
{
}

HRESULT CCandle::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CCandle::Initialize(void* pArg)
{
	PROP_DESC* pSetDesc = static_cast<PROP_DESC*>(pArg);

	PROP_DESC pDesc;

	pDesc.fSpeedPerSec = pSetDesc->fSpeedPerSec;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = pSetDesc->vPosition;
	pDesc.vScale = { 0.03f , 0.03f, 0.03f };
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = pSetDesc->iPropNo;
	m_iCandleNo = pSetDesc->iNaviIndex;
	pDesc.pbCandle = m_pCandleFire;

	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	Set_PropType(TYPE_CANDLE);

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 0.45f, -1.f, 1.f));
	//m_pTransformCom->Scaling(0.03f, 0.03f, 0.03f);

	return S_OK;
}

void CCandle::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CCandle::Update(_float fTimeDelta)
{

	if (m_bChooseObj == true)
		Set_InterAction(fTimeDelta);

	m_pCandleFire = &m_bCandleFire;

	return OBJ_NOEVENT;
}

void CCandle::Late_Update(_float fTimeDelta)
{
	CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));

	if (m_bChooseObj == false)
	{
		for (auto& pCollider : m_pColliderDoor)
		{
			

			if (true == m_pColliderDoor[COLLIDER_AABB1]->Intersect(pTargetCollider))
			{

				CUI_Manager::GetInstance()->Set_BumpType(CUI_Manager::BUMP_CANDLE);
				m_vecPreInsidePosition = CUI_Manager::GetInstance()->Get_Player()->Get_Position();

				if (KEY_UP(KEY::F))
				{
					m_bChooseObj = true;
					InterAction(fTimeDelta);
				}
			}
		}
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}
	else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);



	for (auto& pCollider : m_pColliderDoor)
		pCollider->Update(m_pTransformCom->Get_WorldMatrix());

}

HRESULT CCandle::Render()
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
	
	//for (auto& pCollider : m_pColliderDoor)
	//	pCollider->Render();

	return S_OK;
}

void CCandle::Set_InterAction(_float fTimeDelta)
{
	if (m_bCandleMake == true && m_bCandleFire == true)
	{
		m_bCandleFire = false;
		m_bChooseObj = false;

		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_FIREOFF, false);
		m_pGameInstance->Delete_CandleLight(m_iCandleNo);	
	}
	else if (m_bCandleMake == true && m_bCandleFire == false)
	{
		m_bCandleFire = true;
		m_bChooseObj = false;



		LIGHT_DESC			LightDesc{};

		ZeroMemory(&LightDesc, sizeof LightDesc);

		LightDesc.eType = LIGHT_DESC::TYPE_POINT;
		LightDesc.vDiffuse = _float4(0.86f, 0.39f, 0.08f, 1.f);
		LightDesc.vAmbient = _float4(0.86f, 0.39f, 0.08f, 1.f);
		LightDesc.vSpecular = _float4(0.86f, 0.39f, 0.08f, 1.f);
		LightDesc.fRange = _float(17.f);
		LightDesc.vPosition = { m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1] + 4.1f , m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 1.f};
		LightDesc.TurnOn_Off = true;

		m_pGameInstance->Add_CandleLight(LightDesc, m_iCandleNo);
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_FIREON, false);

	}
	else if (m_bChooseObj == true && m_bCandleMake == false)
	{

	
	const _wstring strLayerTag = TEXT("Layer_CandleLight");

	PROP_DESC CandleDesc;

	_float4 CandlePos{};
	CandlePos.x = m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0];
	CandlePos.y = m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1] + 4.1f;
	CandlePos.z = m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2];
	CandlePos.w = 1.f;

	CandleDesc.vScale = { 0.03f, 0.03f, 0.03f };
	CandleDesc.fSpeedPerSec = 1.f;
	CandleDesc.fRotationPerSec = XMConvertToRadians(90.f);
	CandleDesc.vPosition = CandlePos;

	CandleDesc.fAngle = 0.f;
	CandleDesc.iPropNo = m_iCandleNo;
	CandleDesc.iData = 10;
	CandleDesc.pbCandle = m_pCandleFire;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Candle_Fire"), LEVEL_GAMEPLAY, strLayerTag, &CandleDesc)))
		return;




	//LIGHT_DESC			LightDesc{};
	//
	//ZeroMemory(&LightDesc, sizeof LightDesc);
	//
	//LightDesc.eType = LIGHT_DESC::TYPE_DIRECTOINAL;
	//LightDesc.vDirection = _float4(-1.f, -1.f, -1.f, 0.f);
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	//
	//m_pGameInstance->Add_CandleLight(LightDesc, CandleDesc.iPropNo);



	m_bCandleMake = true;
	m_bCandleFire = true;
	m_bChooseObj = false;
	m_bCandleOn = false;

	return;
	}
}



HRESULT CCandle::Ready_Components(void* pArg)
{
	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);


	CBounding_AABB::BOUND_AABB_DESC ColliderAABB{};

	ColliderAABB.vCenter = { 0.f, 100.f, 0.f };
	ColliderAABB.vExtents = { 10.f, 100.f, 10.f };
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB0]), &ColliderAABB)))
		return E_FAIL;

	ColliderAABB.vExtents = { 20.f, 100.f, 20.f };
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider1"), reinterpret_cast<CComponent**>(&m_pColliderDoor[COLLIDER_AABB1]), &ColliderAABB)))
		return E_FAIL;



	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Candle"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCandle::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CCandle* CCandle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCandle* pInstance = new CCandle(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CCandle Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCandle::Clone(void* pArg)
{
	CCandle* pInstance = new CCandle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CCandle Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCandle::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderDoor)
		Safe_Release(pColliderCom);

	Safe_Release(m_pCameraPOV);
}

