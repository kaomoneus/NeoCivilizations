
#include "StdAfx.h"
#include "GLWorld.h"






void TCursor::Init()
{
	IsRClick = false;
	IsLClick = false;
	ScreenX = 0;
	ScreenY = 0;
	OldScreenX = 0;
	OldScreenY = 0;
	Type = CURSOR_WORLD;
}






void TError::OnError(char* String)
{
	MessageBox(0,String,"Error!",MB_OK|MB_ICONERROR);

	ChangeDisplaySettings(NULL, 0);

	wglMakeCurrent(hDC,NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd,hDC);
//	Game.World.Exit;

	PostQuitMessage(0);


//	SendMessage(hWnd,WM_CLOSE,0,0);
}

//________________________________________________________________________________________________\\
//------------------------------------------------------------------------------------------------\\
//************************************************************************************************\\
//                        //                                         \\                                   \\
//------------------------//   Класс TGame - мир OpenGL + интерфейс  \\-----------------------------------\\  
//                        //                                         \\                                   \\ 
//************************************************************************************************\\
//------------------------------------------------------------------------------------------------\\
//________________________________________________________________________________________________\\



//*****************************- Иниациализация -*************************************************
//
//************************************************************************************************

void TGame::ChangeResolution(int ResX, int ResY)
{
	DEVMODE dmScreenSettings;		

	memset(&dmScreenSettings, 0, sizeof(DEVMODE));	
	dmScreenSettings.dmSize	= sizeof(DEVMODE);		
	dmScreenSettings.dmPelsWidth	= ResX;		
	dmScreenSettings.dmPelsHeight	= ResY;			
	dmScreenSettings.dmFields	= DM_PELSWIDTH | DM_PELSHEIGHT;	
	dmScreenSettings.dmDisplayFrequency = 75;
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

	MoveWindow(hWnd, 0, 0, ResX, ResY, true);



}


void TGame::InitGL()
{
 glViewport(0,0,1024,768);               
 glMatrixMode(GL_PROJECTION);           
 glLoadIdentity();                      

 glClearColor(0.5f, 0.5f, 0.5f, 0.5f);                                
 glClearDepth(100.0f); 
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    
 


// glPolygonMode(GL_FRONT,GL_FILL);
// glPolygonMode(GL_BACK,GL_LINE); 

};

void TGame::Init(HGLRC hRC_, HDC hDC_, HWND hWnd_)
{
	hWnd = hWnd_;
	hRC  = hRC_;
	hDC  = hDC_;

	Error.hWnd = hWnd;
	Error.hDC  = hDC;
	Error.hRC  = hRC;

	Cursor.Init();

    Society.GSocietyInit(true);
    

    World.Society = &Society;
	World.Pointer.Cursor  = &Cursor;
	World.Error   = &Error;

	Interface.Society = &Society;
	Interface.Cursor  = &Cursor;
	Interface.Error   = &Error;
	Interface.World   = &World; 

//-----инициализация дин. массивов---------------

//--------------------------------------------------

	World.Init(hRC_, hDC_);
	Interface.Init(hRC_, hDC_);

	CurrentResolutionX = 1024;
	CurrentResolutionY = 768;


	
}






void TGame::Refresh()
{
	if((Interface.fFocusChanged)&&(Interface.Focus != FOCUS_GAME_MENU))
	{
		Interface.fFocusChanged = false;
		ChangeResolution(800,600);
		glViewport(0, 0, 800, 600);

		CurrentResolutionX = 800;
		CurrentResolutionY = 600;

	}

	if((Interface.fFocusChanged)&&(Interface.Focus == FOCUS_GAME_MENU))
	{
		Interface.fFocusChanged = false;
		ChangeResolution(1024,768);
		glViewport(0,0,1024,768);
		
		CurrentResolutionX = 1024;
		CurrentResolutionY = 768;

	}
	
	float LastTime, CurrentTime;
	LastTime = timeGetTime();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if(Interface.Focus == FOCUS_WORLD)
	{
		Society.RefreshFrames(AnalisInterval,SetAngelsInterval,DoAIInterval,Frame);
	}

	if(Interface.Focus != FOCUS_GAME_MENU)
		World.Refresh();


	Interface.Refresh();

	auxSwapBuffers();

	CurrentTime = timeGetTime();
	Interface.CameraZ = World.Camera.Z;
	Interface.FPS = 1/((CurrentTime - LastTime)/1000);
	Interface.Frame++;  
}


