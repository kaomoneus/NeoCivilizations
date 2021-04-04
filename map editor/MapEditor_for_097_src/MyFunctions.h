#include "StdAfx.h"
#include "math.h"
int   round(float X);
int   roundmax (float X); // округление по максимуму
extern PACKAGE Extended __fastcall Log2(Extended X);
int   kvdrt(int x);
float fkvdrt(float x);

float Degree (float x, int exponent); // возведение в числа "х" в степень с показателем "exponent"

void  GetPoint(float DeltaX,float DeltaY,int &X,int &Y,int &SmallPoint, int &Steps);
bool  GetAngle(float &CurAngle,float Angle,float AngleSpeed,int PossiblyAngleTurn);

float StdAngl(float InputAngle);   //нормализует значение угла

void  SetAngle (float &Angle1, float Angle2  ); //мен€ет знак угла1(оставл€€ его величину такой же) на знак угла 2.

float SetNearestAngle(float *Angles,int &n,float ToAngle, float PrimaryAngle, int *Speeds,int &ReturnSpeed);

bool  InSpace (float X1,float X2,float x,bool RightScobe,bool LeftScobe);
bool  InXYSpace (float X1,float X2,float Y1,float Y2,float x,float y,bool RightScobe,bool LeftScobe,bool TopScobe,bool BottomScobe);
//определ€ет принадлежность числа х промежутку ([X1;X2]) RightScobe и LeftScobe - параметры скобок если false - скобка кругла€
bool  SetTrajTangens (float g,float Speed, float H, float D,bool Roof,float &Tangens);
float GetZ (float Y0,float Speed,float g,float X,float StartTangens);

const float PI = 3.1415926535897932384626433832795;





int round(float X)
    {
    float Drob = fmod (X,1),
    Celaoe = X - Drob;
    if (fabs(Drob) >= 0.5) if(Drob > 0) Drob = 1; else Drob = -1;
                    else Drob = 0;
    int ForReturn = Celaoe + Drob;
    return ForReturn;
    }



int kvdrt(int x)
    {
    return (x*x);
    }
float fkvdrt(float x)
    {
    return (x*x);
    }

void GetPoint(float DeltaX,float DeltaY, int &X,int &Y,int &SmallPoint, int &Steps)
     {
     float absDeltaX = fabs(DeltaX),
            absDeltaY = fabs(DeltaY),

            MinDXDY = min(absDeltaX,absDeltaY),
            MaxDXDY = max(absDeltaX,absDeltaY),
            Tangens = MinDXDY/MaxDXDY;
      if (MaxDXDY ==  absDeltaX)
         {
         Steps+=DeltaX/absDeltaX;
         if(DeltaY!=0) Y = round((abs(Steps) * Tangens)* DeltaY/absDeltaY)
             +
             SmallPoint
              ;
         X+=DeltaX/absDeltaX; //авто инкремент-декремент;

         }

         else {
              Steps+=DeltaY/absDeltaY;
              if(DeltaX!=0)X = round((abs(Steps) * Tangens) * DeltaX/absDeltaX)
                  +
                  SmallPoint
                  ;
              Y += DeltaY/absDeltaY;


              }
     }

bool GetAngle(float &CurAngle,float Angle,float AngleSpeed,int PossiblyAngleTurn)
     {
     float _CurAngle = CurAngle;
     SetAngle(_CurAngle,Angle);
     if (_CurAngle!=Angle)
        {
        float DeltaA = Angle-_CurAngle;
        if (abs(DeltaA)<AngleSpeed)
            CurAngle+=DeltaA;
            else
            CurAngle+=AngleSpeed*abs(DeltaA)/DeltaA;
        _CurAngle = CurAngle;
		SetAngle (_CurAngle,1);
        SetAngle (Angle,1);
        }

     return (fabs(Angle - _CurAngle) <= PossiblyAngleTurn);

     }
float StdAngl(float InputAngle)   // как известно общеприн€то что бы все углы были > 0 и меньше 2ѕи...
                                  // эта функци€ делает любой угол именно таким
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
            InAngle -= (-360) * n;


        }
     if ((InAngle < 0)&&(InAngle>(-360)))
        InAngle+=360;

//     if (InAngle > 180) InAngle = InAngle - 360;

     InputAngle = InAngle;

     return InputAngle;

     }

