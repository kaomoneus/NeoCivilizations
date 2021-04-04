//---------------------------------------------------------------------------
#ifndef MainUnitH
#define MainUnitH

#include "TextConstants.h"
#include "Constants.h"
#include "Standarts.h"

//---------------------------------------------------------------------------
struct TForFound
  {
  int Type,Index;
  float R;
  float Angle;
  };
struct TMax
  {
  int index;
  int index2;
  int index3;
  float value;
  float value2;
  };
struct TXY
	{
	short int X,Y;
	};
struct TMap//MAP
  {
  int Team,Index;
  bool Selected;
  float Z,Tex;
  };




  class TSection
  {

  friend class TGSociety;
  friend class TGObject;

  private:
  // ��� ������ ������
  struct TGList
         {
         TSection **Section; // ��������� �� ������ ������ ������ ���������� �� ������
         int TotalNum;       // ���������� ���� ��������� � ������
         int Num;            // ���������� ��������� � ������ �� ������ ������
         };


  public:
  // ��������� ��������� ����������
  int Time;
  // ------------------------------

  // ���������(����) ������

  // ��� ��� ������� � �����������
  TSection* OldestSection;   // ����� ������ ������.
  bool Open;                 // ������� �� ������ (����� �� ��� �������� ������)
  bool Occupied;             // ���� �� ������ ������ ������������ �������
  TSection *Daughter[2][2];  // ���� ������ ������, �� ��� ����� �������� ����� ( � ����� ������ 4 ��.)
  int Generation;            // ��������� ���� ������ (��������� ������� = 0, � �������� ��������� �� 1 ������
                             // �����������
  int ScionsNumber;          // ����� ��������.
  int MaxExistingGeneration; // ��������� ����� ������� ������.
  int MaxGeneration;         // ����������� ��������� ���������, ��������� ��� ������� ������ ������ - ���� ������

  // ��� ��� �������� �� �������

  bool Creeked;              // ������ �� ������
  TSection *FromSection;     // �� ����� ������ ������ ��� ������
  float Steps;               // ����������, ������� ����� ������ �� ������ ������� �� ���� ������
  TSection *SectionWithA;    // �������� ������, ���������� ����� �
  TSection *SectionWithB;    // �������� ������, ���������� ����� �
  TSection *SectionWithA_B;  // ����� ������� ������, ������� �������� ������ � � � ������������.

  // ������������ �� �����
  TXY *A;    // ����� �.
  TXY *B;    // ����� �.
  int X,Y;   // ���������� ������ ������
  int SX,EX; // ������� ����� ������ StartX, EndX
  int SY,EY; // StartY, EndY
  TSection* SectionWithObject; //�������� ������ � ������� ����� ������.

  // ������� ���
  TGSociety *Society;  // ������� ��� - �����, ������� � �.�. � �.�.
  TGObject* OurObject; //  ������, ��� �������� ������������ �������.


  // ������ ������
  // ������ � ��������
  bool GetNearestPoint();                                 // �������� ��������� ����. �����.

  // �������, ������ � � �.
  bool   GetA(TXY _A/*,TCanvas* Canvas*/);                // �������� ���������� ������ "�"
  bool   GetB(TXY _B/*,TCanvas* Canvas*/);                // �������� ���������� ������ "�"
  bool   Creek(/*TCanvas *Canvas ���������*/);            // ������� �� ������ "�" �� ������ "�"
  bool   FindWay(/*TCanvas* Canvas*/);                    // ���� ����, � ������� ������� ��������� ������� ������
//void   DrawWay(TCanvas* Canvas);       /*����������*/   // ������� ���� �� �����

  // ������ �� �������� ������
  TGList GetAllNeighbour(/*TCanvas* Canvas*/);            // ����� ���������� ������ ���� "����������" ������� ���
                                                          // ���� ������
                                                          // ����� ������ ������ ����������� ���� ���� �������
                                                          // Step � FromSection

  TGList GetDaughterNeighbour (TSection *_Section);           // ����� ���������� ������ �������� ������� ������ Section
                                                          // ��� ���� ������.
                                                          // ������ �� ������, ������� ����� ����� �������� �� ����
                                                          // ����������: ��������� ������� �� ������ ��������� ������
                                                          // � ����, �� � ��, ������� ����� �������� ������� ���� ������,
                                                          // ������ ��� ������� ����� ���� ������ ����� ������������
                                                          // �������� ������ ������.

  bool   CheckAndFillSection(TSection* _Section);         // �������� �� ���������� ������� ������.
                                                          // ���� ��� ����� ����� ��������, �������
                                                          // �������� ������, � ����������� ���� ����-
                                                          // ������� �����.


  bool   AddList (TGList *InList, TGList *SubList);       // ���������� ���������(SubList) � ������ (InList)
  bool   DelFromList(TGList *_List, int ElementIndex);    // �������� �� ������.
  bool   AddSection(TGList *_List, TSection* Section);    // ���������� ������ � ������.

  // ������ � ��������

  bool   Init(TGSociety *_Society, TGObject* _OurObject,
              bool Oldest);                               // ������������� ������, ������ ���������.
  bool   EasyInit();                                      // ������������� �����, ���������� �� �������.
  bool   FingerAllSections(bool EInit,
                           bool CheckOnAB,
                           bool FindObject
                           /*,TCanvas *Canvas*/
                           );                             // ������� ���������� ��� ������, �������� �� ����������
                                                          // ���������� � ������ ���������� ������ � ��� �,
                                                          // ����������� ����� ����� �������� ����� ������ ������,
                                                          // ����� ��������
                                                          // ������������� ����� ��������� � ��������,
                                                          // ����� �������� ������, ���������� ������ � � �,
                                                          // ����� �������� ������, ���������� ������.
                                                          //
                                                          // �������� ������ ������ �� ����������
                                                          // ����������� ������ ����.
                                                          // /**����� ������ ���������� ������ � � �**/

  bool   DelGeneration(int G);                            // ������� ��� ������ � ���������� G � ����
                                                          // �������� ����� ���������.
                                                          // ��������� ������ ���� ������ ���� ������.
                                                          // ����� ������� ������, ������� ���� �������� ��� ���-
                                                          // ������ ������.


  bool   SplitSection();                                  // ���������� ��� ���������� �� �� �������� ������ ������ ��
                                                          // ��������. ���� ��� ����������� �������� �� ������������
                                                          // false ���� �������� �� true
  bool   CheckMap(bool ForSection,int CentX,int CentY);   // �������� ������ ������ ��� ������� ������� ��� ������.
                                                          // ���������� CentX,CentY ��������� �� ���������� ������
                                                          // ����������� ������� � ������������� ������ ��� ��������
  };




