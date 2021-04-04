//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio>
#include "Main.h"
#include "stdlib.h"
#pragma hdrstop
//---------------------------------------------------------------------------
TMain::TMain()
     {
     MEList       = NULL;
     Canvas       = NULL;
     MiniMap      = NULL;

     CurList      = TYPE;                // ���������� ������ �������� �� ����� ����� ��������
     CurTeam      = 0;

     CurX         = 0;
     CurY         = 0;

     LastMapX = LastMapY = MapX = MapY = -1;
     EditMode = EDIT_LAND_SCAPE;
     MouseDown = false;

     BrushRadius  = 4;
     BrushDelta   = 10;
     LSBrush      = SOFT_BRUSH;

     WeaponsNum   = 0;
     StandartsNum = 0;



     }

void TMain::Init(TListBox* _MEList, TCanvas* _Canvas)
     {

     LoadIniFile("Weapons.ini");
     LoadIniFile("Standarts.ini");


     MEList = _MEList;
     Canvas = _Canvas;

     Society.Standart = Standart;
     NUMBER_OF_STANDARTS = StandartsNum;

     Society.Weapon   = Weapon;
     NUMBER_OF_WEAPONS   = WeaponsNum;
     
     Society.GSocietyInit(true);
     }

void TMain::MiniMapAssign(TForm* _MiniMap)
     {
     MiniMap = _MiniMap;
     MiniMap->Width   = MAP_WIDTH;
     MiniMap->Height  = MAP_HEIGHT;
     }
//---------------------------------------------------------------------------
bool TMain::LoadIniFile(char* Path)
     {
     // ��� ������������ ��� ����� - ������ ��� ���� ��������.
     int file_t = -1;
     // 1. ����� ������� �����.
     // 2. ��������� ����������� ������ ��� �������� �����.
     // 3. �������� ����������.

     // 0. �������� �����.
     FILE* f = fopen(Path,"r");
     // 0.1 ����������, ������� ����������� ��� ��������.
     char buf = '\0';
     char word[WORD_LENGTH+1];   // +1 ��� ����-�������.
     long pos;
     // 0.2. ����������� ���� �����.
     for (int i = 0;(buf!=' ')&&(i<WORD_LENGTH);i++)
           {
           fread(&buf,sizeof(char),1,f);
           if (buf!=';')
              word[i] = buf;
              else
              {
              word[i] = '\0';
              break;
              };
           }
     fgetpos(f,&pos);
     if (eqwords(word,WEAPONS))
        file_t = WEAPON;
     if (eqwords(word,OBJECT_TYPES))
        file_t = TYPE;

     // 0.2.2. ���� ��� ��?���������, �� ��������� � �������������� ������;
     if (file_t == -1)
        {
        ShowMessage(SYNTAX_ERROR);
        return false;
        }

     // 1. ����� ������� �����.
     int num = -1;
     char lastbuf = '\0';
     buf = '\0';
     while (!((lastbuf=='[')&&(buf==']')))
           {
           lastbuf = buf;
           fread(&buf,sizeof(char),1,f);
           if(buf == '[')
              num++;
           }
     if (num == 0)
        {
        ShowMessage(SYNTAX_ERROR);
        return false;
        }
        else
        if (file_t == WEAPON)
           WeaponsNum   = num;
        else
        if (file_t == TYPE)
           StandartsNum = num;


     // 2. ��������� ����������� ������ ��� �������� �����.
     if (file_t==WEAPON)
        Weapon   = new TWeapon [num];
     if (file_t==TYPE)
        Standart = new TGStandart [num];
     // 3. �������� ����������.
     int ind = 0; // - ������ �������� ����.
     bool CanExit = false;

     // ��������� �����
     pos++;  // \0
     pos++;  // \n
     fsetpos(f,&pos);
     while (!CanExit)
       {
     // 3.1. ����������� �����, ��� �������� � ��������������� ����.
       char* word = new char[WORD_LENGTH+1];
       char strtname[STRING_LENGTH+1];
          {
          buf = 0;
          int i = 0;
          while((buf!=']')&&(i<=STRING_LENGTH))
               {
               fread(&buf,sizeof(char),1,f);
               if (buf!=']')
                  {
                  strtname[i] = buf;
                  i++;
                  }
                  else strtname[i] = '\0';
               }
          }
       GetWord('[',']',strtname,word);
       if (eqwords(word,"")||(word==NULL))
          {
          CanExit = true;
          continue;
          }
     // 3.1. ��������� ���
          if (file_t == WEAPON)
          ApplyValue("Name",word,file_t,ind);
          else if
             (file_t == TYPE)
             ApplyValue("Name",word,file_t,ind);

     // 3.2. ���������� ����� ����� � ��� ��������.
       bool CanNext = false;
       while (!CanNext)
         {
         char* name = new char[WORD_LENGTH+1];
     // 3.2.1. ���������� ������, ������������ ������ ������ � �������, ���� ���������� �����,
     //        �� ��������� � ���������� ����
         char sname[STRING_LENGTH+1];
             {
             buf = 0;
             int i = 0;
             while((buf!=';')&&(buf!='.')&&(i<=STRING_LENGTH))
                  {
                  fread(&buf,sizeof(char),1,f);
                  if (buf!=';')
                     {
                     // ���� ���������� ����� ��������� � ���������� ����
                     if (buf =='.')
                        CanNext =  true;

                     sname[i] = buf;
                     i++;
                     }
                     else
                        sname[i] = '\0';

                  }
             }
     // ���� �� ���� ������ ������� ������� � ���������� ����, ��:
         if (!CanNext)
            {
            GetWord('|','|',sname,name);
     // 3.2.2. ���������� �������� ����.
            char* val = new char [WORD_LENGTH+1];
            GetValue(sname,val);

     // 3.3. �������� �������� � ��������������� ����(��� �����������).
            if (!ApplyValue(name,val,file_t,ind))
      // 3.3.1. ���� �������� �������� - ������� � ���������� ����
               CanNext = true;
            delete name;
            delete val;
            }
         }
     //      ������� � ���������� ��������.
       ind++;
       delete word;
       }
     }

