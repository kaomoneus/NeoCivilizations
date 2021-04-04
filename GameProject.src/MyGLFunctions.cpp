#include "StdAfx.h"
#include <math.h>
#include <stdlib.h>



bool PointInRect(int X, int Y, RECT Rect)
{
	if((X>=Rect.left)&(X<=Rect.right)&
	   (Y>=Rect.bottom)&(Y<=Rect.top))
	    return true;
	else
		return 	false;

}


int max4i(int a1, int a2, int a3, int a4)
{
	if(a1<a2) a1 = a2;
	if(a1<a3) a1 = a3;
	if(a1<a4) a1 = a4;
	
	return a1;
};

char* ReadLine(HFILE File)
{
 int j=0;
 char* Str;
 while(Str[j]!=13)
  {
   _lread(File,&Str[j],sizeof(char));
   j++;
  }
 return Str;
}

float StdAngle(float InputAngle)   // как известно общепринято что бы все углы были > 0 и меньше 2Пи...
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

int Random(int Max)
{
	return int(rand()*Max/RAND_MAX);
}

