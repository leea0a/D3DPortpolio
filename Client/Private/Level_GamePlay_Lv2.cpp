#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "CubeCollider.h"
#include "Terrain.h"
#include "Level_Loading.h"
#include "Level_GamePlay_Lv2.h"

CLevel_GamePlay_Lv2::CLevel_GamePlay_Lv2(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_GamePlay_Lv2::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camara"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_GameObject(TEXT("Layer_GameObject"))))
		return E_FAIL;

	if (FAILED(Ready_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;


	CUI_Manager::GetInstance()->Set_NextLevelLevelOpen(true);
	return S_OK;
}

void CLevel_GamePlay_Lv2::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		//m_pGameInstance->Clear_UIObjects();//ui는 static에 두면 안됨 아니면 다 static에 박아두고 이거 뺴면됨
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY_LV2));
		return; //-> 오픈 레벨하고 키 입력하면 터짐 왜지? 
	}

	if (MOUSE_UP(MOUSE::DIM_LB)) {
		Picking_And_Create_Collider();
	}
}

HRESULT CLevel_GamePlay_Lv2::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
#endif
	
	
	

	return S_OK;
}

HRESULT CLevel_GamePlay_Lv2::Ready_Layer_Camera(const _wstring & strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC		Desc{};

	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(180.f);
	Desc.fMouseSensor = 0.1f;
	Desc.fFovy = XMConvertToRadians(60.f);
	Desc.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;
	Desc.vEye = _float3(10.f, 10.f, 10.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_GAMEPLAY_LV2, strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay_Lv2::Ready_Layer_BackGround(const _wstring & strLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY_LV2, strLayerTag, nullptr ,reinterpret_cast<CGameObject**>(& m_pTerrain) )))
	//	return E_FAIL;
	//if (nullptr != m_pTerrain)
	//{
	//	m_pPos = m_pTerrain->Get_Vertices();
	//	m_VerticesX = m_pTerrain->Get_VerticesX();
	//	m_VerticesZ = m_pTerrain->Get_VerticesZ();
	//}
	return S_OK;
}

