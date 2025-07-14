#include "..\Public\Font_Manager.h"


#include "CustomFont.h"


CFont_Manager::CFont_Manager(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CFont_Manager::Add_Font(const _wstring & strFontTag, const _tchar * pFontFilePath)
{
	if (nullptr != Find_Font(strFontTag))
		return E_FAIL;

	CCustomFont*			pFont = CCustomFont::Create(m_pDevice, m_pContext, pFontFilePath);
	if (nullptr == pFont)
		return E_FAIL;

	m_Fonts.emplace(strFontTag, pFont);

	return S_OK;
}

HRESULT CFont_Manager::Render_Font(const _wstring & strFontTag, const _tchar * pText, const _float2 & vPosition, _fvector vColor, _float fRotation, const _float2 & vOrigin)
{
	CCustomFont*		pFont = Find_Font(strFontTag);
	if (nullptr == pFont)
		return E_FAIL;


	return pFont->Render(pText, vPosition, vColor, fRotation, vOrigin);
}

CCustomFont * CFont_Manager::Find_Font(const _wstring & strFontTag)
{
	auto	iter = m_Fonts.find(strFontTag);

	if(iter == m_Fonts.end())
		return nullptr;

	return iter->second;
}

CFont_Manager * CFont_Manager::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{	
	return new CFont_Manager(pDevice, pContext);
}

void CFont_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_Fonts)
		Safe_Release(Pair.second);

	m_Fonts.clear();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
