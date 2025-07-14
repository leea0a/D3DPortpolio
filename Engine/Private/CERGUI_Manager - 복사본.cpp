#include "UI_Manager.h"
#include<queue>
CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Initialize()
{
    return S_OK;
}

void CUI_Manager::Update_UIs()
{

    _uint m_iCurIndex = m_iMinPopUpUIDepth;

    priority_queue< pair< _uint, CUIObject*> , vector<pair<_uint,CUIObject*>> , greater<pair<_uint, CUIObject*>>> pq;
    for (auto iter = m_mapUIs.begin(); iter != m_mapUIs.end(); ++iter)
    {
        pq.push({ iter->second->Get_SortDepth(), iter->second });
    }

    while (!pq.empty())
    {
        pair<_uint, CUIObject*> pObjectInfo =pq.top();
        if(pObjectInfo.second->Get_Type() == UI_TYPE::UI_POP)
        {
            if (pObjectInfo.first != m_iCurIndex)
            {
                pObjectInfo.second->Set_SortDepth(m_iCurIndex);
            }
            
            ++m_iCurIndex;
            
        }
       
        pq.pop();
    }
    if(m_iMinPopUpUIDepth != m_iCurIndex)
    m_iMaxPopUpUIDepth = m_iCurIndex - 1;

}

HRESULT CUI_Manager::Add_UI_To_UIMap(const _wstring UI_Tag, CUIObject* pUIObject, UI_TYPE eType)
{
    if (nullptr != Find_UIObject(UI_Tag))
    {
        MSG_BOX("Already have this UI Object");
        return E_FAIL;
    }

    m_mapUIs.emplace(UI_Tag,pUIObject);

    if (UI_TYPE::UI_FIXED == eType)
    {
        pUIObject->Set_SortDepth(0);
    }
    /*else if (UI_TYPE::UI_POP == eType)
    {
        pUIObject->Set_SortDepth(m_iMinPopUpUIDepth);
    }*/

    //Safe_AddRef���Ұ��� 
    // ������Ʈ �Ŵ����� �߰��� �纻 �����ϰ� �ּ� �޾ƿ��� addref �ϴϱ�
    // ui�޾ƿͼ� ���⼭�� relese�ϸ� ������ 
    return S_OK;
}

HRESULT CUI_Manager::Set_Active_A_UI(const _wstring UI_Tag)
{
    CUIObject* pInstance = { nullptr };
    if (nullptr == (pInstance =Find_UIObject(UI_Tag)) )
    {
        MSG_BOX("There's no UI you named like that");
    }
    pInstance->Set_Active();

    if (UI_TYPE::UI_POP == pInstance->Get_Type())
    {
        pInstance->Set_SortDepth(++m_iMaxPopUpUIDepth);
 
    }

    return S_OK;
}

HRESULT CUI_Manager::Set_Active_All_UIs()
{

    for (auto el : m_mapUIs)
    {
       (el.second)->Set_Active();
    }
    return S_OK;
}

HRESULT CUI_Manager::Set_InActive_A_UI(const _wstring UI_Tag)
{
    CUIObject* pInstance = { nullptr };
    if (nullptr == (pInstance = Find_UIObject(UI_Tag)))
    {
        MSG_BOX("There's no UI you named like that");
    }
    pInstance->Set_InActive();
    UI_TYPE eType = pInstance->Get_Type();
    if (UI_TYPE::UI_POP == eType)
    {
        pInstance->Set_SortDepth(m_iMinPopUpUIDepth);
    }


    return S_OK;
}

HRESULT CUI_Manager::Set_InActive_All_UIs()
{
    for (auto el : m_mapUIs)
    {
        (el.second)->Set_InActive();
    }
    return S_OK;
}

HRESULT CUI_Manager::Clear_UIObjects()
{
    for (auto el : m_mapUIs)
    {
        Safe_Release(el.second);
    }
    m_mapUIs.clear();
    return S_OK;
}

HRESULT CUI_Manager::Change_A_UI_SortOrder(const _wstring UI_Tag, _uint iSortOrder, UI_TYPE eType)
{
    CUIObject* pInstance = { nullptr };
    if (nullptr == (pInstance = Find_UIObject(UI_Tag)))
    {
        MSG_BOX("There's no UI you named like that");
    }
    pInstance->Set_SortDepth(iSortOrder);

    return S_OK;
}

CUIObject* CUI_Manager::Find_UIObject(const _wstring UI_Tag)
{
    auto iter = m_mapUIs.find(UI_Tag);

    if (iter == m_mapUIs.end())
        return nullptr;

    return iter->second;
}
//���콺�� ������ ��ġ�� �ֿ켱 ���� ui�� ������ select ���·� ���� �� ���콺 ���¸� �����Ѵ�.-> 
//���콺���� Ŭ��(down)������ ȣ�� (��Ŭ��) ���콺�� ���õ� �����̸� ����� ������ ������, ���콺 ���¿� ���� ���콺 ������Ʈ�� ����(���콺�� üũ ������Ʈ�� ������Ʈ �ø��� ȣ���ؼ� ui ���� ����
CUIObject* CUI_Manager::Select_Object(_float2 MousePos) 
{

    priority_queue<pair<int, CUIObject*>>pq;


    for (auto UI : m_mapUIs)
    {
        pq.emplace( UI.second->Get_SortDepth(), UI.second);
    }
    
    CUIObject* SelectedUI = nullptr;
    pair<int, CUIObject*> UISelectedInfo = {};
    
    /*���콺���� �������Ͱ� �ƴ϶�� -> ���콺 ���¸� �����ؼ� ....*/
    if (pq.empty())
        return nullptr;

    UISelectedInfo = pq.top();
    SelectedUI = UISelectedInfo.second;
    
    
    if (UI_TYPE::UI_POP == SelectedUI->Get_Type())
    {
        SelectedUI->Set_SortDepth(++m_iMaxPopUpUIDepth);
        SelectedUI->Set_Is_Selected();
        pq.pop();
    }
    else
    {
        return nullptr;
    }
        while (!pq.empty())
        {
            if (UI_TYPE::UI_POP == pq.top().second->Get_Type()) {
                pq.top().second->Set_Is_Not_Selected();
                
            }

        }
    
    return SelectedUI;

}

CUI_Manager* CUI_Manager::Create()
{
    CUI_Manager* pInstance = new CUI_Manager();
    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Create UIManager");
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

void CUI_Manager::Free()
{
    for (auto el : m_mapUIs)
    {
        Safe_Release(el.second);
    }
    m_mapUIs.clear();

    __super::Free();
}
