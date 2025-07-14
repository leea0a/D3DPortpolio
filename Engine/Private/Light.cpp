#include "Light.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

CLight::CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice),
    m_pContext(pContext)
{
    Safe_AddRef(pDevice);
    Safe_AddRef(pContext);
}

HRESULT CLight::Initialize(const LIGHT_DESC& LightDesc)
{
    m_LightDesc = LightDesc;

    return S_OK;
}

HRESULT CLight::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	if (m_LightDesc.TurnOn_Off == true)
	{
		_uint		iPassIndex = {};
    
		if (LIGHT_DESC::TYPE_DIRECTOINAL == m_LightDesc.eType)
		{
			if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
				return E_FAIL;
			iPassIndex = 1;
		}
		else if (LIGHT_DESC::TYPE_POINT == m_LightDesc.eType)
		{
			if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
				return E_FAIL;
			if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_LightDesc.fRange, sizeof(_float))))
				return E_FAIL;
    
			iPassIndex = 2;
		}
		else if (LIGHT_DESC::TYPE_SPOT == m_LightDesc.eType)
        {
            if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
                return E_FAIL;
            if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
                return E_FAIL;
            if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_LightDesc.fRange, sizeof(_float))))
                return E_FAIL;
            if (FAILED(pShader->Bind_RawValue("g_fConeAngle", &m_LightDesc.fConeAngle, sizeof(_float))))
                return E_FAIL;
            if (FAILED(pShader->Bind_RawValue("g_fAttenuation", &m_LightDesc.fAttenuation, sizeof(_float))))
                return E_FAIL;
            iPassIndex = 3;
        }
    
		if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
			return E_FAIL;
        if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
            return E_FAIL;
		if (FAILED(pShader->Begin(iPassIndex)))
			return E_FAIL;
	}
    
	return pVIBuffer->Render();
}

CLight* CLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHT_DESC& LightDesc)
{
    CLight* pInstance = new CLight(pDevice, pContext);
    if (FAILED(pInstance->Initialize(LightDesc)))
    {
        MSG_BOX("Created Light:Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CLight::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
