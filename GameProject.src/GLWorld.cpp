#include "StdAfx.h"

#include "GLWorld.h"

int  frame;

PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	= NULL;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		= NULL;

/*
char* StrToPChar(string str)
{
	int len = str.length();
	char* res = new char[len];
	int i;
	for(i = 0; i < len - 1; i++)
		res[i] = str[i];

	res[len] = '/0';

	return res;


}
*/

/*
char* CreateMipMap(int Scale, char Image[MAX_LANDTEX_SIZE][MAX_LANDTEX_SIZE][3])
{
	char Result[MAX_LANDTEX_SIZE/2][MAX_LANDTEX_SIZE/2][3];

	for(int x = 0; x < MAX_LANDTEX_SIZE/Scale; x++)
		for(int y = 0; y < MAX_LANDTEX_SIZE/Scale; y++)
		{
			Result[x][y][0] = (Image[x*Scale][y*Scale][0]+Image[x*Scale+1][y*Scale+1][0]+Image[x*Scale+1][y*Scale][0]+Image[x*Scale][y*Scale+1][0])/4;
			Result[x][y][1] = (Image[x*Scale][y*Scale][1]+Image[x*Scale+1][y*Scale+1][1]+Image[x*Scale+1][y*Scale][1]+Image[x*Scale][y*Scale+1][1])/4;
			Result[x][y][2] = (Image[x*Scale][y*Scale][2]+Image[x*Scale+1][y*Scale+1][2]+Image[x*Scale+1][y*Scale][2]+Image[x*Scale][y*Scale+1][2])/4;
		}

	return &Result[0][0][0];
}

*/

inline void getnormf (float a[3],float b[3],float c[3],float *n)
{
	n[0]=(b[2]-a[2])*(c[1]-a[1])-(b[1]-a[1])*(c[2]-a[2]);
	n[1]=(b[0]-a[0])*(c[2]-a[2])-(c[0]-a[0])*(b[2]-a[2]);
	n[2]=(c[0]-a[0])*(b[1]-a[1])-(b[0]-a[0])*(c[1]-a[1]);

	//n[0]=(b[1]-a[1])*(c[2]-a[2])-(b[2]-a[2])*(c[1]-a[1]);
	//n[1]=(c[0]-a[0])*(b[2]-a[2])-(b[0]-a[0])*(c[2]-a[2]);
	//n[2]=(b[0]-a[0])*(c[1]-a[1])-(c[0]-a[0])*(b[1]-a[1]);
}

T3DPoint TPointer::GetPoint(int X, int Y)
{
  T3DPoint p1,p2, Res;
  GLint    viewport[4];    
  GLdouble projection[16]; 
  GLdouble modelview[16]; 
  GLdouble vx,vy,vz;     
  GLdouble wx,wy,wz;  
  float k;

  glGetIntegerv(GL_VIEWPORT,viewport);         
  glGetDoublev(GL_PROJECTION_MATRIX,projection); 
  glGetDoublev(GL_MODELVIEW_MATRIX,modelview);  
  
  vx = X;
  vy = ResolutionY - Y;
  
  vz = -1;
  gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
  p1.X=wx;
  p1.Y=wy;
  p1.Z=wz;
  
  vz = 1;
  gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
  p2.X=wx;
  p2.Y=wy;
  p2.Z=wz;

  glReadPixels(vx,vy,1,1,GL_DEPTH_COMPONENT, GL_FLOAT, &Res.Z);

  Res.Z = 0;
  k=(Res.Z-p1.Z)/(p1.Z-p2.Z);
  Res.X=p1.X+(p1.X-p2.X)*k;
  Res.Y=p1.Y+(p1.Y-p2.Y)*k;

  return Res;
}

void TPointer::DrawRect()
{
	EndRect = GetPoint(Cursor->ScreenX,Cursor->ScreenY);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(0,1,0);
	glDisable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);

//		glTexCoord2f(0,0);
		glVertex3f(EndRect.X,EndRect.Y,0);

//		glTexCoord2f(1,0);
		glVertex3f(StartRect.X,EndRect.Y,0);

//		glTexCoord2f(1,1);
		glVertex3f(StartRect.X,StartRect.Y,0);

//		glTexCoord2f(0,1);
		glVertex3f(EndRect.X,StartRect.Y,0);

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}











//________________________________________________________________________________________________\\
//------------------------------------------------------------------------------------------------\\
//                        //                                 \\                                   \\
//------------------------//  Класс TLight - источник света  \\-----------------------------------\\  
//                        //                                 \\                                   \\ 
//------------------------------------------------------------------------------------------------\\
//________________________________________________________________________________________________\\


//*****************************- Иниациализация -*************************************************
//
//************************************************************************************************









//________________________________________________________________________________________________\\
//------------------------------------------------------------------------------------------------\\
//                        //                                 \\                                   \\
//------------------------//          Класс TCamera          \\-----------------------------------\\  
//                        //                                 \\                                   \\ 
//------------------------------------------------------------------------------------------------\\
//________________________________________________________________________________________________\\



//*****************************- Иниациализация -*************************************************
//
//************************************************************************************************

void TCamera::Init()
{
	X=100;
	Y=50;
	Z=20;
	XAngle=-60;
	YAngle=0;
	ZAngle=0;
	Height = 10;
}


//*****************************- Установка позиции -*************************************************
//
//************************************************************************************************


//-------------------------------------Текуще положение---------------------------------------------

void TCamera::SetPos()
{


	glRotated(XAngle,1,0,0); 
	glRotated(YAngle,0,1,0);
	glRotated(ZAngle,0,0,1);
	glTranslatef(-X,-Y,-Z);

}





//________________________________________________________________________________________________\\
//------------------------------------------------------------------------------------------------\\
//************************************************************************************************\\
//                        //                                 \\                                   \\
//------------------------//   Класс TGLWorld - мир OpenGL!  \\-----------------------------------\\  
//                        //                                 \\                                   \\ 
//************************************************************************************************\\
//------------------------------------------------------------------------------------------------\\
//________________________________________________________________________________________________\\





//*****************************- Иниациализация -*************************************************
//
//************************************************************************************************

//-------------------------------Общая инициализация----------------------------------------------
void TGLWorld::Init(HGLRC hRC_,	HDC hDC_)
{

	hRC = hRC_;
	hDC = hDC_;
	AnalisInterval=0;
    SetAngelsInterval=0;
    DoAIInterval=0;
    Frame=0;
	
	fObjects = true;
	fLandscape= true;

	Camera.Init();

	LoadTex();
    LoadModelsInfo();
    LoadObjects();

	Land.Camera = &Camera;
	Land.Society = Society;
//	Land.LoadLand();
//	Society->InitMapPassably(); 

//  BuildLandTextures();


	glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");

	Land.VisibleMap = new int*[MAP_WIDTH+1];

	for(int i = 0; i < MAP_WIDTH+1; i++)
		Land.VisibleMap[i] = new int[2];

};	






