#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CLightStone final : public CProp
{
private: 
	explicit		 CLightStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CLightStone(const CLightStone& Prototype);
	virtual			~CLightStone() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual _int	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Components(void* pArg);
	HRESULT			Bind_ShaderResources();
	void			Drop(_float fTimeDelta);

public:
	void			Delete_Light();


public:
	static CLightStone*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

private:
	_float4							m_f4OriginPos{};
	_float3							m_fCalPos{};
	_float							m_fTime = { 0.0f };

	_uint							m_iLightStoneIndex = { 0 };
	_bool							m_bPutSound = { false };
};

END
