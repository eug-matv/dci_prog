//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <dir.h>
#include <math.h>


#include <string>
#include <fstream>
#include <sstream>


#include "paketstruct.h"
#include "soprforworkfile.h"
#include "win_str_dir_utils.h"



//---------------------------------------------------------------------------
#define ABS(x) ((x)>=0?(x):(-(x)))



#pragma package(smart_init)

using namespace std;


//Извлечь дату из файла, которая записана в первой строке
//Вспомогательная процедура
//Вх параметры: char *FileName
//Вых параметры: Y, M,D - год, месяц, день
//Возвращает 0, если нет файла. -1 файл есть но дата не получена, 1 - дата получена
int soprGetDate(const string &FileName,
                int &Y,
                int &M,
                int &D)
{
  fstream fstrm;
  int iRet;

  char c1,c2;

  if (!FileExists(String(FileName.c_str())))
  {
        return 0;
  }

   fstrm.open(FileName.c_str(), ios::in);
   if(!fstrm.is_open())return 0;


   fstrm.seekg(47);

   iRet = fstrm.tellg();

   if(iRet == 47)
   {
        fstrm.close();
        return (-1);
   }

   fstrm>>D>>c1>>M>>c2>>Y;

   if (!fstrm.good())
   {
        fstrm.close();
        return (-1);
   }
   fstrm.close();
   return 1;
}


//Удаление файлов не заданной даты
//Возвращает если все подчищено
int soprDelFiles(const string &path,    //Путь к файлам
                 const string &prefix,  //Префикс
                 const string &ext,     //Расширение
                 char Razdelit,
                 int Y, int M, int D)
{

  int i;
  string tmpStr;
  int M1,Y1,D1;
  int Ret;
  stringstream ss;


  if(!DirectoryExists(String(path.c_str())))
  {
        return 0;
  }


  for(i=0;i<24;i++)
  {
    ss.str(string());
    ss<<path<<Razdelit<<prefix;
    ss.width(2);
    ss.fill('0');
    ss.setf(ios::fixed);
    ss<<i;
    ss<<ext;
    tmpStr=ss.str();
    Ret=soprGetDate(tmpStr,Y1,M1,D1);
    if(Ret==1)
    {
      if(Y1!=Y||M1!=M||D1!=D)
      {
         DeleteFileA(tmpStr.c_str());
      }
    }
    if(Ret==-1)
    {
        DeleteFileA(tmpStr.c_str());
    }
  }
  return 1;
}





/*Записывает в строку данные*/


