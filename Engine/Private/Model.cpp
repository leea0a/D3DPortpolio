#include "Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Bone.h"
#include "Animation.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent {pDevice, pContext}
{
}

CModel::CModel(const CModel& Prototype)
:CComponent(Prototype)
, m_pAIScene{ Prototype.m_pAIScene }
, m_iNumMeshes{ Prototype.m_iNumMeshes }
, m_Meshes{ Prototype.m_Meshes }
, m_iNumMaterials{ Prototype.m_iNumMaterials }
, m_Materials{ Prototype.m_Materials }
, m_PreTransformMatrix{Prototype.m_PreTransformMatrix}
//, m_Bones{Prototype.m_Bones}// ���� ���� ���� ���� ���ҰŴ�. ������ �������
,m_eModelType{Prototype.m_eModelType}
, m_iNumAnimations{ Prototype.m_iNumAnimations }
//, m_Animations{ Prototype.m_Animations }//�ִϸ��̼ǵ� �� ����
, m_isBinary{Prototype.m_isBinary}
, m_vecTriangles{Prototype.m_vecTriangles }//navimesh ��������
{

	/*for (auto& pBone : m_Bones)
		Safe_AddRef(pBone);*/

	for (auto& pPrototypeAnimation : Prototype.m_Animations)
	{
		m_Animations.push_back(pPrototypeAnimation->Clone()); //��(*this) 
	}
	for (auto& pPrototypeBone : Prototype.m_Bones)
	{
		m_Bones.push_back(pPrototypeBone->Clone());
	}
	// ���� ������ ȣ���ؼ� �� ���� ����� �Ѱ��ش� 
	// -> ���� �ڽ��� ��( �ȿ� �����Ͱ� ���� ���� �������� ����)
	//�ִϸ��̼��� ä�ε��� �����ϱ� �ϴµ� -> Ű������ �ε����� �ִϸ��̼��� ä�θ��� ���� �����ؼ� 
	//�ٸ� ��ü�� ����� ��ġ�� ���������� 
	// //(Ű������ �������ϴϱ� (�ѹ� ������Ʈ ���� ����)���������� ����� Ű������ ��� �����ϴ� ���󸷾Ƽ� 
	//������ ���� �帣�� ���� ������ 

	for (auto& Textures : m_Materials)
	{
		for (auto& SRVs : Textures)
		{
			for (auto& pSRV : SRVs)
				Safe_AddRef(pSRV);
		}
	}


	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);
}

_uint CModel::Get_BoneIndex(const _char* pBoneName) const 
{

	_uint iBoneIndex = { 0 };
	auto iter = find_if(m_Bones.begin(), m_Bones.end(),
		[&](CBone* pBone)->_bool
		{
			if (false == strcmp(pBone->Get_Name(), pBoneName))
				return true;// ����
			++iBoneIndex;//���������� ++
			return false;//���� 
		});
	if (iter == m_Bones.end())
		MSG_BOX("����");
	return iBoneIndex;
}
const _float4x4* CModel::Get_BoneMatrix(const _char* pBoneName) const
{
	//���ʹϱ� find_if �� �̸����� ���� ã�� ��Ʈ ������Ʈ�� ���� �̸��� ������ �ְ���. 
	//�ƴϸ� �ϵ��ڵ����� �ڰų�

	auto iter =find_if(m_Bones.begin(),m_Bones.end(), [&](CBone* pBone)->_bool
		{
			if (false == strcmp(pBone->Get_Name(), pBoneName))
				return true;

			return false;
		});

	if (iter == m_Bones.end())
		return nullptr;

	return  (*iter)->Get_CombinedTransformationFloat4x4();
}

void CModel::SetUp_Animation(_uint iAnimIndex, _bool isLoop)
{
	m_Animations[m_iCurrentAnimIndex]->Reset_CurrentKeyFrameIndices();
	m_isChangingMotion = false;
	m_iCurrentAnimIndex = iAnimIndex;  //�ִϸ��̼� �ٲ���
	m_isLoop = isLoop;					//�� �ִϸ��̼� ������������ ���
}

HRESULT CModel::Initialize_Prototype(TYPE eModelType, const _char* pModelFilePath,_fmatrix PreTransformMatrix, _tchar* biModelFilePath, bool _isBinary)
{
	m_isBinary = _isBinary;
	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);
	
	if (nullptr == biModelFilePath)
	{
		_uint		iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };

		if (eModelType == TYPE_NONANIM)
			iFlag |= aiProcess_PreTransformVertices;
	
		m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
		
		if (0 == m_pAIScene)
			return E_FAIL;

		m_eModelType = eModelType;

		XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);// �ִ��̸� ���� , �� �ִ��̸� ������ 

		
		if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
			return E_FAIL;
		
		if (FAILED(Ready_Meshes()))
			return E_FAIL;

 		if (FAILED(Ready_Materials(pModelFilePath)))
			return E_FAIL;

		if (FAILED(Ready_Animations()))
			return E_FAIL;
	
	}
	else
	{
		m_eModelType = eModelType;
		XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);// �ִ��̸� ���� , �� �ִ��̸� ������ 


		if (TYPE::TYPE_NONANIM == eModelType)
		{
			m_aiInfo.Initialze_NonAnim(biModelFilePath);
			if (FAILED(Ready_Meshes_Bin()))
				return E_FAIL;

			if (FAILED(Ready_Materials_Bin()))
				return E_FAIL;
		}
		else
		{
			m_aiInfo.Initialize_Anim(biModelFilePath);
			
			if (FAILED(Ready_Bones_Bin(m_aiInfo.m_RootNode, -1)))
				return E_FAIL;
			
			if (FAILED(Ready_Meshes_Bin()))
				return E_FAIL;

			if (FAILED(Ready_Materials_Bin()))
				return E_FAIL;
 
			 if (FAILED(Ready_Animations_Bin()))
				return E_FAIL;
			

		}
	}

	

	

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

