#include "Animation.h"
#include"Channel.h"
#include"Bone.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& Prototype)
	: m_iNumChannels{ Prototype.m_iNumChannels }
	, m_Channels{ Prototype.m_Channels }
	, m_fDuration{ Prototype.m_fDuration }
	, m_fTickPerSecond{ Prototype.m_fTickPerSecond }
	, m_fCurrentTrackPosition{ Prototype.m_fCurrentTrackPosition }
	, m_CurrentKeyFrameIndices{ Prototype.m_CurrentKeyFrameIndices }
	//ä���� Ű������ �ε����� �ִ��� �����Ѵ�. 
{
	//ä���� �ּ� ����(��(ä��)�� ��ü���� �ٰ����ٰ�? �ȵ� �ʹ�����
	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);

	for (auto Channel : m_Channels)
	{
		_uint iBoneIndex = { 0 };
		KEYFRAME eKeyFrame = {};
		Channel->Get_KeyFrameState_From_Channel(eKeyFrame, iBoneIndex);
		m_mapKeyFramesOfChannels.insert({ iBoneIndex,eKeyFrame });
	}

	strcpy_s(m_szName, Prototype.m_szName);
}
HRESULT CAnimation::Initialize(const aiAnimation* pAIAnimation, const CModel* pModel)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);  //�ִϸ��̼��� �̸��� �����Ѵ� idle attack....

	//�� �ִϸ��̼��� ��ü �÷��� ���� ( �ð�) �� �����´� 
	m_fDuration = pAIAnimation->mDuration;
	/* �ִϸ��̼� ��� �ӵ��� �����´�*/
	m_fTickPerSecond = pAIAnimation->mTicksPerSecond;

	/* �� �ִϸ��̼�(���, ���� etc)�� ��Ʈ���ؾ��ϴ� ��(Channel)���� ���� */
	m_iNumChannels = pAIAnimation->mNumChannels;

	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	/* ������ ������ ���������� �ִ��� ��Ʈ���ϴ� ������ ��������*/
	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		/* ai�� �������� �ǰ�ü channel�� �Ѱܼ� ��������*/
		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	for (auto Channel : m_Channels)
	{
		_uint iBoneIndex = { 0 };
		KEYFRAME eKeyFrame = {};
		Channel->Get_KeyFrameState_From_Channel(eKeyFrame, iBoneIndex);
		m_mapKeyFramesOfChannels.insert({ iBoneIndex,eKeyFrame });
	}

	return S_OK;
}