long __stdcall soprSaveToFileOtmetka(
            fstream &fstrm,         //Размер строки должен быть не меньше 150 - переполнение
            TPaketData &_PaketData,
            bool bVyvodAmpl, //Выводить амлитуду для первичного канала,
            bool bSkorostOut, //Выводить данные о скоростях
            bool bDopDopInfo      //Выводить правильную информацию о значениях высоты и номерхо
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
  stringstream ss;
  string tmpStr;


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

  fstrm<<" 50 |";
  fstrm.width(3);
  fstrm.setf(ios::right, ios::adjustfield);
  fstrm.fill(' ');
  fstrm<<_PaketData.Otmetka.IndexInObzor;

//  fstrm.width(2);
//strMAURC,  strPS,    strNHNH,   strAO,
  fstrm<<string("|")<<string(strMAURC);
  fstrm<<string("|")<<string(strPS);
  fstrm<<string("|")<<string(strNHNH);
  fstrm<<string("|")<<string(strAO)<<string("|");

//  _PaketData.Otmetka.sKodB - длина 4
  fstrm.width(4);
  fstrm.fill(' ');
  fstrm<<_PaketData.Otmetka.sKodB;
  fstrm.width(1);
  fstrm<<string("|");

//  OstatokToplivaOut длина = 3
  fstrm.width(3);
  fstrm.fill(' ');
  fstrm<<OstatokToplivaOut;
  fstrm.width(1);
  fstrm<<string("|");

//_PaketData.Otmetka.lNomerTrassy%0x10000 в 16 ричном виде, длина 4
  fstrm.setf(ios::hex,ios::basefield);
  fstrm.width(4);
  fstrm.fill('0');
  fstrm<<_PaketData.Otmetka.lNomerTrassy;
  fstrm.setf(ios::dec,ios::basefield);
  fstrm.width(1);
  fstrm<<string("|");

//_PaketData.Otmetka.lNomer длина 5
  fstrm.width(5);
  fstrm.fill(' ');
  fstrm << _PaketData.Otmetka.lNomer;
  fstrm.width(1);
  fstrm << "|";

//  _PaketData.Otmetka.lVysota длина 7
  fstrm.width(7);
  fstrm.fill(' ');
  fstrm << _PaketData.Otmetka.lVysota;
  fstrm.width(1);
  fstrm << "|";

//  _PaketData.Otmetka.X длина 7
  fstrm.width(7);
  fstrm.fill(' ');
  fstrm << _PaketData.Otmetka.X;
  fstrm << "|";


//  _PaketData.Otmetka.Y длина 7
  fstrm.width(7);
  fstrm.fill(' ');
  fstrm << _PaketData.Otmetka.Y;
  fstrm << "| ";

//  _PaketData.Azimut,
  fstrm.width(6);
  fstrm.fill(' ');
  fstrm.setf(ios::floatfield,ios::fixed);
  fstrm.precision(2);
  fstrm << _PaketData.Azimut;

  fstrm << " | ";
//_PaketData.Otmetka.Dalnost,
  fstrm.width(6);
  fstrm.fill(' ');
  fstrm.setf(ios::floatfield,ios::fixed);
  fstrm.precision(2);
  fstrm << _PaketData.Otmetka.Dalnost;

  fstrm.width(3);
  fstrm << " | ";
// Time is out
// H - hours
  fstrm.width(2);
  fstrm.fill('0');
  fstrm << H;
  fstrm.width(1);
  fstrm << string(":");
// M - minutes
  fstrm.width(2);
  fstrm.fill('0');
  fstrm << M;
  fstrm.width(1);
  fstrm << string(":");
// S - seconds
  fstrm.width(6);
  fstrm.fill('0');
  fstrm.setf(ios::floatfield,ios::fixed);
  fstrm.precision(3);
  fstrm << S;
  fstrm.width(2);
  fstrm << string(" |");

//Проверим надо ли печатать дополнительные данные

  if(bVyvodAmpl&&_PaketData.Otmetka.Kanal==Kanal_P)
  {
   //Выведем значение амилитуды
        fstrm.width(1);
        fstrm << string(" ");

        fstrm.width(8);
        fstrm.fill(' ');
        fstrm.setf(ios::fixed | ios::scientific, ios::floatfield);
        fstrm.precision(2);
        fstrm << _PaketData.Otmetka.dAmpl;
        fstrm.unsetf(ios::scientific);
        fstrm.width(2);
        fstrm << string(" |");


  }else{
     if(bSkorostOut||bDopDopInfo)
     {
//Выведем 10 пробелов
       fstrm.width(11);
       fstrm<<string("          |");
     }
  }



  if(bSkorostOut)
  {
//Выведем информацию о векторах скоростей
     fstrm.width(5);
     fstrm.fill(' ');
     fstrm<<_PaketData.Otmetka.Vx;
     fstrm.width(1);
     fstrm<<string("|");
     fstrm.width(5);
     fstrm.fill(' ');
     fstrm<<_PaketData.Otmetka.Vy;
     fstrm.width(1);
     fstrm<<string("|");
  }else{
     if(bDopDopInfo)
     {
        fstrm.width(6);
        fstrm<<string("     |");
        fstrm.width(6);
        fstrm<<string("     |");
     }
  }


  if(bDopDopInfo)
  {
     fstrm.width(5);
     fstrm.fill(' ');
     fstrm<<_PaketData.Otmetka.lDopNumber;
     fstrm.width(1);
     fstrm<<string("|");
     fstrm.width(7);
     fstrm.fill(' ');
     fstrm<<_PaketData.Otmetka.lDopVysota;
     fstrm.width(1);
     fstrm<<string("|");
  }

//Добавлено 25.01.2008
//Выведем еще дополнительную информацию
  if(_PaketData.Otmetka.N_of_MustPO||
     _PaketData.Otmetka.N_of_MustSO)
  {
//Если есть информация о первичном канале
    if(_PaketData.Otmetka.N_of_MustPO>0)
    {
       fstrm.width(4);
       fstrm.fill(' ');
       fstrm<<_PaketData.Otmetka.N_of_PO;
       fstrm.width(1);
       fstrm<<string("|");

       fstrm.width(4);
       fstrm.fill(' ');
       fstrm<<_PaketData.Otmetka.N_of_MustPO;
       fstrm.width(1);
       fstrm<<string("|");


       if(_PaketData.Otmetka.P_PO>=0.0)
       {
           fstrm.width(4);
           fstrm.fill(' ');
           fstrm.setf(ios::fixed, ios::floatfield);
           fstrm.precision(2);
           fstrm<<_PaketData.Otmetka.P_PO;
           fstrm.width(1);
           fstrm<<string("|");
       }else{
           fstrm.width(5);
       }   fstrm<<string("   ?|");
    }else{
       fstrm.width(15);
       fstrm << string("    |    |    |");
    }
    if(_PaketData.Otmetka.N_of_MustSO>0)
    {
       fstrm.width(4);
       fstrm.fill(' ');
       fstrm<<_PaketData.Otmetka.N_of_SO;
       fstrm.width(1);
       fstrm<<string("|");

       fstrm.width(4);
       fstrm.fill(' ');
       fstrm<<_PaketData.Otmetka.N_of_MustSO;
       fstrm.width(1);
       fstrm<<string("|");
       if(_PaketData.Otmetka.P_SO>=0.0)
       {
           fstrm.width(4);
           fstrm.fill(' ');
           fstrm.setf(ios::fixed, ios::floatfield);
           fstrm.precision(2);
           fstrm<<_PaketData.Otmetka.P_SO;
           fstrm.width(1);
           fstrm<<string("|");
       }else{
           fstrm.width(5);
           fstrm<<string("   ?|");
       }
    }else{
       fstrm.width(15);
       fstrm << string("    |    |    |");
    }
  }
  fstrm << endl;
  return 1;
}





