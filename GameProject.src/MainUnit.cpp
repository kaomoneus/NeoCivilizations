

//---------------------------------------------------------------------------
#include "StdAfx.h"


/*
��� �������� ������� c �������� ������ � ������� ��������

...
������� ����� ��������...



�� ������ ���� ���������� ��� ��������� �������������� �������:
1. ������� sqrt( (double) );
*/




#include <stdlib.h>
//#include <vcl.h>
#pragma hdrstop
#include "Stdio.h"
#include <dos.h>
#include <stdlib.h>
//#include <ctime.h>
#include "MainUnit.h"
#include "MyFunctions.h"


short int NUMBER_OF_UNITS = 50;
short int NUMBER_OF_GROUPS = 25;
short int NUMBER_OF_TEAMS = 1;
short int NUMBER_OF_STANDARTS = 5;
short int NUMBER_OF_WEAPONS = 5;

//----------------------TSection------------------------------------------------
//------------------------------------------------------------------------------
 // ������������� ������, ������ ���������.
bool TSection::Init(TGSociety *_Society, TGObject* _OurObject,bool Oldest)
     {
     //1. ��� ��� ������� � ����������
     Generation                 = 0;
     Open                       = false;
     MaxExistingGeneration      = 0;
     MaxGeneration              = log10( (double )min(MAP_WIDTH,MAP_HEIGHT))/log10( 2. );
     ScionsNumber               = 0;
     OldestSection              = (Oldest)?this:NULL;
     for (int x = 0; x<2; x++)
     for (int y = 0; y<2; y++)
         Daughter[x][y]         = NULL;

     // 2. ���������� ������ � ������.
     SX  = 0;
     EX  = (Oldest)?MAP_WIDTH-1:0;
     SY  = 0;
     EY  = (Oldest)?MAP_HEIGHT-1:0;
     X   = (Oldest)?round(float(EX-SX)/2):0;
     Y   = (Oldest)?round(float(EY-SY)/2):0;

     // 3. "������� ���".
     Society = _Society;
     OurObject = _OurObject;


     // 4. ��� ��� ������� � ��������
     Creeked                    = false;
     FromSection                = NULL;
     Steps                      = (Oldest)?0:-1;
     SectionWithA               = NULL;
     SectionWithB               = NULL;
     SectionWithA_B             = NULL;
     SectionWithObject          = (Oldest)?this:NULL;
     Occupied = (Oldest)?!CheckMap(true,-1,-1):false;
     // 2.1. ������ � � �
     A = NULL;
     B = NULL;

     return true;

     }

bool TSection::FingerAllSections(bool EInit, bool CheckOnAB,bool FindObject/*,TCanvas* Canvas*/)
     // ��. "�" ����.
     {
     // 1. ������� ������ ���������������� ������ IList.
     TGList IList;
     // 1.1. ����������� ��������� ����� ������ 2^((MaxGeneration+1)*2)
     IList.TotalNum = pow((double)(2),(double)((OldestSection->MaxExistingGeneration+1))*2);
     IList.Section  = new TSection* [IList.TotalNum];
     IList.Num       = 0;
     // 2. ��������� � ������ IList ��� ������.
     AddSection(&IList,this);
     // 3. ������������� FOR ������ �� ����� ������.
     for (int i = 0; i<IList.Num; i++)
     //    ������
         {
     // 3.1. ���� ������ �������� � ����� � � ����� �
     //            � ���� ��� ������ ��������������� ������ ���������� � ����� ������ ������
     //            ��� ���� � ����� ������ ������ ������ ������ �� ��������
                   if (
                       (IList.Section[i]->B!=NULL)&&(IList.Section[i]->A!=NULL)&&
                        (
                        (OldestSection->SectionWithA_B==NULL)||
                        (OldestSection->SectionWithA_B->Generation<IList.Section[i]->Generation)
                       )
                      )
     //             ������� � ��������������� ���� ����� ������ ������ ��� ������
                      OldestSection->SectionWithA_B = IList.Section[i];
     // 3.2. ���� ������ ������� ������� �� � �������� � ����� �� ��������.
         if (IList.Section[i]->Open)
            {
            TSection* CurS = IList.Section[i];
            DelFromList(&IList,i);

            for (int x = 0; x<2; x++)
            for (int y = 0; y<2; y++)
                {
     // 3.2.0 ���� ���������, ��������� ������������� ������� ������ (��� ��������).
                if (EInit)
                CurS->Daughter[x][y]->EasyInit();
                CurS->Daughter[x][y]->Occupied = !CurS->Daughter[x][y]->CheckMap(true,-1,-1);


     // 3.2.1. � ������ ������ ��������� �������������� ����������
     // 3.2.1.1.0. ���� ����� ����� �������� ������ � ��������.
                if (FindObject)
                   {
     // 3.2.1.1.0.1. ���� ������ ������� � ���������� ������� ����� ������ ������
                   if (
                      (!CurS->Daughter[x][y]->Open)
                      &&
                      InXYSpace(
                                 CurS->Daughter[x][y]->SX,CurS->Daughter[x][y]->EX,
                                 CurS->Daughter[x][y]->SY,CurS->Daughter[x][y]->EY,
                                 OurObject->X,OurObject->Y,true,true,true,true
                                 )
                      )
                      {
     // 3.2.1.1.1.1. ������� ���� �������� ������, ��� ��� �������� ������.
                      CurS->Daughter[x][y]->SectionWithObject = CurS->Daughter[x][y];
     // 3.2.1.1.1.1. �������� ������ ����� ������ ������ �� ��� �������� ������.
                      OldestSection->SectionWithObject = CurS->Daughter[x][y];
                      }

                   }

     // 3.2.1.3.0. ���� ������� ���� CHECK ON AB
                if (CheckOnAB)
                   {
     // 3.2.1.3. ���� � ����� ������ ������ ������������������ ����� � ��� �,
     // 3.2.1.3.1. ��� ������ �.
     //            ����� �������� ������ � ������� ��������� �� ��� ���� ����������,
                 if (
                    (OldestSection->A!=NULL)
                    &&
                    InXYSpace(CurS->Daughter[x][y]->SX,CurS->Daughter[x][y]->EX,
                              CurS->Daughter[x][y]->SY,CurS->Daughter[x][y]->EY,
                              OldestSection->A->X,OldestSection->A->Y,true,true,true,true
                              )
                    )
                    {
      //          � �������� ����������.
                    CurS->Daughter[x][y]->A = OldestSection->A;
                       OldestSection->SectionWithA = CurS->Daughter[x][y];
                    }
     // 3.2.1.3.2. ��� ������ �.
     //          ����� �������� ������ � ������� ��������� �� ��� ���� ����������
                 if (
                    (OldestSection->B!=NULL)
                    &&
                    InXYSpace(CurS->Daughter[x][y]->SX,CurS->Daughter[x][y]->EX,
                              CurS->Daughter[x][y]->SY,CurS->Daughter[x][y]->EY,
                              OldestSection->B->X,OldestSection->B->Y,true,true,true,true
                              )
                    )
     //          � �������� ����������.
                    {
                    CurS->Daughter[x][y]->B = OldestSection->B;
                       OldestSection->SectionWithB = CurS->Daughter[x][y];
                    }
                   }

     //          ���� ������ ���������� ������������� ���������� ������ - �������� �������������
                if (!AddSection(&IList,CurS->Daughter[x][y]))
                   return false;
                }
            // 3.3. ��� ��� FOR �������� ������� ������ � ��� ���� ������������������� ��,
            //      ��� ��������� �� ����� ��������� �� ������ ������
            //      �� ���������� FORy �������� ������� �����.
            i--;
            }
//     "���������"
//            else
//            {
//            Canvas->Brush->Color = (IList.Section[i]->Occupied)?((IList.Section[i]->Creeked)?clRed:clWhite):
//                                   (IList.Section[i]->Creeked)?clBlue:(clBlue-128);
//            Canvas->RoundRect(IList.Section[i]->SX,IList.Section[i]->SY,IList.Section[i]->EX,IList.Section[i]->EY,2,2);
//            }
//     ����� "���������"
         }
     // 4. ������ �������� ��� ���� ������ ����� ���������� ��������� � ������.
     OldestSection->ScionsNumber = IList.Num;
     // 5. ������� ������ ���������� ��� ������.
     delete[] IList.Section;
     // 6. ���������� ������������� ���������.
     return true;
     }

bool TSection::EasyInit()       // ������������� �����, ���������� �� �������.
     {
     if (this == NULL)          // ��������
        return false;
     Creeked        = false; // ������ �� ������.
     FromSection    = NULL;  // �� ����� ������ ������ ��� ������.
     SectionWithA_B = NULL;  // ����� ������� ������, ������� �������� ����� � � � ������������.
     Steps          = 0;     // ����������, ������� ����� ������ �� ������ ������� �� ���� ������.
     if (this!=OldestSection)
        {
        B = NULL;
        A = NULL;
        }

     return true;
     }

bool TSection::DelGeneration(int G)
     {
     // ������� ��� ��������� G, ���� ��� ��������,
     // ������� ������ ���������� ��� ��������� ������.
     // ��������� G ������ ���� ������ ��������� ���� ������.
     if (G>OldestSection->MaxExistingGeneration)
        return false;
     if (G<=Generation)
        return false;
     // 1. ��������� � ������ ���� ������, ���������� ��������� ������ ������.
     // 1.1. ����������� ������.
     TGList List;
     // 1.1.1. ����� ��������� ������ �������������� ����� ���������� �� �������
     //                       MaxExistGeneration
     //        ������������ �������� S  (2^[(g - Generation)*2])dg) =
     //                          Generation
     //
     //        = [(2^((MaxExistGeneration-Generation)*2)) - 2]/ln(2)
     //        ���� ������ �������������� ������ ��� ����� �����, �� ��
     //        �� �����������.

     List.TotalNum = ceil((pow((double)(2),(double)((OldestSection->MaxExistingGeneration - Generation))*2) - 2)/log((double)2));
     List.Num      = 0;
     List.Section  = new TSection*[List.TotalNum];
     // 1.2. ���������� � ������ ���� ������.
     AddSection(&List,this);
     // 1.3. ����������� ����� ������� �� ������ ���������� �������� ������(�����������)
     int i;
     for (i = 0; i<List.Num;i++)
     // 1.3.1. ���� ������� ������ (����� ������ �) �������,
     //        �������� � ������ ���� �� ��������.
         if (List.Section[i]->Open)
            for (int x = 0;x<2;x++)
            for (int y = 0;y<2;y++)
                AddSection(&List,List.Section[i]->Daughter[x][y]);

     // 2. ����������� ���� ��������� �� ������� ���� ������ � ������, � ��������� �������
     //    ������ � ����� �� ��� ��������� G.
     // 2.1. ����������� ����� ������������� ��� �������� �����,

     for (i = 0; i<List.Num; i++)
     // 2.1.1. ���� ��������� ������� ������ ������ ��� ����� �� ��� G
         {
         if (List.Section[i]->Generation>=G)
     // 2.1.1.1. �������� ������ ���������� ��� ��� ������.
            {
            delete List.Section[i];
            List.Section[i] = NULL;
            }
     // 2.1.2. ���� ��������� ������� ������ G-1 (������ ��� �� 1 ������ ��������� ���������).
     // 2.1.2.1. ���� ������ �������, ������� ������.
            else if ( (List.Section[i]->Generation == (G-1)) && List.Section[i]->Open )
                    {
                    for (int x = 0;x<2;x++)
                    for (int y = 0;y<2;y++)
                        List.Section[i]->Daughter[x][y] = NULL;
                    List.Section[i]->Open = false;
     // 2.1.2.1.1. ���� � ����������� ������ ���� ��������� �� �, ��� ���������� �������� �������,
                   if (List.Section[i]->A != NULL)
                      {
     //            ����������� ����� �.
     // 2.1.2.1.1.1. ���������: ����������� ������ ��������������� ������.
     //                 List.Section[i]->SectionWithA = List.Section[i];
     // 2.1.2.1.1.2. �������� �������� ���� "������ � ������� �" � ����� ������ ������.
                      OldestSection->SectionWithA = List.Section[i]->SectionWithA;
                      }
     // 2.1.2.1.2. ���������� � ������� �.
                   if (List.Section[i]->B != NULL)
     //               {
     ///*���������:*/ List.Section[i]->SectionWithB = List.Section[i];
                      OldestSection->SectionWithB = List.Section[i]->SectionWithB;
     //               }
                    }
     // 2.2. ������������ ���������� � ������� (bool EasyInit).
     //       ���� ������ ���� ������� EasyInit ������ ������ ����. 
         List.Section[i]->EasyInit();
         }
     // 3. ��������� ��� ������ OldestSection->FingerAllSections (��� ������������� � ��� �������� ������� � � �)
     OldestSection->FingerAllSections(false,false,false);
     // 4. �����.
     // 4.1. �������� ������, ���������� ��� ������.
     delete[] List.Section;
     // 4.2. ���������� ���������.
     return true;
     }


// ���������� ��� ���������� �� �� �������� ������ ������ ��
// ��������. ���� ��� ����������� �������� �� ������������
// false ���� �������� �� true
bool TSection::SplitSection()
     {
     // 1. �������� �� ���������� ��� ��������.
     //    �������� �� ���������.

     // 2. ���������� ������.
     TGList GList;
     // 2.1. ������������ ����� ��������� � ������ ������������ �� �������:
     //      2^((MaxExistingGeneration+1)*2)
     GList.TotalNum = pow((double)(2),(double)((OldestSection->MaxExistingGeneration+1))*2);
     GList.Section  = new TSection* [GList.TotalNum];
     GList.Num = 0;
     // 3. � ������ �������� ��� ������.
     AddSection(&GList,this);
     // 4. ���������� FOR ������ �� ������ �� ����� ������.
     for (int i = 0; i<GList.Num; i++)
     //    ������
         {
     // 4.1. ���� ������ �������, �� ������� �� �� ������ �������� � ����� ������ �� �������.
         if (GList.Section[i]->Open)
            {
            TSection* CurS = GList.Section[i];
            DelFromList(&GList,i);
            for (int x = 0; x<2; x++)
            for (int y = 0; y<2; y++)
                AddSection(&GList,CurS->Daughter[x][y]);
     // 4.1.1. ������� �������� ����� - � FOR ����� ������� ������.
            i--;
     // 4.1.2. ��������� "������".
            continue;

            }
     // 4.2. ���� ������ ������� � ������ ������ ���� ����������� � ��� �� ����������, ��
         if ((!GList.Section[i]->Open) && (GList.Section[i]->Occupied) &&
                                          (GList.Section[i]->Generation<OldestSection->MaxGeneration))
     //      ������
            {
     // 4.2.1. ������ ������ ��������.
            GList.Section[i]->Open = true;
     // 4.2.1.1. ���� ���� ��� ������ ��������� ��� ������ �� �������� �������� - ��� �������,
     //          ������� �� ����� ����� ������� �������� �� ��  �������
            OldestSection->ScionsNumber--;
     //          �� � ��� ��������� ������ ���������� �����, ������� �� ���� ��������� � �����
     //          �����
            OldestSection->ScionsNumber+=4;
     // 4.2.1.2. ���� ����������� ������ ���� ����� �� ����� �������,
     //          �� ������� � ��������� ��� ����� �������� ���������.
            if (OldestSection->MaxExistingGeneration==GList.Section[i]->Generation)
            OldestSection->MaxExistingGeneration++;
     // 4.2.2. ���������� ���� �� �������.
            for (int x = 0; x<2; x++)
            for (int y = 0; y<2; y++)
                {
     // 4.2.2.1. �������� ������ ��� �������� ������.
                GList.Section[i]->Daughter[x][y] = new TSection;
     // 4.2.2.0. ����� ������� ������ ���������� DS (DaughterSection)
                TSection* DS = GList.Section[i]->Daughter[x][y];

     // 4.2.2.2. �������������� �������� ������.
                 DS->Init(OldestSection->Society,OldestSection->OurObject,false);
     // 4.2.2.3. ��������� �������� ������ �� 1 ������ ���������� ������ CurS
                 DS->Generation = GList.Section[i]->Generation+1;

     // 4.2.2.7. �������� "�� ����������" ���������� � ����� ������ ������.
                 DS->OldestSection = GList.Section[i]->OldestSection;
     // 4.2.2.8. ������ � � � ����������� ��� �� �-��, ��� � ����� ������� ���������.
     // 4.2.2.9. -//-
     // 4.2.2.10. ���������� ���������� ������ � ���������� ������ ������� �������� ������
                 DS->SX = (x==0)?GList.Section[i]->SX:GList.Section[i]->X;
                 DS->EX = (x==0)?GList.Section[i]->X:GList.Section[i]->EX;
                 DS->SY = (y==0)?GList.Section[i]->SY:GList.Section[i]->Y;
                 DS->EY = (y==0)?GList.Section[i]->Y:GList.Section[i]->EY;
                 DS->X  = round(float(DS->EX-DS->SX)/2)+DS->SX;
                 DS->Y  = round(float(DS->EY-DS->SY)/2)+DS->SY;
                }
     //      �����
            }
     //    �����
         }
     // 5. ������� ������, ���������� ��� ������.
     delete[] GList.Section;
     // 6. ���������� ������������� ���������.
     return true;
     }
bool TSection::GetA(TXY _A/*,TCanvas* Canvas*/)
     {
     OldestSection->EasyInit();
     // 0. ���� ��������� ����� �,� �� ���������� �����, ��  ���������� � � � �������
     if (!InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,_A.X,_A.Y,true,false,true,false))
        {
        _A.X = OurObject->X;
        _A.Y = OurObject->Y;
        }
     // 1. �������� ������ ��� ����� �
     OldestSection->A = new TXY;
     // 2. ����������� ����������� ������� �������� �
     *OldestSection->A = _A;
     // 3. ����� ������ ������ ��������� ������� � ������� �.
     OldestSection->SectionWithA = OldestSection;

// ����.:
     // 4. �������� �������� ������ ���������� ����� �.
     //    ������ �� ��� ������ ����� �������� ����� ������ ������
     // 5. ��������� ����� ������ ������� � �������� � � �, ����� �������
     // 5.1. ������� Finger �������� �������� ����� ����, ���� ��� �� ���.
//    OldestSection->FingerAllSections(true/*� ��������������*/,
//                                     true,/*������� �� ������� � � � � �������� ������*/
     // 6. ���������� ���� ����� ������� �������� ������, � ������� ��������� ������
//                                      true
                                      /*,Canvas*/
//                                      );
     return true;
     }
bool TSection::GetB(TXY _B/*,TCanvas* Canvas*/)
     {
     if (!InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,_B.X,_B.Y,true,false,true,false))
        return false;
     if (Society->MAP[_B.X][_B.Y].Team == NUMBER_OF_TEAMS)
        return false;
     // 1. �������� ������ ��� ����� B
     OldestSection->B = new TXY;
     // 2. ����������� ����������� ������� �������� B
     *OldestSection->B = _B;
     // 3. ����� ������ ������ ��������� ������� � ������� B.
     OldestSection->SectionWithB   = OldestSection;
     // 4. � ��� ��� �������� ���������� ������� � � � ������������.
     OldestSection->SectionWithA_B = OldestSection;

// ����.:
     // 4. �������� �������� ������ ���������� ����� B.
     //    ������ �� ��� ������ ����� �������� ����� ������ ������
//     OldestSection->FingerAllSections(false/*��� �������������*/,
//                                      true,/*������� �� ������� � � � � �������� ������*/
     // 5. ��� ��������� ��������� ������ �, ���� ������� �������� ������ � ��������
     //    ������� ������ ������ ��� - ���������� .
//                                      false
//                                      /*,Canvas*/);
     return true;
     }


bool TSection::Creek(/*TCanvas* Canvas*/)
     {
     bool Result = false;
     // �������� ������� �������������� ������
     // 1. ������� ��� "���������������" ������.
     // 1.1. �������� ������ ��� ������ ���� ������ ������� ��� ������ ������.
     // 1.2. �������������� ������.
     // 1.3. ��������� � ������ �� ��� �������� ������ � ������� "�" (������� �������)

     // 2. �� �������� �������� ���������� "������������" ���� ����� ����� ���������������� ��������,
     //    ������� ����� "����������" �� ��� ��� ����, � ������� ������ �� �������� ������ (�������� ������).
     //    ���� � ���, ��� ���� ���� ��� ��������, �� � ������ � ����� ������� ����� ������ ����� �������������,
     //    � �����  ��� ����� ������, ���� ����� ������ ����� ����� ��������� ��������������� ������

     //    ������ �����
     // 2.1. ���������� ��� ������ ������.
     // 2.1.1. ���� ������� ������ ������ � � ��� ��������� ����� "�" � ��� �� ������, �� ��������� ����� ������������� (������)
     // 2.1.2. �������� ��.
     //        ������ � ������� ������ ���� ���������!

     // 2.2. ������� ��� ������( ���������������� ��������� ������� ������������ ������ ���������)
     //      �������� ������
     // 2.3. ��������� �� � ��������������� ������.
     // 2.4. ������� ������ ���������.
     //    ����� �����

     // 3. �������.
     // 3.1. ������� ������.
     // 3.2. ���������� ���������.



     // 1. ������� ��� "���������������" ������.
     TGList List[2];

     // 1.1. �������� ������ ��� ������ ���� ������ ������� ��� ������ ������.
     List[0].Section  = new TSection* [OldestSection->ScionsNumber];
     List[1].Section  = new TSection* [OldestSection->ScionsNumber];
     // 1.2. �������������� ������.
     List[0].Num = 0;
     List[0].TotalNum = OldestSection->ScionsNumber;
     List[1].Num = 0;
     List[1].TotalNum = OldestSection->ScionsNumber;
     // 1.3. ��������� � ������ �� ��� �������� ������ � ������� "�" (������� �������)
     // 1.3.1. ������ ��� ������ � ��� ������, ���� ������ �������� ������ - ����� ���������� ����
     if (OldestSection->SectionWithB->CheckAndFillSection(OldestSection->SectionWithB))
        AddSection(&List[0],OldestSection->SectionWithB);
        else
        return false;
     // 2. �� �������� �������� ���������� "������������" ���� ����� ����� ���������������� ��������,
     //    ������� ����� "����������" �� ��� ��� ����, � ������� ������ �� �������� ������ (�������� ������).
     //    ���� � ���, ��� ���� ���� ��� ��������, �� � ������ � ����� ������� ����� ������ ����� �������������,
     //    � �����  ��� ����� ������, ���� ����� ������ ����� ����� ��������� ��������������� ������
     for (int t = 0; List[t].Num > 0; t = abs(t-1))
     //    ������ �����
         {
     // 2.1. ���������� ��� ������ �������� ������.
        for (int i = 0; i<List[t].Num; i++)
             {
             // 2.1.1. ���� ������� ������ ������� � � ��� ��������� ����� "�" � ��� �� ������, �� ��������� ����� ����������-
             //        ��� (������), � ����� �������� �� ������.
             if (
                 (!List[t].Section[i]->Open)
                 &&
                 (!List[t].Section[i]->Occupied)
                 &&
                 (List[t].Section[i] == OldestSection->SectionWithA)
//                  (List[t].Section[i]->X == SectionWithA->X) // ���������� ��������� ������� � ���, ���
//                 &&                                         // �� �� ��� ��������� ��� ��� ��������� -
//                  (List[t].Section[i]->Y == SectionWithA->Y) // ��� ���� ������
                )
                return true;
             // 2.1.2. �������� �� �������� CheckAndFill, ������� ����� ������� �������� GetDaughter...,
             //        ������� � ���� ������� ����� ������� GetAllNeighbour.
             //        ��� ������ � ������� ������ ���� ���������!

//             Canvas->Pixels[List[t].Section[i]->X][List[t].Section[i]->Y] = clWhite;
             // 2.2. ������� ��� ������( ���������������� ��������� ������� ������������ ������ ���������)
             //      �������� ������
             TGList _List = List[t].Section[i]->GetAllNeighbour(/*Canvas*/);

             // 2.3. ��������� �� � ��������������� ������.
             AddList(&List[abs(t-1)],&_List);
             // 2.4. �����! ������� ����������� ���������� �������� GetAllNeighbour ������ ��� ���������
             //      ������ _List.
             delete[] _List.Section;
             }
     // 2.4. ������� ������ ���������.
         List[t].Num = 0;
     //    ����� �����
         }
     // 3. �������.
     // 3.1. ������� ������.
     delete[] List[0].Section;
     delete[] List[1].Section;
     // 3.2. ���������� ���������.
     return Result;
     }


