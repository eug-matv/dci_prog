//---------------------------------------------------------------------------

#pragma hdrstop
#include <windows.h>
#include <alloc.h>
#include <vcl.h>

#include "DrawObjectList.h"
//#include "Crash_alarm.h"
#include "AnyTools.h"




//---------------------------------------------------------------------------
#pragma package(smart_init)

extern  float pub_dfKoefMasshtab, pub_Radius, pub_koef_rad;


// ******************        Определение прямоугольника    **********************
TDrawRect::TDrawRect(double x1, double y1, double x2, double y2,
                     COLORREF Cvet, UINT Stil, POINT Tolshina, LOGBRUSH Brush_transmit)
{
  X0=x1;
  Y0=y1;
  X1=x2;
  Y1=y2;
  Colour=Cvet;
  lopnStyle=Stil;
  lopnWidth.x=Tolshina.x;
  lopnWidth.y=Tolshina.y;
  zalivka.lbStyle = Brush_transmit.lbStyle;
  zalivka.lbColor = Brush_transmit.lbColor;
  zalivka.lbHatch = Brush_transmit.lbHatch;
}

long TDrawRect::Paint(HDC DC, POINT pntRLS,
               SIZE szArea, double dfKoefMasshtab)
{
  long Ret;
  HPEN Pen,OldPen; //Параметры палитры
  LOGPEN LogPen;
  HBRUSH Brush, OldBrush;
  int OldMode;
  COLORREF OldBkColor;

  // Установим цвет фона

  //Создадим перо
  LogPen.lopnStyle=PS_DASH;	//lopnStyle; //Зададим стил
  LogPen.lopnWidth.x=lopnWidth.x;
  LogPen.lopnWidth.y=lopnWidth.y;
  LogPen.lopnColor=Colour;   //Зададим цвет

  Pen=CreatePenIndirect(&LogPen);
  if(Pen==NULL)return (-1);
  OldPen=SelectObject(DC,Pen);

  Brush=CreateBrushIndirect(&zalivka);
   if(Brush==NULL)
   {
     DeleteObject(Pen);
     return (-1);
   }

   OldPen=SelectObject(DC,Pen);
   OldBrush=SelectObject(DC,Brush);
   OldMode=SetBkMode(DC,TRANSPARENT);

  //Выведем прямоугольник...
  Rectangle(DC, X0, Y0, X1, Y1);
  OldMode=SetBkMode(DC,OldMode);

  SelectObject(DC,OldPen);
  SelectObject(DC,OldBrush);
  DeleteObject(Brush);
  DeleteObject(Pen);
  return 1;
}

long TDrawRect::PaintOneMore(HDC DC)
{
  long Ret;
  HPEN Pen,OldPen; //Параметры палитры
  LOGPEN LogPen;
  COLORREF cOldText;

  // Установим цвет фона


  //Создадим перо
  LogPen.lopnStyle=lopnStyle; //Зададим стил
  LogPen.lopnWidth.x=lopnWidth.x;
  LogPen.lopnWidth.y=lopnWidth.y;
  LogPen.lopnColor=Colour;   //Зададим цвет

  Pen=CreatePenIndirect(&LogPen);
  if(Pen==NULL)return (-1);
  OldPen=SelectObject(DC,Pen);

  //Выведем прямоугольник...
  Rectangle(DC, X0, Y0, X1, Y1);

  SelectObject(DC,OldPen);
  DeleteObject(Pen);
  return 1;
}


void TDrawRect::GetClassName(char *ClassName, long szClassName)
{
  if(szClassName>=10)
  {
    strcpy(ClassName,"TDrawRect");
    return;
  }
  if(szClassName>=1)
  {
     memcpy(ClassName,"szClassName",szClassName-1);
     ClassName[szClassName-1]=0;
     return;
  }
  ClassName[0]=0;
}

void TDrawRect::GetLocation(double *xl,double *yl)
{
    *xl =X0;
    *yl =Y0;
}




// ******************               Определение линии           **********************
TDrawLine::TDrawLine(double x1, double y1, double x2, double y2,
                     COLORREF Cvet, UINT Stil, POINT Tolshina)
{
  X0=x1;
  Y0=y1;
  X1=x2;
  Y1=y2;
  Colour=Cvet;
  lopnStyle=Stil;
  lopnWidth.x=Tolshina.x;
  lopnWidth.y=Tolshina.y;

}


