#include "stdafx.h"
#include "LightStone.h"
#include "GameInstance.h"

CLightStone::CLightStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CLightStone::CLightStone(const CLightStone& Prototype)
	: CProp(Prototype)
{
}

HRESULT CLightStone::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CLightStone::Initialize(void* pArg)
{
	PROP_DESC* pSetDesc = static_cast<PROP_DESC*>(pArg);

	PROP_DESC pDesc;



	pDesc.fSpeedPerSec = pSetDesc->fSpeedPerSec;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.vScale = pSetDesc->vScale;
	pDesc.fSpeedPerSec = 0.8f;

	m_f4OriginPos = pSetDesc->vPosition;

	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;

	_vector CameraPos = CUI_Manager::GetInstance()->Get_Camera()->Get_Position();
	_vector Look = CUI_Manager::GetInstance()->Get_Camera()->Get_Look();
	_vector Right = CUI_Manager::GetInstance()->Get_Camera()->Get_Right();
	_vector Up = CUI_Manager::GetInstance()->Get_Camera()->Get_Up();
	_vector offset = Right - Up * 0.14f + Look * 0.1f;
	_vector NewPos = CameraPos + offset;





	_vector Pos = XMVectorSet(m_f4OriginPos.x, m_f4OriginPos.y, m_f4OriginPos.z, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, NewPos);
	m_pTransformCom->Scaling( 0.05f, 0.05f, 0.05f );


	m_iLightStoneIndex = CUI_Manager::GetInstance()->Get_StoneLightIndex();

	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(0.396f, 0.835f, 0.858f, 1.f);
	LightDesc.vAmbient = _float4(0.396f, 0.835f, 0.858f, 1.f);
	LightDesc.vSpecular = _float4(0.396f, 0.835f, 0.858f, 1.f);
	LightDesc.fRange = _float(5.f);
	LightDesc.vPosition = { pDesc.vPosition.x, pDesc.vPosition.y , pDesc.vPosition.z, 1.f };
	LightDesc.TurnOn_Off = true;


	if (FAILED(m_pGameInstance->Add_StoneLight(LightDesc, m_iLightStoneIndex)))
		return E_FAIL;

	CUI_Manager::GetInstance()->Set_StoneLightIndex(CUI_Manager::GetInstance()->Get_StoneLightIndex() + 1);



	Set_PropType(TYPE_LIGHTSTONE);

	return S_OK;
}

void CLightStone::Priority_Update(_float fTimeDelta)
{
	if (m_bLife == true)
	{
		return;
	}
	else 
		__super::Priority_Update(fTimeDelta);
}

_int CLightStone::Update(_float fTimeDelta)
{
	if (m_bLife == true)
	{
		return OBJ_DEAD;
	}
	else if (m_bLife == false)
	{
		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) >= -6.5f)
		{
			Drop(fTimeDelta);
			m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

			if (m_pGameInstance->Get_StoneLightCount() >= 1)
			{
				m_pGameInstance->Update_StoneLight(m_iLightStoneIndex, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			}
		}


		CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider_Player2")));

		if (m_bChooseObj == false)
		{
			_bool bInterSect = m_pColliderCom->Intersect(pTargetCollider);


			if (bInterSect == true)
			{
				_float4 Pos = { m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 0.f };

				if (KEY_DOWN(KEY::F))
				{
					m_bChooseObj = true;

					InterAction(fTimeDelta);
				}
			}
			m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
		}
		else if (m_bChooseObj == true && m_ePropType == TYPE_CANDLE || TYPE_CABINET || TYPE_BIGBOX || TYPE_DOOR)
			m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	}

	
	return OBJ_NOEVENT;
}

void CLightStone::Late_Update(_float fTimeDelta)
{
	if (m_bLife == true)
	{
		m_pGameInstance->StoneLight_TurnOnOff(m_iLightStoneIndex, false);
		return;
	}
	else if (m_bLife == false)
		__super::Late_Update(fTimeDelta);
}

HRESULT CLightStone::Render()
{
	if (m_bLife == true)
	{
		return S_OK;
	}

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

		//m_pColliderCom->Render();
	}

	return S_OK;
}

HRESULT CLightStone::Ready_Components(void* pArg)
{
	PROP_DESC* pDesc = static_cast<PROP_DESC*>(pArg);

	CBounding_Sphere::BOUND_SPHERE_DESC		ColliderSphereDesc{};
	ColliderSphereDesc.fRadius = 10.f;
	ColliderSphereDesc.vCenter = _float3(pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z);

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderSphereDesc)))
		return E_FAIL;
	 
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LightStone"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLightStone::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

void CLightStone::Drop(_float fTimeDelta)
{
	const _float fGravity = 9.8f;
	_float Speed = m_pTransformCom->Get_Speed();
	_float fAngle = XMConvertToRadians(45.f);


	m_fTime += fTimeDelta;

	m_fCalPos.x = m_f4OriginPos.x - Speed * cos(fAngle) * m_fTime;
	m_fCalPos.y = m_f4OriginPos.y - Speed * sin(fAngle) * m_fTime - 0.5f * fGravity * m_fTime * m_fTime;
	m_fCalPos.z = m_f4OriginPos.z - Speed * m_fTime;

	if (m_fCalPos.y <= -6.3f && m_bPutSound == false)
	{
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

		CSoundMgr::GetInstance()->Set_EffectVolume(SoundSize, SFX_TYPE::SFX_LIGHTSTONEPUT);
		CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_LIGHTSTONEPUT, false);

		m_bPutSound = true;
	}

	if (m_fCalPos.y <= -6.5f)
	{
		m_fCalPos.y = -6.5f;
		
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fCalPos.x, m_fCalPos.y, m_fCalPos.z, 1.f));
}

void CLightStone::Delete_Light()
{
	if (m_pGameInstance->Get_StoneLightCount() >= 1)
		m_pGameInstance->Delete_SToneLight(m_iLightStoneIndex);

}

CLightStone* CLightStone::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLightStone* pInstance = new CLightStone(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CLightStone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CLightStone::Clone(void* pArg)
{
	CLightStone* pInstance = new CLightStone(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CLightStone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLightStone::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
}

