//---------------------------------------------------------------------------

#ifndef MiniMapH
#define MiniMapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TMiniMapForm : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
        __fastcall TMiniMapForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMiniMapForm *MiniMapForm;
//---------------------------------------------------------------------------
#endif