long TDrawLine::Paint(HDC DC, POINT pntRLS,
               SIZE szArea, double dfKoefMasshtab)
{
  double x11,y11,x12,y12;
  long lx11,ly11,lx12,ly12;
  long Ret;
  HPEN Pen,OldPen; //Параметры палитры
  LOGPEN LogPen;
  COLORREF cOldText;
  int OldBkMode;

 //Расчитать координаты относительно Левого верхнего угла
  x11=pntRLS.x+X0*dfKoefMasshtab;
  y11=pntRLS.y-Y0*dfKoefMasshtab;
  x12=pntRLS.x+X1*dfKoefMasshtab;
  y12=pntRLS.y-Y1*dfKoefMasshtab;

  lx11=Okrugl(x11);  lx12=Okrugl(x12);
  ly11=Okrugl(y11);  ly12=Okrugl(y12);

  Ret=ObrezatPriamyeNaGranice(lx11,ly11,lx12,ly12,
                               0,0, szArea.cx-1,szArea.cy-1,
                               &iX0,&iY0,&iX1,&iY1);
/*
  Ret=GetTochkiNaGranicePriam(lx11,ly11,lx12,ly12,
                               0,0, szArea.cx-1,szArea.cy-1,
                               &iX0,&iY0,&iX1,&iY1);*/
  if(Ret!=1)
  {
      iX0=iX1=iY0=iY1=-1;
      return 0;
  }

  //Создадим перо
   LogPen.lopnStyle = lopnStyle; //Зададим стиль
   LogPen.lopnWidth.x = lopnWidth.x;
   LogPen.lopnWidth.y = lopnWidth.y;
   LogPen.lopnColor = Colour;   //Зададим цвет

   Pen = CreatePenIndirect(&LogPen);
   if(Pen==NULL)return (-1);
   OldPen=SelectObject(DC,Pen);
   OldBkMode=SetBkMode(DC,TRANSPARENT);

//Выведем линию...
   MoveToEx(DC,iX0,iY0,NULL);
   LineTo(DC,iX1,iY1);
   SetBkMode(DC,OldBkMode);
   SelectObject(DC,OldPen);
   DeleteObject(Pen);
   return 1;
}


long TDrawLine::PaintOneMore(HDC DC)
{
  HPEN Pen,OldPen; //Параметры палитры
  LOGPEN LogPen;
  COLORREF cOldText;



    if(iX0<0)return 0;
  //Создадим перо
   LogPen.lopnStyle=lopnStyle; //Зададим стиль
   LogPen.lopnWidth.x=lopnWidth.x;
   LogPen.lopnWidth.y=lopnWidth.y;
   LogPen.lopnColor=Colour;   //Зададим цвет

   Pen=CreatePenIndirect(&LogPen);
   if(Pen==NULL)return (-1);
   OldPen=SelectObject(DC,Pen);

//Выведем линию...
   MoveToEx(DC,iX0,iY0,NULL);
   LineTo(DC,iX1,iY1);
   SelectObject(DC,OldPen);
   DeleteObject(Pen);
   return 1;
}


void TDrawLine::GetClassName(char *ClassName, long szClassName)
{
  if(szClassName>=10)
  {
    strcpy(ClassName,"TDrawLine");
    return;
  }
  if(szClassName>=1)
  {
     memcpy(ClassName,"szClassName",szClassName-1);
     ClassName[szClassName-1]=0;
     return;
  }
  ClassName[0]=0;
}

void TDrawLine::GetLocation(double *xl,double *yl)
{
    *xl =X0;
    *yl =Y0;
}



/**********************             Определение окружности               ***********************/
TDrawCircle::TDrawCircle (double x0, double y0,  //Координаты центра окружности
            double rad, LOGPEN CirclePen, LOGBRUSH CircleBrush)
{
  X0=x0; Y0=y0; Radius=rad;


  LogPen.lopnStyle = CirclePen.lopnStyle;
  LogPen.lopnWidth = CirclePen.lopnWidth;
  LogPen.lopnColor = CirclePen.lopnColor;

  LogBrush.lbStyle = CircleBrush.lbStyle;
  LogBrush.lbColor = CircleBrush.lbColor;
  LogBrush.lbHatch = CircleBrush.lbHatch;
}


