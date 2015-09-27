//---------------------------------------------------------------------------


#pragma hdrstop



#include <dir.h>
#include <io.h>
#include <fcntl.h>
#include <math.h>

#ifdef linux
#include <sys\stat.h>
#else
#include <sys/stat.h>
#endif

#include <stdio.h>
#include <time.h>

#include "paketstruct.h"
//#include "soprOtmetki.h"
#include "soprforworkfile.h"
#include "win_str_dir_utils.h"



//---------------------------------------------------------------------------
#define ABS(x) ((x)>=0?(x):(-(x)))



#pragma package(smart_init)


static char FullPath[1024];  //Полный путь к данным

static int Cifra2=5;

static int FileName_d;  //Дескриптор файла текущий дескриптор файла








//Извлечь дату из файла, которая записана в первой строке
//Вспомогательная процедура
//Вх параметры: char *FileName
//Вых параметры: Y, M,D - год, месяц, день
//Возвращает 0, если нет файла. -1 файл есть но дата не получена, 1 - дата получена
int soprGetDate(char *FileName,
                int *Y,
                int *M,
                int *D)
{
  FILE *fp;
  char c1,c2;
  int Ret;
  fp=fopen(FileName,"r");
  if(!fp)return 0;
  Ret=fseek(fp,47,0);  //Встанем на 47
  if(Ret)
  {
       fclose(fp);
       return (-1);
  }
  Ret=fscanf(fp,"%d%c%d%c%d", D, &c1, M, &c2, Y);
  fclose(fp);
  if(Ret==5)return 1;
  return (-1);
}


//Удаление файлов не заданной даты
//Возвращает если все подчищено
int soprDelFiles(char *Path,    //Путь к файлам
                 char *Prefix,  //Префикс
                 char *Ext,     //Расширение
                 char Razdelit,
                 int Y, int M, int D)
{
  int i;
  char Stroka[2000];
  int M1,Y1,D1;
  int Ret;
  if(access(Path,00))return 0;


  for(i=0;i<24;i++)
  {
    sprintf(Stroka,"%s%c%s%02d.%s",Path,Razdelit,Prefix,i,Ext);
    Ret=soprGetDate(Stroka,&Y1,&M1,&D1);
    if(Ret==1)
    {
      if(Y1!=Y||M1!=M||D1!=D)
      {
         remove(Stroka);
      }
    }
    if(Ret==-1)
    {
      remove(Stroka);
    }
  }
  return 1;
}



//soprGetMinNomer
//Вернуть минимальный файл в папке но не младше заданного времени
//Возвращает номер файла от 0 до 23, иначе -1
//Используется в методе
int soprGetMinNomer(char *Path,
                    char *Prefix,
                    char *Ext,
                    char Razdelit,
                    TMyDateTime DT)
{
   char Stroka[2000];
   int i;
   TMyDateTime MinDT;
   MinDT.God=2038;
   MinDT.Mesyac=1;
   MinDT.Den=1;
   MinDT.Time=0;
//
   for(i=0;i<24;i++)
   {

   }

}






/*Записывает в строку данные*/


