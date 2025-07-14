#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* ���� �����带 �����Ѵ�.  */
/* ���� ������ �ʿ��� �ڿ��� �ε��ϴ� ��Ȱ */
BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(LEVELID eNextLevelID);
	HRESULT Loading();

#ifdef _DEBUG
public:
	void Show_Debug();
#endif

public:
	_bool isFinished() const {
		return m_isFinished;
	}

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	LEVELID					m_eNextLevelID = { LEVEL_END };
	HANDLE					m_hThread = {};
	CRITICAL_SECTION		m_Critical_Section = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };

private:
	_bool					m_isFinished = { false };
	_tchar					m_szLoadingText[MAX_PATH] = {};	

private:
	HRESULT Loading_Level_Logo();
	HRESULT Loading_Level_GamePlay();

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END