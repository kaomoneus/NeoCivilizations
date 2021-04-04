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
  // Тип списка секций
  struct TGList
         {
         TSection **Section; // Указатель на первую ячейку списка указателей на секции
         int TotalNum;       // Количество всех элементов в списке
         int Num;            // Количество элементов в списке на данный момент
         };


  public:
  // ВРЕМЕННЫЕ СИСТЕМНЫЕ ПЕРЕМЕННЫЕ
  int Time;
  // ------------------------------

  // Параметры(поля) класса

  // Все что связано с поколениями
  TSection* OldestSection;   // Самая старая секция.
  bool Open;                 // Открыта ли секция (имеет ли она дочерние секции)
  bool Occupied;             // Есть ли внутри секции непроходимые участки
  TSection *Daughter[2][2];  // Если секция отрыта, то она имеет дочерние секци ( в нашем случае 4 шт.)
  int Generation;            // Поколение этой секции (поколение главной = 0, у дочерних поколение на 1 больше
                             // материнской
  int ScionsNumber;          // Число потомков.
  int MaxExistingGeneration; // Поколение самых младших секций.
  int MaxGeneration;         // Максимально возможное поколение, поколение при котором ширина секции - одна ячейка

  // Все что отвечает за заливку

  bool Creeked;              // Залита ли секция
  TSection *FromSection;     // Из какой секции залита эта секция
  float Steps;               // Расстояние, которое нужно пройти от центра заливки до этой секции
  TSection *SectionWithA;    // Закрытая секция, содержащая пункт А
  TSection *SectionWithB;    // Закрытая секция, содержащая пункт В
  TSection *SectionWithA_B;  // Самая младшая секция, которая содержит пункты А и В одновременно.

  // Расположение на карте
  TXY *A;    // Пункт А.
  TXY *B;    // Пункт В.
  int X,Y;   // Координаты центра секции
  int SX,EX; // Крайние точки секции StartX, EndX
  int SY,EY; // StartY, EndY
  TSection* SectionWithObject; //Закрытая секция в которой стоит объект.

  // Внешний мир
  TGSociety *Society;  // Внешний мир - карта, объекты и т.д. и т.п.
  TGObject* OurObject; //  Объект, для которого производится заливка.


  // Методы класса
  // Работа с объектом
  bool GetNearestPoint();                                 // Получить ближайшую конт. точку.

  // Заливка, пункты А и В.
  bool   GetA(TXY _A/*,TCanvas* Canvas*/);                // Получить координаты пункта "А"
  bool   GetB(TXY _B/*,TCanvas* Canvas*/);                // Получить координаты пункта "В"
  bool   Creek(/*TCanvas *Canvas индикатор*/);            // Заливка из пункта "В" до пункта "А"
  bool   FindWay(/*TCanvas* Canvas*/);                    // Ищем путь, с помощью подбора поколений младших секций
