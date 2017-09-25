//---------------------------------------------------------------------------
/*


Класс TAirplaneList подготавливает данные к выводу.
Обзор разбит на 12 равных секторов по 30 градусов. Номера этих секторов заданы от
0 до 11.
TAirplaneList осуществляет ввод данных методами
int AddTarget(const TPaketData &PD, //Пакет с данными
                 int IsTail);  //Нужен ли хвост
и AddNord()

и вывод их в объект типа TDrawObjectList *(см.)
SetDataToDrawObjectList

*/

#ifndef airoplanelistH
#define airoplanelistH
#include "DrawObjectList.h"
#include "paketstruct.h"
#include "probality.h"

//---------------------------------------------------------------------------
//#define SECOND_VARIANT





/*Используется для организпции трасс. Содержит информацию о последних трассах. Используется при удалении трассы, то есть очистки всех
элемтов трассы. Когда новый отсчет добавляется, то в первую очередь просматривается список LastAirTail. Если */
struct TLastAirTail
{
  long iNomerTrassy;    //Номер трассы
  long N_Sect30;        //Номер сектора
  long N_Indx;          //Индекс последнего отсчета в данном секторе
  long IsDelete;        //Не известно надо ли нет
  int   FirstDirectionFormulyars; //Направление для формуляра

//Это скорее не надо чем надо
  long N_of_P;          //Число отметок первичного канала
  long N_of_S;          //Число отметок вторичного канала
  long N_of_PS;         //Число объединенных отметок
};


class TAirplaneList
{
//Параметры области вывода
   POINT pntRLS;          //Координаты РЛС в координатах экрана
   SIZE szArea;           //Размер площади вывода
   double dfKoefMasshtab; //Масштаб сжатия

//Свойства настроек
   initialization init;  //Свойства настроек


//Параметры для каждого сектора в отдельности
   double last_time[12];          //Время секторов - время последнего сектора
   int iMaxSize_apl[12];          //Максимальный размер очереди
   int iSize_apl[12];             //Текущий размер
   airplane **apl[12];            //Массив самолетов  на каждый 30 градусный сектор - содержит данные

   airplane **temp_apl[2];         //Временный массивы, которые постоянно копируются
   int iMaxSize_temp_apl[2];      //Максимальный размер (зарезервированные)
   int iSize_temp_apl[2];         //Текущие размеры
   int iSect30_temp_apl[2];     //Номера секторов промежуточных данных
   int iCur_tempIndex;            //Текущий индекс заполнения. Равен 0 или 1.


   airplane_light *apl_tail[12];       //Массив отсчетов в хвосте
   int iMaxSize_apl_tail[12];          //Максимальный размер очереди
   int iSize_apl_tail[12];             //Текущий размер
   airplane_light *temp_apl_tail[2];   //Временные массивы отсчетов в хвосте 
   int iMaxSize_temp_apl_tail[2];      //Максимальные размеры временных массивов		
   int iSize_temp_apl_tail[2];	       //размеры массивов

//Работа со следодом
   TLastAirTail  *LastAirTail;		//Указывает номер трассы, номер сектора от 0 до 11 для поиска в массиве apl при
					//добавлении новых отсчетов
   long N_of_LastAirTail;		//Сколько элементов в массиве
   long MaxN_of_LastAirTail;		//На сколько элементов заполнен данный массив


   double TO;                 		//Темп обзора (время оборота одной антены)
   int iFirstSect30NoOut;       	//Первый номер сектора, который еще не копировался в текущем обзоре


//Критическая секция для синхронизации ModifySector и SetDataToDrawObjectList
   CRITICAL_SECTION csForModifySwap_temp_apl;


/*Частные методы*/
//Модификация
   int ModifyCircle(double Time, int Sect30);


   int ModifySector(int Sect30);  //Все изменения в 1 секторе c учетом lastTime
                                  //Измение на 1 обзор


//Поменять местами сектор в массиве 
//Параметры: i_tempIndex1 указывает на то, что поменять местами
//массивы apl[iSect30_temp_apl[ i_tempIndex1]] и  temp_apl[ i_tempIndex1],
//apl_tail[iSect30_temp_apl[ i_tempIndex1]] и  temp_apl_tail[ i_tempIndex1].
//То есть данная процедура осуществляет перенос из временного буфера в массив, откуд
   int Swap_temp_apl(int i_tempIndex1);


//Временная переменная
   int SummaDebug;



   int iNewView;   //Новый вид надо заново вывести


//Код бедствия
   int iKodB;
   
//Отсчет который вызвал код бедствия
   airplane Kod_bedstvia_apl;  //Отсчет который вызвал код бедствия



public:
   double dLastRadian;       //Последнее значение градуса в углах
   TAirplaneList(
                POINT &_pntRLS,    //Положение РЛС
                SIZE & _szArea,    //Размер экрана
                initialization & _init, //Структура инициализации
                double _KoefMasshtab); //Коэфициент для перехода от реальным к
                                       //координатным данным

   ~TAirplaneList();


//Добавление нового отсчета в список. Добавление осуществляется в методе TMainCircleOut::AddTarget 
//Параметр: PD - ссылка на объект типа TPaketData
   int AddTarget(
           TPaketData& PD); //Пакет с данными

//Добавление нового севера
   int AddNord(double Time);


   int PaintData(HDC DC);  //Вывод данных на дескриптер устройства


//Изменение масштаба, положения координат или размера экрана - пересчет всех координат
   int SetNewAreaStates(
                POINT &_pntRLS,         //Положение РЛС
                SIZE & _szArea,         //Размер экрана
                initialization &_init,  //Структура инициализации
                double _KoefMasshtab);  //

//Принадлежит ли пиксель какому нить формуляру
   int HaveAnyFormulyarPixel(int Xm, int Ym);

//Нарисовать 1 отсчет и его формуляр, который сверху
   int PaintOpaqueFormulyar(HDC DC, int Xm, int Ym);

//Получить формуляр, на отметке которой был щелчок
 //  TFormulyar * GetFormulyar(int Xm, int Ym);


//Возврашает данные изменилось содержание
   int GetNewView(void)
   {
     return iNewView;
   };


// Найти и повернуть формуляр
   int FindAndRotateFormuluar(int Xp, 
                              int Yp, 
                              int Kuda);



//Есть ли код бедствия
   int GetKodBedstvia(void)  //Признак того, что есть код бедствия
   {
     return iKodB;
   };

//Сброс кода бедствия
   void SbrosKodBekstvia(void)
   {
      iKodB=0;
   };





};

#endif
