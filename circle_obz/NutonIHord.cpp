//---------------------------------------------------------------------------
/**/

#pragma hdrstop

#include "NutonIHord.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#define ABS(x) ((x)>0 ? (x) : (-(x)))




long EMMetodHord1(double (*f)(double),
                  double t1,
                  double t2,
                  double dt,
                  double dFt,
                  double *tret)
{
  double dt1,    //Промежуточное значение разниц t1 и t2.
         t_prom; //Промежуточное значение


  double ftProm;
  double t_prev;
  long N_of_Iter=0;
  if(t1>=t2)return 0;
  if(ABS(f(t1))<dFt)
  {
     *tret=t1;
     return 1;
  }

  if(ABS(f(t2))<dFt)
  {
     *tret=t2;
     return 1;
  }



  if(f(t1)>0&&f(t2)>0||
     f(t1)<0&&f(t2)<0)return (-1);


  dt1=t2-t1;


//Пока dt<=dt1 будем сужать диапазон

  if(f(t1)>=0 && f(t2)<=0)
  {
    while(dt<=dt1)
    {
//нарисуем прямую через точки (t1, f(t1)) и (t2, f(t2))
//и найдем пересечение ее с осью f=0
       N_of_Iter++;
       ftProm=f(t2)-f(t1);
       if(ABS(ftProm)<=dFt)break;
       t_prom=t2-f(t2)/ftProm*(t2-t1);

       if(f(t_prom)>0)
       {
          t1=t_prom;
       }else{
          t2=t_prom;
       }

//Сделаем итерацию методом деления пополам
       t_prom=(t1+t2)/2;
       if(f(t_prom)>=0)
       {
           t1=t_prom;
       }else{
           t2=t_prom;
       }
       dt1=t2-t1;
    };
  }else{
    while(dt<=dt1)
    {
//нарисуем прямую через точки (t1, f(t1)) и (t2, f(t2))
//и найдем пересечение ее с осью f=0
       N_of_Iter++;
       ftProm=f(t2)-f(t1);
       if(ABS(ftProm)<=dFt)break;
       t_prom=t2-f(t2)/ftProm*(t2-t1);

       if(f(t_prom)<0)
       {
          t1=t_prom;
       }else{
          t2=t_prom;
       }

       t_prom=(t1+t2)/2;
       if(f(t_prom)<=0)
       {
           t1=t_prom;
       }else{
           t2=t_prom;
       }
       dt1=t2-t1;
    };
  }
//А тут найдем значение
  *tret=(t1+t2)/2.0;

  return N_of_Iter;
}




