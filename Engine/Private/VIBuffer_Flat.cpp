#include"VIBuffer_Flat.h"
CVIBuffer_Flat::CVIBuffer_Flat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Flat::CVIBuffer_Flat(const CVIBuffer_Flat& Prototype)
	: CVIBuffer(Prototype)
	/*, m_vecVertices{ Prototype.m_vecVertices}*/
	, m_IsCopied{ true }
	/*, m_Desc{Prototype.m_Desc}
	, m_iNumVerticesX{Prototype.m_iNumVerticesX}
	, m_iNumVerticesZ{Prototype.m_iNumVerticesZ}*/

{
}

HRESULT CVIBuffer_Flat::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Flat::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		VIBUFFER_FLAT_DESC* pDesc = static_cast<VIBUFFER_FLAT_DESC*>(pArg);
		m_Desc = *pDesc;

		m_iNumVerticesX = m_Desc.iNumX_Vertices;
		m_iNumVerticesZ = m_Desc.iNumZ_Vertices;
	}
	else 
	{	
		m_Desc.fSizeX = 1.f;
		m_Desc.fSizeX = 1.f;
		m_iNumVerticesX =m_Desc.iNumX_Vertices = 50;
		m_iNumVerticesZ =m_Desc.iNumZ_Vertices = 50;
	}



	m_iNumVertexBuffers = 1;

	m_iVertexStride = sizeof(VTXCUBECOL);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_iIndexStride = /*m_iNumVertices >= 65534 ? sizeof(_uint) : sizeof(_ushort)*/sizeof(_uint);
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER	


	m_vecVertices.resize(m_iNumVertices);

	VTXCUBECOL* pVertices = new VTXCUBECOL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXCUBECOL) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(j, 0.f, i);
			pVertices[iIndex].vColor = _float3(1.f, 1.f, 1.f);
			//for terrain Picking;
			m_vecVertices[iIndex] = _float4(j, 0.f, i, 1.f);
		}
	}


#pragma endregion

#pragma region INDEX_BUFFER


	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex,
			};

 
			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

		


			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

		}
	}

#pragma endregion


	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;




	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	ZeroMemory(&m_SubResourceDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceDesc.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;


	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

	return S_OK;
}

CVIBuffer_Flat* CVIBuffer_Flat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Flat* pInstance = new CVIBuffer_Flat(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Flat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Flat::Clone(void* pArg)
{
	CVIBuffer_Flat* pInstance = new CVIBuffer_Flat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Flat");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_Flat::Free()
{
	__super::Free();

}
