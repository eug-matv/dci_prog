//---------------------------------------------------------------------------


#pragma hdrstop

#include <math.h>
#include "AnyTools.h"

//---------------------------------------------------------------------------
#define ABS(X) (((X) > 0) ? (X) : (-(X)))
#define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X,Y) (((X) > (Y)) ? (Y) : (X))



#pragma package(smart_init)

long Okrugl(double Chislo1)
{
 long Chislo2;
 double Chislo3;
 Chislo2=Chislo1;
 Chislo3=Chislo1-Chislo2;
 if(Chislo3<0.5)return (long)Chislo2;
 return ((long)Chislo2+1);
}

int GetTochkiNaGranicePriam(double X1,double Y1,double X2, double Y2, //Координаты прямоq
                       long X1r, long Y1r, long X2r, long Y2r,  //Координаты прямоуголльника
                       long *X1ret,long *Y1ret, long *X2ret, long *Y2ret) //Координаты возвращаемый
{
  double Kx,Ky; //коэффициент наклона
  double Bx,By; //свободный член
  long Y,X;
  double Vrem;

  *X1ret=-10000; *Y1ret=-10000; *X2ret=-10000; *Y2ret=-10000;
//Если Y/X<=1, то построим Y(X)... иначе X от Y
  if(ABS(X2-X1)>=1&&ABS(Y2-Y1)>=1)
  {
     if(X1>X2)
     {
       Vrem=X1; X1=X2;X2=Vrem;
       Vrem=Y1; Y1=Y2;Y2=Vrem;
     }
     Ky=(Y2-Y1)/(X2-X1);
     Kx=(X2-X1)/(Y2-Y1);
     By=Y2-Ky*X2;
     Bx=X2-Kx*Y2;

//Найдем пересечения с X1r
     X=X1r;
     Y=Okrugl(Ky*X1r+By);
     if(Y>=Y1r&&Y<=Y2r)
     {
       *X1ret=X; *Y1ret=Y;
     }else{
       if(Ky<0)Y=Y2r; else Y=Y1r;
       X=Okrugl(Kx*Y+Bx);
       if(X>=X1r&&X<=X2r)
       {
         *X1ret=X; *Y1ret=Y;
       }else{
         return 0;
       }
     }
     X=X2r;
     Y=Okrugl(Ky*X2r+By);
     if(Y>=Y1r&&Y<=Y2r)
     {
       *X2ret=X; *Y2ret=Y;
     }else{
       Y=Y2r;
       if(Ky<0)Y=Y1r; else Y=Y2r;
       X=Okrugl(Kx*Y+Bx);
       if(X>=X1r&&X<=X2r)
       {
         *X2ret=X; *Y2ret=Y;
       }else{
         return 0;
       }
     }
  }else if(ABS(X1-X2)<1)  //Вертикальная линия
  {
      if((X1+X2)/2>=X1r&&(X1+X2)/2<=X2r)
      {
         *X1ret=*X2ret=Okrugl((X1+X2)/2); *Y1ret=Y1r; *Y2ret=Y2r;
      }else{
        return 0;
      }
  }else{
      if((Y1+Y2)/2>=Y1r&&(Y1+Y2)/2<=Y2r)
      {
         *Y1ret=*Y2ret=Okrugl((Y1+Y2)/2); *X1ret=X1r; *X2ret=X2r;
      }else{
        return 0;
      }

  }
  return 1;
}


