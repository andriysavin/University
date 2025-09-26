//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////

#include <stdexcept> 
#include <vector>
#include <string>

#include <d3d9.h>
#include <d3dx9.h>

#include "Utils.h"
#include "W32Exception.h"

//////////////////////////////////////////////////////////////////////////
class CMesh
{
public:
					CMesh(LPDIRECT3DDEVICE9 const pDevice);
	virtual		   ~CMesh(void);
	void			Load(const std::string& sMeshFileName,
						 const std::string& sTexturesPath = "",bool blGenerateNormals = false);
	void			Render();
private:

	LPDIRECT3DDEVICE9 const		m_pDevice;
	LPD3DXMESH					m_pMesh;
	LPDIRECT3DVERTEXBUFFER9		m_pMeshVB;
	LPDIRECT3DINDEXBUFFER9		m_pMeshIB;
	DWORD						m_dwMeshFVF;
	HRESULT						m_hRes;
	
	LPD3DXBUFFER				m_pAdjacencyBuffer;
	DWORD						m_dwNumMaterials;
	DWORD						m_dwVertSize ;

	std::vector<D3DMATERIAL9>		m_arMeshMaterials;
	std::vector<LPDIRECT3DTEXTURE9>	m_arMeshTextures ;
};
//////////////////////////////////////////////////////////////////////////