bool TMain::ApplyValue(char* Name, char* Value,int Type_of_type,int index)
     {
     // �������� �� ������������ �������� ����������.
     if (eqwords(Name,"")||eqwords(Value,""))
        return false;


     // ��� ��� ���� ��� �� ����������� ��������� �� ����� ���-����
     // ��������� - ���������� ����� ������� ���� � ����������� �����.

     // ���� ��� - ������
     if (Type_of_type == WEAPON)
        {
        // ����� ������ ������ SWITCH
        if      (eqwords(Name,NAME_WEAPON_NAME))
           {
           Weapon[index].NameLen = strlen(Value);
//         Weapon[index].Name = new char [Weapon[index].NameLen+1];
           strcpy(Weapon[index].Name,Value);
           }

        else if (eqwords(Name,NAME_LASER))
           Weapon[index].Laser = bool(atoi(Value));

        else if (eqwords(Name,NAME_TARGET))
           Weapon[index].Target = bool(atoi(Value));

        else if (eqwords(Name,NAME_TARGET_SPEED))
           Weapon[index].TargetSpeed = atoi(Value);

        else if (eqwords(Name,NAME_TARGET_DAMAGE))
           Weapon[index].TargetDamage = atoi(Value);

        else if (eqwords(Name,NAME_TARGET_RADIUS))
           Weapon[index].TargetRadius = atoi(Value);

        else if (eqwords(Name,NAME_TARGET_ROOF))
           Weapon[index].TargetRoof = atoi(Value);

        else if (eqwords(Name,NAME_LASER_RANGE))
           Weapon[index].LaserRange = atoi(Value);

        else if (eqwords(Name,NAME_LASER_DAMAGE))
           Weapon[index].LaserDamage = atoi(Value);

        return true;
        }
        else if
// ���� ����������� ���� ��������
        (Type_of_type == TYPE)
             {
             if (eqwords(Name,NAME_OBJECT_NAME))
                {
                Standart[index].NameLen = strlen(Value);
//              Standart[index].Name = new char [Standart[index].NameLen];
                strcpy(Standart[index].Name,Value);
                }
             else if (eqwords(Name,NAME_OBJECT_FILE_NAME))
                strcpy(Standart[index].FileName,Value);


             else
             if (eqwords(Name,NAME_OBJECT_LIVE))
                {
                Standart[index].Live = atoi(Value);
                }
             else
             if (eqwords(Name,NAME_OBJECT_SIGHT))
                {
                Standart[index].Sight = atoi(Value);
                }
             else

             if (eqwords(Name,NAME_OBJECT_SPEED))
                {
                Standart[index].Speed = atoi(Value);
                }
             else
             if (eqwords(Name,NAME_OBJECT_ANGLE_SPEED))
                {
                Standart[index].AngleSpeed = atoi(Value);
                }
             else
             if (eqwords(Name,NAME_OBJECT_ATTACK_PERIOD))
                {
                Standart[index].AttackPeriod = atoi(Value);
                }
             else
             if (eqwords(Name,NAME_OBJECT_ARMOR))
                {
                Standart[index].Armor = atoi(Value);
                }
             else
             if (eqwords(Name,NAME_OBJECT_WEAPON))
                {
                // ���������� ��� ������ � ���� ��,
                // ��� �������� ������������� ���������
                // ����� (��������)
                bool res = false;
                for (int i  = 0; i < WeaponsNum;i++)
                    if (eqwords(Value,Weapon[i].Name))
                       {
                       Standart[index].IndexWeapon = i;
                       res = true;
                       }
                }
             else
             if (eqwords(Name,NAME_OBJECT_WIDTH))
                {
                Standart[index].Width = atoi(Value);
                }
             else
             if (eqwords(Name,NAME_OBJECT_THINKING))
                {
                Standart[index].Thinking = (eqwords(Value,"Yes")||(atoi(Value)==1))?1:0;
                }
             else
             if (eqwords(Name,NAME_OBJECT_NVO))
                {
                Standart[index].NVO = atoi(Value);
                }
             else
             if (eqwords(Name,NAME_OBJECT_N))
                {
                Standart[index].N = atoi(Value);
                }
             else
             if (eqwords(Name,NAME_OBJECT_CAN_ATTACK))
                {
                Standart[index].CanAttack = (eqwords(Value,"Yes")||(atoi(Value)==1))?1:0;
                }
             else
             if (eqwords(Name,NAME_OBJECT_CAN_ATTACK_MOVE))
                {
                Standart[index].CanAttackMove = (eqwords(Value,"Yes")||(atoi(Value)==1))?1:0;
                }
             else
             if (eqwords(Name,NAME_OBJECT_CAN_MOVE))
                {
                Standart[index].CanMove = (eqwords(Value,"Yes")||(atoi(Value)==1))?1:0;
                }
             else
             if (eqwords(Name,NAME_OBJECT_MAX_MODEL_FRAMES))
                {
                Standart[index].MaxModelFrames = atoi(Value);
                }

             }
     return true;
     }
