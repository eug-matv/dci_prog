//---------------------------------------------------------------------------


#pragma hdrstop

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "sopr_aorl_1a.h"
#include "AnyTools.h"
#include "structs.h"
#include "NutonIHord.h"
#define ABS(x) ((x)>=0?(x):(-(x)))

//#define DATA_OUT_DEBUG


//---------------------------------------------------------------------------

#pragma package(smart_init)

#ifdef DATA_OUT_DEBUG
static FILE *fpOutAB;
static char FileNameOutAB[1020];
#endif









static double a_t=3.0,  //ускорение в м/c
            TO_t=4.58,  //темп обзора
            SigmaX_t,   //сигма
            c_t=2,      //Размер лбласти
            L_t;        //разме

double f_A(double A)
{
   return (a_t*TO_t*TO_t/SigmaX_t)+
       (c_t*sqrt((6.0*A-A*A)/(8-8*A+A*A))-L_t/(2*SigmaX_t))*
        A*A/(2-A);
};



//Вычисление сигм по сигнам азимута  и дальностей
long GetSigmyDecartKoordinat(double Azmt_gr, //Значение азимута в градусах
                             double Dlnst_km, //Значение дальности в км
                             double Sig_Azmt_minut, //В минутах
                             double Sig_Dlnst_m,
                             double &Sig_X,
                             double &Sig_Y,
                             double &Sig_XY)
{
   double Azmt_rad=Azmt_gr/180.0*M_PI;
   double Sig_Dlnst_km=Sig_Dlnst_m/1000.0;
   double Sig_Azmt_rad=Sig_Azmt_minut/60.0*M_PI/180.0;

//Расчитаем квадраты
   Sig_X=Sig_Dlnst_km*Sig_Dlnst_km*sin(Azmt_rad)*sin(Azmt_rad)+
Dlnst_km*Sig_Azmt_rad*Dlnst_km*Sig_Azmt_rad*cos(Azmt_rad)*cos(Azmt_rad);

   Sig_Y=Sig_Dlnst_km*Sig_Dlnst_km*cos(Azmt_rad)*cos(Azmt_rad)+
Dlnst_km*Sig_Azmt_rad*Dlnst_km*Sig_Azmt_rad*sin(Azmt_rad)*sin(Azmt_rad);


   Sig_XY=(Sig_Dlnst_km*Sig_Dlnst_km-
        Dlnst_km*Sig_Azmt_rad*Dlnst_km*Sig_Azmt_rad)*
        sin(Azmt_rad)*cos(Azmt_rad)*1000.0;

   Sig_X=sqrt(Sig_X)*1000;
   Sig_Y=sqrt(Sig_Y)*1000;
   return 1;
}



