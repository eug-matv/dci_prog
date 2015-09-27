//---------------------------------------------------------------------------


#pragma hdrstop

#include <dir.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <io.h>
#ifdef linux
#include <sys\stat.h>
#else
#include <sys/stat.h>
#endif

#include "LogFile.h"
#include "win_str_dir_utils.h"



//---------------------------------------------------------------------------

#pragma package(smart_init)
LOGFILE_IKO* LOGFILE_IKO::LogFile=NULL;


int LOGFILE_IKO::Init_StaticLog(
    char *FN,               //Имя файла с расширением, которое будет располагаться по дате
    initialization &_init) //Структура инициализации
{
      int Ret;
      if(LogFile)return 0;
      LogFile=new LOGFILE_IKO;
      if(!LogFile)return 0;
      Ret=LogFile->Start(FN,_init,1);
      if(Ret!=1)
      {
//Ошибка вывода
         delete LogFile;
         LogFile=NULL;
         return Ret;
      }
      LogFile->Init_Out();  //Сохраним данные об Ini в файл
      return 1;
}


int LOGFILE_IKO::Init_StaticFree(void)
{
   if(!LogFile)return 0;
   delete LogFile;
   LogFile=NULL;
   return 1;
}


LOGFILE_IKO* LOGFILE_IKO::GetStaticLog(void)
{
   return LogFile;
}


LOGFILE_IKO::LOGFILE_IKO():fp(NULL)
{
   InitializeCriticalSection(&CS);
}


LOGFILE_IKO::~LOGFILE_IKO()
{
   Stop();
   DeleteCriticalSection(&CS);
}




int LOGFILE_IKO::Start(
            char *FN,    //Имя файла
            initialization &_init,
            int View_init
            )  //Структура инициализации
{
    time_t cur_time;
    struct tm *cur_tm;
    char csDatePapka[1000];
    int NewFile=0;
    int Y,M,D;
    int Ret;
    Stop();

    EnterCriticalSection(&CS);
//1 - скопируем имя и файла и путь
    strcpy(FileName, FN);

//Проверим существование папки


//Скопируем init
    memcpy(&init,&_init,sizeof(initialization));

//Проверка существования папки init.csFullPathToData
   if(access(init.csFullPathToData,00))
   {
//Ощибка - не существует заданной папки
      if(winMakePath(init.csFullPathToData))
      {
        Ret=access(init.csFullPathToData,00);
        if(Ret)
        {
          LeaveCriticalSection(&CS);
          return (-1);
        }  
      }else{
        LeaveCriticalSection(&CS);
        return (-1);
      }
   }


//Определить текущую дату
    time(&cur_time);
    cur_tm=localtime(&cur_time);


//Сформируем папку
    sprintf(csDatePapka, "%s\\%02d",
      init.csFullPathToData, cur_tm->tm_mday);

//Проверка существования PathLogFileFileName
   if(access(csDatePapka,00))
   {
//Ощибка - не существует заданной папки
//Попытаемся создать данный каталог
     if(mkdir(csDatePapka))
     {
//Ошибка - не смогли создать папку
        LeaveCriticalSection(&CS);
        return (-2);
     }
   }

//Проверка существования файла
   sprintf(PathLogFileName,"%s\\%s",csDatePapka,FileName);


//Проверим а существует ли файл и узнаем дату создания
   NewFile=GetDate(&Y,&M,&D);
   if(NewFile)
   {
      if(Y!=cur_tm->tm_year+1900||
         M!=cur_tm->tm_mon+1||
         D!=cur_tm->tm_mday)
      {
         NewFile=1;
      }else{
         NewFile=0;
      }
   }


   if(NewFile)
   {
      fp=fopen(PathLogFileName,"wt");
      if(!fp)
      {
//Ошибка создания файла
         LeaveCriticalSection(&CS);
         return -3;
      }
   }else{
      fp=fopen(PathLogFileName,"at");
      if(!fp)
      {
//Ошибка создания файла
         LeaveCriticalSection(&CS);
         return -4;
      }
   }

//Вывести в файл сообщение о том, что стартовал протокол
   fprintf(fp,
     ">>>   Start log : Time  %02d:%02d:%02d   Date %02d.%02d.%04d\n",  
    cur_tm->tm_hour,cur_tm->tm_min, cur_tm->tm_sec,
    cur_tm->tm_mday, cur_tm->tm_mon+1, cur_tm->tm_year+1900);
   LastDay=cur_tm->tm_mday;
   LastMonth=cur_tm->tm_mon+1;
   LastYear=cur_tm->tm_year+1900;
   LeaveCriticalSection(&CS);

   return 1;
}


int LOGFILE_IKO::Stop()
{
   time_t cur_time;
   struct tm *cur_tm;
   EnterCriticalSection(&CS);

   time(&cur_time);
   cur_tm=localtime(&cur_time);
   if(fp)
   {
      fprintf(fp,">>>   End log : Time  %02d:%02d:%02d   Date %02d.%02d.%04d\n",
  cur_tm->tm_hour,cur_tm->tm_min, cur_tm->tm_sec,
  cur_tm->tm_mday, cur_tm->tm_mon+1, cur_tm->tm_year+1900);
      fprintf(fp,"-----------------------------------------------------------------\n\n\n");
      fclose(fp);
      fp=NULL;
   }
   LeaveCriticalSection(&CS);
   return 1;
}




