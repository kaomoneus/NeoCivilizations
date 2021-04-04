//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MiniMap.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMiniMapForm *MiniMapForm;
//---------------------------------------------------------------------------
__fastcall TMiniMapForm::TMiniMapForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
