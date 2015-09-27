//---------------------------------------------------------------------------


#pragma hdrstop
#include <math.h>
#include "sopr_doptools.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#define ABS(X) (((X) > 0) ? (X) : (-(X)))


#define SQRT_1_3 0.577350269189625764509

#define SQRT_3  1.73205080756887729352


/*Возвращает время в часах*/
double doptools_DVremyaChas(double H1, double H2)
{
   if(H1>23.9 && H2<0.02)
   {
    return H2+23.0-H1;
   }

   if(H2>23.9 && H1<0.02)
   {
    return -(H1+23.0-H2);
   }

   return H2-H1;

}




double doptools_AbsRaznizaAzimutov(double Azmt1, double Azmt2)
{
   double dAzmt=ABS(Azmt2-Azmt1);
   if(dAzmt>180)dAzmt=360-dAzmt;
   return dAzmt;

}

long doptools_TestVyhodZaSektor(
                                 double CurAzmt,double CurTime,    //Текущее время
                                 double PrevAzmt, double PrevTime, //Предыдущее время
                                 double SektorAzmt,
                                 double TempObzora_ch)            //Период обзора
{
    double dTime=CurTime-PrevTime;
    if(dTime<0)
    {
      if(-dTime<TempObzora_ch/4)return 0;
      dTime+=24.0; //Вдруг был переход на новые сутки
    }

    if(dTime>TempObzora_ch/4)return 1;
    if(CurAzmt<PrevAzmt)
    {
//Вдруг был переход на новый обзор
      if(CurAzmt-PrevAzmt+360.0>SektorAzmt)return 1;
    }else{
      if(CurAzmt-PrevAzmt>SektorAzmt)return 1;
    }
    return 0;
}



/*Получение значений азимутов и дальности по деккартовым координатам*/
long doptools_GetAzimutDalnostFromXY(
     long X_m, long Y_m,      //Деккартовы координаты в метрах
     double &Azmt_gr, double &Dlnst_km //Значения азимута и дальности в градусах и км
                              )
{
   double X_km,Y_km;
   if(X_m==0&&Y_m==0)
   {
      Azmt_gr=0;
      Dlnst_km=0;
      return 1;
   }
   X_km=X_m/1000.0;
   Y_km=Y_m/1000.0;
   Dlnst_km=sqrt(X_km*X_km+Y_km*Y_km);
   Azmt_gr=atan2(X_km,Y_km)/M_PI*180.0;
   if(Azmt_gr<0)Azmt_gr+=360.0;
   return 1;
}

//Получение номера 30 сектора по деккартовым координатам
long doptools_GetNomerSektor30(
            double X, double Y  //Координаты X, Y
                              )
{
   double dY_X, dX_Y;
   if(X<0.001 && X>-0.001)
   {
      if(Y<0.001&&Y>-0.001)
      {
          return 0;
      }
   }


   if(ABS(X)>ABS(Y))
   {
      dY_X=Y/X;
      if(X>0)
      {
        if(dY_X>=0.0)
        {
           if(dY_X<=SQRT_1_3)return 2;
           return 1;
        }else{
           if(dY_X>-SQRT_1_3)return 3;
           return 4;
        }
      }else{
        if(dY_X>=0.0)
        {
           if(dY_X<=SQRT_1_3)return 8;
           return 7;
        }else{
           if(dY_X>-SQRT_1_3)return 9;
           return 10;
        }
      }
   }else{
      dX_Y=X/Y;
      if(Y>0)
      {
         if(dX_Y>=0.0)
         {
            if(dX_Y<SQRT_1_3)return 0;
            return 1;
         }else{
            if(dX_Y>=-SQRT_1_3)return 11;
            return 10;
         }
      }else{
         if(dX_Y>=0.0)
         {
            if(dX_Y<SQRT_1_3)return 6;
            return 7;
         }else{
            if(dX_Y>-SQRT_1_3)return 5;
            return 4;
         }
      }
   }

}