//�ش��ϴ� �޽ø� Ŭ��(���ӿ�����Ʈ�� �޽��� ������ �����ְ�, �޽ø� ���� ������ �Ѱ��ְ� �׸��� )
//Ŭ�󿡼� �ݺ������� ó��-> �ϳ��� �׷��� (������ �޽ðԼ��� Ŭ�� �ְ� �ܺο��� �ݺ��� ������.)
HRESULT CModel::Render(_uint iMeshIndex)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	m_Meshes[iMeshIndex]->Bind_BufferDesc();
	m_Meshes[iMeshIndex]->Render();
	return S_OK; 
}


_bool CModel::Play_Animation(_float fTimeDelta, _bool& isChanging)
{
	
	isChanging = m_isChangingMotion;
	/*chaging motion*/
	if (m_isChangingMotion) 
	{
		m_isFinished  = m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices_ForMotionChange(m_Bones, m_isLoop, fTimeDelta);

		if (m_isFinished)
		{
			m_isChangingMotion = false;
			m_iCurrentAnimIndex = m_iNextAnimIndex;
			m_iNextAnimIndex = m_iNextAnimIndex;
			m_isLoop = m_isNextAnimationLoop;
		}
		isChanging = m_isChangingMotion;

		for (auto& pBone : m_Bones)
		{
			pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
		}
		return false;
	}

	/* ǥ���ϰ��� �ϴ� �ִϸ��̼ǿ� ����, ���� ����(TransformationMatrix)�� �������ش�.  */
	else if(false == m_isBinary)
		m_isFinished = m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(m_Bones, m_isLoop,fTimeDelta);
	/* ���� �ٲ� TransformationMatrix�� Ȱ���Ͽ� �������ϱ����� ���� ���� CombinedTransformationMatrix�� ������. */
	
	for (auto& pBone : m_Bones)
	{
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
	}
	if (m_isFinished == true)
		int a = 0;

 	return m_isFinished;
}
HRESULT CModel::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
	return m_Meshes[iMeshIndex]->Bind_BoneMatrices(pShader,pConstantName,m_Bones);
}
HRESULT CModel::Bind_Material(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eType, _uint iTextureIndex)
{
		
	//�ܺηκ��� �޽��ε����� �����ָ� ���Ϳ��� �޽ø� �������� �޽ó�����
	
	// ���ε� ���׸��� �ε��� ������ �������� �Լ� ȣ��
	
	// ->[���׸����� ���� �� � ���׸���][� �ؽ�ó Ÿ��][�� ��° diffuse �ؽ�ó ]				
	if (m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()][eType].empty())
		return S_OK;

	return pShader->Bind_SRV(pConstantName, m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()][eType][iTextureIndex]);
	
}

/*For Next Anim*/
void CModel::Move_NextAnim(_uint NextAnimIndex,_float fDistanceAnimToAnim , _bool isNextAnimationLoop, _bool SlerpWhileSlerp )
{
	
	/*if (true == m_isChangingMotion)
		return;*/

	m_Animations[m_iCurrentAnimIndex]->Store_CurrentAnim_KeyFrame_State(m_Bones, SlerpWhileSlerp);
	m_Animations[m_iCurrentAnimIndex]->Store_AnimToChange_KeyFrame_State(m_Bones, m_Animations[NextAnimIndex]);
	m_Animations[m_iCurrentAnimIndex]->SetUp_DistanceAnimToAnim(fDistanceAnimToAnim);	
	m_iNextAnimIndex = NextAnimIndex;
	m_isNextAnimationLoop = isNextAnimationLoop;
	m_isChangingMotion = true;

}

void CModel::Set_AnimSpeed(int iIndex, _float fRatio)
{
	 m_Animations[iIndex]->Set_TickRatio(fRatio); 
}

void CModel::Add_Triangle(TRIANGLE_VERTICES Triangle)
{
	m_vecTriangles.push_back(Triangle);
}


HRESULT CModel::Ready_Meshes()
{

	m_iNumMeshes = m_pAIScene->mNumMeshes;  
	//aiScene�����޽��� ������ �������� �� ������ŭ �޽ø� �����ؼ� �޽ø� �����Ѵ�( �޽�Ŭ������ �޽ø� ����)
	//mNumMesh �� ������ mesh�� �迭�� �������ش� <-> �޽��� ������ �����ϸ� �׿� �ش��ϴ� �޽õ� ���� ����
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, this,m_pAIScene->mMeshes[i],XMLoadFloat4x4(&m_PreTransformMatrix));//�޽� Ŭ������ aiScene�� ����Ǿ��ִ� �޽�(����)�� ����(���� �ε��� ����)[Ÿ���� aimesh]
		if (nullptr == pMesh)
			return E_FAIL;
  		m_Meshes.push_back(pMesh);
	}
		//
	return S_OK;
}


