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

     CurList      = TYPE;                // Изначально список настроен на показ типов объектов
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
     // Тип считываемого ини файла - оружее или типы объектов.
     int file_t = -1;
     // 1. Общий подсчет типов.
     // 2. Выделение необходимой памяти для хранения типов.
     // 3. Загрузка параметров.

     // 0. Открытие файла.
     FILE* f = fopen(Path,"r");
     // 0.1 Переменные, которые понадобятся для загрузки.
     char buf = '\0';
     char word[WORD_LENGTH+1];   // +1 для нуль-символа.
     long pos;
     // 0.2. Определение типа файла.
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

     // 0.2.2. Если тип не?определен, то известить о синтаксической ошибке;
     if (file_t == -1)
        {
        ShowMessage(SYNTAX_ERROR);
        return false;
        }

     // 1. Общий подсчет типов.
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


     // 2. Выделение необходимой памяти для хранения типов.
     if (file_t==WEAPON)
        Weapon   = new TWeapon [num];
     if (file_t==TYPE)
        Standart = new TGStandart [num];
     // 3. Загрузка параметров.
     int ind = 0; // - индекс текущего типа.
     bool CanExit = false;

     // НЕОБХОДИМ РЕЗЕТ
     pos++;  // \0
     pos++;  // \n
     fsetpos(f,&pos);
     while (!CanExit)
       {
     // 3.1. Обнаружение имени, его загрузка в соответствующее поле.
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
     // 3.1. Загружаем имя
          if (file_t == WEAPON)
          ApplyValue("Name",word,file_t,ind);
          else if
             (file_t == TYPE)
             ApplyValue("Name",word,file_t,ind);

     // 3.2. Считывание имени полей и его загрузка.
       bool CanNext = false;
       while (!CanNext)
         {
         char* name = new char[WORD_LENGTH+1];
     // 3.2.1. Считывание строки, ограниченной справа точкой с запятой, если попадается точка,
     //        то переходим к следующему типу
         char sname[STRING_LENGTH+1];
             {
             buf = 0;
             int i = 0;
             while((buf!=';')&&(buf!='.')&&(i<=STRING_LENGTH))
                  {
                  fread(&buf,sizeof(char),1,f);
                  if (buf!=';')
                     {
                     // если попадается точка переходим к следующему типу
                     if (buf =='.')
                        CanNext =  true;

                     sname[i] = buf;
                     i++;
                     }
                     else
                        sname[i] = '\0';

                  }
             }
     // если не была отдана команда перейти к следующему типу, то:
         if (!CanNext)
            {
            GetWord('|','|',sname,name);
     // 3.2.2. Считывание значения поля.
            char* val = new char [WORD_LENGTH+1];
            GetValue(sname,val);

     // 3.3. Загрузка значения в соответствующее поле(его конвертация).
            if (!ApplyValue(name,val,file_t,ind))
      // 3.3.1. Если загрузка неудачна - перейти к следующему типу
               CanNext = true;
            delete name;
            delete val;
            }
         }
     //      Переход к следующему элементу.
       ind++;
       delete word;
       }
     }