/*Сохранение севера файл с помошью открытого дескриптора fstrm*/
long __stdcall soprSaveToFileSever
(
    fstream &fstrm,
    short God, short Mesyac, short Day,  //Год месяц день
    const string &sAddedText,
    short H, short M, double S   //Время в часах, минутах и секундах
)
{

  int i,j,k;
  string tmpStr;
  fstrm.width(8);
  fstrm<<string("NORD -- ");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm<<Day;
  fstrm.width(1);
  fstrm<<string(".");
  fstrm.width(2);
  fstrm.fill('0');
  fstrm<<Mesyac;
  fstrm.width(1);
  fstrm<<string(".");
  fstrm.width(4);
  fstrm.fill('0');
  fstrm<<God;
  fstrm.width(4);
  fstrm<<string(" -- ");


  if (sAddedText.length() > 64)
  {
     tmpStr = sAddedText.substr(0,64);
  }else{
     tmpStr = sAddedText;
  }


  fstrm.setf(ios::left, ios::adjustfield);
  fstrm.width(64);
  fstrm.fill(' ');
  fstrm << tmpStr;
  fstrm.setf(ios::right, ios::adjustfield);

  fstrm.width(3);
  fstrm << string(" | ");

// H - hours
  fstrm.width(2);
  fstrm.fill('0');
  fstrm << H;
  fstrm.width(1);
  fstrm << string(":");
// M - minutes
  fstrm.width(2);
  fstrm.fill('0');
  fstrm << M;
  fstrm.width(1);
  fstrm << string(":");
// S - seconds
  fstrm.width(6);
  fstrm.fill('0');
  fstrm.setf(ios::floatfield,ios::fixed);
  fstrm.precision(3);
  fstrm << S;
  fstrm.width(2);
  fstrm << string(" |");

  fstrm << endl;
  return 1;
}

