//---------------------------------------------------------------------------

#ifndef EditorUnitH
#define EditorUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "ConfigForm.h" // ==> #include "Main.h"
#include "MiniMap.h"

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TListBox *MEListBox;
        TPanel *ImagePanel;
        TMainMenu *MainMenu1;
        TMenuItem *MenuFile;
        TMenuItem *MenuHelp;
        TMenuItem *Exit1;
        TMenuItem *Open1;
        TMenuItem *Save1;
        TMenuItem *N1;
        TMenuItem *MenuEditMap;
        TMenuItem *MenuChTeam;
        TButton *BRefresh;
        TLabel *Label1;
        TTimer *Timer1;
        TMenuItem *ViewMenu;
        TMenuItem *ChangeMinMap;
        TTimer *MiniMapTimer;
        TLabel *Label2;
        void __fastcall MenuChTeamClick(TObject *Sender);
        void __fastcall BRefreshClick(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Button1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Button4KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Button4KeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Button2KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Button2KeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Button3KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Button3KeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall ChangeMinMapClick(TObject *Sender);
        void __fastcall ViewMenuClick(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations

        __fastcall TForm1(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------



#endif