HRESULT  CModel::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;
	//������ ���׸����������´�
	m_Materials.resize(m_iNumMaterials);
	//���׸��� ������ŭ  ����ũ�� �缳��

	for (size_t i = 0; i < m_iNumMaterials; ++i)
	{
		m_Materials[i].resize(AI_TEXTURE_TYPE_MAX); 
		//���׸��󸶴� �ؽ�ó�� ������ 18���� �а��ؼ� ������� 
		//ũ�⸦ 18�� �����.
		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];
		//m_iNumMaterial�� i��° ���׸����� �����´�.
		for (size_t j = 1;  j < AI_TEXTURE_TYPE_MAX ; j++)//ù��°�� ����Ÿ�� 1��°���� diffuse 2��°�� specular ....18��° Ÿ�Ա��� ������ ����ִ�
		{
			_uint iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(j));
			
			if (0 == iNumTextures && 1 == j)
			{
				_char		szDrive[MAX_PATH] = "";
				_char		szDirectory[MAX_PATH] = "";
				_char		szFileName[MAX_PATH] = "hero_fpp_101_A_c";
				_char		szExt[MAX_PATH] = ".png";

				_char		szFullPath[MAX_PATH] = "";
				_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);


				strcpy_s(szFullPath, szDrive);
				strcat_s(szFullPath, szDirectory);
				strcat_s(szFullPath, szFileName);
				strcat_s(szFullPath, szExt);
				//DRIVE���+ ���丮 ��� + �ؽ�ó �̸� + �ؽ�ó Ȯ���� (�ؽ�ó�� �޽ö� ���� �־�ߵ�)
				_tchar		szPerfectPath[MAX_PATH] = TEXT("");
				MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH);
				ID3D11ShaderResourceView* pSRV = { nullptr };
				if (false == strcmp(szExt, ".dds"))
				{
					if (SUCCEEDED(CreateDDSTextureFromFile(m_pDevice, szPerfectPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}
				else if (false == strcmp(szExt, ".tga"))
				{
					int a = 10;
				}
				else
				{
					if (SUCCEEDED(CreateWICTextureFromFile(m_pDevice, szPerfectPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}
			}
			for (size_t k = 0; k < iNumTextures; ++k)
			{
				aiString strTexturePath;//aiString Ÿ���� ���� ����
				 //i��° ���׸����� jŸ���� �ؽ�óiNumTextures ���� k��° �ؽ�ó�� �ּ� strTexturePath�� �޾ƿ´�
				
				
				if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), k, &strTexturePath)))
					break;

				_char		szDrive[MAX_PATH] = "";
				_char		szDirectory[MAX_PATH] = "";
				_char		szFileName[MAX_PATH] = "";
				_char		szExt[MAX_PATH] = "";

				_char		szFullPath[MAX_PATH] = "";

				_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
				//���� ����� ���丮 fbx �� ������ �����ϴ� ���丮(MODEL���� �����´�.)
 				_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
				//�ؽ�ó�� �ּҸ� �Ѱ�, �ؽ�ó�� ���� �̸� Ȯ���ڸ� �����´�. 

				strcpy_s(szFullPath, szDrive);
				strcat_s(szFullPath, szDirectory);
				strcat_s(szFullPath, szFileName);
				strcat_s(szFullPath, szExt);
				//DRIVE���+ ���丮 ��� + �ؽ�ó �̸� + �ؽ�ó Ȯ���� (�ؽ�ó�� �޽ö� ���� �־�ߵ�)
				_tchar		szPerfectPath[MAX_PATH] = TEXT("");

				MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH);
				ID3D11ShaderResourceView* pSRV = { nullptr };

				if (false == strcmp(szExt, ".dds"))
				{
					if (SUCCEEDED(CreateDDSTextureFromFile(m_pDevice, szPerfectPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}
				else if (false == strcmp(szExt, ".tga"))
				{
					int a = 10;
				}	
				else
				{
					if (SUCCEEDED(CreateWICTextureFromFile(m_pDevice, szPerfectPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}
			}
		}
	}
	return S_OK;
}



HRESULT CModel::Ready_Bones(const aiNode* pAINode, _int iParentBoneIndex)
{
	//(�ڱ� �ڽ��� ��� ����,�θ��� �ε���)
	CBone* pBone = CBone::Create(pAINode, iParentBoneIndex);
	
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);
	iParentBoneIndex = m_Bones.size() - 1;

	for (size_t i = 0; i < pAINode->mNumChildren; ++i)//-> 
		//childnum�� ������ child ��嵵 �����Ѵ�. 
	{
		//child������ �ڱ��ڽ��� �ε����� child bone aiNode�� ������ �θ� �ڽ� ���踦 �˰� �Ѵ�. 
		Ready_Bones(pAINode->mChildren[i], iParentBoneIndex);
	}


	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	//�ִϸ��̼��� ������ �����´� 
	// animation����ü�� ä�� ����ü�� ������, ü�ε� ��(ä�� �̸�) ���� ����Ʈ(Ű������) ����ü�� ������ ...
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}


HRESULT CModel::Ready_Meshes_Bin()
{
	m_iNumMeshes = m_aiInfo.m_iNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, this, &m_aiInfo.m_vecMeshes[i], XMLoadFloat4x4(&m_PreTransformMatrix));//�޽� Ŭ������ aiScene�� ����Ǿ��ִ� �޽�(����)�� ����(���� �ε��� ����)[Ÿ���� aimesh]
		if (nullptr == pMesh)
			return E_FAIL;
		m_Meshes.push_back(pMesh);
	}
	//
	return S_OK;
}
HRESULT CModel::Ready_Materials_Bin()
{

	m_iNumMaterials = m_aiInfo.m_iNumMaterials;  //m_pAIScene->mNumMaterials;
	//������ ���׸����������´�
	m_Materials.resize(m_iNumMaterials);
	//���׸��� ������ŭ  ����ũ�� �缳��

	for (size_t i = 0; i < m_iNumMaterials; ++i)
	{
		m_Materials[i].resize(AI_TEXTURE_TYPE_MAX);
		//���׸��󸶴� �ؽ�ó�� ������ 18���� �а��ؼ� ������� 
		//ũ�⸦ 18�� �����.
		//aiMaterial* pAIMaterial m_pAIScene->mMaterials[i];
		//m_iNumMaterial�� i��° ���׸����� �����´�.
		for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)//ù��°�� ����Ÿ�� 1��°���� diffuse 2��°�� specular ....18��° Ÿ�Ա��� ������ ����ִ�
		{
			_uint iNumTextures = m_aiInfo.m_vecMaterials[i].m_vecNumTextures[j];//pAIMaterial->GetTextureCount(aiTextureType(j));
			//i��° ���͸��� [j��° �ؽ�ó Ÿ��]�� �������� ������ �������´�. 
			for (size_t k = 0; k < iNumTextures; ++k)//diffuse�� 1�� �̻��� �� �� �ִ� 
			{
				_tchar		szPath[MAX_PATH]; 
				lstrcpy(szPath,m_aiInfo.m_vecMaterials[i].m_vecTexturePaths[j][k].FilePath);

				_tchar		szExt[MAX_PATH] = L"";
				_wsplitpath_s(szPath, nullptr, 0, nullptr, 0, nullptr, 0, szExt,MAX_PATH);
				
				ID3D11ShaderResourceView* pSRV = { nullptr };

				if (false == lstrcmp(szExt, L".dds"))
				{
					if (SUCCEEDED(CreateDDSTextureFromFile(m_pDevice, szPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}
				else if (false == lstrcmp(szExt, L".tga"))
				{
					int a = 10;
				}
				else
				{
					if (SUCCEEDED(CreateWICTextureFromFile(m_pDevice, szPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}
			}
		}
	}
	return S_OK;


}
HRESULT CModel::Ready_Bones_Bin( AI_Node* pAINode, _int iParentBoneIndex)
{
	//(�ڱ� �ڽ��� ��� ����,�θ��� �ε���)
	CBone* pBone = CBone::Create(pAINode, iParentBoneIndex);

	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);
	iParentBoneIndex = m_Bones.size() - 1;

	for (size_t i = 0; i < pAINode->m_NumChildren; ++i)
		//-> 
		//childnum�� ������ child ��嵵 �����Ѵ�. 
	{
		//child������ �ڱ��ڽ��� �ε����� child bone aiNode�� ������ �θ� �ڽ� ���踦 �˰� �Ѵ�. 
		Ready_Bones_Bin(pAINode->m_vecChildren[i], iParentBoneIndex);
	}


	return S_OK;
}
HRESULT CModel::Ready_Animations_Bin()
{
	m_iNumAnimations = m_aiInfo.m_iNumAnimations;
	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(&m_aiInfo.m_vecAnims[i],this);

		if (nullptr == pAnimation)
			return E_FAIL;
		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix, _tchar* biModelFilePath,bool _isBinary)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PreTransformMatrix, biModelFilePath,_isBinary)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();
	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();

	for (auto& Textures : m_Materials)
	{
		for (auto& SRVs : Textures)
		{
			for (auto& pSRV : SRVs)
				Safe_Release(pSRV);

			SRVs.clear();
		}
		Textures.clear();
	}
	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	m_Meshes.clear();

	m_Importer.FreeScene();
}




HRESULT CModel::Make_Binary_NonAnim(TYPE eModelType, const _char* pModelFilePath, _tchar* OutputFilePath,_fmatrix PreTransformMatrix)
{
	//for extracting binary
	Assimp::Importer		Importer;
	const aiScene* pAIScene = { nullptr };

	_uint		iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };

	if (eModelType == TYPE_NONANIM)
		iFlag |= aiProcess_PreTransformVertices;

	pAIScene = Importer.ReadFile(pModelFilePath, iFlag);

	if (0 == pAIScene)
		return E_FAIL;

	//pAIScene->mNumMeshes;
/*fileInput*/
	{

		HANDLE hFile = CreateFile(OutputFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return E_FAIL;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);

		//�޽��� ������ �����մϴ�.
		_uint iNumMeshes = pAIScene->mNumMeshes;
		WriteFile(hFile, &iNumMeshes, sizeof(_int), &dwByte, nullptr);
		//�� �޽��� ������ �����մϴ�. 

		for (int i = 0; i < iNumMeshes; ++i)
		{
			const aiMesh* meshInfo = pAIScene->mMeshes[i];
			WriteFile(hFile, &meshInfo->mMaterialIndex, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &meshInfo->mNumVertices, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &meshInfo->mNumFaces, sizeof(_int), &dwByte, nullptr);
			{
				_char			m_szName[MAX_PATH] = "";
				strcpy_s(m_szName, meshInfo->mName.data);
				_int LenName = strlen(m_szName) + 1;//�ι��ڱ���
				WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
				WriteFile(hFile, &meshInfo->mName, LenName, &dwByte, nullptr);
			}

			for (int j = 0; j < meshInfo->mNumVertices; ++j)
			{
				_float3 vPosition = {};
				_float3 vNormal = {};
				_float2 vTexcoord = {};
				_float3 vTangent = {};
				memcpy(&vPosition, &meshInfo->mVertices[j], sizeof(_float3));//���� ����(��ġ)
				XMStoreFloat3(&vPosition, XMVector3TransformCoord(XMLoadFloat3(&vPosition), PreTransformMatrix));

				memcpy(&vNormal, &meshInfo->mNormals[j], sizeof(_float3));	//�븻����
				XMStoreFloat3(&vNormal, XMVector3TransformNormal(XMLoadFloat3(&vNormal), PreTransformMatrix));

				memcpy(&vTexcoord, &meshInfo->mTextureCoords[0][j], sizeof(_float2));//(������ �迭(�����迭�� ����Ű��?))	//texcoord�� 2���� �迭�� �Ǿ��ִ� (texcoord�� 8������ ����Ҽ� �����Ƿ�, diffuse�뵵, ���� �뵵....)
				//TEXCOORD0,1,2,3,.....
				memcpy(&vTangent, &meshInfo->mTangents[j], sizeof(_float3));//TANGENT���� -> �븻�� �ؽ�ó�� �����ϸ� ��δ� ���� ���� ���͹����� -> �׷��� ź��Ʈ ������ ������ ����
				XMStoreFloat3(&vTangent, XMVector3TransformNormal(XMLoadFloat3(&vTangent), PreTransformMatrix));
				WriteFile(hFile, &vPosition, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &vNormal, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &vTexcoord, sizeof(_float2), &dwByte, nullptr);
				WriteFile(hFile, &vTangent, sizeof(_float3), &dwByte, nullptr);
			}
			//�ϳ��� ������ ������ �� ������(�ﰢ��)�� �̷�� �������� �ε����� �����Ǿ��ִ�. -> �����ؼ� ��������.
			for (size_t j = 0; j < meshInfo->mNumFaces; j++)
			{
				int indices[3] = { 0,0,0 };
				indices[0] = meshInfo->mFaces[j].mIndices[0];//������.�ε���1-> �굵 �������ε� ���� �迭(FACE)�� ����ü���� �����迭��(ũ��FLOAT3)�� ������ �ε����� ����ִ�. 
				indices[1] = meshInfo->mFaces[j].mIndices[1];
				indices[2] = meshInfo->mFaces[j].mIndices[2];

				WriteFile(hFile, indices, sizeof(indices), &dwByte, nullptr);
			}
		}
		//���͸��� ���� �����̴ϴ�.

		_uint iNumMaterials = pAIScene->mNumMaterials;
		WriteFile(hFile, &iNumMaterials, sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i < iNumMaterials; ++i)
		{
			aiMaterial* pAIMaterial = pAIScene->mMaterials[i]; //���͸����� �����´�.
			for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)//�ؽ�ó Ÿ�Ը��� �ý�ó�� ������ ����� �����Ѵ�.
			{
				_uint iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(j));
				WriteFile(hFile, &iNumTextures, sizeof(_uint), &dwByte, nullptr);//18���� �ؽ�ó Ÿ���� ���� � �ִ���
				for (size_t k = 0; k < iNumTextures; ++k)//diffuse�� 1�� �̻��� �� �� �ִ� 
				{
					aiString strTexturePath;//aiString Ÿ���� ���� ����
					//i��° ���׸����� jŸ���� �ؽ�óiNumTextures ���� k��° �ؽ�ó�� �ּ� strTexturePath�� �޾ƿ´�
					if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), k, &strTexturePath)))
						break;

					_char		szDrive[MAX_PATH] = "";
					_char		szDirectory[MAX_PATH] = "";
					_char		szFileName[MAX_PATH] = "";
					_char		szExt[MAX_PATH] = "";

					_char		szFullPath[MAX_PATH] = "";

					_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
					//���� ����� ���丮 fbx �� ������ �����ϴ� ���丮(MODEL���� �����´�.)
					_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
					//�ؽ�ó�� �ּҸ� �Ѱ�, �ؽ�ó�� ���� �̸� Ȯ���ڸ� �����´�. 

					strcpy_s(szFullPath, szDrive);
					strcat_s(szFullPath, szDirectory);
					strcat_s(szFullPath, szFileName);
					strcat_s(szFullPath, szExt);
					//DRIVE���+ ���丮 ��� + �ؽ�ó �̸� + �ؽ�ó Ȯ���� (�ؽ�ó�� �޽ö� ���� �־�ߵ�)
					_tchar		szPerfectPath[MAX_PATH] = TEXT("");

					int size = (MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH) + 1) * sizeof(wchar_t);//�ι��ڱ���

					WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
					WriteFile(hFile, szPerfectPath, size, &dwByte, nullptr);

				}

			}

		}






		CloseHandle(hFile);
	}


	return S_OK;
}

