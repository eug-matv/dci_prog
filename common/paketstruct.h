/*
  Автор: Матвеенко Е.А. КБ-29
Участие в программах:  KOI_AORL_1AC.exe, вероятно Analisis.exe
Дата: 10.09.2007 Модификация: 28.11.2007
В данном модуле описаны структуры
1) TMyDateTime - для описания времени,
2) TPaketData - для описания пакета, полученного с COM-порта, файла данных
3) Все вспомогательные классы и структуры
  Описание элементов структур данно в комментариях к описным структурам ниже

  Определена очередь TPaketData::PaketData через массив. Размер массива N_OF_FIFO_PAK_STR
  TPaketData::InIndex - указывает индекс элемент который должен быть записан
  Должен каждый раз изменяться (TPaketData::InIndex+1)%N_OF_FIFO_PAK_STR

  TPaketData::OutIndex - указывает индекс элемент который должен быть считан.
  Должен каждый раз изменяться (TPaketData::OutIndex+1)%N_OF_FIFO_PAK_STR




*/


//---------------------------------------------------------------------------


#ifndef PaketStructH
#define PaketStructH

//---------------------------------------------------------------------------
#define N_OF_FIFO_PAK_STR 1000



/*Описание типа пакета*/

typedef enum
{
   TipPakOtmetka=0,   //Если тип пакета отметка
   TipPakSektor=1,    //Если тип пакета сектор
   TipPakSever=2,      //Если тип пакета север
   TipPakNeOpredelen=3      //Если тип пакета север
} TTipPaketa;



typedef enum
{
    RejimNeIzvesten=0,   //Не известный режим
    RejimUVD=1,          //Режим УВД
    RejimRBS=2          //Режим RBS
} TRejimRaboty;


/*Канал по которму был получен данный отсчет*/
typedef enum
{
    Kanal_P=0,      //Первичный канал
    Kanal_S=1,      //Вторичный канал
    Kanal_PS=2      //Первичный и вторичный каналы
} TKanal;



struct TMyDateTime
{
   short God, Mesyac, Den;   //Год месяц день
   double Time;              //Время в часах от 0 до 24 не включая

   int operator <(TMyDateTime mdt);
   int operator >(TMyDateTime mdt);
   int operator <=(TMyDateTime mdt);
   int operator >=(TMyDateTime mdt);

   int operator ==(TMyDateTime mdt);
   int operator !=(TMyDateTime mdt);
   TMyDateTime& operator =(const TMyDateTime &mdt);
   TMyDateTime& operator =(TMyDateTime mdt);


   TMyDateTime operator +(double dTime);
   TMyDateTime operator -(double dTime);


};



  struct TIshodDannye
  {
    long FileNumber;    //Номер файла - если данные из файла

    long Smesh;
  };



struct TPaketDataOtmetka
{

//Для ПК все false //Для RBS               //Для УВД
  bool bKodN;      //Режим 3А - байт 5_5   байт 5_3
  bool bKodH;      //Режим С - байт 5_4    байт 5_2



  TKanal Kanal;
                     //байт 7. биты 1 и 2 (<Байт7>&0x3)
                     //Так же проверять биты 5,6 байта 8, так как они согласованы
                     //то есть  (7,1)=(8,5) и (7,2)=(8,6)

  TRejimRaboty Rej;   //Режим работы RejimUVD (УВД), RejimRBS(RBS)
                      //Биты:

  bool bP_LCHM;        //Режим ЛЧМ для первичного канала (для вторичного игнор)
  bool bP_Mono;        //Режим моно для первичного канала (для вторичного игнор)
                       //Биты пока не известны

  bool bKO;           //Контрольная отметка (для УВД и RBS)
                      //байт 8, бит 2

  short sKodB;  //Код бедствия, Для RBS значения: 0(нет), 7500, 7600,7700
                //байт 9 - биты 6 и 7.
//                Для УВД байт 20 бит 7. Если бит установлен то sKodB=1 (значит бедствие)



  long X,Y;     //Деккартовы координаты. Xb - байт  10,11 со знаком.
                //Yb - байт 12, 13 со знаком. X=Xb*15, Y=Yb*15
//Про номер
  bool bNomer;   //Достоверный ли номер байт 16, бит 8 (для УВД и RBS)
  long lNomer;   //Номер борта. Только для вторичного канала
                 //RBS: ((16,4)*4+(16,3)*2+(16,2))*1000+
                 //     ((16,1)*4+(17,8)*2+(17,7))*100+
                 //     ((17,6)*4+(17,5)*2+(17,4))*10+
                 //     (17,3)*4+(17,2)*2+(17,1)

