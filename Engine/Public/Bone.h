#pragma once
#include"Base.h"
/* assimp������ ���� ������ Ÿ������ �����Ѵ�.*/
/* aiNode aiBone, aiAniNode*/
#include"AI_Info.h"
BEGIN(Engine)
class CBone final :public CBase
{

private:
	CBone();
	virtual ~CBone() = default;


public:
	const _char* Get_Name() const {
		return m_szName;
	}
	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}
	/* (��Ʈ ������Ʈ�� ����ٴ� ���� ���� ����� ������ �ش� -> �̰ɷ� ��Ʈ�� �𵨿� ��ġ) * (�����̳� ���� �ؼ� -> ���忡 ������ġ) */
	const _float4x4* Get_CombinedTransformationFloat4x4() const {
		return &m_CombinedTransformationMatrix;
	}

	//������ �Լ� // ä��(�� ���� �����) ���� �÷��� �ð������� �����ؼ� ä�����
	void Set_TransformationMatrix(_fmatrix TransformationMatrix)
	{
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

public:
	HRESULT Initialize(const aiNode* pAINode, _int iParentBoneIndex);
	/*for Bin*/
	HRESULT Initialize( AI_Node* pAINode, _int iParentBoneIndex);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);//�� �������� �θ���� �������� ����� ���ؼ� ���� ����� �����ش� . -> �θ��� �ε����κ��� Ž���� �ϰ��� .
private:
	_char		m_szName[MAX_PATH] = {};//���� �̸� �̰ɷ� ���ؼ� �˻��� ����
	
	/* �θ� �������� �������� ������ ���� ��� */
	_float4x4	m_TransformationMatrix = {};

	/* m_Transformation* Parent's  m_CombinedTransformationMatrix */

	_float4x4 m_CombinedTransformationMatrix = {};

	_int	m_iParentBoneIndex = { -1 };
	 

public:
	static CBone* Create(const aiNode* pAINode, _int iParentBoneIndex);
	static CBone* Create( AI_Node* pAINode, _int iParentBoneIndex);
	virtual CBone* Clone();
	virtual void Free() override;
};
END