//Вывод данных о секторе
long __stdcall soprSaveToFileSektor
(
  fstream &fstrm,         //Размер строки должен быть не меньше 150 - переполнение
  double Azmt,
  short H, short M, double S   //Время в часах, минутах и секундах
)
{

   fstrm.width(7);
   fstrm << string("SECTOR ");

   fstrm.width(6);
   fstrm.setf(ios::floatfield,ios::fixed);
   fstrm.precision(2);
   fstrm << Azmt;

   fstrm.width(3);
   fstrm << string(" | ");

// H - hours
   fstrm.width(2);
   fstrm.fill('0');
   fstrm << H;
   fstrm.width(1);
   fstrm << string(":");

// M - minutes
   fstrm.width(2);
   fstrm.fill('0');
   fstrm << M;
   fstrm.width(1);
   fstrm << string(":");

// S - seconds
   fstrm.width(6);
   fstrm.fill('0');
   fstrm.setf(ios::floatfield,ios::fixed);
   fstrm.precision(3);
   fstrm << S;
   fstrm.width(2);
   fstrm << string(" |");
   fstrm << endl;
   return 1;
}


/*Вывод данных в строку Strka*/
long __stdcall  soprSaveToFile(
            fstream &fstrm,         //Размер строки должен быть не меньше 200 - переполнение
            TPaketData &_PaketData,
            const string &sAddedText,//Дополнительный текст для вывода
            bool bVyvodAmpl, //Выводить амлитуду для первичного канала,
            bool bSkorostOut, //Выводить данные о скоростях
            bool bDopDopInfo      //Выводить правильную информацию о значениях высоты и номерхо
                          )
{
   short M,H;
   double S,Ostatok;
   int lRet;
   switch(_PaketData.TipPaketa)
   {
      case TipPakSektor:
        H=(short)(_PaketData.DateTime.Time);
        Ostatok=(_PaketData.DateTime.Time-H)*60;
        M=(short)(Ostatok);
        S=(Ostatok-M)*60.0;
//Выведем данные
        lRet=soprSaveToFileSektor(
          fstrm,
          _PaketData.Azimut,
          H,M,S);
      break;

      case TipPakSever:
        H=(short)(_PaketData.DateTime.Time);
        Ostatok=(_PaketData.DateTime.Time-H)*60;
        M=(short)(Ostatok);
        S=(Ostatok-M)*60.0;

        lRet=soprSaveToFileSever(
           fstrm,
           _PaketData.DateTime.God,
           _PaketData.DateTime.Mesyac,
           _PaketData.DateTime.Den,
           sAddedText,
           H, M, S);
      break;
      case TipPakOtmetka:
           lRet=soprSaveToFileOtmetka(
                    fstrm,
                    _PaketData,
                   bVyvodAmpl,
                   bSkorostOut,
                   bDopDopInfo);
      break;

      default:
        return 0;
   };


   return lRet;
}