//---------------------------------------------------------------------------


void TMain::RefreshMEList()
     {
     // 1. ������� ������.
     MEList->Clear();
     // 2. ���������� ��������� ������.
     if (CurList == WEAPON)
        {
        for(int i = 0;i<WeaponsNum;i++)
           {
           MEList->Items->Add(Weapon[i].Name);
           }
        }
     else
     if (CurList == TYPE)
        {
        for(int i = 0;i<StandartsNum;i++)
           {
           MEList->Items->Add(Standart[i].Name);
           }
        }
     }


void TMain::DrawMap(int SX, int SY, int EX, int EY)
     {
//   Canvas->Refresh();

     // 0. ������ ������� - ��� ����� ������ ����� ��������� ������ �� ������� ����� ��������.
     Canvas->Brush->Color = clBlack;
     Canvas->RoundRect(0,0,FLAT_WIDTH*MAP_SCALE_X+(START_DRAW_X*2),FLAT_HEIGHT*MAP_SCALE_Y+(START_DRAW_Y*2),0,0);

     DrawMiniMap();
     switch (EditMode)
            {
            case EDIT_LAND_SCAPE:
//                                 DrawLandScpNet(SX, SY, EX, EY,true);
                                 DrawLandScpNet(SX, SY, EX, EY,false);
                                break;
            case EDIT_PLACE: DrawLandScape(SX, SY, EX, EY); DrawObjects(); break;
            case EDIT_OBJECT_MAP: DrawLandScape(SX, SY, EX, EY); DrawObjects(); break;            }
     }