HRESULT CModel::Make_Binary_Anim(TYPE eModelType, const _char* pModelFilePath, _tchar* OutputFilePath,_fmatrix PreTransformMatrix)
{
	//for extracting binary
	Assimp::Importer		Importer;
	const aiScene* pAIScene = { nullptr };

	_uint		iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };

	/*if (eModelType == TYPE_NONANIM)
		iFlag |= aiProcess_PreTransformVertices;*/

	pAIScene = Importer.ReadFile(pModelFilePath, iFlag);

	if (0 == pAIScene)
		return E_FAIL;



	//pAIScene->mNumMeshes;
/*fileInput*/
	{

		HANDLE hFile = CreateFile(OutputFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return E_FAIL;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);

		//�޽��� ������ �����մϴ�.
		_uint iNumMeshes = pAIScene->mNumMeshes;
		WriteFile(hFile, &iNumMeshes, sizeof(_uint), &dwByte, nullptr);
		//�� �޽��� ������ �����մϴ�. 

		for (int i = 0; i < iNumMeshes; ++i)
		{
			const aiMesh* meshInfo = pAIScene->mMeshes[i];
			WriteFile(hFile, &meshInfo->mMaterialIndex, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &meshInfo->mNumVertices, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &meshInfo->mNumFaces, sizeof(_int), &dwByte, nullptr);
			{
				_char			m_szName[MAX_PATH] = "";
				strcpy_s(m_szName, meshInfo->mName.data);
				_int LenName = strlen(m_szName) + 1;//�ι��ڱ���
				WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
				WriteFile(hFile, &m_szName, LenName, &dwByte, nullptr);
			}

			for (int j = 0; j < meshInfo->mNumVertices; ++j)
			{
				_float3 vPosition = {};
				_float3 vNormal = {};
				_float2 vTexcoord = {};
				_float3 vTangent = {};
				memcpy(&vPosition, &meshInfo->mVertices[j], sizeof(_float3));//���� ����(��ġ)
				//XMStoreFloat3(&vPosition, XMVector3TransformCoord(XMLoadFloat3(&vPosition), PreTransformMatrix));

				memcpy(&vNormal, &meshInfo->mNormals[j], sizeof(_float3));	//�븻����
				//XMStoreFloat3(&vNormal, XMVector3TransformNormal(XMLoadFloat3(&vNormal), PreTransformMatrix));

				memcpy(&vTexcoord, &meshInfo->mTextureCoords[0][j], sizeof(_float2));//(������ �迭(�����迭�� ����Ű��?))	//texcoord�� 2���� �迭�� �Ǿ��ִ� (texcoord�� 8������ ����Ҽ� �����Ƿ�, diffuse�뵵, ���� �뵵....)
				//TEXCOORD0,1,2,3,.....
				memcpy(&vTangent, &meshInfo->mTangents[j], sizeof(_float3));//TANGENT���� -> �븻�� �ؽ�ó�� �����ϸ� ��δ� ���� ���� ���͹����� -> �׷��� ź��Ʈ ������ ������ ����
				//XMStoreFloat3(&vTangent, XMVector3TransformNormal(XMLoadFloat3(&vTangent), PreTransformMatrix));
				
				WriteFile(hFile, &vPosition, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &vNormal, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &vTexcoord, sizeof(_float2), &dwByte, nullptr);
				WriteFile(hFile, &vTangent, sizeof(_float3), &dwByte, nullptr);
			}

 
			_uint iNumBones = meshInfo->mNumBones;
			WriteFile(hFile, &iNumBones, sizeof(_uint), &dwByte, nullptr);

			for (size_t i = 0; i < iNumBones; ++i)
			{

				aiBone* pAIBone = meshInfo->mBones[i]; //�� �޽ÿ� ������ �ִ� ���� ������ �ƴ� �� �������� �����Ѵ�.

				_float4x4 OffsetMatrix = {};

				memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
				
				XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

				//i��° ���� ���� ��� ����(i��°)
				WriteFile(hFile, &OffsetMatrix, sizeof(_float4x4), &dwByte, nullptr);
				//i��° ���� �̸� ����-> �� �ε��� �������� �뵵
				{
					_char			m_szName[MAX_PATH] = "";
					strcpy_s(m_szName, pAIBone->mName.data);
					_int LenName = strlen(m_szName) + 1;//�ι��ڱ���
					WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
					WriteFile(hFile, m_szName, LenName, &dwByte, nullptr);
				}
				
				_uint m_NumsWeights = pAIBone->mNumWeights;
				WriteFile(hFile, &m_NumsWeights, sizeof(_uint), &dwByte, nullptr);
				 

				//�̻��� ������ �ִ� ������ ���� (weights)
				for (size_t j = 0; j < pAIBone->mNumWeights; ++j)//�ش��ϴ� ���� ����ġ�� aiBone[��](������ ������ �ִ�)�� �ش�.
				{
					_uint ID;
					_float Weight;
					memcpy(&ID, &pAIBone->mWeights[j].mVertexId,sizeof(_uint));
					memcpy(&Weight, &pAIBone->mWeights[j].mWeight, sizeof(_float));
					WriteFile(hFile, &ID, sizeof(_uint), &dwByte, nullptr);//j��° ������ �ε���
					WriteFile(hFile, &Weight, sizeof(_float), &dwByte, nullptr); //j��° ������ ����ġ
					
				}
			}
			






			//�ϳ��� ������ ������ �� ������(�ﰢ��)�� �̷�� �������� �ε����� �����Ǿ��ִ�. -> �����ؼ� ��������.
			for (size_t j = 0; j < meshInfo->mNumFaces; j++)
			{
				int indices[3] = { 0,0,0 };
				indices[0] = meshInfo->mFaces[j].mIndices[0];//������.�ε���1-> �굵 �������ε� ���� �迭(FACE)�� ����ü���� �����迭��(ũ��FLOAT3)�� ������ �ε����� ����ִ�. 
				indices[1] = meshInfo->mFaces[j].mIndices[1];
				indices[2] = meshInfo->mFaces[j].mIndices[2];

				WriteFile(hFile, indices, sizeof(indices), &dwByte, nullptr);
			}
		}
		//���͸��� ���� �����̴ϴ�.

		_uint iNumMaterials = pAIScene->mNumMaterials;
		WriteFile(hFile, &iNumMaterials, sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i < iNumMaterials; ++i)
		{
			aiMaterial* pAIMaterial = pAIScene->mMaterials[i]; //���͸����� �����´�.
			for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)//�ؽ�ó Ÿ�Ը��� �ý�ó�� ������ ����� �����Ѵ�.
			{
				_uint iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(j));


				if (0 == iNumTextures && 1 == j)//Hero Arms
				{
					iNumTextures = 1;
					WriteFile(hFile, &iNumTextures, sizeof(_uint), &dwByte, nullptr);
					iNumTextures = 0;
					_char		szDrive[MAX_PATH] = "";
					_char		szDirectory[MAX_PATH] = "";
					_char		szFileName[MAX_PATH] = "hero_fpp_101_A_c";
					_char		szExt[MAX_PATH] = ".png";

					_char		szFullPath[MAX_PATH] = "";
					_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);


					strcpy_s(szFullPath, szDrive);
					strcat_s(szFullPath, szDirectory);
					strcat_s(szFullPath, szFileName);
					strcat_s(szFullPath, szExt);
					//DRIVE���+ ���丮 ��� + �ؽ�ó �̸� + �ؽ�ó Ȯ���� (�ؽ�ó�� �޽ö� ���� �־�ߵ�)
					_tchar		szPerfectPath[MAX_PATH] = TEXT("");
					int size = (MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH) + 1) * sizeof(wchar_t);//�ι��ڱ���
					WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
					WriteFile(hFile, szPerfectPath, size, &dwByte, nullptr);

				}

				else 
					WriteFile(hFile, &iNumTextures, sizeof(_uint), &dwByte, nullptr);//18���� �ؽ�ó Ÿ���� ���� � �ִ���
				
				for (size_t k = 0; k < iNumTextures; ++k)//diffuse�� 1�� �̻��� �� �� �ִ� 
				{
					aiString strTexturePath;//aiString Ÿ���� ���� ����
					//i��° ���׸����� jŸ���� �ؽ�óiNumTextures ���� k��° �ؽ�ó�� �ּ� strTexturePath�� �޾ƿ´�
					if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), k, &strTexturePath)))
						break;

					_char		szDrive[MAX_PATH] = "";
					_char		szDirectory[MAX_PATH] = "";
					_char		szFileName[MAX_PATH] = "";
					_char		szExt[MAX_PATH] = "";

					_char		szFullPath[MAX_PATH] = "";

					_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
					//���� ����� ���丮 fbx �� ������ �����ϴ� ���丮(MODEL���� �����´�.)
					_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
					//�ؽ�ó�� �ּҸ� �Ѱ�, �ؽ�ó�� ���� �̸� Ȯ���ڸ� �����´�. 

					strcpy_s(szFullPath, szDrive);
					strcat_s(szFullPath, szDirectory);
					strcat_s(szFullPath, szFileName);
					strcat_s(szFullPath, szExt);
					//DRIVE���+ ���丮 ��� + �ؽ�ó �̸� + �ؽ�ó Ȯ���� (�ؽ�ó�� �޽ö� ���� �־�ߵ�)
					_tchar		szPerfectPath[MAX_PATH] = TEXT("");

					int size = (MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH) + 1) * sizeof(wchar_t);//�ι��ڱ���

					WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
					WriteFile(hFile, szPerfectPath, size, &dwByte, nullptr);

				}

			}

		}
		/* ������ ����*/
		Write_Bones_Anim(hFile, pAIScene->mRootNode);

		/*�ִϸ��̼� ������ ä���*/
		_uint iNumAnimations = pAIScene->mNumAnimations;
		WriteFile(hFile, &iNumAnimations, sizeof(_uint), &dwByte, nullptr);


		for(int i=0;i<iNumAnimations;++i)
		{ 
			aiAnimation* pAnim = pAIScene->mAnimations[i];
			{
				_char			m_szName[MAX_PATH] = "";
				strcpy_s(m_szName,pAnim->mName.data);
				_int LenName = strlen(m_szName) + 1;//�ι��ڱ���
				WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
				WriteFile(hFile, m_szName, LenName, &dwByte, nullptr);
			}
			_double Duration= pAnim->mDuration;
			_double TickPerSecond = pAnim->mTicksPerSecond;
			WriteFile(hFile, &Duration, sizeof(_double), &dwByte, nullptr);
			WriteFile(hFile, &TickPerSecond, sizeof(_double), &dwByte, nullptr);
			_uint iNumChannels = pAnim->mNumChannels;
			WriteFile(hFile, &iNumChannels, sizeof(_uint), &dwByte, nullptr);
			
			for (int j = 0; j < iNumChannels; ++j)
			{
				aiNodeAnim* pChannel =pAnim->mChannels[j];
				{
					_char			m_szName[MAX_PATH] = "";
					strcpy_s(m_szName, pChannel->mNodeName.data);
					_int LenName = strlen(m_szName) + 1;//�ι��ڱ���
					WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
					WriteFile(hFile, m_szName, LenName, &dwByte, nullptr);
				}

				_uint iNumPositionsKeys =pChannel->mNumPositionKeys ;
				WriteFile(hFile, &iNumPositionsKeys, sizeof(_uint), &dwSize, nullptr);
				for (int k = 0; k < iNumPositionsKeys; ++k)
				{	
					_float3 vPos = {};
					_double mTime = { 0 };
					memcpy(&vPos, &(pChannel->mPositionKeys[k].mValue), sizeof(_float3));
					memcpy(&mTime, &(pChannel->mPositionKeys[k].mTime), sizeof(_double));
					WriteFile(hFile, &(vPos), sizeof(_float3), &dwByte, nullptr);
					WriteFile(hFile, &mTime, sizeof(_double), &dwByte, nullptr);

				}

				_uint iNumScalingKeys = pChannel->mNumScalingKeys;
				WriteFile(hFile, &iNumScalingKeys, sizeof(_uint), &dwSize, nullptr);
				for (int k = 0; k < iNumScalingKeys; ++k)
				{
					_float3 vScale = {};
					_double mTime = { 0 };
					memcpy(&vScale,&(pChannel->mScalingKeys[k].mValue), sizeof(_float3));
					memcpy(&mTime, &(pChannel->mScalingKeys[k].mTime), sizeof(_double));
					WriteFile(hFile, &vScale, sizeof(_float3), &dwSize, nullptr);
					WriteFile(hFile, &mTime, sizeof(_double), &dwByte, nullptr);
				}
				
				_uint iNumRotationKeys = pChannel->mNumRotationKeys;
				WriteFile(hFile, &iNumRotationKeys, sizeof(_uint), &dwSize, nullptr);
				for (int k = 0; k < iNumRotationKeys; ++k)
				{
					_float4 vRot = {};
					_double mTime = { 0 };
					memcpy(&vRot.x, &(pChannel->mRotationKeys[k].mValue.x), sizeof(_float));
					memcpy(&vRot.y, &(pChannel->mRotationKeys[k].mValue.y), sizeof(_float));
					memcpy(&vRot.z, &(pChannel->mRotationKeys[k].mValue.z), sizeof(_float));
					memcpy(&vRot.w, &(pChannel->mRotationKeys[k].mValue.w), sizeof(_float));

					memcpy(&mTime, &(pChannel->mRotationKeys[k].mTime), sizeof(_double));
					WriteFile(hFile, &vRot, sizeof(_float4), &dwSize, nullptr);
					WriteFile(hFile, &mTime, sizeof(_double), &dwByte, nullptr);
				}
				int a = 0;


			}

		
		}



		CloseHandle(hFile);
	}


	return S_OK;
}