//-------------------------------Загрузка текстур---------------------------------------------
void TGLWorld::LoadTex()                                                                        
{

 FILE* File;

 int X,Y;
 char* data = new char[1024*1024*3];

 File = fopen("Textures/Land.tex","r+b");
 if(!File) Error->OnError(" Не могу загрузить Land.tex");
 else
 {

  fread(&X,sizeof(int),1,File);
  fread(&Y,sizeof(int),1,File);
  fread(data,sizeof(char),X*Y*3,File);

  glGenTextures(1, &LandTexture);                                        
  glBindTexture(GL_TEXTURE_2D, LandTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, X, Y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, X, Y, GL_RGB, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

 }

 fclose(File);


 File = fopen("Textures/Model.tex","r+b");
 if(!File) Error->OnError(" Не могу загрузить Model.tex");
 else
 for (int i = 0; i < MAX_MODELTEX; i++)
 {

  fread(&X,sizeof(int),1,File);
  fread(&Y,sizeof(int),1,File);
  fread(data,sizeof(char),X*Y*3,File);

  glGenTextures(1, &ModelTextures[i]);                                        
  glBindTexture(GL_TEXTURE_2D, ModelTextures[i]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, X, Y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, X, Y, GL_RGB, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

 }

 fclose(File);


 File = fopen("Textures/Misc.tex","r+b");
 if(!File) Error->OnError(" Не могу загрузить Model.tex");
 else
 for (int i = 0; i < MAX_MISCTEX; i++)
 {

  fread(&X,sizeof(int),1,File);
  fread(&Y,sizeof(int),1,File);
  fread(data,sizeof(char),X*Y*3,File);

  glGenTextures(1, &MiscTextures[i]);                                        
  glBindTexture(GL_TEXTURE_2D, MiscTextures[i]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, X, Y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, X, Y, GL_RGB, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

 }

 fclose(File);


 File = fopen("Textures/Floor.tex","r+b");
 if(!File) Error->OnError(" Не могу загрузить Floor.tex");
 else
 for (int i = MAX_MISCTEX; i < MAX_MISCTEX+1; i++)
 {

  fread(&X,sizeof(int),1,File);
  fread(&Y,sizeof(int),1,File);
  fread(data,sizeof(char),X*Y*3,File);

  glGenTextures(1, &MiscTextures[i]);                                        
  glBindTexture(GL_TEXTURE_2D, MiscTextures[i]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, X, Y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, X, Y, GL_RGB, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

 }


 fclose(File);

 File = fopen("Textures/Fire.tex","r+b");
 if(!File) Error->OnError(" Не могу загрузить Fire.tex");
 else
 for (int i = 0; i < 1; i++)
 {

  fread(&X,sizeof(int),1,File);
  fread(&Y,sizeof(int),1,File);
  fread(data,sizeof(char),X*Y*3,File);

  glGenTextures(1, &FireTextures[i]);                                        
  glBindTexture(GL_TEXTURE_2D, FireTextures[i]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, X, Y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, X, Y, GL_RGB, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

 }

/* File = fopen("Images/Mask.raw","r+b");
 if(!File) Error->OnError(" Не могу загрузить Mask.raw");

  fread(data,sizeof(char),64*64,File);

  glGenTextures(1, &Land.Masks[0]);                                        
  glBindTexture(GL_TEXTURE_2D, Land.Masks[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 64, 64, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

*/
 File = fopen("Textures/Land.tex","r+b");
 if(!File) Error->OnError(" Не могу загрузить Land.tex");
 else
 for(int i = 0; i < MAX_LANDTEX; i++)
 {
  fread(&X,sizeof(int),1,File);
  fread(&Y,sizeof(int),1,File);
  fread(data,sizeof(char),X*Y*3,File);

  glGenTextures(1, &Land.Textures[i]);                                        
  glBindTexture(GL_TEXTURE_2D, Land.Textures[i]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, X, Y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


 }

 fclose(File);


 delete [] data;
};


//-------------------------------Загрузка объектов---------------------------------------------
void TGLWorld::LoadObjects()
{

 mesh_t* Mesh;
 int num_obj = 0;

/* CLoad3DS a;

 t3DModel *Model;

 a.Import3DS(Model, "Models/1.3ds");

	for(int i = 0; i < Model->numOfObjects; i++)
	{
		// Free the faces, normals, vertices, and texture coordinates.
		delete [] Model->pObject[i].pFaces;
		delete [] Model->pObject[i].pNormals;
		delete [] Model->pObject[i].pVerts;
		delete [] Model->pObject[i].pTexVerts;
	}
*/

 int num_vertex;

 for (int n = 0; n < MAX_MODELS; n++)
  for (int a = 0; a < MAX_ACTIONS; a++)
   for (int f = 0; f < Society->Standart[n].MaxModelFrames; f++)
   {

   num_obj++;    
   ObjectList[n][a][f] = num_obj;
//Генерация имени файла! глючит:(.. но работает:)..  


		char buf[FILE_NAME_LENGTH], *filename, *folder, fol[FILE_NAME_LENGTH];  

		sprintf(buf, "%i", int(a*MAX_FRAMES + f));
		sprintf(fol, "%s", "Models/");
		strcat(fol,buf); 
        strcat(fol,Society->Standart[n].FileName);
		Models[n].Frames[a][f].ModelFileName = strcat(fol, ".3ds");
   
   
//
   Mesh = Load3DS(Models[n].Frames[a][f].ModelFileName,&num_vertex);



   glNewList(ObjectList[n][a][f], GL_COMPILE);
    glPushMatrix();

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
//	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);

  	glTranslated(Models[n].StartPos.X*Models[n].Scale.X,
		         Models[n].StartPos.Y*Models[n].Scale.Y,
				 Models[n].StartPos.Z*Models[n].Scale.Z);

	glRotatef(Models[n].StartAngle.Angle,
		      Models[n].StartAngle.X,
			  Models[n].StartAngle.Y,
			  Models[n].StartAngle.Z);

	glScalef(Models[n].Scale.X,Models[n].Scale.Y,Models[n].Scale.Z);

   if (Models[n].Frames[a][f].ModelFileName == "0.3ds")
      bool Stop = true;

    for(int k = 0; k < Mesh->num_trimesh; k++)
	 {
//	    glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT);

//	    if(!Models[n].Frames[a][f].Parts[k].ColorEnabled) glColor3f(1,1,1);

//	    glBindTexture(GL_TEXTURE_2D, ModelTextures[Models[n].Frames[a][f].Parts[k].TexId]);

		for (int i = 0; i < Mesh->trimesh[k].num_uvmap; i++)
		{
			Mesh->trimesh[k].uvmap[i].u = Mesh->trimesh[k].vertex[i].x*0.001;
			Mesh->trimesh[k].uvmap[i].v = Mesh->trimesh[k].vertex[i].y*0.001;
		}

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glVertexPointer(3,GL_FLOAT,0,Mesh->trimesh[k].vertex);
		glNormalPointer(GL_FLOAT,0,Mesh->trimesh[k].normal);
		glTexCoordPointer(2,GL_FLOAT,0,Mesh->trimesh[k].uvmap);

		glDrawArrays(GL_TRIANGLES,0, Mesh->trimesh[k].num_vertex);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

//		glPopAttrib();

	}
	free(Mesh);
   glPopMatrix();
   glEndList();
  }

}; 

/*void TGLWorld::BuildLandTextures()
{

 int x,y;
 int ImgX = 128;
 int ImgY = 128;
 char image[32][32][3];

 for (int i = 0; i < MAP_WIDTH/7; i++)
	for (int j = 0; j < MAP_HEIGHT/7; j++)
	{

		for (int s = 0; s < 32/MAX_LANDTEX_SIZE; s++)
			for (int t = 0; t < 32/MAX_LANDTEX_SIZE; t++)
			{
				for (y = 0; y < 16; y++)
				{
					for (x = 0; x < 16; x++)
					{
						image[s*16+x][t*16+y][0] = LandTexData[Society->MAP[i*7+t][j*7+s].Tex][x][y][0];
						image[s*16+x][t*16+y][1] = LandTexData[Society->MAP[i*7+t][j*7+s].Tex][x][y][1];
						image[s*16+x][t*16+y][2] = LandTexData[Society->MAP[i*7+t][j*7+s].Tex][x][y][2];

						if(Society->MAP[i*7+t][j*7+s].Tex != Society->MAP[i*7+t-1][j*7+s].Tex)
						{
							image[s*16+x][t*16][0] = 0;
							image[s*16+x][t*16][1] = 0; 
							image[s*16+x][t*16][2] = 0; 
						}


						if(Society->MAP[i*7+t][j*7+s].Tex != Society->MAP[i*7+t+1][j*7+s].Tex)
						{
							image[s*16+x][t*16+15][0] = 0;
							image[s*16+x][t*16+15][1] = 0; 
							image[s*16+x][t*16+15][2] = 0; 
						}

					}

					if(Society->MAP[i*7+t][j*7+s].Tex != Society->MAP[i*7+t][j*7+s-1].Tex)
					{
						image[s*16][t*16+y][0] = 0;
						image[s*16][t*16+y][1] = 0; 
						image[s*16][t*16+y][2] = 0; 
					}

					if(Society->MAP[i*7+t][j*7+s].Tex != Society->MAP[i*7+t][j*7+s+1].Tex)
					{
						image[s*16+15][t*16+y][0] = 0;
						image[s*16+15][t*16+y][1] = 0; 
						image[s*16+15][t*16+y][2] = 0; 
					}

				}
			}

		glGenTextures(1, &LandTex[i][j]);                                        
		glBindTexture(GL_TEXTURE_2D, LandTex[i][j]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, &image[0][0][0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	}

}
*/

void TGLWorld::LoadModelsInfo()
{

for (int n = 0; n < MAX_MODELS; n++)
 {
	Models[n].Scale.X = 0.032;
	Models[n].Scale.Y = 0.032;
	Models[n].Scale.Z = 0.0315;

	Models[n].StartPos.X = 0;
	Models[n].StartPos.Y = 0;
	Models[n].StartPos.Z = 0;

	Models[n].StartAngle.Angle = 0;
	Models[n].StartAngle.X = 0;
	Models[n].StartAngle.Y = 0;
	Models[n].StartAngle.Z = 1;
	
	for (int a = 0; a < MAX_ACTIONS; a++)
	  for (int f = 0; f < MAX_FRAMES; f++)
	  {
        char num = n+49;
//		Models[n].Frames[a][f].ModelFileName = "1.3ds";//strcat(&num,".3ds");
		ObjectList[n][a][f] = n;

		for(int p = 0; p < MAX_PARTS; p++)
		{
			Models[n].Frames[a][f].Parts[p].TexId = 5;
			Models[n].Frames[a][f].Parts[p].ColorEnabled = true;
		}

	  }

 }
/*
Models[0].Frames[0][MAX_FRAMES].ModelFileName = "0.3ds";


Models[0].Frames[0][1].ModelFileName = "2.3ds";
Models[0].Frames[0][2].ModelFileName = "3.3ds";
Models[0].Frames[0][3].ModelFileName = "4.3ds";
Models[0].Frames[0][4].ModelFileName = "5.3ds";
Models[0].Frames[0][5].ModelFileName = "6.3ds";
Models[0].Frames[0][6].ModelFileName = "7.3ds";
Models[0].Frames[0][7].ModelFileName = "8.3ds";
Models[0].Frames[0][9].ModelFileName = "0.3ds";

Models[0].Frames[1][1].ModelFileName = "12.3ds";
Models[0].Frames[1][2].ModelFileName = "13.3ds";
Models[0].Frames[1][3].ModelFileName = "14.3ds";
Models[0].Frames[1][4].ModelFileName = "15.3ds";
Models[0].Frames[1][5].ModelFileName = "16.3ds";
Models[0].Frames[1][6].ModelFileName = "17.3ds";
Models[0].Frames[1][7].ModelFileName = "18.3ds";
Models[0].Frames[1][9].ModelFileName = "10.3ds";

*/

/*
	//"Правильная" загрузка моделей

for (int m = 0; m < MAX_MODELS; m++)
{

	Models[m].Scale.X = ...;
	Models[m].Scale.Y = ...;
	Models[m].Scale.Z = ...;

	Models[m].StartPos.X = ...;
	Models[m].StartPos.Y = ...;
	Models[m].StartPos.Z = ...;

	Models[m].StartAngle.Angle = ...;
	Models[m].StartAngle.X = ...;
	Models[m].StartAngle.Y = ...;
	Models[m].StartAngle.Z = ...;

	for (int a = 0; a < MAX_ACTIONS; a++)
		for (int f = 0; f < MAX_FRAMES; f++)
		{
			Models[m].Frames[a][f].ModelFileName = ...;
			Models[m].Frames[a][f].num_parts = ...;

			for (int p = 0; p < Models[m].Frames[a][f].num_parts; p++)
			{
				Models[m].Frames[a][f].Parts[p].TexId = ...;
				Models[m].Frames[a][f].Parts[p].ColorEnabled = ...;
			}
		}
}
*/
}


void TGLLand::LoadLand()
{
 FILE* File;
 char Z;

 File = fopen("Images/Land.raw","r+b");
 for (int i = 0; i < MAP_HEIGHT; i++)
	  for (int j = 0; j < MAP_WIDTH; j++)
	  {
		  fread(&Z,sizeof(char),1,File);

		  Society->MAP[j][i].Z = Z*0.6;

	  }
 fclose(File);

 File = fopen("Images/Land.raw","r+b");
 for (int i = 0; i < MAP_HEIGHT; i++)
	  for (int j = 0; j < MAP_WIDTH; j++)
	  {
		  fread(&Z,sizeof(char),1,File);

		  if(Z <=  2) Society->MAP[j][i].Tex = 0;
		  if(Z > 2) Society->MAP[j][i].Tex = 2;
		  if(Z >= 10) Society->MAP[j][i].Tex = 5;

	  }

 fclose(File);

}



//*****************************- Отрисовка ландшафта, объектов -*****************************************
//
//************************************************************************************************


//-------------------------------Обновление карты, объектов; их отрисовка ---------------------------------------------

void TGLWorld::GetViewRegion()
{

	T3DPoint Point = Pointer.GetPoint(1,1);
	Camera.Region.Vertex[0] = Point.Get2DPoint();

	Point = Pointer.GetPoint(799,1);
	Camera.Region.Vertex[1] = Point.Get2DPoint();

	Point = Pointer.GetPoint(799,599);
	Camera.Region.Vertex[2] = Point.Get2DPoint();

	Point = Pointer.GetPoint(1,599);
	Camera.Region.Vertex[3] = Point.Get2DPoint();

		
	int k = 0;
	int x1,x2,y1,y2,y;

	x1 = Camera.Region.Vertex[0].x;
	x2 = Camera.Region.Vertex[1].x;
	y1 = Camera.Region.Vertex[0].y;
	y2 = Camera.Region.Vertex[1].y;

	for(int x = 0; x < MAP_WIDTH; x++)
	{
		Land.VisibleMap[x][0] = 0;
		Land.VisibleMap[x][1] = 0;
	}


	for(int x = (int)GetMin(x1,x2); x < GetMax(x1,x2); x++)
		  {
			  y = 0;
			  y = (int)((x - x1)*(y2 - y1)/(x2 - x1 + 0.00001) + y1);
			  if(PointInInterval(x,0,MAP_WIDTH))
			  {
				  if(PointInInterval(y,0,MAP_HEIGHT)) 
				  {
					  if(!Land.VisibleMap[x][0]) 
						  Land.VisibleMap[x][0] = y;
					  else Land.VisibleMap[x][1] = y; 
				  }

				  if(y > MAP_HEIGHT)
				  {
					  if(!Land.VisibleMap[x][0]) 
						  Land.VisibleMap[x][0] = MAP_HEIGHT-1;
					  else Land.VisibleMap[x][1] = MAP_HEIGHT-1; 
				  }
			  }
	}

	x1 = Camera.Region.Vertex[1].x;
	x2 = Camera.Region.Vertex[2].x;
	y1 = Camera.Region.Vertex[1].y;
	y2 = Camera.Region.Vertex[2].y;

	for(int x = (int)GetMin(x1,x2); x < GetMax(x1,x2); x++)
		  {
			  y = 0;
			  y = (int)((x - x1)*(y2 - y1)/(x2 - x1 + 0.00001) + y1);
			  if(PointInInterval(x,0,MAP_WIDTH))
			  {
				  if(PointInInterval(y,0,MAP_HEIGHT)) 
				  {
					  if(!Land.VisibleMap[x][0]) 
						  Land.VisibleMap[x][0] = y;
					  else Land.VisibleMap[x][1] = y; 
				  }

				  if(y > MAP_HEIGHT)
				  {
					  if(!Land.VisibleMap[x][0]) 
						  Land.VisibleMap[x][0] = MAP_HEIGHT-1;
					  else Land.VisibleMap[x][1] = MAP_HEIGHT-1; 
				  }
			  }
	}

	x1 = Camera.Region.Vertex[2].x;
	x2 = Camera.Region.Vertex[3].x;
	y1 = Camera.Region.Vertex[2].y;
	y2 = Camera.Region.Vertex[3].y;

	for(int x = (int)GetMin(x1,x2); x < GetMax(x1,x2); x++)
	{
			  y = 0;
			  y = (int)((x - x1)*(y2 - y1)/(x2 - x1 + 0.00001) + y1);
			  if(PointInInterval(x,0,MAP_WIDTH))
			  {
				  if(PointInInterval(y,0,MAP_HEIGHT)) 
				  {
					  if(!Land.VisibleMap[x][0]) 
						  Land.VisibleMap[x][0] = y;
					  else Land.VisibleMap[x][1] = y; 
				  }

				  if(y > MAP_HEIGHT)
				  {
					  if(!Land.VisibleMap[x][0]) 
						  Land.VisibleMap[x][0] = MAP_HEIGHT-1;
					  else Land.VisibleMap[x][1] = MAP_HEIGHT-1; 
				  }
			  }
	}

	x1 = Camera.Region.Vertex[3].x;
	x2 = Camera.Region.Vertex[0].x;
	y1 = Camera.Region.Vertex[3].y;
	y2 = Camera.Region.Vertex[0].y;

	for(int x = (int)GetMin(x1,x2); x < GetMax(x1,x2); x++)
		  {
			  y = 0;
			  y = (int)((x - x1)*(y2 - y1)/(x2 - x1 + 0.00001) + y1);
			  if(PointInInterval(x,0,MAP_WIDTH))
			  {
				  if(PointInInterval(y,0,MAP_HEIGHT)) 
				  {
					  if(!Land.VisibleMap[x][0]) 
						  Land.VisibleMap[x][0] = y;
					  else Land.VisibleMap[x][1] = y; 
				  }

				  if(y > MAP_HEIGHT)
				  {
					  if(!Land.VisibleMap[x][0]) 
						  Land.VisibleMap[x][0] = MAP_HEIGHT-1;
					  else Land.VisibleMap[x][1] = MAP_HEIGHT-1; 
				  }
			  }

		  }




}



void TGLWorld::Prepare()
{
// glEnable(GL_CULL_FACE);
// glCullFace(GL_BACK);

 glDisable(GL_NORMALIZE);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(40.0f,(GLfloat)ResolutionX/(GLfloat)ResolutionY,0.1f,1000.0f);
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 glMatrixMode(GL_PROJECTION);
 glMatrixMode(GL_MODELVIEW);  

 glLoadIdentity(); 


 float color[4]={1,1,1,1};
 
 glEnable(GL_TEXTURE_2D);  
 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LEQUAL);   
 
 glEnable(GL_LIGHT1);
 glEnable(GL_COLOR_MATERIAL);
 glShadeModel(GL_SMOOTH);
 glEnable(GL_LIGHTING);
 glLightfv(GL_LIGHT1, GL_DIFFUSE, color);
 glDisable(GL_BLEND);

 glBlendFunc(GL_SRC_ALPHA,GL_ONE);

 GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR }; 

 GLuint fogfilter= 2;                    

 GLfloat fogColor[4]= {0.5f, 0.5, 1, 0.4f}; 

 glEnable(GL_FOG);                       
 glFogi(GL_FOG_MODE, fogMode[fogfilter]);
 glFogfv(GL_FOG_COLOR, fogColor);      
 glFogf(GL_FOG_DENSITY, 0.0018f);   
 glHint(GL_FOG_HINT, GL_NICEST);   
 glFogf(GL_FOG_START, float(FOG_DISTANCE));      
 glFogf(GL_FOG_END, float(FOG_DISTANCE + 50));  
 glColor3f(1,1,1);

}


void TGLWorld::SetNewCameraPos(float dX, float dY, float dZ)
{

	int D = 10;

	if(Camera.X + dX < D)
	{
		Camera.X = D;
		dX = 0;
	}

	if(Camera.X + dX > MAP_WIDTH - D) 
	{
		Camera.X = MAP_WIDTH - D;
		dX = 0;
	}

	if(Camera.Y + dY < D)
	{
		Camera.Y = D;
		dY = 0;
	}

	if(Camera.Y + dY > MAP_HEIGHT - D)
	{
		Camera.Y = MAP_HEIGHT - D;
		dY = 0;
	}

	if(Camera.Z + dZ < CAMERA_MIN_HEIGHT)
	{
		Camera.Z = CAMERA_MIN_HEIGHT;
		dZ = 0;
	}

	if(Camera.Z + dZ > CAMERA_MAX_HEIGHT)
	{
		Camera.Z = CAMERA_MAX_HEIGHT;
		dZ = 0;
	}

	Camera.X +=dX;
	Camera.Y +=dY;
	Camera.Z +=dZ;

	if(Camera.Z - Society->MAP[(int)(Camera.X + dX)][(int)(Camera.Y + dY)].Z < CAMERA_MIN_HEIGHT)
		Camera.Z = Society->MAP[(int)(Camera.X + dX)][(int)(Camera.Y + dY)].Z + CAMERA_MIN_HEIGHT;
		
	
}


void TGLWorld::Refresh()
{
 glPushMatrix();
  glLoadIdentity();

  Prepare();

  float pos[4] = {60,30,-65,1};	


/* if(Camera.bAttached)
  {
	  Camera.X = Camera.AttObject->TempX - 10*cos(Camera.XAngle*OGIR);
	  Camera.Y = Camera.AttObject->TempY - 10*sin(Camera.XAngle*OGIR);
  }
*/

  Camera.SetPos();


  GetViewRegion();


  pos[0] = Camera.X;
  pos[1] = Camera.Y;

  glLightfv(GL_LIGHT1, GL_POSITION, pos);

  if(fLandscape)
	  Land.DrawLand();


  glLightfv(GL_LIGHT1, GL_POSITION, pos);
  glEnable(GL_NORMALIZE);
  glDisable(GL_CULL_FACE);
  glDisable(GL_TEXTURE_2D);

  if((Pointer.Cursor->IsLClick)&&(Pointer.Cursor->Focus == FOCUS_WORLD))
  {
	  Pointer.StartRect = Pointer.GetPoint(Pointer.Cursor->ScreenX, Pointer.Cursor->ScreenY);
	  Pointer.Pos = Pointer.StartRect; 
	  {
	  if(!Society->MAP[(int)Pointer.Pos.X][(int)Pointer.Pos.Y].Team) 
		  Pointer.Object = 1;
	  else 
		  Pointer.Object = 0;
	  }
  }

  if(Command.Is)
  {
	  Command.MapPoint = Pointer.GetPoint(Command.Point.x, Command.Point.y);
	  OnCommand();
	  Command.Is = false;
  }


  glBindTexture(GL_TEXTURE_2D,ModelTextures[5]);
 if((Pointer.Cursor->Enabled)&&(Pointer.bRect))
	 Pointer.DrawRect();


 pos[3] = -65;
   glLightfv(GL_LIGHT1, GL_POSITION, pos);

  if(fObjects)
  {
	  for (int i = 0; i < NUMBER_OF_TEAMS; i++ )
	   for (int j = 0; j < NUMBER_OF_UNITS; j++ )
	   {

			POINT ObjPos;

			ObjPos.x = Society->GObject[i][j].X;
			ObjPos.y = Society->GObject[i][j].Y;

			if((Society->GObject[i][j].Is)&&(Camera.Region.CheckPoint(ObjPos)))
			{
			   Society->GObject[i][j].SetModelFrame();      
			   DrawObject(i,j);
			}
	   }
  }

  if(Pointer.bNewSelection)
  {
	  Pointer.bNewSelection = false;
  }

  Pointer.Cursor->IsLClick = false;
  Pointer.Cursor->IsRClick = false;
  Pointer.Cursor->IsLDbClick = false;


  glColor3f(1,1,1);
 glPopMatrix();

}


//-------------------------------Рисование ландшафта---------------------------------------------

T3DPoint NormalTriangle(float p1x,float p1y,float p1z,
					   float p2x,float p2y,float p2z,
					   float p3x,float p3y,float p3z)
{

	float pt[3];
	float a[3] = {p1x, p1y, p1z};
	float b[3] = {p2x, p2y, p2z};
	float c[3] = {p3x, p3y, p3z};

	getnormf(a, b, c, pt);

	T3DPoint Res;
	Res.X = pt[0];
	Res.Y = pt[1];
	Res.Z = pt[2];

	//float x1, y1, z1, x2, y2, z2, l;
	//
	//x1 = p2x - p1x;
	//x2 = p3x - p2x;

	//y1 = p2y - p1y;
	//y2 = p3y - p2y;

	//z1 = p2z - p1z;
	//z2 = p3z - p2z;

	//Res.X = y1*z2 - z1*y2;
	//Res.Y = z1*x2 - x1*z2;
	//Res.Z = x1*y2 - y1*x2;

	float l = sqrt( (double)Res.X*Res.X + Res.Y*Res.Y + Res.Z*Res.Z );

	Res.X/=-l;
	Res.Y/=-l;
	Res.Z/=-l;

	return Res;
}


void TGLLand::DrawQuad(int X, int Y)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	int R = MAP_RESOLUTION;
	float A = 0;
	float T = 1;
	float K = MAP_TEXTURES_RESOLUTION;
			int tX, tY, lX, lY;

			glBegin(GL_TRIANGLE_FAN);

				TVector Normal = NormalTriangle(R,0,Society->MAP[X][Y].Z,
 												 0,0,Society->MAP[X][Y].Z,
												 0,R,Society->MAP[X][Y+R].Z);

			   	glNormal3f(Normal.Radius.X,Normal.Radius.Y,Normal.Radius.Z);
			    glTexCoord2f(K/2, K/2);
			    glVertex3f(X,Y,Society->MAP[X][Y].Z);
				

				lX = X + R;
				lY = Y - R;
				tX = X;
				tY = Y - R;
				Normal = NormalTriangle(lX,lY,Society->MAP[tX][tY].Z,
										X,Y,Society->MAP[X][Y].Z,
										tX,tY,Society->MAP[lX][lY].Z);
			   	glNormal3f(Normal.Radius.X,Normal.Radius.Y,Normal.Radius.Z);
			    glTexCoord2f(K/2, 0);
			    glVertex3f(tX,tY, Society->MAP[tX][tY].Z);
				
				lX = tX;
				lY = tY;
				tX = X - R;
				tY = Y - R;
				Normal = NormalTriangle(lX,lY,Society->MAP[tX][tY].Z,
										X,Y,Society->MAP[X][Y].Z,
										tX,tY,Society->MAP[lX][lY].Z);
			   	glNormal3f(Normal.Radius.X,Normal.Radius.Y,Normal.Radius.Z);
			    glTexCoord2f(0,0);
			    glVertex3f(tX,tY,Society->MAP[tX][tY].Z); 

				lX = tX;
				lY = tY;
				tX = X - R;
				tY = Y;
				Normal = NormalTriangle(lX,lY,Society->MAP[tX][tY].Z,
										X,Y,Society->MAP[X][Y].Z,
										tX,tY,Society->MAP[lX][lY].Z);
			   	glNormal3f(Normal.Radius.X,Normal.Radius.Y,Normal.Radius.Z);
			    glTexCoord2f(0, K/2);
			    glVertex3f(tX,tY,Society->MAP[tX][tY].Z); 

				lX = tX;
				lY = tY;
				tX = X - R;
				tY = Y + R;
				Normal = NormalTriangle(tX,tY,Society->MAP[tX][tY].Z,
										X,Y,Society->MAP[X][Y].Z,
										lX,lY,Society->MAP[lX][lY].Z);
			   	glNormal3f(Normal.Radius.X,Normal.Radius.Y,Normal.Radius.Z);
			    glTexCoord2f(0, K);
			    glVertex3f(tX, tY,Society->MAP[tX][tY].Z); 

				lX = tX;
				lY = tY;
				tX = X;
				tY = Y + R;
				Normal = NormalTriangle(tX,tY,Society->MAP[tX][tY].Z,
										X,Y,Society->MAP[X][Y].Z,
										lX,lY,Society->MAP[lX][lY].Z);
			   	glNormal3f(Normal.Radius.X,Normal.Radius.Y,Normal.Radius.Z);
			    glTexCoord2f(K/2, K);
			    glVertex3f(tX, tY,Society->MAP[tX][tY].Z); 

				lX = tX;
				lY = tY;
				tX = X + R;
				tY = Y + R;
				Normal = NormalTriangle(tX,tY,Society->MAP[tX][tY].Z,
										X,Y,Society->MAP[X][Y].Z,
										lX,lY,Society->MAP[lX][lY].Z);
			   	glNormal3f(Normal.Radius.X,Normal.Radius.Y,Normal.Radius.Z);
			    glTexCoord2f(K, K);
			    glVertex3f(tX, tY,Society->MAP[tX][tY].Z); 

				lX = tX;
				lY = tY;
				tX = X + R;
				tY = Y;
				Normal = NormalTriangle(tX,tY,Society->MAP[tX][tY].Z,
										X,Y,Society->MAP[X][Y].Z,
										lX,lY,Society->MAP[lX][lY].Z);
			   	glNormal3f(Normal.Radius.X,Normal.Radius.Y,Normal.Radius.Z);
			    glTexCoord2f(K, K/2);
			    glVertex3f(tX, tY,Society->MAP[tX][tY].Z); 

				lX = tX;
				lY = tY;
				tX = X + R;
				tY = Y - R;
				Normal = NormalTriangle(tX,tY,Society->MAP[tX][tY].Z,
										X,Y,Society->MAP[X][Y].Z,
										lX,lY,Society->MAP[lX][lY].Z);
			   	glNormal3f(Normal.Radius.X,Normal.Radius.Y,Normal.Radius.Z);
			    glTexCoord2f(K, 0);
			    glVertex3f(tX, tY,Society->MAP[tX][tY].Z); 

				lX = tX;
				lY = tY;
				tX = X;
				tY = Y - R;
				Normal = NormalTriangle(tX,tY,Society->MAP[tX][tY].Z,
										X,Y,Society->MAP[X][Y].Z,
										lX,lY,Society->MAP[lX][lY].Z);
			   	glNormal3f(Normal.Radius.X,Normal.Radius.Y,Normal.Radius.Z);
			    glTexCoord2f(K/2, 0);
			    glVertex3f(tX , tY,Society->MAP[tX][tY].Z); 



			glEnd();




}

/*
T3DPoint Normal8Faces(int X, int Y)
{

}
*/

void TGLLand::GetNormals(int Column, int MinY, int MaxY)
{
	int X = Column*MAP_RESOLUTION;

	for(int Y = MinY; Y < MaxY; Y++)
	{
//		NormalBuf.Normals[Y][1] = 0;
	}
}


void TGLLand::DrawLand()
{
int s,t;
GLuint Tex;


//glEnable(GL_NORMALIZE);
glEnable(GL_CULL_FACE);
glEnable(GL_TEXTURE_2D);
//glEnable(GL_MAP2_VERTEX_3);
//glEnable(GL_MAP2_TEXTURE_COORD_2);



long int k = 0;
int R = MAP_RESOLUTION;
glEnable(GL_LIGHTING);
glEnable(GL_COLOR_MATERIAL);
glEnable(GL_TEXTURE0_ARB);
glEnable(GL_TEXTURE1_ARB);

glFrontFace(GL_CW);

glColor3f(1,1,1);



for(int j = 4; j < (int)((MAP_WIDTH-1)/R - 4); j+=2)
{
	int Min = (int)(GetMin(VisibleMap[j*R+4*R][0],VisibleMap[j*R+1][1])/R)-R;
	int Min1 = (int)(GetMin(VisibleMap[j*R-4*R][0],VisibleMap[j*R+1][1])/R)-R;

	Min = (int)GetMin(Min,Min1);

	if(Min < 0) Min = 1;

	if(!(Min%2)) Min-=1; 

	int Max = (int)(GetMax(VisibleMap[j*R+4*R][0],VisibleMap[j*R+1][1])/R)+R;
	int Max1 = (int)(GetMax(VisibleMap[j*R-4*R][0],VisibleMap[j*R+1][1])/R)+R;

	Max = (int)GetMax(Max,Max1);

	if(Max >= (MAP_HEIGHT/R)) 
		Max = (int)(MAP_HEIGHT/R) - R;

	for(int i = Min; i < Max; i+=2)
		   {
			   int X = j*R;
			   int Y = i*R;

				if(sqr(Camera->X-X) + sqr(Camera->Y-Y) < sqr(VISIBLE_DISTANCE))
				{
/*					if((Society->MAP[X][Y].Tex == Society->MAP[X + 2*R][Y].Tex)&&
					   (Society->MAP[X][Y].Tex == Society->MAP[X][Y + 2*R].Tex)&&
					   (Society->MAP[X][Y].Tex == Society->MAP[X + 2*R][Y + 2*R].Tex))
*/					{
						glBindTexture(GL_TEXTURE_2D, Textures[(int)Society->MAP[X][Y].Tex]);

						DrawQuad(X,Y);
					}
/*					else
					{

						glActiveTextureARB(GL_TEXTURE0_ARB);
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D,  Textures[4]);

						glActiveTextureARB(GL_TEXTURE1_ARB);
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D,  Textures[2]);

						glBegin(GL_QUADS);
							glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
							glMultiTexCoord2fA
							RB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
							glVertex3f(X - R, Y - R, Society->MAP[X - R][Y - R].Z);

							glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
							glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
							glVertex3f(X - R, Y + R, Society->MAP[X-R][Y+R].Z);

							glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
							glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
							glVertex3f(X + R, Y + R, Society->MAP[X+R][Y+R].Z);

							glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
							glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
							glVertex3f(X + R, Y - R, Society->MAP[X+R][Y-R].Z);
						glEnd();

						glEnable(GL_BLEND);

						glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
						glBindTexture(GL_TEXTURE_2D, Textures[2]);
						DrawQuad(X,Y);

						glBindTexture(GL_TEXTURE_2D, Textures[1]);
						DrawQuad(X,Y);

						glDisable(GL_BLEND);

					}
*/
				}
	}
}
glFrontFace(GL_CCW);


/*
R = 200;
int T = 100;
glBindTexture(GL_TEXTURE_2D,Textures[2]); 

	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f(-R,-R,-0.5);

		glTexCoord2f(0,T);
		glVertex3f(-R,R,-0.5);

		glTexCoord2f(T,T);
		glVertex3f(R,R,-0.5);

		glTexCoord2f(T,0);
		glVertex3f(R,-R,-0.5);
	glEnd();
*/
RECT Rect;
Rect.top = MAP_HEIGHT - VISIBLE_DISTANCE;
Rect.bottom = VISIBLE_DISTANCE;
Rect.left = VISIBLE_DISTANCE;
Rect.right = MAP_WIDTH - VISIBLE_DISTANCE;
if(!PointInRect(Camera->X,Camera->Y,Rect)) DrawBorder();

glDisable(GL_CULL_FACE);
glDisable(GL_TEXTURE0_ARB);
glDisable(GL_TEXTURE1_ARB);
}




void TGLLand::DrawBorder()
{
//glDisable(GL_FOG);
glDisable(GL_LIGHTING);
glDisable(GL_CULL_FACE);
/*
int grid = 2;
float ctlpoints[8][2][3];
float texmap[8][2][2];
int i;



glBindTexture(GL_TEXTURE_2D,MiscTextures[1]);


if(Boundaries.top)
{
	for (i = 0; i < 8; i++)
	 {
		 ctlpoints[i][0][0] = i*MAP_WIDTH/7;
		 ctlpoints[i][0][1] = MAP_WIDTH;
		 ctlpoints[i][0][2] = -10;

		 texmap[i][0][0] = i;
		 texmap[i][0][1] = 0;
 
		 ctlpoints[i][1][0] = i*MAP_WIDTH/7;
		 ctlpoints[i][1][1] = MAP_WIDTH;
		 ctlpoints[i][1][2] = 100;


		 texmap[i][1][0] = i;
		 texmap[i][1][1] = 8;
	}

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 2, 0, 1, 6, 8,
			&ctlpoints[0][0][0]);

	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 8,
			&texmap[0][0][0]);

	glMapGrid2f(grid, 0.0, 1.0, grid, 0.0, 1.0);

	glEvalMesh2(GL_FILL, 0, grid, 0, grid);

}

if(Boundaries.bottom)
{
	for (i = 0; i < 8; i++)
	 {
		 ctlpoints[i][0][0] = i*MAP_WIDTH/7;
		 ctlpoints[i][0][1] = 0;
		 ctlpoints[i][0][2] = -10;

		 texmap[i][0][0] = i;
		 texmap[i][0][1] = 0;
 
		 ctlpoints[i][1][0] = i*MAP_WIDTH/7;
		 ctlpoints[i][1][1] = 0;
		 ctlpoints[i][1][2] = 100;


		 texmap[i][1][0] = i;
		 texmap[i][1][1] = 8;
	}

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 2, 0, 1, 6, 8,
			&ctlpoints[0][0][0]);

	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 8,
			&texmap[0][0][0]);

	glMapGrid2f(grid, 0.0, 1.0, grid, 0.0, 1.0);

	glEvalMesh2(GL_FILL, 0, grid, 0, grid);

}




if(Boundaries.right)
{
	for (i = 0; i < 8; i++)
	 {
		 ctlpoints[i][0][0] = MAP_WIDTH;
		 ctlpoints[i][0][1] = i*MAP_WIDTH/7;
		 ctlpoints[i][0][2] = -10;

		 texmap[i][0][0] = i;
		 texmap[i][0][1] = 0;
 
		 ctlpoints[i][1][0] = MAP_WIDTH;
		 ctlpoints[i][1][1] = i*MAP_WIDTH/7;
		 ctlpoints[i][1][2] = 100;


		 texmap[i][1][0] = i;
		 texmap[i][1][1] = 8;
	}

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 2, 0, 1, 6, 8,
			&ctlpoints[0][0][0]);

	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 8,
			&texmap[0][0][0]);

	glMapGrid2f(grid, 0.0, 1.0, grid, 0.0, 1.0);

	glEvalMesh2(GL_FILL, 0, grid, 0, grid);

}

if(Boundaries.left)
{
	for (i = 0; i < 8; i++)
	 {
		 ctlpoints[i][0][0] = 0;
		 ctlpoints[i][0][1] = i*MAP_WIDTH/7;
		 ctlpoints[i][0][2] = -10;

		 texmap[i][0][0] = i;
		 texmap[i][0][1] = 0;
 
		 ctlpoints[i][1][0] = 0;
		 ctlpoints[i][1][1] = i*MAP_WIDTH/7;
		 ctlpoints[i][1][2] = 100;


		 texmap[i][1][0] = i;
		 texmap[i][1][1] = 8;
	}

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 2, 0, 1, 6, 8,
			&ctlpoints[0][0][0]);

	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 8,
			&texmap[0][0][0]);

	glMapGrid2f(grid, 0.0, 1.0, grid, 0.0, 1.0);

	glEvalMesh2(GL_FILL, 0, grid, 0, grid);

}

*/

glPushMatrix();
	glTranslatef(MAP_WIDTH/2,MAP_HEIGHT/2,0);
	GLUquadricObj* Sphere;
	Sphere = gluNewQuadric();

	gluQuadricOrientation(Sphere,GLU_INSIDE);

//	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);

//	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glBindTexture(GL_TEXTURE_2D,Textures[1]); 
	gluQuadricTexture(Sphere,GL_TRUE);       
	gluSphere(Sphere,MAP_HEIGHT*0.7,20,20);
glPopMatrix();



glDisable(GL_TEXTURE_GEN_S);
glDisable(GL_TEXTURE_GEN_T);


/*
float R = MAP_HEIGHT*0.7;
float T = 100;
glDisable(GL_FOG);

glBindTexture(GL_TEXTURE_2D,MiscTextures[2]); 

	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f(-R,-R,-0.5);

		glTexCoord2f(0,T);
		glVertex3f(-R,R,-0.5);

		glTexCoord2f(T,T);
		glVertex3f(R,R,-0.5);

		glTexCoord2f(T,0);
		glVertex3f(R,-R,-0.5);
	glEnd();
*/
glEnable(GL_CULL_FACE);
glEnable(GL_FOG);
glEnable(GL_LIGHTING);

}




//-------------------------------Рисование объектов---------------------------------------------
void TGLWorld::DrawObject(int Team, int Index)
{
// glEnable(GL_DEPTH_TEST);
glEnable(GL_CULL_FACE);

// glEnable(GL_BLEND);
// glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 int Width = Society->GObject[Team][Index].Width;
 float Angle;

 float X = Society->GObject[Team][Index].TempX;
 float Y = Society->GObject[Team][Index].TempY;
 float Z = Society->GObject[Team][Index].TempZ;        
	       
 	
 if(Team==0) glColor4ub(TEAM_COLOR_0,255);  
 if(Team==1) glColor4ub(TEAM_COLOR_1,255);
 if(Team==2) glColor4ub(TEAM_COLOR_2,255);
 if(Team==3) glColor4ub(TEAM_COLOR_3,255);
 if(Team==4) glColor4ub(TEAM_COLOR_4,255);
 if(Team==5) glColor4ub(TEAM_COLOR_5,255);


 if(Society->GObject[Team][Index].TeamIndex == MAN_TEAM)
 {
	 if(Pointer.bRect)
		if((BigPointInInterval(Society->GObject[Team][Index].X, Width, Pointer.StartRect.X, Pointer.EndRect.X))&&
		   (BigPointInInterval(Society->GObject[Team][Index].Y, Width, Pointer.StartRect.Y, Pointer.EndRect.Y)))
 			{
			 Society->GObject[Team][Index].Selected = true;
			 IsSelection = true;
			}


	 if(Pointer.bNewSelection)

	 {
		 Society->GObject[Team][Index].Selected = false;
		 IsSelection = false;

	 }



	 if(Pointer.Cursor->IsLClick)
		 if((PointInInterval(Society->GObject[Team][Index].X, Pointer.Pos.X - Width, Pointer.Pos.X + Width))&&
			(PointInInterval(Society->GObject[Team][Index].Y, Pointer.Pos.Y - Width, Pointer.Pos.Y + Width)))
			{
			 Society->GObject[Team][Index].Selected = true;
			 IsSelection = true;
			}
 }


	 if(Pointer.Cursor->IsLClick)
	 {
		 T3DPoint Pos = Pointer.GetPoint(Pointer.Cursor->ScreenX, Pointer.Cursor->ScreenY);
		 if((PointInInterval(Society->GObject[Team][Index].X, Pos.X - Width, Pos.X + Width))&&
			(PointInInterval(Society->GObject[Team][Index].Y, Pos.Y - Width, Pos.Y + Width)))
			{
			 Camera.bAttached = !Camera.bAttached;
			 Camera.AttObject = &Society->GObject[Team][Index];
			}
			 else
				 Camera.bAttached = false;
	 }
/*if (!((Society->GObject[Team][Index].ModelFrame==0)&&(!(Society->GObject[Team][Index].OldStatus.Moving||Society->GObject[Team][Index].OldStatus.Fire))))
    Society->GObject[Team][Index].ModelFrame++;*/

/*if (Society->GObject[Team][Index].ModelFrame == (MAX_FRAMES-1))
    Society->GObject[Team][Index].ModelFrame = 0; */

 glPushMatrix();

  glTranslatef(X,Y,Z);


Angle = Society->GObject[Team][Index].TempAngle;
 glRotatef(Angle+90,0,0,1);

 int a = (Society->GObject[Team][Index].OldStatus.Fire)?1:0;
 POINT p;
 p.x = Society->GObject[Team][Index].X;
 p.y = Society->GObject[Team][Index].Y;
if(Camera.Region.CheckPoint(p))
{
glDisable(GL_TEXTURE_2D);

glEnable(GL_DEPTH_TEST);
//    glCallList(ObjectList[Society->GObject[Team][Index].Type][a][Society->GObject[Team][Index].ModelFrame]);
	glCallList(ObjectList[0][a][Society->GObject[Team][Index].ModelFrame]);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

 if(Society->GObject[Team][Index].Selected)
	  DrawSelected(Team, Index, Angle);
 glPopMatrix();

}

 if(Society->GObject[Team][Index].Target.On || Society->GObject[Team][Index].Target.MustFree) 
   DrawAttack(Team,Index);

 glDisable(GL_BLEND);


}

void TGLWorld::DrawSelected(int Team, int Index, int Angle)
{
	float R = 1;//Society->GObject[Team][Index].Width;


	float LiveAngle = Society->GObject[Team][Index].Live*360/Society->Standart[Society->GObject[Team][Index].Type].Live;
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glDisable(GL_TEXTURE_2D);
	glDisable(GL_FOG);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);
	glColor3f(1,1,1);

	glPushMatrix();
	glTranslatef(0,0,1);
	GLUquadricObj* Disk;
	Disk = gluNewQuadric();

	glColor3f(0.1,1,0.2);

	gluDisk(Disk,R-0.1,R+0.2,10,10);


//	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);

//	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	gluDeleteQuadric(Disk);
	glPopMatrix();

/*	glBindTexture(GL_TEXTURE_2D, MiscTextures[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f(-R,-R,1);

		glTexCoord2f(0,1);
		glVertex3f(-R,R,1);

		glTexCoord2f(1,1);
		glVertex3f(R,R,1);

		glTexCoord2f(1,0);
		glVertex3f(R,-R,1);
	glEnd();
*/
	R = 0.1;
	glColor4f(LiveAngle/360,0,1 - LiveAngle/360,1);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	for (int i = 0; i < LiveAngle/15; i++)
	{
		glPushMatrix();
		glRotatef(90 + LiveAngle/15*i,0, 0,1);
		glBegin(GL_QUADS);
			glVertex3f(-R+1,-R,1);

			glVertex3f(-R+1,R,1);

			glVertex3f(R+1,R,1);

			glVertex3f(R+1,-R,1);
		glEnd();
		glPopMatrix();
	}





//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_FOG);

}




void TGLWorld::DrawAttack(int Team,int Index)
{   
if(Society->GObject[Team][Index].TypeWeapon == WP_LASER)
{
		glDisable(GL_FOG);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor4f(1,1,1,1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);

	glBindTexture(GL_TEXTURE_2D,MiscTextures[1]);

	glBegin(GL_QUADS);
	 glTexCoord2f(0,0);
	 glVertex3f(Society->GObject[Team][Index].TempX,
		        Society->GObject[Team][Index].TempY,
		        Society->GObject[Team][Index].PrimaryEnemy->TempZ + 0.4);

	 glTexCoord2f(0,1);
	 glVertex3f(Society->GObject[Team][Index].TempX,
		        Society->GObject[Team][Index].TempY,
		        Society->GObject[Team][Index].PrimaryEnemy->TempZ + 0.5);


 	 glTexCoord2f(1,1);
	 glVertex3f(Society->GObject[Team][Index].PrimaryEnemy->TempX,
		        Society->GObject[Team][Index].PrimaryEnemy->TempY,
		        Society->GObject[Team][Index].PrimaryEnemy->TempZ + 0.5);
				
	 glTexCoord2f(1,0);
	 glVertex3f(Society->GObject[Team][Index].PrimaryEnemy->TempX,
		        Society->GObject[Team][Index].PrimaryEnemy->TempY,
		        Society->GObject[Team][Index].PrimaryEnemy->TempZ + 0.4);

    glEnd();
}
if((Society->GObject[Team][Index].TypeWeapon == WP_HOWITZER)||(Society->GObject[Team][Index].TypeWeapon == WP_GUN))
{

	if(!Society->GObject[Team][Index].Target.MustFree)
	{

		glPushMatrix();
		glTranslatef(Society->GObject[Team][Index].Target.OldCurrentX,
					 Society->GObject[Team][Index].Target.OldCurrentY,
					 Society->GObject[Team][Index].Target.OldCurrentZ);

/*
		glRotated(70-Camera.XAngle,1,0,0); 
		glRotated(-Camera.YAngle,0,1,0);
		glRotated(-Camera.ZAngle,0,0,1);

		glBindTexture(GL_TEXTURE_2D, FireTextures[0]);

		glBegin(GL_QUADS);

			glTexCoord2f(TexFrame,0);
			glVertex3f(0,0,0);

			glTexCoord2f(TexFrame,1);
			glVertex3f(0,0,R);

			glTexCoord2f(TexFrame+0.125,1);
			glVertex3f(0,R,R);

			glTexCoord2f(TexFrame+0.125,0);
			glVertex3f(0,R,0);

		glEnd();
*/
		glDisable(GL_TEXTURE_2D);

		auxSolidSphere(0.1);
		glPopMatrix();

		glEnable(GL_TEXTURE_2D);
	}
	else
	{
	float TexFrame = 0.0625*Society->GObject[Team][Index].Target.FreeTimer;
		glDisable(GL_FOG);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor4f(1,1,1,1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);

		float R = Society->GObject[Team][Index].Target.Radius - 1;
		glPushMatrix();
		glTranslatef(Society->GObject[Team][Index].Target.X,
					 Society->GObject[Team][Index].Target.Y,
					 Society->GObject[Team][Index].Target.Z);

		glBindTexture(GL_TEXTURE_2D, FireTextures[0]);

			glBegin(GL_QUADS);

				glTexCoord2f(TexFrame,0);
				glVertex3f(-R,-R,1);

				glTexCoord2f(TexFrame,1);
				glVertex3f(-R,R,1);

				glTexCoord2f(TexFrame+0.0625,1);
				glVertex3f(R,R,1);

				glTexCoord2f(TexFrame+0.0625,0);
				glVertex3f(R,-R,1);

			glEnd();
		glPopMatrix();

	}
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_FOG);

}


}

/*
Обработка команд..
в классе TCommand есть переменная Type - вид команды( атаковать, идти, и т.п.)
и MapPoint - точка на карте(в координатах карты), на которой щелкнула мышь  
*/
void TGLWorld::OnCommand()
{
	switch (Command.Type)
	{
	case COMMAND_GO:
		{
			//Сюда пиши код для посылки юнитов! Точка, куда посылать - MapPoint
			Society->Teams[MAN_TEAM].SendXY.X = Command.MapPoint.X;
			Society->Teams[MAN_TEAM].SendXY.Y = Command.MapPoint.Y;
			break;
		}

	case COMMAND_ATTACK:
		{


			break;
		}

	case COMMAND_STOP:
		{


			break;
		}

	}


}