void TMain::DrawMiniMap()
     {
      //    ����-����� �������� ����� � ���������
      for (int x = 0; x<MAP_WIDTH;x++)
      for (int y = 0; y<MAP_HEIGHT;y++)
          {
          if ((x == CurX)||(x == (FLAT_WIDTH+CurX)) || (y == CurY) ||(y == (FLAT_HEIGHT+CurY)) )
             {
             Canvas->Pixels[x+MINI_DRAW_X][y+MINI_DRAW_Y] = 256*128;
             }
             else
             if (Society.MAP[x][y].Team==-1)
                Canvas->Pixels[x+MINI_DRAW_X][y+MINI_DRAW_Y] = 256*round(Society.MAP[x][y].Z/MAP_SCALE_Z);
                else
                Canvas->Pixels[x+MINI_DRAW_X][y+MINI_DRAW_Y] = Society.MAP[x][y].Team*64+256*200;
          }

     }
void TMain::DrawLandScape(int SX, int SY, int EX, int EY)
     {

     // 1. ���������� ����, ������������ ��� ������ �� �������� ���� ������
     //    �� ������� ����
     for (int x = SX;x<EX ;x++)
     for (int y = SY;y<EY ;y++)
         {
     // 2.0.
     //     ��������� ���� ��������� ����� PixelX, PixelY, � ����� ��������
     //     ����� �� �������������� ����� � �������.
         int  PixelX = START_DRAW_X+((x-CurX)*MAP_SCALE_X);
         int  PixelY = START_DRAW_Y+((y-CurY)*MAP_SCALE_Y);
         int EPixelX = PixelX+MAP_SCALE_X;
         int EPixelY = PixelY+MAP_SCALE_Y;

         if (
            InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT, x, y,true,false,true,false)
            &&
            InXYSpace(0,FLAT_WIDTH,0,FLAT_HEIGHT,x-CurX,y-CurY,true,false,true,false)

            )
     // 2.1.
     //     ������ �������������� ������� � �������
     //     PixelX = x*MAP_SCALE_X �� (x+1) * PixelX*MAP_SCALE_X;
     //     ���������� ��� �.
     //     ���� ��������������� �������������� ������ ����� � ���� �����.
            {
            Canvas->Brush->Color = 256*round(Society.MAP[x][y].Z*MAP_SCALE_Z);
            Canvas->RoundRect(PixelX,PixelY,EPixelX,EPixelY,0,0);
            }

         }




     }


void TMain::DrawLandScpNet(int SX, int SY, int EX, int EY, bool Delete)
     {
     // 1. ���������� ��� ����� ��������� ������������ �� ������.
     for (int x = SX; x<EX; x++)
     for (int y = SY; y<EY; y++)
         if (InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,x,y,true,false,true,false))
            {
            // 2.Prev
            //    �������� ������� ������� ����� � ������ �����
            k  = (CurY - y)*MAP_SCALE_Y*0.1;

            Canvas->Brush->Color = (!Delete)?256*200:0;
            Canvas->Ellipse(
                           START_DRAW_X+(x-CurX)*MAP_SCALE_X + k - 2,  START_DRAW_Y+(y-CurY)*MAP_SCALE_Y - 1,
                           START_DRAW_X+(x-CurX)*MAP_SCALE_X + k + 2,  START_DRAW_Y+(y-CurY)*MAP_SCALE_Y + 1
                           );

            // 2. �� ������ ����� �������� ����� � ���������� ����� � ���, �������
            //    (��������� ������������ ������� ���������) �������� ����.
            //    ����������, ���� ��� ��������.

            int PrevX,   // ���������� �����, ���������� � �� �� ��� � � �������
                UpY;     // ������� �����.

            TPoint pos;
            pos.x  = (x-CurX)*MAP_SCALE_X;
            pos.y  = (y-CurY)*MAP_SCALE_Y;

            PrevX  = (InSpace(0,MAP_WIDTH,x-1,true,false))?pos.x-MAP_SCALE_X:-1;
            UpY    = (InSpace(0,MAP_HEIGHT,y-1,true,false))?pos.y-MAP_SCALE_Y:-1;




            int H = Society.MAP[x][y].Z;
            // ������� ����� � ������� ����� - ���������� ����� � ������ .

            // 2.0.
            Canvas->Pen->Color = (!Delete)? 256*256*255 + 256*(H/MAP_SCALE_Z)+(H) : 0;
            // 2.1.
            // ���� ���������� ����� ����������.
            if (PrevX != -1)
               {
               int h = Society.MAP[x-1][y].Z;

               Canvas->MoveTo( (pos.x + START_DRAW_X)+k, pos.y - (H/MAP_SCALE_Z) + START_DRAW_Y);
               Canvas->LineTo( (PrevX + START_DRAW_X)+k, pos.y -(h/MAP_SCALE_Z) + START_DRAW_Y);
               }

            // 2.2.
            // ���� ������� ����� ����������.
            if (UpY != -1)
               {
               int h = Society.MAP[x][y-1].Z;

               Canvas->MoveTo( (pos.x + START_DRAW_X)+k, pos.y - (H/MAP_SCALE_Z) + START_DRAW_Y);
               Canvas->LineTo( (pos.x + START_DRAW_X)+k, UpY - (h/MAP_SCALE_Z) + START_DRAW_Y);
               }

            }
     }


