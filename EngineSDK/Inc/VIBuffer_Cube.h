#pragma once
#include"VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Cube final:public CVIBuffer
{
private:
	explicit CVIBuffer_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CVIBuffer_Cube(const CVIBuffer_Cube& Prototype);
	virtual ~CVIBuffer_Cube()=default;
	

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Prototype();


public:
	static CVIBuffer_Cube* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;
};
END
