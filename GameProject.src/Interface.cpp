#include "StdAfx.h"
#include "GLWorld.h"
//#include "xImage.h"
#include <vector>

int f;
//________________________________________________________________________________________________
//------------------------------------------------------------------------------------------------
//                   //                                           \\                              
//-------------------//   Класс TGraphicObject - рисование меню, кнопок и т.д.     \\------------------------------  
//                   //                                           \\                               
//------------------------------------------------------------------------------------------------
//________________________________________________________________________________________________

bool fViewDebug;

void TGraphicObject::Draw()
{

	if(bHide)
	{

		if(HideTime < MaxHideTime)
			HideTime++;
		else
		{
			bHide = false;
			Visible = false;
		}

		CurrentPlacement.left = Placement.left + HideSpeedX * HideTime;
		CurrentPlacement.right = Placement.right + HideSpeedX * HideTime;
		CurrentPlacement.bottom = Placement.bottom + HideSpeedY * HideTime;
		CurrentPlacement.top = Placement.top + HideSpeedY * HideTime;
	}

	if(bShow)
	{
		Visible = true;

		if(HideTime > 0)
			HideTime--;
		else
		{
			HideTime = 0;
			bShow = false;
		}

		CurrentPlacement.left = Placement.left + HideSpeedX * HideTime;
		CurrentPlacement.right = Placement.right + HideSpeedX * HideTime;
		CurrentPlacement.bottom = Placement.bottom + HideSpeedY * HideTime;
		CurrentPlacement.top = Placement.top + HideSpeedY * HideTime;
	}

	if((!bShow)&&(!bHide))
	{
		CurrentPlacement.left = Placement.left;
		CurrentPlacement.right = Placement.right;
		CurrentPlacement.bottom = Placement.bottom;
		CurrentPlacement.top = Placement.top;
	}


	if(ParentPlacement)
	{
		CurrentPlacement.left +=ParentPlacement->left;
		CurrentPlacement.right +=ParentPlacement->left;
		CurrentPlacement.bottom +=ParentPlacement->bottom;
		CurrentPlacement.top +=ParentPlacement->bottom;
	}

	if(Visible)
	{
		glColor4f(1,1,1,Alpha);

		if(IsButton)
		{
			if(Pressed&&(PressTime <= MaxPressTime))
			{
				Frame = 2;
				PressTime++;
			}
			else
			{
				Frame = 0;
				PressTime = 0;
				Pressed = false;
			}
		}

		glBindTexture(GL_TEXTURE_2D, Textures[Frame]);
		glBegin(GL_QUADS);
			glTexCoord2f(0,1);
			glVertex3f(CurrentPlacement.left, CurrentPlacement.bottom,0);

			glTexCoord2f(0,0);
			glVertex3f(CurrentPlacement.left, CurrentPlacement.top,0);

			glTexCoord2f(1,0);
			glVertex3f(CurrentPlacement.right, CurrentPlacement.top,0);

			glTexCoord2f(1,1);
			glVertex3f(CurrentPlacement.right, CurrentPlacement.bottom,0);
		glEnd();
	}

}


//________________________________________________________________________________________________
//------------------------------------------------------------------------------------------------
//                   //                                           \\                              
//-------------------//   Класс TConsole - консоль:)   \\------------------------------  
//                   //                                           \\                               
//------------------------------------------------------------------------------------------------
//________________________________________________________________________________________________


void TConsole::WriteChar(char Sumbol)
{

	num_lines++;
	char tmp[255];
	switch (Sumbol)
	{
		case VK_RETURN:
			{
				sprintf(Lines[num_lines-1], "%s", ">");
				strcpy(Command, Lines[num_lines-2]);
				NewCommand = true;
			}
        break;

/*		case 'F':
			{
				sprintf(Lines[num_lines-1], "%s", ">Filled mode");
				num_lines++;
				sprintf(Lines[num_lines-1], "%s", ">");
			}

		break;

		case 'L':
			{
				sprintf(Lines[num_lines-1], "%s", ">Line mode");
				num_lines++;
				sprintf(Lines[num_lines-1], "%s", ">");
			}
		break;

		case 'E':
			{
				sprintf(Lines[num_lines-1], "%s", ">Change land visible");
				num_lines++;
				sprintf(Lines[num_lines-1], "%s", ">");
			}
		break;
		
		case 'O':
			{
				sprintf(Lines[num_lines-1], "%s", ">Change objects visible");
				num_lines++;
				sprintf(Lines[num_lines-1], "%s", ">");
			}
		break;
*/
		case VK_ESCAPE:
			{
				sprintf(Lines[num_lines-1], "%s", ">Главное меню");
				num_lines++;
				sprintf(Lines[num_lines-1], "%s", ">");
			}
		break;

		default:
			num_lines--;
			if(isalnum(Sumbol)&&(!PointInInterval(Sumbol, float(0x70), float(0x87))))
			{
				sprintf(tmp, "%c", Sumbol);
				strcat(Lines[num_lines - 1], tmp);
			}
        break;

	}

}


