////////////////////////////////////////////////////////////////////////
//                                                                    //  
//                                                                    //
//  Wnd.cpp: interface for the Wnd class.                           //
//                                                                    //
//                                                                    //
////////////////////////////////////////////////////////////////////////
#include "Wnd.h"

std::map<HWND,Wnd*>	  Wnd::m_ThisMap;		
////////////////////////////////////////////////////////////////////////
Wnd::Wnd()
{
	m_blCreated = false;

	XPos.Init(&Wnd::GetXPos, &Wnd::SetXPos,this);
	YPos.Init(&Wnd::GetYPos, &Wnd::SetYPos,this);
	Width .Init(&Wnd::GetWidth , &Wnd::SetWidth,this);
	Height.Init(&Wnd::GetHeight, &Wnd::SetHeight,this);
}
////////////////////////////////////////////////////////////////////////
BOOL Wnd::Create(HINSTANCE hInstance, Wnd *pParentWnd)
{
	if(m_blCreated) return false;

    WNDCREATESTRUCT  CreateStruct;
	WNDCLASSEX       WndClassEx; 
    m_hInstance = hInstance;
	
	WndClassEx.cbClsExtra         =   0;
    WndClassEx.cbSize             =   sizeof(WNDCLASSEX);
	WndClassEx.cbWndExtra         =   0;     
	WndClassEx.hbrBackground      =   (HBRUSH)COLOR_BACKGROUND;
	WndClassEx.hCursor            =   LoadCursor(NULL,IDC_ARROW);
	WndClassEx.hIcon              =   NULL;
	WndClassEx.hIconSm            =   NULL;  
	WndClassEx.hInstance          =   hInstance;
	WndClassEx.lpfnWndProc        =   (WNDPROC)WindowProc;
	WndClassEx.lpszClassName      =   "ClassName";
	WndClassEx.style              =   CS_VREDRAW |CS_HREDRAW;
    WndClassEx.lpszMenuName       =   NULL;
	
    CreateStruct.dwExStyle        =   WS_EX_OVERLAPPEDWINDOW;
    CreateStruct.dwStyle          =   WS_OVERLAPPEDWINDOW;
	CreateStruct.hInstance        =   WndClassEx.hInstance;
	CreateStruct.hMenu            =   NULL;
	CreateStruct.hWndParent       =   (pParentWnd)? pParentWnd->m_hWnd : NULL;
	CreateStruct.lpClassName      =   WndClassEx.lpszClassName;
	CreateStruct.lpParam          =   this;
	CreateStruct.lpWindowName     =   "Application";
	CreateStruct.nHeight          =   CW_USEDEFAULT;
	CreateStruct.nWidth           =   CW_USEDEFAULT;
	CreateStruct.x                =   CW_USEDEFAULT;
	CreateStruct.y                =   CW_USEDEFAULT;
	
	PreCreateWindow(&CreateStruct,&WndClassEx);

	Wnd* pWnd = this;
	pWnd->InitMsgMap();

    RegisterClassEx( &WndClassEx );
	
	m_hWnd = CreateWindowEx( CreateStruct.dwExStyle, CreateStruct.lpClassName,CreateStruct.lpWindowName,CreateStruct.dwStyle, 
		CreateStruct.x ,CreateStruct.y ,CreateStruct.nWidth, CreateStruct.nHeight, 
   	    CreateStruct.hWndParent  , CreateStruct.hMenu , CreateStruct.hInstance, CreateStruct.lpParam );    
	
	if (!m_hWnd) return FALSE;

	// adding pointer to this object to "this map"
	m_ThisMap[m_hWnd] = this;

	::SetCursor(WndClassEx.hCursor);
	
	m_sClssName = CreateStruct.lpClassName;
    m_hInstance = CreateStruct.hInstance;

	m_blCreated = true;

	OnCreated();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
Wnd::~Wnd()
{
	DestroyWindow();
}
////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK Wnd::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam  )
{
	std::map<HWND, Wnd*>::iterator Iter = m_ThisMap.find(hwnd);

	if (Iter == m_ThisMap.end()) return DefWindowProc(hwnd, uMsg, wParam, lParam);

	Wnd* pThis = Iter->second;

	LRESULT Res;
	bool    blHandled = true;

	if (uMsg == WM_DESTROY)
	{
		if (!pThis->m_blCreated) return Res = S_FALSE;
		pThis->m_blCreated = false;

		UnregisterClass(pThis->m_sClssName.c_str(), pThis->m_hInstance);

		// removing pointer to this object from "this map"
		m_ThisMap.erase(pThis->m_hWnd);
	};


	Res = pThis->MsgProc(uMsg, wParam, lParam, blHandled);
	if (blHandled) return Res;
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////
void Wnd::PreCreateWindow(WNDCREATESTRUCT*  pCreateStruct,WNDCLASSEX*  pWndClassEx)
{
}
////////////////////////////////////////////////////////////////////////
void Wnd::OnCreated()
{
}
////////////////////////////////////////////////////////////////////////
BOOL Wnd::ShowWindow(int nCmdShow)
{
	return ::ShowWindow(m_hWnd, nCmdShow);
}
////////////////////////////////////////////////////////////////////////
void Wnd::UpdateWindow()
{
	::UpdateWindow(m_hWnd);
}
////////////////////////////////////////////////////////////////////////
bool Wnd::DestroyWindow()
{
	return ::DestroyWindow(m_hWnd) != 0;
}
//////////////////////////////////////////////////////////////////////////
HDC Wnd::GetDC()
{
	return ::GetDC(m_hWnd);
}
//////////////////////////////////////////////////////////////////////////
int Wnd::GetXPos(const int& Val)
{
	return 0;
}
//////////////////////////////////////////////////////////////////////////
void Wnd::SetXPos(int& ValToSet,int Val)
{
	RECT Rect ;
	::GetWindowRect(m_hWnd,&Rect);

	Rect.right  -= Rect.left ;
	Rect.bottom -= Rect.top  ;

	Rect.left = Val;
	::MoveWindow(m_hWnd,Rect.left,Rect.top,Rect.right,Rect.bottom,TRUE);
}
//////////////////////////////////////////////////////////////////////////
int  Wnd::GetYPos(const int& Val)
{
	return 0;
}
//////////////////////////////////////////////////////////////////////////
void Wnd::SetYPos(int& ValToSet,int Val)
{
	RECT Rect ;
	::GetWindowRect(m_hWnd,&Rect);

	Rect.right  -= Rect.left ;
	Rect.bottom -= Rect.top  ;

	Rect.top = Val;

	::MoveWindow(m_hWnd,Rect.left,Rect.top,Rect.right,Rect.bottom,TRUE);
}
//////////////////////////////////////////////////////////////////////////
int	Wnd::GetWidth(const int& Val)
{
	RECT Rect ;
	::GetWindowRect(m_hWnd,&Rect);
	return 	Rect.right  -= Rect.left ;
}
//////////////////////////////////////////////////////////////////////////
void Wnd::SetWidth(int& ValToSet,int Val)
{
	RECT Rect ;
	::GetWindowRect(m_hWnd,&Rect);

	Rect.right   = Val;
	Rect.bottom -= Rect.top  ;

	::MoveWindow(m_hWnd,Rect.left,Rect.top,Rect.right,Rect.bottom,TRUE);
}
//////////////////////////////////////////////////////////////////////////
int	Wnd::GetHeight(const int& Val)
{
	RECT Rect ;
	::GetWindowRect(m_hWnd,&Rect);
	return Rect.bottom -= Rect.top  ;
}
//////////////////////////////////////////////////////////////////////////
void Wnd::SetHeight(int& ValToSet,int Val)
{
	RECT Rect ;
	::GetWindowRect(m_hWnd,&Rect);

	Rect.right  -= Rect.left ;
	Rect.bottom  = Val ;

	::MoveWindow(m_hWnd,Rect.left,Rect.top,Rect.right,Rect.bottom,TRUE);
}
//////////////////////////////////////////////////////////////////////////
