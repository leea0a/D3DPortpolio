#pragma once

#include "Client_Defines.h"
#include "Level.h"


BEGIN(Client)

class CLevel_GamePlay_Lv2 final : public CLevel
{
private:
	CLevel_GamePlay_Lv2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay_Lv2() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;	
private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_GameObject(const _wstring& strLayerTag);
	HRESULT Ready_Lights();
	HRESULT Ready_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Player(const _wstring& strLayerTag);

	HRESULT Picking_And_Create_Collider();
	void	IMGUI_Creating_Collider();
	void    IMGUI_Editing_Collider();

public:
	static CLevel_GamePlay_Lv2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

private:
	//CImguiWindow* m_pImguiMgr = { nullptr };

	//for Terrain_Setting
	class CTerrain* m_pTerrain = {nullptr};
	const _float4* m_pPos = { nullptr };
	_uint		   m_VerticesX = { 0 };
	_uint		   m_VerticesZ = { 0 };

	/* for Manageming Cube Created*/
	vector<class CCubeCollider*> m_vecCubeCollider;
	_float3						 m_vCollider_Scale = {1.f,1.f,1.f};
	_float4						 m_vCollider_RotAxis = {0.f,1.f,0.f,0.f};
	_float						 m_vCollider_Rotation = { 0.f };
	_float4						 m_vCollider_Position = {0.f,0.f,0.f,1.f};
	_int						 m_iCubeColliderNowIndex = {0};
	_bool						 m_bCreatingCubeMode = { false };


};

END