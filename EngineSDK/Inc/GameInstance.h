#pragma once

#include "Shadow.h"
#include "Height.h"
#include "Renderer.h"
#include "PipeLine.h"
#include "Prototype_Manager.h"
#include "Key_Manager.h"
#include "SoundMgr.h"

/* 1. 클라이언트 개발자가 사용할 만한 엔진에서 제공해주는 기능들을 보여준다 .*/
/* 2. 엔진의 기능을 원활하게 사용할 수 있도록 엔진을 초기화한다.Initialize_Engine */
/* 3. 사용 종료시에 엔진 기능 구현을 위해서 할당했던 모든 데이터를 삭제한다. Release_Engine*/
/* 4. 엔진에서 갱신이 필요한 모든 객체를 한곳에 모아서 갱신해 준다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void	Update_Engine(_float fTimeDelta);
	HRESULT Render_Begin(const _float4& vClearColor = _float4(0.f, 0.f, 0.f, 1.f));
	//HRESULT Render_Begin(const _float4& vClearColor = _float4(0.226f, 0.226f, 0.226f, 1.f));
	HRESULT Draw();
	HRESULT Render_End();
	void	Clear(_int iLevelID);

	_float Compute_Random_Normal();
	_float Compute_Random(_float fMin, _float fMax);

public: /* for.Input_Device */
	KEY_STATE	Get_DIKeyState(KEY eKeyID);
	KEY_STATE	Get_DIMouseState(MOUSE eMouse);
	_long		Get_DIMouseMove(MOUSE_MOVE eMouseState);

public: /* For.timer_Manager */
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT	Add_Timer(const _wstring& strTimerTag);
	void Update_TimeDelta(const _wstring& strTimerTag);

public: /* For.Renderer */
	HRESULT Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Add_DebugComponent(CComponent* pDebugCom);
	HRESULT	Set_WhiteBlackShader(_bool Use);

	/* FADE*/
	HRESULT Start_Fade(_bool _use);
	HRESULT Start_EndFade(_bool _use);
	HRESULT Stop_Fade();
	HRESULT Set_FadeSpeed(_float fSpeed);
	_float Get_FadeAmount();
	HRESULT Set_GlitchShader(_bool _use);

public: /* for.Level_Manager */
	HRESULT Open_Level(_int iLevelIndex, class CLevel* pNewLevel);
	_int	Get_CurrentLevel();
	_int	Get_PreLevelID();

public: /* For.Prototype_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase* Clone_Prototype(Engine::PROTOTYPE eType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.Object_Manager */
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr,CGameObject** ppout = nullptr);
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iObjectIndex = 0);
	_int Get_ComponentCount(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag);
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, _uint iPartObjectIndex, const _wstring& strPartComponentTag, _uint iObjectIndex = 0);
	HRESULT	Remove_Object(_uint iLevelID, const _wstring& strLayerTag, CGameObject* pRemoveObject);
	HRESULT Get_Object(_uint iLevelIndex, const _wstring& strLayerTag, list<class CGameObject*> ppOut);

public: /* For.PipeLine */
	void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
	_matrix Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState);
	_matrix Get_TransformMatrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState);
	_matrix Get_TransformInversedMatrix(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformInversedFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState);
	const _float4* Get_CamPosition() const;

