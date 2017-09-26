//---------------------------------------------------------------------------
/*Матвеенко Е.А  ЧРЗ ПОЛЕТ ОКБ НТЦ КБ-29
  TMainCircleOut.
   Главный класс настройки области вывода. Обеспечивает:
   1)  вывод данных наэкран
   2) измеение массштаба
   3) Прорисовка сетки и текста сетки

   Методика работы с данным классом:
   1) Запустить его методом Start
   2) После нажатия метода в текущем потоке по таймеру или во вторичном добавлять
   методом AddTarget
   3) Вывод методом Paint
   4) Установка новых свойств методом SetNewOption.
   5) Измененение масштаба методом SetNewSizeAndMasshtab,
   а так же использование методов:
     SetNewSize,   SetVnutriRamki, MultyScale, MoveRLS
   6) Обработка событий движения мыщи
MouseButtonDown, MouseButtonDowned,MouseButtonUp.
   7) Вызов проверки кода бедствия командой GetKodBedstvia, данная процедура вызывается по таймеру из главного окна программы, она осуществляет.  Сброс кода бедствия SbrosKodBedstvia. 
   8) Остановка работы и освобождение памяти Stop.
 */


#ifndef main_circleH
#define main_circleH

#include <windows.h>
#include "DrawObjectList.h"
#include "structs.h"
#include "airoplanelist.h"
#include "paketstruct.h"



class TMainCircleOut
{
  int IsWork;          //Данный объект инициализирован и находится в работе
  POINT pntRLS;        //Положение RLS на экране

  SIZE szArea1;         //Размер области вывода внешний
  SIZE szArea;          //Размер области вывода внутренний
  double dfMasshtab;   //Масштаб экрана
  initialization init; //структура инициализации structs.h
  TAirplaneList *AL;   //Список отсчетов борта (airoplanelist.h)



//Обновляется редко - только при свойств сетки
  HDC DC_Setka;     //Дескриптор для вывода сетки
                    //Обновляется сразу после обновления масштаба
  HBITMAP bSetka,OldbSetka;   //Битовые поля где выводится сетка карта и прочее
                    //меняется только если меняется масштаб или свойства вывода

  HDC DCPreOutput;    //Дескриптер файла

  HBITMAP bPreOutput,bOldPreOutput;  //Битовые поля для промежуточного вывода сетки

  int IsSetka;      //Есть ли сетка

//Для вывода данных но без указателя развертки и рамки выделения
  HBITMAP bScreenBack, bOldScreenBack;
  HDC DC_ScreenBack;



//Шаг на который изменяются колца дальности - вычисляется в зависимости от масштаба
  int iShagDalnosti;
  int iOpornShagDalnosti;




  TDrawObjectList *DOL_rect;
                   //Для вывода рамки - когда меняется рамка

//Для вывода масштаба
  stack_mashtab smPrev[20];   //Масштабы
  int N_of_smPrev;            //Число элементов


//Рамка - рисование рамки
  int iPaintSelected;           //Начато рисование рамки
  int X_mouse1,Y_mouse1;      //Координаты начального угла
  int X_mouse2,Y_mouse2;      //Координаты конченчного угла


  long TimeOfLastAddTarget;   //Время добавления последнего отсчета


  long DopustimPeriodV_ms;   //Допустимый период в миллисекундах


  int N_of_PD_Bedstvie;      //Есть бедствие  
                             //Если есть бедствие, то оно равно
			     //числу бедствий
                             //иначе равна 0 
                             //Устанавливается методом AddTarget,
                             //Если новый элемент с бедствием
     	  		     //иначе его значение равно 0
	                     //Рисуется бедственные борты методом 
                             //PaintBedstvie поверх области отметки 
			     //с бедствием. 
                             //сбрасывается методом ResetBedstvie

  long flashBedstvie;	     //мерцание отметки бедствия. Если 0, то 
			     //краcный цвет, иначе - обратный ему
        
  TPaketData PD_Bedstvie[10];//Пакет данных бедствие
			     //Число бедствий равно N_of_PD_Bedstvie					

  char timeOutStr[100];      //Строка с отображением такущего врмени 

  bool bTimeOutVisible;      //Отображать ли время и дату

