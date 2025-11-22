//////////////////////////////////////////////////////////////////////////
#include <windows.h>

#include <mmsystem.h>
#include "3DEngine.h"
#include "matrix4.h"

#define PI 3.14159265358979323846  

C3DEngine Engine;

struct CAMERA
{
	float fAngX;	
	float fAngY;
	float fAngZ;

	VECTOR4 vPos;
} g_Camera;
//////////////////////////////////////////////////////////////////////////
void OnPaint(HWND hwnd);
void Redraw(HWND hwnd);
//////////////////////////////////////////////////////////////////////////
C3DEngine::VERTEX g_arGlassVertices[] = 
{
	VECTOR4(-50/1.5f, -60,   	  0), VECTOR4(-50, 60,   0),
	VECTOR4(-35/1.5f, -60, -35/1.5f), VECTOR4(-35, 60, -35),
	VECTOR4(	   0, -60, -50/1.5f), VECTOR4(  0, 60, -50),
	VECTOR4( 35/1.5f, -60, -35/1.5f), VECTOR4( 35, 60, -35),

	VECTOR4( 50/1.5f, -60,		  0), VECTOR4(50, 60,   0),

	VECTOR4( 35/1.5f, -60,  35/1.5f), VECTOR4( 35, 60,  35),
	VECTOR4(	   0, -60,  50/1.5f), VECTOR4( 0,  60,  50),
	VECTOR4(-35/1.5f, -60,  35/1.5f), VECTOR4(-35, 60,  35),
	VECTOR4(-50/1.5f, -60,	  	  0), VECTOR4(-50, 60,   0),

	VECTOR4(	   0, -60, 		  0),
	VECTOR4(-50/1.5f, -60,   	  0), VECTOR4(-35/1.5f,-60,-35/1.5f),
	VECTOR4(	   0, -60, -50/1.5f), VECTOR4( 35/1.5f,-60,-35/1.5f),
	VECTOR4( 50/1.5f, -60, 	      0), VECTOR4( 35/1.5f,-60, 35/1.5f),
	VECTOR4(	   0, -60,  50/1.5f), VECTOR4(-35/1.5f,-60, 35/1.5f),
	VECTOR4(-50/1.5f, -60, 	      0)
};