long __stdcall soprSaveToFileOtmetkaString(
            char *Strka,         //Размер строки должен быть не меньше 150 - переполнение
            TPaketData &_PaketData,
            bool bVyvodAmpl, //Выводить амлитуду для первичного канала,
            bool bSkorostOut, //Выводить данные о скоростях
            bool bDopDopInfo,      //Выводить правильную информацию о значениях высоты и номерхо
	    int &SizeOfStroka      //Размер строки

                                )
{
  char strMAURC[6];   //Моно, ЛЧМ. УВД, RBS, Контрольная отметка
  char strPS[3];
  char strNHNH[5];
  char strAO[3];
  int Index;
  long OstatokToplivaOut;
  short H,M;
  double S, Ostatok;

  H=(short)(_PaketData.DateTime.Time);
  Ostatok=(_PaketData.DateTime.Time-H)*60;
  M=(short)(Ostatok);
  S=(Ostatok-M)*60.0;



  if(_PaketData.Otmetka.bOstatokTopliva)
    OstatokToplivaOut=_PaketData.Otmetka.lOstatokTopliva;
  else OstatokToplivaOut=-1;

//Заполним массив strMAURC
  if(_PaketData.Otmetka.Kanal==Kanal_P)
  {
//Заполним для первичного канала
    if(_PaketData.Otmetka.bP_Mono)strMAURC[0]='M';
    else strMAURC[0]=' ';
    if(_PaketData.Otmetka.bP_LCHM)strMAURC[1]='A';
    else strMAURC[1]=' ';
    strMAURC[2]=strMAURC[3]=' ';
    if(_PaketData.Otmetka.bKO)strMAURC[4]='C'; else strMAURC[4]=' ';
    strMAURC[5]=0;
    strPS[0]='P'; strPS[1]=' '; strPS[2]=0;
    strAO[0]=strAO[1]=' '; strAO[2]=0;
  }else{
//Заполним для вторичного канала
    if(_PaketData.Otmetka.Kanal==Kanal_PS)
      {strPS[0]='P'; strPS[1]='S'; strPS[2]=0;}
    else
      {strPS[0]=' '; strPS[1]='S'; strPS[2]=0;};
    strMAURC[0]=' ';     strMAURC[1]=' ';
    if(_PaketData.Otmetka.Rej==RejimUVD){strMAURC[2]='U';strMAURC[3]=' ';}
    else {strMAURC[2]=' ';strMAURC[3]='R';}
    if(_PaketData.Otmetka.bKO)strMAURC[4]='C'; else strMAURC[4]=' ';
    strMAURC[5]=0;

    if(_PaketData.Otmetka.bAbsVysota)
      {strAO[0]='A'; strAO[1]=' '; strAO[2]=0;}
      else
      {strAO[0]=' '; strAO[1]='O'; strAO[2]=0;};


  }

//Заполним данные о коде бедствия
  strNHNH[0]=' ';
  strNHNH[1]=' ';
  if(_PaketData.Otmetka.bNomer)strNHNH[2]='N';else strNHNH[2]=' ';
  if(_PaketData.Otmetka.bVysota)strNHNH[3]='H';else strNHNH[3]=' ';
  strNHNH[4]=0;

//Абсолютная или относительная высота

  

  sprintf(Strka, "\
 %c%c |\
%3d|\
%s|\
%s|\
%s|\
%s|\
%4d|\
%3d|\
%04X|\
%5d|\
%7d|\
%7d|\
%7d|\
 %6.02lf |\
 %6.02lf |\
 %02d:%02d:%06.03lf |",
  '5','0',   //В первой строчке
  _PaketData.Otmetka.IndexInObzor,
  strMAURC,
  strPS,
  strNHNH,
  strAO,
  _PaketData.Otmetka.sKodB,
  OstatokToplivaOut,
  _PaketData.Otmetka.lNomerTrassy%0x10000,
  _PaketData.Otmetka.lNomer,
  _PaketData.Otmetka.lVysota,
  _PaketData.Otmetka.X,
  _PaketData.Otmetka.Y,
  _PaketData.Azimut,
  _PaketData.Otmetka.Dalnost,
  H,M,S);

  SizeOfStroka=103;


//Проверим надо ли печатать дополнительные данные

  if(bVyvodAmpl&&_PaketData.Otmetka.Kanal==Kanal_P)
  {
   //Выведем значение амилитуды
       sprintf(Strka+103," %8.1e |",_PaketData.Otmetka.dAmpl);
       SizeOfStroka=114;
  }else{
     if(bSkorostOut||bDopDopInfo)
     {
//Выведем 10 пробелов
       sprintf(Strka+103,"          |");
       SizeOfStroka=114;
     }
  }



  if(bSkorostOut)
  {
//Выведем информацию о векторах скоростей
     sprintf(Strka+114,"%5d|%5d|",_PaketData.Otmetka.Vx,
                                  _PaketData.Otmetka.Vy);  //Выведем значение скорости
     SizeOfStroka=126;
  }else{
     if(bDopDopInfo)
     {
        sprintf(Strka+114,"     |     |");  //Выведем значение скорости
        SizeOfStroka=126;
     }
  }


  if(bDopDopInfo)
  {
    sprintf(Strka+126, "%5d|%7d|",_PaketData.Otmetka.lDopNumber,
                                 _PaketData.Otmetka.lDopVysota);
    SizeOfStroka=140;
  }

//Добавлено 25.01.2008
//Выведем еще дополнительную информацию
  if(_PaketData.Otmetka.N_of_MustPO||
     _PaketData.Otmetka.N_of_MustSO)
  {
//Если есть информация о первичном канале
    if(_PaketData.Otmetka.N_of_MustPO>0)
    {
       sprintf(Strka+SizeOfStroka,"%4d|",_PaketData.Otmetka.N_of_PO);
       SizeOfStroka+=5;
       sprintf(Strka+SizeOfStroka,"%4d|",_PaketData.Otmetka.N_of_MustPO);
       SizeOfStroka+=5;
       if(_PaketData.Otmetka.P_PO>=0.0)sprintf(Strka+SizeOfStroka,"%4.02lf|",_PaketData.Otmetka.P_PO);
       else sprintf(Strka+SizeOfStroka,"   ?|");
       SizeOfStroka+=5;
    }else{
       sprintf(Strka+SizeOfStroka,"    |    |    |");
       SizeOfStroka+=15;
    }
    if(_PaketData.Otmetka.N_of_MustSO>0)
    {
       sprintf(Strka+SizeOfStroka,"%4d|",_PaketData.Otmetka.N_of_SO);
       SizeOfStroka+=5;
       sprintf(Strka+SizeOfStroka,"%4d|",_PaketData.Otmetka.N_of_MustSO);
       SizeOfStroka+=5;
       if(_PaketData.Otmetka.P_SO>=0.0)sprintf(Strka+SizeOfStroka,"%4.02lf|",_PaketData.Otmetka.P_SO);
       else sprintf(Strka+SizeOfStroka,"   ?|");
       SizeOfStroka+=5;
    }else{
       sprintf(Strka+SizeOfStroka,"    |    |    |");
       SizeOfStroka+=15;
    }
  }
  return 1;
}





/*Сохранение севера в строку Strka*/
long __stdcall soprSaveToFileSeverString
(
    char *Strka,         //Размер строки должен быть не меньше 120 - переполнение
    short God, short Mesyac, short Day,  //Год месяц день
    char *DopText,
    short H, short M, double S   //Время в часах, минутах и секундах
)
{
  int i,j,k;
    
     sprintf(Strka, "NORD -- %02d.%02d.%04d -- ",Day,Mesyac,God);
  

  for(i=21,j=0;i<=85;i++,j++)
  {
    if(DopText[j]=='\0')
    {
      for(k=i;k<=85;k++)
      {
         Strka[k]=' ';
      }
      break;
    }
    Strka[i]=DopText[j];
  }
  Strka[86]=' '; Strka[87]='|';
  sprintf(Strka+88," %02d:%02d:%06.03lf |",H,M,S);


  return 1;
}

//Вывод данных о секторе
long __stdcall soprSaveToFileSektorString
(
  char *Strka,         //Размер строки должен быть не меньше 150 - переполнение
  double Azmt,
  short H, short M, double S   //Время в часах, минутах и секундах
)
{
   sprintf(Strka,"SECTOR %6.02lf | %02d:%02d:%06.03lf |" ,Azmt,H,M,S);
   return 1;
}


/*Вывод данных в строку Strka*/
long __stdcall  soprSaveToFileString(
            char *Strka,         //Размер строки должен быть не меньше 200 - переполнение
            TPaketData &_PaketData,
            char *DopTextOut,//Дополнительный текст для вывода
            bool bVyvodAmpl, //Выводить амлитуду для первичного канала,
            bool bSkorostOut, //Выводить данные о скоростях
            bool bDopDopInfo,      //Выводить правильную информацию о значениях высоты и номерхо
	    int &SizeOfStroka      //Размер строки
                          )
{
   short M,H;
   double S,Ostatok;
   char Strka1[250]=" ";
//Расчитаем часы минуты и секунды

   switch(_PaketData.TipPaketa)
   {
      case TipPakSektor:
        H=(short)(_PaketData.DateTime.Time);
        Ostatok=(_PaketData.DateTime.Time-H)*60;
        M=(short)(Ostatok);
        S=(Ostatok-M)*60.0;
//Выведем данные
        soprSaveToFileSektorString(
          Strka,
          _PaketData.Azimut,
          H,M,S);
	  SizeOfStroka=30;
      break;

      case TipPakSever:
        H=(short)(_PaketData.DateTime.Time);
        Ostatok=(_PaketData.DateTime.Time-H)*60;
        M=(short)(Ostatok);
        S=(Ostatok-M)*60.0;
        if(_PaketData.Sever.Soprovojdenie)
        {
          Strka1[15]='К';
          Strka1[16]='Л';
          Strka1[17]=' ';
        } 

        soprSaveToFileSeverString(
           Strka,
           _PaketData.DateTime.God,
           _PaketData.DateTime.Mesyac,
           _PaketData.DateTime.Den,
           Strka1,
           H, M, S);
	   SizeOfStroka=103;
      break;


      case TipPakOtmetka:
           soprSaveToFileOtmetkaString(Strka,
                    _PaketData,
                   bVyvodAmpl,
                   bSkorostOut,
                   bDopDopInfo,
		   SizeOfStroka);
      break;

      default:
        return 0;
   };


   return 1;
}

