//////////////////////////////////////////////////////////////////////////
#include "AppWnd.h"
//////////////////////////////////////////////////////////////////////////
AppWnd::AppWnd(CourseWorkApp& pOwningApp)
	: m_pOwningApp(pOwningApp)
{
	m_pD3D				= NULL; 
	m_pd3dDevice		= NULL; 
	m_blLButtDown		= false;
	GetCursorPos(&m_ptNewCursorPos);
	GetCursorPos(&m_ptOldCursorPos);
}
//////////////////////////////////////////////////////////////////////////
AppWnd::~AppWnd(void)
{
	Cleanup();
}
//////////////////////////////////////////////////////////////////////////
void AppWnd::PreCreateWindow(WNDCREATESTRUCT*  pCreateStruct,WNDCLASSEX*  pWndClassEx)
{
	pWndClassEx->style			= CS_CLASSDC;
	pWndClassEx->hIcon			= LoadIcon(pWndClassEx->hInstance,MAKEINTRESOURCE (IDI_BIG_ICON));
	pWndClassEx->hIconSm		= LoadIcon(pWndClassEx->hInstance,MAKEINTRESOURCE (IDI_SMALL_ICON));
	

	pCreateStruct->lpWindowName = "Coursework - Particle System (F1 - help)";
	pCreateStruct->dwExStyle	= 0;
	pCreateStruct->dwStyle		= WS_OVERLAPPEDWINDOW;
	pCreateStruct->x			= 100;
	pCreateStruct->y			= 100;
	pCreateStruct->nWidth		= 600;
	pCreateStruct->nHeight		= 600;
}
//////////////////////////////////////////////////////////////////////////
void AppWnd::OnCreated()
{
	InitD3D();

	m_ar3DObjects.push_back(new Chair(m_pd3dDevice));
	m_ar3DObjects.push_back(new Table(m_pd3dDevice));
	m_ar3DObjects.push_back(new Room(m_pd3dDevice));
	m_ar3DObjects.push_back(new Teapot(m_pd3dDevice));

	m_pHelpText = new InfoText(m_pd3dDevice, (Teapot*)m_ar3DObjects[3]);
	m_pHelpText->SetIsVisible(false);
	m_ar3DObjects.push_back(m_pHelpText);
	
	m_pCamera = new Camera(m_pd3dDevice);

	// Set good initial view.
	m_pCamera->SetPos(D3DXVECTOR3(42.5721245, 53.9288635, 28.4104633));
	m_pCamera->SetRot(D3DXVECTOR3(0.0466665477, -0.760000110, 0));
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppWnd::InitD3D()
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!m_pD3D) throw(exception("Error Direct3DCreate9"));

	D3DDISPLAYMODE d3ddm;

	m_hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	TestResult(m_hr, "Error GetAdapterDisplayMode\n");

	
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	
	/*
	d3dpp.BackBufferHeight		 = d3ddm.Height;
	d3dpp.BackBufferWidth		 = d3ddm.Width ;
	*/

	d3dpp.Windowed				 = TRUE;
	d3dpp.SwapEffect			 = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat		 = d3ddm.Format;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;


	m_hr = m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL	   , 
									*this			   ,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp			   , 
									&m_pd3dDevice	   ) ;

	TestResult(m_hr,"Error CreateDevice\n");	
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppWnd::ReleaseD3D()
{
	SAFE_RELEASE(m_pd3dDevice)
	SAFE_RELEASE(m_pD3D)
}
////////////////////////////////////////////////////////////////////////
void AppWnd::Cleanup()
{
	for (auto it = m_ar3DObjects.begin(); it != m_ar3DObjects.end(); ++it)
	{
		delete (*it);
	}

	m_ar3DObjects.clear();
	m_pHelpText = NULL;

	SAFE_DELETE(m_pCamera);
	ReleaseD3D();
}
////////////////////////////////////////////////////////////////////////
void AppWnd::Render()
{
	m_hr = m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
													D3DCOLOR_ARGB(255,000,000,255), 1.0f, 0 );
	TestResult(m_hr,"Error Clear\n");

	m_hr = m_pd3dDevice->BeginScene();
	
	TestResult(m_hr,"Error BeginScene\n");
		
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
	m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xff0f0f0f);

	m_pCamera->SetViewProjMatrices(true);

	for (auto it = m_ar3DObjects.begin(); it != m_ar3DObjects.end(); ++it)
	{
		if ((*it)->IsVisible())
		{
			(*it)->Render();
		}
	}

	m_hr = m_pd3dDevice->EndScene();
	TestResult(m_hr,"Error EndScene\n");

	m_hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	TestResult(m_hr,"Error Present\n");
}
//////////////////////////////////////////////////////////////////////////
LRESULT	AppWnd::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled)
{
	if(LOWORD(wParam) == WA_INACTIVE)
	{
		m_pOwningApp.EnableOnIdle(false);
	}
	else
	{
		m_pOwningApp.EnableOnIdle(true);
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
LRESULT AppWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled)
{
	Cleanup();
	PostQuitMessage( 0 );
	return 0;
}
//////////////////////////////////////////////////////////////////////////
LRESULT AppWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled)
{	
	SetCapture(*this);
	m_blLButtDown = true;		
	return 0;
}
//////////////////////////////////////////////////////////////////////////
LRESULT AppWnd::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled)
{
	ReleaseCapture();
	m_blLButtDown = false;		
	return 0;
}
//////////////////////////////////////////////////////////////////////////
LRESULT	AppWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled)
{
	switch(wParam)
	{
	case VK_F1:
		m_pHelpText->SetIsVisible(!m_pHelpText->IsVisible());
		break;
	}

	return 0 ;
}
//////////////////////////////////////////////////////////////////////////
LRESULT	AppWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& blHandled)
{
	PAINTSTRUCT ps; 
	HDC hdc = BeginPaint(*this, &ps); 
	EndPaint(*this, &ps); 

	try
	{
		Render();
	}
	catch(...)
	{
		DestroyWindow();
	}
	
	return 0L; 
}
//////////////////////////////////////////////////////////////////////////
void AppWnd::Update()
{
	GetKeyboardState(m_arKeyboardState);
	GetCursorPos(&m_ptNewCursorPos);
	UpdateCamera();

    for (auto it = m_ar3DObjects.begin(); it != m_ar3DObjects.end(); ++it) 
	{
		(*it)->Update();
    }
}
////////////////////////////////////////////////////////////////////////
void AppWnd::UpdateCamera()
{
	D3DXVECTOR3 vCamPosChange(0,0,0);

	if(KeyPressed(VK_LEFT )) vCamPosChange.x = -0.7f;
	if(KeyPressed(VK_RIGHT)) vCamPosChange.x =  0.7f;
	if(KeyPressed(VK_DOWN )) vCamPosChange.z = -0.7f;
	if(KeyPressed(VK_UP   )) vCamPosChange.z =  0.7f;
	
	if(KeyPressed(VK_HOME )) ((Teapot*)m_ar3DObjects[3])->ChangeTemperature(+1) ;
	if(KeyPressed(VK_END  )) ((Teapot*)m_ar3DObjects[3])->ChangeTemperature(-1) ;
	
	m_pCamera->ChangePosCamSpace(vCamPosChange);

	D3DXVECTOR3 vChRot(0,0,0);
	
	if(m_blLButtDown)
	{
		vChRot = D3DXVECTOR3 (	(m_ptNewCursorPos.y - m_ptOldCursorPos.y)/150.0f, 
								(m_ptNewCursorPos.x - m_ptOldCursorPos.x)/150.0f,0);

		m_pCamera->ChangeRot(vChRot );
	}

	m_ptOldCursorPos = m_ptNewCursorPos;

	m_pCamera->SetViewProjMatrices(true);
}
//////////////////////////////////////////////////////////////////////////