class TPoints
{
friend class TGObject;
friend class TGSociety;

public:
TGObject *OurObject;

TXY A,B;      // �� ������ � �������, � ����� � �������. ��� �� ����������.
struct TWays  // � ��� ����� ����������� ....
       {
       bool Where[3][3]; // Where are you from ?
       float Step;
       bool Creeked;  
       };
       TWays** Point;
//   system methods
bool Init (TGObject* _OurObject);

//   help methods
bool CheckThis(int X,int Y);
bool CheckPoint(int X,int Y);
//   external methods
bool GetA();
bool GetB(TXY _B);
bool Creek(TXY _XY/*,TCanvas* Canvas*/);
bool GetNewPoint();
};

class TTarget
{
friend class TGSociety;
friend class TGObject;
public:
TGSociety* Society;
TGObject* OurObject;
bool On;        //�������� ��� ���
bool Roof;
int X,Y;float Z;      //���������� �� �����
float DX , DY,  // ������� � ��������� �X�
               DZ;
float Distance; //���������� �� ����� �����
float WentWay;  //���������� ����
float OldWentWay;
float Angle;    // ���� � ��������� �X�
float StartZTangens; // ������� � ��������� ��Speed
int Frame;
float CurrentX, CurrentY, CurrentZ;
float OldCurrentX, OldCurrentY, OldCurrentZ;
float StartX, StartY, StartZ;
int Speed;
float SpeedOXY; //�������� � ��������� ��� 

float Timer;      //������� �������
float MaxTime;    //����� ����� ������� � ������ ����� ���������� "�����"
bool  MustFree;
int   FreeTimer;
float Radius;   //������ ���������
int Damage;     //���� ���������� � ������ �����

void Init(TGSociety *_Society,TGObject *_Object);
bool Fire(int X,int Y);
bool CheckFire();
bool GetTarget(int _X,int _Y,float _Z);
void FreeTarget();
void GetCurXYZ();


void Refresh(int _Frame);
};