/*Старт запуска открываются нужные файлы. Не нужные затираюся
  Возвращает: 0 - если уже запущена, -1 - если не существует

пути csPutKDannym. -2 - если не успешно записалось в файл.
-3 - ошибка создании папки
1- если успешно был запуск
*/
int TSoprSaveSession::Start(void)
{


  bool Ret;
  char c1,c2;
  int Y1,Dy1,Mth1;
  string sDatePapka;
  string sFileData;
  stringstream ss;

  unsigned short y1,m1,d1, h2,m2,s2,ms2;


  if(bIsInit)return 0;

//Проверим существование каталога csPutKDannym
  Ret = DirectoryExists(String(csPutKDannym.c_str()));


//Попробуем создать папку
  if(!Ret)
  {
  	if(winMakePath(csPutKDannym.c_str()))
  	{
    	Ret = DirectoryExists(String(csPutKDannym.c_str()));
  	}
  }
  if(!Ret)return (-1);

  TDateTime dt = Now();
  dt.DecodeDate(&y1,&m1,&d1);
  dt.DecodeTime(&h2, &m2, &s2, &ms2);



//Узнаем имя папки по дню
  ss << string(csPutKDannym);
  ss << cRazdelitel;
  ss.width(2);
  ss.setf(ios::right, ios::adjustfield);
  ss.fill('0');
  ss << d1;

  sDatePapka = ss.str();

//Проверим существование данной папки
  Ret=DirectoryExists(String(sDatePapka.c_str()));
  if(!Ret)
  {
//Данная папка не существует - то создать ее
     if(mkdir(sDatePapka.c_str()))
     {
        return (-3);
     }
  }


//Попробуем удалить не актуальные файлы в папке
  soprDelFiles(sDatePapka,csPrefix,csExt,cRazdelitel,
    (int)y1,    (int)m1, (int)d1);




//Откроем файл на запись

  sFileData = (sDatePapka + cRazdelitel)+csPrefix;
  ss.str(string());
  ss.width(2);
  ss.fill('0');
  ss << int(h2);

  sFileData = sFileData + ss.str();

  sFileData  = sFileData + string(".")+csExt;



//Создаем файл
  if(!FileExists(String(sFileData.c_str())))
  {
     fstrm.open(sFileData.c_str(),ios::out);
  }else{
     fstrm.open(sFileData.c_str(),ios::out | ios::app);
  }
  if(!fstrm.is_open())return -4;

//Сделаем запись в файл.
//Подготовим строку
  fstrm.setf(ios::right, ios::adjustfield);
#ifdef ENG_LANG

  fstrm <<  string(">>>   Start log : Time  ");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)h2;
  fstrm.width(1);
  fstrm << string(":");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)m2;
  fstrm.width(1);
  fstrm << string(":");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)s2;
  fstrm.width(1);
  fstrm << string("   Date ");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)d1;
  fstrm.width(1);
  fstrm << string(".");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)m1;
  fstrm.width(1);
  fstrm << string(".");

  fstrm.width(4);
  fstrm.fill('0');
  fstrm << (int)y1;
  fstrm.width(1);
  fstrm << endl;

#else

  fstrm <<  string(">>>   Старт протокола : время  ");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)h2;
  fstrm.width(1);
  fstrm << string(":");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)m2;
  fstrm.width(1);
  fstrm << string(":");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)s2;
  fstrm.width(1);
  fstrm << string("   дата ");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)d1;
  fstrm.width(1);
  fstrm << string(".");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)m1;
  fstrm.width(1);
  fstrm << string(".");

  fstrm.width(4);
  fstrm.fill('0');
  fstrm << (int)y1;
  fstrm.width(1);
  fstrm << endl;


#endif

  bIsInit=true;
  iLastHour=h2;
  return 1;
}

