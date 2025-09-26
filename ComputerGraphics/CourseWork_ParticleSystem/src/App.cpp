////////////////////////////////////////////////////////////////////////
//                                                                    //  
//                                                                    //
//  App.cpp: interface for the Wnd class.                           //
//                                                                    //
//                                                                    //
////////////////////////////////////////////////////////////////////////
#include "App.h"
////////////////////////////////////////////////////////////////////////
App::App(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int  nCmdShow)
{
	m_hInstance = hInstance;
	m_hPrevInstance = hPrevInstance;
	m_lpCmdLine = lpCmdLine;
	m_nCmdShow = nCmdShow;
	m_dwMsgTimeout = INFINITE;
}
////////////////////////////////////////////////////////////////////////
App::~App()
{
}
////////////////////////////////////////////////////////////////////////
int App::Run()
{
	OnInitApp();

	while (TRUE)
	{
		DWORD result;
		MSG msg;

		// Read all of the messages in this next loop, 
		// removing each message as we read it.
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// If it's a quit message, we're out of here.
			if (msg.message == WM_QUIT)
				return 1;
			// Otherwise, dispatch the message.
			DispatchMessage(&msg);
		}

		// Wait for any message sent or posted to this queue 
		// or for one of the passed handles be set to signaled.
		result = MsgWaitForMultipleObjects(0, NULL,
			FALSE, m_dwMsgTimeout, QS_ALLINPUT);

		// The result tells us the type of event we have.
		if (result == (WAIT_OBJECT_0 + 0))
		{
			// New messages have arrived. 
			// Continue to the top of the always while loop to 
			// dispatch them and resume waiting.
			continue;
		}
		else
		{
			OnIdle();
		}
	}
}
////////////////////////////////////////////////////////////////////////
void App::OnIdle()
{
}
////////////////////////////////////////////////////////////////////////
