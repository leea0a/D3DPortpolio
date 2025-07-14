#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
//#include "InnerInventory.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
class CVIBuffer_Collider;
END


BEGIN(Client)
class CProp : public CGameObject
{
public:
	enum COLLIDER { COLL_AABB, COLL_OBB, COLL_END };
	enum PROPTYPE { TYPE_LIGHTER, TYPE_MIRROR, TYPE_HANDLIGHTER, TYPE_COMPASS, TYPE_KAGURA, TYPE_GLOWSTONE, TYPE_KEY, TYPE_SPHER, TYPE_DYNAMITE, 
		TYPE_CANDLE, TYPE_CABINET, TYPE_BIGBOX, TYPE_DOOR, TYPE_TELEPORT, TYPE_END};

public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		_float4		vPosition;
		_float3		vScale;
		_float		fAngle;
		_int		iIndex;
		_int		iPropNo;
		_int		iNaviIndex;

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
	void			Set_PropType(PROPTYPE eType) { m_ePropType = eType; }
	PROPTYPE		Get_PropType() { return m_ePropType; }

	virtual void	Set_InterAction(_float fTimeDelta) {};

public:
	void Update_Transform_By_Desc(PROP_DESC Desc);
	void Update_Transform_Go_Straight(const _float fTimeDelta);
	void Update_Transform_Go_Backward(const _float fTimeDelta);

protected:
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };

	CShader* m_pShader_For_ColliderCom = { nullptr };
	CVIBuffer_Collider* m_pVIBuffer_ColliderCom = { nullptr };

	ID3D11RasterizerState* m_CullMode = { nullptr };
	ID3D11RasterizerState* m_NoneCullMode = { nullptr };

public:
	PROP_DESC				m_Desc;
	float m_arrColiderColor[3] = { 0.f,1.f,0.f };

public:
	BoundingBox m_Box;

protected:

	PROPTYPE				m_ePropType = { TYPE_END };
	_float					m_fAccMotionChangeTime = { 0.f };
	CCollider*				m_pColliderCom = { false };
	_bool					m_bInterSect = { false };
	_bool					m_bChooseObj = { false };

	_bool					m_bDoorOpenClose = { false };
	_bool					m_bCabinetIn = { false };

public:
	//static vector<CInnerInventory*>*	m_pInventory;
	//static void							Set_Inventory(vector<CInnerInventory*>* _Inventory) { m_pInventory = _Inventory; }
	//static vector<CInnerInventory*>*	Get_Inventory() { return m_pInventory; }

protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	HRESULT Bind_ShaderResources_For_Collider();

public:
	static CProp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END