/*Остановка данных. Запись прекращается - выдается сообщение об остановке
Запись производится в текущую папку.
*/
int  TSoprSaveSession::Stop(void)
{


  int Ret;
  char c1,c2;
  int Y1,Dy1,Mth1;

  string sDatePapka;
  string sFileData;
  stringstream ss;
  unsigned short y1,m1,d1,h2,m2,s2,ms2;

  if(!bIsInit)return 0;

//Узнаем текущее время
  TDateTime dt = Now();
  dt.DecodeDate(&y1,&m1,&d1);
  dt.DecodeTime(&h2, &m2, &s2, &ms2);





//Проверим существование каталога csPutKDannym
  if(iLastHour!=(int)h2)
  {
     fstrm.close();  //Закроем текущий дескриптор
     Ret=DirectoryExists(String(csPutKDannym.c_str()));
     if(!Ret)return (-1);


//Узнаем имя папки по дню
     sDatePapka =  csPutKDannym+cRazdelitel;
     ss.width(2);
     ss.setf(ios::right, ios::adjustfield);
     ss.fill('0');
     ss << d1;

     sDatePapka = sDatePapka + ss.str();

//Проверим существование данной папки
     Ret=DirectoryExists(String(csPutKDannym.c_str()));
     if(!Ret)
     {
//Данная папка не существует - то создать ее
        if(mkdir(sDatePapka.c_str()))
        {
           return (-3);
        }
     }

     //Попробуем удалить не актуальные файлы в папке
      soprDelFiles(sDatePapka,csPrefix,csExt,cRazdelitel,
        y1,m1,d1);

    //Откроем файл на чтение
     sFileData = (sDatePapka + cRazdelitel)+csPrefix;
     ss.str(string());
     ss.width(2);
     ss.fill('0');
     ss << int(h2);
     sFileData = sFileData + ss.str();
     sFileData  = sFileData + string(".")+csExt;


//Создаем файл
//Создаем файл
    if(!FileExists(String(sFileData.c_str())))
    {
       fstrm.open(sFileData.c_str(),ios::out);
    }else{
       fstrm.open(sFileData.c_str(),ios::out | ios::app);
    }
    if(!fstrm.is_open())return -4;
  }



//Сделаем запись в файл.
//Подготовим строку
  fstrm.setf(ios::right, ios::adjustfield);
#ifdef ENG_LANG

  fstrm << ">>>   End log : Time  ";

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)h2;
  fstrm.width(1);
  fstrm << string(":");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)m2;
  fstrm.width(1);
  fstrm << string(":");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)s2;
  fstrm.width(1);
  fstrm << "   Date ";

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)d1;
  fstrm.width(1);
  fstrm << string(".");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)m1;
  fstrm.width(1);
  fstrm << string(".");

  fstrm.width(4);
  fstrm.fill('0');
  fstrm << (int)y1;
  fstrm.width(1);
  fstrm << endl;


#else

  fstrm << ">>>   Конец протокола : время  ";

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)h2;
  fstrm.width(1);
  fstrm << string(":");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)m2;
  fstrm.width(1);
  fstrm << string(":");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)s2;
  fstrm.width(1);
  fstrm << "   дата ";

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)d1;
  fstrm.width(1);
  fstrm << string(".");

  fstrm.width(2);
  fstrm.fill('0');
  fstrm << (int)m1;
  fstrm.width(1);
  fstrm << string(".");

  fstrm.width(4);
  fstrm.fill('0');
  fstrm << (int)y1;
  fstrm.width(1);
  fstrm << endl;

#endif

//Выведем в файл данные
  fstrm.close();
  bIsInit=false;
  return 1;
}



