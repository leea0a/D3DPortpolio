#include "Mesh.h"
#include "Shader.h"
#include "Bone.h"
#include "BoneNode.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVIBuffer{pDevice, pContext}
{
}

//aimash _ For Anim 
//������ �ִϸ��̼� ������ ���� ���� ����ü�� ���� �� �ؾ��� 
//HRESULT CMesh::Initialize_Prototype(CModel::TYPE eModelType, class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
//{
//
//	m_iMaterialIndex = pAIMesh->mMaterialIndex;//�ش� �޽ÿ� �����Ǵ� ���׸��� �ε���
////    //�޽� ������ �����ͷ� �ε��ϸ� �޽� �������� �̾ƿ� ���ִ� (�ϳ��� ���� �������� �Ž÷� �и��Ǿ� �����Ѵ�)
//
//	strcpy_s(m_szName, pAIMesh->mName.data);//�޽ÿ��� ���� �̸��� �����ϸ� �̴� �߿��� �����̴�.
//
//	m_iNumVertexBuffers = 1;//���۴� ���� 1������
//	m_iNumVertices = pAIMesh->mNumVertices; //�޽� ���ؽ��� ������ �����´� 
//	m_iIndexStride = sizeof(_uint);//���� ������ �� ���� ���� �����ϱ�int ������ �ε����� ������ �˳��ϰ� ��´�.
//	m_iNumIndices = pAIMesh->mNumFaces * 3;  //�޽��� �ε��� ���� ��ſ� ������(�ﰢ��)�� ������ �����Ѵ�. �ű⿡ 3�� ���ϸ� �ε����� ������ ��ġ�ϹǷ� �̸� �ε��� ������ ��´�.
//	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
//	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//
//#pragma region VERTEX_BUFFER	
//	
//
//	HRESULT hr =CModel::TYPE_NONANIM == eModelType ? Ready_VertexBuffer_For_NonAnim(pAIMesh, PreTransformMatrix) : Ready_VertexBuffer_For_Anim(pAIMesh, pModel);
//	
//	if (FAILED(hr))
//		return E_FAIL;
//	
//#pragma endregion
//
//#pragma region INDEX_BUFFER
//	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
//
//	/* �������۸� �� ����Ʈ �Ҵ��ұ��? */
//	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
//	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//
//	_uint* pIndices = new _uint[m_iNumIndices];
//	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);
//
//	_uint		iNumIndices = { 0 };
//
//	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
//	{
//		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[0];
//		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[1];
//		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[2];
//	}
//
//	ZeroMemory(&m_SubResourceDesc, sizeof(D3D11_SUBRESOURCE_DATA));
//	m_SubResourceDesc.pSysMem = pIndices;
//
//	if (FAILED(__super::Create_Buffer(&m_pIB)))
//		return E_FAIL;
//
//	Safe_Delete_Array(pIndices);
//
//#pragma endregion
//
//
//
//	return S_OK;
//}
/*for binary*/
HRESULT CMesh::Initialize_Prototype(CModel::TYPE eModelType, class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{

	//m_iMaterialIndex = pAIMesh->m_MaterialIndex;//�ش� �޽ÿ� �����Ǵ� ���׸��� �ε���
	////    //�޽� ������ �����ͷ� �ε��ϸ� �޽� �������� �̾ƿ� ���ִ� (�ϳ��� ���� �������� �Ž÷� �и��Ǿ� �����Ѵ�)
	//
	//strcpy_s(m_szName, pAIMesh->m_Name);//�޽ÿ��� ���� �̸��� �����ϸ� �̴� �߿��� �����̴�.
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_pAIMesh = pAIMesh;
	m_iNumVertexBuffers = 1;//���۴� ���� 1������

	
	//m_iNumVertices = pAIMesh->m_NumVertices; //�޽� ���ؽ��� ������ �����´� 

	m_iIndexStride = sizeof(_uint);//���� ������ �� ���� ���� �����ϱ�int ������ �ε����� ������ �˳��ϰ� ��´�.
	//m_iNumIndices = pAIMesh->m_NumFaces* 3;  //�޽��� �ε��� ���� ��ſ� ������(�ﰢ��)�� ������ �����Ѵ�. �ű⿡ 3�� ���ϸ� �ε����� ������ ��ġ�ϹǷ� �̸� �ε��� ������ ��´�.
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER	

	HRESULT hr = CModel::TYPE_NONANIM == eModelType ? Ready_VertexBuffer_For_NonAnim(pAIMesh, PreTransformMatrix) : Ready_VertexBuffer_For_Anim(pAIMesh, pModel);

	if (FAILED(hr))
		return E_FAIL;
#pragma endregion

	



#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* �������۸� �� ����Ʈ �Ҵ��ұ��? */
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	m_iNumPrimitive = m_pAIMesh->mNumFaces;

	//_uint* pIndices = new _uint[m_iNumIndices];
	//ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	//�ϳ��� ������ ������ �� ������(�ﰢ��)�� �̷�� �������� �ε����� �����Ǿ��ִ�. -> �����ؼ� ��������.
	//for (size_t i = 0; i < pAIMesh->m_NumFaces; i++)
	//{
	//	pIndices[iNumIndices++] = pAIMesh->m_vecIndices[i].m_index[0];//������.�ε���1-> �굵 �������ε� ���� �迭(FACE)�� ����ü���� �����迭��(ũ��FLOAT3)�� ������ �ε����� ����ִ�. 
	//	pIndices[iNumIndices++] = pAIMesh->m_vecIndices[i].m_index[1];
	//	pIndices[iNumIndices++] = pAIMesh->m_vecIndices[i].m_index[2];
	//}

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		aiFace		AIFace = m_pAIMesh->mFaces[i];

		pIndices[i]._0 = AIFace.mIndices[0];
		pIndices[i]._1 = AIFace.mIndices[1];
		pIndices[i]._2 = AIFace.mIndices[2];
	}

	ZeroMemory(&m_SubResourceDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceDesc.pSysMem = pIndices;

	//if (FAILED(__super::Create_Buffer(&m_pIB)))
	//	return E_FAIL;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;
	m_pIndices = pIndices;
	Safe_Delete_Array(pIndices);

#pragma endregion
	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CMesh::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, const vector<CBone*>& Bones)
{
	ZeroMemory(m_BoneMatrices, sizeof(_float4x4) * 512); // ������ ���̴��� ������� ������ ���� �Ź� ������Ʈ�� �����Ѵ�.
	_uint iNumBones = { 0 };

	//�� �޽ÿ� ������ �ִ� ���������� ���� Ʈ������ ��Ʈ������ �����������ؼ� ���� �� ���� ������ �ε����� �ϳ��� ��ȸ�Ѵ�.
	//�� �޽ÿ� ������ �ִ� ������ ��Ʈ������ ������ �����ϴ� ����( ������ ���̴��� ��������) 
	for (auto& iBoneIndex/*�� ������ �ε���*/ : m_BoneIndices)
	{
		XMStoreFloat4x4(&m_BoneMatrices[iNumBones],
			XMLoadFloat4x4(&m_BoneOffsetMatrices[iNumBones]) * Bones[iBoneIndex]/*����ü ������*/
				->Get_CombinedTransformationMatrix()
		);
		++iNumBones;
	}
	/* �޽� ������ �������� ���� -> ��Ʈ������ �Žô����� ������ ��*/
	/* ���̴��� ���� ���� ��� �ϴ� �������� �޽� ������ �������ϹǷ� �޽ô����� ����*/
	/* ���̴��� �Žô����� �޽ÿ� ������ �ִ� ������ ���� ��ȯ ��Ʈ�������� �� ������ �־�ߵ�*/
	/*�� ������ ������ �ټ� �ִ� �Žô� �ִ� 4���̴� -> ��� ���� ���̴��� �ִٸ�? -> �ε����� �˺� �ȴ�.*/
	
	/* ���: �޽þȿ� �޽ÿ� ������ �ִ� �������κ��� ù ��°�δ� ���������� ��ȸ�ϸ鼭 
	���� �̸����� �ش� ���� ���εǴ� �� �������� �ε����� ã�Ҵٸ�, 
	-> �޽ú����� i��° ���Ұ��� i��° ��(�޽ñ���)�� (�� ���Ϳ� ����� ���� ����ü)�� �� �ε����̰�*/
	/*��i��° ���ҷ� i��° ���� ��(����)���� ������ ��Ʈ������ ����. */
	
	//�޽ÿ� ������ �ִ� ����Ʈ������ ���� ���̴��� �����Ѵ�, �޽ô����� �������ϱ� ���� 
	//�ϴ� ���̴��� �޽ÿ� ������ ��ġ�� ��� ���� ��Ʈ������ �����ϰ�: �� ��Ʈ����[�޽� �ε���] �� �����ؼ� 
	//�ش� ������ ������ �ִ� ���� [�޽� �ε����� ������ ����Ʈ����]�� ������ ���Ѵ�. 
	return pShader->Bind_Matrices(pConstantName,m_BoneMatrices,512);  
}



HRESULT CMesh::Ready_VertexBuffer_For_NonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{

	m_iVertexStride = sizeof(VTXMESH);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* �������۸� �� ����Ʈ �Ҵ��ұ��? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;



	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PreTransformMatrix));

	}

	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_Anim(const aiMesh* pAIMesh, CModel* pModel)
{
	m_iVertexStride = sizeof(VTXANIMMESH);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);
	//���ϴ� 0���� �ʱ�ȭ vBlendIndex�� �⺻������ default��
	//{0,0,0,0}�� ������.->  ���̴� �ڵ忡�� 
	// bone�� ���� �� ����ġ�� ��� 0�̹Ƿ�
	// w���� ����ġ 1���ְ� �� 1���� ä���ִµ� ����(�ִϸ��̼� ������ �� 1�� ���� -> �޽��̸� =bone�̸� -> �̰ɷ� ����ã�� ��� ã��)
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}
	//������� ���� .
	//�ٵ� ��� �� ������ ������ ��ġ�� 4���� ���� ���Ҽ� ����? 
	//�츮�� ä������� ���� �� ������ ������ ��ġ�� 4����  ���� �ε����� , ������ ��ġ�� ���� ���̴� 
	// 
	/* �� �޽ÿ� ������ ��ġ�� ���� ���� */
	m_iNumBones = pAIMesh->mNumBones;

	/*ai Bone : �� �޽ÿ� ������ �ִ� ���� � �������� �󸶳� ������ �ִ����� ���� ������ ����*/

