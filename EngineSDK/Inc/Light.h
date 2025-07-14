#pragma once
#include"Base.h"
BEGIN(Engine)
class CLight final :public CBase
{
private:
	explicit CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight()=default;

public:
	const LIGHT_DESC* Get_LightDesc() const
	{
		return & m_LightDesc;
	}

	void Set_LightDesc(_vector Pos)
	{
		m_LightDesc.vPosition = { Pos.m128_f32[0], Pos.m128_f32[1] + 1.f, Pos.m128_f32[2], 1.f };
	}

	void Set_PlayerLightDesc(_vector Pos)
	{
		m_LightDesc.vPosition = { Pos.m128_f32[0], Pos.m128_f32[1] + 4.1f, Pos.m128_f32[2], 1.f };
	}

	void Set_PlayerDir(_float4 Dir)
	{
		m_LightDesc.vDirection = Dir;
	}


	
	HRESULT Initialize(const LIGHT_DESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

	_bool	Get_LightOnOff()			{ return m_LightDesc.TurnOn_Off; }
	void	Set_LightOnOff(_bool OnOff) { m_LightDesc.TurnOn_Off = OnOff; }

private:
	ID3D11Device*			m_pDevice	= { nullptr };
	ID3D11DeviceContext*	m_pContext	= { nullptr };
	LIGHT_DESC				m_LightDesc	= {};


				

public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const LIGHT_DESC& LightDesc);
	virtual void Free() override;
}
;
END
