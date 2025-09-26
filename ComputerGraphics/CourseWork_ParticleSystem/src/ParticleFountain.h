//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include "ParticleSystem.h"
#include <stdlib.h>
#include <string>
using namespace std;

#define _USE_MATH_DEFINES
#include <math.h>

#define D3DFVF_POINTVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

typedef struct tagPOINTVERTEX
{
	FLOAT    x, y, z;
	D3DCOLOR  diffuse;
} POINTVERTEX;
//////////////////////////////////////////////////////////////////////////
class ParticleFountain: public ParticleSystem
{
public:
					ParticleFountain(LPDIRECT3DDEVICE9 pd3dDevice,string sTextureFileName,
						float		fTimeToEmit		 ,D3DXCOLOR		clrDiffuse,float fFade,
						float		fParticleLifeTime,float			fRandomfParticleLifeTime,
						D3DXVECTOR3 vVelocity0		 ,float			fRandomVelocity, 
						DWORD       dwQuantity       ,float         fRandomQuantity,
						D3DXVECTOR3 vOrigin		     ,float fGravity,float fDisperseAngle,
						float fParticleSize);
					ParticleFountain(LPDIRECT3DDEVICE9 pd3dDevice,string sTextureFileName,
						string sParametrsFileName);

	virtual void	OnRender(LPPARTICLEINFO parPartInfos,DWORD dwNumParticles);
	virtual		   ~ParticleFountain(void);

	virtual void    OnEmitParticle(LPPARTICLECREATEPARAM lpParticleParam);
	void			SetOrigin(D3DXVECTOR3& vOrigin){m_vOrigin = vOrigin;}
protected:
	DWORD			FtoDW( FLOAT f ) { return *((DWORD*)&f); }
   
	

	D3DXCOLOR		m_clrDiffuse;
	float			m_fFade;
	float			m_fParticleLifeTime;
	float			m_fRandomfParticleLifeTime;
	D3DXVECTOR3		m_vOrigin;
	D3DXVECTOR3		m_vVelocity0;
	float			m_fRandomVelocity;
	float			m_fGravity;
	float			m_fDisperseAngle; // For accuracy of calculations, it is stored multiplied by 1000
	float           m_fParticleSize;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPDIRECT3DTEXTURE9		m_pParticleTexture;	

	DWORD					m_dwSRCBLEND;
	DWORD					m_dwDSTBLEND;
};
//////////////////////////////////////////////////////////////////////////
