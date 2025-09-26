//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include <D3DX9.h>
#include <windows.h>
#include <mmsystem.h>

//////////////////////////////////////////////////////////////////////////
typedef struct tagPARTICLECREATEPARAM 
{
	D3DXVECTOR3 vPos0;
	D3DXVECTOR3 vVel0;
	D3DXVECTOR3 vAccel0;
	D3DXCOLOR	clrDiffuse0;
	float		fLifeTime;
	float		fFade;

} PARTICLECREATEPARAM,*LPPARTICLECREATEPARAM;
	
class Particle
{
public:

					Particle(void);
	virtual		   ~Particle(void);

	void			Create(D3DXVECTOR3 vPos0,D3DXVECTOR3 vVel0,D3DXVECTOR3  vAccel0,
						D3DXCOLOR clrDiffuse0,float fLifeTime, FLOAT  fFade);
	void			Create(LPPARTICLECREATEPARAM lpCreateParam);

	D3DXVECTOR3 	Position()		{return m_vPos;}
	D3DXVECTOR3&  	Velocity()		{return m_vVel;}
	D3DXVECTOR3&	Acceleration()	{return m_vAccel;}
	D3DXCOLOR		ColorDiffuse()	{return m_clrDiffuse;}
	bool			Alive()	        {return m_blAlive;}
	
	void			Move();
	void            Reset();
private:
	D3DXVECTOR3		m_vPos;        // Current position
    D3DXVECTOR3		m_vVel;        // Current velocity
	D3DXVECTOR3		m_vAccel;      // Current acceleration
	D3DXCOLOR		m_clrDiffuse;  // Current (faded) diffuse color

	float			m_fLifeTime; 	// Time of life			

    D3DXVECTOR3		m_vPos0;       // Initial position
    D3DXVECTOR3		m_vVel0;       // Initial velocity
	D3DXVECTOR3		m_vAccel0;     // Initial acceleration
    float			m_fTime0;     // Time of creation
    D3DXCOLOR		m_clrDiffuse0; // Initial diffuse color
	     
    FLOAT			m_fFade;       // Fade progression
	bool			m_blAlive;     // Is alive ?

	bool			m_blNeedFade ;

};
//////////////////////////////////////////////////////////////////////////
