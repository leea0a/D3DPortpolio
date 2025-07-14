#pragma once
#include"Base.h"
BEGIN(Engine)
class CLight_Manager final :public CBase
{
private:
	explicit CLight_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC* Get_LightDesc(_uint iIndex)const;

	HRESULT Initialize();
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Delete_Light();


	HRESULT Add_PlayerLight(_int _index, const LIGHT_DESC& LightDesc);
	HRESULT Update_PlayerPos(_int _index, _vector Pos);
	HRESULT Delete_PlayerLight(_int _index);
	HRESULT PlayerLight_TurnOnOff(_int _index, _bool OnOff);
	HRESULT Delete_PlayerAllLight();

	HRESULT	Update_PlayerDir(_int _index, _float4 Dir);


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

	HRESULT	Add_MagatamaLight(LIGHT_DESC LightDesc, _int _index);
	HRESULT Update_MagatamaPos(_int _index, _vector Pos);
	HRESULT Delete_MagamaLight(_int _index);
	HRESULT MagatamaLight_TurnOnOff(_int _index, _bool OnOff);

	_int	Get_PlayerLightCount();
	_int	Get_StoneLightCount() { return m_StoneLights.size(); }

	
	HRESULT	Add_DynamiteLight(LIGHT_DESC LightDesc, _int _index);
	HRESULT DynamiteLight_TurnOnOff(_int _index, _bool OnOff);





	//void	Update_Pos(_int _index, _vector Pos);

	/* 촛불은 인덱스 값으로 받아와서 해당 인덱스에 불끄고 지우기 할수 있게 vector값으로 넣는게 좋을거같다.*/

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = {nullptr};
	list<class CLight*>		m_Lights;
	map<_int, class CLight*>	m_PlayerLights;
	map<_int, class CLight*>	m_CandleLights;
	map<_int, class CLight*>	m_MonsterLights;
	map<_uint, class CLight*>	m_StoneLights;
	map<_uint, class CLight*>	m_MagatamaLights;
	map<_uint, class CLight*>	m_DynamiteLights;

public:
	static CLight_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};


END