//void   DrawWay(TCanvas* Canvas);       /*недоделана*/   // Выводит путь на экран

  // работа со списками секций
  TGList GetAllNeighbour(/*TCanvas* Canvas*/);            // Метод возвращает список всех "отобранных" соседей для
                                                          // этой секции
                                                          // Также внутри метода заполняются поля этих соседей
                                                          // Step и FromSection

  TGList GetDaughterNeighbour (TSection *_Section);           // Метод возвращает список соседних дочерей секции Section
                                                          // для этой секции.
                                                          // причем те секции, которые имеет смысл заливать из этой
                                                          // ПРИМЕЧАНИЕ: соседними считать не только прлежащие секции
                                                          // к этой, но и ту, которая будет являться предком этой секции,
                                                          // тоесть все крайние точки этой секции будут принадлежать
                                                          // квадрату секции прекда.

  bool   CheckAndFillSection(TSection* _Section);         // Проверка на надобность заливки секции.
                                                          // если все тесты будут пройдены, функция
                                                          // заливает секцию, с заполнением всех необ-
                                                          // ходимых полей.


  bool   AddList (TGList *InList, TGList *SubList);       // Добавление подсписка(SubList) в список (InList)
  bool   DelFromList(TGList *_List, int ElementIndex);    // Удаление из списка.
  bool   AddSection(TGList *_List, TSection* Section);    // Добавление секции в список.

  // работа с секциями

  bool   Init(TGSociety *_Society, TGObject* _OurObject,
              bool Oldest);                               // Инициализация секции, секция закроется.
  bool   EasyInit();                                      // Инициализация полей, отвечающих за заливку.
  bool   FingerAllSections(bool EInit,
                           bool CheckOnAB,
                           bool FindObject
                           /*,TCanvas *Canvas*/
                           );                             // Функция перебирает все секции, передает по наследству
                                                          // Информацию о секции содеражщей пункты А или В,
                                                          // расчитывает общее число потомков самой старой секции,
                                                          // также вохможна
                                                          // ИНИЦИАЛИЗАЦИЯ ПОЛЕЙ СВЯЗАННЫХ С ЗАЛИВКОЙ,
                                                          // ПОИСК ЗАКРЫТЫХ СЕКЦИЙ, СОДЕРЖАЩИХ ПУНКТЫ А и В,
                                                          // ПОИСК ЗАКРЫТОЙ СЕКЦИИ, СОДЕРЖАЩЕЙ ОБЪЕКТ.
                                                          //
                                                          // Проверка кажной секции на содержание
                                                          // препятствий внутри себя.
                                                          // /**Поиск секции содержащей пункты А и В**/

  bool   DelGeneration(int G);                            // Удалить все секции с поколением G и всех
                                                          // потомков этого поколения.
                                                          // Поколение должно быть младше этой секции.
                                                          // Метод очищает память, которая была выделена под уда-
                                                          // ленные секции.


  bool   SplitSection();                                  // Разделеить все заполненые но не открытые секции предки на
                                                          // дочерние. Если они минимальных размеров то возвращается
                                                          // false если разделил то true
  bool   CheckMap(bool ForSection,int CentX,int CentY);   // Проверка центра секции для данного объекта или секции.
                                                          // Переменные CentX,CentY указывают на координаты центра
                                                          // проверяемой области и предназначены только для объектов
  };




