#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
	strcpy_s(m_szName, pAINode->mName.data);// ���� �����ϸ� �� (�ڽ� )���� �̸��� �����´� 
	memcpy(&m_TransformationMatrix,&pAINode->mTransformation,sizeof(_float4x4));//aiNode�κ��� �ڽ��� ����� �����´� -> �θ� ���ؿ����� �ڽŸ��� ���
	
	
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	//aiNode�� col MAJOR�̹Ƿ� ��ġ�ؼ� ROW MAJOR�� �ٲ���Ѵ�.
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());//�⺻������ �θ��� ����� ���Ұ��̱⶧���� �ϴ� �׵���ķ� ä���. 
	
	m_iParentBoneIndex = iParentBoneIndex;
	
	return S_OK;
}

HRESULT CBone::Initialize( AI_Node* pAINode, _int iParentBoneIndex)
{
	strcpy_s(m_szName, pAINode->m_szName);// ���� �����ϸ� �� (�ڽ� )���� �̸��� �����´� 
	memcpy(&m_TransformationMatrix, &pAINode->m_TransformationMatrix, sizeof(_float4x4));//aiNode�κ��� �ڽ��� ����� �����´� -> �θ� ���ؿ����� �ڽŸ��� ���
	//aiNode�� col MAJOR�̹Ƿ� ��ġ�ؼ� ROW MAJOR�� �ٲ���Ѵ�.

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());//�⺻������ �θ��� ����� ���Ұ��̱⶧���� �ϴ� �׵���ķ� ä���. 

	m_iParentBoneIndex = iParentBoneIndex;

	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	//���� �ֻ��� ��Ʈ ����� , �ϴ� ���� ���� ����� ���� ���´�.
	if (-1 == m_iParentBoneIndex) {
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);
		return;
		// �θ� ����� ��, (�ֻ��� ���) -> �θ� ��Ŀ� �ٸ� ����� �ߺ��ؼ� �������� �ȵȴ�. 
		//�ѹ� ���ϰ� ������, ������ 0�� �θ�� -1�ε����̹Ƿ� ������. ( �Ʒ� �Ŀ� ���ϸ�) 
	}
	//���� ��ȯ ������ ������ ��Ŀ�  �θ��� ���� combined����� ���Ѵ� . -> ������ ���� ������ �θ���� ������Ʈ ���ִϱ� �θ���� �ڽ���  ���ʷ� transformation ����� ������ -> ������ ������ĿϷ� 
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) *
		XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
	
}



CBone* CBone::Create(const aiNode* pAINode, _int iParentBoneIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : CBone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBone* CBone::Create( AI_Node* pAINode, _int iParentBoneIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : CBone bin");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBone* CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
	
	__super::Free();
	
}
