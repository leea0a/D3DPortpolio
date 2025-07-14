#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Camera_POV.h"
#include "Player.h"
#include "UIObject.h"
#include "Inventory.h"
#include "InnerInventory.h"
#include "ChooseBox.h"
#include "MiddlePoint.h"
#include "CryingGirl.h"
#include "Spider.h"
#include "MenMonster.h"

#include "Lighter.h"
#include "Sky.h"

const int M_SEMI_STAGE = 6;
const int M_PROP_TYPES = 10;

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;	
private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_ShadowCamera();
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_GameObject(const _wstring& strLayerTag);
	HRESULT Ready_Lights();
	HRESULT Ready_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Player(const _wstring& strLayerTag);
	HRESULT Ready_UI(const _wstring& strLayerTag);
	HRESULT Ready_Props(const _wstring& strLayerTag);
	HRESULT Ready_CollisionProps(const _wstring& strLayerTag, CProp::PROP_DESC* _Desc, _int _Index);
	HRESULT Ready_CabinetProps(const _wstring& strLayerTag, CProp::PROP_DESC* _Desc);
	HRESULT Ready_CandleProps(const _wstring& strLayerTag, CProp::PROP_DESC* _Desc);
	HRESULT Ready_UseProps(const _wstring& strLayerTag);
	HRESULT Ready_EndProp(const _wstring& strLayerTag);

	HRESULT Update_For_ChromaticShader(_float fTimeDelta);



private:
	HRESULT	Set_InventoryUI(const _wstring& strLayerTag);

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

private:
	//CImguiWindow* m_pImguiMgr = { nullptr };

	//for Terrain_Setting

	_uint		   m_VerticesX = { 0 };
	_uint		   m_VerticesZ = { 0 };

	/* for Manageming Cube Created*/
	vector<class CCubeCollider*>	m_vecCubeCollider;
	_float3							m_vCollider_Scale = {1.f,1.f,1.f};
	_float4							m_vCollider_RotAxis = {0.f,1.f,0.f,0.f};
	_float							m_vCollider_Rotation = { 0.f };
	_float4							m_vCollider_Position = {0.f,0.f,0.f,1.f};
	_int							m_iCubeColliderNowIndex = {0};
	_bool							m_bCreatingCubeMode = { false };
	
	class CTerrain*					m_pTerrain = { nullptr };
	const _float4*					m_pPos = { nullptr };
	CPlayer*						m_pPlayer = { nullptr };
	CCamera_POV*					m_pCamera_POV = { nullptr };
	CInventory*						m_pInventory = { nullptr };
	CChooseBox*						m_pChooseBox = { nullptr };
	CLighter*						m_pLigher = { nullptr };
	CMiddlePoint*					m_pMiddlePoint = { nullptr };

	/*For.Monster*/
	CryingGirl*						m_pCryingGirl = { nullptr };
	CSpider*						m_pSpider = { nullptr };
	CMonster*						m_pMenMonster = { nullptr };

	/*For. SkyBox*/
	CSky*							m_pSkyBox = { nullptr };
	
	/*Chromatic Shader 상태*/
	_float					m_fChromaticTime = { 0.0f }; // 크로마틱 쉐이더 진행 시간
	_float					m_fChromaticCoolTime = { 0.0f }; // 크로마틱 쉐이더 쿨타임 진행 시간

private:
	HRESULT Read_Props_Desc_From_File();
	HRESULT Ready_Prop(const _wstring& strLayerTag);


	vector<CProp*>m_vecProps[M_SEMI_STAGE + 1];
	
	vector<CProp::PROP_DESC>m_vecPropDescs[M_SEMI_STAGE + 1][M_PROP_TYPES + 1];
	
	_uint m_iNowStage = { 0 };
	
	_bool m_bCanGo_NextStage = { false };
	
	_tchar m_DescPathes[M_SEMI_STAGE + 1][MAX_PATH] = {
		L"",
		L"../Bin/Resources/Data/Client_Props_Stage01_Cabinet.dat",
		L"../Bin/Resources/Data/Client_Props_Stage01_Door_01.dat",
		L"../Bin/Resources/Data/Client_Props_Stage01_Door_02.dat",
		L"../Bin/Resources/Data/Client_Props_Stage01_Candle.dat",
		L"../Bin/Resources/Data/Client_Props_Stage01_Item_01.dat",
		L"../Bin/Resources/Data/Client_Props_Stage01_Item_02.dat"
	};


};

END