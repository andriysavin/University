//////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include "ParticleFountain.h"
#define  M_PI 3.1415926535897932384626433832795
#include <fstream>
using namespace std;

//////////////////////////////////////////////////////////////////////////
ParticleFountain::ParticleFountain(LPDIRECT3DDEVICE9 pd3dDevice,string sTextureFileName,
	float		fTimeToEmit		 ,D3DXCOLOR		clrDiffuse,float fFade,
	float		fParticleLifeTime,float			fRandomfParticleLifeTime,
	D3DXVECTOR3 vVelocity0		 ,float			fRandomVelocity, 
	DWORD       dwQuantity       ,float         fRandomQuantity,
	D3DXVECTOR3 vOrigin		     ,float fGravity,float fDisperseAngle,float fParticleSize):

	ParticleSystem(dwQuantity,fRandomQuantity,
		fParticleLifeTime * (1 + fRandomfParticleLifeTime),fTimeToEmit),
	
	
	m_clrDiffuse(clrDiffuse),
	m_fFade(fFade),
	
	m_fParticleLifeTime(fParticleLifeTime), m_fRandomfParticleLifeTime(fRandomfParticleLifeTime * fParticleLifeTime),
	m_vVelocity0(vVelocity0)			  , m_fRandomVelocity(fRandomVelocity ), 

	m_vOrigin(vOrigin),
	m_fGravity(fGravity),
	m_fDisperseAngle(fDisperseAngle * 1000.0f), // For accuracy of calculations, it is stored multiplied by 1000
	m_pd3dDevice(pd3dDevice),
	m_fParticleSize(fParticleSize),
	m_dwSRCBLEND( D3DBLEND_ONE ),
	m_dwDSTBLEND( D3DBLEND_ONE )

{
	 m_pd3dDevice->CreateVertexBuffer( m_dwMaxParticles * sizeof(POINTVERTEX),
		 D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 
		D3DFVF_POINTVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL );

	 D3DXCreateTextureFromFile(m_pd3dDevice,sTextureFileName.c_str(),&m_pParticleTexture);
}

