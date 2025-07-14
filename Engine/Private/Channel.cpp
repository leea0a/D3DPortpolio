#include "Channel.h"
#include"Model.h"
#include"Bone.h"
#include"Animation.h"
CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const aiNodeAnim* pAIChannel, const CModel* pModel)
{
	//ä���� �̸� ( ���� �̸��� ���� -> �̰ɷ� �ε��� �˻��ؼ� ���� ���� ã�� ) 
	strcpy_s(m_szName, pAIChannel->mNodeName.data); //bin
    
	m_iBoneIndex = pModel->Get_BoneIndex(m_szName);//�̸����� �ε��� �޾ƿ��°��� �ظ��ϸ� const �� ����

	//�ִϸ��̼��� duration �߿� ������ �����Ͽ� �������� ���¸� �����س���(�̹� ���� ����� �س���)
	// �ϳ��� ä��(��)����
	// �� ���� �������� ���¸� �������� ������ ������ Ű�� ǥ���� �����Ѵ�.-> 
	// �츮�� ���⼭ �� ���� ������ ���¸� ���� ���������Ͽ�(�������̿� ��ƴ�̾�����)
	// �� ���������� ���� ����� ���� ���� ������ �´� ��������� �����ؾߵȴ�
	//waypoint ������ ���¸� ���� Ű������(����) �� ���� ������(����) ������ �Ÿ��� 1�� 
	//������ ���� ��� ������ ���ִ����� ���� ������  ���� Ű�����ӿ� ���Ͽ� 
	//���� ���¸� ��Ÿ����.

	//�� ������ Ű���� srt�� ���ͷ� �����ϰ� -> �̰� ������ �����Ƿ� ���̸� �����ؼ� �ڿ������� �����̵��� �������Ѵ�.
	
//srt�� �����ؾߵǴµ� srt ������ �ٸ��� �����Ѵ�? ��Ģ������.

	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);
		
	
	_float3 vScale, vPosition;
	_float4 vRotation; //ȸ���� ���ʹϾ��̿��� �÷�4���� �ش� wxyz ������ �����ϹǷ� ��¥�� ������������ ������ �����ؾߵ�

	/* ä���� Ű�����Ӹ����� ���¸� ������ �ִ�-> ����Ʈ������ ��ȯ�� ����� ������ �ִ� */
	/* ���� ������Ʈ�� play �ִϸ޿������� ������Ʈ �Ҷ����� �ִϸ��̼��� Trackposition�� �����ϰ�-> Trackposition��ġ�� ����  */
	/* ä���� Ű������ ��ġ�� �´� ���¸� ��ķ� ����� ä�ΰ� ����� ��(�纻�� Ŭ�� ����)�� �� ����� ������Ʈ �Ѵ� (�θ�� ���� �ڽŸ��� �����)*/
	/* ä���� ����� �ִϸ��̼ǿ��� Ű������ �ε����� ������ -> �ִϸ��� ���� ���� Ű������(������)�� ���������Ͽ� ä���� ����ϴ� ���� ����� ������.*/
 	/* ���: ä���� Ʈ�� �����Ǹ� �ѱ�� ������� ������ִ� �� ��� ���丮�̴�*/
	/* ä���� ������ ����: Ű������ ����(������ �ε���+1), Ű������ ������ ũ���� ���(���ͷ� �и�for ��������) */
	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME KeyFrame = {};
		
		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));//Ű������(mTime)�� 0�̸� ���� ���¶� �������� ����Ǿ�����
			KeyFrame.fKeyFramePosition = pAIChannel->mScalingKeys[i].mTime;  //Ű�������� ��ġ:0�ƴϸ� �ٸ� ������ Ű�������̶� ����(0�̶�� �������� ���)
		}

		if (i < pAIChannel->mNumRotationKeys)//��������
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fKeyFramePosition = pAIChannel->mRotationKeys[i].mTime;//���� ���� (��ġ)(Ű �������� ��ġ)
			//���� ȸ�� ����
		}
		if (i < pAIChannel->mNumPositionKeys)//��������
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fKeyFramePosition = pAIChannel->mPositionKeys[i].mTime;
		}//
		//vRotation, vPosition vScale �� ���� for�� �ۿ� �������� ���������ϱ� if �� ��� ���ϸ� ���� ���� �����Ѵ�.
		// ���� ��Ģ�� �����Ҽ� �ִ� .
		//���� ��ġ ���� 
		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}
	//-> ��� Ű�����ӵ��� �ǵ������� ���Ϳ� �� ��Ҵ� 
	
	return S_OK;
}
HRESULT CChannel::Initialize(const AI_Channel* pAIChannel, const CModel* pModel)
{

	strcpy_s(m_szName, pAIChannel->m_szName);

	 m_iBoneIndex = pModel->Get_BoneIndex(pAIChannel->m_szName);

	m_iNumKeyFrames = max(pAIChannel->m_NumScalingKeys, pAIChannel->m_NumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->m_NumPositionKeys);
	
	_float3 vScale, vPosition;
	_float4 vRotation;

	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME KeyFrame = {};

		if (i < pAIChannel->m_NumScalingKeys)//i�� �� �����ϸ� ��ü �������� ũ�� �� ���� ���·� ����
		{
			memcpy(&vScale, &pAIChannel->m_ScalingKeys[i].m_Value, sizeof(_float3));
			KeyFrame.fKeyFramePosition = pAIChannel->m_ScalingKeys[i].KeyFramePos;
		}

		if (i < pAIChannel->m_NumRotationKeys)
		{
			vRotation.x = pAIChannel->m_RotationKeys[i].m_Value.x;
			vRotation.y = pAIChannel->m_RotationKeys[i].m_Value.y;
			vRotation.z = pAIChannel->m_RotationKeys[i].m_Value.z;
			vRotation.w = pAIChannel->m_RotationKeys[i].m_Value.w;
			KeyFrame.fKeyFramePosition = pAIChannel->m_RotationKeys[i].KeyFramePos;//���� ���� (��ġ)(Ű �������� ��ġ)
			//���� ȸ�� ����
		}
 
		if (i < pAIChannel->m_NumPositionKeys)//��������
		{
			memcpy(&vPosition, &pAIChannel->m_PositionKeys[i].m_Value, sizeof(_float3));
			KeyFrame.fKeyFramePosition = pAIChannel->m_PositionKeys[i].KeyFramePos;
		}
		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}

	/*if (false == strcmp(m_szName, "Bip001"))
	{
		_float3 InitialPosition = m_KeyFrames[0].vPosition;
		for (size_t i = 0; i < m_KeyFrames.size(); ++i)
		{
			m_KeyFrames[i].vPosition = InitialPosition;
		}
		
	}*/
 
 
  
 	return S_OK;
}
//�������� �ؼ� ���� �ڽ��� ���� ��� ������Ʈ
void CChannel::Update_TransformationMatrix(_float fCurrentTrackPosition, _uint* pKeyFrameIndex,const vector<class CBone*>& Bones)
{
	
	if (0.f == fCurrentTrackPosition) //�̰� �ִϸ��̼ǿ��� ä�� ������, loop �ٵ������� 0���� ����Ŵ�. �ε����� 0���� ������
		*pKeyFrameIndex = 0;

	//fCurerntTrackTime��  ���� �� ��ġ->������ �������̿��� ���� ��ġ�� ������ ������ ���Ͽ� �����Ѵ�. 
	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;


	//������ Ű�������� �������� ������ Ű������ ���� �����Ѵ�. 
	if (fCurrentTrackPosition >= LastKeyFrame.fKeyFramePosition)  //���� ��ġ ������ ������ Ű�������� ��ġ(����)����������? ������ Ű���������� ����
	{
		vScale = LastKeyFrame.vScale;
		vRotation = LastKeyFrame.vRotation;
		vPosition = LastKeyFrame.vPosition;
	}

	else
	{
		//while�� �������� �������� fTimeDelta�� �ʹ� Ŀ�� ����Ű�������� �ٴٴٴٴ��� Ű���������� �̵�-> ratio�� Ŀ�� 
		//������ ����� �Ͼ�� Ʈ��ŷ �������� ���� ���� ������ �������� �̵��ϵ��� loop���� ���� ���������� �̵� 
		while (fCurrentTrackPosition >= m_KeyFrames[*pKeyFrameIndex + 1].fKeyFramePosition)
			++*pKeyFrameIndex;

		_float			fRatio = (fCurrentTrackPosition - m_KeyFrames[*pKeyFrameIndex].fKeyFramePosition) /
			(m_KeyFrames[*pKeyFrameIndex + 1].fKeyFramePosition - m_KeyFrames[*pKeyFrameIndex].fKeyFramePosition);
		// ��-----------��
		// ��----�� 
		//-> �� ������ ���ϴ� �����̴� 

		_float3			vLeftScale, vRightScale;
		_float4			vLeftRotation, vRightRotation;
		_float3			vLeftPosition, vRightPosition;

		vLeftScale = m_KeyFrames[*pKeyFrameIndex].vScale;
		vLeftRotation = m_KeyFrames[*pKeyFrameIndex].vRotation;
		vLeftPosition = m_KeyFrames[*pKeyFrameIndex].vPosition;

		vRightScale = m_KeyFrames[ *pKeyFrameIndex + 1].vScale;
		vRightRotation = m_KeyFrames[*pKeyFrameIndex + 1].vRotation;
		vRightPosition = m_KeyFrames[*pKeyFrameIndex + 1].vPosition;

		//�����̶� �ΰ���(������) ���� ���� ������ ���������ִ�   �Լ� 
		XMStoreFloat3(&vScale, XMVectorLerp(XMLoadFloat3(&vLeftScale), XMLoadFloat3(&vRightScale), fRatio));
		XMStoreFloat4(&vRotation, XMQuaternionSlerp(XMLoadFloat4(&vLeftRotation), XMLoadFloat4(&vRightRotation), fRatio));
		XMStoreFloat3(&vPosition, XMVectorLerp(XMLoadFloat3(&vLeftPosition), XMLoadFloat3(&vRightPosition), fRatio));
	}
	//�ش��ϴ� ���� ���� ����� �ٽ� ���� (���� ������ ) //ũ���� ������ �˾Ƽ� ������(���ʹϾ������ ��� �־ߵ� ȸ����)
	Bones[m_iBoneIndex]->Set_TransformationMatrix(XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMVectorSetW(XMLoadFloat3(&vPosition), 1.f)));
}


