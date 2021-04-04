

//---------------------------------------------------------------------------
#include "StdAfx.h"


/*
Это тестовый вариант c матрицей данных о видимых объектах

...
неплохо вроде работает...



то отчего надо избавиться для улучшения быстродействия системы:
1. Функция sqrt( (double) );
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
 // Инициализация секции, секция закроется.
bool TSection::Init(TGSociety *_Society, TGObject* _OurObject,bool Oldest)
     {
     //1. Все что связано с поколением
     Generation                 = 0;
     Open                       = false;
     MaxExistingGeneration      = 0;
     MaxGeneration              = log10( (double )min(MAP_WIDTH,MAP_HEIGHT))/log10( 2. );
     ScionsNumber               = 0;
     OldestSection              = (Oldest)?this:NULL;
     for (int x = 0; x<2; x++)
     for (int y = 0; y<2; y++)
         Daughter[x][y]         = NULL;

     // 2. Координаты вершин и центра.
     SX  = 0;
     EX  = (Oldest)?MAP_WIDTH-1:0;
     SY  = 0;
     EY  = (Oldest)?MAP_HEIGHT-1:0;
     X   = (Oldest)?round(float(EX-SX)/2):0;
     Y   = (Oldest)?round(float(EY-SY)/2):0;

     // 3. "Внешний мир".
     Society = _Society;
     OurObject = _OurObject;


     // 4. Все что связано с заливкой
     Creeked                    = false;
     FromSection                = NULL;
     Steps                      = (Oldest)?0:-1;
     SectionWithA               = NULL;
     SectionWithB               = NULL;
     SectionWithA_B             = NULL;
     SectionWithObject          = (Oldest)?this:NULL;
     Occupied = (Oldest)?!CheckMap(true,-1,-1):false;
     // 2.1. Пункты А и В
     A = NULL;
     B = NULL;

     return true;

     }

bool TSection::FingerAllSections(bool EInit, bool CheckOnAB,bool FindObject/*,TCanvas* Canvas*/)
     // см. "Н" файл.
     {
     // 1. Создаем список инициализируемых секций IList.
     TGList IList;
     // 1.1. Максимально возможное число секций 2^((MaxGeneration+1)*2)
     IList.TotalNum = pow((double)(2),(double)((OldestSection->MaxExistingGeneration+1))*2);
     IList.Section  = new TSection* [IList.TotalNum];
     IList.Num       = 0;
     // 2. Добавляем в список IList эту секцию.
     AddSection(&IList,this);
     // 3. Организовывем FOR идущий до конца списка.
     for (int i = 0; i<IList.Num; i++)
     //    начало
         {
     // 3.1. Если секция содержит и пункт А и пункт В
     //            и если она младше соответствующей секции занесенной в самую старую секцию
     //            или если в самую старую секцию вообще ничего не занесено
                   if (
                       (IList.Section[i]->B!=NULL)&&(IList.Section[i]->A!=NULL)&&
                        (
                        (OldestSection->SectionWithA_B==NULL)||
                        (OldestSection->SectionWithA_B->Generation<IList.Section[i]->Generation)
                       )
                      )
     //             занести в соответствующее поле самой старой секции эту секцию
                      OldestSection->SectionWithA_B = IList.Section[i];
     // 3.2. Если секция открыта удалить ее и добавить в конец ее потомков.
         if (IList.Section[i]->Open)
            {
            TSection* CurS = IList.Section[i];
            DelFromList(&IList,i);

            for (int x = 0; x<2; x++)
            for (int y = 0; y<2; y++)
                {
     // 3.2.0 Если требуется, выполнить инициализацию текущей секции (без закрытия).
                if (EInit)
                CurS->Daughter[x][y]->EasyInit();
                CurS->Daughter[x][y]->Occupied = !CurS->Daughter[x][y]->CheckMap(true,-1,-1);


     // 3.2.1. В каждую секцию вписываем наследственную информацию
     // 3.2.1.1.0. Если нужно найти закрытую секцию с объектом.
                if (FindObject)
                   {
     // 3.2.1.1.0.1. Если секция закрыта и координаты объекта лежат внутри секции
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
     // 3.2.1.1.1.1. Говорим этой дочерней секции, что она содержит объект.
                      CurS->Daughter[x][y]->SectionWithObject = CurS->Daughter[x][y];
     // 3.2.1.1.1.1. Передаем ссылку самой старой секции на эту дочернюю секцию.
                      OldestSection->SectionWithObject = CurS->Daughter[x][y];
                      }

                   }

     // 3.2.1.3.0. Если указано поле CHECK ON AB
                if (CheckOnAB)
                   {
     // 3.2.1.3. Если в самой старой секции проинициализирован пункт А или В,
     // 3.2.1.3.1. Для пункта А.
     //            найти дочернюю секцию в котором находятся те или иные координаты,
                 if (
                    (OldestSection->A!=NULL)
                    &&
                    InXYSpace(CurS->Daughter[x][y]->SX,CurS->Daughter[x][y]->EX,
                              CurS->Daughter[x][y]->SY,CurS->Daughter[x][y]->EY,
                              OldestSection->A->X,OldestSection->A->Y,true,true,true,true
                              )
                    )
                    {
      //          и передать информацию.
                    CurS->Daughter[x][y]->A = OldestSection->A;
                       OldestSection->SectionWithA = CurS->Daughter[x][y];
                    }
     // 3.2.1.3.2. Для пункта В.
     //          найти дочернюю секцию в которой находятся те или иные координаты
                 if (
                    (OldestSection->B!=NULL)
                    &&
                    InXYSpace(CurS->Daughter[x][y]->SX,CurS->Daughter[x][y]->EX,
                              CurS->Daughter[x][y]->SY,CurS->Daughter[x][y]->EY,
                              OldestSection->B->X,OldestSection->B->Y,true,true,true,true
                              )
                    )
     //          и передать информацию.
                    {
                    CurS->Daughter[x][y]->B = OldestSection->B;
                       OldestSection->SectionWithB = CurS->Daughter[x][y];
                    }
                   }

     //          Если список переполнен преостановить выполнение метода - резултат отрицательный
                if (!AddSection(&IList,CurS->Daughter[x][y]))
                   return false;
                }
            // 3.3. Так как FOR сдвигает каретку вправо а нам надо проинициализировать то,
            //      что оказалось на месте удаленной из списка секции
            //      мы вротивовес FORy сдвигаем каретку влево.
            i--;
            }
//     "Индикация"
//            else
//            {
//            Canvas->Brush->Color = (IList.Section[i]->Occupied)?((IList.Section[i]->Creeked)?clRed:clWhite):
//                                   (IList.Section[i]->Creeked)?clBlue:(clBlue-128);
//            Canvas->RoundRect(IList.Section[i]->SX,IList.Section[i]->SY,IList.Section[i]->EX,IList.Section[i]->EY,2,2);
//            }
//     Конец "индикации"
         }
     // 4. Числом потомков для этой секции будет количество элементов в списке.
     OldestSection->ScionsNumber = IList.Num;
     // 5. Очищаем память выделенную под список.
     delete[] IList.Section;
     // 6. Возвращаем положительный результат.
     return true;
     }

bool TSection::EasyInit()       // Инициализация полей, отвечающих за заливку.
     {
     if (this == NULL)          // Проверка
        return false;
     Creeked        = false; // Залита ли секция.
     FromSection    = NULL;  // Из какой секции залита эта секция.
     SectionWithA_B = NULL;  // Самая младшая секция, которая содержит пункт А и В одновременно.
     Steps          = 0;     // Расстояние, которое нужно пройти от центра заливки до этой секции.
     if (this!=OldestSection)
        {
        B = NULL;
        A = NULL;
        }

     return true;
     }

bool TSection::DelGeneration(int G)
     {
     // Удаляет все поколение G, всех его потомков,
     // очищает память выделенную под удаленные секции.
     // Поколение G должно быть младше поколения этой секции.
     if (G>OldestSection->MaxExistingGeneration)
        return false;
     if (G<=Generation)
        return false;
     // 1. Занесение в список ВСЕХ секций, являющихся потомками данной секции.
     // 1.1. Организация списка.
     TGList List;
     // 1.1.1. Число элементов списка приблизительно можно определить по формуле
     //                       MaxExistGeneration
     //        определенный интеграл S  (2^[(g - Generation)*2])dg) =
     //                          Generation
     //
     //        = [(2^((MaxExistGeneration-Generation)*2)) - 2]/ln(2)
     //        Есть способ интегрирования только для целых чисел, но он
     //        не подготовлен.

     List.TotalNum = ceil((pow((double)(2),(double)((OldestSection->MaxExistingGeneration - Generation))*2) - 2)/log((double)2));
     List.Num      = 0;
     List.Section  = new TSection*[List.TotalNum];
     // 1.2. Добавление в список этой секции.
     AddSection(&List,this);
     // 1.3. Организация цикла идущего до самого последнего элемента списка(влючительно)
     int i;
     for (i = 0; i<List.Num;i++)
     // 1.3.1. Если текущая секция (далее секция В) открыта,
     //        добавить в список всех ее потомков.
         if (List.Section[i]->Open)
            for (int x = 0;x<2;x++)
            for (int y = 0;y<2;y++)
                AddSection(&List,List.Section[i]->Daughter[x][y]);

     // 2. Уничтожение всех элементов на которые есть ссылки в списке, и поколение которых
     //    младше и такое же как поколение G.
     // 2.1. Организация цикла перебирающего все элементы цикла,

     for (i = 0; i<List.Num; i++)
     // 2.1.1. Если поколение текущей секции младше или такое же как G
         {
         if (List.Section[i]->Generation>=G)
     // 2.1.1.1. Очистить память выделенную под эту секцию.
            {
            delete List.Section[i];
            List.Section[i] = NULL;
            }
     // 2.1.2. Если поколение текущей секции G-1 (тоесть она на 1 старше заданного поколения).
     // 2.1.2.1. Если секция открыта, закрыть секцию.
            else if ( (List.Section[i]->Generation == (G-1)) && List.Section[i]->Open )
                    {
                    for (int x = 0;x<2;x++)
                    for (int y = 0;y<2;y++)
                        List.Section[i]->Daughter[x][y] = NULL;
                    List.Section[i]->Open = false;
     // 2.1.2.1.1. Если в закрываемой секции есть указатель на А, она становится закрытой секцией,
                   if (List.Section[i]->A != NULL)
                      {
     //            содеражащей пункт А.
     // 2.1.2.1.1.1. ОТКЛЮЧЕНО: Присваиваем секции соответствующее звание.
     //                 List.Section[i]->SectionWithA = List.Section[i];
     // 2.1.2.1.1.2. Изменяем значение поля "Секция с пунктом А" у самой старой секции.
                      OldestSection->SectionWithA = List.Section[i]->SectionWithA;
                      }
     // 2.1.2.1.2. Аналогично с пунктом В.
                   if (List.Section[i]->B != NULL)
     //               {
     ///*ОТКЛЮЧЕНО:*/ List.Section[i]->SectionWithB = List.Section[i];
                      OldestSection->SectionWithB = List.Section[i]->SectionWithB;
     //               }
                    }
     // 2.2. Аннулировать информацию о заливке (bool EasyInit).
     //       Если секция была удалена EasyInit просто вернет ложь. 
         List.Section[i]->EasyInit();
         }
     // 3. Перебрать все секции OldestSection->FingerAllSections (без иничиализации и без вложения пунктов А и В)
     OldestSection->FingerAllSections(false,false,false);
     // 4. выход.
     // 4.1. Очищение памяти, выделенной под список.
     delete[] List.Section;
     // 4.2. Возвращаем результат.
     return true;
     }