HRESULT CModel::Write_Bones_Anim(HANDLE hFile, const aiNode* pNode)
{
	_ulong dwByte = 0;
	{
		_char szName[MAX_PATH] = "";
		strcpy_s(szName, pNode->mName.data);// ���� �����ϸ� �� (�ڽ� )���� �̸��� �����´� 
		_int LenName = strlen(szName) + 1;//�ι��ڱ���
		WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, &szName, LenName, &dwByte, nullptr);
	}
	{
		_float4x4 TransformationMatrix;
		XMStoreFloat4x4(&TransformationMatrix, XMMatrixIdentity());
		memcpy(&TransformationMatrix, &pNode->mTransformation, sizeof(_float4x4));//aiNode�κ��� �ڽ��� ����� �����´� -> �θ� ���ؿ����� �ڽŸ��� ���
		XMStoreFloat4x4(&TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&TransformationMatrix)));
		WriteFile(hFile, &TransformationMatrix, sizeof(_float4x4), &dwByte, nullptr);
	}
	_int iNumChildren = pNode->mNumChildren;
	WriteFile(hFile, &iNumChildren, sizeof(_int), &dwByte, nullptr);


	for (int i = 0; i < iNumChildren; ++i)
	{
		Write_Bones_Anim(hFile, pNode->mChildren[i]);
	}


	return S_OK;
}