  int MakeAndPaintSetka(HWND hwnd);

//Сохранение масштаба
  int SaveMassharb_in_smPrev(void);



//Процедура для преобразования координат
  int ScreenToWorld(int Xs, int Ys,  //Откуда
                    double &Xw, double &Yw);

//Процедура для преобразования координат
  int WorldToScreen(double Xw, double Yw, //Откуда
                    int &Xs,   int &Ys);


//Нарисовать текст для окружностей
  int PaintTextCircle(HDC DC, double Radius);  //Радиус окружности в км


//Нарисовать текст для линий
  int PaintTextLine(HDC DC, double Ugol);  //Тут данны координаты уже в пикселях


//Рисовать текст для сетки
  int PaintSetkaText(HDC DC);

//Рисовать линию разветки
  int PaintRazvertka(HDC DC,  double Azmt_Radian, int Left, int Top);


//Рисовать текст об ошибке прямо на экране красным цветом
  int PaintTextError(HDC DC, char *csTextError);


public:
   int NoPackets;       //1 - Признак того, что целый период не было пакетов
                        //2  - ошибка открытия порта -- порт не существует
                        //3  - ошибка открытия порта - порт занят
   int MustRestarting;  //Нужен перезапуск  - это для внешней процедуры,
  // что надо запустить




   TMainCircleOut():IsWork(0),bSetka(NULL),
        bTimeOutVisible(true){
        timeOutStr[0] = 0;
   };
   ~TMainCircleOut(){Stop();};
   int GetWorking()
   {
     return IsWork;
   };

//Запуск вывода данных
   int Start(
    HWND hwnd,            //Дескриптер окна
    const SIZE &_szArea,         //Размер области вывода
    const initialization &_init //свойства отображаения
            );

//Перезапуск
   int Restart(void);


   int Stop(void); //Остановить работу

//Проверка работы
   int TestPeriod(void);


//Изменение размеров области или изменение коэффициента
  int SetNewSizeAndMasshtab(
              POINT _pntRLS,         //Положение РЛС
              SIZE  _szArea,         //Размер области вывода
              double _Masshtab,      //Масштаб
              int Backup             //Режим Backup сохранения
                           );


//Установить размер области
  int SetNewSize(SIZE  _szArea);  //Размеры области


//Установка новых опцый
  int SetNewOption(initialization &_init);



//Вывод данных на экран
  int Paint(HWND hwnd,   //Дескриптер устройства - на экране
            int left, int top); //Верхний левый угол




//Move Down
  int MouseButtonDown(int Xm,int Ym,
                    unsigned long Key);   // 0 - не нажата кнопка
                                // 1 - левая кнопка
                                // 2 - правая кнопка
                                // 3  - не нажата кнопка

  int MouseButtonDowned(int Xm,int Ym,
                    unsigned long Key);   // 0 - не нажата кнопка
                                // 1 - левая кнопка
                                // 2 - правая кнопка
                                // 3  - не нажата кнопка

  int MouseButtonUp(int Xm, int Ym,
                       unsigned long Key);   // 0 - не нажата кнопка
                                // 1 - левая кнопка
                                // 2 - правая кнопка
                                // 3  - не нажата кнопка

//Установить масштаб внутри координат
  int SetVnutriRamki(int X11, int Y11,
                     int X22, int Y22);

//Установка нового коэффициента - то есть изменение коэффициента на значение
  int MultyScale(double Koef);


//Смещение всего отображаемой области вверх или вниз
  int MoveRLS(double K_Move_X,  //Смещение по координатам, >0 - RLS вправо,  <0 - RLS влево на
              double K_Move_Y); //Смещение по координатам, >0 - RLS вверх, <0 - RLS вниз

//Добавление нового отсчета
   int AddTarget(
           TPaketData& PD //Пакет с данными
                );

//Получить координаты РЛС и диапазон координат
//в системе экрана монитора (в пикселях)
   int GetScreenScaleState(POINT &_pntRLS,      //Координаты РЛС
                           int &_Left, int &_Top,      //Левая верхня точка
                           int &_Right, int &_Bottom); //Нижняя нижняя точка

//Получить координаты РЛС и диапазон координат
//в системе экрана монитора (в пикселях)
   int SetScreenScaleState(const POINT &_pntRLS); //РЛС

//Восстановление массштаба
  int RestoreMasshtab_from_smPrev(void);


//Полный массштаб
  int FullScale(void);


//Получение информации о коде бедствия
  int PaintBedstvie(HWND hwnd,  //Дескриптер устройства
          int left, int top);	 //Левый верхний угол	
   
//Сбросить код бедствия
   void ResetBedstvia(void)
   {
      N_of_PD_Bedstvie=0;
   }


//Устаноквка строки со временем и датой
   void SetTimeOutStr(char *str)
   {
        strcpy(timeOutStr,str);
   }

//Время отображается?
   bool GetTimeOutVisisble()
   {
        return bTimeOutVisible;
   }


   void SetTimeOutVisible(bool timeOut)
   {
       bTimeOutVisible = timeOut;
   }

};





//---------------------------------------------------------------------------
#endif
