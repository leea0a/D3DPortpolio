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
		enum TYPE { TYPE_POINT, TYPE_DIRECTOINAL, TYPE_SPOT, TYPE_END };

		TYPE		eType;
		XMFLOAT4	vDirection; 
		XMFLOAT4	vPosition;
		float		fRange;
		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;
		_bool		TurnOn_Off;

		/* ����Ʈ����Ʈ ��*/
		float fConeAngle;
		_float fAttenuation;


	}LIGHT_DESC;

	typedef struct
	{
		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;

	}METERIAL_DESC;

	/*Light.Diffuse * Mtrl.vDiffuse;*/

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

		XMFLOAT3	vPosition;
		XMFLOAT3    vTexcoord;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
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
		XMUINT4			vBlendIndices; //�޽ð� ������ �ִ� ������ ������ �ִ� ���� �ε���
		XMFLOAT4		vBlendWeights;  //�� ���� ������ �ִ� ������ ���� 1�̳������� ����� �����-> blend indices �� 0~4
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
	}KEYFRAME;


	typedef struct ENGINE_DLL
	{
		XMFLOAT3 vPointA;
		XMFLOAT3 vPointB;
		XMFLOAT3 vPointC;
	}TRIANGLE_VERTICES;



	/* v�������� ��ƼŬ�ν��Ͻ��� �������� ���ÿ��� �����ϱ����� (���ؽ� ���ۿ� �����Ұ�)Ʈ������ ����ü*/
	typedef struct ENGINE_DLL
	{
		XMFLOAT4 vRight, vUp, vLook, vTranslation;
		XMFLOAT2 vLifeTime; //x���� ���� lifetime ���� ,y���� ���Ѱ�
	}VTXINSTANCE;
	typedef struct ENGINE_DLL
	{
		static const unsigned int iNumElements = { 7 };
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXRECTINSTANCE;

	typedef struct ENGINE_DLL
	{
		static const unsigned int iNumElements = { 7 };
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXPOINTINSTANCE;



}


#endif // Engine_Struct_h__