void TMain::DrawH()
     {
     if (InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,MapX,MapY,true,false,true,false))
        {
        k  = (CurY - MapY)*MAP_SCALE_Y*0.1;
          // 1. �������� ����� ����-������.
          TColor ClrNullH = clWhite;
          // 2. ����� ����������� ���� ������� � ��������� �����
          TColor ClrRealPointLine = clGreen;
          // 3. ���� �����.
          TColor ClrRealPoint = clRed+clBlue;

        int CanvasX = START_DRAW_X + ((MapX-CurX)*MAP_SCALE_X) + k;
        int CanvasNullY = START_DRAW_Y + ((MapY-CurY)*MAP_SCALE_Y);

        int H = int(Society.MAP[MapX][MapY].Z*MAP_SCALE_Z);
        int CanvasRealY = START_DRAW_Y + ((MapY-CurY)*MAP_SCALE_Y) - H;



//        Canvas->MoveTo(CanvasX,CanvasNullY);

        Canvas->Brush->Color = ClrRealPoint;
        Canvas->Ellipse(CanvasX-3,CanvasRealY-3,CanvasX+3,CanvasRealY+3);

//        Canvas->Pen->Color = ClrRealPointLine;
//        Canvas->LineTo(CanvasX,CanvasRealY);

        Canvas->Brush->Color = ClrNullH;
        Canvas->Ellipse(CanvasX-3,CanvasNullY-3,CanvasX+3,CanvasNullY+3);
        }


     // �������� ���������� �����.
     if (!((LastMapX==MapX)&&(LastMapY==MapY)))
     if (InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,LastMapX,LastMapY,true,false,true,false))
        {
        k  = (CurY - LastMapY)*MAP_SCALE_Y*0.1;
          // 1. �������� ����� ����-������.
          TColor ClrNullH = clBlack;
          // 2. ����� ����������� ���� ������� � ��������� �����
          TColor ClrRealPointLine = clBlack;
          // 3. ���� �����.
          TColor ClrRealPoint = clBlack;

        int LastCanvasX = START_DRAW_X + ((LastMapX - CurX)*MAP_SCALE_X) + k;
        int LastCanvasNullY = START_DRAW_Y + ((LastMapY - CurY)*MAP_SCALE_Y);

        int LastH = int(Society.MAP[LastMapX][LastMapY].Z*MAP_SCALE_Z);
        int LastCanvasRealY = START_DRAW_Y + ((LastMapY - CurY)*MAP_SCALE_Y) - LastH;

//        Canvas->MoveTo(LastCanvasX,LastCanvasNullY);

        Canvas->Brush->Color = ClrRealPoint;
        Canvas->Ellipse(LastCanvasX-3,LastCanvasRealY-3,LastCanvasX+3,LastCanvasRealY+3);


        Canvas->Pixels[LastCanvasX][LastCanvasNullY] = ClrNullH;

//        Canvas->Pen->Color = ClrRealPointLine;
//        Canvas->LineTo(LastCanvasX,LastCanvasRealY);

        Canvas->Brush->Color = ClrNullH;
        Canvas->Ellipse(LastCanvasX-3,LastCanvasNullY-3,LastCanvasX+3,LastCanvasNullY+3);


        Canvas->Pixels[LastCanvasX][LastCanvasRealY] = ClrRealPoint;
        }
     }


