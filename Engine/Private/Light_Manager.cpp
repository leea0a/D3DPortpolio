#include "Light_Manager.h"
#include"Light.h"
CLight_Manager::CLight_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

const LIGHT_DESC* CLight_Manager::Get_LightDesc(_uint iIndex) const
{
    auto iter = m_Lights.begin();
    for (size_t i = 0; i < iIndex; ++i)
    {
        ++iter; 
    }

    return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Initialize()
{
    return S_OK;
}

HRESULT CLight_Manager::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
    for (auto& pLight : m_Lights)
    {
        pLight->Render(pShader, pVIBuffer);
    }

    for (auto& pPlayerLight : m_PlayerLights)
    {
        pPlayerLight.second->Render(pShader, pVIBuffer);
    }

    for (auto& pCandleLight : m_CandleLights)
    {
        pCandleLight.second->Render(pShader, pVIBuffer);
    }

    for (auto& pMonsterLight : m_MonsterLights)
    {
        pMonsterLight.second->Render(pShader, pVIBuffer);
    }

    for (auto& pLightSotneLight : m_StoneLights)
    {
        pLightSotneLight.second->Render(pShader, pVIBuffer);
    }

    for (auto& pMagamataLight : m_MagatamaLights)
    {
        pMagamataLight.second->Render(pShader, pVIBuffer);
    }

    for (auto& pDynamiteLight : m_DynamiteLights)
    {
        pDynamiteLight.second->Render(pShader, pVIBuffer);
    }

    return S_OK;
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC& LightDesc)
{
    CLight* pLight =  CLight::Create(m_pDevice, m_pContext, LightDesc);

    if (nullptr == pLight)
        return E_FAIL;

    m_Lights.push_back(pLight);
    return S_OK;
}

HRESULT CLight_Manager::Delete_Light()
{
    for (auto iter : m_Lights)
        Safe_Release(iter);

    m_Lights.clear();

    return S_OK;
}

HRESULT CLight_Manager::Add_PlayerLight(_int _index, const LIGHT_DESC& LightDesc)
{
    CLight* pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);

    if (nullptr == pLight)
        return E_FAIL;

    m_PlayerLights.emplace(_index, pLight);
    return S_OK;
}

HRESULT CLight_Manager::Add_CandleLight(const LIGHT_DESC& LightDesc, _int _index)
{
    CLight* pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);

    if (nullptr == pLight)
        return E_FAIL;

    m_CandleLights.emplace(_index, pLight);
    return S_OK;
}

HRESULT CLight_Manager::Add_MonsterLight(LIGHT_DESC LightDesc, _int _index)
{
    CLight* pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);

    if (nullptr == pLight)
        return E_FAIL;

    m_MonsterLights.emplace(_index, pLight);
    return S_OK;
}

HRESULT CLight_Manager::Update_MonsterPos(_int _index, _vector Pos)
{
    CLight* iter = m_MonsterLights[_index];

    iter->Set_LightDesc(Pos);

    return S_OK;
}

HRESULT CLight_Manager::Delete_MonsterLight(_int _index)
{
    CLight* _Light = m_MonsterLights[_index];

    Safe_Release(_Light);
    m_MonsterLights.erase(_index);

    return S_OK;
}

HRESULT CLight_Manager::MonsterLight_TurnOnOff(_int _index, _bool OnOff)
{
    CLight* _Light = m_MonsterLights[_index];

    _Light->Set_LightOnOff(OnOff);

    return S_OK;
}



HRESULT CLight_Manager::Update_PlayerPos(_int _index, _vector Pos)
{
    CLight* iter = m_PlayerLights[_index];
    iter->Set_PlayerLightDesc(Pos);

    return S_OK;
}

HRESULT CLight_Manager::Delete_PlayerLight(_int _index)
{

    CLight* _Light = m_PlayerLights[_index];

    Safe_Release(_Light);

    

    return S_OK;
}

HRESULT CLight_Manager::PlayerLight_TurnOnOff(_int _index, _bool OnOff)
{
    CLight* _Light = m_PlayerLights[_index];
    _Light->Set_LightOnOff(OnOff);

    return S_OK;
}

HRESULT CLight_Manager::Delete_PlayerAllLight()
{
    for (auto iter : m_PlayerLights)
        Safe_Release(iter.second);

    m_PlayerLights.clear();
    
    return S_OK;
}