// Разделеить все заполненые но не открытые секции предки на
// дочерние. Если они минимальных размеров то возвращается
// false если разделил то true
bool TSection::SplitSection()
     {
     // 1. Проверка на надобность сей операции.
     //    ПРОВЕРКИ НЕ ПРИДУМАНЫ.

     // 2. Организуем список.
     TGList GList;
     // 2.1. Максимальное число элементов в списке выщитывается по формуле:
     //      2^((MaxExistingGeneration+1)*2)
     GList.TotalNum = pow((double)(2),(double)((OldestSection->MaxExistingGeneration+1))*2);
     GList.Section  = new TSection* [GList.TotalNum];
     GList.Num = 0;
     // 3. В список помещаем эту секцию.
     AddSection(&GList,this);
     // 4. Организуем FOR идущий от начала до конца списка.
     for (int i = 0; i<GList.Num; i++)
     //    начало
         {
     // 4.1. Если секция открыта, то удаляем ее из списка помещаем в конец списка ее дочерей.
         if (GList.Section[i]->Open)
            {
            TSection* CurS = GList.Section[i];
            DelFromList(&GList,i);
            for (int x = 0; x<2; x++)
            for (int y = 0; y<2; y++)
                AddSection(&GList,CurS->Daughter[x][y]);
     // 4.1.1. Каретку сдвигаем влево - а FOR потом сдвинет вправо.
            i--;
     // 4.1.2. Переходим "дальше".
            continue;

            }
     // 4.2. Если секция закрыта и внутри секции есть препятствия и она не минимальна, то
         if ((!GList.Section[i]->Open) && (GList.Section[i]->Occupied) &&
                                          (GList.Section[i]->Generation<OldestSection->MaxGeneration))
     //      начало
            {
     // 4.2.1. Делаем секцию открытой.
            GList.Section[i]->Open = true;
     // 4.2.1.1. Изза того что секция открылась она больше не является закрытой - это логично,
     //          поэтому из числа самых младших потомков мы ее  удаляем
            OldestSection->ScionsNumber--;
     //          но у нее появилось четыре неоткрытых дочки, поэтому их надо прибавить к этому
     //          числу
            OldestSection->ScionsNumber+=4;
     // 4.2.1.2. Если открываемая секция была одной из самых младших,
     //          то говорим у появлении еще более младшего поколения.
            if (OldestSection->MaxExistingGeneration==GList.Section[i]->Generation)
            OldestSection->MaxExistingGeneration++;
     // 4.2.2. Перебираем всех ее дочерей.
            for (int x = 0; x<2; x++)
            for (int y = 0; y<2; y++)
                {
     // 4.2.2.1. Выделяем память под дочернюю секцию.
                GList.Section[i]->Daughter[x][y] = new TSection;
     // 4.2.2.0. Пусть текущая секция называется DS (DaughterSection)
                TSection* DS = GList.Section[i]->Daughter[x][y];

     // 4.2.2.2. Инициализируем дочернюю секцию.
                 DS->Init(OldestSection->Society,OldestSection->OurObject,false);
     // 4.2.2.3. Поколение дочерних секций на 1 больше материнкой секции CurS
                 DS->Generation = GList.Section[i]->Generation+1;

     // 4.2.2.7. Передаем "по наследству" информацию о самой старой секции.
                 DS->OldestSection = GList.Section[i]->OldestSection;
     // 4.2.2.8. Пункты А и В обновляются той же ф-ей, что и самое младшее поколение.
     // 4.2.2.9. -//-
     // 4.2.2.10. Выщитываем координаты центра и координаты выршин текущей дочерней секции
                 DS->SX = (x==0)?GList.Section[i]->SX:GList.Section[i]->X;
                 DS->EX = (x==0)?GList.Section[i]->X:GList.Section[i]->EX;
                 DS->SY = (y==0)?GList.Section[i]->SY:GList.Section[i]->Y;
                 DS->EY = (y==0)?GList.Section[i]->Y:GList.Section[i]->EY;
                 DS->X  = round(float(DS->EX-DS->SX)/2)+DS->SX;
                 DS->Y  = round(float(DS->EY-DS->SY)/2)+DS->SY;
                }
     //      конец
            }
     //    конец
         }
     // 5. Очищаем пямять, выделенную под список.
     delete[] GList.Section;
     // 6. Возвращаем положительный результат.
     return true;
     }
bool TSection::GetA(TXY _A/*,TCanvas* Canvas*/)
     {
     OldestSection->EasyInit();
     // 0. Если значяение точка Х,У не пинадлежит карте, то  используем Х и У объекта
     if (!InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,_A.X,_A.Y,true,false,true,false))
        {
        _A.X = OurObject->X;
        _A.Y = OurObject->Y;
        }
     // 1. Выделяем память под пункт А
     OldestSection->A = new TXY;
     // 2. Присваиваем выделенному участку значение А
     *OldestSection->A = _A;
     // 3. Самая старая секция являемтся секцией с пунктом А.
     OldestSection->SectionWithA = OldestSection;

// ОТКЛ.:
     // 4. Выявляем закрытую секцию содержащую пункт А.
     //    Ссылка на эту секцию будет передана самой старше секции
     // 5. Назначаем самой младше секцией с пунктами А и В, самую старшую
     // 5.1. Функция Finger исправит значение этого поля, если это не так.
//    OldestSection->FingerAllSections(true/*с инициализацией*/,
//                                     true,/*Поверка на наличие А и В у закрытых секций*/
     // 6. Паралельно ищем самую младшую закрытую секцию, в которой находится объект
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
     // 1. Выделяем память под пункт B
     OldestSection->B = new TXY;
     // 2. Присваиваем выделенному участку значение B
     *OldestSection->B = _B;
     // 3. Самая старая секция являемтся секцией с пунктом B.
     OldestSection->SectionWithB   = OldestSection;
     // 4. А еще она содержит координаты пунктов А и В одновременно.
     OldestSection->SectionWithA_B = OldestSection;

// ОТКЛ.:
     // 4. Выявляем закрытую секцию содержащую пункт B.
     //    Ссылка на эту секцию будет передана самой старше секции
//     OldestSection->FingerAllSections(false/*без инициализации*/,
//                                      true,/*Поверка на наличие А и В у закрытых секций*/
     // 5. При получении координат пункта А, была найдена закрытая секция с объектом
     //    поэтому искать второй раз - излишество .
//                                      false
//                                      /*,Canvas*/);
     return true;
     }


