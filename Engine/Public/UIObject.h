#pragma once

#include "GameObject.h"

/* ũ���̾�Ʈ���� ���� �����̿� ��ü�帣�� �θ� �Ǵ� Ŭ������.*/
/* �������ѿ�9���� ���� �����Ϳ� ����� ������ ���� ����. */
enum class UI_STATE
{
	PRESSING,
	AWAY,
	DOWN,
	UP,
	ON,
	NONE,
	STATE_END
};

BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	typedef struct : CGameObject::GAMEOBJECT_DESC
	{
		_float		fX, fY, fSizeX, fSizeY;
		UI_TYPE		eType;
	}UIOBJECT_DESC;
protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	void Set_Active() { m_bActive = true; };
	void Set_InActive() { m_bActive = false; };
	_bool Get_Active() { return m_bActive; };
	
	void Set_SortDepth(_uint iIndex) { m_iDepth = iIndex; };
	_uint Get_SortDepth() { return m_iDepth; };

	UI_TYPE Get_Type() { return m_eType; };
	void Set_Type(UI_TYPE _eType) {	m_eType = _eType;  };

	void Set_PositionX(_float PosX) { 
		m_fX = PosX;
	}
	void Set_PositionY(_float PosY) { m_fY = PosY; }
	
	///*for Checking Button state */
	//void				Set_Is_Clicked() { m_IsClicked = true; }
	//void				Set_Is_Not_Clicked() { m_IsClicked = false;}
	//_bool				Get_Is_Clicked() { return m_IsClicked; }
	///* for Cheking if Mouse Is On UI*/
	_bool				Check_Is_Mouse_On(_float2 MousePos);
	//void				Set_Is_OnMouse() { m_IsOnMouse = true; };
	//void				Set_Is_Not_OnMouse() { m_IsOnMouse = false; }
	//_bool				Get_IsnMouse() { return m_IsOnMouse; };

	/* For Checking wheather It has the last order*/
	void				Set_Is_Selected() { m_IsSelected = true; };
	void				Set_Is_Not_Selected() { m_IsSelected = false; }
	_bool				Get_Is_Selected() { return m_IsSelected; };

	/* update Event depending on state*/
	void				Set_UIState(UI_STATE eState) { m_eUIState = eState; }//���콺 ��ü���� UI �Ŵ����� �����ؼ�  ���콺 �����ȿ� �ִ� UI�� ���� ���߿� �����ϴ� UI�� �����´�.
	void				Update_Event();
	/*�θ� : KEY_DOWN: �켱 ���� �� ���� PRESSING �̵�: ���� SELECTED(���� ���콺 �������� �ִ� UI�� �� �켱 ����)*/

protected:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_ViewMatrix{}, m_ProjMatrix{};

	_uint					m_iDepth = {};
	_bool					m_bActive = { true };
	UI_TYPE					m_eType = { UI_TYPE::UI_END };


	bool					m_IsClicked = { false };
	bool					m_IsOnMouse = { false };
	bool					m_IsSelected = { false };
	std::function<void()>   m_EventFunction[(_uint)UI_STATE::STATE_END] = {nullptr};
	//�ڽĿ��� �� ���콺 ���¿� ���� �ڽ��� ��� �Լ��� ���ε����ش� ( m_EventFunction =std::bind(&Derived::MemberFunc,this(�ڽ� ������), �Ű� ���� ......)
	UI_STATE				m_eUIState = { UI_STATE::STATE_END };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END