class TPoints
{
friend class TGObject;
friend class TGSociety;

public:
TGObject *OurObject;

TXY A,B;      // Из пункта А выходим, в пункт Б заходим. Это их координаты.
struct TWays  // А это карта направлений ....
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
bool On;        //включена или нет
bool Roof;
int X,Y;float Z;      //координаты на карте
float DX , DY,  // Тангенс в плоскости ОXУ
               DZ;
float Distance; //расстояние до точки удара
float WentWay;  //пройденный путь
float OldWentWay;
float Angle;    // Угол в плоскости ОXУ
float StartZTangens; // Тангенс в плоскости ОУSpeed
int Frame;
float CurrentX, CurrentY, CurrentZ;
float OldCurrentX, OldCurrentY, OldCurrentZ;
float StartX, StartY, StartZ;
int Speed;
float SpeedOXY; //скорость в плоскости ОХУ 

float Timer;      //счетчик времени
float MaxTime;    //время через которое в данном месте происходит "взрыв"
bool  MustFree;
int   FreeTimer;
float Radius;   //радиус поражения
int Damage;     //урон нанесенный в центре удара

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
Появилась острая необходимость отнестись к юниту как к живому, поделить его на
разные области, полностью поменять структуру. Создать у него органы чуств -
это такие параметры, как зрение, и чуство врага. пока все это занесено в
корневое поле, но нужно обязательно разделить все свойства в отдельные струткуры
так понятнее и надежнее.  И в соответсвующих разделах создать соответствующие действия.
*/
public:
//паспорт
int TeamIndex;
int Index;
int Type;                              // тип юнита: воин, башня, катапульта и т.д...дерево там какое-нибудь...
bool InGroup;                          // членство в группе
int GroupIndex;
int GroupTeam;
long int GroupPOWER;
int IndexInGroup;

void OutGroup();                       // МЕТОД: выход из группы;

struct {
       int IndexOX,IndexOY,
       ID;                 //индекс юнита в квадрате
       } Kvadrat;                      // информация о квадрате в котором находится юнит
//ОСНОВНЫЕ ПАРАМЕТРЫ


int Live;                              // Здровье
int Sight;                             // Зрение
int Speed,AngleSpeed;                  // Скорость, и скорость поворота соответственно

void Dye();                            // МЕТОД: Умертвление.

//Системные параметры
TGSociety* Society;

TSection SectionWay;                   // Объект служащий для реализации заливки
                                       // методом неравных секций.

//TPoints Points;                        // Тоже самое только методом равных секций.


void Init(TGSociety *_Society,int n/*NUM_OF_STDRTS*/,int _Type,int _Team,int _Index);
                                       // МЕТОД: Инициализация.
void GetWeapon(int ind);              // МЕТОД: Получить оружее.

TWEAPON_TYPE TypeWeapon;               // Тип оружия (лазер, прямобойное, навесное)
int IndexWeapon;                       // Индекс оружия.




int ModelFrame;                        // Текущий анимационный кадр
                                       // (нужен для отображения на экране).
int MAX_ModelFrames;                   // Максимальное число таких кадров.

void SetModelFrame();                  // МЕТОД: Инкремент/сброс кадра
                                       // управляет анимацией.


struct TIncInBase
       {bool Yes;
       int Index;};
       TIncInBase** IncludedInBase;    // База содержащая информацию о том,
                                       // кем объект был обнаружен как враг.

void IncBaseInit();                   // Инициализирует базу обнаруживших.

bool Selected;                         // Поле показывающее выделен ли юнит.


//-------------------
//ATTACK
TTarget Target;                        // Оружие. На данный момент только для
                                       // ядер

bool CanAttack;                        // Свойство показывающее может ли объект
                                       // атаковать.

bool CanAttackMove;                    // Свойтво показывающее может ли объект
                                       // преследовать врага.

void SetAttack(TATTACK_MODE PARAMETER);
                                       // МЕТОД: Настраивает  параметры переме-
                                       // щения и атаки (может/неможет
                                       // преследовать,
                                       // может/неможет передвигаться,
                                       // может/неможет атаковать).

int AttackPeriod;                      // Интервал между выстрелами.

int TimeAfterAttack;                   // Время прошедшее после последней атаки.

int Range;                             // Дальность удара.
int MinRange;                          // Минимальная дальность удара.
int DamageRadius;                      // Радиус поражения.
int Damage;                            // Поражение наносимой объектом.
int Armor;                             // Броня.
//END OF ATTACK


long int POWER;
void GETPOWER();


int X,Y,OldX,OldY;
float Z,OldZ,TempZ; //координаты для тактовых "точек"
float Angle,OldAngle,TempAngle;
float OldTempX,OldTempY,TempX,TempY;
void GetTempXYZAngle(float Frame);
void GetTempAngle(int Frame);
int Width;
bool CheckPoint(int x,int y);
//unsigned int Status:3; //до этого свойство пока руки не дошли - статус юнита (атакует,тратит жизни,ждет и.т.д.)
void GetNewPos(float Angle,float R);
void GetNewPos(int fX,int fY);      //два способа получения новой позиции - куда надо индти в лоб
void FreePosition();

void GetGlobalPos(int fX,int fY, int WhoCommand);   //получение глобальной позиции - цели до которой надо добраться любыми путями
void GetGlobalPos(float R,float Angle,int WhoCommand);

void SetGlobalAngle_R();
void Stop();
//void UnStop();

bool Is,OldIs;
bool CanMove;
bool Thinking;
struct{
      int X,Y,  //координаты новой позиции
          GlobalX, GlobalY; // координаты глобальной позиции - цели к которой придется идти возможно
                            // в обход...

      float  Angle,         // угол (он больше нужен для изображения юнита на экране)
                            // на который нужно повернетьсячтобы встать лицом к новой позиции

             GlobalAngle,   //А этот угол нужен в функции SetPos - угол на который надо повернуться,
                            //чтобы встать лицом к заоблачной цели
             GlobalR;       //расстояние до цели

      int SmallPoint, Steps;//данные в, принципе, временные - нужны для перемещения юнита