HRESULT CLevel_GamePlay_Lv2::Ready_Layer_GameObject(const _wstring& strLayerTag)
{
	//CCubeCollider::CUBE_DESC CubeDesc = {};
	//CubeDesc.vPos = {3.f,6.f,3.f};
	//CubeDesc.vRotAxis = {0.f,1.f,0.f,1.f};
	//CubeDesc.vScale = {2.f,2.f,2.f};
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_GameObject_CubeCollider"), LEVEL_GAMEPLAY_LV2, strLayerTag, &CubeDesc)))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_GameObject_Stage1"), LEVEL_GAMEPLAY_LV2, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay_Lv2::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTOINAL;
	LightDesc.vDirection = _float4(-1.f, -1.f, -1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay_Lv2::Ready_Monster(const _wstring& strLayerTag)
{

	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY_LV2, TEXT("Prototype_GameObject_Monster02"), LEVEL_GAMEPLAY_LV2, strLayerTag)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay_Lv2::Ready_Player(const _wstring& strLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CLevel_GamePlay_Lv2::Picking_And_Create_Collider()
{

	POINT		pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_vector vMousePos = {};
	
	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	//마우스-> ndc 좌표
	vMousePos = { (_float)pt.x / (ViewportDesc.Width * 0.5f) - 1.f,
				  (_float)pt.y / (ViewportDesc.Height * 0.5f) - 1.f,
				0.f,
				1.f
	};
	//투영 ->뷰스페이스
	_matrix matProj = {};
	matProj=m_pGameInstance->Get_TransformInversedMatrix(CPipeLine::D3DTS_PROJ);
	vMousePos = XMVector3TransformCoord(vMousePos, matProj);

	_vector vRayPos, vRayDir;
	
	vRayPos = { 0.f,0.f,0.f,1.f };
	vRayDir = vMousePos - vRayPos;

	//뷰->월드
	_matrix matView = {};
	matView=m_pGameInstance->Get_TransformInversedMatrix(CPipeLine::D3DTS_VIEW);
	vRayPos = XMVector3TransformCoord(vRayPos,matView);
	vRayDir = XMVector3TransformNormal(vRayDir, matView);

	//뷰-> 테레인의 로컬
	_matrix matWorld = {};
	if(nullptr != m_pTerrain)
		matWorld = m_pTerrain->Get_MatWorldInverse();

	vRayPos = XMVector3TransformCoord(vRayPos, matWorld);
	vRayDir = XMVector3TransformNormal(vRayDir, matWorld);
	vRayDir = XMVector3Normalize(vRayDir);//필수적으로 노말라이즈 시켜준다.

	CCubeCollider* pCollider = { nullptr }; //벡터 저장용

	for(int i=0;i<m_VerticesZ-1;++i)
		for (int j = 0; j < m_VerticesX-1; ++j)
		{

			int iIndex = i * m_VerticesX + j;

			int iIndices[4] =
			{
				iIndex + m_VerticesX,
				iIndex + m_VerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_float fDist = { 1 };
			if (TriangleTests::Intersects(
				vRayPos, vRayDir,
				XMLoadFloat4(&m_pPos[iIndices[1]]),
				XMLoadFloat4(&m_pPos[iIndices[0]]),
				XMLoadFloat4(&m_pPos[iIndices[2]]),
				fDist))
			{
				_vector PickingPos = fDist * vRayDir + vRayPos;
				

				_matrix matWorld = {};
				if (nullptr != m_pTerrain)
					matWorld = m_pTerrain->Get_MatWorld();
				PickingPos=XMVector3TransformCoord(PickingPos, matWorld);

				_float4 PickingPosStore;
				XMStoreFloat4(&PickingPosStore, PickingPos);


				CCubeCollider::CUBE_DESC CubeDesc = {};
				CubeDesc.vPos = { PickingPosStore.x ,PickingPosStore.y,PickingPosStore.z };
				CubeDesc.vRotAxis = { 0.f,1.f,0.f,1.f };
				CubeDesc.vScale = { 1.f,1.f,1.f };
				{
					m_vCollider_Position = PickingPosStore;
					m_vCollider_RotAxis = CubeDesc.vRotAxis;
					m_vCollider_Rotation = 0.f;
					m_vCollider_Scale = CubeDesc.vScale;

				}

				if (FAILED(m_pGameInstance->
					Add_GameObject_ToLayer(
						LEVEL_GAMEPLAY,
						TEXT("Prototype_GameObject_CubeCollider"),
						LEVEL_GAMEPLAY,
						L"Layer_GameObject",
						&CubeDesc,
						reinterpret_cast<CGameObject**>( &pCollider))))
					return E_FAIL;


				m_vecCubeCollider.push_back(static_cast<CCubeCollider*>(pCollider));
				return S_OK;
			};
			if (TriangleTests::Intersects(
				vRayPos, vRayDir,
				XMLoadFloat4(&m_pPos[iIndices[2]]),
				XMLoadFloat4(&m_pPos[iIndices[0]]),
				XMLoadFloat4(&m_pPos[iIndices[3]]),
				fDist))
			{
				_vector PickingPos = fDist * vRayDir + vRayPos;
				

				_matrix matWorld = {};
				if (nullptr != m_pTerrain)
					matWorld = m_pTerrain->Get_MatWorld();
				PickingPos = XMVector3TransformCoord(PickingPos, matWorld);

				_float4 PickingPosStore;
				XMStoreFloat4(&PickingPosStore, PickingPos);

				CCubeCollider::CUBE_DESC CubeDesc = {};
				CubeDesc.vPos = { PickingPosStore.x ,PickingPosStore.y,PickingPosStore.z };
				CubeDesc.vRotAxis = { 0.f,1.f,0.f,1.f };
				CubeDesc.vScale = { 1.f,1.f,1.f };

				{
					m_vCollider_Position = PickingPosStore;
					m_vCollider_RotAxis = CubeDesc.vRotAxis;
					m_vCollider_Rotation = 0.f;
					m_vCollider_Scale = CubeDesc.vScale;

				}
				if (FAILED(m_pGameInstance->
					Add_GameObject_ToLayer(
						LEVEL_GAMEPLAY,
						TEXT("Prototype_GameObject_CubeCollider"),
						LEVEL_GAMEPLAY,
						L"Layer_GameObject",
						&CubeDesc,
						reinterpret_cast<CGameObject**>(&pCollider))))
					return E_FAIL;

				m_vecCubeCollider.push_back(static_cast<CCubeCollider*>(pCollider));
				return S_OK;
			};
		}
	return S_OK;
}

void CLevel_GamePlay_Lv2::IMGUI_Creating_Collider()
{
	/*ImGui::Begin("Creating Collider");

	if (m_iCubeColliderNowIndex + 1 <= m_vecCubeCollider.size())
	{
		m_vCollider_Position = m_vecCubeCollider[m_iCubeColliderNowIndex]->Get_Transform_Move_Position();
	}

	ImGui::InputFloat3("ColliderScale", &m_vCollider_Scale.x,"%f", 0);
	ImGui::InputFloat3("Rotation_Axis",&m_vCollider_RotAxis.x,"%f",0);
	ImGui::SameLine();
	ImGui::SliderFloat("RotationRadian",&m_vCollider_Rotation,0.f,360.f,"%f",0);
	ImGui::InputFloat4("Position",&m_vCollider_Position.x,"%f");

	if (m_iCubeColliderNowIndex + 1 <= m_vecCubeCollider.size()) 
	{
		m_vecCubeCollider[m_iCubeColliderNowIndex]->Update_Transform_Rotaion(m_vCollider_RotAxis, m_vCollider_Rotation);
		m_vecCubeCollider[m_iCubeColliderNowIndex]->Update_Transform_Scale(m_vCollider_Scale);
		m_vecCubeCollider[m_iCubeColliderNowIndex]->Update_Transform__Move_Position(m_vCollider_Position);
	}
	if (ImGui::Button("NextObject", ImVec2(50, 20)))
	{
		if(m_iCubeColliderNowIndex + 1 < m_vecCubeCollider.size())
			m_iCubeColliderNowIndex++;
	}

	if (ImGui::Button("PrevObject", ImVec2(50, 20)))
	{
		if (m_iCubeColliderNowIndex > 0)
		m_iCubeColliderNowIndex--;
	}
	ImGui::InputInt("NowColliderIndex", &m_iCubeColliderNowIndex);

	ImGui::End();*/

}

void CLevel_GamePlay_Lv2::IMGUI_Editing_Collider()
{
}


CLevel_GamePlay_Lv2 * CLevel_GamePlay_Lv2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay_Lv2*	pInstance = new CLevel_GamePlay_Lv2(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay_Lv2");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLevel_GamePlay_Lv2::Free()
{
	for (auto iter = m_vecCubeCollider.begin(); iter != m_vecCubeCollider.end(); ++iter)
	{
		Safe_Release(*iter);
	}

	Safe_Release(m_pTerrain);
	__super::Free();
}