C3DEngine::VERTEX g_arCoordSysVertices[] =
{
	VECTOR4(-500,  0,0), VECTOR4(500, 0, 0),
	VECTOR4( 450, 10,0), VECTOR4(500, 0, 0),
	VECTOR4( 450,-10,0), VECTOR4(500, 0, 0),
						 
	VECTOR4(0,-500,  0), VECTOR4(0, 500, 0),
	VECTOR4(0, 450, 10), VECTOR4(0, 500, 0),
	VECTOR4(0, 450,-10), VECTOR4(0, 500, 0),
						 
	VECTOR4(  0,0,-500), VECTOR4(0, 0, 500),
	VECTOR4( 10,0, 450), VECTOR4(0, 0, 500),
	VECTOR4(-10,0, 450), VECTOR4(0, 0, 500),
};
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	MATRIX4 CamRotX, CamRotY;
	VECTOR4 CamDir(0, 0, 25);

	switch (uMsg)
	{
	case WM_TIMER:
		Redraw(hwnd);
		break;
	case WM_KEYDOWN:
		CamRotX.RotationX(g_Camera.fAngX);
		CamRotY.RotationY(g_Camera.fAngY);
		CamDir *= (CamRotX * CamRotY);

		switch (wParam)
		{
		case VK_UP:
			g_Camera.vPos += CamDir;
			break;
		case VK_DOWN:
			g_Camera.vPos -= CamDir;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		OnPaint(hwnd);
		return 0L;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR d3, int nCmdShow)
{
	MSG msg;
	HWND hwnd;
	WNDCLASS wndClass;

	memset(&wndClass, 0, sizeof(wndClass));
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszClassName = "Lab1";

	if (!RegisterClass(&wndClass)) return FALSE;

	hwnd = CreateWindow("Lab1", "Lab 1 - Matrix Transforms",
		WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	g_Camera.fAngX = g_Camera.fAngY = g_Camera.fAngZ = 0;
	g_Camera.vPos = VECTOR4(0, 0, 0);

	UINT_PTR idp = 0;
	SetTimer(hwnd, idp, 10, NULL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
//////////////////////////////////////////////////////////////////////////
void OnDraw(HDC hdc, HWND hwnd)
{
	RECT ClntRect;
	DWORD dwPos;
	POINTS points;
	POINT point;

	GetClientRect(hwnd, &ClntRect);

	dwPos = GetMessagePos();
	points = MAKEPOINTS(dwPos);
	point.x = points.x;
	point.y = points.y;
	ScreenToClient(hwnd, &point);

	point.x -= ClntRect.right / 2;
	point.y -= ClntRect.bottom / 2;

	g_Camera.fAngX = point.y / 200.0f;
	g_Camera.fAngY = point.x / 200.0f;

	GetClientRect(hwnd, &ClntRect);
	HPEN hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 255));
	SelectObject(hdc, hPen);
	FillRect(hdc, &ClntRect, (HBRUSH)(COLOR_WINDOW + 1));

	MATRIX4 mRotX, mRotY, mRotZ, mTransZ, mScale;
	MATRIX4 mWorld, mView, mProj;

	// calculating the world matrix
	mTransZ.Translation(0, 0, 0);
	mRotX.RotationX(timeGetTime() / 1000.0f);
	mRotY.RotationY(timeGetTime() / 1000.0f);
	mRotZ.RotationZ(timeGetTime() / 1000.0f);
	mScale.Scale(3, 3, 3);

	mWorld = ((mScale * (mRotX * mRotZ * mRotY)) * mTransZ);

	// calculating the view matrix

	mView.Identity();

	MATRIX4 CamTrans, CamRotX, CamRotY;
	
	CamTrans.Translation(-g_Camera.vPos.x, -g_Camera.vPos.y, -g_Camera.vPos.z);
	CamRotX.RotationX(-g_Camera.fAngX); 
	CamRotY.RotationY(-g_Camera.fAngY);

	mView = CamTrans * CamRotX * CamRotY;

	// calculating the projection matrix
	mProj.Projection(PI / 4, (float)ClntRect.right, (float)ClntRect.bottom);

	Engine.SetDC(hdc);

	// draw the outside of the glass and the bottom
	Engine.SetTransform(C3DEngine::TT_WORLD, &mWorld);
	Engine.SetTransform(C3DEngine::TT_VIEW, &mView);
	Engine.SetTransform(C3DEngine::TT_PROJECTION, &mProj);

	Engine.SetVertexBuffer(g_arGlassVertices);

	Engine.DrawPrimitive(C3DEngine::PT_TRIANGLESTRIP, 0, 16);
	Engine.DrawPrimitive(C3DEngine::PT_TRIANGLEFAN, 18, 8);

	// draw the inside of the glass
	mScale.Scale(2.8f, 3.0f, 2.8f);
	mWorld = ((mScale * (mRotX * mRotZ * mRotY)) * mTransZ);

	Engine.SetTransform(C3DEngine::TT_WORLD, &mWorld);
	Engine.DrawPrimitive(C3DEngine::PT_TRIANGLESTRIP, 0, 16);

	DeleteObject(hPen);

	// draw coordinate axes
	hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	SelectObject(hdc, hPen);

	Engine.SetVertexBuffer(g_arCoordSysVertices);
	mWorld.Identity();
	Engine.SetTransform(C3DEngine::TT_WORLD, &mWorld);
	Engine.DrawPrimitive(C3DEngine::PT_LINELIST, 0, 9);

	DeleteObject(hPen);
}
//////////////////////////////////////////////////////////////////////////
void Draw(HDC hdc, HWND hwnd)
{
	RECT	ClntRect;
	GetClientRect(hwnd, &ClntRect);

	HDC		hdcCompatible = CreateCompatibleDC(hdc);
	HBITMAP hCompBmp = CreateCompatibleBitmap(hdc, ClntRect.right, ClntRect.bottom);
	HGDIOBJ hOldObj = SelectObject(hdcCompatible, hCompBmp);

	OnDraw(hdcCompatible, hwnd);
	BitBlt(hdc, 0, 0, ClntRect.right, ClntRect.bottom, hdcCompatible, 0, 0, SRCCOPY);

	SelectObject(hdcCompatible, hOldObj);
	DeleteObject(hCompBmp);
	DeleteDC(hdcCompatible);
}
//////////////////////////////////////////////////////////////////////////
void Redraw(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	Draw(hdc, hwnd);
	ReleaseDC(hwnd, hdc);
}
//////////////////////////////////////////////////////////////////////////
void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hwnd, &ps);
	Draw(hdc, hwnd);
	EndPaint(hwnd, &ps);
}