void TGame::OnKey(bool Keys[256])
{
	World.Camera.Radius+=2;

	if( Keys[VK_UP])    World.SetNewCameraPos(0,1,0); 
	if( Keys[VK_DOWN])  World.SetNewCameraPos(0,-1,0);
	if( Keys[VK_LEFT])  World.SetNewCameraPos(-1,0,0);
	if( Keys[VK_RIGHT]) World.SetNewCameraPos(1,0,0); 
/*
	if(Keys['Q'])      World.Camera.XAngle++;
	if(Keys['W'])      World.Camera.XAngle--;
	if(Keys['A'])      World.Camera.ZAngle++;
	if(Keys['S'])      World.Camera.ZAngle--;
	


	if(Keys['F']) 
	{
		glPolygonMode( GL_BACK, GL_FILL );
		glPolygonMode( GL_FRONT, GL_FILL);
	}

	if(Keys['L']) 
	{
		glPolygonMode( GL_BACK, GL_LINE );
		glPolygonMode( GL_FRONT, GL_LINE);
	}

	if(Keys['E']) World.fLandscape=(!World.fLandscape); 
	if(Keys['O']) World.fObjects=(!World.fObjects); 
*/
	if(Keys['z'])
		World.fLandSmooth=(!World.fLandSmooth); 

	if (Keys[VK_ESCAPE])
	{
		if((Interface.Focus == FOCUS_WORLD)||(Interface.Focus == FOCUS_MENU))
		{
			Interface.Buttons[B_MAINMENU].Pressed = true;
			if(Interface.Focus == FOCUS_WORLD)
				Interface.Focus = FOCUS_MENU;
			else
				if(Interface.Focus == FOCUS_MENU)
					Interface.Focus = FOCUS_WORLD;

			Interface.fFocusChanged = true;
		}
	}

	if (Keys[VK_F2])
	{
		Interface.Console.bShow = !Interface.Console.bShow;
		Interface.Console.Visible = !Interface.Console.Visible;
	}



	World.Camera.bChangePos = true;

	if(Interface.Console.Visible)
		for (int i = 0; i < 256; i++)
			if(Keys[i])	
				Interface.Console.WriteChar(i);
}

void TGame::OnRightButtonClick(int X, int Y)
{
/*	Cursor.IsRClick = true;
	TCommand Command;
	Command.Type = COMMAND_GO;
	Command.Point.x = X;
	Command.Point.y = Y;
	World.OnCommand(Command);
	*/
	if(World.IsSelection)
	{
		World.Command.Is = true;
		World.Command.Type = COMMAND_GO;
		World.Command.Point.x = X;
		World.Command.Point.y = Y;
		Cursor.Type = CURSOR_PICK;
	}
}

void TGame::OnLeftButtonClick(int X, int Y)
{
	bool OnMenu = false;
	if(!Interface.Focus == FOCUS_GAME_MENU)
	{
		for (int i = 0; i < MAX_MENU; i++)
			if((Interface.Menu[i].Visible)&&(PointInRect(X,ResolutionY - Y,Interface.Menu[i].CurrentPlacement)))
				OnMenu = true;
	}
	else OnMenu = true;

	if(!OnMenu)
	{
		if(World.Pointer.bGetPoint&&World.IsSelection)
		{
			World.Command.Is = true;
			World.Command.Type = COMMAND_GO;
			World.Command.Point.x = X;
			World.Command.Point.y = Y;
			World.Pointer.bGetPoint = false;
			Cursor.Type = CURSOR_WORLD;
		}
		else
		{
			 Cursor.IsLClick = true;
			 Cursor.ScreenX = X;
			 Cursor.ScreenY = Y;
			 Cursor.Focus   = FOCUS_WORLD;
			 Cursor.Enabled = true;
			 World.Pointer.bNewSelection = true;
		}
	}
	else
	{
	  Interface.OnLeftButtonClick(int(X*ResolutionX/CurrentResolutionX),
								  int(ResolutionY - Y*ResolutionY/CurrentResolutionY));

	  Cursor.Type = CURSOR_MENU;
	}
}