bool TSection::FindWay(/*TCanvas* Canvas*/)
     {
//   struct  time t;
//   int starttime;
//   int endtime;


     bool Result = false;
     bool FirstIteration = true;
     int Iteration = 0;
     // �������� ����� ������
     OldestSection->Occupied = !OldestSection->CheckMap(true,-1,-1);
     // ���� ����, ������ �� ������� �������� ���������.
     while ((OldestSection->MaxExistingGeneration<=OldestSection->MaxGeneration)&&
            (!Result)&&(B!=NULL)&&(Iteration < MaxGeneration))
           {
//         if (!FirstIteration)
           FingerAllSections(true,true,true /*,Canvas*/);

//         gettime(&t);
//         starttime = t.ti_hund;
           if (Creek(/*Canvas*/))
              Result = true;
//         gettime(&t);
//         endtime = t.ti_hund;

           if (!Result)
              {
              Iteration++;
              SplitSection();
              FirstIteration = false;
              }
           }


//   Time = endtime-starttime;
     return Result;
     }

/*
void TSection::DrawWay(TCanvas* Canvas)
     {
     // ���������� ������ �� ������ ����� ������� � ���� ��� �������� ������
     TGList List;
     List.TotalNum = ScionsNumber;
     List.Section = new TSection* [List.TotalNum];
     List.Num = 0;
     AddSection(&List,this);

     for (int i = 0;i<List.Num;i++)
         {
         if (List.Section[i]->Open)
            {
            TSection* CurS = List.Section[i];
            DelFromList(&List,i);
            for (int x = 0;x<2;x++)
            for (int y = 0;y<2;y++)
                AddSection(&List,CurS->Daughter[x][y]);
            i--;
            }
            else // ���� ������ ������� ������ ��
                 {
                 // 1. ���� ������ ������ (� ��� ���� �����������)
                 // ������������ �� �������� �����.
                 if (List.Section[i]->Occupied)
                    {
                    Canvas->Brush->Color = clRed;
                    Canvas->RoundRect(List.Section[i]->SX,List.Section[i]->SY,
                                      List.Section[i]->EX,List.Section[i]->EY,2,2);
                    }
                    else {
                 // 2. ���� ������ ������, ������������ �� ������ �����.
                 // 2.1. ��������� ������� ������ � �� ������� �� ������ ������ (�����)
                         if (List.Section[i]->Creeked)
                            {
                            Canvas->Brush->Color = clBlue+round(float(List.Section[i]->Steps)/2);
                            Canvas->RoundRect(List.Section[i]->SX,List.Section[i]->SY,
                                              List.Section[i]->EX,List.Section[i]->EY,2,2);
                            }
                 // 3. ���� ������ �� ������, ������������ �� ������� ����� - �� ������.
                         }
                 }
         }
*/
/*     for (int j = 0; j<List.Num;j++)
         {
                         if (List.Section[j]->Creeked)
                            {
                            if(List.Section[j]->FromSection!=NULL)
                              {
                              Canvas->Brush->Color = clWhite;
                              Canvas->PenPos.x = List.Section[j]->X;
                              Canvas->PenPos.y = List.Section[j]->Y;
                              Canvas->LineTo(List.Section[j]->FromSection->X,List.Section[j]->FromSection->Y);
                              }
                            }

         }
*/
/*
     delete[] List.Section;
     }
*/

bool TSection::GetNearestPoint()
     {
     // ���������� ������ ���������� ������.
     FingerAllSections(false,false,true);

     if ((A == NULL)||(B==NULL)||(OldestSection->SectionWithObject == NULL)||
         (!OldestSection->SectionWithObject->Creeked)
         )
        return false;
     TSection* CurS = OldestSection->SectionWithObject;
     int _X=-1,_Y=-1;
     if (OldestSection->SectionWithObject->FromSection != NULL)
        {
        if (OldestSection->SectionWithObject->FromSection != SectionWithB)
        {
        _X = OldestSection->SectionWithObject->FromSection->X;
        _Y = OldestSection->SectionWithObject->FromSection->Y;
     // ��������� ������� ����������� ������, ����� FromSection
        OldestSection->SectionWithObject = OldestSection->SectionWithObject->FromSection;
        }
        else
            if (B!=NULL)
               {
               _X = B->X;
               _Y = B->Y;
               OldestSection->DelGeneration(5);
               }
        }
        else
            if (B!=NULL)
               {
               _X = B->X;
               _Y = B->Y;
               OldestSection->DelGeneration(5);
               }





     OurObject->GetGlobalPos(_X,_Y,AI);
     }


TSection::TGList TSection::GetAllNeighbour(/*TCanvas *Canvas*/)
        {
        // ����� ���������� ������ ���� "����������" ������� ���
        // ���� ������
        // ����� ������ ������ ����������� ���� ���� �������
        // Step � FromSection

        // 1. ������� ����������� ��������� ����������.
        // 1.1. ������� ������ ������ SList.
        // 1.2. ���� ������ ����� ����� ���������� ������ �������
        // 1.3. ��������� � ������ ������ ����� ������ ������.

        // 2. ���������������� ����� �������� ������, ������� "������������" ��������� ����� ������ ������

        // 2.1. �������������� ���� for, ������ �� i = i �� ����� ������ SList, � �������
        //      i - ������ �������� �������� ������ SList � � ������� �������� �� ������������� �� 1.
        //      ������� �������� �� ��, ��� ���������� i - ��������� � ����� ������

        //      ������ ����� FOR

        // 2.1.1. ���� ������� ������ (����� CurS) �������.
        //        ������
        // 2.1.1.1. ������� CurS �� ������ (����� ���� �������� ������ �� ��� � ���. ����������).
        // 2.1.1.2. ��������� � ����� ������ SList ���������� ������� ������ CurS (��� ������ ���� ��������
        //          ������ ��� ��������, ������� ����� ������) ��� ������ SA.
        // 2.1.1.3. ��� ����, ��� �� � ��������� �������� �� ����������� �������, �������� �� �����
        //          CurS, ���� ���������� i ��������� �� 1.
        //        �����.
        //      ����� ����� FOR
        //      ����� ������� � ����� ����� ��� �������� ������ S ����� �������.


        // 3. �������.
        // 3.1. ���������� ������ SList.
        // ����� ���������� ������ ���� "����������" ������� ���
        // ���� ������
        // ����� ������ ������ ����������� ���� ���� �������
        // Step � FromSection



        // 1. ������� ����������� ��������� ����������.

        // 1.1. ������� ������ ������ SList.
        TGList SList;
        // 1.1.1. ������������� ������ SList.
        // 1.1.1.1. ������������ ����� �������� ������ ������������ �� �������:
        //          TotalNum = 4*2^(MaxExistingGeneration - Generation) + 4;
        //          MaxExistingGeneration >= Generation
        SList.TotalNum   = 4*pow((double)(2),(double)(OldestSection->MaxExistingGeneration - Generation)) + 4;
        SList.Section    = new TSection* [SList.TotalNum];
        SList.Num        = 0;

        // 1.3. ��������� ������, ������� �������� ������ � � �
        //      ��� �������, ��� ���� ��� �������,�� �� ������.
        if (CheckAndFillSection(OldestSection->SectionWithA_B))
           AddSection(&SList,OldestSection->SectionWithA_B);
        // 2. ���������������� ����� �������� ������, ������� "������������" ��������� ����� ������ ������

        // 2.1. �������������� ���� for, ������ �� i = i �� ����� ������ SList, � �������
        //      i - ������ �������� �������� ������ SList � � ������� �������� �� ������������� �� 1.
        //      ������� �������� ��� ���������� i - ��������� � ����� ������
        for (int i = 0;i<SList.Num;i++)
            {
        // 2.1.1. ���� ������� ������ (����� CurS) �������.
        //        ������
            if (SList.Section[i]->Open)
               {
               TSection *CurS = SList.Section[i];
        // 2.1.1.1. ������� CurS �� ������ (����� ���� �������� ������ �� ��� � ���. ����������).
               DelFromList(&SList,i);
        // 2.1.1.2. ��������� � ����� ������ SList ���������� ������� ������ CurS (��� ������ ���� ��������
        //          ������ ��� ��������, ������� ����� ������) ��� ������ SA(��� ���� ������).
               TGList SubList = GetDaughterNeighbour(CurS);

               AddList(&SList,&SubList);
        // 2.1.1.4. �����! ������� ������, ������� �������� ������� GetDaughterNeighbour ��� �������� ������
        //          SubList.
               delete[] SubList.Section;
        // 2.1.1.3. ��� ����, ��� �� � ��������� �������� �� ����������� �������, �������� �� �����
        //          CurS, ���� ���������� i ��������� �� 1.
        //        �����.
               i--;
               }
            }
        //      ����� ������� � ����� ����� ��� �������� ������ S ����� �������.
//               Canvas->Brush->Color = clGreen;
//              for (int k = 0;k<SList.Num;k++)
//                   Canvas->RoundRect(SList.Section[k]->SX,SList.Section[k]->SY,SList.Section[k]->EX,SList.Section[k]->EY,1,1);

        // 3. �������.
        // 3.1. ���������� ������ SList.
        return SList;
        // ����������: � ������ ������ ������ ���������� ������� ��� ������ ��
        // ���������, ��������� ������ �������� "� ������� ���" ������ ��������� ��
        // ������ ������ �������, ��� ������, ��� ������, ��� ������� ���� ��������
        // ������ ��� ����� ��������������� �������������� ��� ������ � ������� �� ������
        // ������� - ������ ������� ������.

        }

TSection::TGList TSection::GetDaughterNeighbour(TSection *_Section)
        {
        // ����� ���������� ������ �������� ������� ������ Section
        // ��� ���� ������.
        // ����������: ��������� ������� �� ������ ��������� ������
        // � ����, �� � ��, ������� ����� �������� ������� ���� ������,
        // ������ ��� ������� ����� ���� ������ ����� ������������
        // �������� ������ ������.


        // �������� ������� � ����� ������ ����� ����� ������� ��� ������� � �������
        // �������� ������ ������ _Section � ���� ������.

        // 1. ������� ��������� ����������.
        // 1.1. SX, EX, SY, EY - ����� �������, ������ �������, �����
        //      ��������, ������ �������� (��������������) ���� ������.
        //      (��� ���������� �������� �� ����).

        // 1.2. ������� ������ BList.
        TGList BList;
        // 1.2.1. �������������� BList.
        // 1.2.1.1. ������������ ����� �������� ������ ������������ �� �������:
        //          TotalNum = 4*(2^(_Section->Generation - Generation)) + 4;
        BList.TotalNum = 4*/*ceil*/(pow((double)(2),(double)(_Section->Generation - Generation))) + 4;
        BList.Section  = new TSection* [BList.TotalNum];
        BList.Num      = 0;
        // 2. ������� ������ ������ ������������ �������� ������ ������ _Section.
        for (int x = 0; x<2;x++)
        for (int y = 0; y<2;y++)
            {
        //    ������ ������
        // 2.1. BSX, BEX, BSY, BEY - ����� �������, ������ �������, �����
        //      ��������, ������ �������� (��������������) ������� �������� ������.
            int BSX = _Section->Daughter[x][y]->SX;
            int BEX = _Section->Daughter[x][y]->EX;
            int BSY = _Section->Daughter[x][y]->SY;
            int BEY = _Section->Daughter[x][y]->EY;
        // 2.2. ���� ��� ������ ������ ��� ��������� �������
            if (Generation <= _Section->Daughter[x][y]->Generation)
        //      ������
               {
        // 2.2.0.1 ���� ������� ������ �������� ���� ������� (��� ����� ����)
        //         �� ������� � �������� ������ (continue).
               if (this == _Section->Daughter[x][y])
                  continue;
        // 2.2.1. ���� BSX ��� BEX ��� BSY ��� BEY ������ � ���������� [SX;EX;SY;EY]
        //        ����� ������, ���� ������ ���� �� ������ �������� � ������� ����� ������
                  if (
                      InXYSpace(SX,EX,SY,EY,BSX,BSY,true,true,true,true)
                      ||
                      InXYSpace(SX,EX,SY,EY,BEX,BSY,true,true,true,true)
                      ||
                      InXYSpace(SX,EX,SY,EY,BSX,BEY,true,true,true,true)
                      ||
                      InXYSpace(SX,EX,SY,EY,BEX,BEY,true,true,true,true)
                      )
        //        �� �������� ������� ������ � ������ ���������� ������ BList.
                     {
        // 2.2.1.1. �������� ������� ������ - ����� �� ����� �� ��������.
        //          ���� �����, �� ��������� ��� ����������� ����
                     if(CheckAndFillSection(_Section->Daughter[x][y]))
                       AddSection(&BList,_Section->Daughter[x][y]);
        //        � ������� ������, � ��������� ������
                     continue;
                     }
        //      �����
               }
        // 2.3. ����� (���� ��� ������ ������)
                else
        //        ������
                  {
        // 2.3.1. ���� SX ��� EX ������� � ���������� [BSX;BEX]
                  if (
                      InXYSpace(BSX,BEX,BSY,BEY,SX,SY,true,true,true,true)
                      ||
                      InXYSpace(BSX,BEX,BSY,BEY,EX,SY,true,true,true,true)
                      ||
                      InXYSpace(BSX,BEX,BSY,BEY,SX,EY,true,true,true,true)
                      ||
                      InXYSpace(BSX,BEX,BSY,BEY,EX,EY,true,true,true,true)
                      )

        //        �� �������� ������� ������ � ������ ���������� ������ BList.
                     {
        // 2.3.1.1. �������� ������� ������ - ����� �� ����� �� ��������.
        //          ���� �����, �� ��������� ��� ����������� ����
                     if(CheckAndFillSection(_Section->Daughter[x][y]))
                       AddSection(&BList,_Section->Daughter[x][y]);
         //           � ������� � ��������� �������� ������
                     continue;
                     }
        //        �����
                  }
        //    ����� ������
               }
        // 3. �����.
        // 3.1. ���������� ������ BList.
           return BList;
        // ����������: � ������ ������ ������ ���������� ������� ��� ������ ��
        // ���������, ��������� ������ �������� "� ������� ���" ������ ��������� ��
        // ������ ������ �������, ��� ������, ��� ������, ��� ������� ���� ��������
        // ������ ��� ����� ��������������� �������������� ��� ������ � ������� �� ������
        // ������� - ������ ������� ������.
        }

bool TSection::CheckAndFillSection(TSection* _Section)
     {
     bool Result = true;
//    _Section  - ����������� ������.
//   1. ���� ������ �������
     if (!_Section->Open)
//      ������
        {
//   1.1. ���� ������ ������ - ��������� �������������.
        if (_Section->Occupied)
           return false;


//   1.2. ���� ������ ������,
        if (_Section->Creeked)
//            � ���� �� ������ ������� �� ������� ������ ����� ��� ������ �������, ���
//            ����� ��, ������� ��� ��� ���� ������ (��� �����) - ��������� �������������
           {
           if ((Steps + sqrt( (double)kvdrt(_Section->X - X )+kvdrt(_Section->Y - Y))) > _Section->Steps)
              return false;
//   1.2.2. ���� ������ ������, �� �� ��� �� ��� ���� ������, �� ��� ���� � ��� ����� ��������, ��
//          ���������� �� �������� �� ������� �������.
              else Result = false;
           }
//   1.3. ��������� �����, �������� �� ��������� ������ � �������� ������� � ����� ���� ������ ���
//        � �������� ������� ������������ ������ ���� � �������� ������,
//        ���� ��� - ��������� �������������.

        int MidX = _Section->X + round(float(X - _Section->X)/2);  // �������� �������������� �������
        int MidY = _Section->Y + round(float(Y - _Section->Y)/2);
        if (
           (!_Section->CheckMap(false,-1,-1))
           ||
           (!_Section->CheckMap(false,MidX,MidY))
           )
           return false;

//   3.1. ��������� ���� Steps ��� ������ _Section �� �������:
//        ����� ���������� �� ���� ������ � ��������� ����� �������� ���� � ������ _Section.
//        _Section->Steps = Steps + ( (_Section->X - X)^2 + (_Section->Y - Y)^2)^(1/2);
        _Section->Steps = Steps + sqrt( (double)kvdrt(_Section->X - X )+kvdrt(_Section->Y - Y));
//   3.2. ��������� ���� _Section->FromSection - ���������� �� ��� ������.
        _Section->FromSection = (this!=_Section)?this:NULL;
//   3.3. ���� ������ �� ���� ������ ������ - �������� ��.
        _Section->Creeked = true;
        }
//   4. �������.
//   4.1. ��� ����� �������� - ��������� �������������
     return Result;
     }

bool TSection::CheckMap(bool ForSection,int CentX,int CentY)
     {
     // 0. ����������� ������ ����������� ������� �����.
     // 0.1. ������ �������.
     int W  = round(float(OurObject->Width)/2);
     // 0.2. �����, ������, ������ � ������� ������� ��������������.
     int _X = (CentX>=0)?CentX:X;
     int _Y = (CentY>=0)?CentY:Y;
     int LB = (!ForSection)? _X-W: SX;
     int RB = (!ForSection)? _X+W: EX;
     int BB = (!ForSection)? _Y-W: SY;
     int TB = (!ForSection)? _Y+W: EY;

     // 1. ��������� ��������, ���������� �������,
     //    � ������� � ������ ������, � ���� ���� �� ���� ����� ���� ������� �� ������������� ��������
     //    �������, � ������������� �����������.
     for (int x = LB; x<=RB; x++)
     for (int y = BB; y<=TB; y++)
         if (
     // 1.1. ����, ��������, �� ����� � ������� ����� �����������
            (Society->MAP[x][y].Team == NUMBER_OF_TEAMS)
            ||
     // 1.2. ��� ����� ������ ������� �� ������� �����:
            (!InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,x,y,true,false,true,false))
            )
     //    �������, � ������������� �����������.
            return false;
     // 2. ���� ������� - ������ ��� ���������� ������� �����.
     return true;
     }
// ���������� ���������(SubList) � ������ (InList)
bool TSection::AddList (TGList *InList, TGList *SubList)
     {
     // 1. ��������� �� ��������� ������
     // 1.1. ���� ����� !����������! ��������� ������ InList � !����������! ��������� ������ SubList
     //      ������ ������� ������ InList - ���������� ������������� ���������.
        if ((InList->Num+SubList->Num) >= InList->TotalNum)
           return false;
     // 2. �������������� ���� FOR ������� ������������ ��� �������� ������ SubList
        for (int i = 0; i<SubList->Num; i++)
     // 2.1. ��������� ������� ������� ������ SubList � ������ InList.
            if (!AddSection(InList,SubList->Section[i]))
               return false;
     // 3. ������� - ���������� ������������� ���������.
     return true;
     }

// ���������� ������ � ������.
bool TSection::AddSection(TGList* _List, TSection *Section)
     {
     // 1.1. ��������� �� ������������ ����������.
     if ((_List == NULL)||(Section == NULL))
        return false;
     // 1.2. ���������  �� ���� ������������ ������ ����� ������� � ����
     //      ���� ������.
     if (_List->Num >= _List->TotalNum)
        return false;

     // 2. ��������� ������ � �����.
     _List->Section[_List->Num] = Section;
     _List->Num++;
     // 3. ���������� ������.
     return true;   
     }
// �������� �� ������.
bool TSection::DelFromList(TGList *_List, int ElementIndex)
     {
     // 1. �������� �� ��������� ������.
     // 1.1. ���� ������ ������ ��������������� �������� ���
     //      ������ ������� ������ ���� - ��������� �������������.
        if (!InSpace(0,_List->Num,ElementIndex,true,false))
           return false;
     // 2. �������� � �������� �������� ������������ �������� ���������� ��������.
        _List->Section[ElementIndex] = _List->Section[_List->Num-1];
     // 3. ���������� ��������� ��������� �� 1.
        _List->Num--;
     // 4. �����, ���������� 1.
        return true;
     }

//----------------------TPoints-------------------------------------------------
//------------------------------------------------------------------------------
bool TPoints::Init (TGObject *_OurObject)
     {
     if (_OurObject == NULL)
        return false;
     //������������� �����������
     //ways`s initialization;
     Point = new TWays* [MAP_WIDTH];
     for(int X = 0; X<MAP_WIDTH;X++)
        {
        Point[X] = new TWays [MAP_HEIGHT];
        for(int Y = 0; Y<MAP_HEIGHT;Y++)
           {
           for (int i = 0; i<3;i++)
              for (int j = 0; j<3;j++)
                  {
                  Point[X][Y].Where[i][j] = false;
                  Point[X][Y].Step = -1;
                  }
           Point[X][Y].Creeked = 0;
           }
         }
     //������������� ������� �����/������
     //Distination point`s initialization

     A.X = -1;
     A.Y = -1;
     B.X = -1;
     B.Y = -1;

     //������������� ������� - ����������
     //OurObject`s initialization
     OurObject = _OurObject;
       return true;
     }

bool TPoints::GetA()
     {
     if (OurObject==NULL)
        return false;
     if (!OurObject->Is)
        return false;
     A.X = OurObject->X;
     A.Y = OurObject->Y;
     return true;
     }

bool TPoints::GetB(TXY _B)
     {
     if (InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,_B.X,_B.Y,true,false,true,false))
        {
        B = _B;
        return true;
        }
        else
        return false;
     }

bool TPoints::CheckThis(int X,int Y)
     {
     bool Result;
     int W = OurObject->Width;
     int WD = round(float(W)/2);
     for(int x = X - WD; x <= X+WD; x++)
        for (int y = Y - WD; y <= Y + WD; y++)
           {
           if (!InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,x,y,true,false,true,false))
              return false;
           if (
               (OurObject->Society->MAP[x][y].Team==NUMBER_OF_TEAMS)
              )
              return false;

           }

     return true;

     }
bool TPoints::CheckPoint(int X,int Y)
    {
    int Team  = OurObject->TeamIndex;
    int Index = OurObject->Index;
    bool Result;
     int W = OurObject->Width;
     int WD = round(float(W)/2);
     for(int x = X - WD; x <= X+WD; x++)
        for (int y = Y - WD; y <= Y + WD; y++)
           {
           if (!InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,x,y,true,false,true,false))
              return false;
           if (
               (OurObject->Society->MAP[x][y].Team!=-1)&&(OurObject->Society->MAP[x][y].Team!=Team)
               /*!((OurObject->Society->MAP[x][y].Team==Team)&&(OurObject->Society->MAP[x][y].Index==Index))*/
              )
              return false;

           }

     return true;
    }