/*Получение данных из строки в пакет*/
long __stdcall  soprGetFromStringToPaketData
                (
                  char *Strka,                //Разбираемая строка
                  TMyDateTime *LastDateTime,  //Последний север (м.б. равен 0)
                  TPaketData &_PaketData
                )
{
  long Dlina;
  int Ret;
  double dfChislo;
  int M1,H1;
  double S1;
  int M2,Y2,D2;
  char c1,c2;
  struct tm tm1, *tm2;
  time_t time_t1;
  int ForTrek;  //Поправка на трек. Либо 0, если плот, либо 1, если трек.
  char *VrStr;
  double Azimut, Dalnost;
//Считывание данных
//Определим сначала тип отсчета
  Dlina=strlen(Strka);
//Определим типа отсчета
  if(Dlina>=30&&Strka[0]=='S'&&Strka[1]=='E'&&Strka[2]=='C'&&
     Strka[3]=='T'&&Strka[4]=='O'&&Strka[5]=='R')
  {
//Это возможно сектор
//Проверим градусы
     Ret=sscanf(Strka+7,"%lf",&(_PaketData.Azimut));
     if(Ret==0||_PaketData.Azimut<-0.000001||
       _PaketData.Azimut>359.99999)
     {
        return 0;
     }

//Проверим время
     Ret=sscanf(Strka+16,"%d%c%d%c%lf",&H1,&c1,&M1,&c2,&S1);
     if(Ret!=5||H1<0||H1>23||M1<0||M1>59||S1<-0.0000000000001||
       S1>59.999999999999||c1!=':'||c2!=':')
     {
       return 0;
     }
     _PaketData.DateTime.Time=H1+M1/60.0+S1/3600.0;


//Узнаем дату
     if(LastDateTime)
     {
//Сравним дату и время
       if(_PaketData.DateTime.Time<LastDateTime->Time-5.0/60.0)
       {
//Надо расчитать следующий день
           tm1.tm_year=LastDateTime->God-1900;
           tm1.tm_mon=LastDateTime->Mesyac-1;
           tm1.tm_mday=LastDateTime->Den;
           tm1.tm_sec=tm1.tm_min=tm1.tm_hour=0;
           tm1.tm_isdst=-1;
           time_t1=mktime(&tm1);
           time_t1+=24*3600;
           tm2=localtime(&time_t1);
           _PaketData.DateTime.God=tm2->tm_year+1900;
           _PaketData.DateTime.Mesyac=tm2->tm_mon+1;
           _PaketData.DateTime.Den=tm2->tm_mday;
       }else{
           _PaketData.DateTime.God=LastDateTime->God;
           _PaketData.DateTime.Mesyac=LastDateTime->Mesyac;
           _PaketData.DateTime.Den=LastDateTime->Den;
       }
     }else{
       _PaketData.DateTime.God=0;
       _PaketData.DateTime.Mesyac=0;
       _PaketData.DateTime.Den=0;
     }
//Всё нормально
     _PaketData.TipPaketa=TipPakSektor;
     *LastDateTime=_PaketData.DateTime;
     return 1;
  }

//Определим тип отсчета типа север
  if(Dlina>=101&&Strka[0]=='N'&&Strka[1]=='O'&&Strka[2]=='R'&&
     Strka[3]=='D')
  {
//Разберем дату
      Ret=sscanf(Strka+8,"%d%c%d%c%d",&D2,&c1,&M2,&c2,&Y2);
      if(Ret!=5||D2>31||D2<0||M2>12||M2<1||Y2<1900||Y2>2038||
         c1!='.'||c2!='.')
      {
         return 0;
      }
      _PaketData.DateTime.God=Y2;
      _PaketData.DateTime.Mesyac=M2;
      _PaketData.DateTime.Den=D2;

//Разберем время
     Ret=sscanf(Strka+89,"%d%c%d%c%lf",&H1,&c1,&M1,&c2,&S1);
     if(Ret!=5||H1<0||H1>23||M1<0||M1>59||S1<-0.0000000000001||
       S1>59.999999999999||c1!=':'||c2!=':')
     {
       return 0;
     }
     _PaketData.DateTime.Time=H1+M1/60.0+S1/3600.0;
     _PaketData.TipPaketa=TipPakSever;

     _PaketData.Azimut=0;

//Скопируем данные о севере
     *LastDateTime=_PaketData.DateTime;
     return 1;
  }

//А теперь самое сложное - разобрать строку с данными. Причем определить надо
//как и MTA (треки и плоты), так и плоты от Холодного неба (АОРЛ-1А)
  if(Dlina>=101&&Strka[0]==' '&&Strka[1]>=0x30&&Strka[1]<=0x39&&
     Strka[2]>=0x30&&Strka[2]<=0x39&&Strka[3]==' ')
  {
//Возможно это отсчет
     if(Dlina<101)return 0;
     if(Strka[2]=='3')ForTrek=1; else ForTrek=0;

//Определим тип канал отсчета
     if(Strka[16-ForTrek]=='S')
     {
        if(Strka[15-ForTrek]=='P')
        {
//Объединенная отметка
          _PaketData.Otmetka.Kanal=Kanal_PS;
        }else{
          _PaketData.Otmetka.Kanal=Kanal_S;
        }

//Теперь разберемся с режимами
        if(Strka[11]=='U'||
           Strka[31-ForTrek]=='U')
        {
           _PaketData.Otmetka.Rej=RejimUVD;
        }else
        if (Strka[12]=='R'||
           Strka[30-ForTrek]=='I')
        {
           _PaketData.Otmetka.Rej=RejimRBS;
        }else{
           _PaketData.Otmetka.Rej=RejimNeIzvesten;
        }

     }else{
        if(Strka[15-ForTrek]=='P')
        {
           _PaketData.Otmetka.Kanal=Kanal_P;
           _PaketData.Otmetka.bP_LCHM=(Strka[10]=='A');
           _PaketData.Otmetka.bP_Mono=(Strka[9]=='M');
        }else{
           return 0;   //Не распознанный отсчет
        }
     }

//Определимся со всеми остальными значковыми полями для S
    _PaketData.Otmetka.bKodN=(Strka[18-ForTrek]=='N');
    _PaketData.Otmetka.bKodH=(Strka[19-ForTrek]=='H');
    _PaketData.Otmetka.bNomer=(Strka[20-ForTrek]=='N'||
                               Strka[20-ForTrek]=='B');
    _PaketData.Otmetka.bVysota=(Strka[21-ForTrek]=='H');

    _PaketData.Otmetka.bKO=(Strka[13]=='C');

//Рассмотрим относительность высоты
    _PaketData.Otmetka.bAbsVysota=
       (Strka[22-ForTrek]=='A'||Strka[23-ForTrek]=='A');

//Считаем номер
    Ret=sscanf(Strka+40-ForTrek,"%ld",&(_PaketData.Otmetka.lNomer));
    if(!Ret)return 0;

//Считаем высоту
    Ret=sscanf(Strka+47-ForTrek,"%ld",&(_PaketData.Otmetka.lVysota));
    if(!Ret)return 0;

//Считаем деккартовы координаты
    Ret=sscanf(Strka+54-ForTrek,"%ld",&(_PaketData.Otmetka.X));
    if(!Ret)return 0;

    Ret=sscanf(Strka+62-ForTrek,"%ld",&(_PaketData.Otmetka.Y));
    if(!Ret)return 0;

//Считаем время
    if(ForTrek)
    {
      if(Dlina<114)return 0;
      VrStr=Strka+102;
    }else{
      VrStr=Strka+89;
    }

    Ret=sscanf(VrStr,"%d%c%d%c%lf",&H1,&c1,&M1,&c2,&S1);
    if(Ret!=5||H1<0||H1>23||M1<0||M1>59||S1<-0.0000000000001||
      S1>59.999999999999||c1!=':'||c2!=':')
    {
      return 0;
    }
    _PaketData.DateTime.Time=H1+M1/60.0+S1/3600.0;

//Узнаем дату
    if(LastDateTime)
    {
//Сравним дату и время
       if(_PaketData.DateTime.Time<LastDateTime->Time-5.0/60.0)
       {
//Надо расчитать следующий день
           tm1.tm_year=LastDateTime->God-1900;
           tm1.tm_mon=LastDateTime->Mesyac-1;
           tm1.tm_mday=LastDateTime->Den;
           tm1.tm_sec=tm1.tm_min=tm1.tm_hour=0;
           tm1.tm_isdst=-1;
           time_t1=mktime(&tm1);
           time_t1+=24*3600;
           tm2=localtime(&time_t1);
           _PaketData.DateTime.God=tm2->tm_year+1900;
           _PaketData.DateTime.Mesyac=tm2->tm_mon+1;
           _PaketData.DateTime.Den=tm2->tm_mday;
       }else{
           _PaketData.DateTime.God=LastDateTime->God;
           _PaketData.DateTime.Mesyac=LastDateTime->Mesyac;
           _PaketData.DateTime.Den=LastDateTime->Den;
       }
    }else{
       _PaketData.DateTime.God=0;
       _PaketData.DateTime.Mesyac=0;
       _PaketData.DateTime.Den=0;
    }

//Узнаем остаток топлива
    Ret=sscanf(Strka+31-ForTrek,"%ld",
        &(_PaketData.Otmetka.lOstatokTopliva));
    if(!Ret||_PaketData.Otmetka.lOstatokTopliva<0)
    {
       _PaketData.Otmetka.lOstatokTopliva=0;
       _PaketData.Otmetka.bOstatokTopliva=false;
    }else{
       _PaketData.Otmetka.bOstatokTopliva=true;
    }

//Узнаем код бедствия
    long lKodB;
    Ret=sscanf(Strka+26-ForTrek,"%ld",&lKodB);
    if(Ret||lKodB<1000)
    {
      _PaketData.Otmetka.sKodB=0;
    }else{
      _PaketData.Otmetka.sKodB=lKodB;
    }

//Узнаем и отрегулируем азимут и дальность
    Ret=sscanf(Strka+72-ForTrek, "%lf",&Azimut)+
        sscanf(Strka+81-ForTrek, "%lf",&Dalnost);

    long X1,Y1;
    if(Ret==2&&Azimut>-0.000001&&Azimut<359.99999&&Dalnost>=-0.000001)
    {
//Проверим еще и азимутальные координаты
       X1=(long)(Dalnost*sin(Azimut/180.0*M_PI)*1000);
       Y1=(long)(Dalnost*cos(Azimut/180.0*M_PI)*1000);
//сравним X1 и X, Y1 и Y
       if(ABS(X1-_PaketData.Otmetka.X)>10||ABS(Y1-_PaketData.Otmetka.Y)>10)
       {
//Перерасчитаем азимуты и дальность
          if(_PaketData.Otmetka.X==0&&_PaketData.Otmetka.Y==0)
          {
              Azimut=0;
              Dalnost=0;
          }else{
              Azimut=atan2(_PaketData.Otmetka.X/1000.0,_PaketData.Otmetka.Y/1000.0)/M_PI*180.0;
              Dalnost=sqrt(_PaketData.Otmetka.X/1000.0*_PaketData.Otmetka.X/1000.0+
                       _PaketData.Otmetka.Y/1000.0*_PaketData.Otmetka.Y/1000.0);
          }
       }
    }else{
       if(_PaketData.Otmetka.X==0&&_PaketData.Otmetka.Y==0)
       {
          Azimut=0;
          Dalnost=0;
       }else{
          Azimut=atan2(_PaketData.Otmetka.X/1000.0,_PaketData.Otmetka.Y/1000.0)/M_PI*180.0;
          Dalnost=sqrt(_PaketData.Otmetka.X/1000.0*_PaketData.Otmetka.X/1000.0+
                    _PaketData.Otmetka.Y/1000.0*_PaketData.Otmetka.Y/1000.0);
       }
    }
    if(Azimut<0)Azimut+=360.0;
    _PaketData.Otmetka.Dalnost=Dalnost;
    _PaketData.Azimut=Azimut;
    
    
//Узнаем дополнительную информацию за пределы посл.

    if(!ForTrek)
    {
      if(Dlina>=112)
      {
//Проверим уровень сигнала
        if(_PaketData.Otmetka.Kanal==Kanal_P)
          Ret=sscanf(Strka+103,"%lf",
           &(_PaketData.Otmetka.dAmpl));
           if(!Ret)_PaketData.Otmetka.dAmpl=0.0;
        }else _PaketData.Otmetka.dAmpl=0.0;
      }else _PaketData.Otmetka.dAmpl=0.0;

//проверим скорость
      if(Dlina>=126)
      {
        Ret=sscanf(Strka+114,"%d",
              &(_PaketData.Otmetka.Vx));
        Ret+=sscanf(Strka+120,"%d",
              &(_PaketData.Otmetka.Vy));
        if(Ret!=2)
          _PaketData.Otmetka.Vx=_PaketData.Otmetka.Vy=100000;
      }else _PaketData.Otmetka.Vx=_PaketData.Otmetka.Vy=100000;

      if(Dlina>=140)
      {
        Ret=sscanf(Strka+126,"%d",
              &(_PaketData.Otmetka.lDopNumber));
        Ret+=sscanf(Strka+132,"%d",
              &(_PaketData.Otmetka.lDopVysota));

        if(Ret!=2)
_PaketData.Otmetka.lDopNumber=_PaketData.Otmetka.lDopVysota=-100;
      else
        _PaketData.Otmetka.lDopNumber=_PaketData.Otmetka.lDopVysota=-100;
    }//if(!ForTrek

    *LastDateTime=_PaketData.DateTime;
    _PaketData.TipPaketa=TipPakOtmetka;
    return 1;

  } //if(Strka[0]==' '&&Strka[1]>=0x...

  return 0;
}



