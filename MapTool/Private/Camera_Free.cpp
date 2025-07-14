#include "stdafx.h"
#include "..\Public\Camera_Free.h"

#include "GameInstance.h"

CCamera_Free::CCamera_Free(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCamera { pDevice, pContext }
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & Prototype)
	: CCamera{ Prototype }
{

}

HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void * pArg)
{
	CAMERA_FREE_DESC*		pDesc = static_cast<CAMERA_FREE_DESC*>(pArg);

	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{
	if (KEY_PRESSING(KEY::W))
	{
		m_pTransformCom->Go_Straight_for_Camera(fTimeDelta);
	}
	
	if (KEY_PRESSING(KEY::S))
	{
		m_pTransformCom->Go_Backward_for_Camera(fTimeDelta);
	}
	
	if (KEY_PRESSING(KEY::A))
	{
		m_pTransformCom->Go_Left_for_Camera(fTimeDelta);
	}
	
	if (KEY_PRESSING(KEY::D))
	{
		m_pTransformCom->Go_Right_for_Camera(fTimeDelta);
	}
	if (KEY_DOWN(KEY::SHIFT)) 
	{
		m_bFix = (m_bFix == false) ? true : false;
	}
	if (m_bFix)
	{
		Fix_Mouse();
		Free_Mouse(fTimeDelta);
	}
	
	__super::Compute_PipeLineMatrices();

}

_int CCamera_Free::Update(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CCamera_Free::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

void CCamera_Free::Fix_Mouse()
{
	POINT pt = { g_iWinSizeX >>1, (g_iWinSizeY >>1) };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x,pt.y);
}

void CCamera_Free::Free_Mouse(_float fTimeDelta)
{
	_long		MouseMove = {};
	if (MouseMove = GET_MOUSE_MOVE(MOUSE_MOVE::DIMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * fTimeDelta * m_fMouseSensor);
	}

	if (MouseMove = GET_MOUSE_MOVE(MOUSE_MOVE::DIMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * fTimeDelta * m_fMouseSensor);
	}

}

CCamera_Free * CCamera_Free::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCamera_Free*	pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Free::Clone(void * pArg)
{
	CCamera_Free*	pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CCamera_Free::Free()
{
	__super::Free();

}
