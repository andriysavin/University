//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#define PS_TIME_INFINITE (0.0f)
//////////////////////////////////////////////////////////////////////////
#include "Particle.h"
#include "Utils.h"
#include <string>
using namespace std;

typedef struct tagPARTICLEINFO
{
	D3DXVECTOR3	vPos;
	D3DXCOLOR   clrDiffuse;
}PARTICLEINFO,*LPPARTICLEINFO;


typedef struct tagPSCREATEINFO
{
	DWORD dwQuantity;
	float fRandomQuantity;
	float fMaxParticleLifeTime;
	float fTimeToEmit;
}PSCREATEINFO,*LPPSCREATEINFO;

//////////////////////////////////////////////////////////////////////////
class ParticleSystem
{
public:
					ParticleSystem(DWORD dwQuantity,float fRandomQuantity,float fMaxParticleLifeTime,float fTimeToEmit);
					ParticleSystem(string sParametrsFileName);
	virtual		   ~ParticleSystem(void);

	void			Render();
	virtual void	OnRender(LPPARTICLEINFO parPartInfos,DWORD dwNumParticles)=0;
	virtual void    OnEmitParticle(LPPARTICLECREATEPARAM lpParticleParam)=0;
	void			Update(); 
	void            Start() ;
	void            SetQuantity(DWORD dwQuantity){m_dwQuantity = dwQuantity;}	
	DWORD           GetQuantity(){return m_dwQuantity ;}	
protected:
	float           GetTime(){return timeGetTime()/1000.0f; }
	float		    Rand(float fMin,float fMax);

	LPPARTICLEINFO	m_parPartInfos;
	Particle*		m_parParticles;
	DWORD			m_dwMaxParticles;
	float           m_fTimeToEmit;
	float           m_fStartTime;
	bool			m_blStarted;

	// The number of particles emitted per unit of time (intensity)
	DWORD			m_dwQuantity;
	float			m_fRandomQuantity;
};
//////////////////////////////////////////////////////////////////////////
inline float ParticleSystem::Rand(float fMin,float fMax)
{ return ((rand() * (fMax - fMin)) /RAND_MAX + fMin ); }
//////////////////////////////////////////////////////////////////////////