/*Старт запуска открываются нужные файлы. Не нужные затираюся
  Возвращает: 0 - если уже запущена, -1 - если не существует

пути csPutKDannym. -2 - если не успешно записалось в файл.
-3 - ошибка создании папки
1- если успешно был запуск
*/
int TSoprSaveSession::Start(void)
{
  int Ret;
  time_t cur_time;
  struct tm *cur_tm;
  char c1,c2;
  int Y1,Dy1,Mth1;
  FILE *fp;
  char sDatePapka[1250],
       sFileData[1270];
  char StrokaOut[100];


  if(bIsInit||bIsInitLoad)return 0;

//Проверим существование каталога csPutKDannym
  Ret=access(csPutKDannym,00);

//Попробуем создать папку
  if(winMakePath(csPutKDannym))
  {
     Ret=access(csPutKDannym,00);
  }

  if(Ret!=0)return (-1);

//Узнаем текущее время
  time(&cur_time);
  cur_tm=localtime(&cur_time);

//Узнаем имя папки по дню

  sprintf(sDatePapka,"%s%c%02d",
       csPutKDannym,cRazdelitel,cur_tm->tm_mday);

//Проверим существование данной папки
  Ret=access(sDatePapka,00);
  if(Ret)
  {
//Данная папка не существует - то создать ее
     if(mkdir(sDatePapka))
     {
        return (-3);
     }
  }


//Попробуем удалить не актуальные файлы в папке
  soprDelFiles(sDatePapka,csPrefix,csExt,cRazdelitel,
    cur_tm->tm_year+1900,    cur_tm->tm_mon+1, cur_tm->tm_mday);

//Откроем файл на чтение
  sprintf(sFileData,"%s%c%s%02d%c%s",
       sDatePapka,cRazdelitel,csPrefix,cur_tm->tm_hour,'.',csExt);





//Создаем файл
  if(access(sFileData,00))
  {
     lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY|O_TRUNC|O_CREAT,S_IREAD|S_IWRITE);
  }else{
     lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY);
  }
  if(lFD==-1)return -4;