public:/*for Light_Manager*/
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Delete_Light();

	HRESULT Add_PlayerLight(_int _index, const LIGHT_DESC& LightDesc);
	HRESULT Update_PlayerPos(_int _index, _vector Pos);
	HRESULT Delete_PlayerLight(_int _index);
	HRESULT PlayerLight_TurnOnOff(_int _index, _bool OnOff);
	HRESULT Update_PlayerDir(_int _index, _float4 Dir);
	HRESULT Delete_PlayerAllLight();

	HRESULT Add_CandleLight(const LIGHT_DESC& LightDesc, _int _index);
	HRESULT Delete_CandleLight(_int CandleIndex);
	HRESULT CandeLight_TurnOnOff(_int CandleIndex, _bool OnOff);

	HRESULT Add_MonsterLight(LIGHT_DESC LightDesc, _int _index);
	HRESULT Update_MonsterPos(_int _index, _vector Pos);
	HRESULT Delete_MonsterLight(_int _index);
	HRESULT MonsterLight_TurnOnOff(_int _index, _bool OnOff);
	
	HRESULT Add_StoneLight(LIGHT_DESC LightDesc, _uint _index);
	HRESULT Update_StoneLight(_int _index, _vector Pos);
	HRESULT Delete_SToneLight(_int _index);
	HRESULT StoneLight_TurnOnOff(_int _index, _bool OnOff);

	HRESULT Add_MagatamaLight(LIGHT_DESC LightDesc, _int _index);
	HRESULT Update_MagatamaPos(_int _index, _vector Pos);
	HRESULT Delete_MagamaLight(_int _index);
	HRESULT MagatamaLight_TurnOnOff(_int _index, _bool OnOff);

	HRESULT Add_DynamiteLight(LIGHT_DESC LightDesc, _int _index);
	HRESULT DynamiteLight_TurnOnOff(_int _index, _bool OnOff);


	const LIGHT_DESC* Get_LightDesc(_uint iIndex)const;
	HRESULT Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer);
	_int Get_PlayerLightCount();
	_int Get_StoneLightCount();

public: /* Font_Manager */
	HRESULT Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f));

	

public: /* Target_Manager*/
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	HRESULT Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr, _bool isClear = true);
	HRESULT End_MRT();
	HRESULT Bind_RT_ShaderResource(CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag);
	HRESULT Copy_RT_Resource(const _wstring& strTargetTag, ID3D11Texture2D* pDest);



#ifdef _DEBUG
	HRESULT Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_RT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer);
#endif

public: /* Picking */
	_bool isPicked(_float4* pWorldPos);

public: /* Height */
	const _float4x4* Get_Height_Transform(CHeight::D3DTRANSFORMSTATE eState);
	void SetUp_Height_TransformMatrix(CHeight::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
	_float Compute_Height(_fvector vWorldPos);

public: /* Shadow */
	const _float4x4* Get_Shadow_Transform(CShadow::D3DTRANSFORMSTATE eState);
	void SetUp_Shadow_TransformMatrix(CShadow::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);

public: /* Frustum*/
	void Transform_Frustum_ToLocalSpace(_fmatrix WorldMatrix);
	_bool isIn_Frustum_InWorldSpace(_fvector vWorldPos, _float fRange);
	_bool isIn_Frustum_InLocalSpace(_fvector vLocalPos, _float fRange);




private:
	class CFrustum* m_pFrustum = { nullptr };
	class CShadow* m_pShadow = { nullptr };
	class CHeight* m_pHeight = { nullptr };
	class CPicking* m_pPicking = { nullptr };
	class CTarget_Manager* m_pTarget_Manager = { nullptr };
	class CFont_Manager* m_pFontManager = { nullptr };
	class CLight_Manager* m_pLightManager = { nullptr };
	class CKey_Manager* m_pKeyManager = { nullptr };
	class CPipeLine* m_pPipeLine = { nullptr };
	class CRenderer* m_pRenderer = { nullptr };
	class CObject_Manager* m_pObject_Manager = { nullptr };
	class CPrototype_Manager* m_pPrototype_Manager = { nullptr };
	class CLevel_Manager* m_pLevel_Manager = { nullptr };
	class CTimer_Manager* m_pTimer_Manager = { nullptr };
	class CGraphic_Device* m_pGraphic_Device = { nullptr };



	_uint m_iCurrentLevel = { 0 };

public:
	void Set_CurrentLevel(_uint index) { m_iCurrentLevel = index; }
	_uint Get_Level() { return m_iCurrentLevel; }


public:
	static void Release_Engine();

public:
	virtual void Free() override;

};

END


