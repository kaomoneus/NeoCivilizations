//---------------------------------------------------------------------------
bool MiniMove = false;


#include <vcl.h>
#pragma hdrstop

#include "EditorUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TMain MainData;

bool Up = false,Down = false,Left = false,Right = false;


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
Label2->Caption = IntToStr(sizeof(short int));
MainData.Init(MEListBox,Canvas);

MainData.Society.Standart[0] = MainData.Standart[0];
MainData.Society.GSocietyInit(true);

MainData.RefreshMEList();

//MainData.Society.LoadMap();

}



void __fastcall TForm1::MenuChTeamClick(TObject *Sender)
{
// 1. Передаем указатель на все "глобальные" данные
CfgForm->PMainData = &MainData;
// 2. Передаем текущие значения

// Кисть
   switch (MainData.LSBrush)
          {
          case TMain::PICK_BRUSH: CfgForm->BPick->Checked = true; break;
          case TMain::SOFT_BRUSH: CfgForm->BSoft->Checked = true; break;
          case TMain::HARD_BRUSH: CfgForm->BHard->Checked = true; break;
          }

   CfgForm->BRadius->Text = IntToStr(MainData.BrushRadius);
// 3. Делаем видимой форму настроек
CfgForm->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BRefreshClick(TObject *Sender)
{
MainData.DrawMap(
                MainData.CurX, MainData.CurY,
                FLAT_WIDTH + MainData.CurX, FLAT_HEIGHT + MainData.CurY
                );

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
Label1->Caption = IntToStr(Key);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Timer1->Enabled = true;
Up = true;
}
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
//MainData.CurX = (MainData.CurX<MAP_WIDTH)?MainData.CurX+1:MainData.CurX;
MainData.DrawMap(
                MainData.CurX, MainData.CurY,
                FLAT_WIDTH + MainData.CurX, FLAT_HEIGHT + MainData.CurY
                );

Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
Timer1->Enabled = true;
Down = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
Timer1->Enabled = false;
Down = false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button2KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
Timer1->Enabled = true;
Left = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
Timer1->Enabled = false;
Left = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
Timer1->Enabled = true;
Right = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
Timer1->Enabled = false;
Right = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
      MainData.OnMouseUp();
      MainData.OnMouseDown();
// 1. Проверяем если машка попала на мини карту.
   if (InXYSpace(
                MINI_DRAW_X, MINI_DRAW_X+MAP_WIDTH,  MINI_DRAW_Y, MINI_DRAW_Y+MAP_HEIGHT,
                X,Y,true,false,true,false
                )
      )
      {
      int
      MoveToX = (X-MINI_DRAW_X),//MAP_SCALE_X,
      MoveToY = (Y-MINI_DRAW_Y);//MAP_SCALE_Y;
// 1.1. То передвинуть центр экрана в указанное место место.
      MainData.CurX = MoveToX - (FLAT_WIDTH/2);
      MainData.CurY = MoveToY - (FLAT_HEIGHT/2);
// 1.2. Пока клавиша нажата разрешено двигать центр экрана.
      MiniMove = true;
// 1.3. Рисуем карту.
      MainData.DrawMap(
                      MainData.CurX, MainData.CurY,
                      FLAT_WIDTH + MainData.CurX, FLAT_HEIGHT + MainData.CurY
                      );

      }
// 2. Если мышка упала на область карты

/*   if (
      InXYSpace(
               START_DRAW_X, START_DRAW_X + (FLAT_WIDTH*MAP_SCALE_X),
               START_DRAW_Y, START_DRAW_Y + (FLAT_HEIGHT*MAP_SCALE_Y),
               X,Y,true,false,true,false
               )
      )
*/
      {
// 2.1. Определяем на какую ячейку наведена мышь
      int _X_ = X - START_DRAW_X;
      int _Y_ = Y - START_DRAW_Y;
      int k  = (_Y_)*0.1;
      _X_ =(MainData.EditMode == TMain::EDIT_LAND_SCAPE)?_X_+k:_X_;
      _X_ /= MAP_SCALE_X;   //  целочисленно деление
      _Y_ /= MAP_SCALE_Y;

      _X_ += MainData.CurX;
      _Y_ += MainData.CurY;

      if (MainData.OnMouseMove(_X_,_Y_))
// 2.3. Перерисовывем ладншафт.
         {
         if (MainData.EditMode == TMain::EDIT_LAND_SCAPE)
            MainData.DrawH();
         }
      }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
// 1. Если разрешено двигать центр экрана.
//    то запретить.
      MiniMove = false;

      MainData.OnMouseUp();

      MainData.DrawMap(
                      MainData.CurX, MainData.CurY,
                      FLAT_WIDTH + MainData.CurX, FLAT_HEIGHT + MainData.CurY
                      );

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
// 1. Если в данный момент кнопка мыши нажата на мини-карте
   if (MiniMove)
      {
// 1.1. Проверяем все необходимые условия,
// 1.1.1. Проверяем если машка попала на мини карту.
   if (InXYSpace(
                MINI_DRAW_X, MINI_DRAW_X+MAP_WIDTH,  MINI_DRAW_Y, MINI_DRAW_Y+MAP_HEIGHT,
                X,Y,true,false,true,false
                )
      )
      {
// 1.2.1 Если условия удовлетворены - меняем положение экрана.
      int
      MoveToX = (X-MINI_DRAW_X),//MAP_SCALE_X,
      MoveToY = (Y-MINI_DRAW_Y);//MAP_SCALE_Y;

      MainData.CurX = MoveToX - (FLAT_WIDTH/2);
      MainData.CurY = MoveToY - (FLAT_HEIGHT/2);
// 1.2.2.3. Рисуем карту.
      MainData.DrawMap(
                      MainData.CurX, MainData.CurY,
                      FLAT_WIDTH + MainData.CurX, FLAT_HEIGHT + MainData.CurY
                      );
      }
      }

// 2. Редактирование карты.
      {
// 2.1. Определяем на какую ячейку наведена мышь
      int _X_ = X - START_DRAW_X;
      int _Y_ = Y - START_DRAW_Y;
      int k  = (_Y_)*0.1;

      _X_ = (MainData.EditMode == TMain::EDIT_LAND_SCAPE)?_X_+k:_X_;
      _X_ /= MAP_SCALE_X;   //  целочисленно деление
      _Y_ /= MAP_SCALE_Y;

      _X_ += MainData.CurX;
      _Y_ += MainData.CurY;
      if (MainData.OnMouseMove(_X_,_Y_))
// 2.3. Перерисовывем ладншафт.
         {

         if (MainData.EditMode == TMain::EDIT_LAND_SCAPE)
            MainData.DrawH();

         }
         else
         if (MainData.EditMode == TMain::EDIT_LAND_SCAPE)
            MainData.DrawH();
      }


}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChangeMinMapClick(TObject *Sender)
{
if(!MiniMapForm->Visible)
  MiniMapForm->Visible = true;
  else
      MiniMapForm->Visible = false;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------



void __fastcall TForm1::ViewMenuClick(TObject *Sender)
{
if (!MiniMapForm->Visible)
   Form1->ChangeMinMap->Caption = "Mini map ON";
   else
   Form1->ChangeMinMap->Caption = "Mini map OFF";

}
//---------------------------------------------------------------------------







void __fastcall TForm1::Save1Click(TObject *Sender)
{
char* path = new char [13];
path = "firstmap.map";
MainData.SaveMap(path);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Open1Click(TObject *Sender)
{
char* path = new char [13];
path = "firstmap.map";
MainData.LoadMap(path);
MainData.Society.LoadConfig();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormActivate(TObject *Sender)
{
MainData.DrawMap(MainData.CurX, MainData.CurY, MainData.CurX+FLAT_WIDTH, MainData.CurY+FLAT_HEIGHT);
}
//---------------------------------------------------------------------------