//Сделаем запись в файл.
//Подготовим строку
  sprintf(StrokaOut,">>>   Start log : Time  %02d:%02d:%02d   Date %02d.%02d.%04d",    
   cur_tm->tm_hour,cur_tm->tm_min, cur_tm->tm_sec,
   cur_tm->tm_mday, cur_tm->tm_mon+1, cur_tm->tm_year+1900);

//Проверим что надо добавить в строку
  if(bDataOutDOS)
  {
     StrokaOut[57]=13;
     StrokaOut[58]=10;
     write(lFD,StrokaOut,59);

  }else{
     StrokaOut[57]=10;
     write(lFD,StrokaOut,58);
  }

  bIsInit=true;
  iLastHour=cur_tm->tm_hour;
  return 1;
}

/*Остановка данных. Запись прекращается - выдается сообщение об остановке
Запись производится в текущую папку.
*/
int  TSoprSaveSession::Stop(void)
{
  int Ret;
  time_t cur_time;
  struct tm *cur_tm;
  char c1,c2;
  int Y1,Dy1,Mth1;
  FILE *fp;
  char sDatePapka[1250],
       sFileData[1270];
  char StrokaOut[100];


  if(!bIsInit)return 0;

//Узнаем текущее время
  time(&cur_time);
  cur_tm=localtime(&cur_time);


//Проверим существование каталога csPutKDannym
  if(iLastHour!=cur_tm->tm_hour)
  {
     close(lFD);  //Закроем текущий дескриптор
     Ret=access(csPutKDannym,00);
     if(Ret!=0)return (-1);


//Узнаем имя папки по дню

     sprintf(sDatePapka,"%s%c%02d",
          csPutKDannym,cRazdelitel,cur_tm->tm_mday);

//Проверим существование данной папки
     Ret=access(sDatePapka,00);
     if(Ret)
     {
//Данная папка не существует - то создать ее
        if(mkdir(sDatePapka))
        {
           return (-3);
        }
     }


     //Попробуем удалить не актуальные файлы в папке
      soprDelFiles(sDatePapka,csPrefix,csExt,cRazdelitel,
    cur_tm->tm_year+1900,    cur_tm->tm_mon+1, cur_tm->tm_mday);

    //Откроем файл на чтение
     sprintf(sFileData,"%s%c%s%02d%c%s",
          sDatePapka,cRazdelitel,csPrefix,cur_tm->tm_hour,'.',csExt);


//Создаем файл
    if(access(sFileData,00))
    {
       lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY|O_TRUNC|O_CREAT,S_IREAD|S_IWRITE);
    }else{
       lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY);
    }
    if(lFD==-1)return -4;
  }