int Temp;
bool TPoints::Creek(TXY _XY/*,TCanvas* Canvas*/)
    {

    bool Res = false;
    // ������� ��� ���������� ������ ����� ������� � ������ ������� ������ ����������
    struct {
           TXY* XY;
           int Num;
           }
           List[2];
    // ���������� �� ��������������
    List[0].Num   = 0;
    List[0].XY = new TXY [MAP_WIDTH*MAP_HEIGHT];
    List[1].Num   = 0;
    List[1].XY = new TXY [MAP_WIDTH*MAP_HEIGHT];
    // ������ "��������������� �������"
    List[0].XY[0] = B; // (�� �������� ���� ����� - ����� "�")
    List[0].Num  ++;

    // t = abs(t - 1)  - ��� ������, ��� �������� "t" ����� ���������� ���� 0 ���� 1
    // �� �. ���� ������ ������ ���� ������, ��� ��������� � "����������������" ������
    // ����� �������� List[abs(t-1)]
    for (int t = 0; List[t].Num>0; t = abs(t-1))
        {
        Temp++;
        //���������� ��� ����� ������������� �����
        for (int i = 0; i<List[t].Num;i++)
            {
            // �������� ��
            int X = List[t].XY[i].X;
            int Y = List[t].XY[i].Y;
            if(CheckThis(X,Y))  // ���� �� ���� �������� ������ - ����� ����.
              {
              // ���� ������� ����� - ���������� - ������ "�", �� ������ �������
              // ������������ �����
              if (
                   (List[t].XY[i].X == A.X)
                   &&
                   (List[t].XY[i].Y == A.Y)
                 )
                 Res = true;

              // �������� �����
              // �������� ��������, ��� �� ����� ����������� ���������� � ��� ������ ������ �����

              Point[X][Y].Creeked = true;
              /*Canvas->Pixels[X][Y] = 256*256*(256-round(Point[X][Y].Step));*/ // ���-�� ����� ���������� ����� �����

              // ��� ����������� ���,� ����� ���������, � �������
              // ��������� ���������� � ��������������� ������ � ���,
              // ����� ����� �� ����� ������ �� ���� �����.
              for (int x = -1;x<=1;x++)
                 for (int y = -1;y<=1;y++)
                     {

                     //��������� �����
                     if (CheckThis(x+X,y+Y))
                     // ���� ��� ������ �� "��������" ��� �����...
                     if (!Point[X][Y].Where[x+1][y+1])
                     // ���� �� ���� ����� �� ��� �� ������
                     if (!Point[X+x][Y+y].Where[2-(x+1)][2-(y+1)])
                        {
                        // ������� ����� �� ����������� ���������� ����� �� ����� � � �����
                        // (�+�;�+�) ����� ����� (�;�) ���� ��� ����� ��� �� �������� ?
                        float MinStep     = Point[X][Y].Step;

                        if (
                           ((x == -1)||(x == 1))
                           &&
                           ((y == -1)||(y == 1))
                           )
                           MinStep += sqrt( (double)2 )-1;

                        // ��������� �� ����� �� ��� ����� (�+�;�+�) ������ ��� ������
                        // ���� �����, �� �������  ����, � ������ � ������� ��� ����, ���
                        // "�������" ��� ��� ��� ����� �� ��������
                        bool AlreadyCreek = false;
                        bool OurStepMin = true;
                        for (int _x = 0;_x<=2;_x++)
                           for (int _y = 0;_y<=2;_y++)
                              if (InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,X+x+_x-1,Y+y+_y-1,true,false,true,false))
                              if (Point[X+x][Y+y].Where[_x][_y])
                              {
                              AlreadyCreek = true;
                              if (Point[X+x+_x-1][Y+y+_y-1].Step < MinStep)
                                 {
                                 OurStepMin = false;
                                 break;
                                 }
                              }

                        if (!OurStepMin)
                           continue;

                         // �������� ��� ��� ����� ��� ����� ������ � �� ���...
                         Point[X+x][Y+y].Where[2-(x+1)][2-(y+1)] = true;


                        // ���� ������� ���� �� ��������� �� ���������� ������ �� 2-�
                        Point[X+x][Y+y].Step = MinStep+1;




                        // �� ����� (�+�;�+�) ����� ������� �� ���� ������ �����,
                        // ��� � ��� �� "�" ����� ����� (�;�), ��� ��� �� ����� (�;�)
                        if (!AlreadyCreek)
                           {
                           int j = List[abs(t-1)].Num;
                           List[abs(t-1)].Num++;


                           if (List[abs(t-1)].Num >= (MAP_WIDTH*MAP_HEIGHT))
                              {
//                            ShowMessage("������� ������!!!");
                              return false;
                              }

                           List[abs(t-1)].XY[j].X = X+x;
                           List[abs(t-1)].XY[j].Y = Y+y;
                           }
                        }
                     }


              }

            }
         // � ����� ����� ���� ������ ������������������� ������� ������.
         List[t].Num = 0;
        }

        // � ��� ����� �������� ��� ���������� �����, �� ��������� - ���� ��
        // ��� ������ "���" ���������� ���� ���� - ����� �. ������ �� ��
        // ���������, � ������� � ����������
        return Res;


    }


/*     Point[_XY.X][_XY.Y].Creeked = true;

     bool Result = true;
     // ���������� ��� ����� �����
     while (Result)
      {
      Temp++;
      Result = false;
      for (int X = 0;X<MAP_WIDTH;X++)
        for (int Y = 0;Y<MAP_HEIGHT;Y++)
           {
            //���� ������� ����� ������
            if (Point[X][Y].Creeked)
              {
              //���������� �� ����������
              for (int x = -1;x<=1;x++)
                 for (int y = -1;y<=1;y++)
                    {
                    //���� �� ������� ������� ������ ����� ��������� ������ ����� �� ������ - ������.
                    if (InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,x+X,y+Y,true,false,true,false))
                    if ((X!=x)&&(Y!=y))
                    {
                    if (!Point[x+X][y+Y].Where[2-x-1][2-y-1])
                    if (!Point[X][Y].Where[x+1][y+1])
                       {
                       Point[x+X][y+Y].Where[2-x-1][2-y-1] = true;
                       Point[x+X][y+Y].Creeked = true;
                       Result = true;
                       }
                    }
                    }
              }
           }
      }            */

bool TPoints::GetNewPoint()
     {
     if (OurObject == NULL)
        return false;
     if (!OurObject->Is)
        return false;
     if (!InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,OurObject->X,OurObject->Y,true,false,true,false))
        return false;
     if ((OurObject->X == B.X)&&(OurObject->Y == B.Y))
        return false;
     if ((B.X == -1)||(B.Y == -1))
        return false;

     int X  = OurObject->X,X__ = X;
     int Y  = OurObject->Y,Y__ = Y;

     int _X = B.X;
     int _Y = B.Y;
     // ��������� ������ ��� ��������� ������������ ����������
     struct {
             TXY XY;
             float R;
            }
            ToBest;



     for (int i = 0;i<=OurObject->Speed;i++)
           {
           ToBest.R = Point[X__][Y__].Step;
           ToBest.XY.X = X__;
           ToBest.XY.Y = Y__;
           for(int x = 0;x<=2;x++)
              for(int y = 0;y<=2;y++)
                 {
                 if (!((x == 1)&&(y == 1)))
                 if (CheckPoint(x-1+X__,y-1+Y__))
                 if (Point[x-1+X__][y-1+Y__].Creeked)
                 {
                 float __R = Point[x-1+X__][y-1+Y__].Step;
                 if (__R<=ToBest.R)
                    {
                    ToBest.R = __R;
                    ToBest.XY.X = x-1+X__;
                    ToBest.XY.Y = y-1+Y__;
                    }
                 }
                 }
           X__ = ToBest.XY.X;
           Y__ = ToBest.XY.Y;
           }
     OurObject->GetGlobalPos(X__,Y__,AI);
     OurObject->GetNewPos(X__,Y__);
     return true;
     }
//----------------------TTarget-------------------------------------------------
//------------------------------------------------------------------------------
void TTarget::Init(TGSociety *_Society,TGObject *_Object)
     {
     Society   = _Society;
     OurObject = _Object;
     On      = false;
	 Frame   =  0;
     Roof    =  0;
     X       = -10;
     Y       = -10;
     DX      =   0;
     DY      =   0;
     Timer   =  -1;
     MaxTime =   0;
	 MustFree=   0;
	 FreeTimer=  0;
     Radius  =  -1;
     Damage  =   0;
     }
bool TTarget::Fire(int X,int Y)
{
float   Z = Society->MAP[X][Y].Z;
int   DeltaX = X - OurObject->X,
      DeltaY = Y - OurObject->Y;
float R      = sqrt( (double)kvdrt(DeltaX )+kvdrt(DeltaY));


OurObject->Target.Frame = 0;
return (GetTarget(X,Y,Z)); 
//����� ������ ���� ���������� ��������� ���-���� �������, � ����������� �� ��������� �� ��������� ... �����!
}

bool TTarget::CheckFire()
     {
     int _X,_Y;float _Z;
     float  TempZ;

     for (int S = 1;S<round(Distance);S++)
         {
         _X = round(StartX + float(S)*cos(Angle));
	     _Y = round(StartY + float(S)*cos(Angle));
         _Z = GetZ(StartZ,Speed,G,S,StartZTangens);		
         //_Z = (TempZ<(1+StartZ))?SpecRound(TempZ,true):round(TempZ);
         if (Society->MAP[_X][_Y].Z>=_Z)
             return false;
         }
     return true;
     }



bool TTarget::GetTarget(int _X,int _Y,float _Z)
     {
     On         = true;
     
     X        =  _X;
     Y        =  _Y;
     Z        =  _Z;
     StartX   =  OurObject->X;
     StartY   =  OurObject->Y;
     StartZ   =  OurObject->Z+0.1;
     
	 Frame    =  0;

     CurrentX = StartX;
     CurrentY = StartY;
     CurrentZ = StartZ;

     OldCurrentX = CurrentX;
     OldCurrentY = CurrentY;
     OldCurrentZ = CurrentZ;
     OldWentWay  = WentWay;


     DX       = X - StartX;
     DY       = Y - StartY;
     DZ       = Z - StartZ;
     Angle    = (DX!=0)?atan2( (double)DY, (double)DX):(Pi/2);

     Distance = sqrt( (double)kvdrt(DX )+kvdrt(DY));
     WentWay  = 0;
     if (!SetTrajTangens(G,Speed,DZ,Distance,Roof,StartZTangens))
        {
        FreeTarget();
        return false;
        }
     SpeedOXY =  sqrt( (double)kvdrt(Speed )/(1+fkvdrt(StartZTangens)));

     Timer    = 0;
     MaxTime   = Distance/(SpeedOXY*cos(atan(StartZTangens)));
   /* (!CheckFire())
       {
         FreeTarget();
         return false;
       }
   */
     
     }
void TTarget::FreeTarget()
     {
     On       = false;

     X        =  -10;
     Y        =  -10;
     StartX   =  X;
     StartY   =  Y;
     StartZ   =  Z;

	 Frame    =  0;

     CurrentX = StartX;
     CurrentY = StartY;
     CurrentZ = StartZ;

     OldCurrentX = CurrentX;
     OldCurrentY = CurrentY;
     OldCurrentZ = CurrentZ;
     OldWentWay  = WentWay;


     DX       = X - StartX;
     DY       = Y - StartY;
     DZ       = Z - StartZ;
     Angle    = 0;

     Distance = 0;
     WentWay  = 0;
     StartZTangens = 0;


     Timer     = 0;
     MaxTime   = 0;
	 FreeTimer = 0; 
	 MustFree  = 0;


     }

void TTarget::GetCurXYZ()
     {
     CurrentX = StartX + SpeedOXY*cos(Angle)*Timer;
     CurrentY = StartY + SpeedOXY*sin(Angle)*Timer;
     WentWay  = sqrt( (double)fkvdrt(CurrentX-StartX )+fkvdrt(CurrentY-StartY));
     CurrentZ = GetZ(StartZ,Speed,G,WentWay,StartZTangens);

     if (WentWay>=SpeedOXY)
        {
        OldCurrentX = CurrentX - SpeedOXY*cos(Angle);
        OldCurrentY = CurrentY - SpeedOXY*sin(Angle);
        OldWentWay  = WentWay  - SpeedOXY;
        OldCurrentZ = GetZ(StartZ,Speed,G,OldWentWay,StartZTangens);
        }

     }

void TTarget::Refresh(int _Frame)
     {
     Frame = _Frame;

     if (MustFree)
        {
        FreeTimer++;
		if (FreeTimer == TARGET_BLAST_FRAMES)
		   FreeTarget();  
		}

     if (!On) return;
     Timer+=float(Frame)/float(MAX_FRAMES)+1;
     
     GetCurXYZ();


     // ��������� ������� !!!!!!!!!!!!!!!!!!
     {
     if (CurrentX > (MAP_WIDTH-1))
        {
        MustFree = true;
        On = false;
        CurrentX = MAP_WIDTH -1;
        }
     if (CurrentX < 0)
        {
        MustFree = true;
        On = false;
        CurrentX = 0;
        }

     if (CurrentY > (MAP_HEIGHT-1))
        {
        MustFree = true;
        On = false;
        CurrentY = MAP_HEIGHT -1;
        }
     if (CurrentY < 0)
        {
        MustFree = true;
        On = false;
        CurrentY = 0;
        }


     if (OldCurrentX > (MAP_WIDTH-1))
        {
        MustFree = true;
        On = false;
        OldCurrentX = MAP_WIDTH -1;
        }
     if (OldCurrentX < 0)
        {
        MustFree = true;
        On = false;
        OldCurrentX = 0;
        }

     if (OldCurrentY > (MAP_HEIGHT-1))
        {
        MustFree = true;
        On = false;
        OldCurrentY = MAP_HEIGHT -1;
        }
     if (OldCurrentY < 0)
        {
        MustFree = true;
        On = false;
        OldCurrentY = 0;
        }



     }
     // ------------------------------------


     // ��������� �����������
     // ------------------------------------------------------------------------
     if (OldCurrentZ <= Society->MAP[round(OldCurrentX)][round(OldCurrentY)].Z)
         {
	 MustFree = true;
         On       = false;
         }
     //{

     if ((WentWay!=0)&&(CurrentZ <= Society->MAP[round(CurrentX)][round(CurrentY)].Z)) //���� ������ ����� ��������� �������� � ������������ ��������:
        {
        //������� �������� ��� ������
        int _X = round(CurrentX),
		    _Y = round(CurrentY);
        for (int y = round(-Radius);y<=round(Radius);y++)
         for (int x = round(-Radius);x<=round(Radius);x++)
          {
          if (!((InSpace(0,MAP_WIDTH,(_X+x),true,false))&&(InSpace(0,MAP_HEIGHT,(_Y+y),true,false))))
		     continue;
             //���� ���������� ����� ������� �� ������� ����� - ������ �� �������� ��
          
          float XY_;
          float r = sqrt( (double)kvdrt(x )+kvdrt(y)),
		        b = Damage,
		        k = Damage/Radius;
          int damage = (r<=Radius)?(k*r+b):0;

		  if (Radius >=CRASH_RADIUS)
		  // �� ���������� Z �������� �������� ��������������������� ����������� � ������������������������
		  // �������...
             Society->MAP[_X+x][_Y+y].Z =(Society->MAP[_X+x][_Y+y].Z-(float(damage)/500))?Society->MAP[_X+x][_Y+y].Z-(float(damage)/500):0;
          if ((Society->MAP[x+_X][y+_Y].Team!=-1)&&(Society->MAP[x+_X][y+_Y].Team!=NUMBER_OF_TEAMS))
             {
             int Team  = Society->MAP[x+_X][y+_Y].Team;
             int Index = Society->MAP[x+_X][y+_Y].Index;
             Society->GObject[Team][Index].DamageMe(damage);
             if (Society->GObject[Team][Index].Live<=0)

                Society->Dye(&Society->GObject[Team][Index]);
             }

          }
        return;
     	}
      //GetCurXYZ();
      //}
      //  else //����� ������ ������� � ��������� ����������
      




     }


//-------------------------------------------------------------------------------



//void DoAction()
//----------------------TGObject------------------------------------------------
/*
����� �������� � ��������� ������(������ �� ��������� � ������� ��������)
������ ������.
*/
//------------------------------------------------------------------------------

void TGObject::Init(TGSociety *_Society,int n,int _Type,int _Team,int _Index)
{

 Type            = /*(*/(_Type > NUMBER_OF_STANDARTS)/*||(_Type>=NUMBER_OF_TEAMS))*/?0:_Type;
 Society         = _Society;
 TGStandart* Standart = &Society->Standart[_Type];

 TeamIndex       = _Team;
 Index           = _Index;

 ModelFrame      = 0;
 MAX_ModelFrames = Standart->MaxModelFrames;
 Selected        = false;


 Live            = Standart->Live;    // %
 Sight           = Standart->Sight;
 Speed           = Standart->Speed;
 AngleSpeed      = Standart->AngleSpeed;

 Kvadrat.IndexOX = -1;
 Kvadrat.IndexOY = -1;
 Kvadrat.ID      = -1;

 CanMove         = Standart->CanMove;
 Thinking        = Standart->Thinking;
 CanAttack       = Standart->CanAttack;
 CanAttackMove   = Standart->CanAttackMove;
 Character       = AGRESSIVE;

 Target.Init(_Society,this);
 Status.Fire     = false;
 Status.Attack   = false;
 Status.InShock  = false;
 Status.MustMove = false;
 OldStatus      = Status;


 ACTION           =   WAIT_ACT;
 AttackPeriod     =   Standart->AttackPeriod;  // ����� ������ ����������� ������ ��������� ����
 TimeAfterAttack  =   AttackPeriod;
 /*Weapon         =*/ GetWeapon(Standart->IndexWeapon);
 Armor            =   Standart->Armor;
 Width            =   Standart->Width;

 IncBaseInit();


 //Status = NETRAL;
 NewPosition.X       = -1;
 NewPosition.Y       = -1;
 NewPosition.GlobalX = -1;
 NewPosition.GlobalY = -1;
 NewPosition.Steps = 0;
 ACTION = ATTACK_ACT;
 X = OldX            =-10 ;
 Y = OldY            =-10;  // ...
 Angle = OldAngle    = 0;
 TempX = TempY       = 0;
 OldTempX = OldTempY = 0;

 OnMission = false;
 if ((NewPosition.GlobalX - X))
 NewPosition.Angle = 0;/*atan(
                         (NewPosition.GlobalY - Y)
                         /
                         (NewPosition.GlobalX - X)
                         );*/
    else NewPosition.Angle = 0;

 Is = OldIs     = false; // ��� �������� ���������� - ���������� ������ ��� ���.
 N              = 2;
 PrimEnemy.Type = -1;
 InGroup        = false;
 GroupIndex     = -1;
 GroupTeam      = -1;
 IndexInGroup   = -1;
 PrimaryEnemy   = NULL;
 GETPOWER();

// ������� ������ ����������� (SectionWay) ������� ��� ���� ���������� � ������
// ���������� �������, ������� ������� ���������������� ��� ����� ���������.

 SectionWay.Init(Society,this,true);
// Points.Init(this);



}

void TGObject::IncBaseInit()
     {

     IncludedInBase = NULL;

     IncludedInBase = new TIncInBase* [NUMBER_OF_TEAMS];  // Include In Base
     for (int t = 0;t<NUMBER_OF_TEAMS;t++)
         {
         IncludedInBase[t] = new TIncInBase [NUMBER_OF_GROUPS];
         for (int g = 0;g<NUMBER_OF_GROUPS;g++)
             {
             IncludedInBase[t][g].Yes = false;
             }
         }

      }

void TGObject::GetWeapon(int ind)
     {
	 //�������� �� ������
	 ind = (ind >= NUMBER_OF_WEAPONS)?0:ind;

         // ����������� ����
	 TypeWeapon = (Society->Weapon[ind].Laser)?WP_LASER: ((Society->Weapon[ind].TargetRoof)?WP_HOWITZER:WP_GUN);

	 if ((TypeWeapon == WP_GUN) || (TypeWeapon == WP_HOWITZER))
	    {
		Target.Damage = Society->Weapon[ind].TargetDamage;
		       Damage = Society->Weapon[ind].TargetDamage;
		Target.Radius = Society->Weapon[ind].TargetRadius;
		Target.Speed  = Society->Weapon[ind].TargetSpeed;
		Target.Roof   = Society->Weapon[ind].TargetRoof;
    	}

     if (TypeWeapon == WP_LASER)
	    {
	    Range  = Society->Weapon[ind].LaserRange;
		Damage = Society->Weapon[ind].LaserDamage;
		}


     // �������� ������ �� ������ � �������.
     IndexWeapon = ind;

     }

void TGObject::OutGroup()
     {
     if (!InGroup)
        return;
     int IIG = IndexInGroup;
     int GI  = GroupIndex;
     int GT  = GroupTeam;

     int Nbr = Society->Groups[GT][GI].Number;
     if ((!InGroup))
		 return;
     if (Society->Groups[GT][GI].Number<=0)
		 return;
	 if ((IIG == -1)||(GI == -1)||(GT == -1))
		 return;
     Society->Groups[GT][GI].Number--;
     IndexInGroup = -1;
     GroupTeam    = -1;
     InGroup = false;


     Society->Groups[GT][GI].Units[IIG] = Society->Groups[GT][GI].Units[Nbr-1];
     Society->Groups[GT][GI].Units[IIG].PUnit->IndexInGroup = IIG;

     Society->Groups[GT][GI].Units[Nbr-1].Is    = false;
     Society->Groups[GT][GI].Units[Nbr-1].PUnit = NULL;
     if (Society->Groups[GT][GI].Number == 0) //���� �� ������ ��������� ��� ����� �� �����
                                              //���������� ���� ����� RefreshGroupParametres
                                              //������ ��� ������, �� ������� �� ���� "���"
                                              //����� ��������� ��� ���������� ����� OutGroup
                                              //���������� �� ���� ������
                                              //��� �������� ����� ������ �� ��� ����� ���������
                                              //������ ������
         Society->DelGroup(GT,GI,"OutGroup");
      }


void TGObject::SetModelFrame()
     {
     //�� ����� � �������
     if (OldStatus.Fire||OldStatus.Moving||(ModelFrame!=0))
		 ModelFrame = (ModelFrame < MAX_ModelFrames)?ModelFrame+1:0;

     }

void TGObject::GetTempXYZAngle(float Frame)
     {
     OldTempX = TempX;
     TempX = OldX + Frame*(X - OldX)/MAX_FRAMES;

     OldTempY = TempY;
     TempY = OldY + Frame*(Y - OldY)/MAX_FRAMES;

     //OldTempZ = TempZ;
     TempZ = OldZ + Frame*(Z - OldZ)/MAX_FRAMES;

     float Delta  = ((Angle-OldAngle)/float(MAX_FRAMES))*(Frame);
     TempAngle    = Angle + Delta;
     
     }

void TGObject::GETPOWER()
     {
     if (AttackPeriod !=0)
        POWER = Live * Damage * (Armor+1)*Sight / AttackPeriod;


     }
void TGObject::Dye()
     {
     TeamIndex = -2;
     Is = false;



     }

