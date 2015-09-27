//---------------------------------------------------------------------------
/*Реализация главного класса вывода графическ*/



#ifndef DrawObjectListH
#define DrawObjectListH

#include "structs.h"
#include "formulyars.h"

//---------------------------------------------------------------------------


/*Класс описания
Виртуальный класс. От него наследуются классы
                        TDrawRect - описание прямоугольника
                        TDrawLine - описание линии
                        TDrawCircle - описание окружностей и колец
*/

class TDrawObject
{
  public:
   virtual long Paint(HDC DC,
                   POINT pntRLS,
      SIZE szArea, double dfKoefMasshtab)=0;


   virtual long PaintOneMore(HDC DC)=0;

   virtual void GetClassName(char *ClassName, long szClassName)=0;
   virtual void GetLocation(double *xl,double *yl)=0;
};


// прямоугольник
class TDrawRect:public TDrawObject
{
//  public:
  double X0,Y0;  //Координаты относительно РЛС  - не совсем
  double X1,Y1;  //Координаты второй точки относительно РЛС - не совсем
  long iX0, iY0;    //Расчитанные координаты первого отрезка относительно РЛС
                   //Если iX0 или iY0<0, то значит не выводится данный отрезок

  long iX1, iY1;    //Вторая координата
  COLORREF Colour;  //Цвет выводимой линии
  UINT lopnStyle;   //Стиль выводимого отрезка
  POINT lopnWidth;
  LOGBRUSH zalivka; // заливка

public:
  TDrawRect(double x1, double y1, double x2, double y2,
                     COLORREF Cvet, UINT Stil, POINT Tolshina, LOGBRUSH Brush_transmit);

  virtual long Paint(HDC DC, POINT pntRLS,
     SIZE szArea, double dfKoefMasshtab);
  virtual long PaintOneMore(HDC DC);
  virtual void GetClassName(char *ClassName, long szClassName);
  virtual void GetLocation(double *xl,double *yl);



};



// линия
class TDrawLine:public TDrawObject
{
//  public:
  double X0,Y0;  //Координаты относительно РЛС
  double X1,Y1;  //Координаты второй точки относительно РЛС
  long iX0, iY0;    //Расчитанные координаты первого отрезка относительно РЛС
                   //Если iX0 или iY0<0, то значит не выводится данный отрезок

  long iX1, iY1;    //Вторая координата
  COLORREF Colour;  //Цвет выводимой линии
  UINT lopnStyle;   //Стиль выводимого отрезка
  POINT lopnWidth;

public:
  TDrawLine(double x1, double y1, double x2, double y2,
                     COLORREF Cvet, UINT Stil, POINT Tolshina);

  virtual long Paint(HDC DC, POINT pntRLS,
     SIZE szArea, double dfKoefMasshtab);
  virtual long PaintOneMore(HDC DC);
  virtual void GetClassName(char *ClassName, long szClassName);
  virtual void GetLocation(double *xl,double *yl);

};

// окружность
class TDrawCircle : public TDrawObject
{
  double X0,Y0;   //Координаты относительно РЛС
  double Radius;  //Радиус
  long lXl,lYt,lXr,lYb; //Прямоугольная область где будет рисоваться окружность
  LOGPEN LogPen;
  LOGBRUSH LogBrush;


  public:

//Создание данных для компонент которые не опираются на данные
  TDrawCircle (double x0, double y0,  //Координаты центра окружности
            double rad, LOGPEN CirclePen, LOGBRUSH CircleBrush);

  virtual long Paint(HDC DC, POINT pntRLS,
     SIZE szArea, double dfKoefMasshtab);
  virtual long PaintOneMore(HDC DC);
  virtual void GetClassName(char *ClassName, long szClassName);
  virtual void GetLocation(double *xl,double *yl);
};


// цель





/*Список объектов для рисования сетки*/
class TDrawObjectList
{
//  long N_of_Objects;     //Число объектов
//  long MaxN_of_Objects;  //Максимально допустимое число объектов
//  TDrawObject **ArrayDrawObject; //Массив объектов типа TDrawObject
 bool FreeObjectsByDelete;
 public:
  TDrawObject **ArrayDrawObject; //Массив объектов типа TDrawObject
  long N_of_Objects;     //Число объектов
  long MaxN_of_Objects;  //Максимально допустимое число объектов
  TDrawObjectList(bool isFreeObjectsByDelete)
  {
    N_of_Objects=0;
    MaxN_of_Objects=0;
    ArrayDrawObject=(TDrawObject **)0;
    FreeObjectsByDelete=isFreeObjectsByDelete;
  }
  ~TDrawObjectList();


//Добавление нового объекта
  long AddDrawObject(TDrawObject *DrawObject);

//Рисование
  long Paint(HDC DC,
             POINT pntRLS,
             SIZE szArea,
             double dfKoefMasshtab);


//Перерисовка объектов
  long PaintOneMore(HDC DC);

};






#endif
