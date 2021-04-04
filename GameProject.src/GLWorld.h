#include <windows.h>

#include <gl\gl.h>			
#include <gl\glu.h>			
#include <gl\glaux.h>

#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdarg.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>

#include "StdAfx.h"

#include "MainUnit.h"
#include "load3ds.h" 
#include "Menu.h"
#include "Buttons.h"
#include "resource.h"
#include "GLConstants.h"
#include "GLext.h"

#define GET_OBJECT 1;
#define GET_LAND 2;

typedef enum {COMMAND_GO, COMMAND_ATTACK, COMMAND_STOP} COMMAND_TYPE;

typedef struct T_RGBA
{
	char R, G, B, A;
};				


class TError
{
public:
	HWND hWnd;
	HGLRC hRC;
	HDC hDC;

//	char* String;
	bool Exit;
	void OnError(char* String);

};


class TCursor
{
public:
	int ScreenX;
	int ScreenY;

	int OldScreenX;
	int OldScreenY;
	T3DPoint Pos;
	
	bool Enabled;
	bool GetObject;
	bool GetLand;
	bool TypeChanged;
	bool IsRClick;
	bool IsLClick;
	bool IsLDbClick;

	int Type;
	int Focus;

	void Init();

};




//--------------------------------Структуры и классы, относящиеся к GLWorld-------------

struct TPart
{
	char* TexFileName;
	bool  ColorEnabled;
	int TexId;
};


struct TModelFrame
{
	char* ModelFileName;
	int   num_parts;
	TPart Parts[MAX_PARTS+1];
};

class TModel
{
public:
	TModelFrame Frames[MAX_ACTIONS+1][MAX_MODEL_FRAMES+1];

	T3DPoint  StartPos;
	T3DAngle  StartAngle;
	T3DPoint  Scale; 
	int       ModelId;

};


class TCamera
{
public:
	float X;
	float Y;
	float Z;
	float XAngle,YAngle,ZAngle;
	float Radius;

	float Height;

	void Init();
	void SetPos();

	bool bChangePos;

	T4Region Region;

	bool		bAttached;
	TGObject*	AttObject;

};

class TLight
{
public:	
    float Position[4];
    struct Color
	{
		float Red;
		float Green;
		float Blue;
		float Intensivity;
	};
	bool Enabled;

};

class TPointer
{
public:

	TCursor    *Cursor;
	T3DPoint Pos;
	T3DPoint StartRect;
	T3DPoint EndRect;
	bool bRect;
	bool bPoint;
	bool bNewSelection;
	bool bGetPoint;

	void DrawPoint();
	void DrawRect();
	void GetObject();

	int Object;
	T3DPoint GetPoint(int X, int Y);
};


class TCommand
{
public:
	COMMAND_TYPE  Type;
	POINT Point;
	T3DPoint MapPoint;
	bool Is;
};


class TGLLand
{
	struct  TNormalBuf
	{
		//T3DPoint Normals[MAP_HEIGHT][3];
		int Right;
		int Center;
		int Left;
	} NormalBuf;

	T3DPoint  TempNormals[3][3];

	void DrawQuad(int X, int Y);

	void DrawBorder();						  // Рисуем ограду	


public:

	TGSociety* Society;

	TCamera* Camera;

	int** VisibleMap;

	GLuint Textures[MAX_LANDTEX];
	GLuint Masks[10]; 

	void LoadLand();				 // Считывание карты высот и текстур 	

	void DrawLand();                          // Отрисовка ландшафта
	void GetNormals(int Column, int MinY, int MaxY);

};


class TGLWorld 
{
	int        num_models;

	char   LandTexData[MAX_LANDTEX][MAX_LANDTEX_SIZE][MAX_LANDTEX_SIZE][3]; 

	int AnalisInterval;
    int SetAngelsInterval;
    int DoAIInterval;
    int Frame;

	bool flag;
 
	HGLRC hRC;
	HDC hDC;
	RECT ViewRegion;
	RECT Boundaries;

public:

	int        ObjectList[MAX_MODELS+1][MAX_ACTIONS+1][MAX_MODEL_FRAMES+1];


	GLuint ModelTextures[MAX_MODELTEX+1];
	GLuint MiscTextures[MAX_MISCTEX+3];
	GLuint FireTextures[1];
	GLuint LandTexture;


	bool fLandscape;
	bool fObjects;
	bool fLandSmooth;