bool TGObject::DamageMe(int Dmg)
     {
     if (Dmg > Armor)
        Live +=Armor-Dmg;

     return Dmg > Armor;
     }




void TGObject::SetAttack(TATTACK_MODE PARAMETER)
     {
     switch (PARAMETER)
          {
          case NOT_ATTACK: CanAttack = false; break;
          case ATTACK_NOT_MOVE: CanAttack = true; CanAttackMove = false; break;
          case ATTACK_AND_MOVE: CanAttack = true; CanAttackMove = true; break;
          }
     }
bool TGObject::CheckPoint(int x,int y)
     {
     int HW = round(float(Width)/2);
     for (int _x = x-HW;_x<=x+HW;_x++)
        for (int _y = y-HW;_y<=y+HW;_y++)
           {
           if (!InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,_x,_y,true,false,true,false))
              return false;
           if (Society->MAP[_x][_y].Team !=-1)
           if (!((Society->MAP[_x][_y].Team == TeamIndex)&&(Society->MAP[_x][_y].Index==Index)))
              return false;
           }
     return true;
     }

void TGObject::GetNewPos(int fX,int fY)
//��� ������� ������ ����� ������� ������� � ������� ���������   � � � � �   � � � � � � �.
     {
     if ((!InSpace(0,MAP_WIDTH,fX,true,false))||(!InSpace(0,MAP_HEIGHT,fY,true,false)))
         return;
     NewPosition.X = fX;
     NewPosition.Y = fY;
     NewPosition.Steps = 0;
     NewPosition.DeltaX = fX-X;
     NewPosition.DeltaY = fY-Y;
     if (min(abs(fX-X),abs(fY-Y))==abs(fY-Y) )
     NewPosition.SmallPoint = Y; else
     NewPosition.SmallPoint = X;

     NewPosition.Angle = StdAngl(
                                 atan2(
                                      (double)(NewPosition.GlobalY - Y)
                                       ,
                                      (double)(NewPosition.GlobalX - X)
                                      )/OGIR
                                 );
     }
void TGObject::GetNewPos(float Angle,float R)
//��� ������� ������ ����� ������� ������� � ������� ���� � ���������� �� ����
     {
     int DeltaX = round(R*cos(Angle*OGIR)),
         DeltaY = round(R*sin(Angle*OGIR));
     if ((!InSpace(0,MAP_WIDTH,X+DeltaX,true,false))||(!InSpace(0,MAP_HEIGHT,Y+DeltaY,true,false)))
         return;

     NewPosition.X = X + DeltaX;
     NewPosition.Y = Y + DeltaY; //���������� ��������� ����� �������
     if ((NewPosition.X > MAP_WIDTH)&&(NewPosition.X<0)&&(NewPosition.Y>MAP_HEIGHT)&&(NewPosition.Y<0))
        bool Stop = 0;

     NewPosition.DeltaX = DeltaX;
     NewPosition.DeltaY = DeltaY;
     NewPosition.Steps = 0;
     if (min(abs(DeltaX),abs(DeltaY))==abs(DeltaY) )
        NewPosition.SmallPoint = Y; else   //� ��� ���������� ��������������� �������
        NewPosition.SmallPoint = X;        //��� ����������� �� �����.

        NewPosition.Angle = StdAngl(Angle);
     }

void TGObject::FreePosition()
     {
     NewPosition.GlobalX = -1;
     NewPosition.GlobalY = -1;
     NewPosition.X       = -1;
     NewPosition.Y       = -1;
     }

void TGObject::GetGlobalPos(int fX,int fY,int WhoCommand)
     {
     if (((fY-Y) == 0)&&((fX-X) == 0))
        return;
     NewPosition.GlobalX = fX;
     NewPosition.GlobalY = fY;
     NewPosition.GlobalAngle = StdAngl(atan2( (double)fY-Y, (double)fX-X)/OGIR);
     NewPosition.GlobalR = sqrt( (double)kvdrt(fY-Y ) + kvdrt(fX-X));

     if (WhoCommand == MAN)
         OnMission = true;


     }

void TGObject::GetGlobalPos(float R,float Angle,int WhoCommand)
     {
     int DeltaX = round(R * cos(Angle*OGIR));
     int DeltaY = round(R * sin(Angle*OGIR));

     NewPosition.GlobalX = X + DeltaX;
     NewPosition.GlobalY = Y + DeltaY;
     if ((NewPosition.GlobalX > MAP_WIDTH)&&(NewPosition.GlobalX<0)&&(NewPosition.GlobalY>MAP_HEIGHT)&&(NewPosition.GlobalY<0))
        bool Stop = 0;

     NewPosition.GlobalAngle = Angle;
     NewPosition.GlobalR = R;

     if (WhoCommand == MAN)
         OnMission = true;

     }


void TGObject::SetGlobalAngle_R()
     {
     if(NewPosition.GlobalX >=0)
        {
        int GlobalDeltaX = NewPosition.GlobalX-X;
        int GlobalDeltaY = NewPosition.GlobalY-Y;


        if ( (GlobalDeltaX!=0)||(GlobalDeltaY!=0) ) // ���� ���� ���� ����...
           {
           if (GlobalDeltaX!=0) // �� ��������� ������� �� ����
              NewPosition.GlobalAngle = StdAngl(atan2( (double)GlobalDeltaY, (double)GlobalDeltaX)/OGIR);
              else  {
                    if (GlobalDeltaY > 0)
                       NewPosition.GlobalAngle = 90;
                       else
                       NewPosition.GlobalAngle = 270;
                    }

              NewPosition.GlobalR =  sqrt ( (double)kvdrt(GlobalDeltaX)+kvdrt(GlobalDeltaY) );
           }                   // end of <�� ��������� ������� �� ����>
           else
           NewPosition.GlobalAngle = 0;


        }
        else
            {
            NewPosition.GlobalAngle = 0;
            NewPosition.GlobalR = -1;
            }
     }


void TGObject::Stop()
     {
     NewPosition.GlobalX = -1;
     NewPosition.X = -1;
     }

/*void TGObject::UnStop()
     {
     //���� ��������
     }*/
void TGObject::SetStatus() //����������� �� ������ ���������, �� � ������ ��������� ��� ������ ����� (��.DoAtion)
     {
     OldStatus = Status;
     //moving
     if (((OldX!=X)||(OldY!=Y))  ||  ((OldAngle!= Angle)) )
        Status.Moving = true;
        else
        Status.Moving = false;

     if ((NewPosition.GlobalX!=-1)&&(X!=NewPosition.GlobalX) && (Y!=NewPosition.GlobalY))
        Status.MustMove = true; 
     //Attack
     if (((PrimaryEnemy!=NULL)&&(PrimaryEnemy->Is))||(PrimEnemy.Type!=-1))
        Status.Attack = true;
        else
        Status.Attack = false;

     //Fire
        Status.Fire = false; //���� ���� ������ �������� - �� � �-� ���� �� ������ true.
     Status.InShock = false;

	 Status.Deading = ((!Is)&&(OldIs))?true:false;

     }


bool TGObject::CanKill(TGObject *Enemy)
     {
     /*
     ���������� ��������� ������ � ���������
     */
     if (POWER >= Enemy->POWER)
        return true;
          else
          return false;

     }
void TGObject::GetEnemy(TGObject *Enemy)
     {
//     if (Enemy == NULL) {ShowMessage("������� ������� �-� GetEnemy()!!"); return;};
     Status.MustMove = false;
     FreePosition();
     PrimaryEnemy = Enemy;
     PrimEnemy.Type = Enemy->TeamIndex;
     PrimEnemy.Index = Enemy->Index;

     int DeltaX = Enemy->X - X;
     int DeltaY = Enemy->Y - Y;

     PrimEnemy.R = sqrt( (double)kvdrt(DeltaX )+kvdrt(DeltaY));
     PrimEnemy.Angle = StdAngl(atan2( (double)DeltaY, (double)DeltaX)/OGIR);
     ACTION = ATTACK_ACT;
     }
void TGObject::GetEnemy(TForFound Enemy)
     { 
     FreePosition();
     Status.MustMove = false;
     PrimEnemy = Enemy; /*PrimaryEnemy = Enemy;*/ 
     ACTION = ATTACK_ACT;
     }
void TGObject::SetEnemy()
     {
     if (PrimEnemy.Type>=0)
        {
         if((PrimaryEnemy!=NULL) && (PrimaryEnemy->Is) ) //���� �� ����� ������ � ���� PrimaryEnemy
            {//�� ����  ����������� �� � ���� PrimEnemy, � ��� �� ���������� ���� � ���������� �� �����
             int DeltaX = PrimaryEnemy->X - X,
                 DeltaY = PrimaryEnemy->Y - Y;

                 PrimEnemy.Angle = StdAngl(atan2( (double)DeltaY, (double)DeltaX)/OGIR);
                 PrimEnemy.R = sqrt( (double)kvdrt(DeltaX )+kvdrt(DeltaY));
            }//else
             //PrimEnemy.Type =-1;

        }
        else
        {
        PrimaryEnemy =NULL; //���� ����� ����� ������� ���� PrimaryEnymy
         //����������� �������� ��������� ���� � �-� TGSociety::SetEnemy(t,i)


        if (ACTION == ATTACK_ACT)  ACTION = WAIT_ACT;
        }
     }


void TGObject::Refresh()
     {
     if ((PrimaryEnemy!=0)&&!PrimaryEnemy->Is)
          PrimaryEnemy = NULL;
//     GetTempX(OldX);
//     GetTempY(OldY);
	 if (Live >= 0)
        {
        GETPOWER();
        }
     }

//---------------------TGGroup-----------------------------------------------
//              ������ �������� , �������� � ������
//---------------------------------------------------------------------------
void TGGroup::Init(TGSociety *_Society)
     {
     Society          = _Society;
     Mission.Init(this);
     OnMission        = false;
     Is               = false;
     Distination.X    = -1;
     Distination.Y    = -1;
     EGroup           = 0;  // ��������� ������ �� �������
     EUnit            = 0;   // ��������� ���� �� ������


     Units = new TUnitStruct [NUMBER_OF_UNITS];
     for (int i = 0; i<NUMBER_OF_UNITS; i++)
         {
         Units[i].Is  = false;
//         EUnits[i].Is = false;
         Units[i].IndEnemy = -1;
         Units[i].PUnit    = NULL;
         }
     Live = Number = X = Y = 0;

     if (Number!=0)
//         ShowMessage(" �� �������� �������� ������������ ( �-� TGGroup::Init() )");
     NumberToKill=0; //���������� ������ ������� ���� �����;
     NumberOfKilledEnemies=0; //���������� ������ ������;


                                                      // Include In Base
     IncludedInBase = new TIncInBase* [NUMBER_OF_TEAMS];
     for (int t = 0;t<NUMBER_OF_TEAMS;t++)
         {
         IncludedInBase[t] = new TIncInBase [NUMBER_OF_GROUPS];
         for (int g = 0;g<NUMBER_OF_GROUPS;g++)
             IncludedInBase[t][g].Yes = false;

         }

     GetPower();
    }
void TGGroup::Refresh()
     {
     GetPower();
     Analis();
     }
void TGGroup::GetPower()
     {
     POWER = 0;
     for (int i = 0; i<Number;i++)
         {
         if (Units[i].PUnit == NULL)
            {
//            ShowMessage("������ � ������� TGGroups::GetPower");
            break;
            }
         POWER = Units[i].PUnit->POWER;
         }
     }


void TGGroup::GetNewPos(int X,int Y)
     {
     if ((Distination.X == X)&&(Distination.Y == Y))
         return;

     Distination.X = X;
     Distination.Y = Y;
     if ((!InSpace(0,MAP_WIDTH,X,true,false))||(!InSpace(0,MAP_HEIGHT,Y,true,false)))
         return;

     if (Number==0) return;
     /*
     ������, ���� �������� �����: ���������� ���� �� ������� ���� ��������
     ������, � ���������� � ������� ������� ����� ���� ������ �� ����� ����������
     */
     if (Team == 5)
        bool Stop = true;
     //���������� ����� ������� �� ������� � ������ (� ��������� ���������)
     int DeltaX = X - Units[0].PUnit->X,
         DeltaY = Y - Units[0].PUnit->Y;
     float
         R = sqrt( (double)kvdrt(DeltaY ) + kvdrt(DeltaX)),
         Angle =(DeltaX!=0)?StdAngl(atan2( (double)DeltaY, (double)DeltaX)/OGIR):((DeltaY>0)?90:(-90));
         //���� ������� ������
         //���� ������� != 0 ���������� ���� �� ��������� �������
         //����� ���� ������� ������ 0 �� ���� ����� 90�� ����� -90��




     for (int i = 0;i<Number;i++)
           {
           if (!Units[i].Is) continue;
           TXY XY,A;
           A.X = -1;
           XY.X = X + (Units[i].PUnit->X - Units[0].PUnit->X);
           XY.Y = Y + (Units[i].PUnit->Y - Units[0].PUnit->Y);
           Units[i].PUnit->SectionWay.GetA(A);
           Units[i].PUnit->SectionWay.GetB(XY);
           Units[i].PUnit->SectionWay.FindWay();// ==>            Units[i].PUnit->GetGlobalPos(R,Angle,MAN);


//           Units[i].PUnit->Points.Init(Units[i].PUnit);
//           Units[i].PUnit->Points.GetA();
//           Units[i].PUnit->Points.GetB(XY);
//           Units[i].PUnit->Points.Creek(Units[i].PUnit->Points.B);


           }
     }
void TGGroup::GetUnit(TGObject *Unit)
     {
     if (Unit == NULL)
     bool Stop = true;
     if (this == NULL)
     bool Stop = true;
     int i=0;

     if (Number>=0) //��� ��� ������ ����� ������ ����� � ������ ��� ���� �����
     {
     while (
           (i<Number)
           &&
           (Unit->POWER < Units[i].PUnit->POWER)
           ) //���� ���� �� ��� ��������
           i++;


     if (Number == NUMBER_OF_UNITS)
        {
//        ShowMessage("� ������ "+IntToStr(Team) + " ; "+IntToStr(Index)+" ���� ���!");
        return;
        }
     if (Units[i].PUnit!=NULL) //���� ���� ���� �������� ��
     for (int in = (Number-1); in >= i; in--) //�������� ���� �� 1 ������
         {
         Units[in+1] = Units[in];
         Units[in].PUnit->IndexInGroup++;
         }
     }
    // ���������
     Units[i].PUnit = Unit;
     Units[i].Is = true;
     Units[i].IndEnemy = -1;
     Number++;

     /*
     ������, ��, Unit, ��������� ������ ���� ������,
     ������� ������ ��� ���� ������.. ������� ��������� �� � ������ �� ������� !
     */
     (*Unit).InGroup = true;
     (*Unit).GroupIndex = Index;
     (*Unit).GroupTeam = Team;
     (*Unit).IndexInGroup = i;
     /*
     Unit(������ ���� Damage �� ������� Init() - ���� �� ������ ): "������� ��������� ��� ���� ������,��������� �� � ������ �� �������! "
      */
     }
void TGGroup::Sort()
     {
     int i = 0 ;
     while (i<Number)
           {
           int MAX = Units[0].PUnit->POWER,
               IndexMAX = 0;
           //1. ������� ����. ����-�
           for (int j = i; j<Number;j++)
                    {
                    if (MAX < Units[j].PUnit->POWER)
                       {
                       MAX = Units[j].PUnit->POWER;
                       IndexMAX = j;
                       }
                     }
           //2. ������ ������ � ������������ �������� �������.
           TUnitStruct TempUnit = Units[IndexMAX];
           Units[IndexMAX] = Units[i];
           Units[i] = TempUnit;



           }
     }

void TGGroup::SetAttack(TATTACK_MODE PARAMETER)
     {
     for (int i = 0; i < Number; i++)
         {
         if (Units[i].PUnit->Is) Units[i].PUnit->SetAttack(PARAMETER);
         }
     }
void TGGroup::GetEnemy(TGGroup *Enemy)
     {
     EGroup = Enemy;
     EUnit = NULL;
     }

void TGGroup::GetEnemy(TGObject *Enemy)
     {
     EUnit = Enemy;
     EGroup = NULL;
     }

void TGGroup::Free()
	{
	OnMission = false;
	NumberToKill = 0;
	NumberOfKilledEnemies = 0;
	}

void TGGroup::Analis()
     {
     //��� ���� ������� ����� ����������� ����� Dye �� ����� ������� ������ (� ������ TGSociety)
     //� ��� ����� ����� ������ TGGroup::CanKill (TGGroup *EnenyGroup)
     if ((EGroup!=NULL)&&(!EGroup->Is))
        EGroup = NULL;
     if (EUnit!= 0 && !EUnit->Is)
        EUnit = NULL;

     if (EGroup==NULL)
        {
        if (EUnit != NULL)
           {
           for (int i = 0;i<Number;i++)
                Units[i].PUnit->GetEnemy(EUnit);
		   return; //������� ���������� �� ����� ����� � ������� �� �-�
           }
		   else
		   {
			/*��������� �����*/   if (NumberOfKilledEnemies>0)Free(); //���� ���� �����������
		   }
        return;//� ���� ��� �� �����-�����, �� ������-����� �� ��� ������� "�������������" ������
        }

//������� ������� - ��. ������� "������ "����" III ����� 2. ������������"
// �������� ������ �� 1-�� �����, ����� �� 2-�� � �.�.
int i    = 0; //������ ������� ���� ������;
int e    = 0; //������ ������� ��������� ������;
int sum  = 0; //����, ������������������� �����;
int epow = 0; //���� �����;

for (i = 0; i<Number;i++)
    {
    epow = EGroup->Units[e].PUnit->POWER;
    if (sum < epow)
       {
       sum+=Units[i].PUnit->POWER;
       Units[i].PUnit->GetEnemy(EGroup->Units[e].PUnit);
       }
       else
           {
           sum = 0;
           e++;
           if (e>=(EGroup->Number))
               break;
           }
    }


//���� ��������� ������������ ����������
e = 0;
for (i = i+1; i<Number; i++)
    {
	
	int _i = 0;
    float a = float(EGroup->Number) / (float(Number) - float(i)) ;
	if ((a < 1)&&(e<EGroup->Number))
	   Units[i].PUnit->GetEnemy(EGroup->Units[e].PUnit);
	   else 
	        while ((_i <= a)&&(i<Number)&&(e<EGroup->Number))
			      {
				  Units[i].PUnit->GetEnemy(EGroup->Units[e].PUnit);
				  _i++;
				  e++;
			      }

	}
    





/*     //���� �� �������
     //������������ ���� ������ �� ������ ��������
     //�.�. �� ������ ������ � �������
     //���� ������ - �� �������, ����������� � �������
     int NumberOfEnemies;

     NumberOfEnemies = EGroup->Number;
     if (EGroup->POWER == 0) return;
//    if (POWER>EGroup->POWER)
         {
         for (int i = 0; i<Number; i++)
             if (Units[i].PUnit->Is && EGroup->Units[0].PUnit->Is)
             Units[i].PUnit->GetEnemy(EGroup->Units[0].PUnit);
         }
*/
/*       else
         for(int i = 0; i<Number; i++)
            {
            if (Units[i].PUnit->Is && EGroup->Units[0].PUnit->Is)
            Units[i].PUnit->GetEnemy(EGroup->Units[NumberOfEnemies-1].PUnit);
            }*/
     }

void TGGroup::TMission::Init(TGGroup *_OurGroup)
          {
          bool Articulate = false;
          bool Complete = true;
          OurGroup = _OurGroup;

          // ������������� ���� � �������.
          // 1. ������������� ���� IncInBase
/*
          if (OurGroup->Society!=NULL)
          for (int t = 0; t < NUMBER_OF_TEAMS; t++)
              {
               for (int i = 0; i < NUMBER_OF_GROUPS; i++)
                 if ((OurGroup->Society->Groups[t]!=NULL)&&(OurGroup->Society->Groups[t][i].Is))
                  {
                  OurGroup->Society->Groups[t][i].IncludedInBase[t][i].Yes = false;
                  OurGroup->Society->Groups[t][i].IncludedInBase[t][i].Index = -1;
                  }
              for (int i = 0; i < NUMBER_OF_UNITS; i++)
                if ((OurGroup->Society->GObject[t]!=NULL)&&(OurGroup->Society->GObject[t][i].Is))
                  {
                  OurGroup->Society->GObject[t][i].IncludedInBase[t][i].Yes = false;
                  OurGroup->Society->GObject[t][i].IncludedInBase[t][i].Index = -1;
                  }
              }
*/
          EnemyNumber = 0;
          PreferXY = false;
          XY.X = -1;
          XY.Y = -1;
          LastDataSend.XY.X = -1;
          LastDataSend.XY.Y = -1;
          }

bool TGGroup::TMission::GetNewData(short int LOG_DATA, TGGroup::TDATA _DATA)
     {
     if (EnemyNumber>=MAX_ENEMIES)
         return false;



     Complete = false;

     if (LOG_DATA==2)
        {
                // ���� � ��������� ����� ����.
		int Team  = OurGroup->Society->MAP[_DATA.XY.X][_DATA.XY.Y].Team;
		if  (InSpace(0,NUMBER_OF_TEAMS,Team,true,false)&&(Team!=OurGroup->Team))
		    {
                    TDATA SECOND_DATA;
			int Index = OurGroup->Society->MAP[_DATA.XY.X][_DATA.XY.Y].Index;
			/*if (OurGroup->Society->GObject[Team][Index].InGroup)
			   {
			   int GT = OurGroup->Society->GObject[Team][Index].GroupTeam;
			   int GI = OurGroup->Society->GObject[Team][Index].GroupIndex;
			   SECOND_DATA.GGroup = &OurGroup->Society->Groups[GT][GI];
			   SECOND_DATA.Is     = true;
			   SECOND_DATA.GObject=NULL;
			   return GetNewData(0,SECOND_DATA);
			   }*/


			SECOND_DATA.GObject = &OurGroup->Society->GObject[Team][Index];
			SECOND_DATA.Is      = true;
			SECOND_DATA.GGroup  = NULL;
			return GetNewData(1,SECOND_DATA);
			}
                // �����:
        if  (
            (PreferXY)
            ) //���� ������������ ������,������ �����, �� ���������� ������� "������" � ������� ����������
              //(�������� ����������, ����������)
           {
		   if (Enemy[0].Object_Group == 2)
		       {
			   Enemy[0].Object = _DATA;
			   return true;
			   }

           if ((OurGroup->Team == 2) && (OurGroup->Index == 6))
              bool stop = 1;

           EnemyNumber++;

           for (int i = EnemyNumber-1;i>=0;i--) //��������� ��� �� ���� �������� �� ������
                MoveData(i,i+1); //���� i>=MAX_ENEMIES ���������� ���������
           Enemy[0].Object_Group = LOG_DATA;
           Enemy[0].Object       = _DATA;
           }
           else
               {
               XY.X = _DATA.XY.X;
               XY.Y = _DATA.XY.Y;
               }

        }

      if (!_DATA.Is)
         return false;

      //�������� �� ������������ ������ (������, ��� ������(������) � ���� ��� �� ��������)
      if (LOG_DATA==1)
          {
          if (_DATA.GObject->IncludedInBase[OurGroup->Team][OurGroup->Index].Yes)
          return false;
          if (!_DATA.GObject->Is)
          return false;

          if ((OurGroup->Team == 2) && (OurGroup->Index == 6))
             bool stop = 1;

          EnemyNumber++;
          _DATA.GObject->IncludedInBase[OurGroup->Team][OurGroup->Index].Yes = true;
          _DATA.GObject->IncludedInBase[OurGroup->Team][OurGroup->Index].Index = EnemyNumber-1;

		  Enemy[EnemyNumber-1].Object_Group = LOG_DATA;
          Enemy[EnemyNumber-1].Object = _DATA;


          }

          else
          if (LOG_DATA == 0)
          {
          if (_DATA.GGroup->IncludedInBase[OurGroup->Team][OurGroup->Index].Yes)
          return false;
          if (!_DATA.GGroup->Is)
          return false;

          if ((OurGroup->Team == 2) && (OurGroup->Index == 6))
             bool stop = 1;

          EnemyNumber++;
          	  Enemy[EnemyNumber-1].Object_Group = LOG_DATA;
          Enemy[EnemyNumber-1].Object = _DATA;


          _DATA.GGroup->IncludedInBase[OurGroup->Team][OurGroup->Index].Yes = true;
          _DATA.GGroup->IncludedInBase[OurGroup->Team][OurGroup->Index].Index = EnemyNumber-1;
          }


     }
