#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CShader;
class CModel;
class CVIBuffer_Collider;

END


BEGIN(Client)
class CStage : public CGameObject
{
//public:
//	typedef struct :public CGameObject::GAMEOBJECT_DESC
//	{
//		_float4		vPosition;
//		_float3		vScale;
//		_float		fAngle;
//		_int		iIndex;
//		_int		MonsterNo;
//
//	}MONSTER_DESC;

protected:
	explicit CStage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStage(const CStage& Prototype);
	virtual ~CStage() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual _int Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	CTransform*				m_pTransformCom_ForCollider = { nullptr };
	CShader*				m_pShader_For_ColliderCom = { nullptr };
	CVIBuffer_Collider*		m_pVIBuffer_ColliderCom = { nullptr };

public:
	//MONSTER_DESC			m_Desc;
	_float					m_fAccMotionChangeTime = { 0.f};

protected:
	//WIREFRAME for Collider for picking
	ID3D11RasterizerState*	m_CullMode = { nullptr };
	ID3D11RasterizerState*	m_NoneCullMode = { nullptr };

public:
	BoundingBox				m_Box;
	float					m_arrColiderColor[3] = {0.f,1.f,0.f};

protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
protected:
	HRESULT Bind_ShaderResources_For_Collider();

public:
	static CStage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END