void TConsole::Draw()
{
	for (int i = num_lines - num_visible; i < num_lines; i++)
	{
		glRasterPos2f(20,520 + (num_lines - i)*15);
		Text->Draw2DText(Lines[i]);
	}
}




//________________________________________________________________________________________________\\
//------------------------------------------------------------------------------------------------\\
//                   //                                           \\                              \\
//-------------------//   Класс T2DText - вывод текста на экран   \\------------------------------\\  
//                   //                                           \\                              \\ 
//------------------------------------------------------------------------------------------------\\
//________________________________________________________________________________________________\\
 
//*****************************- Иниациализация -*************************************************
//
//************************************************************************************************

bool T2DText::Init(HGLRC hRC, HDC hDC)
{
 base = glGenLists(96);  
 font = CreateFont(  -12,        
        0,        
        0,        
        0,        
        FW_BOLD,    
        FALSE,    
        FALSE,       
        FALSE,       
        RUSSIAN_CHARSET,    
        OUT_TT_ONLY_PRECIS,   
        CLIP_DEFAULT_PRECIS,  
        ANTIALIASED_QUALITY,   
        FF_DONTCARE|VARIABLE_PITCH,
        "Courier");      
 
 SelectObject(hDC, font);
 
 return wglUseFontBitmaps(hDC, 0, 256, base);

}



void T2DText::Draw2DText(char *String, ...)
{

  char    text[256];
  if (String == NULL)
    return;
  va_list    ap; 
  va_start(ap, String);
  vsprintf(text, String, ap);
  va_end(ap);
  glPushAttrib(GL_LIST_BIT);
  glPushMatrix();
  if(base > 18)
  {
  glListBase(base); 
    glColor3f(0,1,0); 
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
  }
  glPopMatrix();
  glPopAttrib(); 

}




//________________________________________________________________________________________________\\
//------------------------------------------------------------------------------------------------\\
//                   //                                           \\                              \\
//-------------------//   Класс T3DText - вывод текста на экран   \\------------------------------\\  
//                   //                                           \\                              \\ 
//------------------------------------------------------------------------------------------------\\
//________________________________________________________________________________________________\\
 
//*****************************- Иниациализация -*************************************************
//
//************************************************************************************************

bool T3DText::Init(HGLRC hRC, HDC hDC)
{
 base = glGenLists(256);  
 font = CreateFont(  72,        
        0,        
        0,        
        0,        
        FW_BOLD,    
        FALSE,    
        FALSE,       
        FALSE,       
        RUSSIAN_CHARSET,    
        OUT_TT_ONLY_PRECIS,   
        CLIP_DEFAULT_PRECIS,  
        ANTIALIASED_QUALITY,   
        FF_DONTCARE|VARIABLE_PITCH,
        "Arial");      
 
 SelectObject(hDC, font);
 
 return (wglUseFontOutlines(hDC,0,255,base,10.0f,0.2f,WGL_FONT_POLYGONS,gmf));

}



void T3DText::Draw3DText(char *String)
{

 float         length=0;     
 char          text[256];    
 va_list              ap; 	
 va_start(ap, String);    
 vsprintf(text, String, ap);
 va_end(ap);  
 for (int loop=0;loop<sizeof(String)/sizeof(char)-1;loop++)
  length+=gmf[text[loop]].gmfCellIncX;
 
 glPushMatrix();
  glTranslatef(-length/2+30,0.0f,0.0f);
  glRotated(90,1,0,0);
   glPushAttrib(GL_LIST_BIT);
   glListBase(base); 
   glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
  glPopAttrib();
 glPopMatrix();

}


//________________________________________________________________________________________________\\
//------------------------------------------------------------------------------------------------\\
//                   //                                       \\                                  \\
//-------------------//     Класс TMenu - игровое меню        \\----------------------------------\\  
//                   //                                       \\                                  \\ 
//------------------------------------------------------------------------------------------------\\
//________________________________________________________________________________________________\\
 
//*****************************- Иниациализация -*************************************************
//
//************************************************************************************************

void TMenu::Init()
{
	Enabled = true;
}



//________________________________________________________________________________________________\\
//------------------------------------------------------------------------------------------------\\
//                   //                                       \\                                  \\
//-------------------//     Класс TMiniMap - мини-карта        \\----------------------------------\\  
//                   //                                       \\                                  \\ 
//------------------------------------------------------------------------------------------------\\
//________________________________________________________________________________________________\\
 