bool TGGroup::TMission::MoveData(int from_i,int on_i)
     {
     if (
        (from_i >=MAX_ENEMIES)
        ||
        (!(Enemy[from_i].Object.Is || Enemy[from_i].Object_Group!=2))
        )
        return false;  //������


     if (InSpace(0,MAX_ENEMIES,on_i,true,false) )
        {
        Enemy[on_i] = Enemy[from_i]; // ��� ���������� ������� "����"(���� ����) � ������ � �������� "on_i"
                                     // ��������.
        
        if ((on_i >=EnemyNumber)&&((Enemy[on_i].Object.Is)||(Enemy[on_i].Object_Group == 2)))
           EnemyNumber++;
        if  (Enemy[on_i].Object.Is)    
            {         
            if (Enemy[on_i].Object_Group == 0) // �� ����� �������� ��� ��������� ��������� ����� � ���, ��� 
                                               // ����� �������� �� ��������� � ���� ����
            Enemy[on_i].Object.GGroup->IncludedInBase[OurGroup->Team][OurGroup->Index].Index  = on_i;
    
            if (Enemy[on_i].Object_Group == 1)
            Enemy[on_i].Object.GObject->IncludedInBase[OurGroup->Team][OurGroup->Index].Index = on_i;
            }
        }

        FreeDataCell(from_i);        //����������� ��� "���������� ������ ����������" (����).
      
     return true;
     }

void TGGroup::TMission::FreeDataCell(int i)
     {
     if (i >= MAX_ENEMIES) return;//������
     //���� � ������ ���������� � �����
          if ((OurGroup->Team == 2) && (OurGroup->Index == 6))
             bool stop = 1;

     if ((i == (EnemyNumber-1))&&((Enemy[i].Object.Is)||(Enemy[i].Object_Group==2)))
        EnemyNumber--; // ����� ���� �� �������� � ����������� ����� �������,



     if ((Enemy[i].Object_Group!=2)&&(Enemy[i].Object.Is))
        
        if (Enemy[i].Object_Group == 0)  //��������� - � ������
            //�������, ��� ����� �����, ��� �� ��������� �� ����� ����
           {
           Enemy[i].Object.GGroup->IncludedInBase[OurGroup->Team][OurGroup->Index].Yes = false;
           Enemy[i].Object.GGroup->IncludedInBase[OurGroup->Team][OurGroup->Index].Index = -1;
           }
           else  
               {
               if (Enemy[i].Object_Group==1)
                //��� � �����
                //�������, ��� ����� �����, ��� �� ��������� �� ����� ����
               {
               Enemy[i].Object.GObject->IncludedInBase[OurGroup->Team][OurGroup->Index].Yes = false;
               Enemy[i].Object.GObject->IncludedInBase[OurGroup->Team][OurGroup->Index].Index = -1;
               }
                  else 
                      {
                      Enemy[i].Object.Is    = false; //�� ������ ������
                      Enemy[i].Object_Group = 0;
                      }
               }
     
        
     //��� ������� - �� � �������! - ���� ����� - ��� !
     Enemy[i].Object.Is = false; 
     
     }



void TGGroup::TMission::DoMission()  //��������� ������� � ��� ������� ��������...
                                     // � ����... ������������� ������� � ��� �������! .. � ��� ����� .. ������!
     {
     // ���� ������� ������:
     // ���� ������������ �������� ���������� ����, �� ������ ��� ���������� � ����� �����������, ��������
     // �������� ������� GetNewData() ����� ��������� � �� �������
     // � ���� ��������������� ����� �� ..����, �� ��� ���������� � �����������, ����� ��, �������� ��������
     // GetNewData, ��������� �, ���������� ��� ��� ���������������� ��������� XY;
     // � ����� ��� ������, ������, ��� �������� ������� �������� ������, �� ���� �� ���� �����������.
     // ��� ���.

         if (Enemy[0].Object_Group==1)                                                // ���� �� ����������� ������
            {
/*          if ((Enemy[0].Object.GObject!=NULL)&&(!Enemy[0].Object.GObject->Is))*/

//          �������� ���������
            if ((Enemy[0].Object.GObject==NULL)||(!Enemy[0].Object.GObject->Is))
//          ------------------

            return;
            OurGroup->GetEnemy(Enemy[0].Object.GObject);
            }

            else
                {
                if  (Enemy[0].Object_Group == 0)
                    {
/*                  if ((Enemy[0].Object.GGroup!=NULL)&&(!Enemy[0].Object.GGroup->Is))*/

//                  �������� ���������.
                    if ((Enemy[0].Object.GGroup==NULL)||(!Enemy[0].Object.GGroup->Is))
//                  ------------------

                    return;
                    OurGroup->GetEnemy(Enemy[0].Object.GGroup);
                    }
                    else
                        OurGroup->GetNewPos(Enemy[0].Object.XY.X,Enemy[0].Object.XY.Y);



                }

        if ((!PreferXY) &&(XY.X>=0))
           {
           if (Complete||(!this->OurGroup->Is))
               return;
           OurGroup->GetNewPos(XY.X,XY.Y);
           }
        }
bool TGGroup::TMission::CheckOnComplete()
     {
     if (!(this->OurGroup->Is))
         return true; // ������ �� � ���� ���� ���������...

     Complete = false;
     int z = POSSIBLY_XY_MISTAKE;
     if (
        (!PreferXY&&XY.X<0)
        ||
        (PreferXY&&(Enemy[0].Object_Group!=2))
        )
         {
         Complete = true;
         return true;
         }

     for (int i = 0; i<OurGroup->Number;i++)
       {
       TGObject* Unit = OurGroup->Units[i].PUnit;
       if (
          /*(EnemyNumber==0) //���� ������ ��������� � ���� �� ������.
          &&*///

          // �������� �������
          (
           (!PreferXY)
           &&
           (Enemy[0].Object_Group < 2)
           &&
           (EnemyNumber == 0)
          )
          ||
          // ----------------
          (
           PreferXY
           &&
           (Enemy[0].Object_Group == 2)
           &&
           InSpace(Enemy[0].Object.XY.X-z,Enemy[0].Object.XY.X+z,Unit->X,true,true)
           &&
           InSpace(Enemy[0].Object.XY.Y-z,Enemy[0].Object.XY.Y+z,Unit->Y,true,true)
          )

          ||

          (
           !PreferXY
           &&
           InSpace(XY.X-z,XY.X+z,Unit->X,true,true)
           &&
           InSpace(XY.Y-z,XY.Y+z,Unit->Y,true,true)
          )
          )
          {
          Complete = true;
          for (int i = 0; i<EnemyNumber; i++)
              MoveData(i,i-1);
          return Complete;
          }
          else return false;
       }
       PreferXY = OurGroup->Society->Teams[OurGroup->Team].PreferXY;
       return Complete;
     }

void TGGroup::TMission::RefreshBase()
      {
      for (int i = 0;i<=EnemyNumber;i++)
          {
          bool ToDel = ((Enemy[i].Object_Group == 1) && Enemy[i].Object.GObject!=NULL && Enemy[0].Object.GObject->Is)?false:(((!Enemy[i].Object_Group) && Enemy[i].Object.GGroup!=NULL && Enemy[i].Object.GGroup->Is)?false:true);
          // ����� �������� �����: ���� ���� - ��� ������ � ���� ��� ��� �� ����������, �� �� �������; ����� ������ ������� ���� ���� - �� ��� ��� �������������� ������.
          // �������� �� NULL �� ������� ��������������� ������� ��������� - ������ ����� ���� ...
          if (ToDel)
             {
             //Enemy[i] = Enemy[EnemyNumber-1];
             //Enemy[EnemyNumber-1].Object.Is = false;
             //EnemyNumber =(EnemyNumber>0)?EnemyNumber-1:0;
			 for (int i = 0; i<EnemyNumber; i++)
              MoveData(i,i-1);
             }
          }

      }






//--------------------TGSociety----------------------------------------------
//� ���� ������ ������� �������������� ��������, � �� ������������ �� �����
//---------------------------------------------------------------------------
void TGSociety::LoadObjectMap()
     {
	 FILE *File;
	 
	 unsigned char Z[2];
     int* Index = new int [NUMBER_OF_TEAMS];
	 for (int t = 0; t<NUMBER_OF_TEAMS; t++)
	     Index[t] = 0;

     int  i =0;
     File = fopen("Images/ObjectMap.raw","r");
     for (int y = 0; y < MAP_HEIGHT; y++)
	     for (int x = 0; x < MAP_WIDTH; x++)
	     {
     	  fread(&Z,sizeof(char)*2,1,File);
		  if ((Z[0] == -1) || (Z[1] == -1)||(Z[0] == 255)||(Z[1] == 255))
		     continue;
		  else
		      {
              bool xxxx = true;
              if ((Index[Z[0]] < NUMBER_OF_UNITS)
                  &&
                  PutUnit(Z[0],Index[Z[0]],Z[1],x,y)
                  )
		         Index[Z[0]] ++;
     		  };

	     }
	 fclose(File);
     return;


	 }
void TGSociety::LoadMap()
     {
     FILE* f = fopen("Images/Land.raw","r+b");
     for (int y = 0;y<MAP_HEIGHT;y++)
     for (int x = 0;x<MAP_WIDTH;x++)
         {
          unsigned char z;
          fread(&z,sizeof(char),1,f);
          MAP[x][y].Z = z;
         }

     fclose(f);
     }
void TGSociety::LoadWeapons()
     {
     FILE *File;
     File = fopen("weapons.ini","r");
     for (int i = 0; i<NUMBER_OF_STANDARTS;i++)
         {
         fscanf(File,"%i",&Weapon[i].Laser);
         fscanf(File,"%i",&Weapon[i].Target);

		 fscanf(File,"%i",&Weapon[i].TargetSpeed);
         fscanf(File,"%i",&Weapon[i].TargetDamage);
         fscanf(File,"%i",&Weapon[i].TargetRadius);
         fscanf(File,"%i",&Weapon[i].TargetRoof);
         fscanf(File,"%i",&Weapon[i].LaserRange);
		 fscanf(File,"%i",&Weapon[i].LaserDamage);
         }
     /*
	 
        bool Laser-------- 1  
		bool Target------- 2
		int  TargetSpeed--- 3
		int  TargetDamage-- 4
		int  TargetRadius-- 5
		int  TargetRoof---- 6

		int  LaserRange---- 7
		int  LaserDamage--- 8
	 */
	 fclose(File);
     }

void TGSociety::LoadStandarts()
     {
     Standart = new TGStandart [NUMBER_OF_STANDARTS];
     FILE *File;
     File = fopen("Standarts.ini","r");
     for (int i = 0; i<NUMBER_OF_STANDARTS;i++)
         {
         fscanf(File,"%s",&Standart[i].FileName);
         fscanf(File,"%i",&Standart[i].Live);
         fscanf(File,"%i",&Standart[i].Sight);
         fscanf(File,"%i",&Standart[i].Speed);
         fscanf(File,"%i",&Standart[i].AngleSpeed);
         fscanf(File,"%i",&Standart[i].AttackPeriod);
         fscanf(File,"%i",&Standart[i].Armor);
         fscanf(File,"%i",&Standart[i].IndexWeapon);
         fscanf(File,"%i",&Standart[i].Width);
         fscanf(File,"%i",&Standart[i].Thinking);
         fscanf(File,"%i",&Standart[i].NVO);
         fscanf(File,"%i",&Standart[i].N);
         fscanf(File,"%i",&Standart[i].CanAttack);
         fscanf(File,"%i",&Standart[i].CanAttackMove);
         fscanf(File,"%i",&Standart[i].CanMove);
		 fscanf(File,"%i",&Standart[i].MaxModelFrames);
         }
     fclose(File); 
     }

bool TGSociety::LoadConfig()
     {
     //  ������ �������� ������.

     //  1.  ������ �����      - 2 �
     //  2.  ������ �����      - 2 �
     //  3.  ����� ������      - 2 �
     //  4.  ����� ������      - 2 �
     //  5.  ����� ��������    - 2 �
     //  6.  ���������� ������ - 2 �

     //  7.  ������:
     //          1.������ ��������� ���������
     //          2.���������� ��� ���� ����� ������

     //  8.  �������:
     //          1. ������ ��������� ���������
     //          2. ���������� � ���� ����� ��������:

     //  9.  �����:
     //          1. ������ ��������� ���������
     //          2. ���������� � ���� ������� ����� ([�][�])


     //  11. ������� (�����):
     //          1. ������ ��������� ���������
     //          2. ���������� � ���� �������� ([�������][������ � �������])

     //  12. �������:
     //          1. ������ ��������� ���������
     //          2. ���������� � ���� ��������



     // ������ � ������������ � ��������.
     FILE* f = fopen(MAP_PATH,"r+b");

     //  1.
     if (MAP != NULL)
        {
        for (int x = 0; x<MAP_WIDTH;x++)
            {
            //  1.1. ������� ������ ���������� ��� �����.
            delete MAP[x];
            MAP[x] = NULL;
            }
        delete MAP;
        MAP = NULL;
        }
     fread(&MAP_WIDTH,sizeof(MAP_WIDTH),1,f);

     //  2.
     fread(&MAP_HEIGHT,sizeof(MAP_HEIGHT),1,f);
     //  3.
     fread(&NUMBER_OF_UNITS,sizeof(NUMBER_OF_UNITS),1,f);

     //  3.1. ������� ������ ���������� ��� ������.
     if (GObject != NULL)
        {
        for (int t = 0; t<NUMBER_OF_TEAMS; t++)
            {
            delete[] GObject[t];
            GObject[t] = NULL;
            }
        delete[] GObject;
        GObject = NULL;
        }

     //  4.
     fread(&NUMBER_OF_TEAMS,sizeof(NUMBER_OF_TEAMS),1,f);
     //  5.
     fread(&NUMBER_OF_STANDARTS,sizeof(NUMBER_OF_STANDARTS),1,f);

     //  6.
     fread(&NUMBER_OF_WEAPONS,sizeof(NUMBER_OF_WEAPONS),1,f);


     //  7.1.
     short int WP_SIZE;
     fread(&WP_SIZE,sizeof(WP_SIZE),1,f);
     if (WP_SIZE != sizeof(TWeapon))
        {
        strcpy(LAST_MESSAGE,FILE_READ_ERROR);
        return false;
        }

     //  7.2

     //  7.2.1 ������� ������ ���������� ��� ������.
     if (Weapon != NULL)
        {
        delete[] Weapon;
        Weapon = NULL;
        }
     Weapon = new TWeapon [NUMBER_OF_WEAPONS];
     //  7.2.2.
     for (int i = 0; i < NUMBER_OF_WEAPONS; i++)
         fread(&Weapon[i],WP_SIZE,1,f);

     //  8.1.
     short int ST_SIZE;
     fread(&ST_SIZE,sizeof(ST_SIZE),1,f);
     if (ST_SIZE != sizeof(TGStandart))
        {
        strcpy(LAST_MESSAGE,FILE_READ_ERROR);
        return false;
        }
     //  8.2.
     //  8.2.1. ������� ������ ���������� ��� ������� ������.

     if (Standart!= NULL)
        {
        delete[] Standart;
        Standart = NULL;
        }

     Standart = new TGStandart [NUMBER_OF_STANDARTS];
     //  8.2.2.
     for (int i = 0; i < NUMBER_OF_STANDARTS; i++)
     fread(&Standart[i],ST_SIZE,1,f);

     //  9.1.
     short int MAP_SIZE;
     fread(&MAP_SIZE,sizeof(MAP_SIZE),1,f);
     if (MAP_SIZE != sizeof(TMap))
        {
        strcpy(LAST_MESSAGE,FILE_READ_ERROR);
        return false;
        }

     //  9.2.
     MAP = new TMap* [MAP_WIDTH];
     for (int x = 0; x < MAP_WIDTH; x++)
         {
         MAP[x] = new TMap [MAP_HEIGHT];
         for (int y = 0; y < MAP_HEIGHT; y++)
             fread(&MAP[x][y],MAP_SIZE,1,f);
         }



     //  10.1.
     short int OBJ_SIZE;
     fread(&OBJ_SIZE,sizeof(OBJ_SIZE),1,f);
     if (OBJ_SIZE != sizeof(TGObject))
        {
        strcpy(LAST_MESSAGE,FILE_READ_ERROR);
        return false;
        }

     //  10.2.
     GObject = new TGObject* [NUMBER_OF_TEAMS];
     for (int t = 0; t<NUMBER_OF_TEAMS; t++)
         {
         GObject[t] = new TGObject [NUMBER_OF_UNITS];
         for (int i = 0; i<NUMBER_OF_UNITS; i++)
             {
             TGObject Unit;
             fread(&Unit,OBJ_SIZE,1,f);
             int UType = Unit.Type;

             GObject[t][i].Init(this,NUMBER_OF_STANDARTS,UType,t,i);
             GObject[t][i] = Unit;

             // ������������� ������������� �������.
             GObject[t][i].IncBaseInit();

             // ��������� �� ��������.
             GObject[t][i].Society = this;

             // ��������� � ������� TARGET
             GObject[t][i].Target.Society = this;
             GObject[t][i].Target.OurObject = &GObject[t][i];
             }
         }

     //  11.1
     short int TM_SIZE;
     fread(&TM_SIZE,sizeof(TM_SIZE),1,f);
     if (TM_SIZE!=sizeof(TGSociety::TTeams))
        {
        strcpy(LAST_MESSAGE,FILE_READ_ERROR);
        return false;
        }
     //  11.2.1. ������� ������ ���������� ��� �������
     if (Teams!= NULL)
        {
        delete[] Teams;
        Teams = NULL;
        }
     //  11.2.2.
     Teams = new TTeams [NUMBER_OF_TEAMS];
     for (int t = 0; t<NUMBER_OF_TEAMS;t++)
         fread(&Teams[t],TM_SIZE,1,f);




     // �������� �����.
     fclose(f);
     return true;
     }





void TGSociety::GSocietyInit(bool first)
     {
      Dead = 0;


//   ������������� "�� ���������".

                                                 // ��� ������������� ��������

     GObject = new TGObject* [NUMBER_OF_TEAMS];
     for (int t = 0; t < NUMBER_OF_TEAMS; t++)
         GObject[t] = new TGObject [NUMBER_OF_UNITS];




													//�������
          Teams = new TTeams [NUMBER_OF_TEAMS];
	  {for (int t = 0; t<NUMBER_OF_TEAMS; t++)
			{
            Teams[t].Centre.X		  = MAP_WIDTH/DEBUG_TEAMS_INTERVAL;
			Teams[t].Centre.Y		  = MAP_HEIGHT/DEBUG_TEAMS_INTERVAL;
			Teams[t].MEN			  = (t == MAN_TEAM);
			Teams[t].Number			  = 0;
			Teams[t].SendXY.X		  = -1;
			Teams[t].SendXY.Y		  = 1;
            Teams[t].PreferXY		  = false;
            Teams[t].SelectedPreferXY = true;
			}

	  }
                                                          //������������� �����
        MAP = new TMap* [MAP_WIDTH];
        for (int j = 0; j < MAP_HEIGHT; j++)
                {
                MAP[j] = new TMap [MAP_HEIGHT];

                for (int i = 0; i < MAP_WIDTH; i++ )
                    {

                    MAP[j][i].Index = -1;
                    MAP[j][i].Team = -1;
					if(first)
                                                          // ������ ������� �����
                    MAP[j][i].Z = 2;
                    MAP[j][i].Tex=2;


		    if(MAP[j][i].Z<-1)
                      MAP[j][i].Tex=0;

		    if((MAP[j][i].Z>1)|(MAP[j][i].Z==-1))
                      MAP[j][i].Tex=1;

                    }
                }

	 //��������� ���������
     ViewMode    = false;
	 AttackMode  = false;
	 ChaseMode   = false;
	 MoveMode    = false;

//   ����� ������������� �� ���������.


//   �������� ������������ �� �����.

     LoadConfig();



//   �������� �������� ��������� �� ������������
      {
       Groups = new TGGroup* [NUMBER_OF_TEAMS];
       GroupsNumber  = new int [NUMBER_OF_TEAMS];
       {for (int t = 0;t<NUMBER_OF_TEAMS;t++)
            GroupsNumber[t] = 0;}

        for (int t = 0; t<NUMBER_OF_TEAMS;t++)    //������
          {
          Groups[t] = new TGGroup [NUMBER_OF_GROUPS];
          for(int i = 0;i<NUMBER_OF_GROUPS;i++)
            {
			Groups[t][i].Distination.X = -1;
			Groups[t][i].Is            = false;
			Groups[t][i].Live          = 0;
            Groups[t][i].Init(this);
            Groups[t][i].Team = t;
            Groups[t][i].Index = i;
            }
          }
       }

        // �����
        InitMapPassably();

        for (int j = 0; j < MAP_HEIGHT; j++)
                {
                for (int i = 0; i < MAP_WIDTH; i++ )
                    {

                                                         // ������ ������� �����
                    MAP[j][i].Tex=2;
		    if(MAP[j][i].Z<-1)
                      MAP[j][i].Tex=0;
		    if((MAP[j][i].Z>1)|(MAP[j][i].Z==-1))
                      MAP[j][i].Tex=1;

                    }
                }


      ANGLE_MATRIX_Init();                             //������� �������

                                                       //�������� �����

     {for (int i = 0; i<NUMBER_OF_KVADRATS;i++)
         for (int j = 0;j<NUMBER_OF_KVADRATS;j++)
             {
             Kvadrats[i][j].Number = 0;
             Kvadrats[i][j].Units = new TGObject* [MEN_IN_KVADRAT];
             for(int k = 0; k<MEN_IN_KVADRAT;k++)
                Kvadrats[i][j].Units[k] = NULL;

             Kvadrats[i][j].X = KVADRAT_WIDTH*i;
             Kvadrats[i][j].Y = KVADRAT_HEIGHT*j;
             Kvadrats[i][j].X_ =Kvadrats[i][j].X + KVADRAT_WIDTH;
             Kvadrats[i][j].Y_ =Kvadrats[i][j].Y + KVADRAT_HEIGHT;
             }
     }



//   ������������� ��������� �������
     for (int t = 0; t < NUMBER_OF_TEAMS; t++)
     for (int i = 0; i < NUMBER_OF_UNITS; i++)
         GObject[t][i].SectionWay.Init(this,&GObject[t][i],true);
     }