void CChannel::Get_KeyFrameState_From_Channel(KEYFRAME& KeyFrame,_uint& BoneIndexOfThisChannel, _uint KeyFrameIndexForNextAnim )
{
	KeyFrame = m_KeyFrames[KeyFrameIndexForNextAnim];
	BoneIndexOfThisChannel = m_iBoneIndex;
}
//���� �ִϸ��̼��� �߰� ������ ���ؼ� �ִϸ��̼ǿ��� ������������ �����Ѵ�. 
void CChannel::Set_Start_Channel_KeyFrame(_float fCurrentTrackPosition, _uint* pKeyFrameIndex, const vector<class CBone*>& Bones, KEYFRAME& StartKeyFrame,_uint& BoneIndex)
{
	if (0.f == fCurrentTrackPosition)
		*pKeyFrameIndex = 0;

	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	
	//������ Ű�������� �������� ������ Ű������ ���� �����Ѵ�. 
	if (fCurrentTrackPosition >= LastKeyFrame.fKeyFramePosition)  //���� ��ġ ������ ������ Ű�������� ��ġ(����)����������? ������ Ű���������� ����
	{
		StartKeyFrame.vScale = LastKeyFrame.vScale;
		StartKeyFrame.vRotation = LastKeyFrame.vRotation;
		StartKeyFrame.vPosition = LastKeyFrame.vPosition;
		BoneIndex = m_iBoneIndex;
	}

	else
	{
		//while�� �������� �������� fTimeDelta�� �ʹ� Ŀ�� ����Ű�������� �ٴٴٴٴ��� Ű���������� �̵�-> ratio�� Ŀ�� 
		//������ ����� �Ͼ�� Ʈ��ŷ �������� ���� ���� ������ �������� �̵��ϵ��� loop���� ���� ���������� �̵� 
		while (fCurrentTrackPosition >= m_KeyFrames[*pKeyFrameIndex + 1].fKeyFramePosition)
			++*pKeyFrameIndex;

		_float			fRatio = (fCurrentTrackPosition - m_KeyFrames[*pKeyFrameIndex].fKeyFramePosition) /
			(m_KeyFrames[*pKeyFrameIndex + 1].fKeyFramePosition - m_KeyFrames[*pKeyFrameIndex].fKeyFramePosition);
		// ��-----------��
		// ��----�� 
		//-> �� ������ ���ϴ� �����̴� 

		_float3			vLeftScale, vRightScale;
		_float4			vLeftRotation, vRightRotation;
		_float3			vLeftPosition, vRightPosition;

		vLeftScale = m_KeyFrames[*pKeyFrameIndex].vScale;
		vLeftRotation = m_KeyFrames[*pKeyFrameIndex].vRotation;
		vLeftPosition = m_KeyFrames[*pKeyFrameIndex].vPosition;

		vRightScale = m_KeyFrames[*pKeyFrameIndex + 1].vScale;
		vRightRotation = m_KeyFrames[*pKeyFrameIndex + 1].vRotation;
		vRightPosition = m_KeyFrames[*pKeyFrameIndex + 1].vPosition;


		XMStoreFloat3(&StartKeyFrame.vScale, XMVectorLerp(XMLoadFloat3(&vLeftScale), XMLoadFloat3(&vRightScale), fRatio));
		XMStoreFloat4(&StartKeyFrame.vRotation, XMQuaternionSlerp(XMLoadFloat4(&vLeftRotation), XMLoadFloat4(&vRightRotation), fRatio));
		XMStoreFloat3(&StartKeyFrame.vPosition, XMVectorLerp(XMLoadFloat3(&vLeftPosition), XMLoadFloat3(&vRightPosition), fRatio));
		BoneIndex = m_iBoneIndex;
	}
}


CChannel* CChannel::Create(const aiNodeAnim* pAIChannel, const CModel* pModel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CChannel* CChannel::Create(const AI_Channel* pAIChannel, const CModel* pModel)
{
	CChannel* pInstance = new CChannel();
	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX("Failed to Created: CChanel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChannel::Free()
{
	
	__super::Free();
}
