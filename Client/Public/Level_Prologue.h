#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Camera_POV.h"
#include "Player.h"
#include "UIObject.h"
#include "Inventory.h"
#include "InnerInventory.h"
#include "ChooseBox.h"
#include "Lighter.h"
#include "Sky.h"


BEGIN(Engine)
class CUIObject;
END

BEGIN(Client)

class CLevel_Prologue final : public CLevel
{
private:
	CLevel_Prologue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Prologue() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_GameObject(const _wstring& strLayerTag);
	HRESULT Ready_Lights();
	HRESULT Ready_UI(const _wstring& strLayerTag);
	HRESULT Ready_Player(const _wstring& strLayerTag);
	HRESULT Ready_Props(const _wstring& strLayerTag);
	HRESULT Ready_CollisionProps(const _wstring& strLayerTag);
	HRESULT Ready_LevelProps(const _wstring& strLayerTag);

private:

public:
	static CLevel_Prologue* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

private:
	CPlayer* m_pPlayer = { nullptr };
	CCamera_POV* m_pCamera_POV = { nullptr };
	CInventory* m_pInventory = { nullptr };
	CChooseBox* m_pChooseBox = { nullptr };
	CLighter* m_pLigher = { nullptr };


	/*For. SkyBox*/
	CSky* m_pSkyBox = { nullptr };
};

END