bool TSection::Creek(/*TCanvas* Canvas*/)
     {
     bool Result = false;
     // Алгоритм заливки неравноправных секций
     // 1. Создаем два "противоположных" списка.
     // 1.1. Выделяем память под массив всех секций предков для данной секции.
     // 1.2. Инициализируем списки.
     // 1.3. Добавляем в первый из них закрытую секцию с пунктом "В" (источик заливки)

     // 2. По принципу маятника организуем "колеблющийся" цикл между двумя противоположными списками,
     //    который будет "колебаться" до тех пор пока, в текущем списке не иссякнут секции (елементы списка).
     //    Дело в том, что если есть что заливать, то в ничале в обоих списках число секций будет увеличиваться,
     //    а когда  все будет залито, телу цикла просто нечем будет заполнить противоположный список

     //    Начало цикла
     // 2.1. Перебираем все секции списка.
     // 2.1.1. Если текущая секция закрыт и в ней находится пункт "А" и она не занята, то результат будет положительный (истина)
     // 2.1.2. Заливаем их.
     //        СЕКЦИИ В СПИСКАХ ДОЛЖНЫ БЫТЬ ЗАКРЫТЫМИ!

     // 2.2. Находим все нужные( удовелетворяющие некоторым условия рациональной работы алгоритма)
     //      соседние секции
     // 2.3. Добавляем их в противоположный список.
     // 2.4. Текущий список анулируем.
     //    Конец цикла

     // 3. Выходим.
     // 3.1. Очищаем память.
     // 3.2. Возвращаем результат.



     // 1. Создаем два "противоположных" списка.
     TGList List[2];

     // 1.1. Выделяем память под массив всех секций предков для данной секции.
     List[0].Section  = new TSection* [OldestSection->ScionsNumber];
     List[1].Section  = new TSection* [OldestSection->ScionsNumber];
     // 1.2. Инициализируем списки.
     List[0].Num = 0;
     List[0].TotalNum = OldestSection->ScionsNumber;
     List[1].Num = 0;
     List[1].TotalNum = OldestSection->ScionsNumber;
     // 1.3. Добавляем в первый из них закрытую секцию с пунктом "В" (источик заливки)
     // 1.3.1. Делаем это только в том случае, если секцию возможно залить - иначе возвращаем ЛОЖЬ
     if (OldestSection->SectionWithB->CheckAndFillSection(OldestSection->SectionWithB))
        AddSection(&List[0],OldestSection->SectionWithB);
        else
        return false;
     // 2. По принципу маятника организуем "колеблющийся" цикл между двумя противоположными списками,
     //    который будет "колебаться" до тех пор пока, в текущем списке не иссякнут секции (елементы списка).
     //    Дело в том, что если есть что заливать, то в ничале в обоих списках число секций будет увеличиваться,
     //    а когда  все будет залито, телу цикла просто нечем будет заполнить противоположный список
     for (int t = 0; List[t].Num > 0; t = abs(t-1))
     //    Начало цикла
         {
     // 2.1. Перебираем все секции текущего списка.
        for (int i = 0; i<List[t].Num; i++)
             {
             // 2.1.1. Если текущая секция закрыта и в ней находится пункт "А" и она не занята, то результат будет положитель-
             //        ный (истина), и можно выходить из функии.
             if (
                 (!List[t].Section[i]->Open)
                 &&
                 (!List[t].Section[i]->Occupied)
                 &&
                 (List[t].Section[i] == OldestSection->SectionWithA)
//                  (List[t].Section[i]->X == SectionWithA->X) // совпадение координат говорит о том, что
//                 &&                                         // то на ЧТО указывают эти два указателя -
//                  (List[t].Section[i]->Y == SectionWithA->Y) // это одна секция
                )
                return true;
             // 2.1.2. Заливаем их функцией CheckAndFill, которая будет вызвана функцией GetDaughter...,
             //        которая в свою очередь будет вызвана GetAllNeighbour.
             //        ВСЕ СЕКЦИИ В СПИСКАХ ДОЛЖНЫ БЫТЬ ЗАКРЫТЫМИ!

//             Canvas->Pixels[List[t].Section[i]->X][List[t].Section[i]->Y] = clWhite;
             // 2.2. Находим все нужные( удовелетворяющие некоторым условия рациональной работы алгоритма)
             //      соседние секции
             TGList _List = List[t].Section[i]->GetAllNeighbour(/*Canvas*/);

             // 2.3. Добавляем их в противоположный список.
             AddList(&List[abs(t-1)],&_List);
             // 2.4. ВЫЖНО! Очищаем динамически выделенную функцией GetAllNeighbour память под эелементы
             //      списка _List.
             delete[] _List.Section;
             }
     // 2.4. Текущий список анулируем.
         List[t].Num = 0;
     //    Конец цикла
         }
     // 3. Выходим.
     // 3.1. Очищаем память.
     delete[] List[0].Section;
     delete[] List[1].Section;
     // 3.2. Возвращаем результат.
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
     // засекаем время работы
     OldestSection->Occupied = !OldestSection->CheckMap(true,-1,-1);
     // Ищем путь, доходя до нужного младшего поколения.
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
     // Организуем список по старой схеме заносим в него все закрытые секции
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
            else // Если секция закрыта рисуем ее
                 {
                 // 1. Если секция занята (в ней есть препятствие)
                 // отрисовываем ее красного цвета.
                 if (List.Section[i]->Occupied)
                    {
                    Canvas->Brush->Color = clRed;
                    Canvas->RoundRect(List.Section[i]->SX,List.Section[i]->SY,
                                      List.Section[i]->EX,List.Section[i]->EY,2,2);
                    }
                    else {
                 // 2. Если секция залита, отрисовываем ее синего цвета.
                 // 2.1. Соединяем залитую секцию и ту которая ее залила линией (белой)
                         if (List.Section[i]->Creeked)
                            {
                            Canvas->Brush->Color = clBlue+round(float(List.Section[i]->Steps)/2);
                            Canvas->RoundRect(List.Section[i]->SX,List.Section[i]->SY,
                                              List.Section[i]->EX,List.Section[i]->EY,2,2);
                            }
                 // 3. Если секция не залита, отрисовываем ее черного цвета - не рисуем.
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
     // Определяем секцию содержащую объект.
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
     // Следующей секцией содеражащей объект, будет FromSection
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
        // Метод возвращает список всех "отобранных" соседей для
        // этой секции
        // Также внутри метода заполняются поля этих соседей
        // Step и FromSection

        // 1. Заводим необходимые локальные переменные.
        // 1.1. Создаем список секций SList.
        // 1.2. Наша секция далее будет называться ДАННОЙ СЕКЦИЕЙ
        // 1.3. Добавляем в начало списка самую старую секцию.

        // 2. Непосредственный поиск соседних секций, методом "рекурсивного" раскрытия самой старой секции

        // 2.1. Организовываем цикл for, идущий от i = i до конца списка SList, в котором
        //      i - индекс текущего элемента списка SList и в какждой итэрации он увеличивается на 1.
        //      обращаю внимание на то, что переменная i - объявлена в самом начале

        //      Начало цикла FOR

        // 2.1.1. Если текущая секция (далее CurS) открыта.
        //        начало
        // 2.1.1.1. Удаляем CurS из списка (перед этим сохранив ссылку на нее в лог. переменной).
        // 2.1.1.2. Добавляем в конец списка SList сососедних дочерей секции CurS (это должны быть открытые
        //          секции или закрытые, которые можно залить) для секции SA.
        // 2.1.1.3. Для того, что бы в следующей итерации мы рассмотрели элемент, попавший на место
        //          CurS, надо переменную i уменьшить на 1.
        //        конец.
        //      Конец цикла FOR
        //      Таким образом к концу цикла все элементы списка S будут закрыты.


        // 3. Выходим.
        // 3.1. Возвращаем список SList.
        // Метод возвращает список всех "отобранных" соседей для
        // этой секции
        // Также внутри метода заполняются поля этих соседей
        // Step и FromSection



        // 1. Заводим необходимые локальные переменные.

        // 1.1. Создаем список секций SList.
        TGList SList;
        // 1.1.1. Инициализация списка SList.
        // 1.1.1.1. Максимальное число соседних секций определяется по формуле:
        //          TotalNum = 4*2^(MaxExistingGeneration - Generation) + 4;
        //          MaxExistingGeneration >= Generation
        SList.TotalNum   = 4*pow((double)(2),(double)(OldestSection->MaxExistingGeneration - Generation)) + 4;
        SList.Section    = new TSection* [SList.TotalNum];
        SList.Num        = 0;

        // 1.3. Добавляем секцию, которая содержит пункты А и В
        //      при условии, что если она закрыта,то не занята.
        if (CheckAndFillSection(OldestSection->SectionWithA_B))
           AddSection(&SList,OldestSection->SectionWithA_B);
        // 2. Непосредственный поиск соседних секций, методом "рекурсивного" раскрытия самой старой секции

        // 2.1. Организовываем цикл for, идущий от i = i до конца списка SList, в котором
        //      i - индекс текущего элемента списка SList и в какждой итэрации он увеличивается на 1.
        //      обращаю внимание что переменная i - объявлена в самом начале
        for (int i = 0;i<SList.Num;i++)
            {
        // 2.1.1. Если текущая секция (далее CurS) открыта.
        //        начало
            if (SList.Section[i]->Open)
               {
               TSection *CurS = SList.Section[i];
        // 2.1.1.1. Удаляем CurS из списка (перед этим сохранив ссылку на нее в лог. переменной).
               DelFromList(&SList,i);
        // 2.1.1.2. Добавляем в конец списка SList сососедних дочерей секции CurS (это должны быть открытые
        //          секции или закрытые, которые можно залить) для секции SA(для этой секции).
               TGList SubList = GetDaughterNeighbour(CurS);

               AddList(&SList,&SubList);
        // 2.1.1.4. ВЫЖНО! Очищаем память, которую выделила функция GetDaughterNeighbour под элементы списка
        //          SubList.
               delete[] SubList.Section;
        // 2.1.1.3. Для того, что бы в следующей итерации мы рассмотрели элемент, попавший на место
        //          CurS, надо переменную i уменьшить на 1.
        //        конец.
               i--;
               }
            }
        //      Таким образом к концу цикла все элементы списка S будут закрыты.
//               Canvas->Brush->Color = clGreen;
//              for (int k = 0;k<SList.Num;k++)
//                   Canvas->RoundRect(SList.Section[k]->SX,SList.Section[k]->SY,SList.Section[k]->EX,SList.Section[k]->EY,1,1);

        // 3. Выходим.
        // 3.1. Возвращаем список SList.
        return SList;
        // ПРИМЕЧАНИЕ: В данном случае память выделенная вначале под массив не
        // очищается, потомучто функия передает "в внешний мир" только указатель на
        // первую ячейку массива, это значит, что данные, под которые была выделена
        // память еще будут непосредственно использоваться вне функии и поэтому их нельзя
        // удалять - нельзя очищать память.

        }

TSection::TGList TSection::GetDaughterNeighbour(TSection *_Section)
        {
        // Метод возвращает список соседних дочерей секции Section
        // для этой секции.
        // ПРИМЕЧАНИЕ: соседними считать не только прлежащие секции
        // к этой, но и ту, которая будет являться предком этой секции,
        // тоесть все крайние точки этой секции будут принадлежать
        // квадрату секции предка.


        // Адгоритм свдится к поску хотябы одной общей границы или вершины у текущей
        // дочерней секции секции _Section и этой секции.

        // 1. Заводим локальные переменные.
        // 1.1. SX, EX, SY, EY - левая абсциса, правая абсциса, левая
        //      ордината, правая ордината (соответственно) этой секции.
        //      (эти переменные заводить не надо).

        // 1.2. Заводим список BList.
        TGList BList;
        // 1.2.1. Инициализируем BList.
        // 1.2.1.1. Максимальное число соседних секций определяется по формуле:
        //          TotalNum = 4*(2^(_Section->Generation - Generation)) + 4;
        BList.TotalNum = 4*/*ceil*/(pow((double)(2),(double)(_Section->Generation - Generation))) + 4;
        BList.Section  = new TSection* [BList.TotalNum];
        BList.Num      = 0;
        // 2. Заводим группу циклов перебирающих дочерние секции секции _Section.
        for (int x = 0; x<2;x++)
        for (int y = 0; y<2;y++)
            {
        //    начало циклов
        // 2.1. BSX, BEX, BSY, BEY - левая абсциса, правая абсциса, левая
        //      ордината, правая ордината (соответственно) текущей дочерней секции.
            int BSX = _Section->Daughter[x][y]->SX;
            int BEX = _Section->Daughter[x][y]->EX;
            int BSY = _Section->Daughter[x][y]->SY;
            int BEY = _Section->Daughter[x][y]->EY;
        // 2.2. Если эта секция старше или ровесница текущей
            if (Generation <= _Section->Daughter[x][y]->Generation)
        //      начало
               {
        // 2.2.0.1 Если текущая секция является этой секцией (это может быть)
        //         то перейти к следущей секции (continue).
               if (this == _Section->Daughter[x][y])
                  continue;
        // 2.2.1. Если BSX или BEX или BSY или BEY впадат в промежуток [SX;EX;SY;EY]
        //        Проще говоря, если хотябы одна из вершин попадает в границы нашей секции
                  if (
                      InXYSpace(SX,EX,SY,EY,BSX,BSY,true,true,true,true)
                      ||
                      InXYSpace(SX,EX,SY,EY,BEX,BSY,true,true,true,true)
                      ||
                      InXYSpace(SX,EX,SY,EY,BSX,BEY,true,true,true,true)
                      ||
                      InXYSpace(SX,EX,SY,EY,BEX,BEY,true,true,true,true)
                      )
        //        то добавить текущую секцию в список граничащих секций BList.
                     {
        // 2.2.1.1. Проверка текущей секции - имеет ли смысл ее включать.
        //          если имеет, то заполняем все необходимые поля
                     if(CheckAndFillSection(_Section->Daughter[x][y]))
                       AddSection(&BList,_Section->Daughter[x][y]);
        //        и перейти дальше, к следующей секции
                     continue;
                     }
        //      конец
               }
        // 2.3. иначе (Если эта секция малдше)
                else
        //        начало
                  {
        // 2.3.1. Если SX или EX впадает в промежуток [BSX;BEX]
                  if (
                      InXYSpace(BSX,BEX,BSY,BEY,SX,SY,true,true,true,true)
                      ||
                      InXYSpace(BSX,BEX,BSY,BEY,EX,SY,true,true,true,true)
                      ||
                      InXYSpace(BSX,BEX,BSY,BEY,SX,EY,true,true,true,true)
                      ||
                      InXYSpace(BSX,BEX,BSY,BEY,EX,EY,true,true,true,true)
                      )

        //        то добавить текущую секцию в список граничащих секций BList.
                     {
        // 2.3.1.1. Проверка текущей секции - имеет ли смысл ее включать.
        //          если имеет, то заполняем все необходимые поля
                     if(CheckAndFillSection(_Section->Daughter[x][y]))
                       AddSection(&BList,_Section->Daughter[x][y]);
         //           и перейти к следующей дочерней секции
                     continue;
                     }
        //        конец
                  }
        //    конец циклов
               }
        // 3. Выход.
        // 3.1. Возвращаем список BList.
           return BList;
        // ПРИМЕЧАНИЕ: В данном случае память выделенная вначале под массив не
        // очищается, потомучто функия передает "в внешний мир" только указатель на
        // первую ячейку массива, это значит, что данные, под которые была выделена
        // память еще будут непосредственно использоваться вне функии и поэтому их нельзя
        // удалять - нельзя очищать память.
        }

bool TSection::CheckAndFillSection(TSection* _Section)
     {
     bool Result = true;
//    _Section  - проверяемая секция.
//   1. Если секция закрыта
     if (!_Section->Open)
//      начало
        {
//   1.1. Если секция занята - результат отрицательный.
        if (_Section->Occupied)
           return false;


//   1.2. Если секция залита,
        if (_Section->Creeked)
//            и путь из центра заливки до текущей секции через эту секцию длиннее, чем
//            через ту, которой она уже была залита (или равен) - результат отрицательный
           {
           if ((Steps + sqrt( (double)kvdrt(_Section->X - X )+kvdrt(_Section->Y - Y))) > _Section->Steps)
              return false;
//   1.2.2. Если секция залита, но от нас до нее идти меньше, то все поля в ней нужно поменять, но
//          отправлять ее повторно на заливку незачем.
              else Result = false;
           }
//   1.3. Проверяем карту, возможно ли поставить объект с заданной шириной в центр этой секции или
//        в середену отрезка соденияющего центры этой и заданной секции,
//        если нет - резальтат отрицательный.

        int MidX = _Section->X + round(float(X - _Section->X)/2);  // середина вышеописанного отрезка
        int MidY = _Section->Y + round(float(Y - _Section->Y)/2);
        if (
           (!_Section->CheckMap(false,-1,-1))
           ||
           (!_Section->CheckMap(false,MidX,MidY))
           )
           return false;

//   3.1. Заполняем поле Steps для секции _Section по формуле:
//        сумма расстояние до этой секции и растояния между центрами этой и секции _Section.
//        _Section->Steps = Steps + ( (_Section->X - X)^2 + (_Section->Y - Y)^2)^(1/2);
        _Section->Steps = Steps + sqrt( (double)kvdrt(_Section->X - X )+kvdrt(_Section->Y - Y));
//   3.2. Заполняем поле _Section->FromSection - указателем на эту секцию.
        _Section->FromSection = (this!=_Section)?this:NULL;
//   3.3. Если секция не была залита раньше - заливаем ее.
        _Section->Creeked = true;
        }
//   4. Выходим.
//   4.1. Все тесты пройдены - результат положительный
     return Result;
     }

bool TSection::CheckMap(bool ForSection,int CentX,int CentY)
     {
     // 0. Определение границ проверяемой области карты.
     // 0.1. Ширина объекта.
     int W  = round(float(OurObject->Width)/2);
     // 0.2. Левая, правая, нижняя и верхняя граници соответственно.
     int _X = (CentX>=0)?CentX:X;
     int _Y = (CentY>=0)?CentY:Y;
     int LB = (!ForSection)? _X-W: SX;
     int RB = (!ForSection)? _X+W: EX;
     int BB = (!ForSection)? _Y-W: SY;
     int TB = (!ForSection)? _Y+W: EY;

     // 1. Сканируем заданную, минимально область,
     //    с центром в центре секции, и если хотя бы одна точка этой области не удовлетворяет условиям
     //    выходим, с отрицательным результатом.
     for (int x = LB; x<=RB; x++)
     for (int y = BB; y<=TB; y++)
         if (
     // 1.1. Если, например, на карте в текущей точке препятствие
            (Society->MAP[x][y].Team == NUMBER_OF_TEAMS)
            ||
     // 1.2. или точка просто выходит за границы карты:
            (!InXYSpace(0,MAP_WIDTH,0,MAP_HEIGHT,x,y,true,false,true,false))
            )
     //    выходим, с отрицательным результатом.
            return false;
     // 2. Тест пройден - секция для размещения объекта годна.
     return true;
     }
// Добавление подсписка(SubList) в список (InList)
bool TSection::AddList (TGList *InList, TGList *SubList)
     {
     // 1. Проверяем на возможные ошибки
     // 1.1. Если сумма !количества! элементов списка InList и !количества! элеиентов списка SubList
     //      больше размера списка InList - возвращаем отрицательный результат.
        if ((InList->Num+SubList->Num) >= InList->TotalNum)
           return false;
     // 2. Организовываем цикл FOR котором перебираются все элементы списка SubList
        for (int i = 0; i<SubList->Num; i++)
     // 2.1. Добавляем текущий элемент списка SubList в список InList.
            if (!AddSection(InList,SubList->Section[i]))
               return false;
     // 3. Выходим - возвращаем положительный результат.
     return true;
     }

// Добавление секции в список.
bool TSection::AddSection(TGList* _List, TSection *Section)
     {
     // 1.1. Проверяем на корректность информации.
     if ((_List == NULL)||(Section == NULL))
        return false;
     // 1.2. Проверяем  на факт переполнения списка после добавки в него
     //      этой секции.
     if (_List->Num >= _List->TotalNum)
        return false;

     // 2. Добавляем секцию в конец.
     _List->Section[_List->Num] = Section;
     _List->Num++;
     // 3. Возвращаем ИСТИНУ.
     return true;   
     }
// Удаление из списка.
bool TSection::DelFromList(TGList *_List, int ElementIndex)
     {
     // 1. Проверка на возможные ошибки.
     // 1.1. Если указан индекс несуществующего элемента или
     //      указан элемент меньше нуля - результат отрицательный.
        if (!InSpace(0,_List->Num,ElementIndex,true,false))
           return false;
     // 2. Элементу с заданным индексом приравниваем значение последнего элемента.
        _List->Section[ElementIndex] = _List->Section[_List->Num-1];
     // 3. Количество элементов уменьшаем на 1.
        _List->Num--;
     // 4. Выход, возвращаем 1.
        return true;
     }

//----------------------TPoints-------------------------------------------------
//------------------------------------------------------------------------------
bool TPoints::Init (TGObject *_OurObject)
     {
     if (_OurObject == NULL)
        return false;
     //инициализация направлений
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
     //инициализация пунктов входа/выхода
     //Distination point`s initialization

     A.X = -1;
     A.Y = -1;
     B.X = -1;
     B.Y = -1;

     //инициализация объекта - обладателя
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
    // создаем два одномерных списка точек которые в данную очередь должны заливаться
    struct {
           TXY* XY;
           int Num;
           }
           List[2];
    // изначально их инициализируем
    List[0].Num   = 0;
    List[0].XY = new TXY [MAP_WIDTH*MAP_HEIGHT];
    List[1].Num   = 0;
    List[1].XY = new TXY [MAP_WIDTH*MAP_HEIGHT];
    // делаем "предварительную заливку"
    List[0].XY[0] = B; // (мы заливаем одну точку - точку "В")
    List[0].Num  ++;

    // t = abs(t - 1)  - это значит, что значение "t" будет поочередно либо 0 либо 1
    // то е. либо первый список либо второй, для обращения к "противоположному" списку
    // нужно написать List[abs(t-1)]
    for (int t = 0; List[t].Num>0; t = abs(t-1))
        {
        Temp++;
        //перебираем все точки вышеописанные точки
        for (int i = 0; i<List[t].Num;i++)
            {
            // заливаем их
            int X = List[t].XY[i].X;
            int Y = List[t].XY[i].Y;
            if(CheckThis(X,Y))  // один из двух чекзисов лишний - вроде этот.
              {
              // если текущая точка - координата - пункта "А", то значит заливка
              // производится незря
              if (
                   (List[t].XY[i].X == A.X)
                   &&
                   (List[t].XY[i].Y == A.Y)
                 )
                 Res = true;

              // заливаем точку
              // обратите внимание, что не ЗДЕСЬ указывается информация о том откуда залита точка

              Point[X][Y].Creeked = true;
              /*Canvas->Pixels[X][Y] = 256*256*(256-round(Point[X][Y].Step));*/ // что-то вроде индикитора наших делов

              // она указывается тут,в части процедуры, в которой
              // заносится информация в противоположный список о том,
              // какие точки мы можно залить из этой точки.
              for (int x = -1;x<=1;x++)
                 for (int y = -1;y<=1;y++)
                     {

                     //проверяем точку
                     if (CheckThis(x+X,y+Y))
                     // если она раньше не "заливала" эту точку...
                     if (!Point[X][Y].Where[x+1][y+1])
                     // если из этой точки ее еще не залили
                     if (!Point[X+x][Y+y].Where[2-(x+1)][2-(y+1)])
                        {
                        // смотрим будет ли минимальным количество шагов из точки А в точку
                        // (Х+х;У+у) через точку (Х;У) если нет зачем нам ее заливать ?
                        float MinStep     = Point[X][Y].Step;

                        if (
                           ((x == -1)||(x == 1))
                           &&
                           ((y == -1)||(y == 1))
                           )
                           MinStep += sqrt( (double)2 )-1;

                        // проверяем не хочет ли уже точку (Х+х;У+у) залить кто другой
                        // если хочет, то смотрим  шаги, а заодно и уясняем для себя, что
                        // "стучать" нам нам эту точку не придется
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

                         // помечаем что эту точку еще можно залить и от нас...
                         Point[X+x][Y+y].Where[2-(x+1)][2-(y+1)] = true;


                        // если заливка идет по диагонали то прибавляем корень из 2-х
                        Point[X+x][Y+y].Step = MinStep+1;




                        // до точки (х+Х;у+У) нужно сделать на один больше шагов,
                        // идя к ней из "В" через точку (Х;У), чем идя до точки (Х;У)
                        if (!AlreadyCreek)
                           {
                           int j = List[abs(t-1)].Num;
                           List[abs(t-1)].Num++;


                           if (List[abs(t-1)].Num >= (MAP_WIDTH*MAP_HEIGHT))
                              {
//                            ShowMessage("УЖАСНАЯ ОШИБКА!!!");
                              return false;
                              }

                           List[abs(t-1)].XY[j].X = X+x;
                           List[abs(t-1)].XY[j].Y = Y+y;
                           }
                        }
                     }


              }

            }
         // в самом конце надо заново проинициализировать текущий список.
         List[t].Num = 0;
        }

        // и вот когда заливать уже невозможно более, мы проверяем - была ли
        // где нибудь "там" достигнута наша цель - ПУНКТ А. Точнее мы не
        // проверяем, а говорим о рузультате
        return Res;


    }


/*     Point[_XY.X][_XY.Y].Creeked = true;

     bool Result = true;
     // перебираем все точки карты
     while (Result)
      {
      Temp++;
      Result = false;
      for (int X = 0;X<MAP_WIDTH;X++)
        for (int Y = 0;Y<MAP_HEIGHT;Y++)
           {
            //если текущяя точка залита
            if (Point[X][Y].Creeked)
              {
              //перебираем ее окрестноть
              for (int x = -1;x<=1;x++)
                 for (int y = -1;y<=1;y++)
                    {
                    //если со стороны текущей данной точки окрестная данная точка не залита - залить.
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
     // структура нужная для нахождния минимального расстояния
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
//проще говоря если траекторию построить все-таки удалось, и препятствий на удивление не оказалось ... ОГОНЬ!
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


     // ВРЕМЕННАЯ ВСТАВКА !!!!!!!!!!!!!!!!!!
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


     // НАНЕСЕНИЕ ПОВРЕЖДЕНИЯ
     // ------------------------------------------------------------------------
     if (OldCurrentZ <= Society->MAP[round(OldCurrentX)][round(OldCurrentY)].Z)
         {
	 MustFree = true;
         On       = false;
         }
     //{

     if ((WentWay!=0)&&(CurrentZ <= Society->MAP[round(CurrentX)][round(CurrentY)].Z)) //если сейчас время сравнятся счетчику с максимальным временем:
        {
        //вначале поражаем все вокруг
        int _X = round(CurrentX),
		    _Y = round(CurrentY);
        for (int y = round(-Radius);y<=round(Radius);y++)
         for (int x = round(-Radius);x<=round(Radius);x++)
          {
          if (!((InSpace(0,MAP_WIDTH,(_X+x),true,false))&&(InSpace(0,MAP_HEIGHT,(_Y+y),true,false))))
		     continue;
             //если поражаемая точка выходит за границы карты - просто не поражаем ее
          
          float XY_;
          float r = sqrt( (double)kvdrt(x )+kvdrt(y)),
		        b = Damage,
		        k = Damage/Radius;
          int damage = (r<=Radius)?(k*r+b):0;

		  if (Radius >=CRASH_RADIUS)
		  // из координаты Z вычитаем значение прямопропорциональное поврежнению и обратнопропорционалльное
		  // радиусу...
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
      //  else //иначе крутим счетчик и обновляем координаты
      




     }


//-------------------------------------------------------------------------------



//void DoAction()
//----------------------TGObject------------------------------------------------
/*
здесь свойства и локальная работа(работа не связанная с другими обектами)
одного обекта.
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
 AttackPeriod     =   Standart->AttackPeriod;  // через каждые АттаскСпеед кадров наносится удар
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

 Is = OldIs     = false; // Это свойство показывает - существует объект или нет.
 N              = 2;
 PrimEnemy.Type = -1;
 InGroup        = false;
 GroupIndex     = -1;
 GroupTeam      = -1;
 IndexInGroup   = -1;
 PrimaryEnemy   = NULL;
 GETPOWER();

// Аппарат обхожа препятствий (SectionWay) требует для себя информацию о многих
// параметрах объекта, поэтому разумно инициализировать его самым последним.

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
	 //проверка на ошибки
	 ind = (ind >= NUMBER_OF_WEAPONS)?0:ind;

         // определение типа
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


     // Загрузка данных об оружее в ПАСПОРТ.
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
     if (Society->Groups[GT][GI].Number == 0) //если из группы исключены все юниты не стоит
                                              //дожидаться пока метод RefreshGroupParametres
                                              //удалит эту группу, по скольку за один "акт"
                                              //может несколько раз вызываться метод OutGroup
                                              //попадающий на одну группу
                                              //или попросту нужно именно на ЭТО место поставить
                                              //другую группу
         Society->DelGroup(GT,GI,"OutGroup");
      }


void TGObject::SetModelFrame()
     {
     //см схему в тетради
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
//эта функция задает новую позицию объекту с помощью координат   н о в о й   п о з и ц и и.
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
//эта функция задает новую позицию объекту с помощью угла и расстояния до него
     {
     int DeltaX = round(R*cos(Angle*OGIR)),
         DeltaY = round(R*sin(Angle*OGIR));
     if ((!InSpace(0,MAP_WIDTH,X+DeltaX,true,false))||(!InSpace(0,MAP_HEIGHT,Y+DeltaY,true,false)))
         return;

     NewPosition.X = X + DeltaX;
     NewPosition.Y = Y + DeltaY; //вычисление координат новой позиции
     if ((NewPosition.X > MAP_WIDTH)&&(NewPosition.X<0)&&(NewPosition.Y>MAP_HEIGHT)&&(NewPosition.Y<0))
        bool Stop = 0;

     NewPosition.DeltaX = DeltaX;
     NewPosition.DeltaY = DeltaY;
     NewPosition.Steps = 0;
     if (min(abs(DeltaX),abs(DeltaY))==abs(DeltaY) )
        NewPosition.SmallPoint = Y; else   //а это вычисление вспомогательных величин
        NewPosition.SmallPoint = X;        //для перемещения по карте.

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


        if ( (GlobalDeltaX!=0)||(GlobalDeltaY!=0) ) // если есть куда идти...
           {
           if (GlobalDeltaX!=0) // во избежании деления на нуль
              NewPosition.GlobalAngle = StdAngl(atan2( (double)GlobalDeltaY, (double)GlobalDeltaX)/OGIR);
              else  {
                    if (GlobalDeltaY > 0)
                       NewPosition.GlobalAngle = 90;
                       else
                       NewPosition.GlobalAngle = 270;
                    }

              NewPosition.GlobalR =  sqrt ( (double)kvdrt(GlobalDeltaX)+kvdrt(GlobalDeltaY) );
           }                   // end of <во избежании деления на нуль>
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
     //пока пустышка
     }*/
void TGObject::SetStatus() //настраивает не только состояние, но и неявно указывает что делать юниту (см.DoAtion)
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
        Status.Fire = false; //если юнит должен стрелять - то в ф-и ФАЙР он станет true.
     Status.InShock = false;

	 Status.Deading = ((!Is)&&(OldIs))?true:false;

     }


bool TGObject::CanKill(TGObject *Enemy)
     {
     /*
     сравниваем мощьность нашего и вражескую
     */
     if (POWER >= Enemy->POWER)
        return true;
          else
          return false;

     }
void TGObject::GetEnemy(TGObject *Enemy)
     {
//     if (Enemy == NULL) {ShowMessage("НЕВЕРНО ВЫЗВАНА Ф-я GetEnemy()!!"); return;};
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
         if((PrimaryEnemy!=NULL) && (PrimaryEnemy->Is) ) //если мы имеем данные в поле PrimaryEnemy
            {//то надо  скопировать их в поле PrimEnemy, а так же просчитать угол и расстояние до врага
             int DeltaX = PrimaryEnemy->X - X,
                 DeltaY = PrimaryEnemy->Y - Y;

                 PrimEnemy.Angle = StdAngl(atan2( (double)DeltaY, (double)DeltaX)/OGIR);
                 PrimEnemy.R = sqrt( (double)kvdrt(DeltaX )+kvdrt(DeltaY));
            }//else
             //PrimEnemy.Type =-1;

        }
        else
        {
        PrimaryEnemy =NULL; //если врага убили очищаем поле PrimaryEnymy
         //аналогичную операцию проделать надо в ф-и TGSociety::SetEnemy(t,i)


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
//              группы объектов , действия в группе
//---------------------------------------------------------------------------
void TGGroup::Init(TGSociety *_Society)
     {
     Society          = _Society;
     Mission.Init(this);
     OnMission        = false;
     Is               = false;
     Distination.X    = -1;
     Distination.Y    = -1;
     EGroup           = 0;  // вражеская группы не выбрана
     EUnit            = 0;   // вражеский юнит не выбран


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
//         ShowMessage(" Не работает оператор присваивания ( ф-я TGGroup::Init() )");
     NumberToKill=0; //количество врагов которое надо убить;
     NumberOfKilledEnemies=0; //количество убитых врагов;


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
//            ShowMessage("Ошибка в функции TGGroups::GetPower");
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
     короче, план действий такой: выщитываем угол на который надо сместить
     группу, и расстояние и смещаем каждого юнита этой группы на такой расстояние
     */
     if (Team == 5)
        bool Stop = true;
     //расстояние будем считать по первому в группе (в прорамном понимании)
     int DeltaX = X - Units[0].PUnit->X,
         DeltaY = Y - Units[0].PUnit->Y;
     float
         R = sqrt( (double)kvdrt(DeltaY ) + kvdrt(DeltaX)),
         Angle =(DeltaX!=0)?StdAngl(atan2( (double)DeltaY, (double)DeltaX)/OGIR):((DeltaY>0)?90:(-90));
         //суть условия такова
         //если ДельтаХ != 0 выщитываем угол по привычной формуле
         //иначе если ДельтаУ больше 0 то угол равен 90гр иначе -90гр




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

     if (Number>=0) //все эти сдвиги нужны только когда в группе уже есть народ
     {
     while (
           (i<Number)
           &&
           (Unit->POWER < Units[i].PUnit->POWER)
           ) //ищем куда бы его вставить
           i++;


     if (Number == NUMBER_OF_UNITS)
        {
//        ShowMessage("В группе "+IntToStr(Team) + " ; "+IntToStr(Index)+" МЕСТ НЕТ!");
        return;
        }
     if (Units[i].PUnit!=NULL) //если есть кого сдвигать то
     for (int in = (Number-1); in >= i; in--) //сдвигаем всех на 1 вправо
         {
         Units[in+1] = Units[in];
         Units[in].PUnit->IndexInGroup++;
         }
     }
    // вставляем
     Units[i].PUnit = Unit;
     Units[i].Is = true;
     Units[i].IndEnemy = -1;
     Number++;

     /*
     Отныне, ты, Unit, являешься членом этой группы,
     клянись делать все ради группы.. клянись исполнять Ее и только Ее приказы !
     */
     (*Unit).InGroup = true;
     (*Unit).GroupIndex = Index;
     (*Unit).GroupTeam = Team;
     (*Unit).IndexInGroup = i;
     /*
     Unit(положа поле Damage на функцию Init() - руку на сердце ): "Клянусь исполнять все ради группы,исполнять Ее и только Ее приказы! "
      */
     }
void TGGroup::Sort()
     {
     int i = 0 ;
     while (i<Number)
           {
           int MAX = Units[0].PUnit->POWER,
               IndexMAX = 0;
           //1. находим макс. знач-я
           for (int j = i; j<Number;j++)
                    {
                    if (MAX < Units[j].PUnit->POWER)
                       {
                       MAX = Units[j].PUnit->POWER;
                       IndexMAX = j;
                       }
                     }
           //2. меняем первый и максимальный элементы местами.
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
     //Для этой функции нужно реализовать метод Dye на более высоком уровне (в классе TGSociety)
     //а еще нужен метод класса TGGroup::CanKill (TGGroup *EnenyGroup)
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
		   return; //каждого натравляем на этого врага и выходим из ф-и
           }
		   else
		   {
			/*временная часть*/   if (NumberOfKilledEnemies>0)Free(); //если всех переубивали
		   }
        return;//а если нет ни юнита-врага, ни группы-врага то тут подавно "анализировать" нечего
        }

//раздача заданий - см. тетрадь "проект "игра" III Глава 2. Модернизация"
// Набираем войнов на 1-го врага, затем на 2-го и т.д.
int i    = 0; //индекс солдата этой группы;
int e    = 0; //индекс солдата вражеской группы;
int sum  = 0; //силы, противопоставляемые врагу;
int epow = 0; //силы врага;

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


//всех остальных распределяем ровномерно
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
    





/*     //если мы сильней
     //наваливаемся всей толпой на самого сильного
     //т.е. на самого првого в ширенге
     //если слабей - на слабого, последенего в ширенге
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

          // Инициализация базы с врагами.
          // 1. Инициализация всех IncInBase
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
                // Если в указанном месте враг.
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
                // иначе:
        if  (
            (PreferXY)
            ) //если предпочтение походу,нежели драке, то отодвигаем очередь "врагов" в сторону последнего
              //(вколючая последнего, разумеется)
           {
		   if (Enemy[0].Object_Group == 2)
		       {
			   Enemy[0].Object = _DATA;
			   return true;
			   }

           if ((OurGroup->Team == 2) && (OurGroup->Index == 6))
              bool stop = 1;

           EnemyNumber++;

           for (int i = EnemyNumber-1;i>=0;i--) //переносим все на одно заданаие на вконец
                MoveData(i,i+1); //если i>=MAX_ENEMIES информация стирается
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

      //проверка на уникальность данных (тоесть, что объект(группа) в базу еще не занесены)
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
        return false;  //защита


     if (InSpace(0,MAX_ENEMIES,on_i,true,false) )
        {
        Enemy[on_i] = Enemy[from_i]; // вся информация которая "была"(если была) в ячейке с индексом "on_i"
                                     // теряется.
        
        if ((on_i >=EnemyNumber)&&((Enemy[on_i].Object.Is)||(Enemy[on_i].Object_Group == 2)))
           EnemyNumber++;
        if  (Enemy[on_i].Object.Is)    
            {         
            if (Enemy[on_i].Object_Group == 0) // не стоит забывать про системное сообщение врагу о том, под 
                                               // каким индексом он находится в этой базе
            Enemy[on_i].Object.GGroup->IncludedInBase[OurGroup->Team][OurGroup->Index].Index  = on_i;
    
            if (Enemy[on_i].Object_Group == 1)
            Enemy[on_i].Object.GObject->IncludedInBase[OurGroup->Team][OurGroup->Index].Index = on_i;
            }
        }

        FreeDataCell(from_i);        //переводится как "освободить ячейку информации" (англ).
      
     return true;
     }

void TGGroup::TMission::FreeDataCell(int i)
     {
     if (i >= MAX_ENEMIES) return;//защита
     //если в ячейке информация о враге
          if ((OurGroup->Team == 2) && (OurGroup->Index == 6))
             bool stop = 1;

     if ((i == (EnemyNumber-1))&&((Enemy[i].Object.Is)||(Enemy[i].Object_Group==2)))
        EnemyNumber--; // можно було бы примнить и скоращенную форму условия,



     if ((Enemy[i].Object_Group!=2)&&(Enemy[i].Object.Is))
        
        if (Enemy[i].Object_Group == 0)  //конкретно - о группе
            //говорим, что этому врагу, что он ВЫЧЕРКНУТ из нашей базы
           {
           Enemy[i].Object.GGroup->IncludedInBase[OurGroup->Team][OurGroup->Index].Yes = false;
           Enemy[i].Object.GGroup->IncludedInBase[OurGroup->Team][OurGroup->Index].Index = -1;
           }
           else  
               {
               if (Enemy[i].Object_Group==1)
                //или о юните
                //говорим, что этому врагу, что он ВЫЧЕРКНУТ из нашей базы
               {
               Enemy[i].Object.GObject->IncludedInBase[OurGroup->Team][OurGroup->Index].Yes = false;
               Enemy[i].Object.GObject->IncludedInBase[OurGroup->Team][OurGroup->Index].Index = -1;
               }
                  else 
                      {
                      Enemy[i].Object.Is    = false; //на всякий случай
                      Enemy[i].Object_Group = 0;
                      }
               }
     
        
     //что сказали - то и написли! - наше слово - КОД !
     Enemy[i].Object.Is = false; 
     
     }



void TGGroup::TMission::DoMission()  //сибирский валенок и тот сложнее устрооен...
                                     // а хотя... представляете сколько в нем волокон! .. и все такие .. разные!
     {
     // суть функции такова:
     // если предпочтение отдается достижению цели, то значит вся информация о новых координатах, согласно
     // принципу функции GetNewData() будет храниться в БД заданий
     // а если предпочитаетсяа битье по ..лицу, то вся информация о координатах, опять же, согласно принципу
     // GetNewData, храниться в, специально для них предназаначенной структуре XY;
     // и тогда эта миссия, прежде, чем вызовать функцию отправки группы, не надо ли кого поколоитить.
     // вот так.

         if (Enemy[0].Object_Group==1)                                                // этот иф выполняется всегда
            {
/*          if ((Enemy[0].Object.GObject!=NULL)&&(!Enemy[0].Object.GObject->Is))*/

//          ТЕСТОВОЕ ИЗМЕНЕНИЕ
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

//                  ТЕСТОВОЕ ИЗМЕНЕНИЕ.
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
         return true; // миссия ее в этом мире завершена...

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
          /*(EnemyNumber==0) //пока только проверяем у цели ли солдат.
          &&*///

          // ТЕСТОВАЯ ВСТАВКА
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
          // смысл примерно такой: если враг - это группа и если она еще не уничтожена, то не удалять; также нельзя удалять если враг - до сих пор неуничтоженный объект.
          // проверка на NULL не столько алгоритмическая сколько системная - всякое может быть ...
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
//в этом классе описано взаимодействие объектов, и их деятельность на карте
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
     //  Формат хранения данных.

     //  1.  Ширина карты      - 2 б
     //  2.  Высота карты      - 2 б
     //  3.  Число юнитов      - 2 б
     //  4.  Число команд      - 2 б
     //  5.  Число эталонов    - 2 б
     //  6.  Количество оружия - 2 б

     //  7.  Оружее:
     //          1.Размер единичной структуры
     //          2.Информация обо всех видах оружия

     //  8.  Эталоны:
     //          1. Размер единичной структуры
     //          2. Информация о всех видах объектов:

     //  9.  Карта:
     //          1. Размер единичной структуры
     //          2. Информация о всех ячейках карты ([Х][У])


     //  11. Объекты (юниты):
     //          1. Размер единичной структуры
     //          2. Информация о всех объектах ([Команда][Индекс в команде])

     //  12. Команды:
     //          1. Размер единичной структуры
     //          2. Информация о всех командах



     // Запись в соответствии с форматом.
     FILE* f = fopen(MAP_PATH,"r+b");

     //  1.
     if (MAP != NULL)
        {
        for (int x = 0; x<MAP_WIDTH;x++)
            {
            //  1.1. Очистка памяти выделенной под карту.
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

     //  3.1. Очистка памяти выделенной под объект.
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

     //  7.2.1 Очистка памяти выделенной под оружее.
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
     //  8.2.1. Очистка памяти выделенной под эталоны юнитов.

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

             // Инициализация динамического массива.
             GObject[t][i].IncBaseInit();

             // Указатель на общество.
             GObject[t][i].Society = this;

             // Указатели в объекте TARGET
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
     //  11.2.1. Очистка памяти выделенной под команды
     if (Teams!= NULL)
        {
        delete[] Teams;
        Teams = NULL;
        }
     //  11.2.2.
     Teams = new TTeams [NUMBER_OF_TEAMS];
     for (int t = 0; t<NUMBER_OF_TEAMS;t++)
         fread(&Teams[t],TM_SIZE,1,f);




     // Закрытие файла.
     fclose(f);
     return true;
     }





void TGSociety::GSocietyInit(bool first)
     {
      Dead = 0;


//   ИНИЦИАЛИЗАЦИЯ "ПО УМОЛЧАНИЮ".

                                                 // ПРЕ Инициализация объектов

     GObject = new TGObject* [NUMBER_OF_TEAMS];
     for (int t = 0; t < NUMBER_OF_TEAMS; t++)
         GObject[t] = new TGObject [NUMBER_OF_UNITS];




													//команды
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
                                                          //Инициализация карты
        MAP = new TMap* [MAP_WIDTH];
        for (int j = 0; j < MAP_HEIGHT; j++)
                {
                MAP[j] = new TMap [MAP_HEIGHT];

                for (int i = 0; i < MAP_WIDTH; i++ )
                    {

                    MAP[j][i].Index = -1;
                    MAP[j][i].Team = -1;
					if(first)
                                                          // Расчет текстур карты
                    MAP[j][i].Z = 2;
                    MAP[j][i].Tex=2;


		    if(MAP[j][i].Z<-1)
                      MAP[j][i].Tex=0;

		    if((MAP[j][i].Z>1)|(MAP[j][i].Z==-1))
                      MAP[j][i].Tex=1;

                    }
                }

	 //системные параметры
     ViewMode    = false;
	 AttackMode  = false;
	 ChaseMode   = false;
	 MoveMode    = false;

//   КОНЕЦ ИНИЦИАЛИЗАЦИИ ПО УМОЛЧАНИЮ.


//   ЗАГРУЗКА КОНФИГУРАЦИИ ИЗ ФАЙЛА.

     LoadConfig();



//   ЗАГРУЗКА ОБЪЕКТОВ ЗАВИСЯЩИХ ОТ КОНФИГУРАЦИИ
      {
       Groups = new TGGroup* [NUMBER_OF_TEAMS];
       GroupsNumber  = new int [NUMBER_OF_TEAMS];
       {for (int t = 0;t<NUMBER_OF_TEAMS;t++)
            GroupsNumber[t] = 0;}

        for (int t = 0; t<NUMBER_OF_TEAMS;t++)    //группы
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

        // карта
        InitMapPassably();

        for (int j = 0; j < MAP_HEIGHT; j++)
                {
                for (int i = 0; i < MAP_WIDTH; i++ )
                    {

                                                         // Расчет текстур карты
                    MAP[j][i].Tex=2;
		    if(MAP[j][i].Z<-1)
                      MAP[j][i].Tex=0;
		    if((MAP[j][i].Z>1)|(MAP[j][i].Z==-1))
                      MAP[j][i].Tex=1;

                    }
                }


      ANGLE_MATRIX_Init();                             //угловая матрица

                                                       //квадраты карты

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



//   ИНИЦИАЛИЗАЦИЯ СИСТЕМНЫХ КЛАССОВ
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
//проверка на ошибки
if (Team>=NUMBER_OF_TEAMS)
   return false;
if (Type >= NUMBER_OF_STANDARTS)
    Type = 0;


int X1 = X - Standart[Type].Width/2,
    X2 = X + Standart[Type].Width/2,
    Y1 = Y - Standart[Type].Width/2,
    Y2 = Y + Standart[Type].Width/2;
//кладем на карту
int Result = true;
{
 for (int y = Y1-POSSIBLY_PUT_INTERVAL;y<=Y2+POSSIBLY_PUT_INTERVAL;y++) //пока он представляет заштрихованный квадрат на карте.
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
	}	 //тоесть если есть хотябы одна "запретная" точка,
                     //то функция вылетает и возвращает ЛОЖЬ
}

//если "запретных точек" нет то можно класть юнита на карту...
for (int y = Y1;y<=Y2;y++) //пока он представляет заштрихованный квадрат на карте.
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
        Unit->SetStatus(); //СТАТУС должен быть выше ДУЭКШЕНА !
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
    // проверяем точчку, на которой стоим, или собираемся встать


//   Этод метод требует ОСОБОГО ВНИМАНИЯ
//   Выше допущена очень большая погрешность,
//   и оно одлжен ее учитывать.
    if(!GObject[Team][Index].CheckPoint(GObject[Team][Index].X,GObject[Team][Index].Y))
      return;

    if (SnAction)
       {
        for (int y = Y1;y<=Y2;y++) //пока он представляет заштрихованный квадрат на карте.
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
          for (int y = Y1;y<=Y2;y++) //пока он представляет заштрихованный квадрат на карте.
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

//Изменение координат
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
//ShowMessage("Юнит из команды "+IntToStr(Team)+" с индексом "+IntToStr(Index)+" вышел за границы карты!");
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
     //тут стоит вспомнить формулу расчета диагоналей в многоугольнике
     //№ диаг. = ((№ углов-1)*№ углов)/2  --  по такойже формуле расчитывается количество отрезков,
     //которые образуются между юнитами.
     //
     //Но тем юнитам которые находятся в группе и не руководят ею не нужно видеть!


     // инициализация
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
     // конец инициализации


//начиная от первого до предпоследнего(для него уже к тому временю будет все расчитано)
//каждому юниту расчитываем уголя относительно остальных, да и остальным заодно просчитываем угол
//для этого юнита.
     for (int T = 0;T<NUMBER_OF_TEAMS;T++)
         for(int I = 0;I<NUMBER_OF_UNITS;I++)

               for (int t = T;t<NUMBER_OF_TEAMS;t++) // вот тут уже перебираем "остальных"
                   {
                   int INDEX;
                   if (t==T) INDEX = I;
                      else INDEX = 0;  //начинаем со следующего юнита своей команды и до конца...
                   for(int i = INDEX;i<NUMBER_OF_UNITS;i++)
                      {

                      if ((t == T) && (i == I))
                         continue;
                         //если один из двух мертв

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

                      int X=GObject[T][I].X, //координаты данного обекта
                          Y=GObject[T][I].Y,

                          x=GObject[t][i].X, //координаты остальных обектов
                          y=GObject[t][i].Y;

                      float DeltaX = x-X,
                            DeltaY = y-Y,
                            R = kvdrt(DeltaX)+kvdrt(DeltaY),
                            Angle=1000;
                            R = sqrt( (double)R );



                      if ((R <= GObject[T][I].Sight)&& (GObject[T][I].NVO < MAX_SIGHT)&&GObject[T][I].Thinking) //если внешний объект в поле зрения данного
                         {

                             if(R!=0)
                               {
                                 if (DeltaX!=0)
                                 Angle = StdAngl((float)atan2( (double)DeltaY, (double)DeltaX)/OGIR);//вычисляем угол
                                     else if(DeltaY > 0) Angle = 90;
                                          else if(DeltaY < 0)Angle = 270;
                               }
                               else Angle = 0;
                         //заносим сведения об обнаруженном объекте в матрицу данноу
                         ANGLE_MATRIX[T][I][GObject[T][I].NVO].R = R;
                         ANGLE_MATRIX[T][I][GObject[T][I].NVO].Angle = Angle;
                         ANGLE_MATRIX[T][I][GObject[T][I].NVO].Index = i;
                         ANGLE_MATRIX[T][I][GObject[T][I].NVO].Type = t;
                         GObject[T][I].NVO ++;
                         }
                         //заноми сведения о данном объекте в матрицу обнаруженнему
                         //если конечно обнаруженнему позволяет сделать это его зрение

                      if ((GObject[t][i].Sight >= R)&&(GObject[t][i].NVO<max_SIGHT)&&GObject[t][i].Thinking)
                           {
                           if (Angle == 1000)
                              {
                              if (DeltaX!=0)
                                Angle = StdAngl(atan2( (double)DeltaY, (double)DeltaX)/OGIR);//вычисляем угол
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
     //Требует новую функцию связанную с квадратами
     //TGSociety::AddUnitInKvadrat();

     int IOX = GObject[Team][Index].Kvadrat.IndexOX;
     int IOY = GObject[Team][Index].Kvadrat.IndexOY;

     int X = Kvadrats[IOX][IOY].X,
         Y = Kvadrats[IOX][IOY].Y,
         X2 = Kvadrats[IOX][IOY].X_,
         Y2 = Kvadrats[IOX][IOY].Y_;
     if (//если данный юнит не вышел за границы квадрата, в котором он есть, то ни чего не трогать
         GObject[Team][Index].Kvadrat.ID==-1
        ||
        (!((GObject[Team][Index].X >= X)&&
        (GObject[Team][Index].Y >= Y)&&
        (GObject[Team][Index].X <= X2)&&
        (GObject[Team][Index].Y <= Y2)))
        )
            {
             if (GObject[Team][Index].Kvadrat.ID == -1) //если он вообще нигде не числется
                {
                int OnOX = div(GObject[Team][Index].X,KVADRAT_WIDTH).quot;
                int OnOY = div(GObject[Team][Index].Y,KVADRAT_HEIGHT).quot;
                GObject[Team][Index].Kvadrat.IndexOX = OnOX;          //индекс квадрата по ОХ
                GObject[Team][Index].Kvadrat.IndexOY = OnOY;          // по ОУ

                AddUnitInKvadrat(OnOX,OnOY,&GObject[Team][Index],PUT); //добавляем "в квадрат"
                return; //тут больше нечего ловить
                }
             //------------

               else
             {
             int OnOX = 0,OnOY = 0; //анализируем в какой квадрат попал юнит (сдвиги по вертикали , горизонтали от данного)
             int KvX = GObject[Team][Index].Kvadrat.IndexOX;
             int KvY = GObject[Team][Index].Kvadrat.IndexOY;
             if (GObject[Team][Index].X < X) OnOX = -1;
                else if (GObject[Team][Index].X > X2) OnOX = 1;

             if (GObject[Team][Index].Y < Y) OnOY = -1;
                else if (GObject[Team][Index].Y > Y2) OnOY = 1;

             AddUnitInKvadrat(KvX,KvY,&GObject[Team][Index], DEL); //Удаляем из предыдущего квадрата,
             AddUnitInKvadrat((KvX+OnOX),(KvY+OnOY),&GObject[Team][Index],PUT); // добавляем в соответсвующий новый
             }

            }
            else return;


     }


     void TGSociety::AddUnitInKvadrat(int OnOX,int OnOY, TGObject *Unit,bool ADD)
          {
                     // OnOX равно OnOX, если мы не "удаляем на всегда", иначе смотрим данные о квадрате в юните
                     OnOX=(OnOX>=0)?OnOX:Unit->Kvadrat.IndexOX,
                     OnOY=(OnOY>=0)?OnOY:Unit->Kvadrat.IndexOY;
          int *Number = &Kvadrats[OnOX][OnOY].Number;//количство народу в квадрате


          if (ADD) //если функция должна добавить
             {
             if ((*Number) < MEN_IN_KVADRAT)
                {
                (*Number)++;
                Kvadrats[OnOX][OnOY].Units[(*Number)-1] = Unit;

                Unit->Kvadrat.ID = (*Number)-1;
                Unit->Kvadrat.IndexOX = OnOX;
                Unit->Kvadrat.IndexOY = OnOY;
                }
//                else ShowMessage("Ошибка в ф-ии <TGSociety::AddUnitInKvadrat>, как следствие функций, связанных с перемещением ");

             }
             else //если удалить
                 {
                 int ID = Unit->Kvadrat.ID; //порядковый номер в квадрате

                 /*
                 алгоритм таков:
                 т.к. данного юнита больше нет в этом квадрате, то,
                 что бы не бало "пробелов" в списке в ставляем вместо него последнего,
                 а то место где был последний освобождаем, и делаем "вакантным".
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

     //вначале смотрим нельзя ли пройти в лоб
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
                    if (ANGLE_MATRIX[Team][Index][i].Angle > (Angle + (AngleOW/2)) )  //сообщаем с какой стороны обойти нельзя
                       Left_Zero_Right = -1; // нельзя обойти справа - возможно... можно :-) обойти слева .
                       else
                       Left_Zero_Right = 1;  //аналогично



                    result = false;
//                    break;
                    }
           i++;
           }
       }

    //теперь попробуем обойти слева;
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

         //а теперь попробуем обойти справа;
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
     {// функция "мысленно" переносит объекто на его ширину и
      // если он оказывается на другом объекте она возвращает 0.
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



void TGSociety::SetPos(int Team,int Index/*, TCanvas *Canvas*/)   // Алгоритм сложного перемещения
     {
      float DeltaX = GObject[Team][Index].NewPosition.DeltaX,
            DeltaY = GObject[Team][Index].NewPosition.DeltaY;

     if (GObject[Team][Index].NewPosition.GlobalX !=-1)//это значение говорит, что ему никуда не надо.
     {/* if
       (
       ((DeltaX!=0)||(DeltaY!=0))&&
       (!CheckPoint(&GObject[Team][Index],123,false,GObject[Team][Index].Speed,20))
       )*/
      {
      float Angles[PART_IN_CYRCLE];
      int   Speeds[PART_IN_CYRCLE];
      float Angle   = GObject[Team][Index].NewPosition.GlobalAngle;
      // если DeltaX != 0 && DeltaY !=0 вычисляем угол по обыкновенной формуле - через арктангенс
      // иначе берем угол из поля "НьюПозишн"
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
          // переменная Speed использется еще и тут
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
     float SmlstR = SmalestR(Team,Index); //функция выводит растояние до ближайшего объекта
     float AngleOW = atan2( (double)OBJECT_WIDTH, (double)SmlstR);
     float Sight = GObject[Team][Index].Sight;

     int i = 0,j;
     bool Plus = true;;

     //инициализация ЧекМув
     for(int z = 0;z < 360;z++ )
         Check_Move[z].Type = -1;

     if (Plus) j = i; else j = 360 - i;
        Check_Move[i] = CheckMove(Angle,Team,Index,Canvas);


     bool C_L = false,C_R = false;
     while (!C_L || !C_R)              //это на случай того если впереди нет объектов
           {
           if(abs(i)>0) break;                // в этот цикл мы войдем, если не вошли в верхний .
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
      while ((i > 0) && (k < AngleOW) ) //этот цикл выполняется после одноко из двух выше написанных
                                        //тут ОБъект в стенке ищет дырку
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
//    ---***  Искусственный интеллект и ведение боя  ***---
//-------------------------------------------------------------

int TGSociety::Analis_Change(int Team,int Index,int index)
    {
    /*
    Если выражаться грубо, то эта функция работает так:
    она анализурет всех врагов, говорит юниту, что ему надо: атаковать или сваливать,
    а еще она ему ему советует, так чисто по дружески: "Вот, браток, засадика ему - мож сляжет".
    Все эти три параметра она заносит ему в МОЗГ а он потом все выполняет.
    */

    //если юнит "членит" в группе то он не имеет право думать!
    if (!GObject[Team][Index].InGroup)
    {

    index = -1;
    int Live =-1;



    int Enemy=0;//оценка врагов в баллах. (пока это просто количество)
    int Ally=1; //оценка боевой силы себя и своего противника
    for (int i = 0;i<=GObject[Team][Index].NVO;i++ )
        {
        if (i>=(NUMBER_OF_UNITS*NUMBER_OF_TEAMS)) break;

        // Если найден враг
        if ((ANGLE_MATRIX[Team][Index][i].Type != Team)
             &&
            (ANGLE_MATRIX[Team][Index][i].Type!=-1)
            )
            {
            // счетчик врагов
            Enemy++;

            //      если враг не здоровее объекта:
                    if (GObject[ANGLE_MATRIX[Team][Index][i].Type]
                               [ANGLE_MATRIX[Team][Index][i].Index].Live <= Live)

                        {
            //          включаем его в базу.
                        Live = GObject[ANGLE_MATRIX[Team][Index][i].Type]
                                      [ANGLE_MATRIX[Team][Index][i].Index].Live;
                        index = i;


                        }
            //      если в базу еще не включено ни одного врага:
                    if (Live == -1)
                       {
            //      включаем этого
                       Live = GObject[ANGLE_MATRIX[Team][Index][i].Type]
                                     [ANGLE_MATRIX[Team][Index][i].Index].Live;
                       index = i;
                       }



            }
            // если обнаружен союзник - крутим счетчик союзников.
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
//    Включаем врага в мозг.
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


//        --------------М о д у л ь------------
//        ---   * * *   A T T A C K * * *   ---
void TGSociety::Attack(int Team,int Index)
     {
     if (!GObject[Team][Index].CanAttack)  return; //если идти в атаку запрещено , то не хрена тут делать .
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

      if(GObject[Team][Index].TimeAfterAttack == (GObject[Team][Index].AttackPeriod)) // Если чувачок готов нанести удар
          //отнимаем жизни у врага
          {if  (//если Враг еще не мертв и его броня пробиваема для данного юнита, то
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
              //делаем юнита неготовым к атаке
              GObject[Team][Index].Status.Fire = true; //состояние выстреала
	      GObject[Team][Index].Status.FireMovie = (rand() > 30000)?1:0;
              GObject[Team][Index].TimeAfterAttack = 0;

              if (GObject[type][index].Live <= 0)
	         {
		 Dye(&GObject[type][index]);
                 //в условие написано примерно следующее
		 //если юнит в группе, а группа в которой он состоит на задании, то ...
		 if ((GObject[Team][Index].InGroup)&&(Groups[GObject[Team][Index].GroupTeam][GObject[Team][Index].GroupIndex].OnMission))
		    {
                    //у этой группы увеличить число убитых ею врагов на 1
		    Groups[GObject[Team][Index].GroupTeam][GObject[Team][Index].GroupIndex].NumberOfKilledEnemies++;
		    //и если группа по числу убитых выполнила норму то она освобождается от задания
		    if(
		      Groups[GObject[Team][Index].GroupTeam][GObject[Team][Index].GroupIndex].NumberOfKilledEnemies
		      ==
		      Groups[GObject[Team][Index].GroupTeam][GObject[Team][Index].GroupIndex].NumberToKill
		      )
                      Groups[GObject[Team][Index].GroupTeam][GObject[Team][Index].GroupIndex].Free();
		    }
	         }
              }
              //else ShowMessage("Не могу атаковать врага: внутренняя ошибка. Или не позволяет его броня.");
          }
	    else  {
                  GObject[Team][Index].TimeAfterAttack ++;
				  //GObject[Team][Index].Status.Fire = false;
	          }



    }

void TGSociety::Dye(TGObject *Unit)
     {
     Dead++;
     Unit->Is = false;//юнит больше не существует

     if ((Unit->TeamIndex == 1)&&(Unit->Index == 6))
        bool Stop = true;
     //нужна модернизация метода TGGroup::GetUnit (вносить сведения о группе в пасорт юнита)
     const int Team        = Unit->TeamIndex;//запоминаем его данные для последствующего использования
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
        for (int y = Y1;y<=Y2;y++) //пока он представляет заштрихованный квадрат на карте.
         for (int x = X1;x<=X2;x++)
         if (
            (InSpace(0,MAP_WIDTH,x,true,false))
            &&
            (InSpace(0,MAP_HEIGHT,y,true,false))
            )
            {
            // Добавить проверку, на принадлежность данной ячейки
            // карты юниту ( точнее наоборот) - слишком большая погрешность
            // при вычислении.

            MAP[x][y].Team = -1;
            MAP[x][y].Index = -1;
            }


     int OX = Unit->Kvadrat.IndexOX,
         OY = Unit->Kvadrat.IndexOY;
     AddUnitInKvadrat(-1,-1,Unit,DEL);


     if (Unit->InGroup)   //стираем данные о юните в группе
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
     //удаляем его из всех баз групп.
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
     Unit->Y = -10; //уносим душу усопшего из проекции мира сего в пространство астрала  ..
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
//                        И. И. группы

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
//некорректная часть
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
//        ShowMessage("Не могу создать группу! Т.к. в этой команде уже максимальное число групп!");
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


     for (int i = 0;i<Groups[Team][Index].Number;i++)//стираем
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
     Groups[Team][Index].POWER=0; //все остальное
     Groups[Team][Index].Is = false;



     //некорректная часть функции
     /*if (Index!=(GroupsNumber[Team]-1))
     {
      Groups[Team][Index] = Groups[Team][GroupsNumber[Team]-1];   //ставим вместо данной группы последнюю
       for (int i = 0;i<Groups[Team][Index].Number;i++) //меняем данные в паспортах юнитов
       {
       Groups[Team][Index].Units[i].PUnit->GroupIndex = Index;
       Groups[Team][Index].Units[i].PUnit->GroupPOWER = Groups[Team][GroupsNumber[Team]-1].POWER;
       Groups[Team][Index].Mission.OurGroup = &Groups[Team][Index];
       }
       //и в "паспорте" самой группы надо все обновить
      Groups[Team][Index].Index=Index;
     }*/

     //некорректная часть функции
     /*
       for (int i = 0;i<Groups[Team][GroupsNumber[Team]-1].Number;i++)//стираем последнюю группу(юнитов)
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
     //для этой ф-и надо реалезовать
     //поле OnMission
     //возможность получения в качестве врага одного юнита
     //еще одну ф-ю TGGroup::GetEnemy(TGObject Enemy)
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
        for (int i=0;ANGLE_MATRIX[Team][Unit->Index][i].Type >= 0;i++) //находим самого сильного и самого слабого
            {
            if (i>=(NUMBER_OF_UNITS*NUMBER_OF_TEAMS)) break;
            TGObject *EUnit = &GObject[ANGLE_MATRIX[Team][Unit->Index][i].Type][ANGLE_MATRIX[Team][Unit->Index][i].Index];
            if ((EUnit->TeamIndex == Team)||(!EUnit->Is)) continue;

            //анализируем силу врага, а если он состоит в группе, то силу группы, в которой он состоит.
               if (!EUnit->InGroup) //если юнит НЕ В ГРУППЕ
               {
               if (EUnit->POWER > MaxPower.value)
                  {MaxPower.value = EUnit->POWER;
                   MaxPower.EUnit = EUnit;}

               if ((EUnit->POWER < MinPower.value) || (MinPower.value==-1))
                  {MinPower.value = EUnit->POWER;
                   MinPower.EUnit = EUnit;}

               }
                  else             //если юнит В ГРУППЕ
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
         
            //натравляем группу на выбранного врага
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
//                 В Ы С Ш Е Е     К О М А Н Д О В А Н И Е
//------------------------------------------------------------------------------
TMax GetMax(int X1,int X2,int Y1,int Y2,int Team,TGSociety::TKvadrats KV[NUMBER_OF_KVADRATS][NUMBER_OF_KVADRATS])
    {                                             //суть этой ф-ии такова находит самый: концентрированный квадрат.
                                                 // путем рекурсивного разбиения карты на 4-инки
                                                //а концентрация-отношение кол-ва юнитов к площади квадрата карты, котороая измеряется в инимальных квадратах.
    int DeltaX = X2-X1,
        DeltaY = Y2-Y1;
    TMax Max;
    Max.value = 0;
    Max.index = -1;
    Max.index2 = -1;

    if ((DeltaX >= (KVADRAT_WIDTH*2))&&(DeltaY >=(KVADRAT_HEIGHT*2))) // возвращаем координаты центра квадрата
        // из 4-х "4-тинок" с максимальной концентрацией и саму концентрацию
        {
        TMax Max_Kv[4];
        Max_Kv[0] = GetMax(X1, round(DeltaX/2)+X1 ,Y1 , round(DeltaY/2)+Y1,Team,KV); //квадрат 1;1
        Max_Kv[1] = GetMax(X1+round(DeltaX/2),X2 , Y1 , round(DeltaY/2)+Y1,Team,KV);  //квадрат 2;1
        Max_Kv[2] = GetMax(X1, round(DeltaX/2)+X1 ,Y1+round(DeltaY/2), Y2,Team,KV);//квадрат 1;2
        Max_Kv[3] = GetMax(X1+round(DeltaX/2),X2 , Y1+round(DeltaY/2), Y2,Team,KV); //квадрат 2;2
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
//            if (Max.index2 == -1) ShowMessage("Ошибка в ф-и GetMax!");
            return Max;


            }


    }

TMax TGSociety::GetEnemyCentre(int Team)
     {//поиск врага, если врага не найден в качестве его координа возвращаются "-1";
     TMax MAX,ret;
     MAX.index = -1;
     MAX.index2 = -1;
     MAX.value = 0;
     if (Teams[Team].Number==0)return MAX;
     for (int t = 0;t<NUMBER_OF_TEAMS;t++)// из всех команд выбираем сильнейшую
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
              (i<=Teams[Team].Number)//"<=" а не "<" для того, что его надо поставить в первую пустую ячейку
              &&         //
              (i<NUMBER_OF_UNITS)//но максимальное число юнитов превышать ни-ни.
              &&
              (!Result);
              i++
             )
             {
             if (!GObject[Team][i].Is)//как только находим пустую ячейку, то кладем туда юнита.
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
                       GObject[OtherTeam][OtherIndex].GetGlobalPos(2,(rand()/1000),AI);//!!!надо проверить существует ли этот объект!!
                       }
                       {bool PutUnit_ = PutUnit(Team,i,Team,X,Y);
                       while(!PutUnit_) //пытаемся положить юнита
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
     if((X==-1)||(Y==-1)) return; //если враг не найден;
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

     // 1 вначале всех выносим из групп
     // 2 потом создаем из них группу
     // 3 а потом отсылаем, куда надо


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
//                       ГЛОБАЛЬНЫЕ ФУНКЦИИ И МЕТОДЫ   
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
// перезапуск
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


     //      --**Перемещение юнитов**--

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
//     --**конец перемещения юнитов**--

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




