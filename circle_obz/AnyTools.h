//---------------------------------------------------------------------------

#ifndef AnyToolsH
#define AnyToolsH
//---------------------------------------------------------------------------
int GetTochkiNaGranicePriam(double X1,double Y1,double X2, double Y2, //Координаты прямоq
                       long X1r, long Y1r, long X2r, long Y2r,  //Координаты прямоуголльника
                       long *X1ret,long *Y1ret, long *X2ret, long *Y2ret); //Координаты возвращаемый

int ObrezatPriamyeNaGranice(double X1,double Y1,double X2, double Y2, //Координаты прямоq
                       long X1r, long Y1r, long X2r, long Y2r,  //Координаты прямоуголльника
                       long *X1ret,long *Y1ret, long *X2ret, long *Y2ret); //Координаты возвращаемый


//Поиск пересечений окружности и линии
int GetPixelsForCircleAndLine(
             double X1, double Y1, double X2, double Y2,
             double X0,  double Y0,  double radius,  //Данные окружности
             double X[], double Y[], int *N_of_R);


//Возвращает число пересечений
int GetPixelsForCircleAndRect(
       double X1r, double Y1r, double X2r, double Y2r, //Координаты точки
       double X0,  double Y0,  double radius,  //Данные окружности
       double X[], double Y[], int *N_of_R);

             


long Okrugl(double Chislo1);

//Принадлежит ли точка отрезку
int PixelAndLine(int X1,int Y1,  //Первая координата отрезка
                 int X2,int Y2,  //Вторая кооордината отрезук
                 int Xp, int Yp, //Координата точки
                 int Tochnost);  //Пиксель

#endif
