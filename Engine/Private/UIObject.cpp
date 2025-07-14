#include "..\Public\UIObject.h"

CUIObject::CUIObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{
}

CUIObject::CUIObject(const CUIObject & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CUIObject::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUIObject::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	
	UIOBJECT_DESC*	pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fSizeX = pDesc->fSizeX;
	m_fSizeY = pDesc->fSizeY;
	/*m_fX = pDesc->fX;
	m_fY = pDesc->fY;*/
	m_fX = pDesc->fX + (pDesc->fSizeX * 0.5);
	m_fY = pDesc->fY + (pDesc->fSizeY * 0.5);
	m_eType = pDesc->eType;

	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);


	/* ����Ƣ���� ���� ���, �����ร�� �������. */
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* ������ fX, fY,  fSizeX, fSizeY�� �׷��� �� �ֵ��� ��������� ���¸� �����صд�. */
	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - ViewportDesc.Width * 0.5f, -m_fY + ViewportDesc.Height * 0.5f, 0.f, 1.f));


	return S_OK;
}

_bool CUIObject::Check_Is_Mouse_On(_float2 MousePos)
{
	_float2 LeftTop = { m_fX - m_fSizeX*0.5f , m_fY -m_fSizeY*0.5f};
	_float2 RightBottom = {m_fX+ m_fSizeX*0.5f , m_fY +m_fSizeY*0.5f };


	_bool bCondition = (LeftTop.x < MousePos.x && MousePos.x < RightBottom.x) && (LeftTop.y < MousePos.y && MousePos.y < RightBottom.y);
	return bCondition;
}

void CUIObject::Update_Event()
{
	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
	{
		if (i == (_uint)m_eUIState)
		{
			if (m_EventFunction)
			{
				m_EventFunction[(_uint)m_eUIState]();
			}
		}
	}
}



void CUIObject::Free()
{
	__super::Free();
}