//Сделаем запись в файл.
//Подготовим строку
  sprintf(StrokaOut,">>>   End log : Time  %02d:%02d:%02d   Date %02d.%02d.%04d",
       cur_tm->tm_hour,cur_tm->tm_min, cur_tm->tm_sec,
    cur_tm->tm_mday, cur_tm->tm_mon+1, cur_tm->tm_year+1900);

//Проверим что надо добавить в строку
  if(bDataOutDOS)
  {
     StrokaOut[57]=13;
     StrokaOut[58]=10;
     write(lFD,StrokaOut,59);

  }else{
     StrokaOut[57]=10;
     write(lFD,StrokaOut,58);
  }

//Выведем в файл данные
  close(lFD);
  bIsInit=false;
  return 1;
}



int TSoprSaveSession::SaveDataToFile(TPaketData &_PaketData)
{
  int Ret;
  time_t cur_time;
  struct tm *cur_tm;
  char c1,c2;
  int Y1,Dy1,Mth1;
  FILE *fp;
  char sDatePapka[1250],
       sFileData[1270];
  char StrokaOut[150];
  int SizeOfStrka;

  if(!bIsInit)return 0;

//Проверка праметров вывода
  switch(_PaketData.TipPaketa)
  {
     case TipPakOtmetka:
         if(!bSaveDataOtmetka)return 1;
         break;

     case TipPakSever:
         if(!bSaveDataSever)return 1;
         break;

     case TipPakSektor:
         if(!bSaveDataSektor)return 1;
         break;

     default:
         return 1;
  };

//Узнаем текущее время
  time(&cur_time);
  cur_tm=localtime(&cur_time);


//Проверим существование каталога csPutKDannym
  if(iLastHour!=cur_tm->tm_hour)
  {
     close(lFD);  //Закроем текущий дескриптор
     Ret=access(csPutKDannym,00);
     if(Ret!=0)return (-1);



//Узнаем имя папки по дню

     sprintf(sDatePapka,"%s%c%02d",
          csPutKDannym,cRazdelitel,cur_tm->tm_mday);


//Проверим существование данной папки
     Ret=access(sDatePapka,00);
     if(Ret)
     {
//Данная папка не существует - то создать ее
        if(mkdir(sDatePapka))
        {
           return (-3);
        }
     }

  //Попробуем удалить не актуальные файлы в папке
      soprDelFiles(sDatePapka,csPrefix,csExt,cRazdelitel,
    cur_tm->tm_year+1900,    cur_tm->tm_mon+1, cur_tm->tm_mday);



//Откроем файл на чтение
     sprintf(sFileData,"%s%c%s%02d%c%s",
          sDatePapka,cRazdelitel,csPrefix,cur_tm->tm_hour,'.',csExt);


//Создаем файл
//Создаем файл
    if(access(sFileData,00))
    {
       lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY|O_TRUNC|O_CREAT,S_IREAD|S_IWRITE);
    }else{
       lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY);
    }

    if(lFD==-1)return -4;
     //Сделаем запись в файл.
//Подготовим строку
    sprintf(StrokaOut,">>>   Start log: Time  %02d:%02d:%02d   Date %02d.%02d.%04d",
      cur_tm->tm_hour,cur_tm->tm_min, cur_tm->tm_sec,
      cur_tm->tm_mday, cur_tm->tm_mon+1, cur_tm->tm_year+1900);

//Проверим что надо добавить в строку
       if(bDataOutDOS)
       {
          StrokaOut[57]=13;
          StrokaOut[58]=10;
          write(lFD,StrokaOut,59);
       }else{
          StrokaOut[57]=10;
          write(lFD,StrokaOut,58);
       }
  }

//Выведем данные в строку
  Ret=DataToStr(StrokaOut,_PaketData, DopTextOut, bVyvodAmpl, bSkorostOut, bDopDopInfo, SizeOfStrka);


//Тут осуществляется вывод в файл самих данных подготовленной строки
  iLastHour=cur_tm->tm_hour;
  if(Ret==1)
  {
     if(bDataOutDOS)
     {
        StrokaOut[SizeOfStrka]=13;
        StrokaOut[SizeOfStrka+1]=10;
        write(lFD,StrokaOut,SizeOfStrka+2);
     }else{
        StrokaOut[SizeOfStrka]=10;
        write(lFD,StrokaOut,SizeOfStrka+1);
     }
  }else{
    return 0;
  }
  return 1;
}




/*Считывание данных из файла.
*/
/*Загрузка данных из файла*/
//Начало процесса загрузка данных из файла
//  Функция возвращает 1, если всё  ОК.
//0, если уже запущены загрузка или запись в файл.
//-1, уже кончилось время, или не смогла открыть файлы


int TSoprSaveSession::
    StartLoad(char *csFileNamePath, //Путь к каталогу или файлу
                TMyDateTime *FirstTime, //FirstTime - время откуда считывать данные из файла
                                        //Если равен нулю, начальное время
                int iType,       //Если Type=1, то FileNamePath - это путь к файлам.
                                 //             файлы непосредсьвено лежат в папке FileNamePath.
                                 //Если Type=2, то FileNamePath - это полный путь с именем
                                 //             файла
                                 //В остальных случаях берутся данные из папки
                                 //csPutKDannym с учетом дней

                TPaketData *PDO)   //Данные
{
//Процесс открытия
   if(bIsInit||bIsInitLoad)return 0;

 //Обнулим массив



   switch(iType)
   {
      case 1:
        GetDTForEveryHour(csFileNamePath,1);
        pTempMyDateTime=DTForEveryHour[0]; //Присвоим каталог с данными
        FirstDen=0;                        //Это для работы цикла внутри GetDataDirForFirstTime
//Путь к папке где файл открыт
        fOpen=GetDataDirForFirstTime(csFileNamePath,
                                     FirstTime,1,PDO);
        strcpy(csFileNamePath1,csFileNamePath);
        iTypeLoad=1;
      break;

      case 2:
        fOpen=GetDataFileForFirstTime(csFileNamePath,
                                      FirstTime,
                                      PDO);
        if(!fOpen)return (-1);
        bIsInitLoad=true;
        iTypeLoad=2;
      return 1;

      default:
        GetDTForEveryHour(csFileNamePath,0);
        fOpen=GetDataDirForFirstTime(csFileNamePath,
                                     FirstTime,0,PDO);
        strcpy(csFileNamePath1,csPutKDannym);
        iTypeLoad=0;

   };
   if(!fOpen)return (-1);
   bIsInitLoad=true;
   if(FirstTime)
   {
      FirstDateTime=*FirstTime;
   }else{
      memset(&FirstDateTime,0,sizeof(TMyDateTime));
   }
   //Заполним номер сектора
   if(fOpen)
   {
       PDO->NomerSektoraRazmerom30=(short)(PDO->Azimut/30);
       if(PDO->NomerSektoraRazmerom30==12)PDO->NomerSektoraRazmerom30=0;
       return 1;
   }

   return 1;
}