ParticleFountain::ParticleFountain(LPDIRECT3DDEVICE9 pd3dDevice, string sTextureFileName,
	string sParametrsFileName) :ParticleSystem(sParametrsFileName)
{
	char sTemp[256];

	float			fFade = 0.0f;
	float			fParticleLifeTime = 1.0f;
	float			fRandomfParticleLifeTime = 0.0f;
	float			fRandomVelocity = 0.0f;
	float			fGravity = 0.0f;
	float			fDisperseAngle = 3.14f;
	float			fParticleSize = 1.0f;

	D3DXCOLOR		clrDiffuse(255, 255, 255, 255);
	D3DXVECTOR3		vVelocity0(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3		vOrigin(0.0f, 0.0f, 0.0f);

	m_dwSRCBLEND = D3DBLEND_ONE;
	m_dwDSTBLEND = D3DBLEND_ONE;

	ifstream f(sParametrsFileName.c_str());

	if (f)
	{
		while (!f.eof())
		{
			f.getline(sTemp, 255);

			if (!strncmp(sTemp, "Fade", strlen("Fade")))
			{
				fFade = (float)atof(strlen("Fade") + sTemp);
			}

			if (!strncmp(sTemp, "ParticleLifeTime", strlen("ParticleLifeTime")))
			{
				fParticleLifeTime = (float)atof(strlen("ParticleLifeTime") + sTemp);
			}

			if (!strncmp(sTemp, "RandomParticleLifeTime", strlen("RandomParticleLifeTime")))
			{
				fRandomfParticleLifeTime = (float)atof(strlen("RandomParticleLifeTime") + sTemp);
			}

			if (!strncmp(sTemp, "RandomVelocity", strlen("RandomVelocity")))
			{
				fRandomVelocity = (float)atof(strlen("RandomVelocity") + sTemp);
			}

			if (!strncmp(sTemp, "Gravity", strlen("Gravity")))
			{
				fGravity = (float)atof(strlen("Gravity") + sTemp);
			}

			if (!strncmp(sTemp, "DisperseAngle", strlen("DisperseAngle")))
			{
				fDisperseAngle = (float)atof(strlen("DisperseAngle") + sTemp) * ((float)M_PI) / 180.0f;
			}

			if (!strncmp(sTemp, "ParticleSize", strlen("ParticleSize")))
			{
				fParticleSize = (float)atof(strlen("ParticleSize") + sTemp);
			}

			if (!strncmp(sTemp, "Velocity", strlen("Velocity")))
			{
				sscanf(sTemp + strlen("Velocity"), "%f%f%f", &vVelocity0.x, &vVelocity0.y, &vVelocity0.z);
			}

			if (!strncmp(sTemp, "Origin", strlen("Origin")))
			{
				sscanf(sTemp + strlen("Origin"), "%f%f%f", &vOrigin.x, &vOrigin.y, &vOrigin.z);
			}

			if (!strncmp(sTemp, "DiffuseColor ", strlen("DiffuseColor ")))
			{
				sscanf(sTemp + strlen("DiffuseColor "), "%f%f%f%f",
					&clrDiffuse.a, &clrDiffuse.r, &clrDiffuse.g, &clrDiffuse.b);
			}

			if (!strncmp(sTemp, "SRCBLEND", strlen("SRCBLEND")))
			{
				m_dwSRCBLEND = atol(strlen("SRCBLEND") + sTemp);
			}

			if (!strncmp(sTemp, "DSTBLEND", strlen("DSTBLEND")))
			{
				m_dwDSTBLEND = atol(strlen("DSTBLEND") + sTemp);
			}


		}

		f.close();
	}

	m_clrDiffuse = clrDiffuse;
	m_fFade = fFade;
	m_fParticleLifeTime = fParticleLifeTime;
	m_fRandomfParticleLifeTime = fRandomfParticleLifeTime * fParticleLifeTime;
	m_vVelocity0 = vVelocity0;
	m_fRandomVelocity = fRandomVelocity;
	m_vOrigin = vOrigin;
	m_fGravity = fGravity;
	m_fDisperseAngle = fDisperseAngle * 1000.0f;
	m_pd3dDevice = pd3dDevice;
	m_fParticleSize = fParticleSize;

	m_pd3dDevice->CreateVertexBuffer(m_dwMaxParticles * sizeof(POINTVERTEX),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,
		D3DFVF_POINTVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL);

	D3DXCreateTextureFromFile(m_pd3dDevice, sTextureFileName.c_str(), &m_pParticleTexture);
}
//////////////////////////////////////////////////////////////////////////
ParticleFountain::~ParticleFountain(void)
{
	if (m_pParticleTexture) m_pParticleTexture->Release();
	if (m_pVB) m_pVB->Release();
}
//////////////////////////////////////////////////////////////////////////
void ParticleFountain::OnEmitParticle(LPPARTICLECREATEPARAM lpParticleParam)
{
	D3DXVECTOR4 vVel0;
	D3DXMATRIX  mVelTransform;

	float		fAngle1 = Rand(- m_fDisperseAngle,m_fDisperseAngle)/1000.0f;
	float		fAngle2 = Rand(- m_fDisperseAngle,m_fDisperseAngle)/1000.0f;
	float		fAngle3 = Rand(- m_fDisperseAngle,m_fDisperseAngle)/1000.0f;

	D3DXMatrixRotationYawPitchRoll(&mVelTransform,fAngle1,fAngle2,fAngle3);
	D3DXVec3Transform(&vVel0,&m_vVelocity0,&mVelTransform);
	D3DXVec4Scale(&vVel0,&vVel0,1 + Rand(-m_fRandomVelocity , m_fRandomVelocity ) );
   
	lpParticleParam->clrDiffuse0	= m_clrDiffuse;
	lpParticleParam->fFade			= m_fFade;
	
	lpParticleParam->fLifeTime		=  
		Rand(m_fParticleLifeTime - m_fRandomfParticleLifeTime,
			 m_fParticleLifeTime + m_fRandomfParticleLifeTime);

	lpParticleParam->vAccel0		= D3DXVECTOR3(0,m_fGravity,0);
	lpParticleParam->vPos0			= m_vOrigin;
	lpParticleParam->vVel0.x		= vVel0.x;
	lpParticleParam->vVel0.y		= vVel0.y;
	lpParticleParam->vVel0.z		= vVel0.z;
}
//////////////////////////////////////////////////////////////////////////
void ParticleFountain::OnRender(LPPARTICLEINFO parPartInfos,
								 DWORD dwNumParticles)
{
	POINTVERTEX* pVertices;

	m_pVB->Lock( 0, sizeof(POINTVERTEX)*dwNumParticles, (void**)&pVertices, D3DLOCK_NOOVERWRITE )  ;
			
	for(DWORD i = 0; i < dwNumParticles; i++) 
	{
		pVertices[i].diffuse = D3DCOLOR_RGBA((DWORD)parPartInfos[i].clrDiffuse.r, 
												(DWORD)parPartInfos[i].clrDiffuse.g,
												(DWORD)parPartInfos[i].clrDiffuse.b, 
												(DWORD)parPartInfos[i].clrDiffuse.a);
		pVertices[i].x		 = parPartInfos[i].vPos.x;
		pVertices[i].y		 = parPartInfos[i].vPos.y;
		pVertices[i].z		 = parPartInfos[i].vPos.z;	
	}
	    
	m_pVB->Unlock();

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING , FALSE);
	m_pd3dDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE , D3DMCS_COLOR1);
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  m_dwSRCBLEND ); //!!!!
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, m_dwDSTBLEND ); //!!!!
	m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, false); 	
	m_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(m_fParticleSize) );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(0.00f) );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.00f) );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.00f) );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(2.00f) );
	
    m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(POINTVERTEX) );
    m_pd3dDevice->SetFVF(D3DFVF_POINTVERTEX);
	m_pd3dDevice->SetTexture(0,m_pParticleTexture);

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP  , D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP  , D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE  );
	
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, 0,dwNumParticles);
	
	m_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );
	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, true); 
}
//////////////////////////////////////////////////////////////////////////