void TGame::OnWheel(int Rotation, int X, int Y)
{
	float R = Rotation/20;
/*
	World.Camera.Z += R*cos(World.Camera.XAngle/OGIR);
	World.Camera.Y += R*sin(World.Camera.XAngle/OGIR);

	World.Camera.Y += R*cos(World.Camera.ZAngle/OGIR);
	World.Camera.X += R*sin(World.Camera.ZAngle/OGIR);


	World.Camera.bChangePos = true;

*/

	World.SetNewCameraPos(0,0,R);




}

void TGame::OnMouseMove(int Button, int X, int Y)
{

	bool OnMenu = false;
	for (int i = 0; i < MAX_MENU; i++)
		if((Interface.Menu[i].Visible)&&(PointInRect(X,ResolutionY - Y,Interface.Menu[i].CurrentPlacement)))
			OnMenu = true;

	  Interface.OnMouseMove(Button,
							int(X*ResolutionX/CurrentResolutionX),
							int(ResolutionY - Y*ResolutionY/CurrentResolutionY));

		if(X == 0)               World.SetNewCameraPos(-5,0,0);
		if(X == ResolutionX - 1) World.SetNewCameraPos(5,0,0);

		if(Y == 0)               World.SetNewCameraPos(0,5,0);
		if(Y == ResolutionY - 1) World.SetNewCameraPos(0,-5,0);




	if(!OnMenu)
	{
		if(World.Pointer.bGetPoint)
			Cursor.Type = CURSOR_PICK;
		else 
			Cursor.Type = CURSOR_WORLD;
/*		 if(Cursor.Type != CURSOR_WORLD)
		 {
			 Cursor.Type = CURSOR_WORLD;
			 Cursor.TypeChanged = true;
		 }
*/
		Cursor.ScreenX = X;
		Cursor.ScreenY = Y;

		if((Cursor.OldScreenX!=0)&&(Cursor.OldScreenY!=0)&&(Button == MK_MBUTTON))
		{
			if(PointInInterval(World.Camera.XAngle + (Cursor.ScreenY - Cursor.OldScreenY)/5,-15,-65))
				World.Camera.XAngle = StdAngle(World.Camera.XAngle + (Cursor.ScreenY - Cursor.OldScreenY)/5);
			World.Camera.ZAngle = StdAngle(World.Camera.ZAngle + (Cursor.ScreenX - Cursor.OldScreenX)/5);
			World.Camera.bChangePos = true;
		}

		if((Cursor.OldScreenX!=0)&&(Cursor.OldScreenY!=0)&&(Button == MK_LBUTTON))
		{
			World.Pointer.bRect = true;

		}
		else
		{
			World.Pointer.bRect = false;
			World.Pointer.StartRect.X = 0;
			World.Pointer.StartRect.Y = 0;
			World.Pointer.StartRect.Z = 0;
			World.Pointer.EndRect.X = 0;
			World.Pointer.EndRect.Y = 0;
			World.Pointer.EndRect.Z = 0;
		}

		Cursor.OldScreenX = Cursor.ScreenX;
		Cursor.OldScreenY = Cursor.ScreenY;


	}
	else
		Cursor.Type = CURSOR_MENU;
/*		 if(Cursor.Type != CURSOR_MENU)
		  {
			  Cursor.Type = CURSOR_MENU;
			  Cursor.TypeChanged = true;
		  }
*/
}


void TGame::OnLeftDbClick(int X, int Y)
{
	Cursor.IsLDbClick = true;
}






void TGame::Exit()
{
	for (int m = 0; m < MAX_MODELS; m++)
		for (int a = 0; a < MAX_ACTIONS; a++)
			for (int f = 0; f < MAX_FRAMES; f++)
				glDeleteLists(World.ObjectList[m][a][f],1);


    glDeleteTextures(MAX_MODELTEX, World.ModelTextures);
	glDeleteTextures(1,World.FireTextures);
	glDeleteTextures(1,&(World.LandTexture));

	glDeleteTextures(MAX_IMAGES,Interface.Images);

	delete [] World.Land.VisibleMap;
	delete [] Interface.MiniMap.PixelMap;
}