void TGSociety::InitMapPassably()
	{
	for (int x = 0; x<MAP_WIDTH;x++)
		for (int y = 0;y<MAP_HEIGHT;y++)
		{
		int RightDelta = (InSpace(x,MAP_WIDTH,x+1,true,false))?MAP[x+1][y].Z-MAP[x][y].Z:0;
        int UpDelta    = (InSpace(y,MAP_WIDTH,y+1,true,false))?MAP[x][y+1].Z-MAP[x][y].Z:0;   //in international system C

         if (abs(RightDelta) > MAP_PASSABLY_DELTA)
			{
			MAP[x][y].Team   = NUMBER_OF_TEAMS;
			MAP[x+1][y].Team = NUMBER_OF_TEAMS;
			}
		 if (abs(UpDelta) > MAP_PASSABLY_DELTA)
			{
            MAP[x][y].Team     = NUMBER_OF_TEAMS;
			MAP[x][y+1].Team   = NUMBER_OF_TEAMS;
			}
		}
	}


bool TGSociety::PutUnit(int Team, int Index,int Type, int X,int Y)
{
//�������� �� ������
if (Team>=NUMBER_OF_TEAMS)
   return false;
if (Type >= NUMBER_OF_STANDARTS)
    Type = 0;


int X1 = X - Standart[Type].Width/2,
    X2 = X + Standart[Type].Width/2,
    Y1 = Y - Standart[Type].Width/2,
    Y2 = Y + Standart[Type].Width/2;
//������ �� �����
int Result = true;
{
 for (int y = Y1-POSSIBLY_PUT_INTERVAL;y<=Y2+POSSIBLY_PUT_INTERVAL;y++) //���� �� ������������ �������������� ������� �� �����.
 for (int x = X1-POSSIBLY_PUT_INTERVAL;x<=X2+POSSIBLY_PUT_INTERVAL;x++)
 if (
    (InSpace(0,MAP_WIDTH,x,true,false))
    &&
    (InSpace(0,MAP_HEIGHT,y,true,false))
    )
	{
	int Team = MAP[x][y].Team;
    if ((Team!=-1)&&(Team!=NUMBER_OF_TEAMS))
        return false;
	}	 //������ ���� ���� ������ ���� "���������" �����,
                     //�� ������� �������� � ���������� ����
}

//���� "��������� �����" ��� �� ����� ������ ����� �� �����...
for (int y = Y1;y<=Y2;y++) //���� �� ������������ �������������� ������� �� �����.
 for (int x = X1;x<=X2;x++)
 {
 if (
    (InSpace(0,MAP_WIDTH,x,true,false))
    &&
    (InSpace(0,MAP_HEIGHT,y,true,false))
    )
    {
       MAP[x][y].Index = Index;
       MAP[x][y].Team  = Team;
    }
 }

GObject[Team][Index].Init(this,NUMBER_OF_STANDARTS,Type,Team,Index);
GObject[Team][Index].Is = true;
GObject[Team][Index].X = X;
GObject[Team][Index].Y = Y;
GObject[Team][Index].OldX = X;
GObject[Team][Index].OldY = Y;

GObject[Team][Index].Z    = MAP[X][Y].Z;
GObject[Team][Index].OldZ    = MAP[X][Y].Z;
GObject[Team][Index].TempZ    = MAP[X][Y].Z;

GObject[Team][Index].TempX = X;
GObject[Team][Index].OldTempX = X;
GObject[Team][Index].TempY = X;
GObject[Team][Index].OldTempY = X;

GObject[Team][Index].GetNewPos(
                               -1,
                               -1
                               );
Teams[Team].Number++;
Check_Put_UnitInKvadrat(Team,Index);
return true;
}

void TGSociety::RefreshObjectParametres(int Team,int Index)
     {
     TGObject* Unit = &GObject[Team][Index];
     int _Team = Unit->PrimEnemy.Type;
     int _Index = Unit->PrimEnemy.Index;


     Unit->OldIs = Unit->Is;
     GObject[Team][Index].OldX = GObject[Team][Index].X;
     GObject[Team][Index].OldY = GObject[Team][Index].Y;
     GObject[Team][Index].OldZ = GObject[Team][Index].Z;
     GObject[Team][Index].OldAngle = GObject[Team][Index].Angle;

     Unit->Refresh();

     SetEnemy(Team,Index);
     if (Unit->CanMove)
        {
//        Unit->Points.GetNewPoint();
        Unit->SectionWay.GetNearestPoint();
        SetPos(Team,Index);
        MoveUnit(Team,Index);
        }
     Unit->Z = MAP[Unit->X][Unit->Y].Z;
     if (Unit->Thinking)
        {
        Unit->SetGlobalAngle_R();
        Analis_Change(Team,Index,0);
        Unit->SetStatus(); //������ ������ ���� ���� �������� !
        DoAction(Team,Index);
        //Unit->Target.Refresh(0,true);
        }


     }








void TGSociety::Sinchronize(int Team,int Index, bool SnAction)
    {
      int X1 = GObject[Team][Index].X-round(float(GObject[Team][Index].Width)/2),
          X2 = GObject[Team][Index].X+round(float(GObject[Team][Index].Width)/2),
          Y1 = GObject[Team][Index].Y-round(float(GObject[Team][Index].Width)/2),
          Y2 = GObject[Team][Index].Y+round(float(GObject[Team][Index].Width)/2);
    // ��������� ������, �� ������� �����, ��� ���������� ������


//   ���� ����� ������� ������� ��������
//   ���� �������� ����� ������� �����������,
//   � ��� ������ �� ���������.
    if(!GObject[Team][Index].CheckPoint(GObject[Team][Index].X,GObject[Team][Index].Y))
      return;

    if (SnAction)
       {
        for (int y = Y1;y<=Y2;y++) //���� �� ������������ �������������� ������� �� �����.
         for (int x = X1;x<=X2;x++)
              {



               MAP[x][y].Index = Index;

               MAP[x][y].Team = Team;
//               GObject[Team][Index].Z =
//                                       MAP[GObject[Team][Index].X]
//                                          [GObject[Team][Index].Y].Z;
              }


       }
          else
          {
          for (int y = Y1;y<=Y2;y++) //���� �� ������������ �������������� ������� �� �����.
           for (int x = X1;x<=X2;x++)
           {
            MAP[x][y].Index = -1;

            MAP[x][y].Team = -1;
           }
          }
    }


void TGSociety::MoveUnit(int Team,int Index)
{
if (!GObject[Team][Index].CanMove)
    return;
if  (GetAngle(GObject[Team][Index].Angle,GObject[Team][Index].NewPosition.Angle,GObject[Team][Index].AngleSpeed,POSSIBLY_ANGLE_TURN))
{
if  (
    (GObject[Team][Index].NewPosition.GlobalX>=0)&&
    (GObject[Team][Index].NewPosition.X>=0)
    &&
    (GObject[Team][Index].NewPosition.X < MAP_WIDTH)&&
    (GObject[Team][Index].NewPosition.GlobalX < MAP_WIDTH)
    &&
    (GObject[Team][Index].NewPosition.GlobalY>=0)&&
    (GObject[Team][Index].NewPosition.Y>=0)
    &&
    (GObject[Team][Index].NewPosition.Y < MAP_HEIGHT)&&
    (GObject[Team][Index].NewPosition.GlobalY < MAP_HEIGHT)
    )
{
float TDX =GObject[Team][Index].NewPosition.X - GObject[Team][Index].X,
       TDY =GObject[Team][Index].NewPosition.Y - GObject[Team][Index].Y,
       DeltaX = GObject[Team][Index].NewPosition.DeltaX,
       DeltaY = GObject[Team][Index].NewPosition.DeltaY;
Sinchronize(Team,Index,DEL);

//��������� ���������
GObject[Team][Index].OldX = GObject[Team][Index].X;
GObject[Team][Index].OldY = GObject[Team][Index].Y;
if (((DeltaX!=0) || (DeltaY!=0))&& GObject[Team][Index].NewPosition.X!=-1)
for (int i = 0;
      (
       ( kvdrt(i)+fkvdrt(
                        i*min(fabs(DeltaX),fabs(DeltaY))
                        /
                        max(abs(DeltaX),abs(DeltaY)))
                        )
       <
         kvdrt(GObject[Team][Index].Speed)
      )
      &&
      GObject[Team][Index].NewPosition.X!=-1;
    i++
    )

    {
    float Angle = GObject[Team][Index].NewPosition.Angle;

    if(CheckPoint(&GObject[Team][Index],GObject[Team][Index].NewPosition.Angle,false,0,0))
       GetPoint(DeltaX,DeltaY,
                GObject[Team][Index].X,
                GObject[Team][Index].Y,
                GObject[Team][Index].NewPosition.SmallPoint,
                GObject[Team][Index].NewPosition.Steps
                );



    TDX = GObject[Team][Index].NewPosition.X - GObject[Team][Index].X;
    TDY = GObject[Team][Index].NewPosition.Y - GObject[Team][Index].Y;

    if ((TDX == 0) && (TDY ==0)) GObject[Team][Index].NewPosition.X = -1;

    int
    GTDX = GObject[Team][Index].NewPosition.GlobalX - GObject[Team][Index].X,
    GTDY = GObject[Team][Index].NewPosition.GlobalY - GObject[Team][Index].Y;
    if ((GTDX == 0) && (GTDY ==0))
    GObject[Team][Index].FreePosition();


    }
Check_Put_UnitInKvadrat(Team,Index);

if (InXYSpace(160,164,61,64,GObject[Team][Index].X,GObject[Team][Index].Y,true,true,true,false))
   bool Stop = true;


Sinchronize(Team,Index,PUT);
//ShowMessage("���� �� ������� "+IntToStr(Team)+" � �������� "+IntToStr(Index)+" ����� �� ������� �����!");
}
}
}
//               -** ANGLE's MATRIX **-

void TGSociety::ANGLE_MATRIX_Init()
     {
     ANGLE_MATRIX = new TForFound** [NUMBER_OF_TEAMS];    //[NUMBER_OF_TEAMS][NUMBER_OF_UNITS][NUMBER_OF_UNITS*NUMBER_OF_TEAMS];
     for(int T = 0; T<NUMBER_OF_TEAMS; T++)
        {
        ANGLE_MATRIX[T] = new TForFound* [NUMBER_OF_UNITS];
        for (int I = 0;I<NUMBER_OF_UNITS;I++)
             {
             ANGLE_MATRIX[T][I] = new TForFound [NUMBER_OF_UNITS*NUMBER_OF_TEAMS];
             GObject[T][I].NVO = 0;
             for (int i = 0;i<(NUMBER_OF_UNITS*NUMBER_OF_TEAMS);i++)
                 ANGLE_MATRIX[T][I][i].Type =  -1;

             }
        }
     }

void TGSociety::ANGLE_MATRIX_GetAngles()
     {
     //��� ����� ��������� ������� ������� ���������� � ��������������
     //� ����. = ((� �����-1)*� �����)/2  --  �� ������� ������� ������������� ���������� ��������,
     //������� ���������� ����� �������.
     //
     //�� ��� ������ ������� ��������� � ������ � �� ��������� �� �� ����� ������!


     // �������������
     {for(int T = 0; T<NUMBER_OF_TEAMS; T++)
        for (int I = 0;I<NUMBER_OF_UNITS;I++)
             {
             GObject[T][I].NVO = 0;
             for (int i = 0;
                  (ANGLE_MATRIX[T][I][i].Type!=-1)
                  &&
                  (i < (NUMBER_OF_UNITS*NUMBER_OF_TEAMS));
                  i++)
                 ANGLE_MATRIX[T][I][i].Type=-1;
             }
      }
     // ����� �������������


//������� �� ������� �� ��������������(��� ���� ��� � ���� ������� ����� ��� ���������)
//������� ����� ����������� ����� ������������ ���������, �� � ��������� ������ ������������ ����
//��� ����� �����.
     for (int T = 0;T<NUMBER_OF_TEAMS;T++)
         for(int I = 0;I<NUMBER_OF_UNITS;I++)

               for (int t = T;t<NUMBER_OF_TEAMS;t++) // ��� ��� ��� ���������� "���������"
                   {
                   int INDEX;
                   if (t==T) INDEX = I;
                      else INDEX = 0;  //�������� �� ���������� ����� ����� ������� � �� �����...
                   for(int i = INDEX;i<NUMBER_OF_UNITS;i++)
                      {

                      if ((t == T) && (i == I))
                         continue;
                         //���� ���� �� ���� �����

                      if ((!GObject[T][I].Is) || (!GObject[t][i].Is))
                         continue;

                      if ((!GObject[T][I].Thinking) && (!GObject[t][i].Thinking))
                         continue;

                      int MAX_SIGHT,max_SIGHT;
                      if (GObject[T][I].IndexInGroup!=0)MAX_SIGHT = SECOND_MAX_SIGHT;
                         else MAX_SIGHT = GENERAL_MAX_SIGHT;
                      if (GObject[t][i].IndexInGroup!=0)max_SIGHT = SECOND_MAX_SIGHT;
                         else max_SIGHT = GENERAL_MAX_SIGHT;



                      if ((GObject[T][I].NVO>=MAX_SIGHT)&&(GObject[t][i].NVO>=max_SIGHT))
                          continue;

                      int X=GObject[T][I].X, //���������� ������� ������
                          Y=GObject[T][I].Y,

                          x=GObject[t][i].X, //���������� ��������� �������
                          y=GObject[t][i].Y;

                      float DeltaX = x-X,
                            DeltaY = y-Y,
                            R = kvdrt(DeltaX)+kvdrt(DeltaY),
                            Angle=1000;
                            R = sqrt( (double)R );



                      if ((R <= GObject[T][I].Sight)&& (GObject[T][I].NVO < MAX_SIGHT)&&GObject[T][I].Thinking) //���� ������� ������ � ���� ������ �������
                         {

                             if(R!=0)
                               {
                                 if (DeltaX!=0)
                                 Angle = StdAngl((float)atan2( (double)DeltaY, (double)DeltaX)/OGIR);//��������� ����
                                     else if(DeltaY > 0) Angle = 90;
                                          else if(DeltaY < 0)Angle = 270;
                               }
                               else Angle = 0;
                         //������� �������� �� ������������ ������� � ������� ������
                         ANGLE_MATRIX[T][I][GObject[T][I].NVO].R = R;
                         ANGLE_MATRIX[T][I][GObject[T][I].NVO].Angle = Angle;
                         ANGLE_MATRIX[T][I][GObject[T][I].NVO].Index = i;
                         ANGLE_MATRIX[T][I][GObject[T][I].NVO].Type = t;
                         GObject[T][I].NVO ++;
                         }
                         //������ �������� � ������ ������� � ������� �������������
                         //���� ������� ������������� ��������� ������� ��� ��� ������

                      if ((GObject[t][i].Sight >= R)&&(GObject[t][i].NVO<max_SIGHT)&&GObject[t][i].Thinking)
                           {
                           if (Angle == 1000)
                              {
                              if (DeltaX!=0)
                                Angle = StdAngl(atan2( (double)DeltaY, (double)DeltaX)/OGIR);//��������� ����
                                    else if(DeltaY > 0) Angle = 90;
                                         else if(DeltaY < 0)Angle = 270;
                              }
                           Angle = StdAngl(180-Angle);
                           ANGLE_MATRIX[t][i][GObject[t][i].NVO].R = R;
                           ANGLE_MATRIX[t][i][GObject[t][i].NVO].Angle = Angle + 180;
                           ANGLE_MATRIX[t][i][GObject[t][i].NVO].Index = I;
                           ANGLE_MATRIX[t][i][GObject[t][i].NVO].Type = T;
                           GObject[t][i].NVO ++;
                           }





                      }
                   }






     }
void TGSociety::Check_Put_UnitInKvadrat(int Team,int Index)
     {
     //������� ����� ������� ��������� � ����������
     //TGSociety::AddUnitInKvadrat();

     int IOX = GObject[Team][Index].Kvadrat.IndexOX;
     int IOY = GObject[Team][Index].Kvadrat.IndexOY;

     int X = Kvadrats[IOX][IOY].X,
         Y = Kvadrats[IOX][IOY].Y,
         X2 = Kvadrats[IOX][IOY].X_,
         Y2 = Kvadrats[IOX][IOY].Y_;
     if (//���� ������ ���� �� ����� �� ������� ��������, � ������� �� ����, �� �� ���� �� �������
         GObject[Team][Index].Kvadrat.ID==-1
        ||
        (!((GObject[Team][Index].X >= X)&&
        (GObject[Team][Index].Y >= Y)&&
        (GObject[Team][Index].X <= X2)&&
        (GObject[Team][Index].Y <= Y2)))
        )
            {
             if (GObject[Team][Index].Kvadrat.ID == -1) //���� �� ������ ����� �� ��������
                {
                int OnOX = div(GObject[Team][Index].X,KVADRAT_WIDTH).quot;
                int OnOY = div(GObject[Team][Index].Y,KVADRAT_HEIGHT).quot;
                GObject[Team][Index].Kvadrat.IndexOX = OnOX;          //������ �������� �� ��
                GObject[Team][Index].Kvadrat.IndexOY = OnOY;          // �� ��

                AddUnitInKvadrat(OnOX,OnOY,&GObject[Team][Index],PUT); //��������� "� �������"
                return; //��� ������ ������ ������
                }
             //------------

               else
             {
             int OnOX = 0,OnOY = 0; //����������� � ����� ������� ����� ���� (������ �� ��������� , ����������� �� �������)
             int KvX = GObject[Team][Index].Kvadrat.IndexOX;
             int KvY = GObject[Team][Index].Kvadrat.IndexOY;
             if (GObject[Team][Index].X < X) OnOX = -1;
                else if (GObject[Team][Index].X > X2) OnOX = 1;

             if (GObject[Team][Index].Y < Y) OnOY = -1;
                else if (GObject[Team][Index].Y > Y2) OnOY = 1;

             AddUnitInKvadrat(KvX,KvY,&GObject[Team][Index], DEL); //������� �� ����������� ��������,
             AddUnitInKvadrat((KvX+OnOX),(KvY+OnOY),&GObject[Team][Index],PUT); // ��������� � �������������� �����
             }

            }
            else return;


     }


     void TGSociety::AddUnitInKvadrat(int OnOX,int OnOY, TGObject *Unit,bool ADD)
          {
                     // OnOX ����� OnOX, ���� �� �� "������� �� ������", ����� ������� ������ � �������� � �����
                     OnOX=(OnOX>=0)?OnOX:Unit->Kvadrat.IndexOX,
                     OnOY=(OnOY>=0)?OnOY:Unit->Kvadrat.IndexOY;
          int *Number = &Kvadrats[OnOX][OnOY].Number;//��������� ������ � ��������


          if (ADD) //���� ������� ������ ��������
             {
             if ((*Number) < MEN_IN_KVADRAT)
                {
                (*Number)++;
                Kvadrats[OnOX][OnOY].Units[(*Number)-1] = Unit;

                Unit->Kvadrat.ID = (*Number)-1;
                Unit->Kvadrat.IndexOX = OnOX;
                Unit->Kvadrat.IndexOY = OnOY;
                }
//                else ShowMessage("������ � �-�� <TGSociety::AddUnitInKvadrat>, ��� ��������� �������, ��������� � ������������ ");

             }
             else //���� �������
                 {
                 int ID = Unit->Kvadrat.ID; //���������� ����� � ��������

                 /*
                 �������� �����:
                 �.�. ������� ����� ������ ��� � ���� ��������, ��,
                 ��� �� �� ���� "��������" � ������ � �������� ������ ���� ����������,
                 � �� ����� ��� ��� ��������� �����������, � ������ "���������".
                 */
                 Kvadrats[OnOX][OnOY].Units[ID] = Kvadrats[OnOX][OnOY].Units[(*Number)-1];
                 Kvadrats[OnOX][OnOY].Units[(*Number)-1] = NULL;
                 (*Number) --;
                 }
          }





//                   -**Moving**-


TForFound TGSociety::CheckMove(float Angle, int Team,int Index)
    {
    TForFound Result;

    Result.Type = -1;
    Result.R = -1;

    for (int i = 0;(ANGLE_MATRIX[Team][Index][i].Type !=-1)&&(i<NUMBER_OF_UNITS);i++)
    {
    float MatrixAngle/*Graduses*/ = ANGLE_MATRIX[Team][Index][i].Angle,
          CheckingAngle/*Graduses*/ = StdAngl(Angle);

    if ( round(MatrixAngle) == round(CheckingAngle) )

       if (ANGLE_MATRIX[Team][Index][i].R <= GObject[Team][Index].Sight)
       {
       Result = ANGLE_MATRIX[Team][Index][i];
       break;
       }


    }



    return Result;

    }
float TGSociety::SmalestR(int Team,int Index)
{
int i = 0;
float R = ANGLE_MATRIX[Team][Index][i].R;
while(ANGLE_MATRIX[Team][Index][i].Type != -1)
     {
     if (i>=(NUMBER_OF_UNITS*NUMBER_OF_TEAMS)) break;
     if (R > ANGLE_MATRIX[Team][Index][i].R)
         R = ANGLE_MATRIX[Team][Index][i].R;
     i++;
     }

if (R == 0) R = GObject[Team][Index].Sight;
return R;

}


bool TGSociety::Check(float Angle,int Team,int Index,int &Left_Zero_Right,float R)
     {
     float AngleOW =StdAngl(atan2( (double)(GObject[Team][Index].Width+1), (double)SmalestR(Team,Index))/OGIR);
     int i;
     bool result = true;

     //������� ������� ������ �� ������ � ���
     if (Left_Zero_Right == 0)
     {
     i=0;
     float FastAngle = Angle;
     while (
            (ANGLE_MATRIX[Team][Index][i].Type != -1 )
            //|| ((ANGLE_MATRIX[Team][Index][i].R - R) >=GObject[Team][Index].Width)
           )
           {
           if (i>=(NUMBER_OF_UNITS*NUMBER_OF_TEAMS)) break;
           Angle = FastAngle;
           float ScAngle = ANGLE_MATRIX[Team][Index][i].Angle;

           if ((Angle <= -90) ||(Angle >= 90 ) )
           SetAngle(Angle, ScAngle);
                 if (

                     (ScAngle >= (Angle - (AngleOW/2)  ))
                     &&
                     (ScAngle <= (Angle + (AngleOW/2)  ))
                     &&
                     (ANGLE_MATRIX[Team][Index][i].R < GObject[Team][Index].NewPosition.GlobalR)


                    )
                    {
                    if (ANGLE_MATRIX[Team][Index][i].Angle > (Angle + (AngleOW/2)) )  //�������� � ����� ������� ������ ������
                       Left_Zero_Right = -1; // ������ ������ ������ - ��������... ����� :-) ������ ����� .
                       else
                       Left_Zero_Right = 1;  //����������



                    result = false;
//                    break;
                    }
           i++;
           }
       }

    //������ ��������� ������ �����;
    if (Left_Zero_Right == -1)
    {
    i=0;
    result = true;
    while (ANGLE_MATRIX[Team][Index][i].Type != -1)
          {
          if (i>=(NUMBER_OF_UNITS*NUMBER_OF_TEAMS)) break;
          float ScAngle = ANGLE_MATRIX[Team][Index][i].Angle;

          if ((Angle <= -90) ||(Angle >= 90 ) )
          SetAngle(Angle, ScAngle);

          if (

             (ScAngle > (Angle - AngleOW))
             &&
             (ScAngle < Angle)
             &&
             (ANGLE_MATRIX[Team][Index][i].R < GObject[Team][Index].NewPosition.GlobalR)

              )
             result = false;
//             break;
          i++;
          }
     }

         //� ������ ��������� ������ ������;
    if (Left_Zero_Right == 1)
    {
    i=0;
    result = true;
    while (ANGLE_MATRIX[Team][Index][i].Type != -1)
          {
           if (i>=(NUMBER_OF_UNITS*NUMBER_OF_TEAMS)) break;
           float ScAngle = ANGLE_MATRIX[Team][Index][i].Angle;

           if ((Angle <= -90) ||(Angle >= 90 ) )
           SetAngle(Angle, ScAngle);

          if (

             (ScAngle > Angle)
             &&
             (ScAngle < (Angle+AngleOW) )
             &&
             (ANGLE_MATRIX[Team][Index][i].R < GObject[Team][Index].NewPosition.GlobalR)


             )
             result = false;
//             break;
          i++;
          }
     }




     return result;
     }