	bool IsSelection;

	TGSociety  *Society;
	TError     *Error;

	TGLLand	   Land;
	TCommand   Command;
	TPointer   Pointer;
	TCamera    Camera;
	TLight     Lights[MAX_LIGHTS+1];
	TModel     Models[MAX_MODELS+1];

	void LoadObjects();              // Считывание моделей файла
	void LoadModelsInfo();           // Считывание инвормации о модели* 
	void InitGL();                   // Инициализация OpenGL
	void LoadTex();                  // Считывание текстур из .tex файлов
	void Init(HGLRC hRC, HDC hDC);   // Начало инициализации   

	void Refresh();                           // Перерисовка всего
	void Prepare();

	void DrawObject(int Team,int Index);      // Отрисовка объектов
	void DrawAttack(int Team, int Index);     // Отрисовка выстрелов 
	void DrawSelection(int Team, int Index);  // Отрисовка выбранного объекта
	void DrawSelected(int Team, int Index, int Angle); // Рисуем выделенного юнита*

	void OnCommand();

	void SetNewCameraPos(float dX, float dY, float dZ);

	void GetViewRegion();                     // Вычисление видимой части карты* 

};


















//---------------------Классы и струкктуры, относящиеся к интерфейсу----------------------------


struct TString
{
 POINT    Pos;
 char*    Text;
};


class TGraphicObject
{
public:
	float Alpha;
	int   Textures[MAX_BTN_TEX];
	RECT Placement;
	RECT* ParentPlacement;
	RECT CurrentPlacement;
	void Draw();

	int HideTime;
	int MaxHideTime;
	int HideSpeedX;
	int HideSpeedY;
	bool bHide;
	bool bShow;
	bool Visible;
	bool IsButton;
	bool Pressed;
	int PressTime;
	int MaxPressTime;
	int Frame;
};

class T2DText
{
 GLYPHMETRICSFLOAT gmf[256];
 HFONT  font;

public:

    GLuint base;
	bool Init(HGLRC hRC, HDC hDC);
	void Draw2DText(char* String, ...);
};


class TConsole
{
public:

	bool Visible;
	bool bShow;
	int  Timer; 
	bool NewCommand;
	char Command[255];
	T2DText* Text;
	char Lines[MAX_LINES][255];
	int num_lines;
	int num_visible;

	void Draw();
	void WriteChar(char Sumbol);
};



class T3DText
{
 GLYPHMETRICSFLOAT gmf[256];
 HFONT  font;

public:

    GLuint base;
	bool Init(HGLRC hRC, HDC hDC);
	void Draw3DText(char* String);
};


class TMenuButton : public TGraphicObject
{
public:
	bool Enabled;
	int Id;
	void Init();
	char* HintString;
	bool IsHint;
	bool HintEnabled;
	int HintTime;
	int ParentMenuId;

	int Tex1Id;
	int Tex2Id;

};

class TMenu : public TGraphicObject
{
public:
	void Init();
	TString Strings[MAX_STRINGS];
	TMenuButton* Buttons[MAX_BUTTONS];
	bool Enabled;
	int TexId;

};


class TContextMenu : public TGraphicObject
{
public:

};


class TPanel : public TGraphicObject
{
public:

};

class TMiniMap
{
public:
	TGLWorld* World;
	RECT  Boundaries;

	GLuint Map;
	float Scale;
	T_RGBA** PixelMap;

	void Init();

//	void Init(TMap Map[MAP_WIDTH][MAP_HEIGHT]);
//	void Refresh();
	void Draw();
};



class TInterface
{
public:

	int       Focus;

	bool      fStartGame;
	bool      fFocusChanged;

	TGLWorld  *World;
	TCursor   *Cursor;  
	TGSociety *Society;

	TError     *Error;

	int        Frame;
	float      CameraZ; 
	float      FPS;
	bool       fChangeFocus;
	bool       bExit;
	GLuint     Images[MAX_IMAGES+1];


	T2DText    Text2D;
	TMenu           Menu[MAX_MENU];
	TMenuButton     Buttons[MAX_BUTTONS];
	TPanel          Panels[MAX_PANELS];
	TContextMenu    ConMenu[MAX_CONMENU];

	TMiniMap   MiniMap;
	TConsole   Console;