void SetAngle (float &Angle1, float Angle2  )
      {
      //если знаки углов не одинаковы:
      if ( (Angle1*Angle2) < 0 )
         {
         if (Angle2 < 0)

            Angle1 = Angle1 - 360;
            else
            Angle1 = 360 + Angle1;
         }
      //а если одинаковы - не фига их преобразовывать
      }
float SetNearestAngle(float *Angles,int &n, float ToAngle,float PrimaryAngle,int *Speeds,int &ReturnSpeed)
     {
     // 1.  –ассматриваем все углы в системе координат ’'OY'
     bool FindAbsMin = (ToAngle == PrimaryAngle)?true:false;
     bool Find_Max_Min;
     float PrimAngle = StdAngl(PrimaryAngle-ToAngle);
     struct
            {
            float value;
            int index;
            } Max_Min;
     Max_Min.value = StdAngl(Angles[0]-ToAngle);
     if (FindAbsMin)
        Max_Min.value =(Max_Min.value > 180)?(Max_Min.value - 360):Max_Min.value;

     Max_Min.index = 0;
     //если угол предпочтени€ в системе X'oY' лежит в первой четверти ищем минимальный угол иначе - максимальный
     Find_Max_Min = (InSpace(0,180,PrimAngle,true,true))?false:true;


     for (int i = 0; i<n;i++)
        {
        float CurAngle  = StdAngl(Angles[i] - ToAngle);
        if (!FindAbsMin)
         {
         // 1.  –ассматриваем все углы в системе координат ’'OY'

         //≈сли угол предпочтени€ в системе X'oY' лежит в первой четверти ищем минимальный угол иначе - максимальный
         if (Find_Max_Min)
            {
            if (Max_Min.value < CurAngle)
               {
               Max_Min.value = CurAngle;
               Max_Min.index = i;
               }
            }
            else
               if (Max_Min.value > CurAngle)
                  {
                  Max_Min.value = CurAngle;
                  Max_Min.index = i;
                  }
         }
         else {
              CurAngle = (CurAngle > 180)?(CurAngle - 360):CurAngle;
              if (fabs(Max_Min.value) > fabs(CurAngle))
                 Max_Min.value = CurAngle;
              }
        }
      // !! ¬ј∆Ќќ  !!
      ReturnSpeed = Speeds[Max_Min.index];
      Angles[Max_Min.index] = Angles[n-1]; //найденный угол удал€ем
      Speeds[Max_Min.index] = Speeds[n-1];
      n--;
      return StdAngl(Max_Min.value + ToAngle);

     }

bool InSpace (float X1,float X2,float x,bool RightScobe,bool LeftScobe)
     {
     bool biggerX1 = false,smalestX2 = false;
     if (RightScobe)
        {if (X1<=x) biggerX1 = true;}
        else
        {if (X1<x) biggerX1 = true;}
     if (LeftScobe)
        {if (X2>=x) smalestX2 = true;}
        else
        {if (X2>x) smalestX2 = true;}
     return (biggerX1 && smalestX2);
     }
bool InXYSpace (float X1,float X2,float Y1,float Y2,float x,float y,bool RightScobe,bool LeftScobe,bool TopScobe,bool BottomScobe)
     {
     return (InSpace (X1,X2,x,RightScobe,LeftScobe)&&InSpace(Y1,Y2,y,TopScobe,BottomScobe));
     }

bool SetTrajTangens (float g,float Speed, float H, float D,bool Roof,float &Tangens)
     {
     //область допустимых значений
     bool Result = (g!=0);
     if (!Result) return Result;



     float Angle = ((fkvdrt(Speed)*H)-((-g)*fkvdrt(D))) / (fkvdrt(Speed)*sqrt(fkvdrt(H)+fkvdrt(D)));
     if (fabs(Angle)>1)
        return false;

     Angle = (Roof)?(PI - asin(Angle) + atan(H/D))/2:(asin(Angle) + atan(H/D))/2;

     Tangens = tan(Angle);


     return true;
     }


float GetZ (float Y0,float Speed,float g,float X,float StartTangens)
           {
           float SpeedOX      = sqrt(fkvdrt(Speed)/(float(1)+fkvdrt(float(StartTangens))));

           float StartSpeedOY = SpeedOX*StartTangens;
           float T            = X/SpeedOX;

           return Y0+(StartSpeedOY*T) - (g*fkvdrt(T)/2);
           }