void TMain::DrawObjects()
     {
     for (int t = 0;t<NUMBER_OF_TEAMS;t++)
     for (int i = 0;i<NUMBER_OF_UNITS;i++)
         {
     // 2.0.
     //     ��������� ���� ��������� ����� PixelX, PixelY, � ����� ��������
     //     ����� �� �������������� ����� � �������.
         int  PixelX = START_DRAW_X+(((Society.GObject[t][i].X-CurX) - ceil(float(Society.GObject[t][i].Width)/2))*MAP_SCALE_X);
         int  PixelY = START_DRAW_Y+(((Society.GObject[t][i].Y-CurY) - ceil(float(Society.GObject[t][i].Width)/2))*MAP_SCALE_Y);
         int EPixelX = PixelX + (Society.GObject[t][i].Width*MAP_SCALE_X);
         int EPixelY = PixelY + (Society.GObject[t][i].Width*MAP_SCALE_Y);

         if (
            Society.GObject[t][i].Is
            &&
            InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT, Society.GObject[t][i].X, Society.GObject[t][i].Y,
                                                                            true,false,true,false)
            &&
            InXYSpace(0,FLAT_WIDTH,0,FLAT_HEIGHT,Society.GObject[t][i].X-CurX, Society.GObject[t][i].Y-CurY,
                                                                            true,false,true,false)
            )
     // 2.1.
     //     ������ �������������� ������� � �������
     //     PixelX = x*MAP_SCALE_X �� (x+1) * PixelX*MAP_SCALE_X;
     //     ���������� ��� �.
     //     ���� ��������������� �������������� ������ ����� � ���� �����.
            {
            Canvas->Brush->Color = (Society.GObject[t][i].TeamIndex*16) + 256*255 +
                                   (256*256*(Society.GObject[t][i].TeamIndex+1)*16);

            Canvas->RoundRect(PixelX,PixelY,EPixelX,EPixelY,2,2);
            }

         }
     }

//---------------------------------------------------------------------------

void TMain::EditLandScp(int _x,int _y,float delta, int radius)
     {
     // ���������������
     delta*=MAP_SCALE_Z;


     DrawLandScpNet(
                   _x - radius, _y - radius,
                   _x + radius+1, _y + radius+1, true
                   );

     // ���������� ��� ����� �������� �� ������� 2*radius � ������� x;y
     for (int x = _x - radius; x < _x + radius; x++)
     for (int y = _y - radius; y < _y + radius; y++)
         {
     //  ��������� ������� ����� �� �������������� �����.
         if (InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,x,y,true,false,true,false))
            {
            switch (LSBrush)
                   {
                   case PICK_BRUSH:
                        {
                        //  ���������� ������� ���������� �� ������ ����� �� ������ ��������.
                        float r_pow_2 = fkvdrt(x - _x) + fkvdrt(y - _y);
                        //  ���� ���������� �� ����� 0
                        if  (r_pow_2!=0)
                            Society.MAP[x][y].Z +=delta*(1/r_pow_2);
                            else
                            Society.MAP[x][y].Z += delta;
                        }break;
                   case SOFT_BRUSH:
                        {
                        float r = sqrt(fkvdrt(x - _x) + fkvdrt(y - _y));
                        // �������� ��������������� �����������
                        Society.MAP[x][y].Z += delta*(pow(0.7,(r/radius)*4));
                        }break;
                   case HARD_BRUSH:
                        {
                        Society.MAP[x][y].Z+=delta;
                        }break;
                   }
            }
         }
     DrawLandScpNet(
                   _x - radius, _y - radius,
                   _x + radius+1, _y + radius+1, false
                   );

     }



// �������������� ����� ��������
//---------------------------------------------------------------------------

void TMain::SetCurStandart()
     {
     CurStandart = MEList->ItemIndex;
     }

//---------------------------------------------------------------------------



// ����������

void TMain::OnMouseDown()
     {
     MouseDown = true;
     }

bool TMain::OnMouseMove(int MAP_X,int MAP_Y)
     {
     if (InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,MAP_X,MAP_Y,true,false,true,false))
     if (InXYSpace(0,FLAT_WIDTH,0,FLAT_HEIGHT,MAP_X-CurX,MAP_Y-CurY,true,false,true,false))
      {
      LastMapX = MapX;
      LastMapY = MapY;

      MapX     = MAP_X;
      MapY     = MAP_Y;
      if (MouseDown)
         {
         if (!((MapX == LastMapX)&&(MapY == LastMapY)))
            {
            switch (EditMode)
                   {
                   case EDIT_LAND_SCAPE:
                        {
                        EditLandScp(MAP_X,MAP_Y,BrushDelta,BrushRadius);
                        return true;
                        }
                        break;
                   case EDIT_PLACE:
                        {
                        return true;
                        }
                        break;
                   case EDIT_OBJECT_MAP:
                        {
                        SetCurStandart();
                        Society.MakeUnit(CurTeam,CurStandart,MapX,MapY);
                        return true;
                        }
                        break;
                   }
            }
         }
       }
       else
           {
           LastMapX = -1;
           LastMapY = -1;
           }
      return false;
      }

