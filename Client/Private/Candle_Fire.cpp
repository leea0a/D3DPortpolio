#include "stdafx.h"
#include "Candle_Fire.h"
#include "GameInstance.h"

CCandle_Fire::CCandle_Fire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProp {pDevice, pContext}
{
}

CCandle_Fire::CCandle_Fire(const CCandle_Fire& Prototype)
	: CProp(Prototype)
{
}

HRESULT CCandle_Fire::Initialize_Prototype()
{
	return S_OK;
}
 

HRESULT CCandle_Fire::Initialize(void* pArg)
{
	PROP_DESC* pSetDesc = static_cast<PROP_DESC*>(pArg);

	PROP_DESC pDesc;



	pDesc.fSpeedPerSec = pSetDesc->fSpeedPerSec;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);
	pDesc.vPosition = pSetDesc->vPosition;
	pDesc.vScale = pSetDesc->vScale;
	pDesc.fAngle = 0.f;
	pDesc.iData = 10;
	pDesc.iPropNo = pSetDesc->iPropNo;
	m_pCandleFireOn_Off = pSetDesc->pbCandle;






	if (FAILED(__super::Initialize(&pDesc)))

		return E_FAIL;

	if (FAILED(Ready_Components(&pDesc)))
		return E_FAIL;


	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(0.86f, 0.39f, 0.08f, 1.f);
	LightDesc.vAmbient = _float4(0.86f, 0.39f, 0.08f, 1.f);
	LightDesc.vSpecular = _float4(0.86f, 0.39f, 0.08f, 1.f);
	LightDesc.fRange = _float(17.f);
	LightDesc.vPosition = { pDesc.vPosition.x, pDesc.vPosition.y , pDesc.vPosition.z, 1.f };
	LightDesc.TurnOn_Off = true;


	if(FAILED(m_pGameInstance->Add_CandleLight(LightDesc, pDesc.iPropNo)))
		return E_FAIL;

	CSoundMgr::GetInstance()->StartEffectPlay(SFX_TYPE::SFX_FIREON, false);


	Set_PropType(TYPE_CANDLE);

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 0.45f, -1.f, 1.f));
	//m_pTransformCom->Scaling(0.03f, 0.03f, 0.03f);

	return S_OK;
}

void CCandle_Fire::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

_int CCandle_Fire::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
	
}

void CCandle_Fire::Late_Update(_float fTimeDelta)
{
	if (*m_pCandleFireOn_Off == true )
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CCandle_Fire::Render()
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

	return S_OK;
}

void CCandle_Fire::Set_InterAction(_float fTimeDelta)
{
	if (false == m_bCabinetIn)
		return;


	if (false == m_bHide && true == m_bCabinetIn)
	{
		CUI_Manager::GetInstance()->Get_Player()->Set_PrePos(CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_POSITION));
		CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		m_bHide = true;
		CUI_Manager::GetInstance()->Get_Player()->Set_PlayerHide(m_bHide);
		
		m_bChooseObj = false;
	}
	else if (true == m_bHide)
	{
		CUI_Manager::GetInstance()->Get_Player()->Get_Transform()->Set_State(CTransform::STATE_POSITION, CUI_Manager::GetInstance()->Get_Player()->Get_PrePos());
		m_bHide = false;

		CUI_Manager::GetInstance()->Get_Player()->Set_PlayerHide(m_bHide);
		m_bCabinetIn = false;
		m_bChooseObj = false;
	}
}

HRESULT CCandle_Fire::Ready_Components(void* pArg)
{


	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Candle_Fire"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCandle_Fire::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CCandle_Fire* CCandle_Fire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCandle_Fire* pInstance = new CCandle_Fire(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CCandle_Fire Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCandle_Fire::Clone(void* pArg)
{
	CCandle_Fire* pInstance = new CCandle_Fire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CCandle_Fire Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCandle_Fire::Free()
{
	__super::Free();
	for (auto& pColliderCom : m_pColliderDoor)
		Safe_Release(pColliderCom);

	Safe_Release(m_pCameraPOV);
}