bool TGSociety::CheckPoint (TGObject *Unit, float Angle,bool WithAngle,int R,int I)
     {// ������� "��������" ��������� ������� �� ��� ������ �
      // ���� �� ����������� �� ������ ������� ��� ���������� 0.
     bool Result = true;
         if (
            (!InSpace(0,MAP_WIDTH,Unit->X,true,false))
            ||
            (!InSpace(0,MAP_HEIGHT,Unit->Y,true,false))
            )
            return false;
       float DeltaX     = (WithAngle)?cos(Angle*OGIR):Unit->NewPosition.DeltaX,
             DeltaY     = (WithAngle)?sin(Angle*OGIR):Unit->NewPosition.DeltaY;
       int   SmallPoint = (WithAngle)?/**/((fabs(DeltaX)>fabs(DeltaY))?Unit->Y:Unit->X)/**/:Unit->NewPosition.SmallPoint,
             Steps      = (WithAngle)?0:Unit->NewPosition.Steps;

       int   X          = Unit->X,
             Y          = Unit->Y;

       float WentR      = (fabs(DeltaX)>fabs(DeltaY))?sqrt( (double)kvdrt(0 ) + kvdrt(Unit->Y - SmallPoint)):sqrt( (double)kvdrt(0 ) + kvdrt(Unit->X - SmallPoint));
       for  (int i = 0;(WentR <= float(R))&&(i<=I); i++)
        {

         GetPoint(DeltaX,DeltaY,X,Y,SmallPoint,Steps);
         if (!Unit->CheckPoint(X,Y))
            return false;

       WentR      = (fabs(DeltaX)>fabs(DeltaY))?sqrt( (double)fkvdrt(i ) + fkvdrt(Unit->Y - SmallPoint)):sqrt( (double)(fkvdrt(i ) + fkvdrt(Unit->X - SmallPoint)));
       }

      return Result;
     }



void TGSociety::SetPos(int Team,int Index/*, TCanvas *Canvas*/)   // �������� �������� �����������
     {
      float DeltaX = GObject[Team][Index].NewPosition.DeltaX,
            DeltaY = GObject[Team][Index].NewPosition.DeltaY;

     if (GObject[Team][Index].NewPosition.GlobalX !=-1)//��� �������� �������, ��� ��� ������ �� ����.
     {/* if
       (
       ((DeltaX!=0)||(DeltaY!=0))&&
       (!CheckPoint(&GObject[Team][Index],123,false,GObject[Team][Index].Speed,20))
       )*/
      {
      float Angles[PART_IN_CYRCLE];
      int   Speeds[PART_IN_CYRCLE];
      float Angle   = GObject[Team][Index].NewPosition.GlobalAngle;
      // ���� DeltaX != 0 && DeltaY !=0 ��������� ���� �� ������������ ������� - ����� ����������
      // ����� ����� ���� �� ���� "���������"
      float _Angle  = GObject[Team][Index].NewPosition.Angle;
      float R =-1;
      int X = GObject[Team][Index].X,
          Y = GObject[Team][Index].Y;
      int N = 0;
      int Speed;
      for (float A = _Angle; A<360+_Angle;A+=360/PART_IN_CYRCLE)
          {
          bool Res = false;
          for (Speed = 0;Speed <= GObject[Team][Index].Speed;Speed++)
             {
             if (CheckPoint(&GObject[Team][Index],A,true/*WITH_ANGLE*/,Speed,Speed*2))
                {
                Angles[N] = StdAngl(A);
                Speeds[N] = Speed;
                Res = true;
      	     	          if (N == PART_IN_CYRCLE-1)
      			  break;
                }
             }
          if (Res)
             N++;

          }
          // ���������� Speed ����������� ��� � ���
          float ToAngle = SetNearestAngle(Angles,N,Angle,_Angle,Speeds,Speed);
                GObject[Team][Index].GetNewPos(ToAngle,float(Speed+1));

          while (
                 (!CheckPoint(&GObject[Team][Index],GObject[Team][Index].NewPosition.Angle,false,0,0))
                 &&
                 (N > 0)
                )
                {
                float ToAngle = SetNearestAngle(Angles,N,Angle,_Angle,Speeds,Speed);
                GObject[Team][Index].GetNewPos(ToAngle,float(Speed+1));
                }



      }


     }
     }





//---------------------------------------------------------------------------


/*     if((GObject[Team][Index].NewPosition.GlobalX > 0) && GObject[Team][Index].Is)
     {

     TForFound Check_Move[360];
     float Angle = GObject[Team][Index].NewPosition.GlobalAngle;
     float SmlstR = SmalestR(Team,Index); //������� ������� ��������� �� ���������� �������
     float AngleOW = atan2( (double)OBJECT_WIDTH, (double)SmlstR);
     float Sight = GObject[Team][Index].Sight;

     int i = 0,j;
     bool Plus = true;;

     //������������� ������
     for(int z = 0;z < 360;z++ )
         Check_Move[z].Type = -1;

     if (Plus) j = i; else j = 360 - i;
        Check_Move[i] = CheckMove(Angle,Team,Index,Canvas);


     bool C_L = false,C_R = false;
     while (!C_L || !C_R)              //��� �� ������ ���� ���� ������� ��� ��������
           {
           if(abs(i)>0) break;                // � ���� ���� �� ������, ���� �� ����� � ������� .
           float Left,Right;

           if (i == 0)i++; else i+=i/abs(i);
           if (Plus) j = i; else j = 360 - i;
           Check_Move[j] = CheckMove(Angle+i,Team,Index,Canvas);

           if (Check_Move[j].Type!=-1)
                                       {if (i>0) C_L = true;
                                        else C_R = true;}
                                      else
                                          {
                                          if ((i>0) && (!C_L))
                                             Left ++;
                                             else if((i<0) && (!C_R))
                                                  Right ++;
                                          }

           if ((Left+Right)>=AngleOW  )
            {if (GObject[Team][Index].NewPosition.GlobalR > Sight)
               GObject[Team][Index].GetNewPos( Angle+((Left+Right)/2),Sight);
               else GObject[Team][Index].GetNewPos(
                                                  GObject[Team][Index].NewPosition.GlobalAngle,
                                                  GObject[Team][Index].NewPosition.GlobalR
                                                   );
            }
           i = -i; if (Plus) Plus = false;
                             else Plus = true;

           }

      int k = 0;
      while ((i > 0) && (k < AngleOW) ) //���� ���� ����������� ����� ������ �� ���� ���� ����������
                                        //��� ������ � ������ ���� �����
            {
            if (i<0) Plus = false;
                            else Plus = true;

            if (Plus) j = i+k; else j = 360 - i-k;
            Check_Move[j] = CheckMove(Angle+i+k,Team,Index,Canvas);

            if(Check_Move[j].Type==-1) k+= i/abs(i);

            if (k >= AngleOW)
            GObject[Team][Index].GetNewPos(Angle+i+(k/2),Sight);
            }
     }
     if (
        GObject[Team][Index].NewPosition.GlobalX == GObject[Team][Index].X &&
        GObject[Team][Index].NewPosition.GlobalY == GObject[Team][Index].Y
        )
        GObject[Team][Index].NewPosition.GlobalX = -1;
*/




















//-------------------------------------------------------------
//    ---***  ������������� ��������� � ������� ���  ***---
//-------------------------------------------------------------

int TGSociety::Analis_Change(int Team,int Index,int index)
    {
    /*
    ���� ���������� �����, �� ��� ������� �������� ���:
    ��� ���������� ���� ������, ������� �����, ��� ��� ����: ��������� ��� ���������,
    � ��� ��� ��� ��� ��������, ��� ����� �� ��������: "���, ������, �������� ��� - ��� ������".
    ��� ��� ��� ��������� ��� ������� ��� � ���� � �� ����� ��� ���������.
    */

    //���� ���� "������" � ������ �� �� �� ����� ����� ������!
    if (!GObject[Team][Index].InGroup)
    {

    index = -1;
    int Live =-1;



    int Enemy=0;//������ ������ � ������. (���� ��� ������ ����������)
    int Ally=1; //������ ������ ���� ���� � ������ ����������
    for (int i = 0;i<=GObject[Team][Index].NVO;i++ )
        {
        if (i>=(NUMBER_OF_UNITS*NUMBER_OF_TEAMS)) break;

        // ���� ������ ����
        if ((ANGLE_MATRIX[Team][Index][i].Type != Team)
             &&
            (ANGLE_MATRIX[Team][Index][i].Type!=-1)
            )
            {
            // ������� ������
            Enemy++;

            //      ���� ���� �� �������� �������:
                    if (GObject[ANGLE_MATRIX[Team][Index][i].Type]
                               [ANGLE_MATRIX[Team][Index][i].Index].Live <= Live)

                        {
            //          �������� ��� � ����.
                        Live = GObject[ANGLE_MATRIX[Team][Index][i].Type]
                                      [ANGLE_MATRIX[Team][Index][i].Index].Live;
                        index = i;


                        }
            //      ���� � ���� ��� �� �������� �� ������ �����:
                    if (Live == -1)
                       {
            //      �������� �����
                       Live = GObject[ANGLE_MATRIX[Team][Index][i].Type]
                                     [ANGLE_MATRIX[Team][Index][i].Index].Live;
                       index = i;
                       }



            }
            // ���� ��������� ������� - ������ ������� ���������.
            else Ally ++;
         }



//    if ((index >= 0)&&(index < (NUMBER_OF_TEAMS*NUMBER_OF_UNITS)&&GObject[Team][Index].PrimEnemy.Type!=-1))
    if (GObject[Team][Index].PrimaryEnemy!=NULL)
       int Stop = 1;



    if (((Ally*GObject[Team][Index].N) >= Enemy)&&(index!= -1))
       {
//       if (
//          (index >= 0)&&(index < (NUMBER_OF_TEAMS*NUMBER_OF_UNITS))
//          &&
//          (GObject[Team][Index].Character == AGRESSIVE)
//          )
//
//          {
//    �������� ����� � ����.
       GObject[Team][Index].GetEnemy(
                                    ANGLE_MATRIX[Team][Index][index]
                                    );

       GObject[Team][Index].ACTION = ATTACK_ACT;
       return ATTACK_ACT;
//          }
       }
       else
           {
            if (index!=-1)
            {
            GObject[Team][Index].ACTION = GOBACK_ACT;
            return GOBACK_ACT;
            }
            else {
                 GObject[Team][Index].ACTION = WAIT_ACT;
                 return WAIT_ACT;
                 }
           }
    }
    }

void TGSociety::DoAction(int Team,int Index)
     {

      if (GObject[Team][Index].Status.Attack)
         {
         if ((GObject[Team][Index].CanAttack)&&(!GObject[Team][Index].Status.MustMove))
                             {
                             if ((GObject[Team][Index].PrimaryEnemy!=0)||(GObject[Team][Index].PrimEnemy.Type!=-1))
                             Attack(Team,Index);
                             }
         }

//         case GOBACK_ACT:
//                        GoBackOn(Team,Index,0,false /*(withoutR)*/);
//                        break;*/
//      else
//                        {
//                        if (!GObject[Team][Index].OnMission) GObject[Team][Index].Stop();
//                        };


     }


//        --------------� � � � � �------------
//        ---   * * *   A T T A C K * * *   ---
void TGSociety::Attack(int Team,int Index)
     {
     if (!GObject[Team][Index].CanAttack)  return; //���� ���� � ����� ��������� , �� �� ����� ��� ������ .
     int type = GObject[Team][Index].PrimEnemy.Type;
     int index = GObject[Team][Index].PrimEnemy.Index;
     float R = GObject[Team][Index].PrimEnemy.R;
     float Angle = GObject[Team][Index].PrimEnemy.Angle;
     if (type>=0)
     {
     if (!TryFire(Team,Index,type,index,GObject[Team][Index].TypeWeapon))
         GObject[Team][Index].GetGlobalPos(GObject[Team][Index].PrimEnemy.R - 10,GObject[Team][Index].PrimEnemy.Angle,  AI);
         //else GObject[Team][Index].Stop();
     }
     }



int TGSociety::TryFire(int Team,int Index,int type,int index,int TypeWeapon)
     {
     GObject[Team][Index].
           TimeAfterAttack = (GObject[Team][Index].TimeAfterAttack<GObject[Team][Index].AttackPeriod)
                             ?GObject[Team][Index].TimeAfterAttack+1:0;
     if ((TypeWeapon == WP_GUN) || (TypeWeapon == WP_HOWITZER))
        {
        if((!GObject[Team][Index].Target.On)&&(GObject[Team][Index].TimeAfterAttack==0))
          {
              int X = GObject[type][index].X,
                  Y = GObject[type][index].Y;
              int Damage  = GObject[Team][Index].Damage;
              int MaxTime = GObject[Team][Index].AttackPeriod-1;
              int Radius = 10;
              if (!GObject[Team][Index].Target.Fire(X,Y))
                 return 0;
                 else 
				 {
					 GObject[Team][Index].Status.Fire = true;
					 return 2;
				 }
				 
          }
          else
          return 1;
        }
     }

void TGSociety::Fire(int Team,int Index,int type,int index)
     {

      if(GObject[Team][Index].TimeAfterAttack == (GObject[Team][Index].AttackPeriod)) // ���� ������� ����� ������� ����
          //�������� ����� � �����
          {if  (//���� ���� ��� �� ����� � ��� ����� ���������� ��� ������� �����, ��
              GObject[type][index].Is &&
              (GObject[Team][Index].Damage > GObject[type][index].Armor)
              )
              {
              //GObject[type][index].Live -= (GObject[Team][Index].Damage - GObject[type][index].Armor);

              int X = GObject[type][index].X,
                  Y = GObject[type][index].Y;
              int Damage  = GObject[Team][Index].Damage;
              int MaxTime = GObject[Team][Index].AttackPeriod-1;
              int Radius = 10;
              GObject[Team][Index].Target.Fire(X,Y); //GetTarget(X,Y,Damage,Radius,MaxTime);

			  //            GObject[type][index].GETPOWER();
              //������ ����� ��������� � �����
              GObject[Team][Index].Status.Fire = true; //��������� ���������
	      GObject[Team][Index].Status.FireMovie = (rand() > 30000)?1:0;
              GObject[Team][Index].TimeAfterAttack = 0;

              if (GObject[type][index].Live <= 0)
	         {
		 Dye(&GObject[type][index]);
                 //� ������� �������� �������� ���������
		 //���� ���� � ������, � ������ � ������� �� ������� �� �������, �� ...
		 if ((GObject[Team][Index].InGroup)&&(Groups[GObject[Team][Index].GroupTeam][GObject[Team][Index].GroupIndex].OnMission))
		    {
                    //� ���� ������ ��������� ����� ������ �� ������ �� 1
		    Groups[GObject[Team][Index].GroupTeam][GObject[Team][Index].GroupIndex].NumberOfKilledEnemies++;
		    //� ���� ������ �� ����� ������ ��������� ����� �� ��� ������������� �� �������
		    if(
		      Groups[GObject[Team][Index].GroupTeam][GObject[Team][Index].GroupIndex].NumberOfKilledEnemies
		      ==
		      Groups[GObject[Team][Index].GroupTeam][GObject[Team][Index].GroupIndex].NumberToKill
		      )
                      Groups[GObject[Team][Index].GroupTeam][GObject[Team][Index].GroupIndex].Free();
		    }
	         }
              }
              //else ShowMessage("�� ���� ��������� �����: ���������� ������. ��� �� ��������� ��� �����.");
          }
	    else  {
                  GObject[Team][Index].TimeAfterAttack ++;
				  //GObject[Team][Index].Status.Fire = false;
	          }



    }

void TGSociety::Dye(TGObject *Unit)
     {
     Dead++;
     Unit->Is = false;//���� ������ �� ����������

     if ((Unit->TeamIndex == 1)&&(Unit->Index == 6))
        bool Stop = true;
     //����� ������������ ������ TGGroup::GetUnit (������� �������� � ������ � ������ �����)
     const int Team        = Unit->TeamIndex;//���������� ��� ������ ��� ��������������� �������������
     const int Index       = Unit->Index;
     int NumberOfUnits = Teams[Team].Number;
     int X             = Unit->X,
         Y             = Unit->Y;


     Unit->PrimEnemy.Type = -1;
     Unit->PrimaryEnemy   = NULL;




        int X1 = X-round(float(GObject[Team][Index].Width)/2),
            X2 = X+round(float(GObject[Team][Index].Width)/2),
            Y1 = Y-round(float(GObject[Team][Index].Width)/2),
            Y2 = Y+round(float(GObject[Team][Index].Width)/2);
        for (int y = Y1;y<=Y2;y++) //���� �� ������������ �������������� ������� �� �����.
         for (int x = X1;x<=X2;x++)
         if (
            (InSpace(0,MAP_WIDTH,x,true,false))
            &&
            (InSpace(0,MAP_HEIGHT,y,true,false))
            )
            {
            // �������� ��������, �� �������������� ������ ������
            // ����� ����� ( ������ ��������) - ������� ������� �����������
            // ��� ����������.

            MAP[x][y].Team = -1;
            MAP[x][y].Index = -1;
            }


     int OX = Unit->Kvadrat.IndexOX,
         OY = Unit->Kvadrat.IndexOY;
     AddUnitInKvadrat(-1,-1,Unit,DEL);


     if (Unit->InGroup)   //������� ������ � ����� � ������
        {


        int NumberGroup = Groups[Unit->GroupTeam][Unit->GroupIndex].Number;
        for (int i = (Unit->IndexInGroup);i<(NumberGroup-1); i++)
            {
            Groups[Unit->GroupTeam][Unit->GroupIndex].Units[i] = Groups[Unit->GroupTeam][Unit->GroupIndex].Units[i+1];
            Groups[Unit->GroupTeam][Unit->GroupIndex].Units[i].PUnit->IndexInGroup--;
            }
        Groups[Unit->GroupTeam][Unit->GroupIndex].Number--;
        Groups[Unit->GroupTeam][Unit->GroupIndex].Units[NumberGroup-1].Is = false;
        Groups[Unit->GroupTeam][Unit->GroupIndex].Units[NumberGroup-1].PUnit = NULL;
        if (Groups[Unit->GroupTeam][Unit->GroupIndex].Number==0)
        DelGroup(Unit->GroupTeam,Unit->GroupIndex,"GObject::Dye");
        }
     //������� ��� �� ���� ��� �����.
     for (int t = 0;t<NUMBER_OF_TEAMS;t++)
         for (int g = 0;g<NUMBER_OF_GROUPS;g++)
             {
             if (Unit->IncludedInBase[t][g].Yes)
                {
                int i      = Unit->IncludedInBase[t][g].Index;
                int _end_i = Groups[t][g].Mission.EnemyNumber-1;

                Groups[t][g].Mission.Enemy[i] = Groups[t][g].Mission.Enemy[_end_i];

                if (Groups[t][g].Mission.Enemy[i].Object_Group==1)
                {
                if (Groups[t][g].Mission.Enemy[i].Object.GObject!=NULL)
                Groups[t][g].Mission.Enemy[i].Object.GObject->IncludedInBase[t][g].Index = i;
                }
                else
                if (Groups[t][g].Mission.Enemy[i].Object_Group==0)
                {
                if (Groups[t][g].Mission.Enemy[i].Object.GGroup!=NULL)
                Groups[t][g].Mission.Enemy[i].Object.GGroup->IncludedInBase[t][g].Index = i;
                }
                //Groups[t][g].Mission.Enemy[Groups[t][g].Mission.EnemyNumber] = NULL;
                Unit->IncludedInBase[t][g].Yes = false;

                if (Groups[t][g].Mission.EnemyNumber>0)
                Groups[t][g].Mission.EnemyNumber--;
//                if ((t==2)&&(g == 6))
//                   bool stop = 1;
//
//                if (Groups[t][g].Mission.EnemyNumber==0)
//                   bool stop = true;

                }
             }



     Teams[Team].Number--;
     Unit->X = -10;
     Unit->Y = -10; //������ ���� �������� �� �������� ���� ���� � ������������ �������  ..
     }
void TGSociety::SetEnemy(int T,int I)
     {
     GObject[T][I].SetEnemy();
     int ETeam = GObject[T][I].PrimEnemy.Type,
         EIndex = GObject[T][I].PrimEnemy.Index;
     if (ETeam==-1)
        return;
     if (!GObject[ETeam][EIndex].Is)
          GObject[T][I].PrimEnemy.Type =-1;
     }
void TGSociety::GoBackOn(int Team,int Index,int R,bool WithR)
     {
     int fR;
     if (!WithR)
        fR = GObject[Team][Index].PrimEnemy.R;
             else fR = R;
     GObject[Team][Index].GetGlobalPos(float(fR),float(180.0-GObject[Team][Index].PrimEnemy.Angle),AI);

     }

void TGSociety::GetGroupPower(int Team,int Index)
     {
     Groups[Team][Index].GetPower();
     if (Groups[Team][Index].POWER > 0)
     for (int i = 0;i < Groups[Team][Index].Number;i++)
         Groups[Team][Index].Units[i].PUnit->GroupPOWER = Groups[Team][Index].POWER;
     }


//------------------------------------------------------------------------------
//                        �. �. ������

TGGroup* TGSociety::PutGroup(int Team,TGObject* Unit)
     {
     for (int i = 0; i<NUMBER_OF_GROUPS;i++)
         {
         if (!Groups[Team][i].Is)
            {
//          GroupsNumber[Team]++;
            Groups[Team][i].Init(this);
            Groups[Team][i].Team = Team;
            Groups[Team][i].Is = true;
            Groups[Team][i].GetUnit(Unit);
            Groups[Team][i].Mission.Init(&Groups[Team][i]);
            return &Groups[Team][i];
            }
         }
//������������ �����
/*     if (GroupsNumber[Team]<NUMBER_OF_GROUPS)
        {
        GroupsNumber[Team]++;
        Groups[Team][GroupsNumber[Team]-1].Init();
        Groups[Team][GroupsNumber[Team]-1].Team = Team;
        Groups[Team][GroupsNumber[Team]-1].Is = true;
        Groups[Team][GroupsNumber[Team]-1].GetUnit(Unit);
        Groups[Team][GroupsNumber[Team]-1].Mission.Init(&Groups[Team][GroupsNumber[Team]-1]);
        return &Groups[Team][GroupsNumber[Team]-1];
        }*/
//        else
        return NULL;
//        ShowMessage("�� ���� ������� ������! �.�. � ���� ������� ��� ������������ ����� �����!");
     }

