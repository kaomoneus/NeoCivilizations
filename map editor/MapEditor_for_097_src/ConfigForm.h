//---------------------------------------------------------------------------

#ifndef ConfigFormH
#define ConfigFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Main.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TCfgForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *LTeamNum;
        TEdit *ETeam;
        TButton *BTeamNum;
        TButton *BClose;
        TLabel *Label1;
        TGroupBox *CfgBrush;
        TRadioButton *BPick;
        TRadioButton *BSoft;
        TRadioButton *BHard;
        TUpDown *RadiusUpDown;
        TEdit *BRadius;
        TLabel *Label3;
        TLabel *Label4;
        TEdit *BDelta;
        TUpDown *UpDown1;
        TGroupBox *GroupMode;
        TRadioButton *LSMode;
        TRadioButton *PlaceMode;
        TRadioButton *ObjectsMode;
        void __fastcall BTeamNumClick(TObject *Sender);
        void __fastcall BCloseClick(TObject *Sender);
        void __fastcall FormDeactivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TCfgForm(TComponent* Owner);
        TMain* PMainData;
};
//---------------------------------------------------------------------------
extern PACKAGE TCfgForm *CfgForm;
//---------------------------------------------------------------------------
#endif