void TMain::OnMouseUp()
     {
     MapX = -1;
     MapY = -1;
     MouseDown = false;
     }





// ��������� ������� � ������
//---------------------------------------------------------------------------

void TMain::SaveMap(char* Path)
     {
     //  ������ �������� ������.

     //  1.  ������ �����      - 2 ��
     //  2.  ������ �����      - 2 ��
     //  3.  ����� ������      - 2 ��
     //  4.  ����� ������      - 2 ��
     //  5.  ����� ��������    - 2 ��
     //  6.  ���������� ������ - 2 ��

     //  7.  ������:
     //          1.������ ��������� ���������
     //          2.���������� ��� ���� ����� ������

     //  8.  �������:
     //          1. ������ ��������� ���������
     //          2. ���������� � ���� ����� ��������:


     //  9. �����:
     //          1. ������ ��������� ���������
     //          2. ���������� � ���� ������� ����� ([�][�])


     //  10.  ������� (�����):
     //          1. ������ ��������� ���������
     //          2. ���������� � ���� �������� ([�������][������ � �������])

     //  11. �������:
     //          1. ������ ��������� ���������
     //          2. ���������� � ���� ��������



     // ������ � ������������ � ��������.
     FILE* f = fopen(Path,"w+b");

     //  1.
     fwrite(&MAP_WIDTH,sizeof(MAP_WIDTH),1,f);
     //  2.
     fwrite(&MAP_HEIGHT,sizeof(MAP_HEIGHT),1,f);
     //  3.
     fwrite(&NUMBER_OF_UNITS,sizeof(NUMBER_OF_UNITS),1,f);
     //  4.
     fwrite(&NUMBER_OF_TEAMS,sizeof(NUMBER_OF_TEAMS),1,f);
     //  5.
     fwrite(&StandartsNum,sizeof(StandartsNum),1,f);
     //  6.
     fwrite(&WeaponsNum,sizeof(WeaponsNum),1,f);

     //  7.1.
     short int WP_SIZE = sizeof(TWeapon);
     fwrite(&WP_SIZE,sizeof(WP_SIZE),1,f);

     //  7.2
     for (int i = 0; i < WeaponsNum; i++)
         fwrite(&Weapon[i],WP_SIZE,1,f);

     //  8.1.
     short int ST_SIZE = sizeof(TGStandart);
     fwrite(&ST_SIZE,sizeof(ST_SIZE),1,f);

     //  8.2.
     for (int i = 0; i < StandartsNum; i++)
     fwrite(&Standart[i],ST_SIZE,1,f);

     //  9.1.
     short int MAP_SIZE = sizeof(TMap);
     fwrite(&MAP_SIZE,sizeof(MAP_SIZE),1,f);

     //  9.2.
     for (int x = 0; x < MAP_WIDTH; x++)
     for (int y = 0; y < MAP_HEIGHT; y++)
         fwrite(&Society.MAP[x][y],MAP_SIZE,1,f);


     //  10.1.
     short int OBJ_SIZE = sizeof (TGObject);
     fwrite(&OBJ_SIZE,sizeof(OBJ_SIZE),1,f);

     //  10.2.
     for (int t = 0; t<NUMBER_OF_TEAMS; t++)
     for (int i = 0; i<NUMBER_OF_UNITS; i++)
         fwrite(&Society.GObject[t][i],OBJ_SIZE,1,f);

     //  11.1
     short int TM_SIZE  = sizeof (TGSociety::TTeams);
     fwrite(&TM_SIZE,sizeof(TM_SIZE),1,f);
     //  11.2
     for (int t = 0; t<NUMBER_OF_TEAMS;t++)
         fwrite(&Society.Teams[t],TM_SIZE,1,f);


     // �������� �����.
     fclose(f);
     ShowMessage("Saved");
     }