int LOGFILE_IKO::NewDay(int Year, int Month, int Day,
                        int H, int M, int S) //Переход на следующий день
{
  int Ret;
  FILE *fpVrem;   //Временный дескриптер файла
  char csFullFileName[1500];  //Полное имя к файлу
  char csDateFileName[1500];  //Полное имя
  if(!fp)return 0;
//  EnterCriticalSection(&CS);

  if(Day!=LastDay||Month!=LastMonth||Year!=LastYear)
  {
//Надо закрыть файл
//Надо завершить и закрыть файл
      fprintf(fp,
  ">>>Последний  отсчет  : время  %02d:%02d:%02d   дата %02d.%02d.%04d",
     23,59,59,LastDay,LastMonth,LastYear);

//Сформируем путь к каталогу
     sprintf(csDateFileName,"%s%\\%02d",
       PathLogFileName, Day);
     if(access(csDateFileName,00))
     {
//Нету попробуем создать папку
        Ret=mkdir(csDateFileName);
        if(Ret)
        {
//Ошибка создания файла лог файла
           fclose(fp);
           fp=NULL;
//           LeaveCriticalSection(&CS);
           return 0;
        }
     }

     sprintf(csFullFileName,"%s\\%s",
      csDateFileName,FileName);

     fpVrem=fopen(csFullFileName,"wt");
     if(!fpVrem)
     {
           fclose(fp);
           fp=NULL;
//           LeaveCriticalSection(&CS);
           return 0;
     }
     fprintf(fpVrem,
  ">>>Continued log   : Time  %02d:%02d:%02d   Date %02d.%02d.%04d",
     H,M,S,Day,Month,Year);
     fclose(fp);
     fp=fpVrem;
     LastDay=Day;
     LastMonth=Month;
     LastYear=Year;
  }
//  LeaveCriticalSection(&CS);

  return 1;
}

int LOGFILE_IKO::GetDate(int *Y, int *M, int *D)
{
   int Ret;

   FILE *Vrem_fp;
   char c1,c2;
   if(access(PathLogFileName,00))
   {
//Нечего нет можно создавать
       return 0;
   }
   Vrem_fp=fopen(PathLogFileName,"rt");
   if(!Vrem_fp)return 0;
   Ret=fseek(Vrem_fp,47,0);
   if(Ret)
   {
     fclose(Vrem_fp);
     return 0;
   }

   Ret=fscanf(Vrem_fp,"%d%c%d%c%d", D, &c1, M, &c2, Y);
   fclose(Vrem_fp);
   if(Ret==5)return 1;
   return 0;
}


int LOGFILE_IKO::DataOut(int IsData, char *Text)
{
  FILE *fpTemp;
  time_t cur_time;
  struct tm *cur_tm;
  int Ret;
  if(!fp)return 0;
//  EnterCriticalSection(&CS);
//Определить текущую дату
  if(IsData)
  {
    time(&cur_time);
    cur_tm=localtime(&cur_time);

//Вывести дату
    if(cur_tm->tm_year+1900!=LastYear||
       cur_tm->tm_mon+1!=LastMonth||
       cur_tm->tm_mday!=LastDay)
    {
      Ret=NewDay(cur_tm->tm_year+1900,cur_tm->tm_mon+1,cur_tm->tm_mday,
                 cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec);
      if(Ret!=1||fp==NULL)
      {
         fp=NULL;
         LeaveCriticalSection(&CS);
         return 0;
      }
    }
    fprintf(fp,"%02d:%02d:%02d ",
        cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec);

  }
  fputs(Text,fp);
  fputs("\n",fp); //Вывести перенос строки
//  LeaveCriticalSection(&CS);

  return 1;
}


//Вывести данные файла инициализации
int LOGFILE_IKO::Init_Out(void)
{
//Вывод некоторых настроек
   char Strka[200];
   EnterCriticalSection(&CS);
   if(!fp)return 0;

   DataOut(1, "Options: ");
   sprintf(Strka,"\tScan of rate in seconds: %6.2lf с",init.fTempObzora);
   DataOut(0, Strka);

   if(init.iTail)sprintf(Strka,"\tShow tracks");
   else sprintf(Strka,"\tHide tracks");
   DataOut(0, Strka);

   sprintf(Strka,"\History: %d scans", init.iTailLength);
   DataOut(0, Strka);

   sprintf(Strka, "\tGetting data: port %s, baudrate: %d ",
      init.csComPortStr, init.iBaudRate);
   DataOut(0, Strka);

   DataOut(0, "-----------------------------------------------------------------");
   LeaveCriticalSection(&CS);

   return 1;
}


