#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Camera_POV.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CSky final : public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{

	}BACKGROUND_DESC;
private:
	CSky(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSky(const CSky& Prototype);
	virtual ~CSky() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual _int Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void			Set_Camera(CCamera_POV* _pCamera)	{ m_pCamera = _pCamera; Safe_AddRef(m_pCamera); }
	CCamera_POV*	Get_Camera()						{ return m_pCamera; }

private:
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Cube*			m_pVIBufferCom = { nullptr };


	CCamera_POV*			m_pCamera = { nullptr };


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CSky* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END