//*****************************- Иниациализация -*************************************************
//
//************************************************************************************************
inline	void Set(T_RGBA &P,
			 unsigned char Red,
		     unsigned char Green,
			 unsigned char Blue,
			 unsigned char Alpha)

			 {
				P.R = Red;
				P.G = Green;
				P.B = Blue;
				P.A = Alpha;
			};

void TMiniMap::Init()
{
std::vector<unsigned char> M(MAP_WIDTH*MAP_HEIGHT*4);

//  unsigned char M[MAP_WIDTH*MAP_HEIGHT*4];
/*	for(int x = 0; x < MAP_WIDTH; x++)
		for(int y = 0; y < MAP_HEIGHT; y++)
		{
			if(World->Society->MAP[x][y].Z <= 3)
				Set(PixelMap[y][x],50, 150, 40, 150);
			else 
				Set(PixelMap[y][x],60, 70, 170, 150);
		}
*/
int i = 0;
for(int x = 0; x < MAP_WIDTH; x++)
 for(int y = 0; y < MAP_HEIGHT; y++)
 {
	 if(World->Society->MAP[y][x].Z < 3)
	 {
	  M[i] = 100;
	  M[i+1] = 200;
	  M[i+2] = 100;
	  M[i+3] = 150;
	 }
	 else
	 {
	  M[i] = 20;
	  M[i+1] = 55;
	  M[i+2] = 100;
	  M[i+3] = 150;
	 }
     i += 4;
 }

/*  unsigned char TexMap[256][256][4];

  int C = (int)MAP_WIDTH/256;

  for(x = 0; x < 256; x++)
	  for(int y = 0; y < 256; y++)
	  {
		  TexMap[x][y][0] = PixelMap[C*x][C*y][0];
		  TexMap[x][y][1] = PixelMap[C*x][C*y][1];
		  TexMap[x][y][2] = PixelMap[C*x][C*y][2];
		  TexMap[x][y][3] = PixelMap[C*x][C*y][3];
	  }
*/


  glGenTextures(1, &Map);                                        
  glBindTexture(GL_TEXTURE_2D, Map);
//  glTexImage2D(GL_TEXTURE_2D, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, &TexMap[0][0][0]);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, (int)MAP_WIDTH, (int)MAP_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, &M.front());
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
void TMiniMap::Refresh()
{

	for(int t = 0; t < NUMBER_OF_TEAMS; t++)
		for(int i = 0; i < NUMBER_OF_UNITS; i++)
		{
			if(World->Society->GObject[t][i].Is)
			{
				char r = TEAM_COLORS[t][0];
				char g = TEAM_COLORS[t][1];
				char b = TEAM_COLORS[t][2];

				PixelMap[World->Society->GObject[t][i].Y][World->Society->GObject[t][i].X].Set(r,g,b,255);
			}
		}

}
*/
void TMiniMap::Draw()
{
/*	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	glRasterPos2f(Boundaries.left,Boundaries.bottom);
	glPixelZoom(Scale,Scale);
	glDrawPixels(MAP_WIDTH,MAP_HEIGHT,
	             GL_RGBA,
	    		 GL_UNSIGNED_BYTE,
			     PixelMap);
*/
float StartX = Boundaries.left;//ResolutionX - MAP_WIDTH*MINIMAP_SCALE - 10;
float StartY = Boundaries.bottom;//ResolutionY - MAP_HEIGHT*MINIMAP_SCALE - 10;

glEnable(GL_SCISSOR_TEST);

glDisable(GL_BLEND);

glScissor(StartX, StartY, MAP_WIDTH*Scale, MAP_HEIGHT*Scale);

glBegin(GL_POINTS);

	for(int t = 0; t < NUMBER_OF_TEAMS; t++)
		for(int i = 0; i < NUMBER_OF_UNITS; i++)
		{
			if(World->Society->GObject[t][i].Is)
			{
				glColor3ubv(&TEAM_COLORS[t][0]);
				glVertex3f(StartX + World->Society->GObject[t][i].X*Scale,
						   StartY + World->Society->GObject[t][i].Y*Scale,0);
			}
		}


glEnd();

glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA,GL_ONE);
glColor4f(0.8,0.4,0.8,0.5);
glBegin(GL_QUADS);

	glVertex3f(StartX + World->Camera.Region.Vertex[0].x*Scale,
			   StartY + World->Camera.Region.Vertex[0].y*Scale,0);
	glVertex3f(StartX + World->Camera.Region.Vertex[1].x*Scale,
			   StartY + World->Camera.Region.Vertex[1].y*Scale,0);

/*	glVertex3f(StartX + World->Camera.Region.Vertex[1].x*Scale,
			   StartY + World->Camera.Region.Vertex[1].y*Scale,0);
	glVertex3f(StartX + World->Camera.Region.Vertex[2].x*Scale,
			   StartY + World->Camera.Region.Vertex[2].y*Scale,0);
*/
	glVertex3f(StartX + World->Camera.Region.Vertex[2].x*Scale,
			   StartY + World->Camera.Region.Vertex[2].y*Scale,0);
	glVertex3f(StartX + World->Camera.Region.Vertex[3].x*Scale,
			   StartY + World->Camera.Region.Vertex[3].y*Scale,0);

//	glVertex3f(StartX + World->Camera.Region.Vertex[3].x*Scale,
//			   StartY + World->Camera.Region.Vertex[3].y*Scale,0);
	glVertex3f(StartX + World->Camera.Region.Vertex[0].x*Scale,
			   StartY + World->Camera.Region.Vertex[0].y*Scale,0);

glEnd();
glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
glEnable(GL_TEXTURE_2D);
glColor4f(1,1,1,0.5);

		glBindTexture(GL_TEXTURE_2D, Map);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(Boundaries.left, Boundaries.bottom,0);

			glTexCoord2f(0,1);
			glVertex3f(Boundaries.left, Boundaries.top,0);

			glTexCoord2f(1,1);
			glVertex3f(Boundaries.right, Boundaries.top,0);

			glTexCoord2f(1,0);
			glVertex3f(Boundaries.right, Boundaries.bottom,0);
		glEnd();


glDisable(GL_SCISSOR_TEST);
glDisable(GL_BLEND);

}


