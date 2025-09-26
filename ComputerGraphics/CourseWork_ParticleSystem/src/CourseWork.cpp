// CourseWork.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "CourseWork.h"
#include "App.h"
#include "CourseWorkApp.h"

#include <stdexcept>
#include <windows.h>

using namespace std;

int APIENTRY WinMain(
	_In_		HINSTANCE hInstance,
	_In_opt_	HINSTANCE hPrevInstance,
	_In_		LPSTR	  lpCmdLine,
	_In_		int       nCmdShow)
{
	CourseWorkApp app(
		hInstance,
		hPrevInstance,
		lpCmdLine,
		nCmdShow);

	try
	{
		app.Run();
	}
	catch (exception& e)
	{
		MessageBox(
			NULL,
			(string("Exception type:") + e.what()).c_str(),
			"Error:Exception",
			MB_ICONERROR | MB_OK | MB_SETFOREGROUND);
	}
	catch (...)
	{
		MessageBox(
			GetDesktopWindow(),
			"Unknown exception!",
			"Error:Exception",
			MB_ICONERROR | MB_OK | MB_SETFOREGROUND);
	}

	return 0;
}
