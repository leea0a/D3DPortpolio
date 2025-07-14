#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct
	{
		HINSTANCE		hInstance;
		HWND			hWnd;
		bool			isWindowed;
		unsigned int	iNumLevels;
		unsigned int	iViewportWidth;
		unsigned int	iViewportHeight;

	}ENGINE_DESC;


	typedef struct
	{
		enum TYPE {TYPE_POINT, TYPE_DIRECTIONAL, TYPE_END};


		TYPE		eType;
		XMFLOAT4	vDirection; 
		XMFLOAT4	vPosition;
		float		fRange;
		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;
	}LIGHT_DESC;

	typedef struct
	{
		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;

	}METERIAL_DESC;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;

		static const unsigned int					iNumElements = { 1 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[1];
	}VTXPOS;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vSize;
	}VTXPOINT;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[2];
	}VTXPOSTEX;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNORTEX;

	typedef struct
	{
		class CTexture* pTextures[AI_TEXTURE_TYPE_MAX];
	}MESHMATERIAL;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexcoord;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[2];
	}VTXCUBE;

	typedef struct ENGINE_DLL
	{

		XMFLOAT3	vPosition;
		XMFLOAT3    vNormal;
		XMFLOAT3    vTexcoord;

		static const unsigned int					iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXCUBETEX;

	typedef struct ENGINE_DLL
	{

		XMFLOAT3	vPosition;
		XMFLOAT3    vColor;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXCUBECOL;

	/* vertex for nonamination*/
	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;
		XMFLOAT2			vTexUV;

		static const unsigned int					iNumElements = { 4 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXMESH;
	/* vertex for animation*/
	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;
		XMFLOAT2			vTexUV;
		XMUINT4			vBlendIndices; //�޽ð� ������ �ִ� ������ ������ �ִ� ���� �ε���
		XMFLOAT4		vBlendWeights;  //�� ���� ������ �ִ� ������ ���� 1�̳������� ����� �����-> blend indices �� 0~4
		XMUINT4				vBlendIndex;
		XMFLOAT4			vBlendWeight;
		static const unsigned int					iNumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXANIMMESH;

	
	//ASSIMP���� �����ϴ� Ű�����Ӹ����� �ִϸ��̼� ���������� 
	//����� �ƴ϶� ���ͷ� SRT�� �����Ѵ� ( �������� �Ϸ��� ���� �ؾߵ� )
	//��� ��°�� ���� ������ �Ѳ����� �������� ó���� �Ҽ� ������ 
	typedef struct
	{
		XMFLOAT3 vScale;
		XMFLOAT4 vRotation;
		XMFLOAT3 vPosition;
		float	fKeyFramePosition;
		float fTime;
	}KEYFRAME;

	typedef struct
	{
		XMFLOAT4		vRight, vUp, vLook, vTranslation;
		XMFLOAT2		vLifeTime;

	}VTXINSTANCE;


	typedef struct ENGINE_DLL
	{
		static const unsigned int					iNumElements = { 7 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXRECTINSTANCE;

	typedef struct ENGINE_DLL
	{
		static const unsigned int					iNumElements = { 7 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXPOINTINSTANCE;

	/*For Data*/


	typedef struct tagFaceIndices32
	{
		unsigned long		_0, _1, _2;
	}FACEINDICES32;

	typedef struct tagBinBoneNode
	{

		char cName[MAX_PATH];
		char cParent[MAX_PATH];
		int  iDepth;
		XMFLOAT4X4 mTransform;

	}DATA_BINNODE;

	typedef struct tagBinBone
	{
		char		cNames[MAX_PATH];
		XMFLOAT4X4	mOffsetTransform;
	}DATA_BINBONE;

	typedef struct tagBinMesh
	{
		char				cName[MAX_PATH];
		int					iMaterialIndex;

		int					NumVertices;
		VTXMESH* pNonAnimVertices;
		VTXANIMMESH* pAnimVertices;

		int					iNumPrimitives;
		FACEINDICES32* pIndices;

		int					iNumBones;
		DATA_BINBONE* pBones;

	}DATA_BINMESH;

	typedef struct tagBinMaterial
	{

		char cNames[AI_TEXTURE_TYPE_MAX][MAX_PATH];

	}DATA_BINMATERIAL;

	typedef struct tagBinChannel
	{

		char				szName[MAX_PATH];
		int					iNumKeyFrames;
		KEYFRAME* pKeyFrames;

	}DATA_BINCHANNEL;

	typedef struct tagBinAnim
	{

		bool				bLoop;
		char				szName[MAX_PATH];
		int					iNumChannels;
		float				fDuration;
		float				fTickPerSecond;
		DATA_BINCHANNEL* pBinChannel;


	}DATA_BINANIM;

	typedef struct tagBinScene
	{

		int iNodeCount;
		DATA_BINNODE* pBinNodes;

		int iMaterialCount;
		DATA_BINMATERIAL* pBinMaterial;

		int iMeshCount;
		DATA_BINMESH* pBinMesh;

		int iNumAnimations;
		DATA_BINANIM* pBinAnim;

	}DATA_BINSCENE;

	typedef struct tagModelMaterial
	{
		class CTexture* pMaterials[AI_TEXTURE_TYPE_MAX];
	}MODELMATERIAL;


	typedef struct ENGINE_DLL
	{
		XMFLOAT3 vPointA;
		XMFLOAT3 vPointB;
		XMFLOAT3 vPointC;
	}TRIANGLE_VERTICES;
}



	//typedef struct
	//{
	//	XMFLOAT3		vPosition;
	//	XMFLOAT2		vTexcoord0;
	//	XMFLOAT2		vTexcoord1;
	//}VTXPOSTEX;




#endif // Engine_Struct_h__
