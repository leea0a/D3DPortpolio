#pragma once
#include"Base.h"
#include"AI_Info.h"
BEGIN(Engine)
class CChannel final : public CBase
{


private:
	CChannel();
	virtual ~CChannel() = default;

public: 
	HRESULT Initialize(const aiNodeAnim* pAIChannel, const class CModel* pModel);
	/*FOR BINARY*/
	HRESULT Initialize(const AI_Channel* pAIChannel, const class CModel* pModel);
	void Update_TransformationMatrix(_float fCurrentTrackPosition,_uint* pKeyFrameIndex, const vector<class CBone*>& Bones);
	
	/* For Joining With NextAnimation*/
	
	//�ִϸ��̼� �ٲ� �̾��� Ű�������� �ִϸ��̼ǿ��� ä�κ��� �����Ѵ� .map( ä���� ���ε���, Ű������)-> �̾��� Ű�������� �ִϸ��̼ǿ� ������(�ʱ�ȭ��)  
	void Get_KeyFrameState_From_Channel(KEYFRAME& KeyFrame, _uint& BoneIndexOfThisChannel, _uint KeyFrameIndexForNextAnim = 0); 
	void Set_Start_Channel_KeyFrame(_float fCurrentTrackPosition, _uint* pKeyFrameIndex, const vector<class CBone*>& Bones,KEYFRAME& StartKEyFrame,_uint& BoneIndex);
	_uint Get_BoneIndex() { return m_iBoneIndex; };

private:
	_char					m_szName[MAX_PATH] = {};// ���� �̸��� ����ȭ�Ǿ��ִ�. 
	_uint					m_iNumKeyFrames = {};
	vector<KEYFRAME>		m_KeyFrames;
	_uint					m_iBoneIndex = {}; //���� ���� �̸��� �ƴϱ� �ε����� �̸����� �����ͼ� ������ ���� �� ���� �̻��� ���� �����( �����Ͽ� �������ش�.)
	


public:
	static CChannel* Create(const aiNodeAnim* pAIChannel, const class CModel* pModel);
	static CChannel* Create(const AI_Channel* pAIChannel, const class CModel* pModel);
	virtual void Free() override;
};
END