//Поиск пересечений окружности и линии
int GetPixelsForCircleAndLine(
             double X1, double Y1, double X2, double Y2,
             double X0,  double Y0,  double radius,  //Данные окружности
             double X[], double Y[], int *N_of_R)
{
   double kvY, kvX;
   double rY,rX;
   int N_of_R1=0;
   if(X1>X2-0.001&&X1<X2+0.001)
   {
//Вертикальная линия
     kvY=radius*radius-(X0-X1)*(X0-X1);
     if(kvY<0.00000000000000000001)return 0;
     kvY=sqrt(kvY);
     rY=Y0-kvY;
     if(Y2<Y1&&rY<=Y2&&rY>=Y1||rY<=Y1&&rY>=Y2)
     {
        X[N_of_R1]=X1;
        Y[N_of_R1]=rY;
        N_of_R1++;
     }

     rY=Y0+kvY;
     if(Y2<Y1&&rY<=Y2&&rY>=Y1||rY<=Y1&&rY>=Y2)
     {
        X[N_of_R1]=X1;
        Y[N_of_R1]=rY;
        N_of_R1++;
     }
     if(N_of_R)*N_of_R=N_of_R1;
     return N_of_R1;
   }

   if(Y1>Y2-0.001&&Y1<Y2+0.002)
   {
     kvX=radius*radius-(Y0-Y1)*(Y0-Y1);
     if(kvX<0.00000000000000000001)return 0;
     kvX=sqrt(kvX);
     rX=X0-kvX;
     if(X2<X1&&rX<=X1&&rX>=X2||rX<=X2&&rX>=X1)
     {
        X[N_of_R1]=rX;
        Y[N_of_R1]=Y1;
        N_of_R1++;
     }

     rX=X0+kvX;
     if(X2<X1&&rX<=X1&&rX>=X2||rX<=X2&&rX>=X1)
     {
        X[N_of_R1]=rX;
        Y[N_of_R1]=Y1;
        N_of_R1++;
     }
     if(N_of_R)*N_of_R=N_of_R1;
     return N_of_R1;
   }

//Пока не реализована процедура поиска пересечений с наклонными прямыми

     if(N_of_R)*N_of_R=N_of_R1;
     return N_of_R1;
}




//Возвращает число пересечений
int GetPixelsForCircleAndRect(
       double X1r, double Y1r, double X2r, double Y2r, //Координаты точки
       double X0,  double Y0,  double radius,  //Данные окружности
       double X[], double Y[], int *N_of_R)
{
  int N_of_R1=0;

//Пойдем почасовй стрелке
  N_of_R1+=GetPixelsForCircleAndLine(X1r,Y1r,X2r,Y1r,X0,Y0,radius,
                                     X+N_of_R1, Y+N_of_R1,NULL);

  N_of_R1+=GetPixelsForCircleAndLine(X2r,Y1r,X2r,Y2r,X0,Y0,radius,
                                     X+N_of_R1, Y+N_of_R1,NULL);

  N_of_R1+=GetPixelsForCircleAndLine(X1r,Y2r,X2r,Y2r,X0,Y0,radius,
                                     X+N_of_R1, Y+N_of_R1,NULL);

  N_of_R1+=GetPixelsForCircleAndLine(X1r,Y1r,X1r,Y2r,X0,Y0,radius,
                                     X+N_of_R1, Y+N_of_R1,NULL);
  if(N_of_R)*N_of_R=N_of_R1;
  return N_of_R1;                                   

}




