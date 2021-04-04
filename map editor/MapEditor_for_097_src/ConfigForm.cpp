//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ConfigForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCfgForm *CfgForm;
//---------------------------------------------------------------------------
__fastcall TCfgForm::TCfgForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCfgForm::BTeamNumClick(TObject *Sender)
{
// Команды
PMainData->CurTeam = StrToInt(ETeam->Text);
if (NUMBER_OF_TEAMS <= PMainData->CurTeam)
NUMBER_OF_TEAMS = PMainData->CurTeam+1;

// Режимы редактора
if (LSMode->Checked) PMainData->EditMode = TMain::EDIT_LAND_SCAPE; else
if (PlaceMode->Checked) PMainData->EditMode = TMain::EDIT_PLACE; else
if (ObjectsMode->Checked) PMainData->EditMode = TMain::EDIT_OBJECT_MAP;

// Кисть для редактора ландшафта
if (PMainData->EditMode == TMain::EDIT_LAND_SCAPE)
   {
    if (StrToInt(BRadius->Text) > 0)
       {
       PMainData->BrushRadius = StrToInt(BRadius->Text);
       PMainData->BrushDelta  = StrToInt(BDelta->Text);

       bool _true_ = true;
              if ( BPick->Checked) PMainData->LSBrush = TMain::PICK_BRUSH; else
              if ( BSoft->Checked) PMainData->LSBrush = TMain::SOFT_BRUSH; else
              if ( BHard->Checked) PMainData->LSBrush = TMain::HARD_BRUSH;

       }
       else ShowMessage(BRUSH_ERROR);
    }
    PMainData->DrawMap(
                      PMainData->CurX, PMainData->CurY,
                      FLAT_WIDTH + PMainData->CurX, FLAT_HEIGHT + PMainData->CurY
                      );

}
//---------------------------------------------------------------------------

void __fastcall TCfgForm::BCloseClick(TObject *Sender)
{
this->Close();
}
//---------------------------------------------------------------------------


void __fastcall TCfgForm::FormDeactivate(TObject *Sender)
{
Visible = false;
}
//---------------------------------------------------------------------------



