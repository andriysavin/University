///////////////////////////////////////////////////////////////////////////////////
//                                                                               //  
//                                                                               //
//           Wnd.h: interface for the Wnd class.                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef   __WND_H__
#define   __WND_H__
///////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>
#include <map>
#include <string>
#include "property.h"
///////////////////////////////////////////////////////////////////////////////////
#define BEG_MSG_MAP(theClass) \
private: \
	typedef LRESULT (theClass::*PMESSAGEHANDLER)(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled); \
	std::map<UINT,PMESSAGEHANDLER> m_MsgMap; \
	LRESULT	MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled) \
	{ \
		LRESULT Res = S_OK; \
		std::map<UINT,PMESSAGEHANDLER>::iterator MsgIter = m_MsgMap.find(uMsg); \
		if(MsgIter != m_MsgMap.end()) \
		{ \
			PMESSAGEHANDLER mh = MsgIter->second;\
			Res = (this->*mh)(uMsg,wParam,lParam,blHandled ); \
		} \
		else\
		{\
			blHandled = false;\
		}\
		return Res; \
	}\
	virtual void InitMsgMap(void) \
	{ 

#define MSG_HANDLER(msg, func)\
	m_MsgMap[msg] = (PMESSAGEHANDLER)func;

#define END_MSG_MAP()\
	}


///////////////////////////////////////////////////////////////////////////////////
typedef struct tagWNDCREATESTRUCT
{
  DWORD     dwExStyle;    // extended window style
  LPCTSTR   lpClassName;  // registered class name
  LPCTSTR   lpWindowName; // window name
  DWORD     dwStyle;      // window style
  int       x;            // horizontal position of window
  int       y;            // vertical position of window
  int       nWidth;       // window width
  int       nHeight;      // window height
  HWND      hWndParent;   // handle to parent or owner window
  HMENU     hMenu;        // menu handle or child identifier
  HINSTANCE hInstance;    // handle to application instance
  LPVOID    lpParam ;     // window-creation data
} WNDCREATESTRUCT,*PWNDCREATESTRUCT;
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
class Wnd
{
protected:
	virtual void               PreCreateWindow(WNDCREATESTRUCT*  pCreateStruct,WNDCLASSEX*  pWndClassEx);
	virtual void               OnCreated();
public:
	
	BOOL                       Create(HINSTANCE hInstance,Wnd* pParentWnd = NULL);
	void                       UpdateWindow( );
	BOOL                       ShowWindow( int nCmdShow );
	bool					   DestroyWindow();
	HDC						   GetDC();

                               operator HWND() const{return m_hWnd;};

							   Wnd();
	virtual                   ~Wnd();
public:
	property<Wnd,int>		   XPos;			
	property<Wnd,int>		   YPos;
	property<Wnd,int>		   Width ;			
	property<Wnd,int>		   Height;

private:
	int						   GetXPos(const int& Val);
	void					   SetXPos(int& ValToSet,int Val);

	int						   GetYPos(const int& Val);
	void					   SetYPos(int& ValToSet,int Val);

	int						   GetWidth(const int& Val);
	void					   SetWidth(int& ValToSet,int Val);

	int						   GetHeight(const int& Val);
	void					   SetHeight(int& ValToSet,int Val);

protected:

	virtual void			   InitMsgMap(void) = 0;
	virtual LRESULT			   MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled) {return 0;};

private:
	static  LRESULT CALLBACK   WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam  );
	

	HWND						m_hWnd = NULL;
	std::string					m_sClssName;
	HINSTANCE					m_hInstance = NULL;
	static std::map<HWND,Wnd*>	m_ThisMap;		
	bool						m_blCreated;
};

///////////////////////////////////////////////////////////////////////////////////

#endif // __WND_H__

///////////////////////////////////////////////////////////////////////////////////
