#pragma once

#include "Base.h"


/* 1. ���� ȭ�鿡 ��������� ������ü�� ��� �ִ´�. */
/* 2. ���ο� ������ ��ü�ϰ� ������������ �����Ѵ�. */
/* 2_1. ���� ������ �ڿ�(���ҽ�, ���ӿ�����Ʈ)���� �����Ѵ�. */
/* 3. Ȱ��ȭ�� ������ �ݺ����� ������Ʈ�� �����Ѵ�. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Open_Level(_int iLevelIndex, class CLevel* pNewLevel);
	void Update(_float fTimeDelta);
	HRESULT Render();
	_int	Get_CurrnetLevelID() { 
		return m_iCurrentLevelID; 
	}
	_int	Get_PreLevelID() { 
		return m_iPreLevelID; 
	}


private:
	_int					m_iCurrentLevelID = { -1 };
	_int					m_iPreLevelID = { -1 };
	class CLevel*			m_pCurrentLevel = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

END