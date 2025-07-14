#pragma once

#include "GameObject.h"

/* 크랄이언트에서 만든 유아이용 객체드르이 부모가 되는 클래스다.*/
/* 직교투ㅡ여9ㅇ을 위한 데이터와 기능을 가지고 있을 꺼야. */
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
	void				Set_UIState(UI_STATE eState) { m_eUIState = eState; }//마우스 객체에서 UI 매니저로 접근해서  마우스 범위안에 있는 UI중 가장 나중에 존재하는 UI를 가져온다.
	void				Update_Event();
	/*부모 : KEY_DOWN: 우선 순위 맨 위로 PRESSING 이동: 조건 SELECTED(현재 마우스 범위내에 있는 UI중 최 우선 순위)*/

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
	//자식에서 각 마우스 상태에 대한 자신의 멤버 함수를 바인딩해준다 ( m_EventFunction =std::bind(&Derived::MemberFunc,this(자식 포인터), 매개 변수 ......)
	UI_STATE				m_eUIState = { UI_STATE::STATE_END };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END