int TSoprSaveSession::GetDataFromFile(TPaketData *PDO)
{
  if(!bIsInitLoad)return 0;
  if(fOpen==NULL)return 0;
  int Ret;
//Будем читать строку пока не будет успеха или пока не убедимся, что его не может быть
    Ret=privGetDataFromFile(PDO);
    if(Ret==1)
    {
      PDO->NomerSektoraRazmerom30=(short)(PDO->Azimut/30);
      if(PDO->NomerSektoraRazmerom30==12)PDO->NomerSektoraRazmerom30=0;

      return 1;
    }
    fclose(fOpen);
    fOpen=NULL;
    if(iTypeLoad==2)return 0;   //Для одиночного файла все - завершено

//Надо попробывать найти следующий отсчет в папке
//Разберемся со временем
    TMyDateTime tempMyDateTime;   //Временный

    tempMyDateTime.God=LastDateTime.God;
    tempMyDateTime.Mesyac=LastDateTime.Mesyac;
    tempMyDateTime.Den=LastDateTime.Den;
    if(LastDateTime.Time<23)
    {
       tempMyDateTime.Time=(int)(LastDateTime.Time)+1;
    }else{
       tempMyDateTime.Time=23+59.0/60.0+59.9995/3600.0;
    }

//Попробуем открыть и считать следующее данное из файла
    fOpen=GetDataDirForFirstTime(csFileNamePath1,
                        &tempMyDateTime,iTypeLoad,PDO);

//Заполним номер сектора
    if(fOpen)
    {
       PDO->NomerSektoraRazmerom30=(short)(PDO->Azimut/30);
       if(PDO->NomerSektoraRazmerom30==12)PDO->NomerSektoraRazmerom30=0;

       return 1;
    }
    return 0;
}



int TSoprSaveSession::StopLoad(void)
{
  if(!bIsInitLoad)return 0;
  if(fOpen)
  {
     fclose(fOpen);
     fOpen=NULL;
  }
  bIsInitLoad=false;
  return 1;
}




/*ПРОЦЕДУРЫ типа private*/

//Найти и открыть файл, где есть данные которые не раньше текущего времени
//возвращает описатель типа FILE.
FILE* TSoprSaveSession::GetDataDirForFirstTime
                           (
                             char *TekPapka,  //Папка поиска
                             TMyDateTime *FirstTime, //Первое время
                             int iType, //1 - по пути,
                                             //0 - по папкам разбитым по датам
                             TPaketData *PDO   //Масив с данными который заполняется
                           )
{
   int Chislo,i,j;
   FILE *fp;
   char Strka[1200];

   TMyDateTime MDT,minMDT;

   if(iType==0)
   {
       if(FirstTime&&FirstTime->God>0&&FirstTime->Mesyac>0&&
          FirstTime->Den>0)
       {
          MDT.Time=(int)(FirstTime->Time); //Округлим до наименьшего
          MDT.God=FirstTime->God;
          MDT.Mesyac=FirstTime->Mesyac;
          MDT.Den=FirstTime->Den;
       }else{
          MDT.God=1901;
          MDT.Den=1;
          MDT.Mesyac=1;
          MDT.Time=0;
       }
       minMDT.God=2038;
       minMDT.Mesyac=12;
       minMDT.Den=31;
       Chislo=-1;
       FirstDen=0;
//Определим стреди всех фалов и папок такую, которая минимальна
       for(i=1;i<=31;i++)
       {
          for(j=0;j<24;j++)
          {
              if(DTForEveryHour[i][j].God!=0&&
                 DTForEveryHour[i][j]>=MDT&&DTForEveryHour[i][j]<minMDT)
              {
                 minMDT=DTForEveryHour[i][j];
                 FirstDen=i;
                 FirstChas=j;
              }

          }
       }

       if(FirstDen==0)return NULL;  //Не найдено такого файла
       

//Ищем по всем папкам начиная с текущей...
       i=FirstDen;
       do
       {
         sprintf(Strka,"%s%c%02d",TekPapka,cRazdelitel,i);
         pTempMyDateTime=DTForEveryHour[i];  //Скопируем подмассив
         fOpen=GetDataDirForFirstTime(Strka,FirstTime,1,PDO);
         if(fOpen)
         {
            return fOpen;
         }
         i=(i+1)%31;
       }while(i!=FirstDen);

       return NULL;  //Не найден отсчет
   };



//Найти самый младший файл, но старше нашего


//Определим начальное время откуда считывать
//Определить по часу
//Определим первый час откуда считывать

   if(FirstDen==0)
   {
      if(FirstTime&&FirstTime->God>0&&FirstTime->Mesyac>0&&
         FirstTime->Den>0)
      {
          MDT.Time=(int)(FirstTime->Time); //Округлим до наименьшего
          MDT.God=FirstTime->God;
          MDT.Mesyac=FirstTime->Mesyac;
          MDT.Den=FirstTime->Den;
      }else{
          MDT.God=1901;
          MDT.Den=1;
          MDT.Mesyac=1;
          MDT.Time=0;
      }
      minMDT.God=2038;
      minMDT.Mesyac=12;
      minMDT.Den=31;
      Chislo=-1;
      for(i=0;i<24;i++)
      {
         if(pTempMyDateTime[i].God!=0)
         {
            if(pTempMyDateTime[i]>=MDT&&pTempMyDateTime[i]<minMDT)
            {
               Chislo=i;
               minMDT=pTempMyDateTime[i];
            }
         }
      }
      if(Chislo==-1)
      {
         return NULL;
      }
   }else{
     Chislo=FirstChas;
   }
   i=Chislo; //Пройтись по всем файлам(часам)
//Проверим каждый час тоесть файлы типа <префикс><часы>.<ext>
   do
   {
       sprintf(Strka,"%s%c%s%02d.%s",
       TekPapka,cRazdelitel,csPrefix,i,csExt);
       fOpen=GetDataFileForFirstTime(Strka,FirstTime,PDO);
       if(fOpen)
       {
          break;
       }
       i=(i+1)%24;
   }while(i!=Chislo);
   if(!fOpen)return NULL;
   return fOpen;
}