HRESULT CAnimation::Initialize(const AI_Animation* pAIAnimation, const CModel* pModel)
{
	strcpy_s(m_szName, pAIAnimation->m_szName);
	m_fDuration = pAIAnimation->m_fDuration;
	m_fTickPerSecond = pAIAnimation->m_fTickPerSecond;
	m_iNumChannels = pAIAnimation->m_iNumChannels;
	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	for (int i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(&pAIAnimation->m_vecChannels[i], pModel);
		if (nullptr == pChannel)
			return E_FAIL;
		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

_bool CAnimation::Update_TransformationMatrices(const vector<class CBone*>& Bones, _bool isLoop, _float fTimeDelta)
{
	/* �ִϸ��̼� ��ü �÷��̱����� ���� ����*///->�÷��� �ӵ��� ftimedelta�� ���� ���� �����켭 ����
	//-> ���� ������ ������
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta * m_fTickRatio;

	if (true == isLoop && m_fCurrentTrackPosition >= m_fDuration)
	{
		m_fCurrentTrackPosition = 0.f;
		//������� ���Ÿ��� (Ʈ��ŷ Ÿ��) �� �ٽ� 0���� �ǵ����� 
		//�ٵ� �׷��� �� ���� �ش��ϴ� ä���� Ű������ �ε����� 0���� �������ߵǴµ�....
	}
	else if (m_fCurrentTrackPosition >= m_fDuration) //������ �ƴϳ� ?!
		return true;  //�ִ� ������ 



	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		if (i == 50)
			int a = 0;
		m_Channels[i]->Update_TransformationMatrix(m_fCurrentTrackPosition, &m_CurrentKeyFrameIndices[i], Bones);
		//-> Ű������ ���� �ٲ��� �Ǵϱ� �����ͷ� Ű������ �ε������� �ٲ��ش�. 
	}

	return false; //�ִ� �ȳ����� ...
}

bool CAnimation::Get_FrameInfo(KEYFRAME& eFrame, _uint ChanennelIndexYouWant)
{
	//��ġ�� Ű�����Ӹ� �ٲܲ��� ����?
	auto iter = m_mapKeyFramesOfChannels.find(ChanennelIndexYouWant);

	if (m_mapKeyFramesOfChannels.end() == iter)
		return false;

	/* �� ä���� next keyframe ã�����ϱ� ��ȯ -> transform �Ҷ� ���� ������ nextkeyframe���� ���ڴ� */
	eFrame = (*iter).second;
	return true;
}

void CAnimation::Store_CurrentAnim_KeyFrame_State(const vector<class CBone*>& Bones, _bool SlerpWhileSlerp)
{

	if (SlerpWhileSlerp)
	{

		if (m_mapStartKeyFrames.empty())
		{
			MSG_BOX("Store CurrentAnimState Before Changing Motion");
			return;
		}

		for (auto& FramePair : m_mapStartKeyFrames)
		{
			KEYFRAME keyFrame = {};
			if (m_mapLastKeyFrames.end() != m_mapLastKeyFrames.find(FramePair.first)) {

				_uint iBoneIndex = FramePair.first;
				Set_KeyFrames_For_Spline_To_Spline(Bones, m_mapStartKeyFrames[iBoneIndex], m_mapLastKeyFrames[iBoneIndex], keyFrame);
				m_mapStartKeyFrames[FramePair.first] = keyFrame;
			}
		}


	}
	else
	{
		//�������� 
		m_mapStartKeyFrames.clear();

		m_KeyFramePosOfStartAnim = m_fCurrentTrackPosition;

		for (size_t i = 0; i < m_iNumChannels; i++)
		{
			KEYFRAME keyFrame = {};
			_uint BoneIndex = { 0 };
			m_Channels[i]->Set_Start_Channel_KeyFrame(m_fCurrentTrackPosition, &m_CurrentKeyFrameIndices[i], Bones, keyFrame, BoneIndex);
			m_mapStartKeyFrames.emplace(BoneIndex, keyFrame);
		}
	}
}

void CAnimation::Store_AnimToChange_KeyFrame_State(const vector<class CBone*>& Bones, CAnimation* NextAnim)
{

	if (m_mapStartKeyFrames.empty())
	{
		MSG_BOX("Store CurrentAnimState Before Changing Motion");
		return;
	}

	m_mapLastKeyFrames.clear();
	m_KeyFramePosOfNextAnim = m_fDistance + m_KeyFramePosOfStartAnim;

	for (auto& CurKeyFramePair : m_mapStartKeyFrames)
	{
		KEYFRAME KeyFrame = {};
		_uint BoneIndex = { 0 };
		if (NextAnim->Get_FrameInfo(KeyFrame, CurKeyFramePair.first)) //  ���� �ִϸ��̼ǰ� ��ġ�� �����϶��� �߰�����.
			m_mapLastKeyFrames.emplace(CurKeyFramePair.first, KeyFrame);

	}
	int a = 0;
}

_bool CAnimation::Update_TransformationMatrices_ForMotionChange(const vector<class CBone*>& Bones, _bool isLoop, _float fTimeDelta)
{
	m_szName;

	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta * m_fTickRatio;

	_bool IsFinished = true;
	_bool bReturn = true;
	for (auto& LastKeyFramePair : m_mapLastKeyFrames)
	{
		_uint iBoneKey = LastKeyFramePair.first;
		IsFinished = Update_TransformMatrix_ForMotionChange(Bones, iBoneKey, m_mapStartKeyFrames[iBoneKey], LastKeyFramePair.second);
		if (false == IsFinished)
			bReturn = false;
	}
	if (bReturn == true)//�ִϸ��̼ǳ�
	{
		m_KeyFramePosOfStartAnim = 0;
		m_KeyFramePosOfNextAnim = 0;
		m_mapStartKeyFrames.clear();
		m_mapLastKeyFrames.clear();
		m_fCurrentTrackPosition = 0.f;
		Reset_CurrentKeyFrameIndices();
	}


	return bReturn;
}

_bool CAnimation::Update_TransformMatrix_ForMotionChange(const vector<class CBone*>& Bones, _uint iBoneIndex, KEYFRAME Start, KEYFRAME Last)
{
	//if (0.f == fCurrentTrackPosition) //�̰� �ִϸ��̼ǿ��� ä�� ������, loop �ٵ������� 0���� ����Ŵ�. �ε����� 0���� ������
	//	*pKeyFrameIndex = 0;


		//�ð��� �����ų�, �����ӵ������ ���� ���ϸ� �ٷ� ���� �ִϸ��̼����� �Ѿ
	if ((m_fCurrentTrackPosition - m_KeyFramePosOfStartAnim) > m_fDistance)
		return true; //������ true 

	_float fRatio = (m_fCurrentTrackPosition - m_KeyFramePosOfStartAnim) / m_fDistance;

	// ��-----------��
	// ��----�� 
	//-> �� ������ ���ϴ� �����̴� 

	_float3 vScale;
	_float3 vPosition;
	_float4 vRotation;

	_float3			vLeftScale, vRightScale;
	_float4			vLeftRotation, vRightRotation;
	_float3			vLeftPosition, vRightPosition;

	vLeftScale = Start.vScale;
	vLeftRotation = Start.vRotation;
	vLeftPosition = Start.vPosition;

	vRightScale = Last.vScale;
	vRightRotation = Last.vRotation;
	vRightPosition = Last.vPosition;

	//�����̶� �ΰ���(������) ���� ���� ������ ���������ִ�   �Լ� 
	XMStoreFloat3(&vScale, XMVectorLerp(XMLoadFloat3(&vLeftScale), XMLoadFloat3(&vRightScale), fRatio));
	XMStoreFloat4(&vRotation, XMQuaternionSlerp(XMLoadFloat4(&vLeftRotation), XMLoadFloat4(&vRightRotation), fRatio));
	XMStoreFloat3(&vPosition, XMVectorLerp(XMLoadFloat3(&vLeftPosition), XMLoadFloat3(&vRightPosition), fRatio));

	Bones[iBoneIndex]->Set_TransformationMatrix(XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMVectorSetW(XMLoadFloat3(&vPosition), 1.f)));
	return false;
}