	void Init(HGLRC hRC_, HDC hDC_);
	void LoadImages();
	void LoadInterface();
	void Prepare();
	void Refresh();

	void SetMenuVisible();

	TGObject* GetFirstSelected();

	void OnKey(bool Keys[256]);
	void OnLeftButtonClick(int X, int Y);
	void OnMouseMove(int Button, int X, int Y);

	void ProcessConsole(char Command[255]);
};

//--------------------------------Класс TGame!-------------------------------

class TGame
{
public:

	int        CurrentResolutionX;
	int        CurrentResolutionY;


	HWND       hWnd;
	HGLRC      hRC;
	HDC        hDC;


	TError     Error;          // Сообщеня об ошибках
	TCursor    Cursor;         // Курсор 
	TGSociety  Society;        // Искуственный интеллект
	TInterface Interface;      // Класс, отвечающий за взаимодействие с пользователем
	TGLWorld   World;          // Собственно игра


	
	int AnalisInterval;
    int SetAngelsInterval;
    int DoAIInterval;
    int Frame;

	int Focus;
	void InitGL();
	void Init(HGLRC hRC_, HDC hDC_ ,HWND hWnd_);
	void Refresh();

	void OnRightButtonClick(int X, int Y);        
	void OnLeftButtonClick(int X, int Y);   
	void OnLeftButtonUp(int X, int Y);
	void OnWheel(int Rotation, int X, int Y);
	void OnKey(bool Keys[256]);
	void OnMouseMove(int Button, int X, int Y);
	void OnLeftDbClick(int X, int Y);

	void ChangeResolution(int ResX, int ResY);

	void Exit();
};

















//-----------------------------------Полезные и бесполезные функции-------------------------------


inline bool PointInRect(int X, int Y, RECT Rect)
{
	if((X>=Rect.left)&(X<=Rect.right)&
	   (Y>=Rect.bottom)&(Y<=Rect.top))
	    return true;
	else
		return 	false;

}

inline int max4i(int a1, int a2, int a3, int a4)
{
	if(a1<a2) a1 = a2;
	if(a1<a3) a1 = a3;
	if(a1<a4) a1 = a4;
	
	return a1;
};


inline float StdAngle(float InputAngle)   // как известно общепринято что бы все углы были > 0 и меньше 2Пи...
                                  // эта функция делает любой угол именно таким
{
     double n;
     float InAngle = InputAngle,Circle = 360;
     if (InAngle > 360)
        {
        modf((InAngle/Circle),&n);

            InAngle -= 360 * n;
        }


     if (InAngle < (-360) )
        {
        modf(InAngle/(-Circle),&n);
            InAngle -= (-360) * float(n);


        }
     if ((InAngle < 0)&&(InAngle>(-360)))
        InAngle+=360;

     if (InAngle > 180) InAngle = InAngle - 360;

     InputAngle = InAngle;

     return InputAngle;

}

inline int Random(int Max)
{
	return int(rand()*Max/RAND_MAX);
}


inline bool PointInInterval(float P, float P1, float P2)
{
	if(((P >= P1) && (P <= P2)) || ((P >= P2) && (P <= P1)))
		return true;
	else 
		return false;
}


inline RECT addRect(RECT rBtn, RECT rMenu)
{
	RECT Result;
	Result.left = rBtn.left + rMenu.left;
	Result.right = rBtn.right + rMenu.left;
	Result.bottom = rBtn.bottom + rMenu.bottom;
	Result.top = rBtn.top + rMenu.bottom;

	return Result;
}


inline bool BigPointInInterval(float P, float Width, float P1, float P2)
{
	if(PointInInterval(P - Width,P1,P2)||PointInInterval(P + Width,P1,P2))
		return true;
	else
		return false;
}

inline bool BigPointInRect(int X, int Y, int Width, RECT Rect)
{
	if(PointInRect(X-Width,Y-Width, Rect)||
	   PointInRect(X-Width,Y+Width, Rect)||
	   PointInRect(X+Width,Y+Width, Rect)||
	   PointInRect(X+Width,Y-Width, Rect))
		return true;
	else
		return false;
}

inline float GetMax(float x1, float x2)
{
	if(x1 > x2)
		return x1;
	else
		return x2;
}

inline float GetMin(float x1, float x2)
{
	if(x1 < x2)
		return x1;
	else
		return x2;
}

inline float sqr(float arg)
{
	return arg*arg;
}