//Получение весового коэффициента
//Если меньше нуля то надо уже удалить эту трассу. Если больше 100000 то даже и не рассматривать,
//а в остальном как оценивать кооэфициенты и сравнивать
double TSoprTrassa::GetVesKoef(TPaketData &PD, initialization &_init, double TO)
{
  double dTime;
  long dObzor2,
       dObzor1;
  double dA, dD;  //Разница азимутов и дальностей
  bool Dekkart=false;

  double L_x,L_y,L_xy;    //Размеры координат
  double Sigma_x,Sigma_y,Sigma_xy;
  double f_A0, f_A1;
  int Ret;
//Оценим сколько обзоров прошло с момента привязки последнего отсчета к данной трассе
  dTime=PD.DateTime.Time-DateTime.Time;
  if(dTime<0)dTime+=24.0;
  dObzor1=Okrugl(dTime/TO);


  if(dObzor1>_init.N_of_Potery)return (-10.0);

  if(Predvaritelno)
  {
//Если предварительно то тоже надо проверить чтобы на 1 отличалась
     if(dObzor1>1)return (-10.0);
  }

//Оценим сколько обзоров прошло с отсчета который относился мог бы принадлежать данной трассе
  dTime=PD.DateTime.Time-DateTimeSrav.Time;
  if(dTime<0)dTime+=24.0;
  dObzor2=Okrugl(dTime/TO);



//Далее типа пакета
  if(PD.NomerSektoraRazmerom30!=NomerSektora&&
     PD.NomerSektoraRazmerom30!=((NomerSektora+1)%12)&&
     PD.NomerSektoraRazmerom30!=((NomerSektora+11)%12))
  {

      return 200000.0;
  }

  if(dObzor1<1&&LastPD==NULL)
  {
  //Это означает, что трасса только что заваязана и обзора не прошло
     return 200000.0;
  }


  if(PD.TipPaketa!=TipPakOtmetka)
  {
     return 200000.0;
  }

  if(PD.Otmetka.Kanal!=Kanal_P&&Rejim==0)
  {
    return 200000.0;
  }

  if(PD.Otmetka.Rej==RejimUVD&&Rejim!=1)
  {
     return 200000.0;
  }

  if(PD.Otmetka.Rej==RejimRBS&&Rejim!=2)
  {
     return 200000.0;
  }

  if(Rejim&&PD.Otmetka.lNomer!=0&&Nomer==0)
  {
     return 200000.0;
  }

//Проверка попадания в обзор
  if(dObzor2>0)
  {
//Оценим координаты экстраполированные
     Xe=Xi+dObzor2*TO*V_Xi;
     Ye=Yi+dObzor2*TO*V_Yi;
     if(Xe<0.001&&Xe>-0.001&&Ye<0.001&&Ye>-0.001)
     {
        Ae=0;
        De=0;
     }else{
       Ae=atan2(Xe,Ye)/M_PI*180.0;
       De=sqrt(Xe*Xe+Ye*Ye);
       if(Ae<0)Ae+=360.0;
     }
     V_Xe=V_Xi;   //Установили значение скорости
     V_Ye=V_Yi;
     Xi=Xe;
     Yi=Ye;

//Оценим Alfa и Beta
     TO_t=TO*3600.0;     //Вычислим темп обзора в секундах
     GetSigmyDecartKoordinat(Ae,De,_init.sigma_A_min, _init.sigma_D_m,Sigma_x, Sigma_y, Sigma_xy);
     GetSigmyDecartKoordinat(Ae,De,_init.strob_azimuth_g*60.0,
        _init.strob_dalnost_m,L_x, L_y, L_xy);

     if(L_x<_init.strob_dekkart_m)L_x=_init.strob_dekkart_m;
     if(L_y<_init.strob_dekkart_m)L_y=_init.strob_dekkart_m;

//Оценим Alfa_x
     SigmaX_t=Sigma_x;
     L_t=L_x;
     f_A0=f_A(0);
     f_A1=f_A(1);
     if(f_A0*f_A1<0)
     {
        Ret=EMMetodHord1(f_A,0,1.0,0.1,0.0001, &Alfa_x);
        if(Alfa_x<0)Alfa_x=0;
        if(Alfa_x>1)Alfa_x=1;
     }else{
        Alfa_x=1;
     }
     Beta_x=Alfa_x*Alfa_x/(2-Alfa_x);

     SigmaX_t=Sigma_y;
     L_t=L_y;
     f_A0=f_A(0);
     f_A1=f_A(1);
     if(f_A0*f_A1<0)
     {
        Ret=EMMetodHord1(f_A,0,1.0,0.1,0.0001, &Alfa_y);
        if(Alfa_y<0)Alfa_y=0;
        if(Alfa_y>1)Alfa_y=1;
     }else{
        Alfa_y=1;
     }
     Beta_y=Alfa_y*Alfa_y/(2-Alfa_y);
     DateTimeSrav=PD.DateTime;
     Koef=200000.0;

#ifdef DATA_OUT_DEBUG
     SaveDataToFileDebug(fpOutAB);
#endif
  }
//Оценим попадание в строб
  if(ABS(Xe*1000-PD.Otmetka.X)<=_init.strob_dekkart_m&&
     ABS(Ye*1000-PD.Otmetka.Y)<=_init.strob_dekkart_m)Dekkart=true;

  if(Dekkart==false)
  {
    dD=ABS(De-PD.Otmetka.Dalnost)*1000;
    if(dD>_init.strob_dalnost_m)
    {
      return 20000;
    }
    dA=ABS(Ae-PD.Azimut);
    if(dA>180)dA=360-dA;
    if(dA>_init.strob_azimuth_g)
    {
      return 20000;
    }
  }

//В одном стробе - точно . Оценим расстояние между отметкой и экстраполиролванной
  dD=(Xe-PD.Otmetka.X*0.001)*(Xe-PD.Otmetka.X*0.001)+
     (Ye-PD.Otmetka.Y*0.001)*(Ye-PD.Otmetka.Y*0.001);

  if(Rejim!=0)
  {
    if(PD.Otmetka.Kanal==Kanal_P)
    {

      dD+=2000.0;
    }
    else if(PD.Otmetka.lNomer!=Nomer)
    {
      dD+=1000;
    }
  }
  if(dD<Koef)
  {
    return dD;
  }
  return 200000.0;
}



