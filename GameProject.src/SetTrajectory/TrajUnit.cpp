//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TrajUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
Form1->Width  = FormWidth;
Form1->Height = FormHeight;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BDrawClick(TObject *Sender)
{
float Tangens;
float DrawY;

if (!SetTrajTangens(Speed,g,Height,Distance,Tangens))
   {
   ShowMessage("«аданна€ скорость непозвол€ет стрел€ть");
   return;
   }

for (float i = 0; i<=Distance; i+=1)
    {
    DrawY = FormHeight - GetAbsciss(Form1->Width-_Y,g,Speed,Tangens,_X+i);
    Canvas->Pixels[_X+i][DrawY] = clBlack;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
ClickNumber = (ClickNumber <= 2)?ClickNumber+1:1;
if (ClickNumber == 1)
   {
   _X = X, _Y = FormHeight - Y;

   Canvas->Pixels[X][Y] = clRed;
   }
   else
       {
       TargetX  = X;
       TargetY  = FormHeight - Y;
       Height   = TargetY - _Y;
       Distance = TargetX - _X;

       Canvas->Pixels[X][Y] = clBlue;
       };
        
}
//---------------------------------------------------------------------------