      bool OnX;             //аналогично вышенаписанному
      float DeltaX,DeltaY;  // для удобства что бы видеть сколько осталось до цели

      }NewPosition;


// МОЗГ
int NVO;  // Number of Visible Objects
int N;  //коэфиент успеха
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
//сортировка юнитов в круппе по идет по возрасатающей от слабого к сильному
public:
void Init(TGSociety *_Society);
bool Is;
TGSociety *Society;
TXY Distination;
int Team,Index; // команда и индекс группы в этой команде
bool OnMission;
void Refresh();
struct TIncInBase {bool Yes;int Index;};
       TIncInBase** IncludedInBase;


// --**О б щ а я информация о группе**---
//------------------------------------------
int Number; /* Численность                */
int Live;   /* Суммирующие жизни (МОЩь!)  */
long int POWER;
void GetPower();
int X,Y;
//------------------------------------------

// Информация о юнитах которые в ней состоят
//------------------------------------------

struct  TUnitStruct
        {
        TGObject *PUnit;  /* ссылки на юнита */
        int IndEnemy; /*это поле только "для своих" в
                                        нем занесены индекс ячейки массива Units вражеской группы,
                                        в которой хранится указатель на вражеского юнита
                                        */
        bool Is;
        };
        TUnitStruct* Units;
void GetUnit(TGObject *Unit);
void Sort();
//------------------------------------------


//          П Е Р Е М Е Щ Е Н И Е
//------------------------------------------
void GetNewPos(int X,int Y);
//------------------------------------------


//              В Р А Г И
//------------------------------------------

int NumberToKill; //количество врагов которое надо убить;
int NumberOfKilledEnemies; //количество убитых врагов;

void Free();//освобождение группы от задания;
void SetAttack(TATTACK_MODE PARAMETER);
TGGroup *EGroup;    //вражеская группа
TGObject *EUnit;
void GetEnemy(TGObject *Enemy);
void GetEnemy(TGGroup *Enemy);
void Analis ();  //выбираем "врага-партнера" каждому члену группы

//------------------------------------------
//К Л А С С   З А Д А Н И Й
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
  bool GetNewData(short int LOG_DATA, TDATA _DATA);  // получение новых данных
  bool MoveData(int from_i,int on_i);                // перенос данных из ячейки from_i в ячейку on_i
                                                     // вся информация в ячейке on_i при этом перезаписывается
  void FreeDataCell(int i);                          // функция очистки ячейки информации
  void DoMission();                                  // выполнение задания 
  bool CheckOnComplete();                            // проверка на завершение задания 
  void RefreshBase();                                // обновление, а точнее избавление от всех "пустот" в базе
  }Mission;



};








