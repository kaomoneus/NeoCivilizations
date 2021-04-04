//---------------------------------------------------------------------------
#include "MainUnit.cpp" // ==> #include "EditConstants.h"
#include "MapEditConst.h"

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
char* GetWord(char firstsym,char secsymb,char* string,char* instring)        // ������� �����
                                                                             // ������������ �����
                                                                             // ����� ���������
      {
      int si = 0;
      char buf ='\0';

      for (si = 0;(string[si]!=firstsym)&&(si<STRING_LENGTH);si++);
      si++; // firstsym

      for (int i = 0;(i<WORD_LENGTH)&&(buf!=secsymb);i++)
          {
          buf = string[si+i];
          instring[i] = (buf!=secsymb)?buf:'\0';
          }
      return instring;
      }
char* GetValue(char* string,char* instring)                                  // ������� � ������ ��������
      {
      char buf = '\0';
      int si = 0;
      //1. ������� ������ �� ������� "=".
      while ((si<STRING_LENGTH)&&(string[si-1]!='='))
            si++;

      //2. ����� ���������� ��� ������� "_".
      while ((si<STRING_LENGTH)&&(string[si-1]!=' '))
            si++;


      //3. ��������� ��� �� ������� ";".
      int i;
      for(i = 0;(buf!=';')&&(i<WORD_LENGTH);i++)
         {
         buf = string[si+i];

         // ��������� � ������������ �������� ������ �����
         if (buf!=';')
             instring[i] = buf;
             else
             instring[i] = '\0';
         }


      return instring;
      }

bool eqwords(const char* word1,const char* word2)                            // ���������� �����
     {
     bool result = true;
     for (int i = 0;(i<WORD_LENGTH)&&(result);i++)
         {
         if (word1[i]!=word2[i])
            result = false;
         if (word1[i] == 0)
            break;
         }
     return result;
     }
                                                                             // �������� ���� ����� � ������
                                                                             // � ���������� ������
//---------------------------------------------------------------------------

const char* SYNTAX_ERROR = "�������������� ������.";

class TMain
{
public:
// ����

   // ��������������� ����
TListBox* MEList;                        // ������ � ���-����
TCanvas*  Canvas;                        // ����� ��� ���������
TForm*    MiniMap;                       // �����-�����

   // ��������������� ����
int CurX,CurY;                           // ��������� ������ (� ������ ������������
                                         // ������� ��������� ����������
                                         // - ��� ������� ����� ���� ������)
short int StandartsNum;
short int WeaponsNum;

   // ��������������� ����
TGSociety Society;                       // ���� ���������� ������ ����.
TGStandart* Standart;                    // ���� ��������.
TWeapon* Weapon;                         // ������ (������).

   // ������� ������� ������ �����, �� ��� �������, ��� ��� ���������� � �������
   // Society.

   // ���� ��������������� ��� ��������������
   TChangeType CurList;                     // ������� ��� ������.
   int CurTeam;                             // ������������� �������.
   int CurStandart;                         // ������� ��������� ���.

   enum TEditMode {EDIT_LAND_SCAPE, EDIT_PLACE ,EDIT_OBJECT_MAP};
   TEditMode EditMode;
   bool MouseDown;
   int LastMapX,LastMapY,
       MapX,MapY;

   //                   �������������� ���������.
                        enum TLSBrush {PICK_BRUSH,SOFT_BRUSH,HARD_BRUSH};
                        TLSBrush LSBrush; 
                        int BrushRadius;
                        int BrushDelta; 


// ��������������� (������������)
// ������������
TMain();
void Init(TListBox* _MEList,TCanvas* _Canvas);
void MiniMapAssign (TForm* _MiniMap);
// ������.

   // ������������� ������ � ��������.
bool LoadIniFile(char* Path);
     bool ApplyValue(char* Name, char* Value,int Type_of_type,int index);
                                             // �������� �������� � ����,
                                             // ��������������� ��������, � ���� �����

   // �������������/���������� ������.
   void DrawMap(int SX, int SY, int EX, int EY);
        void DrawMiniMap();
        void DrawLandScape(int SX, int SY, int EX, int EY);    // ������ �������� � ����-�����.
        void DrawObjects();      // ������ �������.

        void DrawLandScpNet(int SX,int SY,int EX,int EY, bool Delete);   // ������ ���� ��� ���������.
             int k;
             void DrawH();       // �������� ����� �� ������� ��������� ����
                                 // ������� ���������� MapX � MapY.


   // �������������� ���������.
   void OnMouseDown();
   bool OnMouseMove(int MAP_X,int MAP_Y);    // � ����� ���������� ����������
                                             // ������ �����.

   void OnMouseUp();                         // ����� �� ��������� ������ ����,
                                             // ��� �������� ������������.


   void EditLandScp(int _x, int _y, float delta,int radius);


   // �������������� ����� ��������
   void SetCurStandart();


void RefreshMEList();
   // ��������� ������� � ������
   void SaveMap(char* Path);     // ���������� �����.
   bool LoadMap(char* Path);     // �������� �����.
};





#endif