long TDrawCircle::Paint(HDC DC, POINT pntRLS,
               SIZE szArea, double dfKoefMasshtab)
{
  double x11,y11,x12,y12;
  long Ret;
  HPEN Pen,OldPen; //Параметры палитры
  HBRUSH Brush, OldBrush;
  COLORREF cOldText;
  int OldBkMode;
 //Расчитать координаты относительно Левого верхнего угла

  x11=pntRLS.x+(X0-Radius)*dfKoefMasshtab;
  y11=pntRLS.y-(Y0+Radius)*dfKoefMasshtab;
  x12=pntRLS.x+(X0+Radius)*dfKoefMasshtab;
  y12=pntRLS.y-(Y0-Radius)*dfKoefMasshtab;


  if(x11<0&&x12<0||x11>=szArea.cx&&x12>=szArea.cx||
     y11<0&&y12<0||y11>=szArea.cy&&y12>=szArea.cy)
  {
      lXl=lXr=lYt=lYb=2000000000L;
      return 0;
  }
   lXl=Okrugl(x11); lXr=Okrugl(x12);
   lYt=Okrugl(y11); lYb=Okrugl(y12);

   //Создадим перо
   Pen=CreatePenIndirect(&LogPen);
   if(Pen==NULL)return (-1);

   Brush=CreateBrushIndirect(&LogBrush);
   if(Brush==NULL)
   {
     DeleteObject(Pen);
     return (-1);
   }

   OldPen=SelectObject(DC,Pen);
   OldBrush=SelectObject(DC,Brush);
   OldBkMode=SetBkMode(DC,TRANSPARENT);
   Ellipse(DC,lXl,lYt,lXr,lYb);
   SetBkMode(DC,OldBkMode);
   SelectObject(DC,OldPen);
   SelectObject(DC,OldBrush);
   DeleteObject(Pen);
   DeleteObject(Brush);
   return 1;
}




long TDrawCircle::PaintOneMore(HDC DC)
{
  HPEN Pen,OldPen; //Параметры палитры
  HBRUSH Brush, OldBrush;
  COLORREF cOldText;

  if(lXl>1000000000L)return 0;

  //Создадим перо
  Pen=CreatePenIndirect(&LogPen);
  if(Pen==NULL)return (-1);

  Brush=CreateBrushIndirect(&LogBrush);
  if(Brush==NULL)
   {
     DeleteObject(Pen);
     return (-1);
   }


  OldPen=SelectObject(DC,Pen);
  OldBrush=SelectObject(DC,Brush);
  Ellipse(DC,lXl,lYt,lXr,lYb);

  SelectObject(DC,OldPen);
  SelectObject(DC,OldBrush);
  DeleteObject(Pen);
  DeleteObject(Brush);
  return 1;
}

void TDrawCircle::GetClassName(char *ClassName, long szClassName)
{
  if(szClassName>=12)
  {
    strcpy(ClassName,"TDrawCircle");
    return;
  }
  if(szClassName>=1)
  {
    memcpy(ClassName,"szClassName",szClassName-1);
    ClassName[szClassName-1]=0;
    return;
  }
  ClassName[0]=0;
}

void TDrawCircle::GetLocation(double *xl,double *yl)
{
    *xl =X0;
    *yl =Y0;
}



/******************              Реализация списка графических объектов           ***************/
TDrawObjectList::~TDrawObjectList()
{
  if(FreeObjectsByDelete)
  {
    for(long i=0; i<N_of_Objects;i++)
    {
      if(ArrayDrawObject[i])delete ArrayDrawObject[i];
    }
  }
  free(ArrayDrawObject);
}


long TDrawObjectList::AddDrawObject(TDrawObject *DrawObject)
{
  if(IsBadReadPtr(DrawObject, sizeof(TDrawObject)))
  {
    return 0;
  }
  if(N_of_Objects==MaxN_of_Objects)
  {
    MaxN_of_Objects+=10;
    (void*)ArrayDrawObject=realloc(ArrayDrawObject,
                               sizeof(TDrawObject*)*MaxN_of_Objects);
    if(!ArrayDrawObject)
    {
      return (-1);
    }
  }
  ArrayDrawObject[N_of_Objects]=DrawObject;
  N_of_Objects++;
  return 1;
}


//Рисование
long TDrawObjectList::Paint(HDC DC, POINT pntRLS,
     SIZE szArea, double dfKoefMasshtab)
{
  long i;
  for(i=0;i<N_of_Objects;i++)
  {
//Нарисуем каждый объект
    ArrayDrawObject[i]->Paint(DC, pntRLS, szArea, dfKoefMasshtab);
  }
  return 1;
}


long TDrawObjectList::PaintOneMore(HDC DC)
{
  long i;
  for(i=0;i<N_of_Objects;i++)
  {
//Нарисуем каждый объект
    ArrayDrawObject[i]->PaintOneMore(DC);
  }
  return 1;
}

