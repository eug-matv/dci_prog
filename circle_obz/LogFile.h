/*Автор: Матвеенко Е.А.  ЧРЗ Полет ОКБ НТЦ КБ-29

*/
//---------------------------------------------------------------------------

#ifndef LogFileH
#define LogFileH
#include <stdlib.h>
#include "structs.h"

//---------------------------------------------------------------------------
class LOGFILE_IKO
{
  static LOGFILE_IKO *LogFile;   //Файл который


  char PathLogFileName[1000]; //Путь к лог-файлу
  char FileName[20];
  initialization init;
  FILE *fp;
  int LastSec,LastMin, LastHour;  //Дата

  int LastDay,LastMonth, LastYear;  //Дата
  int GetDate(int *Y, int *M, int *D);

//Промежуточный функ для вывода данных
  int DataOut(int IsData, char *Text);

  //Проверка, а не надо ли запустить другой отсчет
  int NewDay(int Year, int Month, int Day, int H, int M, int S); //Переход на следующий день


//Критическая секция
  CRITICAL_SECTION CS;


public:
//Возвращает 1, если успешно
  static int Init_StaticLog(
    char *FN,               //Имя файла с расширением, которое будет располагаться по дате
    initialization &_init); //Структура инициализации

  static int Init_StaticFree(void);



  static LOGFILE_IKO* GetStaticLog(void);

  LOGFILE_IKO();
  ~LOGFILE_IKO();



//Запустить программу
  int Start(
    char *FN,              //Имя файла с расширением, которое будет располагаться по дате
    initialization &_init, //Структура инициализации
    int View_init          //Выводить ли данные о структуре
  );

  int Stop();       //Остановка работы



//Вывести данные файла инициализации
  int Init_Out(void);


//Вывести данные об ошибке
  int OutCriticalError(
                 char *_TextIskl,
                 char *_ModuleName,
                 char *_ClassName,
                 char *_MethodName,
                 char *_RegExName); //Сообщение об критической ошибки - ошибки приводящей к закрытию программы


//Системное сообщение об ошибке
  int OutCriticalError(
                char *ClassName,
                char *Message);

//Вывести рабочую информацию. Информацию о сигналах бедствий от борта.
//и прочее
  int OutWorkInfo(char *Mess);    //Информация о выводе текущей обстановке

//Информация о некритической ошибке
  int OutNotCriticalError(char *Mess);
                                     

//Информация о смене важных опций - отключение сопровождений и прочее.
  int OutChangeOption(const initialization &_init);


//Проверка, которая должна происходить каждые 5-10 секунд
//Перехода на следующие сутки
  int TestNextDay(void);

};

#endif
