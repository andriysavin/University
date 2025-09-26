//////////////////////////////////////////////////////////////////////////
#include "ParticleSystem.h"
#include <fstream>
using namespace std;
//////////////////////////////////////////////////////////////////////////
ParticleSystem::ParticleSystem(DWORD dwQuantity, float fRandomQuantity, float fMaxParticleLifeTime, float fTimeToEmit) :
	m_dwMaxParticles((DWORD)(dwQuantity* (1 + fRandomQuantity)* (fMaxParticleLifeTime + 1.0f))),
	m_fTimeToEmit(fTimeToEmit),
	m_fStartTime(0),
	m_blStarted(false),
	m_dwQuantity(dwQuantity),
	m_fRandomQuantity(fRandomQuantity)
{
	m_parParticles = new Particle[m_dwMaxParticles];
	m_parPartInfos = new PARTICLEINFO[m_dwMaxParticles];
	srand(0);
}
//////////////////////////////////////////////////////////////////////////
ParticleSystem::ParticleSystem(string sParametrsFileName)
{
	char sTemp[256];
	DWORD dwQuantity			= 1;
	float fRandomQuantity		= 0.0f;
	float fMaxParticleLifeTime	= 1.0f;
	float fTimeToEmit			= 0;
	
	ifstream f(sParametrsFileName.c_str());

	if (f)
	{
		while (!f.eof())
		{
			f.getline(sTemp, 255);

			if (!strncmp(sTemp, "TimeToEmit", strlen("TimeToEmit")))
			{
				fTimeToEmit = (float)atof(strlen("TimeToEmit") + sTemp);
			}

			if (!strncmp(sTemp, "Quantity", strlen("Quantity")))
			{
				dwQuantity = atol(strlen("Quantity") + sTemp);
			}

			if (!strncmp(sTemp, "RandomQuantity", strlen("RandomQuantity")))
			{
				fRandomQuantity = (float)atof(strlen("RandomQuantity") + sTemp);
			}

			if (!strncmp(sTemp, "MaxParticleLifeTime", strlen("MaxParticleLifeTime")))
			{
				fMaxParticleLifeTime = (float)atof(strlen("MaxParticleLifeTime") + sTemp);
			}
		}

		f.close();
	}

	m_dwMaxParticles  = (DWORD)(dwQuantity*(1 + fRandomQuantity) * 
										(fMaxParticleLifeTime + 1.0f));
	m_fTimeToEmit     = fTimeToEmit;
	m_fStartTime      = 0;
	m_blStarted       = false;
	m_dwQuantity      = dwQuantity;
	m_fRandomQuantity = fRandomQuantity;

	m_parParticles = new Particle[m_dwMaxParticles];
	m_parPartInfos = new PARTICLEINFO[m_dwMaxParticles];
	srand(0);

}
//////////////////////////////////////////////////////////////////////////
ParticleSystem::~ParticleSystem(void)
{
	SAFE_DELETE_ARRAY(m_parParticles);
	SAFE_DELETE_ARRAY(m_parPartInfos);
}
//////////////////////////////////////////////////////////////////////////
void ParticleSystem::Start() 
{
	m_fStartTime = GetTime();
	m_blStarted  = true;
}
//////////////////////////////////////////////////////////////////////////
void  ParticleSystem::Render()
{
	DWORD dwNumAlive = 0;

	for (DWORD i = 0 ; i < m_dwMaxParticles ; i++)
	{	
		if(m_parParticles[i].Alive())
		{
				m_parPartInfos[dwNumAlive  ].vPos = m_parParticles[i].Position();
				m_parPartInfos[dwNumAlive++].clrDiffuse = m_parParticles[i].ColorDiffuse() ;
		}
	}

	if(dwNumAlive) 
		OnRender(m_parPartInfos, dwNumAlive);
}
//////////////////////////////////////////////////////////////////////////
void  ParticleSystem::Update()
{
	static PARTICLECREATEPARAM CreateParam;
	static DWORD dwStartIndex;
	static float fOldTime = GetTime();
		   float fNewTime = GetTime();

	DWORD   dwNumToEmit  = (DWORD) (m_dwQuantity * (fNewTime - fOldTime));
	if(dwNumToEmit) fOldTime = fNewTime ;
	

	if(m_blStarted)
	{
		for (DWORD i = 0, dwEmitted = 0 ; i < m_dwMaxParticles && dwEmitted < dwNumToEmit  ; i++)
			if(!m_parParticles[i].Alive())
			{
				OnEmitParticle(&CreateParam);
				m_parParticles[i].Create(&CreateParam);
				dwEmitted++;
			}
	}

	for (DWORD i = 0 ; i < m_dwMaxParticles ; i++)
		m_parParticles[i].Move();

	if (m_blStarted && m_fTimeToEmit)
		if ((GetTime() - m_fStartTime) > m_fTimeToEmit)
			m_blStarted = false;

}
//////////////////////////////////////////////////////////////////////////
