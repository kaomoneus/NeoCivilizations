//---------------------------------------------------------------------------

#ifndef TrajUnitH
#define TrajUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MyFunctions.h"
//---------------------------------------------------------------------------
float _X = 50, _Y = 50;
float TargetX     = 150,
      TargetY     = 50;
float Speed       = 10,
      g           = 10,
      Height      = TargetY - _Y,
      Distance    = TargetX - _X;
int   ClickNumber = 0;
int FormWidth  = 640;
int FormHeight = 480;

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TButton *BDraw;
        void __fastcall BDrawClick(TObject *Sender);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 