void CAnimation::Reset_CurrentKeyFrameIndices()
{
	m_fCurrentTrackPosition = 0.f;
	for (auto& index : m_CurrentKeyFrameIndices)
		index = 0;
}

_bool CAnimation::Set_KeyFrames_For_Spline_To_Spline(const vector<class CBone*>& Bones, KEYFRAME Start, KEYFRAME Last, KEYFRAME& Result)
{
	//������ Ű�������� �������� ������ Ű������ ���� �����Ѵ�. 
	if ((m_fCurrentTrackPosition - m_KeyFramePosOfStartAnim) > m_fDistance) //���� ��ġ ������ ������ Ű�������� ��ġ(����)����������? ������ Ű���������� ����
	{
		Result.vScale = Last.vScale;
		Result.vRotation = Last.vRotation;
		Result.vPosition = Last.vPosition;
	}

	else
	{


		_float			fRatio = (m_fCurrentTrackPosition - m_KeyFramePosOfStartAnim) / m_fDistance;
		// ��-----------��
		// ��----�� 
		//-> �� ������ ���ϴ� �����̴� 

		_float3			vLeftScale, vRightScale;
		_float4			vLeftRotation, vRightRotation;
		_float3			vLeftPosition, vRightPosition;

		vLeftScale = Start.vScale;
		vLeftRotation = Start.vRotation;
		vLeftPosition = Start.vPosition;

		vRightScale = Last.vScale;
		vRightRotation = Last.vRotation;
		vRightPosition = Last.vPosition;


		XMStoreFloat3(&Result.vScale, XMVectorLerp(XMLoadFloat3(&vLeftScale), XMLoadFloat3(&vRightScale), fRatio));
		XMStoreFloat4(&Result.vRotation, XMQuaternionSlerp(XMLoadFloat4(&vLeftRotation), XMLoadFloat4(&vRightRotation), fRatio));
		XMStoreFloat3(&Result.vPosition, XMVectorLerp(XMLoadFloat3(&vLeftPosition), XMLoadFloat3(&vRightPosition), fRatio));
	}

	return true;
}

//�𵨿� �̸����� �ε��� �������� �Լ������� -> �̰ɷ� ���� ��ȯ�� ã��
CAnimation* CAnimation::Create(const aiAnimation* pAIAnimation, const  CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAnimation* CAnimation::Create(const AI_Animation* pAIAnimation, const CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();
	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed ToCreate: CAnimation bin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	__super::Free();

	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}
