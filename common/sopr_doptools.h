//---------------------------------------------------------------------------

#ifndef soprDopToolsH
#define soprDopToolsH

#define ABS(X) (((X) > 0) ? (X) : (-(X)))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))



//---------------------------------------------------------------------------

#define SET_NOMER_SEKTOR_30(A, S) S=(((short)(A))/30)



double doptools_DVremyaChas(double H1, double H2);

/*Возвращает время в секундах*/


double doptools_AbsRaznizaAzimutov(double Azmt1, double Azmt2);


/*Проверка - выхода за пределы сектора от текущего азимута.
Если есть выход за границу по времени или азимуту - выйти
Возвращает 1, если вышла за прелеоы. 0 - если в секторе
*/
long doptools_TestVyhodZaSektor(
                                 double CurAzmt,double CurTime,    //Текущее время
                                 double PrevAzmt, double PrevTime, //Предыдущее время
                                 double SektorAzmt,                //Значение сектора азимута
                                 double TempObzora_ch);            //Период обзора



/*Получение значений азимутов и дальности по деккартовым координатам*/
long doptools_GetAzimutDalnostFromXY(
     long X_m, long Y_m,      //Деккартовы координаты в метрах
     double &Azmt_gr, double &Dlnst_km //Значения азимута и дальности в градусах и км
                                    );



#endif
