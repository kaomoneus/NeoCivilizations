//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

USERES("MapEditor.res");
USEUNIT("Main.cpp");
USEFORM("ConfigForm.cpp", CfgForm);
USEFORM("MiniMap.cpp", MiniMapForm);
USEFORM("EditorUnit.cpp", Form1);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TMiniMapForm), &MiniMapForm);
                 Application->CreateForm(__classid(TCfgForm), &CfgForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
