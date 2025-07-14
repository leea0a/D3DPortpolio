#pragma once

/* ��. */
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
	// �߰�
	const char* Get_ParentName() const {
		if (nullptr == m_pParent)
			return "";
		return m_pParent->Get_Name();
	}
	// �����
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
	// �߰�
	XMFLOAT4X4 Get_OffSetMatrixXM() {
		return m_OffsetMatrix;
	}
	// �߰�
	_float4x4 Get_OriTransformationMatrix() { return m_OriTransformationMatrix; }
	// �߰�
	CBoneNode* Get_Parent() { return m_pParent; }
public:
	HRESULT Initialize(const aiNode* pNode, CBoneNode* pParent);
	HRESULT Bin_Initialize(DATA_BINNODE* pNode); // �߰�
	void Invalidate_CombinedTransformationmatrix();
	void Set_FindParent(CBoneNode* pNode); // �߰�

private:
	char				m_szName[MAX_PATH] = "";
	char				m_szParentName[MAX_PATH] = ""; // �߰�
	CBoneNode*		m_pParent = nullptr;
	_uint				m_iDepth = 0;	// �����

	/* �⺻�� �׵�, �޽þȿ���(aiBone) ���� ä���ش�. */
	_float4x4			m_OffsetMatrix;
	_float4x4			m_OriTransformationMatrix; // �߰�

	_float4x4			m_TransformationMatrix;

	/* m_TransformationMatrix * m_pParent->m_CombinedTransformationMatrix */
	_float4x4			m_CombinedTransformationMatrix;

public:
	static CBoneNode* Create(const aiNode* pNode, CBoneNode* pParent);
	static CBoneNode* Bin_Create(DATA_BINNODE* pNode); // �߰�
	virtual void Free() override;
};

END