class TGObject
{
friend class TTarget;
friend class TGSociety;
/*
��������� ������ ������������� ��������� � ����� ��� � ������, �������� ��� ��
������ �������, ��������� �������� ���������. ������� � ���� ������ ����� -
��� ����� ���������, ��� ������, � ������ �����. ���� ��� ��� �������� �
�������� ����, �� ����� ����������� ��������� ��� �������� � ��������� ���������
��� �������� � ��������.  � � �������������� �������� ������� ��������������� ��������.
*/
public:
//�������
int TeamIndex;
int Index;
int Type;                              // ��� �����: ����, �����, ���������� � �.�...������ ��� �����-������...
bool InGroup;                          // �������� � ������
int GroupIndex;
int GroupTeam;
long int GroupPOWER;
int IndexInGroup;

void OutGroup();                       // �����: ����� �� ������;

struct {
       int IndexOX,IndexOY,
       ID;                 //������ ����� � ��������
       } Kvadrat;                      // ���������� � �������� � ������� ��������� ����
//�������� ���������


int Live;                              // �������
int Sight;                             // ������
int Speed,AngleSpeed;                  // ��������, � �������� �������� ��������������

void Dye();                            // �����: �����������.

//��������� ���������
TGSociety* Society;

TSection SectionWay;                   // ������ �������� ��� ���������� �������
                                       // ������� �������� ������.

//TPoints Points;                        // ���� ����� ������ ������� ������ ������.


void Init(TGSociety *_Society,int n/*NUM_OF_STDRTS*/,int _Type,int _Team,int _Index);
                                       // �����: �������������.
void GetWeapon(int ind);              // �����: �������� ������.

TWEAPON_TYPE TypeWeapon;               // ��� ������ (�����, �����������, ��������)
int IndexWeapon;                       // ������ ������.




int ModelFrame;                        // ������� ������������ ����
                                       // (����� ��� ����������� �� ������).
int MAX_ModelFrames;                   // ������������ ����� ����� ������.

void SetModelFrame();                  // �����: ���������/����� �����
                                       // ��������� ���������.


struct TIncInBase
       {bool Yes;
       int Index;};
       TIncInBase** IncludedInBase;    // ���� ���������� ���������� � ���,
                                       // ��� ������ ��� ��������� ��� ����.

void IncBaseInit();                   // �������������� ���� ������������.

bool Selected;                         // ���� ������������ ������� �� ����.


//-------------------
//ATTACK
TTarget Target;                        // ������. �� ������ ������ ������ ���
                                       // ����

bool CanAttack;                        // �������� ������������ ����� �� ������
                                       // ���������.

bool CanAttackMove;                    // ������� ������������ ����� �� ������
                                       // ������������ �����.

void SetAttack(TATTACK_MODE PARAMETER);
                                       // �����: �����������  ��������� ������-
                                       // ����� � ����� (�����/�������
                                       // ������������,
                                       // �����/������� �������������,
                                       // �����/������� ���������).

int AttackPeriod;                      // �������� ����� ����������.

int TimeAfterAttack;                   // ����� ��������� ����� ��������� �����.

int Range;                             // ��������� �����.
int MinRange;                          // ����������� ��������� �����.
int DamageRadius;                      // ������ ���������.
int Damage;                            // ��������� ��������� ��������.
int Armor;                             // �����.
//END OF ATTACK


long int POWER;
void GETPOWER();


int X,Y,OldX,OldY;
float Z,OldZ,TempZ; //���������� ��� �������� "�����"
float Angle,OldAngle,TempAngle;
float OldTempX,OldTempY,TempX,TempY;
void GetTempXYZAngle(float Frame);
void GetTempAngle(int Frame);
int Width;
bool CheckPoint(int x,int y);
//unsigned int Status:3; //�� ����� �������� ���� ���� �� ����� - ������ ����� (�������,������ �����,���� �.�.�.)
void GetNewPos(float Angle,float R);
void GetNewPos(int fX,int fY);      //��� ������� ��������� ����� ������� - ���� ���� ����� � ���
void FreePosition();

void GetGlobalPos(int fX,int fY, int WhoCommand);   //��������� ���������� ������� - ���� �� ������� ���� ��������� ������ ������
void GetGlobalPos(float R,float Angle,int WhoCommand);

void SetGlobalAngle_R();
void Stop();
//void UnStop();

bool Is,OldIs;
bool CanMove;
bool Thinking;
struct{
      int X,Y,  //���������� ����� �������
          GlobalX, GlobalY; // ���������� ���������� ������� - ���� � ������� �������� ���� ��������
                            // � �����...

