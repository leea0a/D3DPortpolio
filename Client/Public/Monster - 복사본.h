#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
#include "Player.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
class CVIBuffer_Collider;
END


BEGIN(Client)
class CMonster : public CGameObject
{
public:
	enum COLLIDER { COLL_AABB, COLL_OBB, COLL_SPHERE, COLL_END };
	enum COLLIDER_MONSTER { COLLM_OBB, COLLM_SPHERE, COLLM_SPHERE2, COLLM_END };

private:
	typedef struct Distance
	{
		_float x;
		_float z;
	}DISTANCE;

	

public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		_float4		vPosition;
		_float3		vScale;
		_float		fAngle;
		_int		iIndex;
		_int		MonsterNo;

	}MONSTER_DESC;

	typedef struct
	{
		_float3 vCenter = {0, 0, 0};
		_float fRadius = { 0.f };
		_float3 vExtents = {0, 0, 0};
		_float3 vRotation = {0.f, 0.f ,0.f};
		
		

	}COLLIDERDESC;

protected:
	explicit CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	CCollider*				m_pColliderCom[COLLM_END] = { nullptr, nullptr };

protected:
	CPlayer*				m_pPlayer = { nullptr };
	_vector					m_PlayerPosition;

	_bool					m_isChange = { true };
	_vector					m_matSpawnPos;

public:
	MONSTER_DESC			m_Desc;
	_float					m_fAccMotionChangeTime = { 0.f};

protected:
	_bool					m_bHidePlayer = { false };

protected:
	virtual void	Monster_Turn(_float fTimeDelta);
	virtual void	Monster_Turn_Back(_float fTimeDelta);

	_vector			ToLookDirection(const _vector& Rotation);



protected:
	virtual HRESULT Ready_Components(COLLIDERDESC* pDesc);
	HRESULT Bind_ShaderResources();

public:
	virtual void	Set_Player(CPlayer* pPlayer);

public:
	static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END