//________________________________________________________________________________________________\\
//------------------------------------------------------------------------------------------------\\
//                   //                                       \\                                  \\
//-------------------//     Класс TInterface - интерфейс    \\----------------------------------\\  
//                   //                                       \\                                  \\ 
//------------------------------------------------------------------------------------------------\\
//________________________________________________________________________________________________\\
 
//*****************************- Иниациализация -*************************************************
//
//************************************************************************************************

void TInterface::Init(HGLRC hRC_, HDC hDC_)
{
	f = 0;

	FPS = 0;
	Frame = 0;
	CameraZ = 0;
	fChangeFocus = false;
	Text2D.Init(hRC_,hDC_);

	Focus = FOCUS_GAME_MENU;

	MiniMap.Scale = MINIMAP_SCALE;

	MiniMap.World = World;

	MiniMap.Boundaries.bottom = ResolutionY - MAP_HEIGHT*MINIMAP_SCALE - 10;
	MiniMap.Boundaries.right   = ResolutionX - 10;
	MiniMap.Boundaries.left  = ResolutionX - MAP_WIDTH*MINIMAP_SCALE - 10; 
	MiniMap.Boundaries.top    = ResolutionY - 10; 

	MiniMap.Init();

	LoadImages();

	sprintf(Console.Lines[0], "%s", "Загрузка.........OK");
	sprintf(Console.Lines[1], "%s", "Инициализация....OK");
	sprintf(Console.Lines[2], "%s", ">");
	Console.num_lines = 3;
	Console.num_visible = 3;

	Console.Text = &Text2D;


	Buttons[B_EXIT].HintString = "Выход в главное меню";
	Buttons[B_COMMAND_GO].HintString = "Отправить юнитов";
	Buttons[B_COMMAND_ATTACK].HintString = "Изменить способ атаки";
	Buttons[B_MAINMENU].HintString = "Меню";
	Buttons[B_COMMAND_STOP].HintString = "Изменить способ движения";
	Buttons[B_COMMAND_CHASE].HintString = "Преследовать";  
	Buttons[B_GAME_START].HintString = "Начать игру!";
	Buttons[B_GAME_EXIT].HintString = "Выход из игры";


	Panels[P_UNIT].Placement.left = 15;  
	Panels[P_UNIT].Placement.right = 115;  
	Panels[P_UNIT].Placement.bottom = 25;  
	Panels[P_UNIT].Placement.top = 113;
	Panels[P_UNIT].Textures[0] = Images[6];
	Panels[P_UNIT].Visible = false;
	Panels[P_UNIT].Alpha = 0.7;
	Panels[P_UNIT].ParentPlacement =  &Menu[M_UNIT].CurrentPlacement;


	LoadInterface();

	Menu[M_UNIT].Strings[0].Text = "Жизнь:";
	Menu[M_UNIT].Strings[0].Pos.x = 160;
	Menu[M_UNIT].Strings[0].Pos.y = 330;

	Menu[M_UNIT].Strings[1].Text = "%i";
	Menu[M_UNIT].Strings[1].Pos.x = 170;
	Menu[M_UNIT].Strings[1].Pos.y = 318;



	Menu[M_UNIT].Strings[2].Text = "Урон:";
	Menu[M_UNIT].Strings[2].Pos.x = 160;
	Menu[M_UNIT].Strings[2].Pos.y = 250;

	Menu[M_UNIT].Strings[3].Text = "%i";
	Menu[M_UNIT].Strings[3].Pos.x = 165;
	Menu[M_UNIT].Strings[3].Pos.y = 238;

	
	Menu[M_UNIT].Strings[4].Text = "Скорость:";
	Menu[M_UNIT].Strings[4].Pos.x = 152;
	Menu[M_UNIT].Strings[4].Pos.y = 215;

	Menu[M_UNIT].Strings[5].Text = "%i";
	Menu[M_UNIT].Strings[5].Pos.x = 163;
	Menu[M_UNIT].Strings[5].Pos.y = 203;


	Menu[M_UNIT].Strings[6].Text = "Зрение:";
	Menu[M_UNIT].Strings[6].Pos.x = 160;
	Menu[M_UNIT].Strings[6].Pos.y = 285;

	Menu[M_UNIT].Strings[7].Text = "%i";
	Menu[M_UNIT].Strings[7].Pos.x = 165;
	Menu[M_UNIT].Strings[7].Pos.y = 272;

	ConMenu[0].Placement.left = 100;
	ConMenu[0].Placement.right = 200;
	ConMenu[0].Placement.bottom = 400;
	ConMenu[0].Placement.top = 100;

	ConMenu[0].ParentPlacement = NULL;
	ConMenu[0].Alpha = 255;



}