int TSoprSaveSession::SaveDataToFile(TPaketData &_PaketData)
{

  int Ret;
  char c1,c2;
  int Y1,Dy1,Mth1;
  FILE *fp;
  string sDatePapka;
  string sFileData;
  stringstream ss;
  int SizeOfStrka;
  unsigned short y1,m1,d1,h2,m2,s2,ms2;

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


  TDateTime dt = Now();
  dt.DecodeDate(&y1,&m1,&d1);
  dt.DecodeTime(&h2, &m2, &s2, &ms2);



//Проверим существование каталога csPutKDannym
  if(iLastHour!=(int)h2)
  {
     fstrm.close();  //Закроем текущий дескриптор
     Ret=DirectoryExists(csPutKDannym.c_str());
     if(!Ret)return (-1);

//Узнаем имя папки по дню
     sDatePapka =  csPutKDannym+cRazdelitel;
     ss.width(2);
     ss.setf(ios::right, ios::adjustfield);
     ss.fill('0');
     ss << d1;

     sDatePapka = sDatePapka + ss.str();


//Проверим существование данной папки
     Ret=DirectoryExists(sDatePapka.c_str());
     if(!Ret)
     {
//Данная папка не существует - то создать ее
        if(mkdir(sDatePapka.c_str()))
        {
           return (-3);
        }
     }

  //Попробуем удалить не актуальные файлы в папке
      soprDelFiles(sDatePapka,csPrefix,csExt,cRazdelitel,
            (int)y1,(int)m1,(int)d1);

    //Откроем файл на чтение
     sFileData = (sDatePapka + cRazdelitel)+csPrefix;
     ss.str(string());
     ss.width(2);
     ss.fill('0');
     ss << int(h2);
     sFileData = sFileData + ss.str();
     sFileData  = sFileData + string(".")+csExt;


//Создаем файл
    if(!FileExists(String(sFileData.c_str())))
    {
       fstrm.open(sFileData.c_str(), ios::out);
    }else{
       fstrm.open(sFileData.c_str(), ios::out | ios::app);
    }
    if(!fstrm.is_open())return -4;

     //Сделаем запись в файл.
//Подготовим строку
//Сделаем запись в файл.
//Подготовим строку
    fstrm.setf(ios::right, ios::adjustfield);
#ifdef ENG_LANG

    fstrm <<  string(">>>   Start log : Time  ");

    fstrm.width(2);
    fstrm.fill('0');
    fstrm << (int)h2;
    fstrm.width(1);
    fstrm << string(":");

    fstrm.width(2);
    fstrm.fill('0');
    fstrm << (int)m2;
    fstrm.width(1);
    fstrm << string(":");

    fstrm.width(2);
    fstrm.fill('0');
    fstrm << (int)s2;
    fstrm.width(1);
    fstrm << string("   Date ");

    fstrm.width(2);
    fstrm.fill('0');
    fstrm << (int)d1;
    fstrm.width(1);
    fstrm << string(".");

    fstrm.width(2);
    fstrm.fill('0');
    fstrm << (int)m1;
    fstrm.width(1);
    fstrm << string(".");

    fstrm.width(4);
    fstrm.fill('0');
    fstrm << (int)y1;
    fstrm.width(1);
    fstrm << endl;

#else

    fstrm <<  string(">>>   Старт протокола : время  ");

    fstrm.width(2);
    fstrm.fill('0');
    fstrm << (int)h2;
    fstrm.width(1);
    fstrm << string(":");

    fstrm.width(2);
    fstrm.fill('0');
    fstrm << (int)m2;
    fstrm.width(1);
    fstrm << string(":");

    fstrm.width(2);
    fstrm.fill('0');
    fstrm << (int)s2;
    fstrm.width(1);
    fstrm << string("   дата ");

    fstrm.width(2);
    fstrm.fill('0');
    fstrm << (int)d1;
    fstrm.width(1);
    fstrm << string(".");

    fstrm.width(2);
    fstrm.fill('0');
    fstrm << (int)m1;
    fstrm.width(1);
    fstrm << string(".");

    fstrm.width(4);
    fstrm.fill('0');
    fstrm << (int)y1;
    fstrm.width(1);
    fstrm << endl;
#endif
  }

//Выведем данные в строку
  Ret=soprSaveToFile(fstrm,_PaketData, DopTextOut, bVyvodAmpl,
                                       bSkorostOut, bDopDopInfo);


//Тут осуществляется вывод в файл самих данных подготовленной строки
  iLastHour=(int)h2;
  if(Ret!=1)
  {
    return 0;
  }
  return 1;
}