      float  Angle,         // ���� (�� ������ ����� ��� ����������� ����� �� ������)
                            // �� ������� ����� ���������������� ������ ����� � ����� �������

             GlobalAngle,   //� ���� ���� ����� � ������� SetPos - ���� �� ������� ���� �����������,
                            //����� ������ ����� � ���������� ����
             GlobalR;       //���������� �� ����

      int SmallPoint, Steps;//������ �, ��������, ��������� - ����� ��� ����������� �����

      bool OnX;             //���������� ���������������
      float DeltaX,DeltaY;  // ��� �������� ��� �� ������ ������� �������� �� ����

      }NewPosition;


// ����
int NVO;  // Number of Visible Objects
int N;  //�������� ������
struct TStatus
        {
        bool Moving;
        bool MustMove;
		bool Attack;
		bool Fire;
		int  FireMovie;
        bool InShock;
		bool Deading;
        } Status,OldStatus;
void SetStatus();
bool CanKill (TGObject *Enemy);
void GetEnemy(TGObject *Enemy);
void GetEnemy(TForFound Enemy);
void SetEnemy();
int ACTION;
TForFound PrimEnemy;
TGObject *PrimaryEnemy;
void DoMission();
void GetMission();
bool OnMission;


void Refresh();
int Character;
};


//---------------------------------------------------------------------------
//                            G R O U P
//---------------------------------------------------------------------------

class TGGroup
{
friend class TGSociety;
//���������� ������ � ������ �� ���� �� ������������� �� ������� � ��������
public:
void Init(TGSociety *_Society);
bool Is;
TGSociety *Society;
TXY Distination;
int Team,Index; // ������� � ������ ������ � ���� �������
bool OnMission;
void Refresh();
struct TIncInBase {bool Yes;int Index;};
       TIncInBase** IncludedInBase;


// --**� � � � � ���������� � ������**---
//------------------------------------------
int Number; /* �����������                */
int Live;   /* ����������� ����� (����!)  */
long int POWER;
void GetPower();
int X,Y;
//------------------------------------------

// ���������� � ������ ������� � ��� �������
//------------------------------------------

struct  TUnitStruct
        {
        TGObject *PUnit;  /* ������ �� ����� */
        int IndEnemy; /*��� ���� ������ "��� �����" �
                                        ��� �������� ������ ������ ������� Units ��������� ������,
                                        � ������� �������� ��������� �� ���������� �����
                                        */
        bool Is;
        };
        TUnitStruct* Units;
void GetUnit(TGObject *Unit);
void Sort();
//------------------------------------------


//          � � � � � � � � � � �
//------------------------------------------
void GetNewPos(int X,int Y);
//------------------------------------------


//              � � � � �
//------------------------------------------

int NumberToKill; //���������� ������ ������� ���� �����;
int NumberOfKilledEnemies; //���������� ������ ������;

void Free();//������������ ������ �� �������;
void SetAttack(TATTACK_MODE PARAMETER);
TGGroup *EGroup;    //��������� ������
TGObject *EUnit;
void GetEnemy(TGObject *Enemy);
void GetEnemy(TGGroup *Enemy);
void Analis ();  //�������� "�����-��������" ������� ����� ������

//------------------------------------------
//� � � � �   � � � � � � �
struct
    TDATA
    {
    bool Is;
    TGObject* GObject;
    TGGroup* GGroup;
    struct {int X,Y;} XY;
    };
class TMission
  {
  public:
  void Init(TGGroup *_OurGroup);
    TGGroup  *OurGroup;

  int      EnemyNumber;

  struct   TEnemy
           {
           int Object_Group;
           TDATA Object;
           } Enemy[MAX_ENEMIES];
  bool     Complete;
  struct   {int X,Y;}
           XY;
  bool     Articulate;
  bool     PreferXY;