void TInterface::LoadInterface()
{
 FILE* File;

 File = fopen("Menu.ini","r+b");
 for (int i = 0; i < MAX_MENU; i++)
 {
	fscanf(File,"%i",&Menu[i].Placement.left);
	fscanf(File,"%i",&Menu[i].Placement.right);
	fscanf(File,"%i",&Menu[i].Placement.bottom);
	fscanf(File,"%i",&Menu[i].Placement.top);

	fscanf(File,"%f",&Menu[i].Alpha);

	fscanf(File,"%i",&Menu[i].bHide);
	fscanf(File,"%i",&Menu[i].bShow);
	fscanf(File,"%i",&Menu[i].Visible);

	fscanf(File,"%i",&Menu[i].HideSpeedX);
	fscanf(File,"%i",&Menu[i].HideSpeedY);
	fscanf(File,"%i",&Menu[i].MaxHideTime);

	fscanf(File,"%i",&Menu[i].TexId);
	Menu[i].Textures[0] = Images[Menu[i].TexId];
 }

 File = fopen("Buttons.ini","r+b");
 for (int i = 0; i < MAX_BUTTONS; i++)
 {
	fscanf(File,"%i",&Buttons[i].Placement.left);
	fscanf(File,"%i",&Buttons[i].Placement.right);
	fscanf(File,"%i",&Buttons[i].Placement.bottom);
	fscanf(File,"%i",&Buttons[i].Placement.top);

	fscanf(File,"%f",&Buttons[i].Alpha);

	fscanf(File,"%i",&Buttons[i].Visible);
	fscanf(File,"%i",&Buttons[i].IsButton);

	fscanf(File,"%i",&Buttons[i].MaxPressTime);

	fscanf(File,"%i",&Buttons[i].ParentMenuId);
		Buttons[i].ParentPlacement = &Menu[Buttons[i].ParentMenuId].CurrentPlacement;

	fscanf(File,"%i",&Buttons[i].HintEnabled);

	fscanf(File,"%i",&Buttons[i].Tex1Id);
		Buttons[i].Textures[0] = Images[Buttons[i].Tex1Id];

	fscanf(File,"%i",&Buttons[i].Tex2Id);
		Buttons[i].Textures[2] = Images[Buttons[i].Tex2Id];

 }


}




