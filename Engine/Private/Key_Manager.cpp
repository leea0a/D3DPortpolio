#include "Key_Manager.h"

unsigned char g_arrKeyMapping[(_uint)KEY::KEY_END] =
{
	DIK_LEFT,
	DIK_RIGHT,
	DIK_UP,
	DIK_DOWN,
	DIK_Q,
	DIK_W,
	DIK_E,
	DIK_R,
	DIK_T,
	DIK_Y,
	DIK_U,
	DIK_I,
	DIK_O,
	DIK_P,
	DIK_A,
	DIK_S,
	DIK_D,
	DIK_F,
	DIK_G,
	DIK_H,
	DIK_J,
	DIK_K,
	DIK_L,
	DIK_Z,
	DIK_X,
	DIK_C,
	DIK_V,
	DIK_B,
	DIK_N,
	DIK_M,
	DIK_SPACE,
	DIK_RETURN,
	DIK_TAB,
	DIK_LSHIFT,
	DIK_ESCAPE,
	DIK_1,
	DIK_2,
	DIK_3,
	DIK_4,
	DIK_5,
	DIK_6,
	DIK_7,
	DIK_8,
	DIK_9,
	DIK_0,
};

CKey_Manager::CKey_Manager(void)
{
}

HRESULT CKey_Manager::Initialize_DInputDev(HINSTANCE hInst, HWND hWnd)
{

	if(FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION, //�ֽ� ������ ����ϰڴٴ� �ǹ�
		IID_IDirectInput8,   //IDirectInput8.0�������̽��� ���
		(void**)&m_pDirectInput,//LPDIRECTINPUT8 �� ��ü �����͸� ����
		NULL)))
		return E_FAIL;

	// Ű���� ��ü ����
	if (FAILED(m_pDirectInput->CreateDevice(
		GUID_SysKeyboard,  //globally uniqued identifier   :  �����Ϸ��� ��ġ�� guid�� Enum_Device�Լ��� ȣ���� �Է� ��ġ�夷�� guid�� ������ ����
		&m_pKeyboard,     //��ġ �������̽� ������ (�İ�ü�� ����Ű�� ������)
		nullptr)))
		return E_FAIL;

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pKeyboard->SetDataFormat(&c_dfDIKeyboard); //������ ������ ������(�̸� �����ǵ� ������ ������ ���� ( ��� ���� ) (Ű���� ���콺 ���̽�ƽ�� ���� ������ ���� ���� 

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	//������ �ڵ�� �÷��� flag-> ������ Ȱ��ȭ�̰ų� ��Ȱ���̰ų� ��� �Է� ��ġ�� ���  and(���� ���μ����� �Է����϶� �ٸ� ���μ����� �������� ���� (�޸��� �Է� �� )

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pKeyboard->Acquire();
	//�Է���ġ�� ȹ���ϴ� �Լ� : �Է���ġ�� ���� ������� ������, �����͸� �޾ƿ��� �Լ� unrequired �Լ��� �ݵ�� ȣ���ؾ���


	// ���콺 ��ü ����
	if(FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pMouse->Acquire();

	for (size_t i = 0; i < (_uint)KEY::KEY_END; ++i)
	{
		
		m_vecKeyState.push_back(tKeyInfo{KEY_STATE::NONE, false});
	}
	for (size_t i = 0; i < (_uint)MOUSE::DIM_END; ++i)
	{
		
		m_vecMouseState.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
	


	return S_OK;
}

void CKey_Manager::Update_KeyManager(void)
{
	m_pKeyboard->GetDeviceState(256, m_KeyboardState);// Ű���� �Է»��¸� �˷��ش�(����)
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState); //���콺 �Է� ���¸� �˷��ش�(����)

	//keyboard
	for (int i = 0; i < (int)KEY::KEY_END; ++i)
	{
		//���� Ű�� �����ִ� ����
		if (m_KeyboardState[g_arrKeyMapping[i]] & 0x80)
		{
			//������ Ű�� ���ȴ� ���
			if (true == m_vecKeyState[i].PrevPressed)
			{
				m_vecKeyState[i].eState = KEY_STATE::PRESSING;
			}
			//������ Ű�� �������� ���Ҵ� ���
			else
			{
				m_vecKeyState[i].eState = KEY_STATE::DOWN;
			}

			m_vecKeyState[i].PrevPressed = true;
		}
		else
		{
			//������ Ű�� ���ȴ� ���
			if (true == m_vecKeyState[i].PrevPressed)
			{
				m_vecKeyState[i].eState = KEY_STATE::UP;
			}
			//������ Ű�� �������� ���Ҵ� ���
			else
			{
				m_vecKeyState[i].eState = KEY_STATE::NONE;
			}

			m_vecKeyState[i].PrevPressed = false;

		}
	}
	//Mouse

	for (int i = 0; i < (_uint)MOUSE::DIM_END; ++i)
	{
		//���� ���콺�� ��ư�� ���ȴ�
		if (m_MouseState.rgbButtons[i])
		{
			//������ ���콺�� ���ȴ� 
			if (true == m_vecMouseState[i].PrevPressed)
			{
				m_vecMouseState[i].eState = KEY_STATE::PRESSING;
			}
			//������ ���콺�������� ����.
			else
			{
				m_vecMouseState[i].eState = KEY_STATE::DOWN;
			}
			m_vecMouseState[i].PrevPressed = true;
		}
		//���� ���콺�� ��ư�� ������ �ʾҴ�
		else
		{
			if (true == m_vecMouseState[i].PrevPressed)
			{
				m_vecMouseState[i].eState = KEY_STATE::UP;
			}
			else
			{
				m_vecMouseState[i].eState = KEY_STATE::NONE;
			}
			m_vecMouseState[i].PrevPressed = false;
		}

	}

 
 }




CKey_Manager* CKey_Manager::Create(HINSTANCE hInst, HWND hWnd)
{
	CKey_Manager* pInstance = new CKey_Manager();
	if (FAILED(pInstance->Initialize_DInputDev(hInst, hWnd)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create: Failed CKey_Manager");
		return nullptr;
	}
	return pInstance;
}

void CKey_Manager::Free(void)
{
	if (m_pKeyboard) {
		m_pKeyboard->Unacquire();
		Safe_Release(m_pKeyboard);
		m_pKeyboard = nullptr;
	}
	if (m_pMouse) {
		m_pMouse->Unacquire();
		Safe_Release(m_pMouse);
		m_pMouse =nullptr;
	}
	Safe_Release(m_pDirectInput);

	__super::Free();
}