//-> �� �޽ÿ� ������ �ִ� (�޽���)���� �� ��ȸ�ϸ鼭 -> ������ �ִ� 4���� ���� �� ã�ƾ��Ѵ� . 

	for (size_t i = 0; i < m_iNumBones; ++i)
	{

		aiBone* pAIBone = pAIMesh->mBones[i]; //�� �޽ÿ� ������ �ִ� ���� ������ �ƴ� �� �������� �����Ѵ�.
	
		_float4x4 OffsetMatrix = {};
		
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		//->ai���� �����ϴ� �����Լ��� ������������ ai_process->�ڵ����� ȿ���� ��Ÿ�����ʴ´�( ��ü �и���)
		//-> �� �޾ƿ��� ������ COL MAJOR�̹Ƿ� ��ġ�ؼ� �޾ƿ;� �Ѵ� .
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
		
		
		m_BoneOffsetMatrices.push_back(OffsetMatrix);
		//->���� ������ŭ offset ��Ʈ�������� ������. (�� �ε��� ��° ���� �ش��ϴ� ���� ����� �����Ѵ�.) 
		// i�� �ִ´�( i�� �� ������ �ε���)
		m_BoneIndices.push_back(pModel->Get_BoneIndex(pAIBone->mName.data));

		//�̻��� ������ �ִ� ������ ���� (weights)
			for (size_t j = 0; j < pAIBone->mNumWeights; ++j)//�ش��ϴ� ���� ����ġ�� aiBone[��](������ ������ �ִ�)�� �ش�.
			{
				//pAIBone->mWeights[j]: �̻��� ������ �ִ��� (�޽���) j��° ������ ����*/
				//pAIBone->mWeights[j].mVertexId : �̻��� ������ �ִ� j���� ������ �ε���  (�ε��� ������ �ε���)

				if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.x = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x = pAIBone->mWeights[j].mWeight;
				}
				else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.y = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y = pAIBone->mWeights[j].mWeight;
				}

				else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.z = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z = pAIBone->mWeights[j].mWeight;
				}

				else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.w = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w = pAIBone->mWeights[j].mWeight;
				}
					//��� ���� �ٵ��鼭 ������ ������ ��ĥ�� �ִ� ���� 4���� ����, �׸��� x,y,z,w ���ʷ� ä��������.  ������ ���̻� ��ä��
					//���� �갡 pBone�� �޽ð� ������ �����ϱ�,  ������ �ִ����� �ε����� ����
			}
	}
	//� �Žô� ���� ��� ���� �ʴ� -> �̰��� �ִϸ��̼��� ���� ��������,  �� �Žø� ��ġ�ϱ����� ���� ���� �ϱ� ������ 
	//�� �޽��� ���ϴ� ���� ��ü ���� �����Ϳ��� ��ȸ�ϸ鼭 �̸����� ��ġ�ϴ� ���� ã�ƾ� �Ѵ�.
	// �� ���� �̸��� �޽��� �̸��� ��ġ�Ѵ�.( ��ġ�� ���� �� (�ִϸ��̼��� �ƴ�����)) 
	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_BoneIndices.push_back(pModel->Get_BoneIndex(m_szName));// ù��° Į�޽ô� ���� aimesh���� �������� ������ ��ġ�� ���ѻ��� model�� �����ϹǷ�,  �׻��� �̸�����ã�ƾ��Ѵ�
		_float4x4 OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());// �޽ð� �������� �ʴ»��� ��ġ���� ������ķ� �׵������ �����ָ� �ȴ�.
		m_BoneOffsetMatrices.push_back(OffsetMatrix);
	
	}
	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}
