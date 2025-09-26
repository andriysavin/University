//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include "mesh3dobject.h"
#include "ParticleFountain.h"
//////////////////////////////////////////////////////////////////////////
class Teapot :
	public Mesh3DObject
{
public:
					 Teapot(LPDIRECT3DDEVICE9 const pDevice) ;
	virtual			~Teapot(void);

	void ChangeTemperature(int iDeltaTemperature)
	{ 
		m_iBaseFogQuantity += (int(m_iMaxFogQuantity / 2) * iDeltaTemperature) / 100;
		if (m_iBaseFogQuantity > m_iMaxFogQuantity) m_iBaseFogQuantity = m_iMaxFogQuantity;
		else if (m_iBaseFogQuantity < 0)  m_iBaseFogQuantity = 0;
		else m_iTemperature += iDeltaTemperature;
		m_pSteam->SetQuantity(m_iBaseFogQuantity);
	}

	int GetTemperature() const {return m_iTemperature; }

	virtual	void	Update() ;
	virtual	void	Render() ;
	ParticleFountain*  m_pSteam; 
	int					m_iMaxFogQuantity ;	
	int					m_iBaseFogQuantity;	
	int					m_iTemperature;
};
//////////////////////////////////////////////////////////////////////////