void TInterface::LoadImages()
{

 FILE* File;

 int X,Y;
 char* data = new char[1024*512*4];

 File = fopen("Images/Main.raw","r+b");
 if(!File) Error->OnError(" Не могу загрузить Menu.raw");

  fread(data,sizeof(char),256*256*4,File);

  glGenTextures(1, &Images[0]);                                        
  glBindTexture(GL_TEXTURE_2D, Images[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

 File = fopen("Images/Button.raw","r+b");
 if(!File) Error->OnError(" Не могу загрузить Button.raw");

  fread(data,sizeof(char),128*64*4,File);

  glGenTextures(1, &Images[1]);                                        
  glBindTexture(GL_TEXTURE_2D, Images[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 128, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


 fclose(File);

 File = fopen("Images/MiniMap.raw","r+b");
 if(!File) Error->OnError(" Не могу загрузить Button.raw");

  fread(data,sizeof(char),256*256*4,File);

  glGenTextures(1, &Images[2]);                                        
  glBindTexture(GL_TEXTURE_2D, Images[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


 fclose(File);

 File = fopen("Images/Unit.raw","r+b");
 if(!File) Error->OnError(" Не могу загрузить Unit.raw");

  fread(data,sizeof(char),256*512*4,File);

  glGenTextures(1, &Images[3]);                                        
  glBindTexture(GL_TEXTURE_2D, Images[3]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 256, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


 fclose(File);

 File = fopen("Images/Btn1.raw","r+b");
 if(!File) Error->OnError(" Не могу загрузить Btn1.raw");

  fread(data,sizeof(char),64*32*4,File);

  glGenTextures(1, &Images[4]);                                        
  glBindTexture(GL_TEXTURE_2D, Images[4]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


 fclose(File);

 File = fopen("Images/Btn2.raw","r+b");
 if(!File) Error->OnError(" Не могу загрузить Btn2.raw");

  fread(data,sizeof(char),64*32*4,File);

  glGenTextures(1, &Images[5]);                                        
  glBindTexture(GL_TEXTURE_2D, Images[5]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


 fclose(File);

 File = fopen("Images/Panel1.raw","r+b");
 if(!File) Error->OnError(" Не могу загрузить Panel1.raw");

  fread(data,sizeof(char),128*128*4,File);

  glGenTextures(1, &Images[6]);                                        
  glBindTexture(GL_TEXTURE_2D, Images[6]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


 fclose(File);

 File = fopen("Images/Start.raw","r+b");
 if(!File) Error->OnError(" Не могу загрузить Start.raw");

  fread(data,sizeof(char),1024*512*4,File);

  glGenTextures(1, &Images[7]);                                        
  glBindTexture(GL_TEXTURE_2D, Images[7]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 1024, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


 fclose(File);


/* CxImage *Img;

 Img->Load("Images/Menu.png", CXIMAGE_FORMAT_PNG);

  glGenTextures(1, &Images[7]);                                        
  glBindTexture(GL_TEXTURE_2D, Images[7]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, Img->GetWidth(), Img->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Img->GetBits());
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

*/



 delete [] data;

}




void TInterface::Prepare()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,ResolutionX,0,ResolutionY,-1,1);
    glMatrixMode(GL_MODELVIEW);                                       
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

}


void TInterface::SetMenuVisible()
{
	if(Focus == FOCUS_GAME_MENU)
	{
		Menu[M_MAP].Visible = false;
		Menu[M_MAIN].Visible = false;

		Menu[M_START].Visible = true;
		ConMenu[0].Visible = false;
	}

	if((Focus == FOCUS_MENU))
	{
		Menu[M_START].Visible = false;

		Menu[M_MAP].Visible = true;
		Menu[M_MAIN].Visible = true;
		ConMenu[0].Visible = false;

	}

	if((Focus == FOCUS_WORLD))
	{
		Menu[M_START].Visible = false;

		Menu[M_MAP].Visible = true;
		Menu[M_MAIN].Visible = false;

	}

}




void TInterface::Refresh()
{

	Prepare();
	glPushMatrix();

	SetMenuVisible();

	if(f < MAX_FRAMES)  f = 0;

//	if(!f) 	MiniMap.Refresh();

	f++;

    if(Menu[M_MAP].Visible)
		MiniMap.Draw();

	int s = 0;

	TGObject* Unit;

	for (int t = 0; t < NUMBER_OF_TEAMS; t++)
		for (int u = 0; u < NUMBER_OF_UNITS; u++)
			if(Society->GObject[t][u].Selected&&Society->GObject[t][u].Is)
			{
				s++;
				Unit = &Society->GObject[t][u];
			}
	if((s > 0)&&(!Menu[M_UNIT].bHide)&&(!Menu[M_START].Visible))
	{
		Menu[M_UNIT].bShow = true;
		Buttons[B_COMMAND_GO].Visible = true;
	}
	else 
		if(!Menu[M_UNIT].bShow)
			Menu[M_UNIT].bHide = true;



glDisable(GL_BLEND);
	glColor4f(0,1,0,1);
	glDisable(GL_TEXTURE_2D);

	if(!fViewDebug)
	{
		glDisable(GL_BLEND);
		glRasterPos2f(20 ,490);
		Text2D.Draw2DText("FPS = %7.1f",FPS);

		glRasterPos2f(20, 475);
		Text2D.Draw2DText("Кадр = %7i",Frame);
	}


	if(Console.bShow)
	{
		Console.bShow = false;
		Console.Timer = timeGetTime();
	}

		glEnable(GL_BLEND);

	if(timeGetTime() - Console.Timer > CONSOLE_VISIBLE_TIME*1000)
		Console.Visible = false;
	if(Console.Visible)
		Console.Draw();

	glEnable(GL_TEXTURE_2D);


	for (int i = 0; i < MAX_MENU; i++)
			Menu[i].Draw();

	Buttons[B_EXIT].Visible = Menu[M_MAIN].Visible;
	Buttons[B_COMMAND_GO].Visible = Menu[M_UNIT].Visible;
	Buttons[B_COMMAND_STOP].Visible = Menu[M_UNIT].Visible;
	Buttons[B_COMMAND_CHASE].Visible = Menu[M_UNIT].Visible;
	Buttons[B_COMMAND_ATTACK].Visible = Menu[M_UNIT].Visible;
	Buttons[B_MAINMENU].Visible = Menu[M_MAP].Visible;
	Buttons[B_GAME_START].Visible = Menu[M_START].Visible;
	Buttons[B_GAME_EXIT].Visible = Menu[M_START].Visible;

	Panels[P_UNIT].Visible = Menu[M_UNIT].Visible;


	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		if(Buttons[i].Visible)
		{
			Buttons[i].Draw();
			if(Buttons[i].IsHint)
			{
				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				glColor4f(0,1,0,1);
				glRasterPos2f(Buttons[i].ParentPlacement->left + Buttons[i].Placement.left + 5,Buttons[i].ParentPlacement->bottom + Buttons[i].Placement.top + 1);
				Text2D.Draw2DText(Buttons[i].HintString);
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
				Buttons[i].HintTime--;
				if(Buttons[i].HintTime == 1)
					Buttons[i].IsHint = false;
			}
		}

	}

	if(Menu[M_UNIT].Visible&&(s > 0))
	{
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor4f(0,1,0,1);
		glRasterPos2f(Menu[M_UNIT].CurrentPlacement.left + Menu[M_UNIT].Strings[0].Pos.x,
					  Menu[M_UNIT].CurrentPlacement.bottom + Menu[M_UNIT].Strings[0].Pos.y);
		Text2D.Draw2DText(Menu[M_UNIT].Strings[0].Text);

		glRasterPos2f(Menu[M_UNIT].CurrentPlacement.left + Menu[M_UNIT].Strings[1].Pos.x,
					  Menu[M_UNIT].CurrentPlacement.bottom + Menu[M_UNIT].Strings[1].Pos.y);
		Text2D.Draw2DText(Menu[M_UNIT].Strings[1].Text,Unit->Live);


		glRasterPos2f(Menu[M_UNIT].CurrentPlacement.left + Menu[M_UNIT].Strings[2].Pos.x,
					  Menu[M_UNIT].CurrentPlacement.bottom + Menu[M_UNIT].Strings[2].Pos.y);
		Text2D.Draw2DText(Menu[M_UNIT].Strings[2].Text);

		glRasterPos2f(Menu[M_UNIT].CurrentPlacement.left + Menu[M_UNIT].Strings[3].Pos.x,
					  Menu[M_UNIT].CurrentPlacement.bottom + Menu[M_UNIT].Strings[3].Pos.y);
		Text2D.Draw2DText(Menu[M_UNIT].Strings[3].Text,Unit->Damage);


		glRasterPos2f(Menu[M_UNIT].CurrentPlacement.left + Menu[M_UNIT].Strings[4].Pos.x,
					  Menu[M_UNIT].CurrentPlacement.bottom + Menu[M_UNIT].Strings[4].Pos.y);
		Text2D.Draw2DText(Menu[M_UNIT].Strings[4].Text);

		glRasterPos2f(Menu[M_UNIT].CurrentPlacement.left + Menu[M_UNIT].Strings[5].Pos.x,
					  Menu[M_UNIT].CurrentPlacement.bottom + Menu[M_UNIT].Strings[5].Pos.y);
		Text2D.Draw2DText(Menu[M_UNIT].Strings[5].Text,Unit->Speed);

		glRasterPos2f(Menu[M_UNIT].CurrentPlacement.left + Menu[M_UNIT].Strings[6].Pos.x,
					  Menu[M_UNIT].CurrentPlacement.bottom + Menu[M_UNIT].Strings[6].Pos.y);
		Text2D.Draw2DText(Menu[M_UNIT].Strings[6].Text);

		glRasterPos2f(Menu[M_UNIT].CurrentPlacement.left + Menu[M_UNIT].Strings[7].Pos.x,
					  Menu[M_UNIT].CurrentPlacement.bottom + Menu[M_UNIT].Strings[7].Pos.y);
		Text2D.Draw2DText(Menu[M_UNIT].Strings[7].Text,Unit->Sight);

		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);

	}

	Panels[P_UNIT].Draw();


	if((World->Pointer.Object)&&(Focus == FOCUS_WORLD))
	    ConMenu[0].Visible = true;


//	ConMenu[0].Draw();



	glPopMatrix();

	if(Console.NewCommand)
		ProcessConsole(Console.Command);

}


void TInterface::OnKey(bool Keys[256])
{
	for(int i = 0; i < 256; i++)
		if(Keys[i]) fChangeFocus = true;
}

void TInterface::OnLeftButtonClick(int X, int Y)
{
	if(Buttons[B_EXIT].Visible&&PointInRect(X,Y,addRect(Buttons[B_EXIT].Placement,*Buttons[B_EXIT].ParentPlacement)))
	{
		Buttons[B_EXIT].Pressed = true;
//		fStartGame = false;
		Focus = FOCUS_GAME_MENU;
		fFocusChanged = true;

	}

	if(Buttons[B_MAINMENU].Visible&&PointInRect(X,Y,addRect(Buttons[B_MAINMENU].Placement,*Buttons[B_MAINMENU].ParentPlacement)))
	{
		Buttons[B_MAINMENU].Pressed = true;
		if(Focus == FOCUS_WORLD)
			Focus = FOCUS_MENU;
		else
			if(Focus == FOCUS_MENU)
				Focus = FOCUS_WORLD;

		fFocusChanged = true;

	}

	if(Buttons[B_COMMAND_GO].Visible&&PointInRect(X,Y,addRect(Buttons[B_COMMAND_GO].Placement,*Buttons[B_COMMAND_GO].ParentPlacement)))
	{
		World->Pointer.bGetPoint = true;
		Buttons[B_COMMAND_GO].Pressed = true;

	}

	if(PointInRect(X,Y,MiniMap.Boundaries))
	{
		World->Camera.X = (X - MiniMap.Boundaries.left)/MINIMAP_SCALE;
		World->Camera.Y = (Y - MiniMap.Boundaries.bottom)/MINIMAP_SCALE;
	}

	if(Buttons[B_COMMAND_STOP].Visible&&PointInRect(X,Y,addRect(Buttons[B_COMMAND_GO].Placement,*Buttons[B_COMMAND_GO].ParentPlacement)))
	{
		Society->ChangeMoveModeForSelected();
		Buttons[B_COMMAND_STOP].Pressed = true;

	}

	if(Buttons[B_COMMAND_ATTACK].Visible&&PointInRect(X,Y,addRect(Buttons[B_COMMAND_GO].Placement,*Buttons[B_COMMAND_GO].ParentPlacement)))
	{
		Society->ChangeAttackModeForSelected();
		Buttons[B_COMMAND_ATTACK].Pressed = true;

	}

	if(Buttons[B_COMMAND_CHASE].Visible&&PointInRect(X,Y,addRect(Buttons[B_COMMAND_GO].Placement,*Buttons[B_COMMAND_GO].ParentPlacement)))
	{
		Society->ChangeChaseModeForSelected();
		Buttons[B_COMMAND_CHASE].Pressed = true;

	}


	if(Buttons[B_GAME_START].Visible&&PointInRect(X,Y,addRect(Buttons[B_GAME_START].Placement,*Buttons[B_GAME_START].ParentPlacement)))
	{
		Buttons[B_GAME_START].Pressed = true;
		fStartGame = true;
		Focus = FOCUS_WORLD;
		fFocusChanged = true;
	}

	if(Buttons[B_GAME_EXIT].Visible&&PointInRect(X,Y,addRect(Buttons[B_GAME_EXIT].Placement,*Buttons[B_GAME_EXIT].ParentPlacement)))
	{
		Buttons[B_GAME_EXIT].Pressed = true;
		bExit = true;
	}

}



void TInterface::OnMouseMove(int Button, int X, int Y)
{
	for (int i = 0; i < MAX_BUTTONS; i++)
		if(Buttons[i].Visible&&PointInRect(X,Y,addRect(Buttons[i].Placement,*Buttons[i].ParentPlacement)))
		{
			if(Buttons[i].HintEnabled)
			{
				Buttons[i].IsHint = true;
				Buttons[i].HintTime = HINT_TIME;
			}


		}


}

void TInterface::ProcessConsole(char Command[255])
{

	if(strstr(Command, "LAND"))
			World->fLandscape = !World->fLandscape;

	if(strstr(Command, "OBJECTS"))
			World->fObjects = !World->fObjects;

	if(strstr(Command, "FILL"))
	{
		glPolygonMode( GL_BACK, GL_FILL );
		glPolygonMode( GL_FRONT, GL_FILL);
	}

	if(strstr(Command, "LINE"))
	{
		glPolygonMode( GL_BACK, GL_LINE );
		glPolygonMode( GL_FRONT, GL_LINE);
	}

	if(strstr(Command, "DEBUG"))
	{
		fViewDebug = !fViewDebug;
	}

	if(strstr(Command, "RESTART"))
	{
		Society->GSocietyInit(false);
	}

	Console.NewCommand = false;
}