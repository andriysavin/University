//////////////////////////////////////////////////////////////////////////
#include "room.h"
//////////////////////////////////////////////////////////////////////////
Room::Room(LPDIRECT3DDEVICE9 const pDevice): Mesh3DObject(pDevice)
{
	Load("assets\\room.x", ".\\assets\\",true);
}
//////////////////////////////////////////////////////////////////////////
Room::~Room(void)
{
}
//////////////////////////////////////////////////////////////////////////
void	Room::Update() 
{
}
//////////////////////////////////////////////////////////////////////////
void	Room::Render() 
{
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	light.Diffuse.a  = 1.0f;
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;
	
	light.Ambient.a  = 0.9f;
	light.Ambient.r  = 0.9f;
	light.Ambient.g  = 0.9f;
	light.Ambient.b  = 1.0f;
	
	m_pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE , D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE , D3DMCS_MATERIAL);
	
	m_pDevice->SetLight   ( 0, &light );
	m_pDevice->LightEnable( 0, TRUE   );
	m_pDevice->SetRenderState( D3DRS_COLORVERTEX	 , TRUE  );
	m_pDevice->SetRenderState( D3DRS_LIGHTING		 , TRUE  );
	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	m_pDevice->SetRenderState( D3DRS_SPECULARENABLE  , FALSE  );
	m_pDevice->SetRenderState( D3DRS_ZENABLE		 , TRUE  );

	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pDevice->SetTransform( D3DTS_WORLD, &m_mWorld  );

	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	
	m_pMesh->Render();
}
//////////////////////////////////////////////////////////////////////////
