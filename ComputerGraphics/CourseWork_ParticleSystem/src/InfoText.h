//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include "text.h"
#include "Teapot.h"
//////////////////////////////////////////////////////////////////////////
class InfoText :
	public Text
{
public:

	InfoText(LPDIRECT3DDEVICE9 const pDevice, const Teapot* pTeapot) : Text(pDevice),
		m_pTeapot(pTeapot)
	{
		D3DXFONT_DESCA fontDesc;

		memset(&fontDesc, 0, sizeof(D3DXFONT_DESCA));
		fontDesc.Height = 25;                      
		fontDesc.Weight = FW_NORMAL;
		fontDesc.CharSet = DEFAULT_CHARSET;
		strcpy_s(fontDesc.FaceName, std::size(fontDesc.FaceName), "Arial");

		Create(&fontDesc);
	}

	virtual void Update()
	{

		char s[1024];
		sprintf_s(s, std::size(s), "Controls:\n"
			"   camera movement - Up, Down, Left, Right\n"
			"   camera rotation - mouse with left mouse button pressed\n"
			"   changing the temperature of the kettle  - Home,End.\n"
		    "Current temperature %d",m_pTeapot->GetTemperature());
		
		
		RECT Rect = {0,0,600,600};
		ChangeText(s, &Rect,DT_LEFT,0xFF0FFF0F);
	}
	
	virtual ~InfoText(void)
	{
	}
private:
	const Teapot* const	m_pTeapot;
};
//////////////////////////////////////////////////////////////////////////
