#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "Dynamite_Use.h"

BEGIN(Engine)
class CVIBuffer_Particle_Point;
class CTexture;
class CShader;
END


BEGIN(Client)

class CParticle_SparkLv2 final : public CGameObject
{
private:
	CParticle_SparkLv2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_SparkLv2(const CParticle_SparkLv2& Prototype);
	virtual ~CParticle_SparkLv2() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual _int Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:	
	CShader*					m_pShaderCom = { nullptr };	
	CTexture*					m_pTextureCom = { nullptr };
	CVIBuffer_Particle_Point*	m_pVIBufferCom = {nullptr};



private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	CTransform* Get_Transform() { return m_pTransformCom; }
	CVIBuffer_Particle_Instance* Get_SparkViBuffer() { return m_pVIBufferCom; }
	void Set_Loop(_bool _Loop) { m_pVIBufferCom->Set_Loop(_Loop); }
	void Set_Show(_bool _Show) { m_pVIBufferCom->Set_Show(_Show); }
	_bool Get_Show() { return m_pVIBufferCom->Get_Show(); }

public:
	static CParticle_SparkLv2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END