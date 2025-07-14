#pragma once
#include"VIBuffer.h"
#include"AI_Info.h"/* for binary*/
#include"Model.h"
BEGIN(Engine)
class CMesh final :public CVIBuffer
{
private: 
	explicit CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex()const {
		return m_iMaterialIndex;
	}
public: 
	/*(for binary NonanimMesh)*/
	//virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, class CModel* pModel, const AI_MeshInfo* pAIMesh, _fmatrix PreTransformMatrix);

	virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, class CModel* pModel, const aiMesh* pAIMesh,_fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, const vector<CBone*>& Bones);
private: 
	_char			m_szName[MAX_PATH] = "";//�޽��� �̸�
	_uint			m_iMaterialIndex = {};
	_uint			m_iNumBones = {};// �ϳ��� �޽ô� �������� ���� �������ִ�. 
	
	/*�� ���� �����ϴ� ��ü ���� �� ��° ������ �� �޽ÿ� ������ �ִ� �ž� ��� ������ �����Ѵ�. */
	vector<_uint>	m_BoneIndices;//�޽ÿ� ������ �ִ� ���� (�𵨿��� �ο���)�ε����� ���ͷ� ���� ->//�ε���0,1,2,3,4,...->���� �� �ε��� ���� -> �θ�� 0  �ڽĻ� 2 3 4......(��ͼ����� �ε����� �Կ��� ���� )(������� �ϸ� ) 
	/*�� ���� ���εǴ� ��������� ai���� �� �̸� ���ؾ� �Ѵ�)*/
	vector<_float4x4> m_BoneOffsetMatrices; 
	//�޽ô� ���� ��Ʈ������ �迭�� ������ �ִٰ� ���̴��� ������ ���� 

	_float4x4		m_BoneMatrices[512]; 
	

public:
	HRESULT Ready_VertexBuffer_For_NonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);//���� �޽ô� �ִ��� �ִ°Ŷ� ���°Ŷ� �����Ѵ�.
	HRESULT Ready_VertexBuffer_For_Anim(const aiMesh* pAIMesh, class CModel* pModel);

	/*FOR BINARY */
	HRESULT Ready_VertexBuffer_For_NonAnim(const  AI_MeshInfo* pAIMesh, _fmatrix PreTransformMatrix);//���� �޽ô� �ִ��� �ִ°Ŷ� ���°Ŷ� �����Ѵ�.
	HRESULT Ready_VertexBuffer_For_Anim(const  AI_MeshInfo* pAIMesh, class CModel* pModel);

public:
	/*�� ���Ͱ� �ƴ϶� ���� �ٰ����ͼ� ���� ���� */
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType,class CModel* pModel, const aiMesh* pAIMesh,_fmatrix PreTransformMatrix);
	/*for binary*/
	//static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, class CModel* pModel, const AI_MeshInfo* pAIMesh, _fmatrix PreTransformMatrix);
	
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;


public:
	HRESULT SetUp_Bones(class CModel* pModel);

private:
	vector<class CBoneNode*>	m_Bones;
	const aiMesh* m_pAIMesh = nullptr;
	FACEINDICES32* m_pIndices = nullptr;

};
END

