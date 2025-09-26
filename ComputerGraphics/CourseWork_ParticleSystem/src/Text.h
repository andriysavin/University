//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include "3dobject.h"
#include "W32Exception.h"
#include "utils.h"
#include <d3dx9.h>
//////////////////////////////////////////////////////////////////////////
class Text :
	public C3DObject
{
public:

	Text(LPDIRECT3DDEVICE9 const pDevice) : C3DObject(pDevice)
	{
		m_pFont = NULL;
		m_Format = DT_LEFT;
		m_Color = 0xffffffff;
		m_sText = "";

		SetRect(&m_Rect, 0, 0, 100, 100);
	}

	HRESULT Create(CONST D3DXFONT_DESCA* pLogFont)
	{
		return D3DXCreateFontIndirect(m_pDevice, pLogFont, &m_pFont);
	}

	virtual void Render()
	{
		m_pFont->DrawText(NULL, m_sText.c_str(), m_sText.length(), &m_Rect, m_Format, m_Color);
	}

	virtual void Update()
	{

	}

	void ChangeText(const std::string& sText, LPRECT pRect, DWORD Format = DT_LEFT,
		D3DCOLOR Color = 0xffffffff)
	{
		m_sText = sText;
		m_Format = Format;
		m_Color = Color;
		m_Rect = *pRect;
	}


	virtual ~Text(void)
	{
		SAFE_RELEASE(m_pFont);
	}

private:

	LPD3DXFONT		m_pFont;
	std::string		m_sText;
	RECT			m_Rect;
	DWORD			m_Format;
	D3DCOLOR		m_Color;
};
//////////////////////////////////////////////////////////////////////////
