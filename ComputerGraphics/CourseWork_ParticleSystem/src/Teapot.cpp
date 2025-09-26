//////////////////////////////////////////////////////////////////////////
#include "teapot.h"
//////////////////////////////////////////////////////////////////////////
Teapot::Teapot(LPDIRECT3DDEVICE9 const pDevice) : Mesh3DObject(pDevice)
{
	Load("assets\\teapot.x", ".\\assets\\", true);
	ChangePos(D3DXVECTOR3(0, 45.8f, 70));
	SetScale(D3DXVECTOR3(6, 6, 6));

	m_pSteam = new ParticleFountain(m_pDevice, "assets\\water.tga", "assets\\fountain.dat");

	m_iMaxFogQuantity = m_pSteam->GetQuantity();
	m_iBaseFogQuantity = m_iMaxFogQuantity / 2;
	m_pSteam->SetQuantity(m_iBaseFogQuantity);
	m_iTemperature = 235;

	m_pSteam->Start();
}
//////////////////////////////////////////////////////////////////////////
Teapot::~Teapot(void)
{
	SAFE_DELETE(m_pSteam);
}
//////////////////////////////////////////////////////////////////////////
void	Teapot::Update() 
{
	m_pSteam->Update();
}
//////////////////////////////////////////////////////////////////////////
void	Teapot::Render() 
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
	light.Diffuse.r  = 0.0f;
	light.Diffuse.g  = 0.5f;
	light.Diffuse.b  = 0.5f;

	light.Ambient.a  = 1.0f;
	light.Ambient.r  = 0.3f;
	light.Ambient.g  = 0.3f;
	light.Ambient.b  = 0.3f;

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

	D3DXMATRIX matWorld ;
	D3DXMatrixIdentity(&matWorld );
	m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	
	m_pSteam->Render();

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS,FALSE);
}
//////////////////////////////////////////////////////////////////////////