/*Находит под отрезок, который принадлежит данной прямой*/
int ObrezatPriamyeNaGranice(double X1,double Y1,double X2, double Y2, //Координаты прямоq
                       long X1r, long Y1r, long X2r, long Y2r,  //Координаты прямоуголльника
                       long *X1ret,long *Y1ret, long *X2ret, long *Y2ret) //Координаты возвращаемый
{
  double Kx,Ky; //коэффициент наклона
  double Bx,By; //свободный член
  long Y,X;
  double Vrem;
  long TochkaVnutri[2]={0,0};  //Указвает, что точка внутри


  *X1ret=-10000; *Y1ret=-10000; *X2ret=-10000; *Y2ret=-10000;
  if(X1>=X1r&&X1<=X2r&&Y1>=Y1r&&Y1<=Y2r)
  {
    if(X2>=X1r&&X2<=X2r&&Y2>=Y1r&&Y2<=Y2r)
    {
      *X1ret=Okrugl(X1);    *X2ret=Okrugl(X2);
      *Y1ret=Okrugl(Y1);    *Y2ret=Okrugl(Y2);
       return 1;
    }
    TochkaVnutri[0]=1;
  }else{
    if(X2>=X1r&&X2<=X2r&&Y2>=Y1r&&Y2<=Y2r)
    {
      TochkaVnutri[1]=1;
    }
  }

//Если Y/X<=1, то построим Y(X)... иначе X от Y
  if(ABS(X2-X1)>=1&&ABS(Y2-Y1)>=1)
  {
     if(X1>X2)
     {
       Vrem=X1; X1=X2;X2=Vrem;
       Vrem=Y1; Y1=Y2;Y2=Vrem;
       Vrem=TochkaVnutri[0];TochkaVnutri[0]=TochkaVnutri[1];
       TochkaVnutri[1]=Vrem;
     }
     Ky=(Y2-Y1)/(X2-X1);
     Kx=(X2-X1)/(Y2-Y1);
     By=Y2-Ky*X2;
     Bx=X2-Kx*Y2;

//Найдем пересечения с X1r
     if(TochkaVnutri[0])
     {
        *X1ret=Okrugl(X1);
        *Y1ret=Okrugl(Y1);
     }else{
        X=X1r;
        Y=Okrugl(Ky*X1r+By);
        if(Y>=Y1r&&Y<=Y2r)
        {
          *X1ret=X; *Y1ret=Y;
        }else{
          if(Ky<0)Y=Y2r; else Y=Y1r;
          X=Okrugl(Kx*Y+Bx);
          if(X>=X1r&&X<=X2r)
          {
            *X1ret=X; *Y1ret=Y;
          }else{
            return 0;
          }
        }
     }

     if(TochkaVnutri[1])
     {
        *X2ret=Okrugl(X2);
        *Y2ret=Okrugl(Y2);
     }else{
        X=X2r;
        Y=Okrugl(Ky*X2r+By);
        if(Y>=Y1r&&Y<=Y2r)
        {
          *X2ret=X; *Y2ret=Y;
        }else{
          Y=Y2r;
          if(Ky<0)Y=Y1r; else Y=Y2r;
          X=Okrugl(Kx*Y+Bx);
          if(X>=X1r&&X<=X2r)
          {
            *X2ret=X; *Y2ret=Y;
          }else{
            return 0;
          }  
        }
     }

  }else if(ABS(X1-X2)<1)  //Вертикальная линия
  {

      if((X1+X2)/2>=X1r&&(X1+X2)/2<=X2r)
      {
         *X1ret=*X2ret=Okrugl((X1+X2)/2);
         *Y1ret=MAX(Y1r,Okrugl(Y1));
         *Y2ret=MIN(Y2r,Okrugl(Y2));
      }else{
        return 0;
      }

  }else{
      if((Y1+Y2)/2>=Y1r&&(Y1+Y2)/2<=Y2r)
      {
         *Y1ret=*Y2ret=Okrugl((Y1+Y2)/2);
         *X1ret=MAX(X1r,Okrugl(X1));
         *X2ret=MIN(X2r,Okrugl(X2));
      }else{
        return 0;
      }
  }
//Последняя проверка того принадлежит ли отрезок прямой
  if(X1<X2)
  {
     if(*X1ret>X2||*X1ret<X1||*X2ret>X2||*X2ret<X1)return 0;
  }else{
     if(*X1ret>X1||*X1ret<X2||*X2ret>X1||*X2ret<X2)return 0;
  }


  return 1;
}




//Принадлежит ли точка отрезку
int PixelAndLine(int X1,int Y1,  //Первая координата отрезка
                 int X2,int Y2,  //Вторая кооордината отрезук
                 int Xp, int Yp, //Координата точки
                 int Tochnost)   //Пиксель
{
  double Koef;
  int dX,dY,X,Y;
  if(X2<X1)
  {
    int Prom;
    Prom=X1; X1=X2; X2=Prom; Prom=Y1; Y1=Y2; Y2=Prom;
  }

  if(X1=X2&&Y1==Y2)
  {
     return 0;
  }

  dX=X2-X1;
  dY=Y2-Y1;
  if(dX<ABS(dY))
  {
     Koef=((double)dX)/dY;
     if(Xp<X1-Tochnost||Xp>X2+Tochnost)return 0;
     if(Y1<Y2&&(Yp<Y1-Tochnost||Yp>Y2+Tochnost))return 0;
     if(Y1>Y2&&(Yp<Y2-Tochnost||Yp>Y1+Tochnost))return 0;
     X=X1+Okrugl((Yp-Y1)*Koef);
     if(ABS(X-Xp)<=Tochnost)return 1;
  }else{
     Koef=((double)dY)/dX;
     if(Xp<X1-Tochnost||Xp>X2+Tochnost)return 0;
     if(Y1<Y2&&(Yp<Y1-Tochnost||Yp>Y2+Tochnost))return 0;
     if(Y1>=Y2&&(Yp<Y2-Tochnost||Yp>Y1+Tochnost))return 0;
     Y=Y1+Okrugl((Xp-X1)*Koef);
     if(ABS(Y-Yp)<=Tochnost)return 1;
  }
  return 0;
}