//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
class TGSociety
{
public:
int Dead;

// Карта местности и расположения объектов, все связанные с нею поля, потом методы работы с ними

TMap **MAP;/*[MAP_WIDTH][MAP_HEIGHT];*/
                                  // ячейка карты представляет собой стуктуру, в которой указан ее тип
                                  // если там стоит объект, то индекс той команды к которой он принад-
                                  // лежит.
                                  // Если этот участок непроходим то тип ячейки будет равен NUMBER_OF_TEAMS,
                                  // тоесть на один больше номера последней по индексу команды.
                                  // Участок считается занятым, если значяение типа больше нуля.
                                  // Если значение типа в этом месте равно -1, то ячейка карты свободна.
                                  // Если ячейка занята объектом то поле Index содержит индекс этого объекта в его
                                  // команде.
                                  // Также в ячейке указана ее высота отн. нулевого уровня.
                                  // Поле Selected ...


TForFound***
ANGLE_MATRIX;//[NUMBER_OF_TEAMS][NUMBER_OF_UNITS][NUMBER_OF_UNITS*NUMBER_OF_TEAMS];
                                  // Список углов и расстояний между юнитами
                                  // Для каждого юнита хранится список других юнитов, которых он
                                  // может увидеть. Количество видимых юнитов ограничено константой.
                                  // Юнит может видеть объекты не дальше поля своего зрения.
                                  // Первый индекс массива указывает на команду, к которой принадлежит
                                  // этот юнит, второй индекс - индекс юнита в команде, третий индекс
                                  // указывает на индекс объекта в списке видимых объектов для этого юнита
                                  // Ячейка массива представляет собой структуру, содержащую
                                  // номер команды, к которой принадлежит объект, индекс этого объекта в команде
                                  // Угол образованный отрезком, соединяющим данного и обнаруженного объекта, и
                                  // осью ОХ.



void InitMapPassably ();          // МЕТОД: в соответствии с ландшафтом
                                  // определяет непроходимые участки карты.

struct TKvadrats
{
int X,Y,X_,Y_;   // Координаты верхнего левого угла и нижнего правого соответсвенно.
int a,b;         // Ширина и длинна "квадрата".
TGObject* *Units;// Юниты, которые в этом квадрате находятся.
int Number;      // Количество юнитов в квадрате в данный момент.

}
Kvadrats[NUMBER_OF_KVADRATS][NUMBER_OF_KVADRATS];
                                  // Карта разделена на квадраты, это нужно для
                                  // более быстрого обнаружения близких объектов.


void Check_Put_UnitInKvadrat(int Team,int Index);
     void AddUnitInKvadrat(int OnOX,int OnOY, TGObject *Unit,bool ADD);
                                  // МЕТОДЫ: Проверить/положить объект в квадрат.
                                  //                  Добавить объект в квадрат.



void ANGLE_MATRIX_Init();
void ANGLE_MATRIX_GetAngles();      // расчитывает углы и расстояния между объектами
float SmalestR(int Team, int Index);

struct TTeams                       // КОМАНДЫ
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

void LoadObjectMap();               // МЕТОД: Считывает карту объектов.
void LoadMap();                     // МЕТОД: Считывает карту.
void LoadWeapons();                 // МЕТОД: Считывает оружее.
void LoadStandarts();               // МЕТОД: Считывает эталоны.

bool LoadConfig();                  // МЕТОД: Считывает все настройки:
                                    // карту, карту объектов, оружее, эталоны.


void RefreshObjectParametres(int Team,int Index);
void GSocietyInit(bool first /*временный*/);
bool PutUnit(int Team,int Index,int Type, int X,int Y); //кладет юнита на карту
void Dye(int Team,int Index);
void Sinchronize(int Team,int Index,bool SnAction); //меняет координаты юнита на карте путем снация и и накладываеия
void MoveUnit(int Team,int Index); // ...

TForFound CheckMove(float Angle/*так задается угол*/,int Team,int Index); // проверяет можноли идти под этим углом
bool Check(float Angle,int Team,int Index,int &Left_Zero_Right/*обход слева-прямо-справа*/,
           float R/*Растояние до последнего замеченного обекта*/);

/*
эта функция уже почти не нужна за нее всю работу делают функции связанные с
МАТРИЦЕЙ УГЛОВ(ANGLE_MATRIX).
*/
void SetPos(int Team, int Index/* TCanvas *Canvas*/);   // Алгоритм сложного перемещения
     //там по идеи должно идти вторичное сканировани
     /*
     вся суть функции такова:"если препятсвия непозволяют дойти до цели в лоб - то надо их обойти"
     */
bool CheckPoint (TGObject *Unit, float Angle,bool WithAngle,int R,int I);


//--------------------------------------------------------------
//    ---  С И С Т Е М Н Ы Е   Ф У Н К Ц И И ---
//--------------------------------------------------------------

//------------------
    bool ViewMode;     // режим просмотра параметров без объединения в группы и указания цели
	bool AttackMode;   // режим атаки
	bool ChaseMode;    // режим преследования
	bool MoveMode;     // режим передвижения


	void ChangeViewMode();    // включить выключить
	void ChangeAttackMode();  // включить выключить
	void ChangeAttackModeForSelected();
	void ChangeChaseMode();   // включить выключить
	void ChangeChaseModeForSelected();
	void ChangeMoveMode();    // включить выключить
	void ChangeMoveModeForSelected();

void SetSelectedUnits(int _X,int _Y,int X_,int Y_);

//------------------

//-------------------------------------------------------------
//    ---***  Искусственный интеллект и ведение боя  ***---
//-------------------------------------------------------------

//------------------
//"высшие инстанции"
//------------------

void DoAIAction(int Team);
 TMax GetEnemyCentre(int Team);   //анализируем карту ищем самые концентрированные части врагов
  TGGroup* MakeGroup(int Team,int Number);
  TGObject* MakeUnit(int Team,int Type, int X, int Y);
  void DestroyGroup(TGGroup *Group); //DEL GROUP
    void SendGroupsInAttack(int Team,int Number,int X,int Y,int Value);
    void SendSelectedUnits (int Team,short int LOG_DATA,TGGroup::TDATA DATA);


//------------------
//   бой в группе
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
//   атака юнитом
//------------------

/* bool DoActoin()      */
      int Analis_Change(int Team,int Index,int index); //2. Юнит анализируют параметры всех врагов по очереди (здоровье, броню,атаку),
                     //   выбирает наиболее слабого из них.
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