//Вывести данные об ошибке
int LOGFILE_IKO::OutCriticalError(
                 char *_TextIskl,
                 char *_ModuleName,
                 char *_ClassName,
                 char *_MethodName,
                 char *_RegExName) //Сообщение об критической ошибки - ошибки приводящей к закрытию программы
{
  char Strka[1200];
  EnterCriticalSection(&CS);
  if(!fp)return 0;

  DataOut(1, "\tCriticаl error");
  sprintf(Strka, "\tDescription of the error: %s",_TextIskl);
  DataOut(0, Strka);
  if(_ModuleName&&_ModuleName[0]!=0)
  {
    sprintf(Strka, "\tName of the module with error: %s",_ModuleName);
    DataOut(0, Strka);
  }

  if(_ClassName&&_ClassName[0]!=0||
     _MethodName&&_MethodName[0]!=0)
  {
    sprintf(Strka, "\tFunction with error: %s::%s",_ClassName,_MethodName);
    DataOut(0, Strka);
  }

  if(_RegExName&&_RegExName[0]!=0)
  {
    sprintf(Strka, "\tExpression: %s",_RegExName);
    DataOut(0, Strka);
  }

  DataOut(0, "App will be closed!");
  DataOut(0, "-----------------------------------------------------------------");
  LeaveCriticalSection(&CS);

  return 1;
}

//Системное сообщение об ошибке
int LOGFILE_IKO::OutCriticalError(
                char *ClassName,
                char *Message)
{
   char Strka[200];
   EnterCriticalSection(&CS);
   if(!fp)return 0;

   DataOut(1, "\tCritical error");
   sprintf(Strka,"\t%s %s",ClassName,Message);
   DataOut(0, Strka);
   DataOut(0, "App will be closed!");
   DataOut(0, "-----------------------------------------------------------------");
   LeaveCriticalSection(&CS);
   return 1;
}



//Вывести рабочую информацию. Информацию о сигналах бедствий от борта.
//и прочее
int LOGFILE_IKO::OutWorkInfo(char *Mess)    //Информация о выводе текущей обстановке
{
  char Strka[1200];
  EnterCriticalSection(&CS);
  if(!fp)return 0;

  sprintf(Strka,"Work. %s",Mess);
  DataOut(1, Strka);
  DataOut(0, "-----------------------------------------------------------------");
  LeaveCriticalSection(&CS);

  return 1;
}


//Информация о некритической ошибке
int LOGFILE_IKO::OutNotCriticalError(char *Mess)  //Например, информация что не открыт порт.
{
  char Strka[1200];
  EnterCriticalSection(&CS);
  if(!fp)return 0;

  sprintf(Strka, "Error %s",Mess);
  DataOut(1, Strka);
  DataOut(0, "-----------------------------------------------------------------");
  LeaveCriticalSection(&CS);
  return 1;
}

//Информация о смене важных опций - отключение сопровождений и прочее.
int LOGFILE_IKO::OutChangeOption(const initialization &_init)
{
//Смена опций

  char Strka[1200];
  int WasTextNewOption=0;
  EnterCriticalSection(&CS);
  if(!fp)return 0;

  if(init.iTail!=_init.iTail)
  {
     if(!WasTextNewOption)
        DataOut(1, "\tChanged important settings: ");
     if(_init.iTail)sprintf(Strka,"\t Track up");
      else sprintf(Strka,"\tTrack down");
      DataOut(0, Strka);
     WasTextNewOption=1;
  }
  if(init.iTailLength!=_init.iTailLength)
  {
     if(!WasTextNewOption)
        DataOut(1, "\tChanged important settings: ");

      sprintf(Strka,"\t New size of scans: %d ",
            _init.iTailLength);
      DataOut(0, Strka);
     WasTextNewOption=1;
  }

  if(strcmp(init.csComPortStr,_init.csComPortStr)!=0||
     init.iBaudRate!=_init.iBaudRate)
  {
     if(!WasTextNewOption)
        DataOut(1, "\tChanged important settings: ");

     sprintf(Strka,"\t New settings of port: %s, %d baud",
      _init.csComPortStr, _init.iBaudRate);
     DataOut(0, Strka);
     WasTextNewOption=1;
  }

  memcpy(&init, &_init,sizeof(initialization));
  DataOut(0, "-----------------------------------------------------------------");
  LeaveCriticalSection(&CS);
  return 1;
}


//Проверка, которая должна происходить каждые 5-10 секунд
//Перехода на следующие сутки
int LOGFILE_IKO::TestNextDay(void)
{
   int Y1,M1,D1,H,M,S;
    time_t cur_time;
    struct tm *cur_tm;
    int Ret;

//Узнаем текущее время
   EnterCriticalSection(&CS);
   if(!fp)return 0;
   time(&cur_time);
   cur_tm=localtime(&cur_time);
   if(cur_tm->tm_year+1900!=LastYear||
       cur_tm->tm_mon+1!=LastMonth||
       cur_tm->tm_mday!=LastDay)
   {
      Ret=NewDay(cur_tm->tm_year+1900,cur_tm->tm_mon+1,cur_tm->tm_mday,
                 cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec);


      if(!Ret||!fp)
      {
         fp=NULL;
         return 0;
      }
   }

   LeaveCriticalSection(&CS);
   return 1;
}