void TGSociety::DelGroup(int Team, int Index,char* caller)
     {
     for (int t = 0;t<NUMBER_OF_TEAMS;t++)
         for (int g = 0;g<NUMBER_OF_GROUPS;g++)
             {
             if (Groups[Team][Index].IncludedInBase[t][g].Yes)
                {
                int i      = Groups[Team][Index].IncludedInBase[t][g].Index;
                int _end_i = Groups[t][g].Mission.EnemyNumber-1;
                Groups[t][g].Mission.Enemy[i] = Groups[t][g].Mission.Enemy[_end_i];

                if (Groups[t][g].Mission.Enemy[i].Object_Group==1)
                {
                if (Groups[t][g].Mission.Enemy[i].Object.GObject!=NULL)
                Groups[t][g].Mission.Enemy[i].Object.GObject->IncludedInBase[t][g].Index = i;
                }
                else
                if (Groups[t][g].Mission.Enemy[i].Object_Group==0)
                {
                if (Groups[t][g].Mission.Enemy[i].Object.GGroup!=NULL)
                Groups[t][g].Mission.Enemy[i].Object.GGroup->IncludedInBase[t][g].Index = i;
                }
                Groups[Team][Index].IncludedInBase[t][g].Yes = false;
                //Groups[t][g].Mission.Enemy[Groups[t][g].Mission.EnemyNumber] = NULL;
                if (Groups[t][g].Mission.EnemyNumber>0)
                Groups[t][g].Mission.EnemyNumber--;
//                if ((t == 2) && (g == 6))
//                   bool stop = 1;
//
//                if (Groups[t][g].Mission.EnemyNumber<0)
//                   bool stop = true;


                }
             }


     for (int i = 0;i<Groups[Team][Index].Number;i++)//�������
         {
		 if (Index == 201) 
		    bool Stop = true;
         Groups[Team][Index].Units[i].Is = false;
         Groups[Team][Index].Units[i].PUnit->GroupIndex = -1;
         Groups[Team][Index].Units[i].PUnit->GroupTeam = -1;
         Groups[Team][Index].Units[i].PUnit->GroupPOWER = 0;
         Groups[Team][Index].Units[i].PUnit->InGroup = false;
         Groups[Team][Index].Units[i].PUnit = NULL;
         }
     Groups[Team][Index].POWER=0; //��� ���������
     Groups[Team][Index].Is = false;



     //������������ ����� �������
     /*if (Index!=(GroupsNumber[Team]-1))
     {
      Groups[Team][Index] = Groups[Team][GroupsNumber[Team]-1];   //������ ������ ������ ������ ���������
       for (int i = 0;i<Groups[Team][Index].Number;i++) //������ ������ � ��������� ������
       {
       Groups[Team][Index].Units[i].PUnit->GroupIndex = Index;
       Groups[Team][Index].Units[i].PUnit->GroupPOWER = Groups[Team][GroupsNumber[Team]-1].POWER;
       Groups[Team][Index].Mission.OurGroup = &Groups[Team][Index];
       }
       //� � "��������" ����� ������ ���� ��� ��������
      Groups[Team][Index].Index=Index;
     }*/

     //������������ ����� �������
     /*
       for (int i = 0;i<Groups[Team][GroupsNumber[Team]-1].Number;i++)//������� ��������� ������(������)
         {
         Groups[Team][GroupsNumber[Team]-1].Units[i].Is = false;

         Groups[Team][GroupsNumber[Team]-1].Units[i].PUnit->GroupIndex = -1;
         Groups[Team][GroupsNumber[Team]-1].Units[i].PUnit->GroupTeam = -1;
         Groups[Team][GroupsNumber[Team]-1].Units[i].PUnit->GroupPOWER = 0;
         Groups[Team][GroupsNumber[Team]-1].Units[i].PUnit->InGroup = false;

         Groups[Team][GroupsNumber[Team]-1].Units[i].PUnit = NULL;
         }
     */
//     GroupsNumber[Team]--;
     }
void TGSociety::Group_Analis(int Team,int Index)
     {
     //��� ���� �-� ���� �����������
     //���� OnMission
     //����������� ��������� � �������� ����� ������ �����
     //��� ���� �-� TGGroup::GetEnemy(TGObject Enemy)
     TGGroup *Group = &Groups[Team][Index];
     if (Groups[Team][Index].Number>0 &&
        //!Groups[Team][Index].OnMission &&
        ANGLE_MATRIX[Team][Group->Units[0].PUnit->Index][0].Type !=-1)
        {

        struct{long int value;TGObject *EUnit;} MaxPower,MinPower;
        MaxPower.value = 0;
        MaxPower.EUnit = NULL;
        MinPower.value = -1;
        MinPower.EUnit = NULL;
        /*GObject[ANGLE_MATRIX[Team][Unit->Index][0].Type]
                                                    [ANGLE_MATRIX[Team][Unit->Index][0].Index]
                                                    .POWER;

		MinPower.EUnit = &GObject[ANGLE_MATRIX[Team][Unit->Index][0].Type]
                                                    [ANGLE_MATRIX[Team][Unit->Index][0].Index];*/

  
        TGObject *Unit = Group->Units[0].PUnit;
        for (int i=0;ANGLE_MATRIX[Team][Unit->Index][i].Type >= 0;i++) //������� ������ �������� � ������ �������
            {
            if (i>=(NUMBER_OF_UNITS*NUMBER_OF_TEAMS)) break;
            TGObject *EUnit = &GObject[ANGLE_MATRIX[Team][Unit->Index][i].Type][ANGLE_MATRIX[Team][Unit->Index][i].Index];
            if ((EUnit->TeamIndex == Team)||(!EUnit->Is)) continue;

            //����������� ���� �����, � ���� �� ������� � ������, �� ���� ������, � ������� �� �������.
               if (!EUnit->InGroup) //���� ���� �� � ������
               {
               if (EUnit->POWER > MaxPower.value)
                  {MaxPower.value = EUnit->POWER;
                   MaxPower.EUnit = EUnit;}

               if ((EUnit->POWER < MinPower.value) || (MinPower.value==-1))
                  {MinPower.value = EUnit->POWER;
                   MinPower.EUnit = EUnit;}

               }
                  else             //���� ���� � ������
                  {
                  int GroupPOWER =  Groups[EUnit->GroupTeam][EUnit->GroupIndex].POWER;

                  if (GroupPOWER >= MaxPower.value)
                     {MaxPower.value = GroupPOWER;
                      MaxPower.EUnit = EUnit;}


                         if ((GroupPOWER < MinPower.value) || (MinPower.value==-1))
                            {MinPower.value = GroupPOWER;
                             MinPower.EUnit = EUnit;}



                  }


            }
         
            //���������� ������ �� ���������� �����
            if ((Group->POWER > MaxPower.value) && MaxPower.EUnit!=NULL)
               {
               int t = MaxPower.EUnit->GroupTeam,
                   i = MaxPower.EUnit->GroupIndex;
               if(MaxPower.EUnit->InGroup)
               {
               TGGroup::TDATA DATA;
               DATA.Is      = true;
			   DATA.GObject = NULL;
               DATA.GGroup  = &Groups[t][i];
               Group->Mission.GetNewData(false, DATA);/*Group->GetEnemy(&Groups[t][i]*/
               }
               else {
                    TGGroup::TDATA DATA;
                    DATA.Is      = true;
					DATA.GGroup  = NULL;
                    DATA.GObject = MaxPower.EUnit;
                    Group->Mission.GetNewData(true,DATA);//Group->GetEnemy(MaxPower.EUnit)
                    }
               }
               else
                   {
                   if (MinPower.EUnit!=NULL)
                   {
                   int t = MinPower.EUnit->GroupTeam,
                   i = MinPower.EUnit->GroupIndex;
                   if(MinPower.EUnit->InGroup)
                      {
                     // if (Groups[t][i].Team == Team)
                     //    bool Stop = true;
                        {
                        TGGroup::TDATA DATA;
						DATA.GObject = NULL;
                        DATA.GGroup  = &Groups[t][i];
                        DATA.Is      = true;
                        Group->Mission.GetNewData(false, DATA);//Group->GetEnemy(&Groups[t][i]);
                        }
                       }
                       else {
                            TGGroup::TDATA DATA;
							DATA.GGroup  = NULL;
                            DATA.Is      = true;
                            DATA.GObject = MaxPower.EUnit;
                            Group->Mission.GetNewData(true,DATA);//Group->GetEnemy(MaxPower.EUnit);
                            }

                   }
                   }
        }
     }

void TGSociety::RefreshGroupParametres()
     {
       {
       for (int t=0;t<NUMBER_OF_TEAMS;t++)
       for (int i=0;i<NUMBER_OF_GROUPS;i++)
           {
              GetGroupPower(t,i);  //!!!
              if ((Groups[t][i].Is) && (Groups[t][i].POWER == 0))
       				DelGroup(t,i,"RefreshGroupParametres");
           }
       }
     for (int t=0;t<NUMBER_OF_TEAMS;t++)
     for (int i=0;i<NUMBER_OF_GROUPS;i++)
         {
            if (Groups[t][i].Is)
               {
            //   Groups[t][i].Mission.RefreshBase();
               Group_Analis(t,i);     //!!!
               Groups[t][i].Mission.DoMission();
               Groups[t][i].Analis(); //!!!
               Groups[t][i].Mission.CheckOnComplete();
               }
         }

     }

//------------------------------------------------------------------------------
//                 � � � � � �     � � � � � � � � � � � �
//------------------------------------------------------------------------------
TMax GetMax(int X1,int X2,int Y1,int Y2,int Team,TGSociety::TKvadrats KV[NUMBER_OF_KVADRATS][NUMBER_OF_KVADRATS])
    {                                             //���� ���� �-�� ������ ������� �����: ����������������� �������.
                                                 // ����� ������������ ��������� ����� �� 4-����
                                                //� ������������-��������� ���-�� ������ � ������� �������� �����, �������� ���������� � ���������� ���������.
    int DeltaX = X2-X1,
        DeltaY = Y2-Y1;
    TMax Max;
    Max.value = 0;
    Max.index = -1;
    Max.index2 = -1;

    if ((DeltaX >= (KVADRAT_WIDTH*2))&&(DeltaY >=(KVADRAT_HEIGHT*2))) // ���������� ���������� ������ ��������
        // �� 4-� "4-�����" � ������������ ������������� � ���� ������������
        {
        TMax Max_Kv[4];
        Max_Kv[0] = GetMax(X1, round(DeltaX/2)+X1 ,Y1 , round(DeltaY/2)+Y1,Team,KV); //������� 1;1
        Max_Kv[1] = GetMax(X1+round(DeltaX/2),X2 , Y1 , round(DeltaY/2)+Y1,Team,KV);  //������� 2;1
        Max_Kv[2] = GetMax(X1, round(DeltaX/2)+X1 ,Y1+round(DeltaY/2), Y2,Team,KV);//������� 1;2
        Max_Kv[3] = GetMax(X1+round(DeltaX/2),X2 , Y1+round(DeltaY/2), Y2,Team,KV); //������� 2;2
        for (int i=0;i<4;i++)
            {
            if (Max_Kv[i].value > Max.value)
               {
               Max.value = Max_Kv[i].value;
               Max.index = Max_Kv[i].index;
               Max.index2 = Max_Kv[i].index2;
               }
            }
        return Max;
        }
        else
            {
            int OX = X1/KVADRAT_WIDTH,
                OY = Y1/KVADRAT_HEIGHT;

            int _max = 0;
            if (KV[OX][OY].Number!=0)
            for (int i = 0;
                 i<KV[OX][OY].Number;
                 i++)
                {
                if (KV[OX][OY].Units[i]->TeamIndex==Team)
                _max++;
                }


            Max.value = _max;
            Max.index = OX; //+ (KVADRAT_WIDTH/2);
            Max.index2 = OY;// + (KVADRAT_HEIGHT/2);
//            if (Max.index2 == -1) ShowMessage("������ � �-� GetMax!");
            return Max;


            }


    }

TMax TGSociety::GetEnemyCentre(int Team)
     {//����� �����, ���� ����� �� ������ � �������� ��� �������� ������������ "-1";
     TMax MAX,ret;
     MAX.index = -1;
     MAX.index2 = -1;
     MAX.value = 0;
     if (Teams[Team].Number==0)return MAX;
     for (int t = 0;t<NUMBER_OF_TEAMS;t++)// �� ���� ������ �������� ����������
         {
         if (t == Team) continue;
         if (Teams[t].Number==0) continue;
          ret = GetMax(0,MAP_WIDTH,0,MAP_HEIGHT,t,Kvadrats);
          ret.index3 = t;
          if (ret.value > MAX.value) MAX = ret;
         };
     if ((MAX.index == -1)||(MAX.index2==-1))return MAX;
     MAX.index *= KVADRAT_WIDTH;
     MAX.index2 *= KVADRAT_HEIGHT;
     return MAX;
     }
TGObject* TGSociety::MakeUnit(int Team, int Type, int X, int Y)
          {//number...
          bool Result = false;
          TGObject* Obj = NULL;
          for(int i = 0;
              (i<=Teams[Team].Number)//"<=" � �� "<" ��� ����, ��� ��� ���� ��������� � ������ ������ ������
              &&         //
              (i<NUMBER_OF_UNITS)//�� ������������ ����� ������ ��������� ��-��.
              &&
              (!Result);
              i++
             )
             {
             if (!GObject[Team][i].Is)//��� ������ ������� ������ ������, �� ������ ���� �����.
                {
                Result = true;
                PutUnit(Team,i,Type,X,Y);
                /*int k=0;
                int X = Teams[Team].Centre.X,
                    Y = Teams[Team].Centre.Y;
                int OtherIndex,OtherTeam;
                    if (MAP[X][Y].Team>=0)
                       {
                       Result = false;
                       OtherTeam = MAP[X][Y].Team;
                       OtherIndex = MAP[X][Y].Index;
                       GObject[OtherTeam][OtherIndex].GetGlobalPos(2,(rand()/1000),AI);//!!!���� ��������� ���������� �� ���� ������!!
                       }
                       {bool PutUnit_ = PutUnit(Team,i,Team,X,Y);
                       while(!PutUnit_) //�������� �������� �����
                       {

                       X += rand()/10000;
                       Y += rand()/10000;
                       if (X<0) X = -X;
                       if (Y<0) Y = -Y;
                       PutUnit_ = PutUnit(Team,i,Team,X,Y);
                       }
                       }
                       Obj = &GObject[Team][i];

                       GObject[Team][i].GetGlobalPos(5,0,AI);
                */
                return Obj;

                }
             }
             return Obj;
          }



TGGroup* TGSociety::MakeGroup(int Team,int Number)
     {
     TGGroup* Group=NULL;
     int n  = 0;
     for (
         int i = 0;
          (i<NUMBER_OF_UNITS)
          &&
          (n<Number);
         i++

         )
      {

          if ((GObject[Team][i].Is)&&(!GObject[Team][i].InGroup))
             {
             if (n==0)
             Group = PutGroup(Team,&GObject[Team][i]);
             else
                {
                if (Group!=NULL)
                    {
                    if(!GObject[Team][i].InGroup)
                    Group->GetUnit(&GObject[Team][i]);
                    }
                }
             n++;
             }

      }
     return Group;



     }
void TGSociety::DestroyGroup(TGGroup *Group)
{
DelGroup(Group->Team,Group->Index,"DestroyGroup");
}

void TGSociety::SendGroupsInAttack(int Team,int Number,int X,int Y,int Value)
     {
     if((X==-1)||(Y==-1)) return; //���� ���� �� ������;
     int n = 0;
	 int i = 0;
     for (
           i = 0;
           (i<NUMBER_OF_GROUPS)
		   &&
		   (Groups[Team][i].Is)
           &&
           (n<Number);
           i++
         )
      {
      if ((!Groups[Team][i].Mission.Complete)&&(Groups[Team][i].Is))
         {
         TGGroup::TDATA DATA;
         DATA.Is = true;
         DATA.XY.X = X;
         DATA.XY.Y = Y;
         Groups[Team][i].Mission.GetNewData(2,DATA);
         Groups[Team][i].OnMission = true;
		 Groups[Team][i].NumberToKill = Value;
         n++;
         }
      }

     }
void TGSociety::SendSelectedUnits (int Team,short int LOG_DATA, TGGroup::TDATA DATA)
     {
     if (ViewMode)
	     return;



     if (DATA.XY.X == -1)
		 return;


     bool CanReturn = true;
     for (int i = 0; i<NUMBER_OF_UNITS;i++)
         {
         if (!GObject[Team][i].Selected)
            continue;

         TGObject *Unit = &GObject[Team][i];
         TGGroup  *Group = &Groups[Unit->GroupTeam][Unit->GroupIndex];

         if (!Unit->InGroup)
            CanReturn = false;
         if ((Unit->InGroup)&&((Group->Mission.LastDataSend.XY.X != DATA.XY.X) || (Group->Mission.LastDataSend.XY.Y != DATA.XY.Y)) )
            CanReturn = false;

         }
         if (CanReturn)
		 return;

     // 1 ������� ���� ������� �� �����
     // 2 ����� ������� �� ��� ������
     // 3 � ����� ��������, ���� ����


     TGGroup* CGroup = NULL;
     bool Created = false;
     // 1, 2
     for (int i = 0; i<NUMBER_OF_UNITS; i++)
         {
         if (GObject[Team][i].Selected)
             {
/*1*/        GObject[Team][i].OutGroup();
             if (!Created)
				{

/*2*/           CGroup = PutGroup(Team,&GObject[Team][i]);
                if (CGroup!=NULL)
					Created = true;

				}
                else
                CGroup->GetUnit(&GObject[Team][i]);

             }


      }

if   (!Created)
     return;
     /*3*/
     CGroup->Mission.PreferXY = Teams[MAN_TEAM].SelectedPreferXY;

     CGroup->Mission.LastDataSend = DATA;
     CGroup->Mission.GetNewData(LOG_DATA,DATA);

                
     }

void TGSociety::DoAIAction(int Team)
     {
	 TMax Enemy = GetEnemyCentre(Team);
     int makedgroups = 0;
     for (int i = 0;i<10;i++)
         {
         //if (rand()>22000)
//         MakeUnit(Team);
//         MakeUnit(Team);
         if (MakeGroup(Team,round(Enemy.value*2))!=NULL)
            makedgroups++;
         }
//     if (Team!= MAN_TEAM)
     if (Enemy.value > 0)
     SendGroupsInAttack(Team,2,Enemy.index,Enemy.index2,round(Enemy.value*2));
     }

//------------------------------------------------------------------------------
//                       ���������� ������� � ������   
//------------------------------------------------------------------------------
void TGSociety::SetSelectedUnits(int _X,int _Y,int X_,int Y_)
     {
     for (int i = 0; i < NUMBER_OF_UNITS; i++)
         {
         int x = GObject[MAN_TEAM][i].X,
             y = GObject[MAN_TEAM][i].Y;
         if (InXYSpace(_X,X_,_Y,Y_,x,y,true,true,true,true))
            GObject[MAN_TEAM][i].Selected = true;
            else
            GObject[MAN_TEAM][i].Selected = false;
         }
     }


void TGSociety::ChangeViewMode()
     {
	 ViewMode   = (ViewMode)?false:true;
	 }
void TGSociety::ChangeAttackMode()
     {
     AttackMode = (AttackMode)?false:true;
	 }

void TGSociety::ChangeChaseMode()
     {
	 ChaseMode  = (ChaseMode)?false:true; 
	 }

void TGSociety::ChangeMoveMode()
     {
	 MoveMode   = (MoveMode)?false:true;
	 }  

void TGSociety::ChangeAttackModeForSelected()
     {
	 for (int i = 0; i<NUMBER_OF_UNITS;i++)
	     if (GObject[MAN_TEAM][i].Selected)
		     GObject[MAN_TEAM][i].CanAttack = AttackMode;

	 }
void TGSociety::ChangeChaseModeForSelected()
     {
	 for (int i = 0; i<NUMBER_OF_UNITS;i++)
	     if (GObject[MAN_TEAM][i].Selected)
		     GObject[MAN_TEAM][i].CanAttackMove = ChaseMode;
     }

void TGSociety::ChangeMoveModeForSelected()
     {
	 for (int i = 0; i<NUMBER_OF_UNITS;i++)
	     if (GObject[MAN_TEAM][i].Selected)
		     GObject[MAN_TEAM][i].CanMove = MoveMode;
	 }
   

void TGSociety::RefreshAll(int &AnalisInterval,int &SetAngelsInterval,int &DoAIInterval)
{
// ����������
/*if (NUMBER_OF_TEAMS>1)
   {
   int TeamsNumber = 0, ind;
   for (ind = 0; ind<NUMBER_OF_TEAMS; ind++)
       if (Teams[ind].Number > 0) TeamsNumber++;
   if (!(TeamsNumber-1))
      GSocietyInit(true);
   };       */



// ----------

	    TGGroup::TDATA Data;
		Data.XY.X = Teams[MAN_TEAM].SendXY.X;// Pointer.Pos.X;
		Data.XY.Y = Teams[MAN_TEAM].SendXY.Y;// Pointer.Pos.Y;
		if (Data.XY.X >= 0)
		SendSelectedUnits(MAN_TEAM,2,Data);
		
		Teams[MAN_TEAM].SendXY.X = -1;


RefreshGroupParametres();



AnalisInterval ++;
if (AnalisInterval>ANALIS_INTERVAL)
    AnalisInterval = 0;

    SetAngelsInterval ++;
    if (SetAngelsInterval>SET_ANGELS_INTERVAL)
        SetAngelsInterval = 0;

    DoAIInterval++;
    if (DoAIInterval>DO_AI_INTERVAL)
        DoAIInterval=0;


     //      --**����������� ������**--

//if (SetAngelsInterval==SET_ANGELS_INTERVAL)
ANGLE_MATRIX_GetAngles();

for (int t=0; t<NUMBER_OF_TEAMS; t++)
  {
if (DoAIInterval==DO_AI_INTERVAL)
		DoAIAction(t);


  for (int i=0; i<NUMBER_OF_UNITS; i++)
    {
    int Index,Team,c_i;

    //if ((t==1)&&(i==78))
    //   bool Stop = true;

    if(GObject[t][i].Is)
        {

        /*if (AnalisInterval == SET_ANGELS_INTERVAL-2)

        Analis_Change(t,i,c_i);*/
        RefreshObjectParametres(t,i);

        };
//     --**����� ����������� ������**--

    };

  }
}
void TGSociety::RefreshFrames(int &AnalisInterval,int &SetAngelsInterval,int &DoAIInterval,int &Frame)
          {
              if (Frame == 0)
                 {
                 RefreshAll(AnalisInterval,SetAngelsInterval,DoAIInterval);
				     for (int t = 0; t<NUMBER_OF_TEAMS;t++)
                        for (int i = 0; i<NUMBER_OF_UNITS;i++)
                           GObject[t][i].Target.Refresh(Frame);
                 }
                 else
                     {
                     for (int t = 0; t<NUMBER_OF_TEAMS;t++)
                        for (int i = 0; i<NUMBER_OF_UNITS;i++)
                           {
                           GObject[t][i].Target.Refresh(Frame);
                              if (!GObject[t][i].Is)
                                  continue;
                           GObject[t][i].GetTempXYZAngle(Frame);
                           }
                     }
          Frame++;
          if (Frame>=MAX_FRAMES)
             Frame=0;

          }




