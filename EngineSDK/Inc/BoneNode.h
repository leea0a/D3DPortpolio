#pragma once

/* 뼈. */
#include "Base.h"

BEGIN(Engine)

class CBoneNode final : public CBase
{
private:
	CBoneNode();
	virtual ~CBoneNode() = default;

public:
	const char* Get_Name() const {
		return m_szName;
	}
	// 추가
	const char* Get_ParentName() const {
		if (nullptr == m_pParent)
			return "";
		return m_pParent->Get_Name();
	}
	// 없어도됨
	_uint Get_Depth() const {
		return m_iDepth;
	}

	void Set_OffsetMatrix(_fmatrix OffsetMatrix) {
		XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

	_matrix Get_OffsetMatrix() const {
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}
	// 추가
	XMFLOAT4X4 Get_OffSetMatrixXM() {
		return m_OffsetMatrix;
	}
	// 추가
	_float4x4 Get_OriTransformationMatrix() { return m_OriTransformationMatrix; }
	// 추가
	CBoneNode* Get_Parent() { return m_pParent; }
public:
	HRESULT Initialize(const aiNode* pNode, CBoneNode* pParent);
	HRESULT Bin_Initialize(DATA_BINNODE* pNode); // 추가
	void Invalidate_CombinedTransformationmatrix();
	void Set_FindParent(CBoneNode* pNode); // 추가

private:
	char				m_szName[MAX_PATH] = "";
	char				m_szParentName[MAX_PATH] = ""; // 추가
	CBoneNode*		m_pParent = nullptr;
	_uint				m_iDepth = 0;	// 없어도됨

	/* 기본은 항등, 메시안에서(aiBone) 값을 채워준다. */
	_float4x4			m_OffsetMatrix;
	_float4x4			m_OriTransformationMatrix; // 추가

	_float4x4			m_TransformationMatrix;

	/* m_TransformationMatrix * m_pParent->m_CombinedTransformationMatrix */
	_float4x4			m_CombinedTransformationMatrix;

public:
	static CBoneNode* Create(const aiNode* pNode, CBoneNode* pParent);
	static CBoneNode* Bin_Create(DATA_BINNODE* pNode); // 추가
	virtual void Free() override;
};

END