int TSoprTrassa::SaveDataToFileDebug(FILE *fp)
{
   char Rj;
   if(Rejim==0)Rj='P';
   else if(Rejim==1)Rj='U';
   else Rj='R';
   fprintf(fp,
"%04X|%c|%5d|%7.02lf|%7.02lf|%6.02lf|%6.02lf|%8.01lf|%8.01lf|%4.02lf|%4.02lf|%4.02lf|%4.02lf|\n",
NomerTrassy,
      Rj,Nomer,
             Xe,     Ye,     Ae,      De,    V_Xe,   V_Ye,   Alfa_x, Beta_x, Alfa_y, Beta_y);
  return 1;
}



//Установить последние данные для трассы
//Должны быть уже определены
int TSoprTrassa::SetDataTrassy(double _Koef,
                              TPaketData &PD,
                              initialization &_init,
                              double TO)
{
//Перерасчитаем
  airplane *apl;

  Xi=Xe+Alfa_x*(PD.Otmetka.X/1000.0-Xe);
  V_Xi=V_Xe+Beta_x/TO*(PD.Otmetka.X/1000.0-Xe);
  Yi=Ye+Alfa_y*(PD.Otmetka.Y/1000.0-Ye);
  V_Yi=V_Ye+Beta_y/TO*(PD.Otmetka.Y/1000.0-Ye);
  NomerSektora=PD.NomerSektoraRazmerom30;
  DateTime=PD.DateTime;
  Koef=_Koef;
  LastPD=&PD;
  PD.Otmetka.lNomerTrassy=NomerTrassy;
  PD.Otmetka.lDopNumber=Nomer;  //Номер трассы

  if(Predvaritelno)
  {
     PD.Otmetka.iTail=0;
     PD.Otmetka.iFormulyar=0;
     PD.Otmetka.PrivyazanPredvaritelno=Predvaritelno;
     Predvaritelno++;
     if(Predvaritelno==2)Predvaritelno=0;  //Сбросить предварительно
  }else{
     PD.Otmetka.iTail=1;
     PD.Otmetka.iFormulyar=1;
     PD.Otmetka.PrivyazanPredvaritelno=0;
  }
  PD.Otmetka.Vx=V_Xi;
  PD.Otmetka.Vy=V_Yi;

  if(PD.Otmetka.apl)
  {
    apl=(airplane *)(PD.Otmetka.apl);
    apl->lDopNumber=Nomer;
    apl->lNomerTrassy=NomerTrassy;
    apl->Formulyar=1;
    apl->WasTail=1;
  }



  return 1;
}



TSoprTrassaList::TSoprTrassaList(const initialization &_init)
{
   memcpy(&init, &_init,sizeof(initialization));
   N_of_Trass=0;
   FirstTrassa=new TSoprTrassa;
   FirstTrassa->Head=1;
   FirstTrassa->Next=FirstTrassa;
   FirstTrassa->Prev=FirstTrassa;
   TO=init.fTempObzora/3600.0;
   N_of_P=0;
   ZavytazPervKanal=false;

#ifdef DATA_OUT_DEBUG
   sprintf(FileNameOutAB,"%s\\debug_escout.txt",init.csFullPathToData);
   fpOutAB=fopen(FileNameOutAB, "w");
#endif


}

int TSoprTrassaList::Delete(TSoprTrassa *Trassa)
{
   TSoprTrassa *Prom;

   if(FirstTrassa->Next==FirstTrassa)return 0;
   if(Trassa==NULL||Trassa==FirstTrassa)
   {
        Trassa=FirstTrassa->Next;
   }

   Trassa->Prev->Next=Trassa->Next;
   Trassa->Next->Prev=Trassa->Prev;
   delete Trassa;
   return 1;
}


