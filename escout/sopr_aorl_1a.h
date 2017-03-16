//---------------------------------------------------------------------------
#ifndef sopr_aorl_1aH
#define sopr_aorl_1aH
#include "paketstruct.h"
#include "structs.h"
//---------------------------------------------------------------------------
struct TSoprTrassa
{
   int Head;                 //Признак головы. Голова не содержит никакой информации
   double Xe, Ye;            //Экстрополированное значение координат в км
   double Ae, De;            //Экстраполированное значение азимута и дальности
   double V_Xe, V_Ye;        //Экстраполированное значение скорости в км/ч
   double Xi, Yi;            //Интерполированные значения координат в км
   double V_Xi, V_Yi;        //Экстраполированные значения скорости в км/ч
   double A_X, A_Y;          //Оценка ускорений в км/ч^2 получается
   double Alfa_x, Beta_x;    //Значения Alfa и Beta для кооординат x
   double Alfa_y, Beta_y;    //Значения Alfa и Beta для кооординат y
   long Nomer;               //номер борта
   long Rejim;               //0 - ПК, 1 - УВД, 2 - RBS
   long NomerSektora;        //Номер сектора от 0 до
   TMyDateTime DateTime;      //Время локации последнего привязанного отсчета
   TMyDateTime DateTimeSrav;  //  Последний отсчет, который сравнивался
   double Koef;               //Последний коэффициент
   TPaketData *LastPD;        //Последний добавленный пакет данных
   long Predvaritelno;        //Предварительная ли завязка
   unsigned long NomerTrassy;

   struct TSoprTrassa *Next; //Указывает на следующий элеиент списка трасс
   struct TSoprTrassa *Prev; //Указывает на предыдущий элемент списка трасс
//Получение весового коэффициента
//Если меньше нуля то надо уже удалить эту трассу. Если больше 100000 то даже и не рассматривать,
//а в остальном как оценивать кооэфициенты и сравнивать
   double GetVesKoef(TPaketData &PD, initialization &_init, double TO);

//Установить последние данные для трассы
   int SetDataTrassy(double _Koef, TPaketData &PD, initialization &_init, double TO);


//Процедуры для отладки - сохранение данных в файл
   int SaveDataToFileDebug(FILE *fp);


};

class TSoprTrassaList
{
   double TO;   //темп обзора в часах
   initialization init;
   TSoprTrassa *FirstTrassa;
   unsigned long N_of_Trass;

   bool ZavytazPervKanal;   //Сопровождать первичный канал
   int N_of_P;            //Число отметок первичного канала

public:
   TSoprTrassaList(const initialization &_init);
   int Delete(TSoprTrassa *Trassa);
   int ZavyazatTrassu(TPaketData &PD1, TPaketData &PD2);
   int AddOtschet(TPaketData &PD);  //Добавления отсчета
   ~TSoprTrassaList();
 //Процедуры для отладки - сохранение данных в файл
   int SaveDataToFileDebug(FILE *fp, const TPaketData &PD);



};


#endif
