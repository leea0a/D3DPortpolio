#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct
	{
		_int	iStartIndex = { -1 };
	}NAVIGATION_DESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& Prototype);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationDataFile);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Update(_fmatrix WorldMatrix);

public:
	_vector			SetUp_OnNavigation(_fvector vPosition);
	_bool			isMove(_fvector vWorldPos);

	/*For Maptool*/
	HRESULT			Add_Cells(TRIANGLE_VERTICES Triangle);
	void			Edit_CurCell(TRIANGLE_VERTICES TriangleNew);
	void			Pop_Cell();

	/*For Transform SlidingVector*/
	_vector			Get_Normal_From_CurCell();
	_vector			Get_Line_To_Slide_From_CurCell(_fvector vObjPos);
	_bool			Check_Near_To_Points(_fvector vObjPos);
	_int			Get_Index()	{ return m_iCurrentIndex; }
	void			Set_Index(_int iCell) { m_iCurrentIndex = iCell; }

	HRESULT			Delete_Cell();

#ifdef _DEBUG
public:
	virtual HRESULT Render();
#endif

private:
	vector<class CCell*>			m_Cells;
	static _float4x4				m_WorldMatrix;
	_int							m_iCurrentIndex = { -1 };



#ifdef _DEBUG
private:
	class CShader*					m_pShader = { nullptr };
#endif

private:
	HRESULT SetUp_Neighbors();
	_float Compute_Height(_int iCellIndex, _fvector vPosition);

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFile);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END