                 //УВД: A, B, C, D, E должны в двоичном лежать от 0000 до 1001
                 //если
//      (проверка<9) ((16,4)*8+(16,3)*4+(16,2)*2+(16,1))*10000+
//      (проверка<9) ((17,8)*8+(17,7)*4+(17,6)*2+(17,5))*1000+
//      (проверка<9) ((17,4)*8+(17,3)*4+(17,2)*2+(17,1))*100+
//      (проверка<9) ((18,8)*8+(18,7)*4+(18,6)*2+(18,5))*10+
//      (проверка<9) (18,4)*8+(18,3)*4+(18,2)*2+(18,1)




//Про высоту
  bool bVysota;     //Достоверность высоты
                    //Для УВД = true. Для RBS байт 18 бит 8
  bool bAbsVysota;  //Признак высоты (ПрH)
                    //Для RBS всегда true, для УВД байт 20 бит 8

  long lVysota;      //Значение высоты
                    //RBS: старшие разряды байт - 18, биты 1 - 6(6 -знаковый)
                    //младшие разряды байт 19 биты 1-8
                    //УВД: старшие разряды байт - 20, биты 1 - 6(6 -знаковый)
                    //младщие разряды байт 21

  bool bOstatokTopliva; //Только для УВД. Байт 19. Бит
  long lOstatokTopliva; //Остаток топлива  - только для УВД,
                       //Для RBS = 0.
                       //Для УВД байт 19 - биты 1 - 4


//Дорастчитываемые данные
  double Dalnost;      //Дальность в км. Расчитывается по X,Y
  double Vx, Vy;         //Координаты скорости проекции скоростей
                         //в км/ч



//Дополнительные данные
  double dAmpl;      //Значение амплитуды для первичного канала

//Определенный номер борта
  long lDopNumber, lDopVysota;  //Правильные значения номера и высоты

  long  lNomerTrassy;  //Номер трассы

  short IndexInObzor;         //Индекс отсчета в обзоре


//Данные введены 22.11.2007
  int iTail;      //Нужен ли хвост
  int iFormulyar; //Нужен ли формуляр
  void *apl;     //Указатель на источник, куда скопировалась данное
                 //вводится в методе

//Добавлена 20.01.2008
  long PrivyazanPredvaritelno;


//Для вывода вероятности в файл
  long N_of_PO;        //Число отметок первичного канала
  long N_of_MustPO;    //Число отметок первичного канала которые должны быть 
  double P_PO;
  long N_of_SO;        //Число отметок вторичного канала
  long N_of_MustSO;    //Число отметок вторичного канала которое должно быть
  double P_SO;

};


/*Пакет сектор*/
struct TPaketSektor
{


  int Dummy;



};



struct TPaketSever
{

  int Soprovojdenie;   //Указывает на то, что сопровождение было включено. 
                       // исключительно ДЛЯ вывода

};



struct TPaketData;

//Описание пакета
struct TPaketData
{


  static TPaketData PaketData[N_OF_FIFO_PAK_STR];   //Размер массива данных
  static long InIndex;                 //Входной индекс - изменяется при вводе
  static long OutIndex;                //Выходной индекс
                                       //По секторам

  TTipPaketa TipPaketa;  //Тип пакета. Равен TipPakOtmetka,
                         //                  TipPakSektor или
                         //                  TipPakSever

//Данные
  union
  {
     TPaketDataOtmetka Otmetka;   //Отметка - описание выше.
                               //Анализируется если TipPaketa=TipPakOtmetka

     TPaketSektor Sektor;         //Сектор - описание выше
                               //Анализируется если TipPaketa=TipPakSektor

     TPaketSever  Sever;          //Север- описание выше
                               //Анализируется если TipPaketa=TipPakSever
  };

//Общие данные для всех - Время
  TMyDateTime  DateTime;  //Дата и время - описание выше
  unsigned char cKodStrany;      //Код страны байт 6
  double Azimut;                 //Азимут
  short NomerSektoraRazmerom30;  //Номер сектора размером
                                 //изменяется от 0 до 11
  TIshodDannye IshodDannye;   //Исходные данные - указывает откуда получены данные


};



//Заново инициализировать данные
void ReinitializePaketData(void);


#endif
