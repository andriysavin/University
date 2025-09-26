///////////////////////////////////////////////////////////////////////////////////
//                                                                               //  
//                                                                               //
//           App.h: interface for the App class.                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef   __APP_H__
#define   __APP_H__
///////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
///////////////////////////////////////////////////////////////////////////////////
class App
{
public:
	App(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int  nCmdShow);
	int						Run();
	virtual BOOL			OnInitApp() = 0;
	
	virtual					~App();
	virtual void			OnIdle();
	void					EnableOnIdle(bool blEnable) { m_dwMsgTimeout = (blEnable ? 0 : INFINITE); }
protected:
	HINSTANCE				m_hInstance;
	HINSTANCE				m_hPrevInstance;
	LPSTR					m_lpCmdLine;
	int						m_nCmdShow;
	DWORD					m_dwMsgTimeout;
};
///////////////////////////////////////////////////////////////////////////////////
#endif // __APP_H__
///////////////////////////////////////////////////////////////////////////////////
