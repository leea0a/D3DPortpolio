#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
#include "InnerInventory.h"




BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
class CVIBuffer_Collider;
END


BEGIN(Client)

class CLightStone;

class CProp : public CGameObject
{
	

public:
	enum COLLIDER { COLL_AABB, COLL_AABB2, COLL_END };
	enum PROPTYPE { TYPE_LIGHTER, TYPE_MIRROR, TYPE_HANDLIGHTER, TYPE_COMPASS, TYPE_KAGURA, TYPE_GLOWSTONE,  TYPE_KEY, TYPE_SPHER, TYPE_DYNAMITE, TYPE_LIGHTSTONE, TYPE_DYNAMITE_THROW,
		TYPE_CANDLE, TYPE_CABINET, TYPE_BIGBOX, TYPE_DOOR, TYPE_TELEPORT, TYPE_MAGATAMA, TYPE_ENDPROP, TYPE_STOP, TYPE_LEVEL, TYPE_END};

public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		_float4		vPosition;
		_float3		vScale;
		_float		fAngle;
		_int		iIndex;
		_int		iPropNo;
		_int		iNaviIndex;
		_bool*		pbCandle;

	}PROP_DESC;

protected:
	explicit CProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CProp(const CProp& Prototype);
	virtual ~CProp() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual _int Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	void			InterAction(_float fTimedelta);
	void			Set_PropType(PROPTYPE eType) { 
		m_ePropType = eType;
	}
	PROPTYPE		Get_PropType() { return m_ePropType; }

	virtual void	Set_InterAction(_float fTimeDelta) {};


private:
	_vector	m_vecRayOrigin	= XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	_vector m_vecRayDir{};

protected:
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };

protected:
	_bool					m_bLife = { false };

	PROP_DESC				m_Desc;
	PROPTYPE				m_ePropType = { TYPE_END };
	_float					m_fAccMotionChangeTime = { 0.f };
	CCollider*				m_pColliderCom = { false };
	_bool					m_bInterSect = { false };
	_bool					m_bChooseObj = { false };


	_bool					m_bDoorOpenClose = { false };
	_bool					m_bCabinetIn = { false };

	_bool					m_bCandleOn = { false };


//private:
//	map<_uint,   CLightStone*>	m_mapLightStone;
//	_uint						m_iLightStoneCount;
//
//public:
//	map<_uint,  CLightStone*>	Get_LightStone() { return m_mapLightStone; }
//	void						Set_LightStone( CLightStone* LightStone) { m_mapLightStone.emplace(m_iLightStoneCount, LightStone); Safe_AddRef(LightStone); }




public:
	static vector<CInnerInventory*>*	m_pInventory;
	static void							Set_Inventory(vector<CInnerInventory*>* _Inventory) { m_pInventory = _Inventory; }
	static vector<CInnerInventory*>*	Get_Inventory() { return m_pInventory; }







protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CProp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END