bool TMain::ApplyValue(char* Name, char* Value,int Type_of_type,int index)
     {
     // Проверка на правильность указания параметров.
     if (eqwords(Name,"")||eqwords(Value,""))
        return false;


     // Тут для того что бы разобарться навверное не стоит что-либо
     // объяснять - достаточно знать перевод слов с английского языка.

     // Если тип - оружее
     if (Type_of_type == WEAPON)
        {
        // здесь должен стоять SWITCH
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
// Если считываются типы объектов
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
                // перебираем все оружия и ищем то,
                // имя которого соответствует заданному
                // имени (значению)
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
     // 1. Очистка списка.
     MEList->Clear();
     // 2. Добавление элементов списка.
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

     // 0. Рисуем рамочку - так чтобы небыло видно выходящие иногда за границы части объектов.
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
      //    МИНИ-КАРТА РИСУЕТСЯ СРАЗУ С ОБЪЕКТАМИ
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

     // 1. Огранизуем цикл, перебирающий все ячейки от верхнего угла экрана
     //    до нижнего угла
     for (int x = SX;x<EX ;x++)
     for (int y = SY;y<EY ;y++)
         {
     // 2.0.
     //     Проверяем ниже описанные точки PixelX, PixelY, а также конечные
     //     точки на принадлежность карте и канвасу.
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
     //     Рисуем прямоугольники начиная с пикселя
     //     PixelX = x*MAP_SCALE_X до (x+1) * PixelX*MAP_SCALE_X;
     //     аналогично для У.
     //     ЦВЕТ прямоугольников пропорционален высоте карты в этой точке.
            {
            Canvas->Brush->Color = 256*round(Society.MAP[x][y].Z*MAP_SCALE_Z);
            Canvas->RoundRect(PixelX,PixelY,EPixelX,EPixelY,0,0);
            }

         }




     }


void TMain::DrawLandScpNet(int SX, int SY, int EX, int EY, bool Delete)
     {
     // 1. Перебираем все точки ландшафта показываемые на экране.
     for (int x = SX; x<EX; x++)
     for (int y = SY; y<EY; y++)
         if (InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,x,y,true,false,true,false))
            {
            // 2.Prev
            //    Помечаем нулевой уровень карты в данной точке
            k  = (CurY - y)*MAP_SCALE_Y*0.1;

            Canvas->Brush->Color = (!Delete)?256*200:0;
            Canvas->Ellipse(
                           START_DRAW_X+(x-CurX)*MAP_SCALE_X + k - 2,  START_DRAW_Y+(y-CurY)*MAP_SCALE_Y - 1,
                           START_DRAW_X+(x-CurX)*MAP_SCALE_X + k + 2,  START_DRAW_Y+(y-CurY)*MAP_SCALE_Y + 1
                           );

            // 2. От каждой точки проводим линию к предыдущей точке и той, которая
            //    (учитываея компьютерную систему координат) осталась ниже.
            //    Разумеется, если это возможно.

            int PrevX,   // Предыдущая точка, координата У та же что и у текущей
                UpY;     // Верхняя точка.

            TPoint pos;
            pos.x  = (x-CurX)*MAP_SCALE_X;
            pos.y  = (y-CurY)*MAP_SCALE_Y;

            PrevX  = (InSpace(0,MAP_WIDTH,x-1,true,false))?pos.x-MAP_SCALE_X:-1;
            UpY    = (InSpace(0,MAP_HEIGHT,y-1,true,false))?pos.y-MAP_SCALE_Y:-1;




            int H = Society.MAP[x][y].Z;
            // Перевод ручки в текущую точку - проведение линии к данной .

            // 2.0.
            Canvas->Pen->Color = (!Delete)? 256*256*255 + 256*(H/MAP_SCALE_Z)+(H) : 0;
            // 2.1.
            // Если предыдущая точка существует.
            if (PrevX != -1)
               {
               int h = Society.MAP[x-1][y].Z;

               Canvas->MoveTo( (pos.x + START_DRAW_X)+k, pos.y - (H/MAP_SCALE_Z) + START_DRAW_Y);
               Canvas->LineTo( (PrevX + START_DRAW_X)+k, pos.y -(h/MAP_SCALE_Z) + START_DRAW_Y);
               }

            // 2.2.
            // Если верхняя точка существует.
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
          // 1. Выделяем точку ноль-уровня.
          TColor ClrNullH = clWhite;
          // 2. Линия соединяющая нуль уровень и настоящую точку
          TColor ClrRealPointLine = clGreen;
          // 3. Сама точка.
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


     // Затираем предыдущую линию.
     if (!((LastMapX==MapX)&&(LastMapY==MapY)))
     if (InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,LastMapX,LastMapY,true,false,true,false))
        {
        k  = (CurY - LastMapY)*MAP_SCALE_Y*0.1;
          // 1. Выделяем точку ноль-уровня.
          TColor ClrNullH = clBlack;
          // 2. Линия соединяющая нуль уровень и настоящую точку
          TColor ClrRealPointLine = clBlack;
          // 3. Сама точка.
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
     //     Проверяем ниже описанные точки PixelX, PixelY, а также конечные
     //     точки на принадлежность карте и канвасу.
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
     //     Рисуем прямоугольники начиная с пикселя
     //     PixelX = x*MAP_SCALE_X до (x+1) * PixelX*MAP_SCALE_X;
     //     аналогично для У.
     //     ЦВЕТ прямоугольников пропорционален высоте карты в этой точке.
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
     // Масштабирование
     delta*=MAP_SCALE_Z;


     DrawLandScpNet(
                   _x - radius, _y - radius,
                   _x + radius+1, _y + radius+1, true
                   );

     // Перебираем все точки квадрата со стороно 2*radius с центрем x;y
     for (int x = _x - radius; x < _x + radius; x++)
     for (int y = _y - radius; y < _y + radius; y++)
         {
     //  Проверяем текужую точку на принадлежность карты.
         if (InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,x,y,true,false,true,false))
            {
            switch (LSBrush)
                   {
                   case PICK_BRUSH:
                        {
                        //  Определяем квадрат расстояния до каждой точки от центра квадрате.
                        float r_pow_2 = fkvdrt(x - _x) + fkvdrt(y - _y);
                        //  Если расстояние не равно 0
                        if  (r_pow_2!=0)
                            Society.MAP[x][y].Z +=delta*(1/r_pow_2);
                            else
                            Society.MAP[x][y].Z += delta;
                        }break;
                   case SOFT_BRUSH:
                        {
                        float r = sqrt(fkvdrt(x - _x) + fkvdrt(y - _y));
                        // обратная экспонентальная зависимость
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



// Редактирование карты объектов
//---------------------------------------------------------------------------

void TMain::SetCurStandart()
     {
     CurStandart = MEList->ItemIndex;
     }

//---------------------------------------------------------------------------



// Управление

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





// Системные команды и методы
//---------------------------------------------------------------------------

void TMain::SaveMap(char* Path)
     {
     //  Формат хранения данных.

     //  1.  Ширина карты      - 2 кб
     //  2.  Высота карты      - 2 кб
     //  3.  Число юнитов      - 2 кб
     //  4.  Число команд      - 2 кб
     //  5.  Число эталонов    - 2 кб
     //  6.  Количество оружия - 2 кб

     //  7.  Оружее:
     //          1.Размер единичной структуры
     //          2.Информация обо всех видах оружия

     //  8.  Эталоны:
     //          1. Размер единичной структуры
     //          2. Информация о всех видах объектов:


     //  9. Карта:
     //          1. Размер единичной структуры
     //          2. Информация о всех ячейках карты ([Х][У])


     //  10.  Объекты (юниты):
     //          1. Размер единичной структуры
     //          2. Информация о всех объектах ([Команда][Индекс в команде])

     //  11. Команды:
     //          1. Размер единичной структуры
     //          2. Информация о всех командах



     // Запись в соответствии с форматом.
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


     // Закрытие файла.
     fclose(f);
     ShowMessage("Saved");
     }

bool TMain::LoadMap(char* Path)
     {
     // Формат смотри в реализации метода SaveMap.

     // Запись в соответствии с форматом.
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
     // Закрытие файла.
     fclose(f);
     }
#pragma package(smart_init)