/* GetDataFileForFirstTime
  Вспомогательная процедура. Используется в методах: GetDataDirForFirstTime.
Передается полный путь к текущему файду TekFile,
и начальное время TMyDateTime *FirstTime. Значение FirstTime может быть
равно NULL, тогда ищется первый отсчет в данном файле. Если
FirstTime->Den, FirstTime->Mesyac или FirstTime->God равны 0, анализ идет только по времени.
При успешном чтении заполняются данные пакета PDO типа TPaketData.
Возвращаемое значение:
Описатель файла типа FILE при успешном поиске элемента, который больше FirstTime
При неудачи - значение NULL.
*/
FILE* TSoprSaveSession::GetDataFileForFirstTime(
                            char *TekFile,           //Текущий файл
                            TMyDateTime *FirstTime,  //Начальное время
                            TPaketData *PDO   //Масив с данными который заполняется
                           )
{
   short M1,S1,H1;
   long lRet;
   int Y2,M2,D2;
   int Ret,i;
   char Strka[300],*RetStr;
   int iStrLen;
   if(fOpen)return NULL;
   bool bDataBolshe=false;  //Признак того, что дата больше чем текущая.
   //Если так то время анализироваться не будет
//Проверка существования файла
   if(access(TekFile,00))
   {
//Нечего оттуда считывать
     return NULL;
   }



//Проверка даты - только в случае, если определен день месяц и число
   if(FirstTime&&FirstTime->Den&&FirstTime->God&&FirstTime->Mesyac)
   {
     Ret=soprGetDate(TekFile,&Y2,&M2,&D2);
     if(Ret==1)
     {
//Сравним дату
         if(Y2==FirstTime->God)
         {
             if(M2==FirstTime->Mesyac)
             {
                if(D2<FirstTime->Den)return 0;
                if(D2>FirstTime->Den)bDataBolshe=true;

             }else
               if(M2<FirstTime->Mesyac)return 0; else bDataBolshe=true;
         }else
            if(Y2<FirstTime->God)return 0; else bDataBolshe=true;
     }else{
        return 0;
     }
     LastDateTime=*FirstTime; //Скопируем
   }else{
//Скопируем только время
     LastDateTime=*FirstTime;
     LastDateTime.God=1901;
     LastDateTime.Mesyac=1;
     LastDateTime.Den=1;
   }


   fOpen=fopen(TekFile,"rb");
   if(!fOpen)return NULL;

//Будем последовательно скачивать
   do
   {
//Получим данные о смещении
      PDO->IshodDannye.FileNumber=(int)(FirstTime->Time);
      PDO->IshodDannye.Smesh=ftell(fOpen);

//Получим строку
      RetStr=fgets(Strka, 299, fOpen);
      if(!RetStr)
      {
         fclose(fOpen);
         return NULL;  //Ничего нет в данном файле
      }
//Обрезание строки от символов 13,10
      for(i=0;Strka[i]!='\0';i++)if(Strka[i]==13||Strka[i]==10){Strka[i]=0; break;}

//Разберем строку
      lRet=DataFromStr(Strka,&LastDateTime,*PDO);
      if(lRet)
      {
//Проверим. Если время меньше то выйдем
         if(bDataBolshe)return fOpen;

//А тут проверим время
         if(FirstTime->Time>PDO->DateTime.Time)
         {
            fclose(fOpen);
            return NULL;
         }
         LastFirstDateTime=PDO->DateTime;
         return fOpen;  //Возвращаем открытый файл
      }
   }while(!feof(fOpen));
   fclose(fOpen);
   return NULL;
}


/*privGetDataFromFile - функция чтения данных из файла*/
int TSoprSaveSession::privGetDataFromFile(TPaketData *PDO)
{
  char Strka[300],*RetStr;
  int Ret;
  int i;
  while(!feof(fOpen))
  {
     RetStr=fgets(Strka,299,fOpen);
     if(!RetStr) return 0;
     for(i=0;Strka[i]!='\0';i++)if(Strka[i]==13||Strka[i]==10){Strka[i]=0; break;}
     Ret=DataFromStr(Strka,&LastDateTime, *PDO);
     if(Ret==1)return 1;
  };
  return 0;
}

/*Заполнение элементов DTForEveryHour по данным директории*/
/*Получение данных о времени*/
int TSoprSaveSession::GetDTForEveryHour(char *TekPapka, //Полный путь
                                         int iType)     //типа отсчета
{
   int i,j,Ret;
   int M2,D2,Y2;
   char Strka[1500];
   if(iType==0)
   {
//Пройдемся по всем каталогам
       for(i=1;i<=31;i++)
       {
          for(j=0;j<24;j++)
          {
//Делаем путь к файлу
             sprintf(Strka,"%s%c%02d%c%s%02d.%s",
    TekPapka, cRazdelitel,i,cRazdelitel,csPrefix,j,csExt);
             Ret=soprGetDate(Strka,&Y2,&M2,&D2);
             if(Ret==1)
             {
                DTForEveryHour[i][j].God=Y2;
                DTForEveryHour[i][j].Mesyac=M2;
                DTForEveryHour[i][j].Den=D2;
                DTForEveryHour[i][j].Time=j;
             }else{
                DTForEveryHour[i][j].God=0;
                DTForEveryHour[i][j].Mesyac=0;
                DTForEveryHour[i][j].Den=0;
                DTForEveryHour[i][j].Time=0;
             }
          }
       }
       return 1;
   }

   if(iType==1)
   {
          for(j=0;j<24;j++)
          {
//Делаем путь к файлу
             sprintf(Strka,"%s%c%s%02d.%s",
    TekPapka, cRazdelitel,csPrefix,j,csExt);
             Ret=soprGetDate(Strka,&Y2,&M2,&D2);
             if(Ret==1)
             {
                DTForEveryHour[0][j].God=Y2;
                DTForEveryHour[0][j].Mesyac=M2;
                DTForEveryHour[0][j].Den=D2;
                DTForEveryHour[0][j].Time=j;
             }else{
                DTForEveryHour[0][j].God=0;
                DTForEveryHour[0][j].Mesyac=0;
                DTForEveryHour[0][j].Den=0;
                DTForEveryHour[0][j].Time=0;
             }
          }
       return 1;   
   }
   return 0;
}

