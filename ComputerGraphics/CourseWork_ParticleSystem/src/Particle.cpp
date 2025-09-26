//////////////////////////////////////////////////////////////////////////
#include "Particle.h"
//////////////////////////////////////////////////////////////////////////
Particle::Particle():
	m_vPos0(0,0,0),
	m_vPos (0,0,0),
	m_vVel0(0,0,0),
	m_vVel (0,0,0),
	m_vAccel0(0,0,0),
	m_vAccel (0,0,0),
	m_clrDiffuse0(0xffffffff),
	m_clrDiffuse (0xffffffff),
	m_fLifeTime(0.0),
	m_fTime0(0.0f),
	m_fFade(0.0f),
	m_blAlive(false)
{
}
//////////////////////////////////////////////////////////////////////////
Particle::~Particle(void)
{
}
//////////////////////////////////////////////////////////////////////////
void Particle::Create(D3DXVECTOR3 vPos0,D3DXVECTOR3 vVel0,D3DXVECTOR3  vAccel0,
						D3DXCOLOR clrDiffuse0,float fLifeTime, FLOAT  fFade)
{
	m_vPos0		  = vPos0;
	m_vPos		  = vPos0;
	m_vVel0		  = vVel0;
	m_vVel		  = vVel0;
	m_vAccel0	  = vAccel0;
	m_vAccel	  = vAccel0;
	m_clrDiffuse0 = clrDiffuse0;
	m_clrDiffuse  = clrDiffuse0;
	m_fLifeTime	  = fLifeTime;
	m_fTime0	  = timeGetTime() /1000.0f;
	m_fFade		  = fFade;
	m_blAlive	  = true;
	m_blNeedFade  = true;
}
//////////////////////////////////////////////////////////////////////////
void Particle::Create(LPPARTICLECREATEPARAM lpCreateParam)
{
	Create(	lpCreateParam->vPos0,
			lpCreateParam->vVel0,
			lpCreateParam->vAccel0,
			lpCreateParam->clrDiffuse0,
			lpCreateParam->fLifeTime,
			lpCreateParam->fFade);

}
//////////////////////////////////////////////////////////////////////////
void Particle::Reset()
{
	m_vPos		  = m_vPos0;
	m_vVel		  = m_vVel0;
	m_vAccel	  = m_vAccel0;
	m_clrDiffuse  = m_clrDiffuse0;
	m_fTime0	  = timeGetTime()/1000.0;
	m_blAlive	  = true;
	m_blNeedFade  = true;
}
//////////////////////////////////////////////////////////////////////////
void Particle::Move()
{
	if (!m_blAlive) return;

	float fPassedTime = (timeGetTime() / 1000.0 - m_fTime0);

	if (fPassedTime > m_fLifeTime) { m_blAlive = false; return; }

	// Calculating current position
	m_vPos = m_vPos0 + m_vVel0 * fPassedTime +
		(m_vAccel * fPassedTime * fPassedTime) / 2.0f;

	if (m_vVel0.y < 0) { m_blAlive = false; return; }

	// Calculating current velocity
	m_vVel = m_vVel0 + m_vAccel * fPassedTime;

	// Calculating current diffuse color
	if (m_blNeedFade)
	{
		if (fPassedTime > m_fFade)
		{
			m_blNeedFade = false;
			m_clrDiffuse *= 0;
		}
		else
		{
			m_clrDiffuse = m_clrDiffuse0 * (1.0f - fPassedTime / m_fFade);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