HRESULT CLight_Manager::Update_PlayerDir(_int index, _float4 Dir)
{
    if (m_PlayerLights.size() <= 0)
    {
        return S_OK;
    }
    
    auto Light = m_PlayerLights[index];
    Light->Set_PlayerDir(Dir);

    return S_OK;
}

HRESULT CLight_Manager::Update_StoneLight(_int _index, _vector Pos)
{
    auto iter = m_StoneLights[_index];

    iter->Set_LightDesc(Pos);

    return S_OK;
}

HRESULT CLight_Manager::Add_StoneLight(LIGHT_DESC LightDesc, _uint _index)
{
    CLight* pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);

    if (nullptr == pLight)
        return E_FAIL;

    m_StoneLights.emplace(_index, pLight);

    return S_OK;
}

HRESULT CLight_Manager::Delete_SToneLight(_int _index)
{
    CLight* _Light = m_StoneLights[_index];

    Safe_Release(_Light);
    m_StoneLights.erase(_index);

    return S_OK;
}

HRESULT CLight_Manager::StoneLight_TurnOnOff(_int _index, _bool OnOff)
{
    CLight* _Light = m_StoneLights[_index];
    _Light->Set_LightOnOff(OnOff);

    return S_OK;
}

_int CLight_Manager::Get_PlayerLightCount()
{
    return m_PlayerLights.size();
}

HRESULT CLight_Manager::Add_DynamiteLight(LIGHT_DESC LightDesc, _int _index)
{
    CLight* pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);

    if (nullptr == pLight)
        return E_FAIL;

    m_DynamiteLights.emplace(_index, pLight);

    return S_OK;
}

HRESULT CLight_Manager::DynamiteLight_TurnOnOff(_int _index, _bool OnOff)
{
    CLight* _Light = m_DynamiteLights[_index];
    _Light->Set_LightOnOff(OnOff);

    return S_OK;
}

HRESULT CLight_Manager::Delete_CandleLight(_int CandleIndex)
{
    auto iter = m_CandleLights.find(CandleIndex);
    CLight* _Light = iter->second;

    Safe_Release(_Light);
    m_CandleLights.erase(CandleIndex);

    return S_OK;
}

HRESULT CLight_Manager::CandeLight_TurnOnOff(_int CandleIndex, _bool OnOff)
{
    auto iter = m_CandleLights.find(CandleIndex);
    CLight* _Light = iter->second;

    _Light->Set_LightOnOff(OnOff);

    return S_OK;
}

HRESULT CLight_Manager::Add_MagatamaLight(LIGHT_DESC LightDesc, _int _index)
{
    CLight* pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);

    if (nullptr == pLight)
        return E_FAIL;

    m_MagatamaLights.emplace(_index, pLight);

    return S_OK;
}

HRESULT CLight_Manager::Update_MagatamaPos(_int _index, _vector Pos)
{
    auto iter = m_MagatamaLights[_index];

    iter->Set_LightDesc(Pos);

    return S_OK;
}

HRESULT CLight_Manager::Delete_MagamaLight(_int _index)
{

    CLight* _Light = m_MagatamaLights[_index];

    Safe_Release(_Light);
    m_MagatamaLights.erase(_index);

    return S_OK;
}

HRESULT CLight_Manager::MagatamaLight_TurnOnOff(_int _index, _bool OnOff)
{
    CLight* _Light = m_MagatamaLights[_index];
    _Light->Set_LightOnOff(OnOff);

    return S_OK;
}



CLight_Manager* CLight_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLight_Manager* pInstance = new CLight_Manager(pDevice, pContext);
    if (pInstance->Initialize())
    {
        MSG_BOX("Create Clight_manager: Failed");
        Safe_Release(pInstance);
        return nullptr;

    }
    return pInstance;
}

void CLight_Manager::Free()
{
    __super::Free();
    for (auto Light : m_Lights)
    {
        Safe_Release(Light);
    }

    for (auto& Pair : m_PlayerLights)
       Safe_Release(Pair.second);

    m_PlayerLights.clear();

    for (auto& Pair : m_CandleLights)
        Safe_Release(Pair.second);

    m_CandleLights.clear();

    for (auto& Pair : m_MonsterLights)
        Safe_Release(Pair.second);

    m_MonsterLights.clear();

    for (auto& Pair : m_StoneLights)
        Safe_Release(Pair.second);

    m_StoneLights.clear();


    for (auto& Pair : m_MagatamaLights)
        Safe_Release(Pair.second);

    m_MagatamaLights.clear();

    for (auto& Pair : m_DynamiteLights)
        Safe_Release(Pair.second);

    m_DynamiteLights.clear();


    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
