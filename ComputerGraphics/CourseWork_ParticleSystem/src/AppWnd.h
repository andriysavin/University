//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include "Utils.h"
#include "Wnd.h"
#include "CourseWorkApp.h"
#include "W32Exception.h"
#include "resource.h"
#include "3DObject.h"

#include "Chair.h"
#include "Table.h"
#include "Room.h"
#include "Teapot.h"
#include "Camera.h"
#include "InfoText.h"

#include <vector> 
#include <d3d9.h> 
#include <d3dx9.h> 
#pragma warning( disable : 4800 )
#pragma warning( disable : 4305 )
//////////////////////////////////////////////////////////////////////////
class CourseWorkApp;
class AppWnd : public Wnd
{
public:
					AppWnd(CourseWorkApp& pOwningApp);
	virtual			~AppWnd(void);

	void			Update();
	void			Render();
protected:
	virtual void	PreCreateWindow(WNDCREATESTRUCT*  pCreateStruct,WNDCLASSEX*  pWndClassEx);
	
	virtual void	OnCreated	 ();
	LRESULT			OnDestroy	 (UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled);
	LRESULT			OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled);
	LRESULT			OnLButtonUp  (UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled);
	LRESULT			OnPaint		 (UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled);
	LRESULT			OnActivate   (UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled);
	LRESULT			OnKeyDown	 (UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled);
	
	void			InitD3D();
	void			ReleaseD3D();
	void			Cleanup();
	void			UpdateCamera();
	
	BEG_MSG_MAP(AppWnd)
		MSG_HANDLER(WM_PAINT	  , &AppWnd::OnPaint		)
		MSG_HANDLER(WM_ACTIVATE   , &AppWnd::OnActivate		)
		MSG_HANDLER(WM_DESTROY	  , &AppWnd::OnDestroy		)
		MSG_HANDLER(WM_LBUTTONDOWN, &AppWnd::OnLButtonDown	)
		MSG_HANDLER(WM_LBUTTONUP  , &AppWnd::OnLButtonUp	)
		MSG_HANDLER(WM_KEYDOWN	  , &AppWnd::OnKeyDown		)
	END_MSG_MAP()

private:
	CourseWorkApp&			m_pOwningApp;
	LPDIRECT3D9             m_pD3D		; 
	LPDIRECT3DDEVICE9       m_pd3dDevice;
	HRESULT					m_hr;
	Camera*					m_pCamera;
	bool					m_blLButtDown;

	POINT					m_ptNewCursorPos;
	POINT					m_ptOldCursorPos;
	BYTE					m_arKeyboardState[256];
	bool					KeyPressed(DWORD dwVKeyCode)
							{ return (m_arKeyboardState[dwVKeyCode] & 0x80); }

	std::vector<C3DObject*>	m_ar3DObjects;
	InfoText*				m_pHelpText;
};
//////////////////////////////////////////////////////////////////////////
