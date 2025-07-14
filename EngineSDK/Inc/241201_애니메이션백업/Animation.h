#pragma once
#include"Base.h"
#include"AI_Info.h"
/* �ִϸ��̼� Ŭ������ �ϳ��� �ִϸ��̼��� ������ */
/* �ִϸ���� �ϳ� �����µ� �ʿ��� �������� ������ �ִϸ��̼��� ������*/
/* �̻��� ä���̶�� �Ҹ���. (ä���� ���̴�) */
/* �� �������� ��ȯ�� ä�ο��� �ذ��Ѵ�*/
/* �� ä��(����)�� ���������� ���� ����� ���Ῡ �� ��ü�� ����� ���� (���� ������ ����� )*/
/* ������ �°� ��ü�Ѵ�*/
/* �� ������ ä�ο��� (�ڽſ� �̸��� ���ε� ���� (�ε�����)ã�ƿͼ�  ������ �°� ���� �����Ѵ�)*/
BEGIN(Engine)
class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;


public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const class CModel* pModel);
	/* for binary*/
	HRESULT Initialize(const AI_Animation* pAIAnimation, const class CModel* pModel);
	_bool Update_TransformationMatrices(const vector<class CBone*>& Bones, _bool isLoop, _float fTimeDelta);


public:
	/*For Anim Change*/
	bool Get_FrameInfo(KEYFRAME& eFrame, _uint ChanennelIndexYouWant); //ã������ ������ �Բ� true, �ƴϸ� false ��ȯ 

	void Store_CurrentAnim_KeyFrame_State(const vector<class CBone*>& Bones, _bool SlerpWhileSlerp);
	void Store_AnimToChange_KeyFrame_State(const vector<class CBone*>& Bones, CAnimation* NextAnim);
	_bool Update_TransformationMatrices_ForMotionChange(const vector<class CBone*>& Bones, _bool isLoop = false, _float fTimeDelta = 0.f);
	_bool Update_TransformMatrix_ForMotionChange(const vector<class CBone*>& Bones, _uint iBoneIndex, KEYFRAME Start, KEYFRAME Last);
	void SetUp_DistanceAnimToAnim(_float fDistance = 0.2) { m_fDistance = fDistance; }
	void Reset_CurrentKeyFrameIndices();
	/*while Slerping*/
	_bool Set_KeyFrames_For_Spline_To_Spline(const vector<class CBone*>& Bones, KEYFRAME Start, KEYFRAME Last, KEYFRAME& Result);
	void Set_TickRatio(_float fSpeed) { m_fTickRatio = fSpeed; }
private:
	_char		m_szName[MAX_PATH] = {};
	_uint		m_iNumChannels = {};
	vector<class CChannel*> m_Channels;

	_float		m_fTickRatio = { 1.f };
	_float		m_fDuration = {};
	_float m_fTickPerSecond = {};
	_float m_fCurrentTrackPosition = {};
	vector<_uint> m_CurrentKeyFrameIndices;//d���� ä���� Ű�������� �� �ִϸ��̼��� �����Ѵ�.


	/* for Motion Change*/
	/* 1. Other animation will use this info  first Key frame of Channels this Anim has */
	map< _uint, KEYFRAME>m_mapKeyFramesOfChannels;
	/* 2. we need these for changing to other Anim*/
	map<_uint, KEYFRAME>  m_mapStartKeyFrames; // ���� �ִϸ��̼��� ���¸� <ä�� �ε���,Ű������>���� ����  
	map<_uint, KEYFRAME>  m_mapLastKeyFrames;
	_float m_KeyFramePosOfStartAnim = { 0 };
	_float m_KeyFramePosOfNextAnim = { 0 };
	_float m_fDistance = { 0.4f };

public:
	static CAnimation* Create(const  aiAnimation* pAIAnimation, const  class CModel* pModel);
	/*for binary*/
	static CAnimation* Create(const AI_Animation* pAIAnimation, const class CModel* pModel);
	virtual CAnimation* Clone();
	virtual void Free() override;
};
END

