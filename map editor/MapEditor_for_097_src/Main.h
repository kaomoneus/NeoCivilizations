//---------------------------------------------------------------------------
#include "MainUnit.cpp" // ==> #include "EditConstants.h"
#include "MapEditConst.h"

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
char* GetWord(char firstsym,char secsymb,char* string,char* instring)        // находит слово
                                                                             // ограниченное между
                                                                             // двумя символами
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
char* GetValue(char* string,char* instring)                                  // находит в строке значение
      {
      char buf = '\0';
      int si = 0;
      //1. Вначале шагаем до символа "=".
      while ((si<STRING_LENGTH)&&(string[si-1]!='='))
            si++;

      //2. Потом пропускаем все символы "_".
      while ((si<STRING_LENGTH)&&(string[si-1]!=' '))
            si++;


      //3. Считываем все до символа ";".
      int i;
      for(i = 0;(buf!=';')&&(i<WORD_LENGTH);i++)
         {
         buf = string[si+i];

         // Загружаем в возвращаемой значение только цифры
         if (buf!=';')
             instring[i] = buf;
             else
             instring[i] = '\0';
         }


      return instring;
      }

bool eqwords(const char* word1,const char* word2)                            // сравнивает слова
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
                                                                             // копирует одно слово в другое
                                                                             // с выделением памяти
//---------------------------------------------------------------------------

const char* SYNTAX_ERROR = "Синтаксическая ошибка.";

class TMain
{
public:
// поля

   // Внешнесистемные поля
TListBox* MEList;                        // Список с чем-либо
TCanvas*  Canvas;                        // Место для отрисовки
TForm*    MiniMap;                       // Минни-карта

   // Вспомогательные поля
int CurX,CurY;                           // Положение экрана (с учетом особенностей
                                         // системы координат кмопбютера
                                         // - это верхний левый угол экрана)
short int StandartsNum;
short int WeaponsNum;

   // Внутресистемные поля
TGSociety Society;                       // Весь внутренний каркас игры.
TGStandart* Standart;                    // Типы объектов.
TWeapon* Weapon;                         // Оружее (орудия).

   // КОМАНДЫ объвляю только устно, по той причине, что они существуют в объекте
   // Society.

   // Поля предназначенные для редактирования
   TChangeType CurList;                     // Текущий вид списка.
   int CurTeam;                             // Редактируемая команда.
   int CurStandart;                         // Текущий выбранный тип.

   enum TEditMode {EDIT_LAND_SCAPE, EDIT_PLACE ,EDIT_OBJECT_MAP};
   TEditMode EditMode;
   bool MouseDown;
   int LastMapX,LastMapY,
       MapX,MapY;

   //                   редактирование ландшафта.
                        enum TLSBrush {PICK_BRUSH,SOFT_BRUSH,HARD_BRUSH};
                        TLSBrush LSBrush; 
                        int BrushRadius;
                        int BrushDelta; 


// Взаимподейстиви (подпрограммы)
// Конструкторы
TMain();
void Init(TListBox* _MEList,TCanvas* _Canvas);
void MiniMapAssign (TForm* _MiniMap);
// Методы.

   // Инициализация оружия и объектов.
bool LoadIniFile(char* Path);
     bool ApplyValue(char* Name, char* Value,int Type_of_type,int index);
                                             // Загрузка нзачения в поле,
                                             // соответствующее названию, и типу файла

   // Инициализация/обновление списка.
   void DrawMap(int SX, int SY, int EX, int EY);
        void DrawMiniMap();
        void DrawLandScape(int SX, int SY, int EX, int EY);    // Рисуем ландшафт и мини-карту.
        void DrawObjects();      // Рисуем объекты.

        void DrawLandScpNet(int SX,int SY,int EX,int EY, bool Delete);   // Рисуем сеть для ландшафта.
             int k;
             void DrawH();       // Выделяем точку на которую навведена мышь
                                 // Функция пользуется MapX и MapY.


   // Редактирование ландшафта.
   void OnMouseDown();
   bool OnMouseMove(int MAP_X,int MAP_Y);    // В метод передаются координаты
                                             // ячейки карты.

   void OnMouseUp();                         // Когда мы отпускаем кнопку мыши,
                                             // все значения сбрасываются.


   void EditLandScp(int _x, int _y, float delta,int radius);


   // Редактирование карты объектов
   void SetCurStandart();


void RefreshMEList();
   // Системные команды и методы
   void SaveMap(char* Path);     // сохранение карты.
   bool LoadMap(char* Path);     // загрузка карты.
};





#endif