/*for binary*/
HRESULT CMesh::Ready_VertexBuffer_For_NonAnim(const AI_MeshInfo* pAIMesh, _fmatrix PreTransformMatrix)
{
	m_iVertexStride = sizeof(VTXMESH);
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* �������۸� �� ����Ʈ �Ҵ��ұ��? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	/*_vector vecAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_matrix marRot= DirectX::XMMatrixRotationAxis(vecAxis,XMConvertToRadians(180.f));*/

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		//��memcpy�� �޾ƿ��°�? aiSenen���� �������� Ÿ���� dx11�������� Ÿ�԰� ��ġ���� �ʴ´�. �ٸ� ����ü (�ٸ� Ÿ��) �̹Ƿ� memcpy�� 1����Ʈ�� �о �����͸� �о �����´�.


		memcpy(&pVertices[i].vPosition, &pAIMesh->m_vecPositions[i], sizeof(_float3));//���� ����(��ġ)
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->m_vecNormals[i], sizeof(_float3));	//�븻����
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

		memcpy(&pVertices[i].vTexcoord, &pAIMesh->m_vecTexcoords[i], sizeof(_float2));//(������ �迭(�����迭�� ����Ű��?))	//texcoord�� 2���� �迭�� �Ǿ��ִ� (texcoord�� 8������ ����Ҽ� �����Ƿ�, diffuse�뵵, ���� �뵵....)
		//TEXCOORD0,1,2,3,.....
		memcpy(&pVertices[i].vTangent, &pAIMesh->m_vecTangents[i], sizeof(_float3));//TANGENT���� -> �븻�� �ؽ�ó�� �����ϸ� ��δ� ���� ���� ���͹����� -> �׷��� ź��Ʈ ������ ������ ����
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PreTransformMatrix));
	}

	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	return S_OK;
}
HRESULT CMesh::Ready_VertexBuffer_For_Anim(const AI_MeshInfo* pAIMesh, CModel* pModel)
{
	


	m_iVertexStride = sizeof(VTXANIMMESH);
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* �������۸� �� ����Ʈ �Ҵ��ұ��? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	/*_vector vecAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_matrix marRot= DirectX::XMMatrixRotationAxis(vecAxis,XMConvertToRadians(180.f));*/

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		
		memcpy(&pVertices[i].vPosition, &pAIMesh->m_vecPositions[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->m_vecNormals[i], sizeof(_float3));	
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->m_vecTexcoords[i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->m_vecTangents[i], sizeof(_float3));
		
	}
	/*���⼭ ���ο� ������ ä��*/



	m_iNumBones = pAIMesh->m_iNumBones;

	for (size_t i = 0; i < m_iNumBones; ++i)
	{

		//aiBone* pAIBone = pAIMesh->mBones[i]; //�� �޽ÿ� ������ �ִ� ���� ������ �ƴ� �� �������� �����Ѵ�.

		//_float4x4 OffsetMatrix = {};

		//memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		////->ai���� �����ϴ� �����Լ��� ������������ ai_process->�ڵ����� ȿ���� ��Ÿ�����ʴ´�( ��ü �и���)
		////-> �� �޾ƿ��� ������ COL MAJOR�̹Ƿ� ��ġ�ؼ� �޾ƿ;� �Ѵ� .
		//XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));


		m_BoneOffsetMatrices.push_back(pAIMesh->m_vecOffsetMatrices[i]);
		//->���� ������ŭ offset ��Ʈ�������� ������. (�� �ε��� ��° ���� �ش��ϴ� ���� ����� �����Ѵ�.) 
		// i�� �ִ´�( i�� �� ������ �ε���)
		m_BoneIndices.push_back(pModel->Get_BoneIndex(pAIMesh->m_vecBoneNames[i].Name));

		//�̻��� ������ �ִ� ������ ���� (weights)
		for (size_t j = 0; j < pAIMesh->m_vecNumWeights[i] ; ++j)//�ش��ϴ� ���� ����ġ�� aiBone[��](������ ������ �ִ�)�� �ش�.
		{
			//pAIBone->mWeights[j]: �̻��� ������ �ִ��� (�޽���) j��° ������ ����*/
			//pAIBone->mWeights[j].mVertexId : �̻��� ������ �ִ� j���� ������ �ε���  (�ε��� ������ �ε���)

			if (0 == pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.x)
			{
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendIndices.x = i;
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.x =pAIMesh->m_vecWeights[i][j];
			}
			else if (0 == pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.y)
			{
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendIndices.y = i;
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.y = pAIMesh->m_vecWeights[i][j];
			}

			else if (0 == pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.z)
			{
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendIndices.z = i;
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.z = pAIMesh->m_vecWeights[i][j];
			}
			else if (0 == pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.w)
			{
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendIndices.w = i;
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.w = pAIMesh->m_vecWeights[i][j];
			}
			//��� ���� �ٵ��鼭 ������ ������ ��ĥ�� �ִ� ���� 4���� ����, �׸��� x,y,z,w ���ʷ� ä��������.  ������ ���̻� ��ä��
			//���� �갡 pBone�� �޽ð� ������ �����ϱ�,  ������ �ִ����� �ε����� ����
		}
	}
	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_BoneIndices.push_back(pModel->Get_BoneIndex(m_szName));// ù��° Į�޽ô� ���� aimesh���� �������� ������ ��ġ�� ���ѻ��� model�� �����ϹǷ�,  �׻��� �̸�����ã�ƾ��Ѵ�
		_float4x4 OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());// �޽ð� �������� �ʴ»��� ��ġ���� ������ķ� �׵������ �����ָ� �ȴ�.
		m_BoneOffsetMatrices.push_back(OffsetMatrix);

	}



	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	return S_OK;




}
/*��for binary non anim mesh */
//CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
//{
//	CMesh* pInstance = new CMesh(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModel,pAIMesh, PreTransformMatrix)))
//	{
//		MSG_BOX("Failed to Created : CMesh_bin");
//		Safe_Release(pInstance);
//	}
//	return pInstance;
//}
/*for Ai*/
CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{

	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModel, pAIMesh, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
    CMesh* pInstance = new CMesh(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Clone : CMesh");
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
	/*�Ⱦ���- ����� ���ϰ� ���� �����ϱ�.*/
}

void CMesh::Free()
{
    __super::Free();
}

HRESULT CMesh::SetUp_Bones(CModel* pModel)
{
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = m_pAIMesh->mBones[i];

		CBoneNode* pBoneNode = pModel->Get_BonePtr(pAIBone->mName.data);
		if (nullptr == pBoneNode)
			return E_FAIL;

		m_Bones.push_back(pBoneNode);

		Safe_AddRef(pBoneNode);

		_float4x4		OffsetMatrix;

		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		pBoneNode->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
	}


	if (0 == m_iNumBones)
	{
		CBoneNode* pNode = pModel->Get_BonePtr(m_szName);
		if (nullptr == pNode)
			return S_OK;

		m_iNumBones = 1;

		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}

	return S_OK;
}