int TSoprTrassaList::ZavyazatTrassu(TPaketData &PD1, TPaketData &PD2)
{
  double dTime;
  double dR,V;
  TSoprTrassa *Trassa, *Prom;

//Надо определить, что не завязаны трасса
   if(PD1.TipPaketa==TipPakNeOpredelen)return (-1);
   dTime=PD2.DateTime.Time-PD1.DateTime.Time;
   if(dTime>1.4*TO)return (-1);   //Все закончено
   if(PD2.TipPaketa!=TipPakOtmetka)return (-1);
   if(PD1.TipPaketa!=TipPakOtmetka)return (0);
   if(PD2.Otmetka.lNomerTrassy>0)return (-1);
   if(PD1.Otmetka.lNomerTrassy>0)return 0;
   if(dTime<=0.4*TO)return 0;   //Все закончено
   if(PD1.NomerSektoraRazmerom30!=PD2.NomerSektoraRazmerom30 &&
      ((PD1.NomerSektoraRazmerom30+11)%12)!=PD2.NomerSektoraRazmerom30 &&
      ((PD1.NomerSektoraRazmerom30+1)%12)!=PD2.NomerSektoraRazmerom30)return 0;

   if(PD1.Otmetka.Kanal==Kanal_P&&PD2.Otmetka.Kanal!=Kanal_P||
      PD1.Otmetka.Kanal!=Kanal_P&&PD2.Otmetka.Kanal==Kanal_P)return 0;

   if(PD1.Otmetka.Kanal!=Kanal_P&&PD2.Otmetka.Kanal!=Kanal_P)
   {
      if(PD1.Otmetka.Rej!=PD2.Otmetka.Rej)return 0;
      if(PD1.Otmetka.lNomer!=PD2.Otmetka.lNomer)return 0;
   }

   if(PD1.Otmetka.Kanal==Kanal_P&&PD2.Otmetka.Kanal==Kanal_P&&
     !ZavytazPervKanal
     )
   {
      return 0;
   }


//Проверка скорости
   dR=(PD1.Otmetka.X-PD2.Otmetka.X)/1000.0*
      (PD1.Otmetka.X-PD2.Otmetka.X)/1000.0+
      (PD1.Otmetka.Y-PD2.Otmetka.Y)/1000.0*
      (PD1.Otmetka.Y-PD2.Otmetka.Y)/1000.0;
   dR=sqrt(dR);

//Найдем скорость
   V=dR/TO;
   if(V>init.MaxV_km_ch||
      V<init.MinV_km_ch)return 0;

//А теперь попробуем завязать пару
   Trassa=new TSoprTrassa;
   Trassa->Head=0;
   Trassa->Xi=PD2.Otmetka.X/1000.0;
   Trassa->Yi=PD2.Otmetka.Y/1000.0;
   Trassa->V_Xi=(PD2.Otmetka.X-PD1.Otmetka.X)/1000.0/TO;
   Trassa->V_Yi=(PD2.Otmetka.Y-PD1.Otmetka.Y)/1000.0/TO;
   Trassa->Nomer=PD2.Otmetka.lNomer;
   Trassa->NomerSektora=PD2.NomerSektoraRazmerom30;
   Trassa->DateTime=PD2.DateTime;
   Trassa->DateTimeSrav=PD2.DateTime;
   Trassa->LastPD=NULL;
   Trassa->Xe=Trassa->Ye=-100000;
   Trassa->Koef=200000.0;
   if(PD2.Otmetka.Kanal==Kanal_P)
   {
     Trassa->Rejim=0;
     Trassa->Predvaritelno=1;
   }
   else
   {
     if(PD2.Otmetka.Rej==RejimUVD)Trassa->Rejim=1;
     else Trassa->Rejim=2;
     Trassa->Predvaritelno=0;
   }
   Prom=FirstTrassa->Prev;
   Prom->Next=Trassa;
   Trassa->Prev=Prom;
   Trassa->Next=FirstTrassa;
   FirstTrassa->Prev=Trassa;
   Trassa->NomerTrassy=++N_of_Trass;
   PD2.Otmetka.lNomerTrassy=Trassa->NomerTrassy;
   PD2.Otmetka.PrivyazanPredvaritelno=Trassa->Predvaritelno;
   if(PD2.Otmetka.PrivyazanPredvaritelno)
   {
      PD2.Otmetka.iFormulyar=0;
      PD2.Otmetka.iTail=0;
   }else{
      PD2.Otmetka.iFormulyar=1;
      PD2.Otmetka.iTail=1;
   }
   PD2.Otmetka.Vx=Trassa->V_Xi;
   PD2.Otmetka.Vy=Trassa->V_Yi;
   return 1;
}


