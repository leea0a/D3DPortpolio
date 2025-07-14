#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_HEIGHT, RG_SHADOW, RG_NONBLEND, RG_NONPICKED, RG_NONLIGHT, RG_BLEND, RG_UI, RG_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw_RenderObject();

	/* 흑백 쉐이더*/
	HRESULT	Set_WhiteBlackShader(_bool Use) { m_bUseWhiteBlack = Use; return S_OK; }

	/* 페이드 쉐이더*/
	HRESULT Start_Fade(_bool _use) { m_bIsFading = _use; m_bFadeIn = _use; /*m_fFadeAmount = 0.0f;*/ return S_OK; }
	HRESULT Start_EndFade(_bool _use) { m_bEndFadeIN = _use; return S_OK; }
	HRESULT Stop_Fade() { m_bFadeIn = false; return S_OK; }
	HRESULT Set_FadeSpeed(_float fSpeed) { m_fFadeSpeed = fSpeed; return S_OK; }
	_float	Get_FadeAmount() const { return m_fFadeAmount; }


	/* 색수차 쉐이더*/
	HRESULT Set_GlitchShader(_bool _use) { m_bUseChromatic = _use;  return S_OK; }


#ifdef _DEBUG
public:
	HRESULT Add_DebugComponent(class CComponent* pDebugCom) { m_DebugComponents.push_back(pDebugCom); Safe_AddRef(pDebugCom); return S_OK; }

#endif

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	list<class CGameObject*>			m_RenderObjects[RG_END];
	class CGameInstance* m_pGameInstance = { nullptr };
	class CShader* m_pShader = { nullptr };
	class CVIBuffer_Rect* m_pVIBuffer = { nullptr };
	ID3D11DepthStencilView* m_pMaxDepthStencilView = { nullptr };
	_float4x4							m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};
	_uint								m_iOriginalViewportWidth{}, m_iOriginalViewportHeight{};


	_bool								m_bUseWhiteBlack = { false };

private:
	_float m_fFadeAmount = { 0.0f };
	_float m_fFadeSpeed = { 0.f };		// 페이드 속도
	_bool m_bFadeIn = { false };	// 페이드 방향
	_bool m_bIsFading = { false };    // 페이드 활성화 여부

	_bool m_bEndFadeIN = { false };
	_float m_fEndFadeAmount = { 0.0f };
	_float m_fEndFadeSpeed = { 0.08f };

	/* GLITCH*/
	float m_fChromaticAmount	= { 0.005f };  // 색수차 강도
	bool m_bUseChromatic		= { false };      // 색수차 사용 
	float m_fTime				= { 0.0f };
	float m_fShakeAmount		= { 0.3f };


#ifdef _DEBUG
private:
	list<class CComponent*>				m_DebugComponents;



#endif

private:
	HRESULT Render_Priority();
	HRESULT Render_Height();
	HRESULT Render_Shadow();
	HRESULT Render_NonBlend();
	HRESULT Render_Lights();
	HRESULT Render_Final();
	HRESULT Render_Blur_X();
	HRESULT Render_Blur_Y();
	HRESULT Render_NonLight();
	HRESULT Render_Blend();
	HRESULT Render_Glitch();
	HRESULT Render_Fade();
	HRESULT Render_UI();
	HRESULT Render_EndFade();

	

	HRESULT Ready_DepthStencilView(_uint iWidth, _uint iHeight);
	HRESULT SetUp_Viewport(_uint iWidth, _uint iHeight);

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END