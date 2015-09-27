//---------------------------------------------------------------------------

#ifndef soprForWorkFileH
#define soprForWorkFileH
//---------------------------------------------------------------------------
/*Функция ввода данных в строку*/
  long __stdcall  soprSaveToFileString(
            char *Strka,         //Размер строки должен быть не меньше 150 - переполнение
            TPaketData &_PaketData,
            char *DopTextOut,//Дополнительный текст для вывода
            bool bVyvodAmpl, //Выводить амлитуду для первичного канала,
            bool bSkorostOut, //Выводить данные о скоростях
            bool bDopDopInfo,      //Выводить правильную информацию о значениях высоты и номерхо
            int &SizeOfStroka      //Размер строки
                          );




/*Распознавание строки из файла*/
  long __stdcall  soprGetFromStringToPaketData
                (
                char *Strka,
                TMyDateTime *LastDateTime,  //Последний север (м.б. равен 0)
                TPaketData &_PaketData
                );




/*Сохранение файлов*/


struct TSoprSaveSession
{
  char csPutKDannym[1200];  //Путь к основной папке, где хранятся папки
                            //С именами 01, 02, ..., 31
  char csPrefix[20];  //Префикс имени файла к нему пририсовается двухзначное число
                      //Например plot или plotout или datasort
  char csExt[5];      //Расширение

  bool bDataOutDOS;   //Формат ДОСа

//Вывод данных
  bool bVyvodAmpl;    //Выводить амлитуду для первичного канала,
  bool bSkorostOut;   //Выводить данные о скоростях
  bool bDopDopInfo;   //Выводить правильную информацию о значениях высоты и номерхо
  bool bSaveDataOtmetka;
  bool bSaveDataSever;
  bool bSaveDataSektor;
  char DopTextOut[200];   //Дополнительный текст для вывода

/*Указатель на функцию которая получает строки определенного формата по данным*/
   long __stdcall  (*DataToStr)(
            char *Strka,         //Размер строки должен быть не меньше 150 - переполнение
            TPaketData &_PaketData,
            char *DopTextOut,   //Дополнительный текст для вывода
            bool bVyvodAmpl,    //Выводить амлитуду для первичного канала,
            bool bSkorostOut,   //Выводить данные о скоростях
            bool bDopDopInfo,
            int &SizeOfStroka      //Размер строки
         );  //Выводить правильную информацию о значениях высоты и номерхо



/*Указатель на получение данных*/
   long __stdcall  (*DataFromStr)
                (
                char *Strka,            //Строка
                TMyDateTime *LastDateTime,  //Последний север (м.б. равен 0)
                TPaketData &_PaketData
                );



/*Конструктор*/
  TSoprSaveSession()
  {
    lFD=-1;
    fOpen=0;
    DataToStr=soprSaveToFileString;
    DataFromStr=soprGetFromStringToPaketData;
    csPrefix[0]='o';csPrefix[1]=0;
    csExt[0]='t'; csExt[1]=0;
    DopTextOut[0]=0;
#ifdef linux
       cRazdelitel='//';
#else
       cRazdelitel='\\';
#endif

    bIsInit=false;
    bIsInitLoad=false;
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



/*Загрузка данных из файла*/
//Начало процесса загрузка данных из файла
//  Функция возвращает 1, если всё  ОК.
//0, если уже запущены загрузка или запись в файл
//-1, уже кончилось время

  int StartLoad(char *csFileNamePath, //Путь к каталогу или файлу
                TMyDateTime *FirstTime, //FirstTime - время откуда считывать данные из файла
                                        //Если равен нулю, начальное время
                int iType,       //Если Type=1, то FileNamePath - это путь к файлам.
                                 //             файлы лежат в папке FileNamePath.
                                 //Если Type=2, то FileNamePath - это полный путь с именем
                                 //             файла
                                 //В остальных случаях берутся данные из папки
                                 //csPutKDannym
                TPaketData *PDO);   //Данные из файла


//Считать данные из файла
//Возвращает 1 в случае успеха, 0 в случае не успеха.
//Данная процедура вызывается только после успешного выполнения
//StartLoad. Если процедура вызывается с не успехом в данном файле больше нет
//данных
  int GetDataFromFile(TPaketData *PDO);



  int StopLoad(void);


private:
  bool bIsInit;     //Проверка инициализирован ли данные
  bool bIsInitLoad; //Начало загрузки из файла
  long lFD;                 //Дискрипторы текущего файла
  FILE *fOpen;              //Дескриптер файла

  char cRazdelitel; //Разделитель каталогов
  int iLastHour;    //Последний час

  int iTypeLoad;   //Загрузка данных

  char csFileNamePath1[1200];

  TMyDateTime LastDateTime;     //Последний введенный отсчет


  TMyDateTime FirstDateTime;
  TMyDateTime LastFirstDateTime; //Последнее время начала
                                 //Обновляется после каждого удачного чтения

  TMyDateTime DTForEveryHour[32][24];  //Время и дата для каждого часа
  TMyDateTime *pTempMyDateTime;        //Временный массив
  int FirstDen, FirstChas;             //Первый день и первый час


//Найти и открыть файл, где есть данные которые не раньше текущего времени
//возвращает описатель типа FILE. Файл открыт в соответвии с наличием количества
//разделителем между строками
  FILE *GetDataDirForFirstTime(char *TekPapka,
                            TMyDateTime *FirstTime,
                            int iType,         //1 - по пути,
                                             //0 - по папкам разбитым по датам
                            TPaketData *PDO   //Масив с данными который заполняется
                           );

  FILE *GetDataFileForFirstTime(char *TekPapka,
                            TMyDateTime *FirstTime,
                            TPaketData *PDO   //Масив с данными который заполняется
                           );

/*privGetDataFromFile - функция чтения данных из файла*/
   int privGetDataFromFile(TPaketData *PDO);


/*Получение данных о времени*/
   int GetDTForEveryHour(char *TekPapka, //Полный путь
                         int iType);     //типа отсчета

};





#endif


