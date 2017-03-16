//---------------------------------------------------------------------------
//Описание в "paketstruct.h"

#pragma hdrstop
//#include <windows.h>


#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "paketstruct.h"

//---------------------------------------------------------------------------
#define ABS(x) ((x)>=0?(x):(-(x)))

#pragma package(smart_init)


  TPaketData TPaketData::PaketData[N_OF_FIFO_PAK_STR];   //Размер массива данных
  long TPaketData::InIndex=0;                        //Входной индекс
  long TPaketData::OutIndex=0;                       //Выходной индекс


//Вспомогательная процедура для перевода не правильного числа в правильное
  int pakstructPerevod(
           int God, int Mesyac, int Den, long DSutki,
           int &God1, int &Mesyac1, int &Den1
           )
  {
     struct tm tm1, *tm2;

     time_t tmt;
     tm1.tm_sec=tm1.tm_min=tm1.tm_hour=1;
     tm1.tm_year=God-1900;
     tm1.tm_mon=Mesyac-1;
     tm1.tm_mday=Den;
     tmt=mktime(&tm1);
     tmt=tmt+DSutki*3600*24;
     tm2=localtime(&tmt);
     God1=tm2->tm_year+1900;
     Mesyac1=tm2->tm_mon+1;
     Den1=tm2->tm_mday;
     return 1;
  }



   int TMyDateTime::operator <(TMyDateTime mdt)
   {
      if(God<mdt.God)return 1;
      if(God>mdt.God)return 0;
      if(Mesyac<mdt.Mesyac)return 1;
      if(Mesyac>mdt.Mesyac)return 0;
      if(Den<mdt.Den)return 1;
      if(Den>mdt.Den)return 0;
      if(Time<mdt.Time)return 1;
      return 0;
   }

   int TMyDateTime::operator >(TMyDateTime mdt)
   {
      if(God>mdt.God)return 1;
      if(God<mdt.God)return 0;
      if(Mesyac>mdt.Mesyac)return 1;
      if(Mesyac<mdt.Mesyac)return 0;
      if(Den>mdt.Den)return 1;
      if(Den<mdt.Den)return 0;
      if(Time>mdt.Time)return 1;
      return 0;
   }

   int TMyDateTime::operator ==(TMyDateTime mdt)
   {
      if(God==mdt.God&&Mesyac==mdt.Mesyac&&
         Den==mdt.Den)
      {
        if(ABS(Time-mdt.Time)<0.001/3600.0)return 1;
      }
      return 0;
   }


   int TMyDateTime::operator !=(TMyDateTime mdt)
   {
       return !(*this==mdt);
   }


  TMyDateTime& TMyDateTime::operator =(const TMyDateTime &mdt)
  {
      if(&mdt==this)return *this;
      memcpy(this,&mdt,sizeof(TMyDateTime));
      return *this;
  }


  TMyDateTime& TMyDateTime::operator =(TMyDateTime mdt)
  {
     memcpy(this,&mdt,sizeof(TMyDateTime));
     return *this;
  }



   int TMyDateTime::operator <=(TMyDateTime mdt)
   {
      if(*this<mdt||*this==mdt)return 1;
      return 0;
   }

   int TMyDateTime::operator >=(TMyDateTime mdt)
   {
      if(*this>mdt||*this==mdt)return 1;
      return 0;
   }



   TMyDateTime TMyDateTime::operator +(double dTime)
   {
      int dSutki=0;    //Насколько суток вперед перевести
      TMyDateTime MyDateTime;
      MyDateTime.God=this->God;
      MyDateTime.Mesyac=this->Mesyac;
      MyDateTime.Den=this->Den;
      MyDateTime.Time=this->Time+dTime;
      if(MyDateTime.Time>24.0-0.0005/3600.0)
      {
         //Следующие сутки
         while(MyDateTime.Time>24.0-0.0005/3600.0)
         {
            MyDateTime.Time-=24.00;
            dSutki++;
         }
         pakstructPerevod(this->God,this->Mesyac,this->Den,dSutki,
           MyDateTime.God,MyDateTime.Mesyac,MyDateTime.Den);
         return MyDateTime;
      }

      if(MyDateTime.Time<0.0)
      {
         //Следующие сутки
         while(MyDateTime.Time<0)
         {
            MyDateTime.Time+=24.00;
            dSutki--;
         }
         pakstructPerevod(this->God,this->Mesyac,this->Den,dSutki,
           MyDateTime.God,MyDateTime.Mesyac,MyDateTime.Den);
         return MyDateTime;
      }
      return MyDateTime;
   }

   TMyDateTime TMyDateTime::operator -(double dTime)
   {
      return (*this+(-dTime));
   }



//Заново инициализировать данные
void ReinitializePaketData(void)
{
  TPaketData::InIndex=TPaketData::OutIndex=0;
  for(int i=0;i<N_OF_FIFO_PAK_STR;i++)
  {
    TPaketData::PaketData[i].TipPaketa=TipPakNeOpredelen;
  }
}

