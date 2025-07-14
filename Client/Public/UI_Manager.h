#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Player.h"
#include "GameInstance.h"
#include "Camera_POV.h"



class CUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

public:
	enum BUMP {BUMP_PROP, BUMP_CANDLE, BUMP_CABINET, BUMP_DOOR, BUMP_ENDPROP, BUMP_STOP, BUMP_LEVEL, BUMP_EMPTY, BUMP_END };

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

public:
	void							Update(_float fTimeDelta);

public:
	_int							Get_ChooseIndex()	{ return m_iChooseIndex; }
	vector<class CInnerInventory*>	Get_Inventory()		{ return m_vecInventory; }
	CPlayer*						Get_Player()		{ return m_pPlayer; }
	void							Delete_Player();

public:
	void							Set_ChooseIndex(_int _ChooseIndex) { m_iChooseIndex = _ChooseIndex; }
	void							Set_InventoryObj(class CInnerInventory* InventoryObj);
	void							Delete_Inventory();
	void							Set_Player(CPlayer* _Player) { m_pPlayer = _Player; Safe_AddRef(m_pPlayer); }
	void							Set_NextLevelGo(_bool GOGO) { m_bNextLevelGo = GOGO; }
	_bool							Get_NextLevelGo() { return m_bNextLevelGo; }
	void							Set_NextLevelLevelOpen(_bool Open) { m_bNextLevelGo = Open; }
	_bool							Get_NextLevelLevelOpen() { return m_bNextLevelOpen; }
	void							Set_MirrorUse(_bool _Use) { 
		m_MirrorUse = _Use; 
	}
	_bool							Get_MirrorUse() { 
		return m_MirrorUse; 
	}
	void							Set_CompassUse(_bool _Use) { m_CompassUse = _Use; }
	_bool							Get_CompassUse() { return m_CompassUse; }
	void							Set_KaguraUse(_bool _Use) { m_KaguraUse = _Use; }
	_bool							Get_KaguraUse() { return m_KaguraUse; }
	void							Set_GlowStoneUse(_bool _Use) { m_GlowStoneUse = _Use; }
	_bool							Get_GlowStoneUse() { return m_GlowStoneUse; }	
	void							Set_SpherUse(_bool _Use) { m_bSphereUse = _Use; }
	_bool							Get_SpherUse() { return m_bSphereUse; }
	void							Set_DynamiteUse(_bool _Use) { m_bDynamiteUse = _Use; }
	_bool							Get_DynamiteUse() { return m_bDynamiteUse; }
	void							Set_Camera(CCamera_POV* _Camera) { m_pCamera = _Camera; Safe_AddRef(m_pCamera); }
	CCamera_POV*					Get_Camera() { return m_pCamera; }
	void							Delete_Camera() { Safe_Release(m_pCamera); m_pCamera = nullptr; }
	void							Set_HandLightTime(_float Time) { m_fHandLightTime = Time; }
	_float							Get_HandLightTime() { return m_fHandLightTime; }
	void							Set_SphereTime(_float Time) { m_fSphereTime = Time; }
	_float							Get_SphereTime() { return m_fSphereTime; }
	void							Set_ChooseBox(class CChooseBox* pChooseBox) { m_pChooseBox = pChooseBox; }
	_bool							Get_FirstDynamite() { return m_bFistDynamite; }
	void							Set_FirstDynamite(_bool _First) { m_bFistDynamite = _First; }
	_bool							Get_FirstMade() { return m_bFirstMade; }
	void							Set_FirstMade(_bool _First) { m_bFirstMade = _First; }
	_int							Get_StoneLightIndex() { return m_iStoneLightIndex; }
	void							Set_StoneLightIndex(_uint _StoneLightIndex) { m_iStoneLightIndex = _StoneLightIndex; }
	_uint							Get_DynamiteCount() { return m_iDynamiteCount; }
	void							Set_DynamiteCount(_uint Count) { m_iDynamiteCount = Count; }
	_bool							Get_FinalEnd() { return m_bFinalEnd; }
	void							Set_FinalEnd(_bool Finish) { m_bFinalEnd = Finish; }

	void							Cal_DitanceSoundSize(SFX_TYPE _Type, CTransform* _transForm);
	


	void							Update_ChooseIndex();



	vector<class CHandLighter_Battery_Count*> Get_BatteryCount() { return m_vecBatteryCount; }

	

	void							BatteryCount(class CHandLighter_Battery_Count* _data) { m_vecBatteryCount.push_back(_data); }