bool TMain::LoadMap(char* Path)
     {
     // ������ ������ � ���������� ������ SaveMap.

     // ������ � ������������ � ��������.
     FILE* f = fopen(Path,"r+b");

     //  1.
     for (int x = 0; x<MAP_WIDTH;x++)
         delete Society.MAP[x];
     delete Society.MAP;
     fread(&MAP_WIDTH,sizeof(MAP_WIDTH),1,f);

     //  2.
     fread(&MAP_HEIGHT,sizeof(MAP_HEIGHT),1,f);
     //  3.
     fread(&NUMBER_OF_UNITS,sizeof(NUMBER_OF_UNITS),1,f);

     for (int t = 0; t<NUMBER_OF_TEAMS; t++)
         delete[] Society.GObject[t];

     delete[] Society.GObject;

     //  4.
     fread(&NUMBER_OF_TEAMS,sizeof(NUMBER_OF_TEAMS),1,f);
     //  5.
     fread(&StandartsNum,sizeof(StandartsNum),1,f);
     NUMBER_OF_STANDARTS = StandartsNum;
     //  6.
     fread(&WeaponsNum,sizeof(WeaponsNum),1,f);
     NUMBER_OF_WEAPONS = WeaponsNum;

     //  7.1.
     short int WP_SIZE;
     fread(&WP_SIZE,sizeof(WP_SIZE),1,f);
     if (WP_SIZE != sizeof(TWeapon))
        {
        ShowMessage(LOAD_ERROR);
        return false;
        }

     //  7.2

     delete[] Weapon;
     Weapon = new TWeapon [WeaponsNum];

     for (int i = 0; i < WeaponsNum; i++)
         fread(&Weapon[i],WP_SIZE,1,f);

     Society.Weapon = Weapon;    
     //  8.1.
     short int ST_SIZE;
     fread(&ST_SIZE,sizeof(ST_SIZE),1,f);
     if (ST_SIZE != sizeof(TGStandart))
        {
        ShowMessage(LOAD_ERROR);
        return false;
        }
     //  8.2.
     delete[] Standart;
     Standart = new TGStandart [StandartsNum];

     for (int i = 0; i < StandartsNum; i++)
     fread(&Standart[i],ST_SIZE,1,f);

     Society.Standart = Standart;

     //  9.1.
     short int MAP_SIZE;
     fread(&MAP_SIZE,sizeof(MAP_SIZE),1,f);
     if (MAP_SIZE != sizeof(TMap))
        {
        ShowMessage(LOAD_ERROR);
        return false;
        }


     //  9.2.
     Society.MAP = new TMap* [MAP_WIDTH];
     for (int x = 0; x < MAP_WIDTH; x++)
         {
         Society.MAP[x] = new TMap [MAP_HEIGHT];
         for (int y = 0; y < MAP_HEIGHT; y++)
             fread(&Society.MAP[x][y],MAP_SIZE,1,f);
         }


     //  10.1.
     short int OBJ_SIZE;
     fread(&OBJ_SIZE,sizeof(OBJ_SIZE),1,f);
     if (OBJ_SIZE != sizeof(TGObject))
        {
        ShowMessage(LOAD_ERROR);
        return false;
        }

     //  10.2.
     Society.GObject = new TGObject* [NUMBER_OF_TEAMS];
     for (int t = 0; t<NUMBER_OF_TEAMS; t++)
         {
         Society.GObject[t] = new TGObject [NUMBER_OF_UNITS];
         for (int i = 0; i<NUMBER_OF_UNITS; i++)
             fread(&Society.GObject[t][i],OBJ_SIZE,1,f);
         }

     //  11.1
     short int TM_SIZE;
     fread(&TM_SIZE,sizeof(TM_SIZE),1,f);
     if (TM_SIZE!=sizeof(TGSociety::TTeams))
        {
        ShowMessage(LOAD_ERROR);
        return false;
        }
     //  11.2
     delete[] Society.Teams;
     Society.Teams = new TGSociety::TTeams [NUMBER_OF_TEAMS];
     for (int t = 0; t<NUMBER_OF_TEAMS;t++)
         fread(&Society.Teams[t],TM_SIZE,1,f);



     RefreshMEList();
     DrawMap(CurX,CurY,CurX + FLAT_WIDTH, CurY + FLAT_HEIGHT);
     // �������� �����.
     fclose(f);
     }
#pragma package(smart_init)
