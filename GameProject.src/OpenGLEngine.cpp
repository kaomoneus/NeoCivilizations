#include "StdAfx.h"
#include "GLWorld.h"
#include <windows.h>



HGLRC hRC;
HDC hDC;
HWND		hWnd, hSplash;
HINSTANCE AppInstance;
TGame Game;

bool	keys[256];




void ShowMovie(char* FileName)
{

	char ExeName[20], *Text, buf[20];
	char* PlayerText = "Player";
	sprintf(buf,"%s", FileName);
	sprintf(ExeName, "%s", "Player.exe ");
	strcat(ExeName,buf); 

	WinExec(ExeName, SW_NORMAL);
	while (IsWindow(FindWindow(NULL,"Player")));

}


void ShowSplash()
{
	hSplash = CreateDialog(AppInstance,(char*)IDD_SPLASH,hWnd,0);
	ShowWindow(hSplash, SW_SHOW);
}



LRESULT CALLBACK WndProc(
				HWND	hWnd,
				UINT	message,
				WPARAM	wParam,
				LPARAM	lParam)
{
	RECT	Screen;		
	GLuint	PixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),	
		1,			
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,	
		16,				
		0, 0, 0, 0, 0, 0,
		0,				
		0,				
		0,				
		0, 0, 0, 0,		
		16,				
		0,				
		0,				
		PFD_MAIN_PLANE,	
		0,				
		0, 0, 0			
	};
	switch (message)	
	{
		case WM_CREATE:
		hDC = GetDC(hWnd);	
		PixelFormat = ChoosePixelFormat(hDC, &pfd);
		if (!PixelFormat)
		{
			MessageBox(0,"Can't Find A Suitable PixelFormat.","Error",MB_OK|MB_ICONERROR);
			PostQuitMessage(0);
			
			break;	
		}
		if(!SetPixelFormat(hDC,PixelFormat,&pfd))
		{
			MessageBox(0,"Can't Set The PixelFormat.","Error",MB_OK|MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}
		hRC = wglCreateContext(hDC);
		if(!hRC)
		{
			MessageBox(0,"Can't Create A GL Rendering Context.","Error",MB_OK|MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}
		if(!wglMakeCurrent(hDC, hRC))
		{
			MessageBox(0,"Can't activate GLRC.","Error",MB_OK|MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}
		GetClientRect(hWnd, &Screen);
		break;

		case WM_DESTROY:
		case WM_CLOSE:
		ChangeDisplaySettings(NULL, 0);

		wglMakeCurrent(hDC,NULL);
		wglDeleteContext(hRC);
		ReleaseDC(hWnd,hDC);
		Game.Exit();


		KillTimer(hWnd,1);
        PostQuitMessage(0);
		break;

		case WM_KEYDOWN:
		 keys[wParam] = TRUE;
		 Game.OnKey(keys);
		break;

		case WM_KEYUP:
		 keys[wParam] = FALSE;

		break;

		case WM_SIZE:
    	
		break;

//        case WM_TIMER:

//        break;

//		case WM_LBUTTONDBLCLK:
//			Game.OnLeftDbClick(LOWORD(lParam),HIWORD(lParam));
//		break;

		case WM_LBUTTONDOWN:
			Game.OnLeftButtonClick(LOWORD(lParam),HIWORD(lParam));
        break;

		case WM_RBUTTONDOWN:
			Game.OnRightButtonClick(LOWORD(lParam),HIWORD(lParam));
		break;

		case 0x020A:
			Game.OnWheel((short)HIWORD(wParam),(short)HIWORD(lParam),(short)LOWORD(lParam));
		break;

		case WM_MOUSEMOVE:
			Game.OnMouseMove(wParam,LOWORD(lParam),HIWORD(lParam));
		break;


		default:
	return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return (0);
}

int WINAPI WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance, 
		LPSTR lpCmdLine,
		int nCmdShow)
{
	MSG		msg;	
	WNDCLASS	wc; 

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC) WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(hInstance,(char*)IDI_MAIN_ICON);
	wc.hCursor			= LoadCursor(hInstance, (char*)IDC_MENU);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "OpenGL WinClass";

	if(!RegisterClass(&wc))
	{
	MessageBox(0,"Failed To Register The Window Class.","Error",MB_OK|MB_ICONERROR);
	return FALSE;
	}

	AppInstance = hInstance;
	//ShowMovie("Intro.avi");

	hWnd = CreateWindow(
	"OpenGL WinClass",
	"NeoCivilizations",	
	WS_POPUP |
	WS_CLIPCHILDREN |
	WS_CLIPSIBLINGS,

	0, 0,			
	1024, 768,		

	NULL,
	NULL,
	hInstance,
	NULL);

	

	if(!hWnd)
	{
	MessageBox(0,"Window Creation Error.","Error",MB_OK|MB_ICONERROR); 
		return FALSE;
	}


	Game.InitGL();


	ShowSplash();

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

    Game.Init(hRC, hDC, hWnd);


	HCURSOR C_World = LoadCursor(hInstance,(char*)IDC_WORLD);
	HCURSOR C_Menu = LoadCursor(hInstance,(char*)IDC_MENU);
	HCURSOR C_Pick = LoadCursor(hInstance,(char*)IDC_CURSOR2);



	Sleep(1000);


	Game.ChangeResolution(1024,768);

	DestroyWindow(hSplash);

//	ShowMovie("Intro.avi");

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

//	SetTimer(hWnd,1,TimerInterval,0);

	while (1)
	{

//		if(Game.Cursor.TypeChanged)
		{
			if(Game.Cursor.Type == CURSOR_WORLD) SetCursor(C_World);
			if(Game.Cursor.Type == CURSOR_MENU) SetCursor(C_Menu);
			if(Game.Cursor.Type == CURSOR_PICK) SetCursor(C_Pick);
			Game.Cursor.TypeChanged = false;
		}
		POINT P;
		WPARAM Btn;
		GetCursorPos(&P);
		if(GetKeyState(VK_RBUTTON)) Btn = MK_RBUTTON;
		if(GetKeyState(VK_LBUTTON)) Btn = MK_LBUTTON;
		if(GetKeyState(VK_MBUTTON)) Btn = MK_MBUTTON;

		//Game.OnMouseMove(Btn,P.x,P.y);

		

		Game.Refresh();
	    SwapBuffers(hDC);			
		if(Game.Interface.bExit) 
			SendMessage(hWnd,WM_CLOSE,0,0);	
	
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return TRUE;
			}
		}

	}
}


		