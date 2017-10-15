//---------------------------------------------------------------------------

#ifndef soprForWorkFileH
#define soprForWorkFileH

#include <string>
#include <fstream>


using namespace std;

//---------------------------------------------------------------------------

/*Сохранение файлов*/

struct TSoprSaveSession
{
//Путь к основной папке, где хранятся папки
  string csPutKDannym;
                            //С именами 01, 02, ..., 31

//Префикс имени файла к нему пририсовается двухзначное число
                      //Например plot или plotout или datasort
  string csPrefix;
  string csExt;      //Расширение

  bool bDataOutDOS;   //Формат ДОСа

//Вывод данных
  bool bVyvodAmpl;    //Выводить амлитуду для первичного канала,
  bool bSkorostOut;   //Выводить данные о скоростях
  bool bDopDopInfo;   //Выводить правильную информацию о значениях высоты и номерхо
  bool bSaveDataOtmetka;
  bool bSaveDataSever;
  bool bSaveDataSektor;
  string DopTextOut;   //Дополнительный текст для вывода в



/*Конструктор*/
  TSoprSaveSession()
  {

#ifdef linux
       cRazdelitel='//';
#else
       cRazdelitel='\\';
#endif

    bIsInit=false;
    bSaveDataOtmetka=true;
    bSaveDataSever=true;
    bSaveDataSektor=false;
  };


/*Старт запуска открываются нужные файлы. Не нужные затираюся
  Возвращает: 0 - если уже запущена, -1 - если не существует

пути csPutKDannym. -2 - если не успешно записалось в файл.
1- если успешно был запуск
*/
  int Start(void);


/*Остановка данных. Запись прекращается - выдается сообщение об остановке
Запись производится в текущую папку.
*/
  int Stop(void);

/* Сохранение данных в файл
*/
  int SaveDataToFile(TPaketData &_PaketData);


private:
  bool bIsInit;     //Проверка инициализирован ли данные
  fstream fstrm;    //Дескриптор текущего файла
  char cRazdelitel; //Разделитель каталогов - линукса и юникса
  int iLastHour;    //Последний час
  
};





#endif