private:
	void							Use_Item();

public:
	void							All_DontUse(_uint UseItemType);

private:
	_int								m_iChooseIndex = { -1 };
	CGameInstance*						m_pGameInstance;
	
	vector<class CInnerInventory*>		m_vecInventory;
	CPlayer*							m_pPlayer = { nullptr };


private:
	_bool								m_bNextLevelGo = { false };
	_bool								m_bNextLevelOpen = { true };
	class CChooseBox*					m_pChooseBox = { nullptr };

private:
	_bool								m_MirrorUse		= { false };
	_bool								m_CompassUse	= { false };
	_bool								m_KaguraUse		= { false };
	_bool								m_GlowStoneUse	= { false };
	_bool								m_bDynamiteUse	= { false };
	_bool								m_bSphereUse	= { false };

	_bool								m_bFinalEnd		= { false };

	

	CCamera_POV*						m_pCamera		= { nullptr };
	
	_float								m_fHandLightTime = { 300.f };
	vector<class CHandLighter_Battery_Count*>			m_vecBatteryCount;

	_bool								m_bFistDynamite = { false };
	_bool								m_bFirstMade = { false };
	_float								m_fSphereTime	= { 0.f };

	/*Cool time*/

	_bool								m_bDynamiteCool = { false };
	_float								m_fDynamiteCool = { 0.0f };

	/* Stone Light*/
	_uint								m_iStoneLightIndex = { 0 };


	/* MagatamaCount*/
	_uint								m_MagatamaCount = { 0 };

	/* DynamiteCount*/
	_uint								m_iDynamiteCount = { 0 };
	_int								m_iDynamiteSountCount = { 0 };

	class CNavigation*					m_pNavigation = { nullptr };


	_bool								m_bEnd = { false };
	_bool								m_bEEnd = { false };
	_bool								m_bEEEnd = { false };

	_bool								m_bPlayerRevive = { false };


	_bool								m_bPlayerHeartBeat = { false };



public:
	void Set_Navigation(class CNavigation* pNavigation) { m_pNavigation = pNavigation; Safe_AddRef(m_pNavigation); }
	void Delete_Navigation();

	/*Cool Time / GET/SET */
public:
	_bool	Get_DynamiteCool() { return m_bDynamiteCool; }
	void	Set_DynamiteCool(_bool DynamiteCool) { m_bDynamiteCool = DynamiteCool; }
	_float	Get_DynamiteCoolF() { return m_fDynamiteCool; }
	void	Set_DynamiteCoolF(_float CoolTime) { m_fDynamiteCool = CoolTime; }

	void	Set_DynamiteSoundCount(_int _count) { m_iDynamiteSountCount += _count; }
	_int	Get_DynamiteSoundCount() { return m_iDynamiteSountCount; }

	/* For Font Count GET/SET*/
	_uint	Get_MagatamaCount() { return m_MagatamaCount; }
	void	Set_MagatamaCount(_uint MagatamaCount) { m_MagatamaCount = MagatamaCount; }

	/* For Center Word*/
	BUMP	m_eBumpType = BUMP_EMPTY;
	BUMP	Get_BumpType() { 
		return m_eBumpType; }
	void	Set_BumpType(BUMP _BumpType) { 
		m_eBumpType = _BumpType; }

	/* END */
	void Set_End(_bool _end) { m_bEnd = _end; }
	_bool Get_End() { return m_bEnd; }


	/* REVIVE */
	void Set_PlayerRevive(_bool _Revive) { m_bPlayerRevive = _Revive; }
	_bool Get_PlayerRevive() { return m_bPlayerRevive; }

	/* HeartBear */
	void Set_PlayerHeartBeat(_bool HeartBeat) { m_bPlayerHeartBeat = HeartBeat; }
	_bool Get_PlayerHeartBeat() { return m_bPlayerHeartBeat; }

public:
	void Free() override;
};