int TSoprTrassaList::AddOtschet(TPaketData &PD)  //Добавления отсчета
{
   double RetKoef,MinRetKoef=20000;
   double dT;
   long dObzor;
   airplane *apl;
   int k=0;
   TSoprTrassa *Trassa, *TrassaNaid=NULL,*Prom;
   TPaketData *PD1=NULL;


   if(PD.TipPaketa==TipPakOtmetka&&
      PD.Otmetka.Kanal==Kanal_P)
   {
      N_of_P++;
   }

   if(PD.TipPaketa==TipPakSever)
   {
     if(N_of_P>30)
     {
        ZavytazPervKanal=false;
     }else{
        ZavytazPervKanal=true;
     }
     N_of_P=0;
   }

//Рассмотрим каждую трассу
   int Debug_k=0;
#ifdef DATA_OUT_DEBUG
   SaveDataToFileDebug(fpOutAB, PD);
#endif


   for(Trassa=FirstTrassa->Next;Trassa!=FirstTrassa;)
   {
//Определим значения весовых кооэфициентов для каждой трассы
     RetKoef=Trassa->GetVesKoef(PD,init,TO);
     Debug_k++;
     if(RetKoef<0)
     {
//Если коэфициент меньше нуля  - надо пудумать об удалении трассы.
       Prom=Trassa;
       Trassa=Trassa->Next;
       Delete(Prom);
       continue;
     }else if(RetKoef<10000.0)
     {
        if(MinRetKoef>RetKoef)
        {
           MinRetKoef=RetKoef;
           TrassaNaid=Trassa;
        }
     }
     Trassa=Trassa->Next;
   }

   if(TrassaNaid)
   {
//Есть куда привязать
//Установим данные
      if(TrassaNaid->LastPD)
      {
        dT=PD.DateTime.Time-TrassaNaid->DateTime.Time;
        if(dT<0)dT+=24.0;
        dObzor=Okrugl(dT/TO);
        if(dObzor<1)
        {
           PD1=TrassaNaid->LastPD;
        }
      }
      TrassaNaid->SetDataTrassy(MinRetKoef,PD,init,TO);
      if(PD1)
      {
          PD1->Otmetka.lNomerTrassy=0;
          PD1->Otmetka.lDopNumber=PD1->Otmetka.lNomer;
          PD1->Otmetka.PrivyazanPredvaritelno=0;
          if(PD1->Otmetka.apl)
          {
             apl=(airplane*)(PD1->Otmetka.apl);
             apl->lDopNumber=apl->nomer_borta;
             apl->lNomerTrassy=0;
             apl->Formulyar=0;
             apl->WasTail=0;
          }
          AddOtschet(*PD1);
      }
      return 1;
   }
   return 0;
}

TSoprTrassaList::~TSoprTrassaList()
{
#ifdef DATA_OUT_DEBUG
  if(fpOutAB)
  {
     fclose(fpOutAB);
  }
#endif

  while(Delete(NULL));
  delete FirstTrassa;   //Удалим первую трассу
}


 //Процедуры для отладки - сохранение данных в файл
int TSoprTrassaList::
    SaveDataToFileDebug(FILE *fp, const TPaketData &PD)
{
   char Rj;
   int M,H;
   double M1,S;
   if(PD.TipPaketa!=TipPakOtmetka)return 0;
   if(PD.Otmetka.Kanal==Kanal_P)
   {
      Rj='P';
   }else{
      if(PD.Otmetka.Rej==RejimUVD) Rj='U';
      else Rj='R';
   }
   H=(int)(PD.DateTime.Time);
   M1=60.0*(PD.DateTime.Time-H);
   M=(int)M1;
   S=(M1-M)*60.0;
   fprintf(fp,
"%04X|%c|%5d|%7.02lf|%7.02lf|%6.02lf|%6.02lf|%02d:%02d:%06.03lf|\n",
PD.Otmetka.lNomerTrassy,
      Rj,PD.Otmetka.lNomer,
             PD.Otmetka.X/1000.0,
                     PD.Otmetka.Y/1000.0,
                             PD.Azimut,
                                     PD.Otmetka.Dalnost,
                                              H,M,S);
   return 1;
}

