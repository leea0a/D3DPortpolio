#include "stdafx.h"

#include "../Public/Camera_POV.h"
#include "GameInstance.h"

CCamera_POV::CCamera_POV(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCamera { pDevice, pContext }
{
}

CCamera_POV::CCamera_POV(const CCamera_POV & Prototype)
	: CCamera{ Prototype }
{

}

HRESULT CCamera_POV::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_POV::Initialize(void* pArg)
{
	CAMERA_POV_DESC* pDesc = static_cast<CAMERA_POV_DESC*>(pArg);

	m_fMouseSensor = pDesc->fMouseSensor;
	//m_Position = pDesc->vPosition;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_f4Position = _float4(m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1], m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 1.f);

	m_pf4Position = &m_f4Position;

	return S_OK;
}

void CCamera_POV::Priority_Update(_float fTimeDelta)
{

	if (m_bFix)
	{
		Fix_Mouse();
		Free_Mouse(fTimeDelta);
	}
}

_int CCamera_POV::Update(_float fTimeDelta)
{
	m_f4Position = _float4(m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0],
		m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1],
		m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 1.f);




	// 카메라 위치 업데이트
	_vector vCameraPos;
	if (m_pPlayer->Get_PlayerSit() == false)
	{
		vCameraPos = XMVectorSetY(m_pPlayer->Get_Position(), XMVectorGetY(m_pPlayer->Get_Position()) + 5.f);
	
	}
		
	else
		vCameraPos = XMVectorSetY(m_pPlayer->Get_Position(), XMVectorGetY(m_pPlayer->Get_Position()) + 3.1f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);
	

	__super::Compute_PipeLineMatrices();




	// 백업 //
	//m_f4Position = _float4(m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0], 
	//	m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1], 
	//	m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2], 1.f);
	//
	//if (m_pPlayer->Get_PlayerSit() == false)
	//{
	//
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pPlayer->Get_Position(), XMVectorGetY(m_pPlayer->Get_Position()) + 5.f));
	//}
	//else
	//{
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pPlayer->Get_Position(), XMVectorGetY(m_pPlayer->Get_Position()) + 3.1f));
	//}
	//
	//__super::Compute_PipeLineMatrices();
	return OBJ_NOEVENT;
}

void CCamera_POV::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CCamera_POV::Render()
{
	return S_OK;
}

void CCamera_POV::Set_Position(_vector Position)
{
	//m_Position = Position;
}

void CCamera_POV::Set_Look(_vector Look)
{
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, Look);
}

void CCamera_POV::Set_Player(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
	Safe_AddRef(m_pPlayer);
}

void CCamera_POV::Fix_Mouse()
{
	POINT pt = { g_iWinSizeX >>1, (g_iWinSizeY >>1) };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x,pt.y);
}

void CCamera_POV::Free_Mouse(_float fTimeDelta)
{
	_long		MouseMove = {};
	if (MouseMove = GET_MOUSE_MOVE(MOUSE_MOVE::DIMS_X))
	{
		//if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) < -5)
		//{
			m_pTransformCom->Turn(XMVectorSet(0.f, -1 * m_pPlayer->Get_Position().m128_f32[1] + 0.5f, 0.f, 0.f), MouseMove * fTimeDelta * m_fMouseSensor);
		//}
		//else
		//	m_pTransformCom->Turn(XMVectorSet(0.f, m_pPlayer->Get_Position().m128_f32[1] + 0.5f, 0.f, 0.f), MouseMove * fTimeDelta * m_fMouseSensor);
	}
	if (MouseMove = GET_MOUSE_MOVE(MOUSE_MOVE::DIMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * fTimeDelta * m_fMouseSensor);
	}
}

CCamera_POV * CCamera_POV::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCamera_POV*	pInstance = new CCamera_POV(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_POV");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_POV::Clone(void * pArg)
{
	CCamera_POV*	pInstance = new CCamera_POV(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_POV");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CCamera_POV::Free()
{
	Safe_Release(m_pPlayer);
	__super::Free();
	
}