  TDATA LastDataSend;
  bool GetNewData(short int LOG_DATA, TDATA _DATA);  // ��������� ����� ������
  bool MoveData(int from_i,int on_i);                // ������� ������ �� ������ from_i � ������ on_i
                                                     // ��� ���������� � ������ on_i ��� ���� ����������������
  void FreeDataCell(int i);                          // ������� ������� ������ ����������
  void DoMission();                                  // ���������� ������� 
  bool CheckOnComplete();                            // �������� �� ���������� ������� 
  void RefreshBase();                                // ����������, � ������ ���������� �� ���� "������" � ����
  }Mission;



};








//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
class TGSociety
{
public:
int Dead;

// ����� ��������� � ������������ ��������, ��� ��������� � ��� ����, ����� ������ ������ � ����

TMap **MAP;/*[MAP_WIDTH][MAP_HEIGHT];*/
                                  // ������ ����� ������������ ����� ��������, � ������� ������ �� ���
                                  // ���� ��� ����� ������, �� ������ ��� ������� � ������� �� ������-
                                  // �����.
                                  // ���� ���� ������� ���������� �� ��� ������ ����� ����� NUMBER_OF_TEAMS,
                                  // ������ �� ���� ������ ������ ��������� �� ������� �������.
                                  // ������� ��������� �������, ���� ��������� ���� ������ ����.
                                  // ���� �������� ���� � ���� ����� ����� -1, �� ������ ����� ��������.
                                  // ���� ������ ������ �������� �� ���� Index �������� ������ ����� ������� � ���
                                  // �������.
                                  // ����� � ������ ������� �� ������ ���. �������� ������.
                                  // ���� Selected ...


TForFound***
ANGLE_MATRIX;//[NUMBER_OF_TEAMS][NUMBER_OF_UNITS][NUMBER_OF_UNITS*NUMBER_OF_TEAMS];
                                  // ������ ����� � ���������� ����� �������
                                  // ��� ������� ����� �������� ������ ������ ������, ������� ��
                                  // ����� �������. ���������� ������� ������ ���������� ����������.
                                  // ���� ����� ������ ������� �� ������ ���� ������ ������.
                                  // ������ ������ ������� ��������� �� �������, � ������� �����������
                                  // ���� ����, ������ ������ - ������ ����� � �������, ������ ������
                                  // ��������� �� ������ ������� � ������ ������� �������� ��� ����� �����
                                  // ������ ������� ������������ ����� ���������, ����������
                                  // ����� �������, � ������� ����������� ������, ������ ����� ������� � �������
                                  // ���� ������������ ��������, ����������� ������� � ������������� �������, �
                                  // ���� ��.



void InitMapPassably ();          // �����: � ������������ � ����������
                                  // ���������� ������������ ������� �����.

struct TKvadrats
{
int X,Y,X_,Y_;   // ���������� �������� ������ ���� � ������� ������� �������������.
int a,b;         // ������ � ������ "��������".
TGObject* *Units;// �����, ������� � ���� �������� ���������.
int Number;      // ���������� ������ � �������� � ������ ������.

}
Kvadrats[NUMBER_OF_KVADRATS][NUMBER_OF_KVADRATS];
                                  // ����� ��������� �� ��������, ��� ����� ���
                                  // ����� �������� ����������� ������� ��������.


void Check_Put_UnitInKvadrat(int Team,int Index);
     void AddUnitInKvadrat(int OnOX,int OnOY, TGObject *Unit,bool ADD);
                                  // ������: ���������/�������� ������ � �������.
                                  //                  �������� ������ � �������.



void ANGLE_MATRIX_Init();
void ANGLE_MATRIX_GetAngles();      // ����������� ���� � ���������� ����� ���������
float SmalestR(int Team, int Index);

struct TTeams                       // �������
       {
       int Number;
	   bool MEN;
	   TXY SendXY;
       bool PreferXY;
       bool SelectedPreferXY;
       struct TCentre{int X,Y;}Centre;
       };
       TTeams* Teams;
TGObject** GObject;
TGStandart* Standart;
TWeapon*    Weapon;

void LoadObjectMap();               // �����: ��������� ����� ��������.
void LoadMap();                     // �����: ��������� �����.
void LoadWeapons();                 // �����: ��������� ������.
void LoadStandarts();               // �����: ��������� �������.

bool LoadConfig();                  // �����: ��������� ��� ���������:
                                    // �����, ����� ��������, ������, �������.


void RefreshObjectParametres(int Team,int Index);
void GSocietyInit(bool first /*���������*/);
bool PutUnit(int Team,int Index,int Type, int X,int Y); //������ ����� �� �����
void Dye(int Team,int Index);
void Sinchronize(int Team,int Index,bool SnAction); //������ ���������� ����� �� ����� ����� ������ � � ������������
void MoveUnit(int Team,int Index); // ...

TForFound CheckMove(float Angle/*��� �������� ����*/,int Team,int Index); // ��������� ������� ���� ��� ���� �����
bool Check(float Angle,int Team,int Index,int &Left_Zero_Right/*����� �����-�����-������*/,
           float R/*��������� �� ���������� ����������� ������*/);

/*
��� ������� ��� ����� �� ����� �� ��� ��� ������ ������ ������� ��������� �
�������� �����(ANGLE_MATRIX).
*/
void SetPos(int Team, int Index/* TCanvas *Canvas*/);   // �������� �������� �����������
     //��� �� ���� ������ ���� ��������� �����������
     /*
     ��� ���� ������� ������:"���� ���������� ����������� ����� �� ���� � ��� - �� ���� �� ������"
     */
bool CheckPoint (TGObject *Unit, float Angle,bool WithAngle,int R,int I);


//--------------------------------------------------------------
//    ---  � � � � � � � � �   � � � � � � � ---
//--------------------------------------------------------------

//------------------
    bool ViewMode;     // ����� ��������� ���������� ��� ����������� � ������ � �������� ����
	bool AttackMode;   // ����� �����
	bool ChaseMode;    // ����� �������������
	bool MoveMode;     // ����� ������������


	void ChangeViewMode();    // �������� ���������
	void ChangeAttackMode();  // �������� ���������
	void ChangeAttackModeForSelected();
	void ChangeChaseMode();   // �������� ���������
	void ChangeChaseModeForSelected();
	void ChangeMoveMode();    // �������� ���������
	void ChangeMoveModeForSelected();

void SetSelectedUnits(int _X,int _Y,int X_,int Y_);

//------------------

//-------------------------------------------------------------
//    ---***  ������������� ��������� � ������� ���  ***---
//-------------------------------------------------------------

//------------------
//"������ ���������"
//------------------

void DoAIAction(int Team);
 TMax GetEnemyCentre(int Team);   //����������� ����� ���� ����� ����������������� ����� ������
  TGGroup* MakeGroup(int Team,int Number);
  TGObject* MakeUnit(int Team,int Type, int X, int Y);
  void DestroyGroup(TGGroup *Group); //DEL GROUP
    void SendGroupsInAttack(int Team,int Number,int X,int Y,int Value);
    void SendSelectedUnits (int Team,short int LOG_DATA,TGGroup::TDATA DATA);


//------------------
//   ��� � ������
//------------------
TGGroup** Groups;//[NUMBER_OF_TEAMS][NUMBER_OF_GROUPS];
int* GroupsNumber;//[NUMBER_OF_TEAMS];
//               Create the group
        TGGroup* PutGroup(int Team, TGObject* Unit);
//           Destroy group
        void DelGroup(int Team, int Index,char* caller);
        void Group_Analis(int Team,int Index);
        void RefreshGroupParametres();
        void GetGroupPower(int Team,int Index);
//------------------
//   ����� ������
//------------------

/* bool DoActoin()      */
      int Analis_Change(int Team,int Index,int index); //2. ���� ����������� ��������� ���� ������ �� ������� (��������, �����,�����),
                     //   �������� �������� ������� �� ���.
      void DoAction(int Team,int Index);
      void Attack(int Team,int Index);
           int TryFire(int Team,int Index,int type,int index,int TypeWeapon);
                void Fire (int Team,int Index,int type ,int  index);
                void SetEnemy(int T,int I);
                void Dye (TGObject *Unit);
                     void GoBackOn(int Team,int Index, int R,bool WithR);



void RefreshAll(int &AnalisInterval,int &SetAngelsInterval,int &DoAIInterval);
void RefreshFrames(int &AnalisInterval,int &SetAngelsInterval,int &DoAIInterval,int &Frame);


};

//-----------------------------------------------------------------------------
//             |                                           |
/*             |*//*          TGSociety Society;       *//*|                 */
//             |                                           |
//-----------------------------------------------------------------------------
TMax GetMax(int X1,int X2,int Y1,int Y2,int Team,TGSociety::TKvadrats KV[NUMBER_OF_KVADRATS][NUMBER_OF_KVADRATS]);


#endif
