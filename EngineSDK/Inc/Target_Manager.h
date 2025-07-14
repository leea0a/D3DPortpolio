#pragma once

#include "Base.h"

/* �츮�� ������ ����Ÿ�ٵ��� ��Ƴ��´�. */
/* ��ġ�� ����(�ִ� 8��)�� ���ε��� Ÿ�ٵ��� �ѹ��� ������´�. */
/* ��ġ�� ���ε��ض�! (������� ����Ÿ�ٵ��� ���ÿ� ��ġ�� �ø���.) */


BEGIN(Engine)

class CTarget_Manager final : public CBase
{
private:
	CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	HRESULT Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr, _bool isClear = true);
	HRESULT End_MRT();
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag);
	HRESULT Copy_RT_Resource(const _wstring& strTargetTag, ID3D11Texture2D* pDest);
#ifdef _DEBUG
public:
	HRESULT Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

#endif


private:
	ID3D11Device*										m_pDevice = { nullptr };
	ID3D11DeviceContext*								m_pContext = { nullptr };

	ID3D11RenderTargetView*								m_pBackRTV = { nullptr };
	ID3D11DepthStencilView*								m_pOriginalDSV = { nullptr };
	map<const _wstring, class CRenderTarget*>			m_RenderTargets;
	map<const _wstring, list<class CRenderTarget*>>		m_MRTs;

public:
	class CRenderTarget* Find_RenderTarget(const _wstring& strTargetTag);
	class list<class CRenderTarget*>* Find_MRT(const _wstring& strMRTTag);

public:
	static CTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END