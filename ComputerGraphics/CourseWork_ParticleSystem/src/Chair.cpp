//////////////////////////////////////////////////////////////////////////
#include "chair.h"
//////////////////////////////////////////////////////////////////////////
Chair::Chair(LPDIRECT3DDEVICE9 const pDevice) : Mesh3DObject(pDevice)
{
	Load("assets\\chair.x", ".\\assets\\",true);
	ChangePos(D3DXVECTOR3(0,28.0,110)); 
	SetScale (D3DXVECTOR3(5,5,5));
}
//////////////////////////////////////////////////////////////////////////
Chair::~Chair(void)
{
}
//////////////////////////////////////////////////////////////////////////
void	Chair::Update() 
{
}
//////////////////////////////////////////////////////////////////////////
void	Chair::Render() 
{
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	light.Specular.r = 1.0f;
	light.Specular.g = 1.0f;
	light.Specular.b = 1.0f;
	light.Specular.a = 1.0f;

	light.Diffuse.a  = 1.0f;
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;
	
	light.Ambient.a  = 0.5f;
	light.Ambient.r  = 0.5f;
	light.Ambient.g  = 0.5f;
	light.Ambient.b  = 1.0f;
	
	m_pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE , D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE , D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);

	m_pDevice->SetLight   ( 0, &light );
	m_pDevice->LightEnable( 0, TRUE   );
	m_pDevice->SetRenderState( D3DRS_COLORVERTEX	 , TRUE  );
	m_pDevice->SetRenderState( D3DRS_LIGHTING		 , TRUE  );
	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	m_pDevice->SetRenderState( D3DRS_SPECULARENABLE  , TRUE  );
	m_pDevice->SetRenderState( D3DRS_ZENABLE		 , TRUE  );
	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS,TRUE);
	
	m_pDevice->SetTransform( D3DTS_WORLD, &m_mWorld  );

	m_pDevice->SetTexture(0,NULL);

	m_pMesh->Render();

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS,FALSE);
}
//////////////////////////////////////////////////////////////////////////
