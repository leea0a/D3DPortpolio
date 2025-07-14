#include"stdafx.h"
#include"UI_Parent.h"
#include"GameInstance.h"
CUI_Parent::CUI_Parent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{pDevice,pContext}
{
}

CUI_Parent::CUI_Parent(const CUI_Parent& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CUI_Parent::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CUI_Parent::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Child_UI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Parent::Add_Child_UI(_uint iPrototypeLevelIndex, const _wstring& strPrototype_Object_Tag, const _wstring& strLayerTag, const _wstring& strChild_UIObject_Tag, _uint iLevelIndex, CUIObject** ppOut, void* pArg)
{

	if (nullptr != Find_Child_UI(strChild_UIObject_Tag))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(iPrototypeLevelIndex, strPrototype_Object_Tag, iLevelIndex, strLayerTag, pArg, reinterpret_cast<CGameObject**>(ppOut))))
		return E_FAIL;

	Safe_AddRef(*ppOut);//�ڽĿ��� ref-1- �ڽ� ���� ��� ������ ���Ѱ�(P_UIButton, pUIxxx...)-> map���� ������ �Ŷ�� SafeRelease�ø��� �ȵ�(�ٵ� �ϴ� ��� ���� �ϳ� �α� �ҰŴ�)
	m_ChildUIs.emplace(strChild_UIObject_Tag, *ppOut); //�θ𿡼� map �����Ҷ� ref-1
	//������Ʈ �Ŵ������� clear


	return S_OK;
}

CUIObject* CUI_Parent::Find_Child_UI(const _wstring& strChild_UI_Tag)
{
	auto iter = m_ChildUIs.find(strChild_UI_Tag);

	if (m_ChildUIs.end() == iter)
		return nullptr;

	return iter->second;
}

HRESULT CUI_Parent::Ready_Child_UI()
{
	return S_OK;
}

void CUI_Parent::Free()
{
	for (auto UI : m_ChildUIs)
	{
		Safe_Release(UI.second);
	}
	__super::Free();
}
