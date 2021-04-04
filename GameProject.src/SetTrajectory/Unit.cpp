//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
float _X = 0;
float _Y = 0;
float  H = 0;
float  g = 10;
float  D = 0;
float Tg = 0;
float  V = 100;
float TX = 0;
float TY = 0;

int  NOC = 0;


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
NOC = (NOC <= 1)? NOC+1:1;
if (NOC == 1)
   {
   _X = X;
   _Y = Form1->Height - Y;
   Canvas->Pixels[X][Y] = clRed;
   }
   else
       {
       TX = X;
       TY = Form1->Height - Y;
       D  = TX - _X;
       H  = TY - _Y;
       Canvas->Pixels[X][Y] = clBlue;
       if (!SetTrajTangens(g,V,H,D,0,Tg))
          ShowMessage("Скорость мала для такого расстояния.");
       }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
for (float i = 0;i<=41;i+=(30/9))
    {
    Tg = 4.36110;
    V  = 30;
    g  = 9.8;
    float DrawY = Form1->Height - GetZ(_Y,V,g,i,Tg);
    Canvas->Pixels[_X + i][round(DrawY)] = clBlack;
    }
}
//---------------------------------------------------------------------------

