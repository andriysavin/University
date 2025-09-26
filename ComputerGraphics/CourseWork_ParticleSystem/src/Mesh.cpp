//////////////////////////////////////////////////////////////////////////
#include "mesh.h"
//////////////////////////////////////////////////////////////////////////
CMesh::CMesh(LPDIRECT3DDEVICE9 const pDevice):
	m_pMesh				(NULL				),
	m_pDevice			(pDevice			),
	m_hRes				(S_OK				),
	m_dwNumMaterials	(0					),
	m_pAdjacencyBuffer  (NULL				),
	m_pMeshVB			(NULL				),
	m_pMeshIB			(NULL				)

{
	
}
//////////////////////////////////////////////////////////////////////////
CMesh::~CMesh(void)
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pAdjacencyBuffer);

	for( UINT i = 0 ; i < m_dwNumMaterials ; i++ )
		 SAFE_RELEASE( m_arMeshTextures[i] );

	SAFE_RELEASE(m_pMeshVB );
	SAFE_RELEASE(m_pMeshIB );

}
//////////////////////////////////////////////////////////////////////////
void CMesh::Load(const std::string& sMeshFileName,
				 const std::string& sTexturesPath /* = ""*/,bool blGenerateNormals /*= false*/)
{
	LPD3DXBUFFER	pMaterialBuffer  = NULL;
	LPD3DXMATERIAL  pMaterials;
	std::vector<std::string>  vTexNames;
	
	m_hRes = D3DXLoadMeshFromX(
		(LPSTR) sMeshFileName.c_str(), 
		D3DXMESH_MANAGED, 
		m_pDevice, 
		&m_pAdjacencyBuffer, 
		&pMaterialBuffer, 
		nullptr,
		&m_dwNumMaterials, 
		&m_pMesh );

	TestResult(m_hRes,"Error reading model from file " + sMeshFileName + "\n");

	if(blGenerateNormals)
	{
		LPD3DXMESH   pMeshTemp;
		m_hRes = m_pMesh->CloneMeshFVF( m_pMesh->GetOptions(), m_pMesh->GetFVF() | D3DFVF_NORMAL, 
			m_pDevice, &pMeshTemp );

		m_pMesh->Release();
		m_pMesh = pMeshTemp;
		D3DXComputeNormals( m_pMesh, NULL );
	}

	pMaterials = (D3DXMATERIAL*)pMaterialBuffer->GetBufferPointer();

	m_arMeshMaterials.resize(m_dwNumMaterials) ;
	m_arMeshTextures .resize(m_dwNumMaterials) ;  
	
	for(DWORD i = 0 ; i < m_dwNumMaterials ; i++ )
	{
		m_arMeshMaterials[i]			= pMaterials [i].MatD3D;
		m_arMeshMaterials[i].Ambient    = m_arMeshMaterials[i].Diffuse;
		m_arMeshTextures [i]			= NULL;
		
		if (pMaterials[i].pTextureFilename != NULL)
		{
			// Load the texture
			bool blBreak = false;
			for(DWORD j= 0 ; j < vTexNames.size() ; j++)
				if(vTexNames[j] == pMaterials[i].pTextureFilename)
				{
					m_arMeshTextures[i] = m_arMeshTextures[j];
					blBreak = true;
					break;
				}

			if(blBreak) continue;

			m_hRes = D3DXCreateTextureFromFile( m_pDevice, 
				(sTexturesPath + pMaterials[i].pTextureFilename).c_str() , &m_arMeshTextures[i] );

			//TestResult(m_hRes,"Error reading model from file  " + 
			//		sTexturesPath + pMaterials[i].pTextureFilename + "\n");

			vTexNames.push_back(pMaterials[i].pTextureFilename);
		}
	}
	SAFE_RELEASE(pMaterialBuffer);

	m_hRes = m_pMesh->GetVertexBuffer( &m_pMeshVB );	
	TestResult(m_hRes,"Error m_pMesh->GetVertexBuffer\n" );
	m_hRes = m_pMesh->GetIndexBuffer ( &m_pMeshIB );	
	TestResult(m_hRes,"Error m_pMesh->GetIndexBuffer\n"); 
	m_dwMeshFVF = m_pMesh->GetFVF ( );	

	
	DWORD dwNumVertices;
	D3DVERTEXBUFFER_DESC  VBDesc;
	m_pMeshVB->GetDesc(&VBDesc);

	dwNumVertices = m_pMesh->GetNumVertices();
	m_dwVertSize = VBDesc.Size / dwNumVertices;
}
//////////////////////////////////////////////////////////////////////////
void CMesh::Render()
{
	for (DWORD iMaterial = 0; iMaterial < m_dwNumMaterials; iMaterial++)
	{
		m_hRes = m_pDevice->SetMaterial(&m_arMeshMaterials[iMaterial]);

		if (m_arMeshTextures[iMaterial])
			m_hRes = m_pDevice->SetTexture(0, m_arMeshTextures[iMaterial]);

		m_hRes = m_pMesh->DrawSubset(iMaterial);
	}
}
//////////////////////////////////////////////////////////////////////////

