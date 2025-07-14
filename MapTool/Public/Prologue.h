#pragma once
#include "Stage.h"
#include "Client_Defines.h"


BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CPrologue final : public CStage
{
private: 
	explicit		 CPrologue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit		 CPrologue(const CPrologue& Prototype);
	virtual			~CPrologue() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual _int	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Components();
	HRESULT			Bind_ShaderResources();

public:
	vector<TRIANGLE_VERTICES> m_vecTriangles;

public:
	static CPrologue*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free()override;

};

END
