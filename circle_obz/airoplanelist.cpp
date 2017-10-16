//---------------------------------------------------------------------------
#pragma hdrstop
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "structs.h"
#include "sopr_doptools.h"
#include "DrawObjectList.h"

#include "airoplanelist.h"
#include "AnyTools.h"
#include "formulyars.h"
#include "probality.h"
#include "kio_exeptions.h"


//---------------------------------------------------------------------------
//#define ABS(X) ((X)>=0?(X):(-(X)))


#pragma package(smart_init)



/*�����������.
*/
TAirplaneList::TAirplaneList(
                POINT &_pntRLS,    //��������� ���
                SIZE & _szArea,    //������ ������
                initialization & _init, //����������������� ����
                double _KoefMasshtab) //���������� ��� �������� �� �������� �                                       //������������ ������
{
   int i;
   dfKoefMasshtab=_KoefMasshtab;
   pntRLS.x=_pntRLS.x;    pntRLS.y=_pntRLS.y;
   szArea.cx=_szArea.cx;    szArea.cy=_szArea.cy;
   memcpy(&init,&_init,sizeof(initialization));
//�������������
   TFormulyarDraw::SetInitialization(
              init,_szArea.cx,_szArea.cy);

   memset(apl,0,12*sizeof(airplane **));
   temp_apl[0]=NULL;    temp_apl[1]=NULL;
   memset(apl_tail,0,12*sizeof(airplane_light*));
   temp_apl_tail[0]=temp_apl_tail[1]=NULL;

   TO=init.fTempObzora/3600.0;
//�� ������� 30-���������� ������� ��������� �������
   for(i=0;i<12;i++)
   {
      iMaxSize_apl[i]=0;
      iSize_apl[i]=0;
      iMaxSize_apl_tail[i]=0;
      iSize_apl_tail[i]=0;
      apl[i]=NULL;
      apl_tail[i]=NULL;

/*
      apl[i]=(airplane *)malloc(sizeof(airplane)*iMaxSize_apl[i]);
      apl_tail[i]=(airplane_light*)
           malloc(sizeof(airplane_light)*iMaxSize_apl_tail[i]);
*/
      last_time[i]=-1;
   }
   iMaxSize_temp_apl[0]=iMaxSize_temp_apl[1]=0;
   iSize_temp_apl[0]=iSize_temp_apl[1]=0;

   iMaxSize_temp_apl_tail[0]=iMaxSize_temp_apl_tail[1]=0;
   iSize_temp_apl_tail[0]=iSize_temp_apl_tail[1]=0;

/*
   temp_apl[0]=(airplane *)malloc(sizeof(airplane)*iMaxSize_temp_apl[0]);
   temp_apl[1]=(airplane *)malloc(sizeof(airplane)*iMaxSize_temp_apl[1]);
*/
   temp_apl[i]=NULL;
   temp_apl_tail[i]=NULL;

   iSect30_temp_apl[0]=iSect30_temp_apl[1]=-1;
   iCur_tempIndex=0;

   dLastRadian=0;

  iNewView=1;

  LastAirTail=NULL;
  N_of_LastAirTail=MaxN_of_LastAirTail=0;

  iKodB=0;

//���������������� ����������� ������
   InitializeCriticalSection (&csForModifySwap_temp_apl);
}

/*���������� - �� ������*/
TAirplaneList::~TAirplaneList()
{

  for(int i=0;i<12;i++)
  {
     for(int j=0;j<iMaxSize_apl[i];j++)
     {

/*        if(j<iSize_apl[i]&&apl[i][j]->GetProbData)
           delete ((TOcenkaVeroyatnosti*)(apl[i][j]->GetProbData));
������ �����������           
*/

        free(apl[i][j]);
     }
     if(apl[i])free(apl[i]);
     if(apl_tail[i])free(apl_tail[i]);
  }
  if(temp_apl[0])
  {
    for(int j=0;j<iMaxSize_temp_apl[0];j++)
    {
/*
        if(j<iSize_temp_apl[0]&&
           temp_apl[0][j]->GetProbData)
          delete ((TOcenkaVeroyatnosti*)(temp_apl[0][j]->GetProbData));
������ �����������          
*/          
        free(temp_apl[0][j]);
    }
    free(temp_apl[0]);
  }
  if(temp_apl[1])
  {
    for(int j=0;j<iMaxSize_temp_apl[1];j++)
    {
/*
        if(j<iSize_temp_apl[1]&&
           temp_apl[1][j]->GetProbData)
              delete ((TOcenkaVeroyatnosti*)(temp_apl[1][j]->GetProbData));
������ �����������              
*/
        free(temp_apl[1][j]);
    }
    free(temp_apl[1]);
  }
  if(temp_apl_tail[0])free(temp_apl_tail[0]);
  if(temp_apl_tail[1])free(temp_apl_tail[1]);
  if(LastAirTail)free(LastAirTail);
  TFormulyarDraw::Free();//��������� ������ ��� ���������
  DeleteCriticalSection(&csForModifySwap_temp_apl);
}


/*���������� ����*/
int TAirplaneList::AddTarget(
           TPaketData &PD       //����� � �������
                             )        //�������� �� �������� ��� ������ ����
{
  int i_retIndex;
  int i;
  int N_Sektor1, N_Sektor2, N_Sektor3;
  int NomTrassy;
  airplane *NaidAir=NULL;  //��������� ����
  int dN_of_O[3]={0,0,0},kn;  //��������������� ��� �������
  EnterCriticalSection(&csForModifySwap_temp_apl);

  i_retIndex=ModifyCircle(PD.DateTime.Time, PD.NomerSektoraRazmerom30);


  if(i_retIndex<0)
  {
     LeaveCriticalSection(&csForModifySwap_temp_apl);
     return 0;
  }


  if(PD.TipPaketa==TipPakOtmetka)
  {
//���������� ������ �������
    if(iSize_temp_apl[i_retIndex]==iMaxSize_temp_apl[i_retIndex])
    {
       temp_apl[i_retIndex]=(airplane**)realloc(temp_apl[i_retIndex],
                sizeof(airplane*)*(iMaxSize_temp_apl[i_retIndex]+10));
if(temp_apl[i_retIndex]==NULL)throw EIkoException ("Memory allocation error",
"airoplanelist.cpp","TAirplaneList","AddTarget",
  "temp_apl[i_retIndex]=(airplane**)realloc(temp...");

       for(i=iMaxSize_temp_apl[i_retIndex];i<iMaxSize_temp_apl[i_retIndex]+10;i++)
       {
         temp_apl[i_retIndex][i]=(airplane*)malloc(sizeof(airplane));
if(temp_apl[i_retIndex][i]==NULL)throw EIkoException ("Memory allocation error",
"airoplanelist.cpp","TAirplaneList","AddTarget",
"temp_apl[i_retIndex][i]=(airplane*)malloc(sizeof(airplane));");
/* ������ ����������� ����
         temp_apl[i_retIndex][i]->GetProbData=NULL;
*/
       }
       iMaxSize_temp_apl[i_retIndex]+=10;

    }
//� ������ ������� ������
    kn=temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->type=PD.Otmetka.Kanal;

    if(temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->type==0)
    {
      if(PD.Otmetka.bP_LCHM)
        temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->mono_lhm=1;
      else temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->mono_lhm=0;
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->nomer_borta=99999;
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->altitude=1999;
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->abs_altitude=1;
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->fuel=0;
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->rejim=0;
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->lDopNumber=PD.Otmetka.lDopNumber;
      dN_of_O[0]=1;
    }else{
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->nomer_borta=PD.Otmetka.lNomer;
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->altitude=PD.Otmetka.lVysota;
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->abs_altitude=PD.Otmetka.bAbsVysota;
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->fuel=PD.Otmetka.lOstatokTopliva;
      if(PD.Otmetka.Rej==RejimUVD)
         temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->rejim=1;
      else temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->rejim=2;
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->lDopNumber=PD.Otmetka.lDopNumber;
      dN_of_O[kn]=1;
    }
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->x=PD.Otmetka.X/1000.0;
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->y=PD.Otmetka.Y/1000.0;
    if(PD.Otmetka.sKodB>0)
    {
      iKodB=PD.Otmetka.sKodB;
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->alarm=PD.Otmetka.sKodB;
    }
    else temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->alarm=0;
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->dalnost=PD.Otmetka.Dalnost;
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->azimuth=PD.Azimut;

//���������� �� ���������
    if(PD.Otmetka.Vx>50000||PD.Otmetka.Vy>50000)
    {
        temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->velocity=0;
    }else{
        temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->velocity=
         sqrt(PD.Otmetka.Vx*PD.Otmetka.Vx+PD.Otmetka.Vy*PD.Otmetka.Vy);
    }


//���������� � ��������
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->sector30_number=PD.NomerSektoraRazmerom30;
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->Time=PD.DateTime.Time;
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->WasTail=PD.Otmetka.iTail;

    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->Formulyar=PD.Otmetka.iFormulyar;
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->TransFormulyar=1;  //����������

//�������� ������ ���������  � ��������� ������ ��������
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->i_x=pntRLS.x+
        Okrugl(temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->x*dfKoefMasshtab);
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->i_y=pntRLS.y-
        Okrugl(temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->y*dfKoefMasshtab);

    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->N_of_Propusk=0;  //��� ���������
    if(PD.Otmetka.PrivyazanPredvaritelno)
    {
       NomTrassy=temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->lNomerTrassy=0;
    }else{
       NomTrassy=temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->lNomerTrassy=PD.Otmetka.lNomerTrassy;
    }
    if(temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->lNomerTrassy>0)
    {
       temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->DeleteIt=0;
    }else{
       temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->DeleteIt=1;
       temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->WasTail=0;
    }
/*
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->GetProbData=NULL;
    ������ ����������� ����
*/

//� ������ ���� �������� ��������
/*
      temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->FD=
        TFormulyarDraw::AddFormulyar(
         temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->i_x,
         temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->i_y,
         temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]);
*/
    PD.Otmetka.apl=(void*)temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]];
    temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->PaketStruct=
           (void*)(&PD);
//��������� ����� ������� ��������

    if(NomTrassy>0)
    {
      N_Sektor2=temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->sector30_number;
      N_Sektor1=(N_Sektor2+11)%12;
      N_Sektor3=(N_Sektor2+1)%12;
      for(i=0;i<iSize_apl[N_Sektor2];i++)
      {
         if(apl[N_Sektor2][i]->lNomerTrassy==NomTrassy)
         {
            if(!apl[N_Sektor2][i]->DeleteIt)
            {
               apl[N_Sektor2][i]->DeleteIt=1;
               NaidAir=apl[N_Sektor2][i];

//               break;
            }
         }
      }

      for(i=0;i<iSize_apl[N_Sektor1];i++)
      {
         if(apl[N_Sektor1][i]->lNomerTrassy==NomTrassy)
         {
            if(!apl[N_Sektor1][i]->DeleteIt)
            {
               apl[N_Sektor1][i]->DeleteIt=1;
               NaidAir=apl[N_Sektor1][i];
//               break;
            }
         }
      }

//      if(!NaidenDeleteIt)
      for(i=0;i<iSize_apl[N_Sektor3];i++)
      {
         if(apl[N_Sektor3][i]->lNomerTrassy==NomTrassy)
         {
            if(!apl[N_Sektor3][i]->DeleteIt)
            {
               apl[N_Sektor3][i]->DeleteIt=1;
               NaidAir=apl[N_Sektor3][i];
//               break;
            }
         }
      }
      if(NaidAir)
      {
    //   ������ ����������� ����
  /*
         if(NaidAir->GetProbData)
         {
            ((TOcenkaVeroyatnosti*)(NaidAir->GetProbData))->AddOtmetka(
                temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->type);

                temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->GetProbData=
                NaidAir->GetProbData;
                if(NaidAir->GetProbData)
                {
//��������� ������ �� ���������� ������ ����
                 PD.Otmetka.N_of_MustPO=
        	 ((TOcenkaVeroyatnosti*)(NaidAir->GetProbData))->GetN_of_MustO(0);
                 PD.Otmetka.N_of_MustSO=
         	 ((TOcenkaVeroyatnosti*)(NaidAir->GetProbData))->GetN_of_MustO(1);
                 PD.Otmetka.N_of_PO=
        	 ((TOcenkaVeroyatnosti*)(NaidAir->GetProbData))->GetN_of_O(0);
                 PD.Otmetka.N_of_SO=
        	 ((TOcenkaVeroyatnosti*)(NaidAir->GetProbData))->GetN_of_O(1);
           PD.Otmetka.P_PO=
                ((TOcenkaVeroyatnosti*)(NaidAir->GetProbData))->GetPropably(0);
           PD.Otmetka.P_SO=
                ((TOcenkaVeroyatnosti*)(NaidAir->GetProbData))->GetPropably(1);

                }

  //      ������ ����������� ����


      }
    */

         temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->FirstDirectionFormulyars=
         NaidAir->FirstDirectionFormulyars;



      }else{
/*
         TOcenkaVeroyatnosti *ppppp;
         ppppp=new TOcenkaVeroyatnosti(NomTrassy);
         if(ppppp)
         {
//������� ����� ������ �� �������

           ppppp->AddOtmetka(temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->type);

           temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->GetProbData=
           (TOcenkaVeroyatnosti *)ppppp;
           PD.Otmetka.N_of_MustPO=ppppp->GetN_of_MustO(0);
           PD.Otmetka.N_of_MustSO=ppppp->GetN_of_MustO(1);
           PD.Otmetka.N_of_PO=ppppp->GetN_of_O(0);
           PD.Otmetka.N_of_SO=ppppp->GetN_of_O(1);
           PD.Otmetka.P_PO=ppppp->GetPropably(0);
           PD.Otmetka.P_SO=ppppp->GetPropably(1);
      }
      ������ ���� ������ �����������
*/
//��������� ��� ����������� ���������
         switch(temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->sector30_number)
         {

           case 0:
           case 11:
             temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->
                FirstDirectionFormulyars=5;
           break;
           case 1:
             temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->
                FirstDirectionFormulyars=6;
           break;
           case 2:
           case 3:
             temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->
                FirstDirectionFormulyars=7;
           break;
           case 4:
               temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->
                FirstDirectionFormulyars=0;
           break;
           case 5:
           case 6:
               temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->
                FirstDirectionFormulyars=1;
           break;
           case 7:
               temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->
                FirstDirectionFormulyars=2;
           break;
           case 8:
           case 9:
               temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->
                FirstDirectionFormulyars=3;
           break;
           case 10:
               temp_apl[i_retIndex][iSize_temp_apl[i_retIndex]]->
                FirstDirectionFormulyars=4;
           break;

         };


      }

    }
    iSize_temp_apl[i_retIndex]++;
  }
  LeaveCriticalSection(&csForModifySwap_temp_apl);
  return 1;
}

int TAirplaneList::AddNord(double _Time)
{
   EnterCriticalSection(&csForModifySwap_temp_apl);
    ModifyCircle(_Time,0);
  LeaveCriticalSection(&csForModifySwap_temp_apl);
  return 1;
}



int TAirplaneList::ModifyCircle(double _Time,  //����� � �����
                                     int Sect30)       //����� ������� �� 0 �� 30
{
  int i;
  int iNotCur_tempIndex=(iCur_tempIndex+1)%2;
  double dT,dT1;   //��������� �������
  double dTSec,T1; //������
  int dSect;   //������� ��������
  int dObzor;
  int iCurSect;
  unsigned long MyESP;
  if(Sect30>11||Sect30<0)
  {
    return (-1);
  }
  if(iSect30_temp_apl[iCur_tempIndex]<0)
  {
//��� ������, ��� ����� ������ ������� ��������
//���� ����� ������� <0
//�� ���������� ��������� ������
    iSect30_temp_apl[iCur_tempIndex]=Sect30;
    iSect30_temp_apl[iNotCur_tempIndex]=-1;
    iSize_temp_apl[iCur_tempIndex]=0;
    iFirstSect30NoOut=Sect30;   //��������� ������ ������, ������� �� ���������
    last_time[Sect30]=_Time;
    return iCur_tempIndex;
  }



//������ ������� �������
  dT=_Time-last_time[iSect30_temp_apl[iCur_tempIndex]];

  if(dT<-TO)dT+=24.0;

  if(Sect30==(iSect30_temp_apl[iCur_tempIndex]+1)%12&&dT<0.51*TO)
  {
//���� ����������� ����� ������ �������

      if(iSect30_temp_apl[iNotCur_tempIndex]>=0)
      {
          Swap_temp_apl(iNotCur_tempIndex);   //���������� ����� ����������� �������
      }
      iCur_tempIndex=(iCur_tempIndex+1)%2; //������� �� ������ ������
//������� ������ �������� �������
      iSect30_temp_apl[iCur_tempIndex]=Sect30;


      ModifySector(Sect30);
      last_time[iSect30_temp_apl[iCur_tempIndex]]=_Time;
      return iCur_tempIndex;
  }



  if(Sect30==iSect30_temp_apl[iCur_tempIndex]&&dT<0.51*TO)
  {
//����  ����� ������� iSect30_temp_apl[iCur_tempIndex] ���������� �������
//��� ����� ��  ��� � ����� ������� Sect30 ������...
       last_time[iSect30_temp_apl[iCur_tempIndex]]=_Time;
       return iCur_tempIndex;
  }

  if((Sect30+1)%12==iSect30_temp_apl[iCur_tempIndex]&&dT<0.51*TO)
  {
//���� ������������ ���������� �����
      if(iSect30_temp_apl[iNotCur_tempIndex]<0)
      {
         iSect30_temp_apl[iNotCur_tempIndex]=(Sect30+11)%12;
         iSize_temp_apl[iNotCur_tempIndex]=0;
         iSize_temp_apl_tail[iNotCur_tempIndex]=0;
      }
      last_time[iSect30_temp_apl[iNotCur_tempIndex]]=_Time;
      return iNotCur_tempIndex;
  }



//���������� ������. ��� ����� ��������
//��������� ������� ���� ����� ������ �������


   dSect=Sect30-iSect30_temp_apl[iCur_tempIndex];
   if(dSect<0)dSect+=12;

//������ ��������� ����� ��������� ������ �������
   dTSec=TO/12.0;
   dT1=dT-dTSec*dSect;  //������ ������� ������� ���� �, ���� ��� ��� �� 1 ������

//������ ������� ������� ������
   dObzor=Okrugl(dT1/TO);
   if(dObzor>0)
   {
     dSect=12*dObzor+dSect;
   }



   dTSec=dT/dSect;   //������ ��� ��������� ����� �� ������
   T1=last_time[iSect30_temp_apl[iCur_tempIndex]]+dTSec;
   if(T1>23.99999999999999999) T1-=24.0;
   iCurSect=(iSect30_temp_apl[iCur_tempIndex]+1)%12;
   for(i=0;i<dSect-1;i++)
   {
      ModifyCircle(T1,iCurSect);  //��������� �� �������
      T1+=dTSec;
      if(T1>23.99999999999999999) T1-=24.0;
      iCurSect=(iCurSect+1)%12;
   }
   return ModifyCircle(T1,iCurSect);
}




int TAirplaneList::ModifySector(int Sect30)  //��� ��������� � 1 �������
{
//���� ���������� ��������� ����� � �������� ���  ������� �� ���������� �����
   int dObz,i,i1,j,k;
   airplane_light *apl_t1, *apl_t2;
   airplane **apl1;
   int  iSize_apl_t1, iSize_apl1;
   int  iMaxSize_apl_t2, iSize_apl_t2;

   if(init.iTailLength==0)
   {
 //���� ����� �� ����� - ������ ������������ � �������
        if(iSect30_temp_apl[iCur_tempIndex]==Sect30)
        {
           iSize_temp_apl[iCur_tempIndex]=0;
        }else{
           iSize_apl[iCur_tempIndex]=0;
        }
        return 1;
   }
   apl_t1=apl_tail[Sect30];      //������ ����� ������ ��������
   apl1=apl[Sect30];             //������ ����� ����� ������
   iSize_apl_t1=iSize_apl_tail[Sect30];
   iSize_apl1=iSize_apl[Sect30];


//�������� ���� ���������� � ����� ��� � ������� ��� ������
   if(iSect30_temp_apl[iCur_tempIndex]==Sect30)
   {
//���������� �� ��������� �����
     apl_t2=temp_apl_tail[iCur_tempIndex];
     iMaxSize_apl_t2=iMaxSize_temp_apl_tail[iCur_tempIndex];
   }else{
//���������� � �������, �������������� � ������
     apl_t2=apl_tail[Sect30];
     iMaxSize_apl_t2=iMaxSize_apl_tail[Sect30];
   }


//��������� ������� ������������ ����������� ��������, �� ���� �� ������� ������� �� ����� �����
   for(i=0;i<iSize_apl_t1;i++)
   {
      if(apl_t1[i].circle_number<init.iTailLength)break;
   }
//i - ��� ������� �������� ���� ���������

   if(iSize_apl_t1-i+iSize_apl1>iMaxSize_apl_t2) //�������� ������������
   {
//���� ����� ������������ �������, �� ������ ������ ���� ���������
       iMaxSize_apl_t2=iSize_apl_t1-i+iSize_apl1;
       apl_t2=(airplane_light *)realloc(apl_t2,sizeof(airplane_light)*iMaxSize_apl_t2);
if(apl_t2==NULL)throw EIkoException ("Memory allocation error",
"airoplanelist.cpp","TAirplaneList","ModifySector",
 "apl_t2=(airplane_light *)realloc(apl_t2,sizeof(airplane_light)*iMaxSize_apl_t2)");

//��������� ������� ����� ������� ������
       if(iSect30_temp_apl[iCur_tempIndex]==Sect30)
       {
         temp_apl_tail[iCur_tempIndex]=apl_t2;
         iMaxSize_temp_apl_tail[iCur_tempIndex]=iMaxSize_apl_t2;
       }else{
         apl_tail[Sect30]=apl_t2;
         iMaxSize_apl_tail[Sect30]=iMaxSize_apl_t2;
       }
   }
   i1=i;
   iSize_apl_t2=iSize_apl_t1-i1;


//������ ��������������
   for(i=0;i<iSize_apl_t2;i++)
   {

     apl_t2[i].x=apl_t1[i+i1].x;
     apl_t2[i].y=apl_t1[i+i1].y;
     apl_t2[i].type=apl_t1[i+i1].type;
     apl_t2[i].i_x=apl_t1[i+i1].i_x;
     apl_t2[i].i_y=apl_t1[i+i1].i_y;
//     apl_t2[i].azimuth=apl_t1[i+i1].azimuth;
//     apl_t2[i].dalnost=apl_t1[i+i1].dalnost;
     apl_t2[i].lDopNumber=apl_t1[i+i1].lDopNumber;
     apl_t2[i].circle_number=apl_t1[i+i1].circle_number+1;
     apl_t2[i].IsVisible=apl_t1[i+i1].IsVisible;
     apl_t2[i].lNomerTrassy=apl_t1[i+i1].lNomerTrassy;
     apl_t2[i].lIndexNomerTrassy=apl_t1[i+i1].lIndexNomerTrassy;
     apl_t2[i].lSectorPrev=apl_t1[i+i1].lSectorPrev;
     apl_t2[i].lIndexPrev=apl_t1[i+i1].lIndexPrev-i1;

   }

//������� �������� ������� ��������
   for(i=0;i<N_of_LastAirTail;i++)
   {
//�������� ��������
      if(LastAirTail[i].iNomerTrassy>0&&
         LastAirTail[i].N_Sect30==Sect30)
      {
         LastAirTail[i].N_Indx-=i1;
      }
   }


//��������� �� ����� ��������� 1 � ������ ������
   for(i=0;i<iSize_apl1;i++)
   {
     if(apl1[i]->WasTail&&apl1[i]->lNomerTrassy>0&&
       apl1[i]->N_of_Propusk==0)
     {
        int NaidenaTrassa;  //������� ����, ��� ������ � ������� ������ ���������
        int FirstFree=-1;      //���������
        apl_t2[iSize_apl_t2].x=apl1[i]->x;
        apl_t2[iSize_apl_t2].y=apl1[i]->y;
        apl_t2[iSize_apl_t2].type=apl1[i]->type;
        apl_t2[iSize_apl_t2].i_x=apl1[i]->i_x;
        apl_t2[iSize_apl_t2].i_y=apl1[i]->i_y;
        apl_t2[iSize_apl_t2].circle_number=1;
        apl_t2[iSize_apl_t2].lDopNumber=apl1[i]->lDopNumber;
        apl_t2[iSize_apl_t2].IsVisible=1;      //������� ������� ������
        apl_t2[iSize_apl_t2].lNomerTrassy=apl1[i]->lNomerTrassy;
//
//��������� �����  ����� ������ � ������ �����

        NaidenaTrassa=0;
        for(k=0; k<N_of_LastAirTail;k++)
        {
           if(LastAirTail[k].iNomerTrassy==0&&
              FirstFree<0)FirstFree=k;

           if(LastAirTail[k].iNomerTrassy==apl_t2[iSize_apl_t2].lNomerTrassy)
           {
              NaidenaTrassa=1;
              apl_t2[iSize_apl_t2].lSectorPrev=LastAirTail[k].N_Sect30;
              apl_t2[iSize_apl_t2].lIndexPrev=LastAirTail[k].N_Indx;
              LastAirTail[k].N_Sect30=Sect30;
              LastAirTail[k].N_Indx=iSize_apl_t2;

              break;
           }
        }

//�������� �� ������ ������, � ��� �� ��� ���� ������ ����� �����������, �����
//��������� ������������ �� ������� �����



//������ ���� �������� ������
//��� ��� ��������
        if(!NaidenaTrassa)
        {
           if(FirstFree<0)
           {
            if(N_of_LastAirTail==MaxN_of_LastAirTail)
            {
              LastAirTail=(TLastAirTail*)realloc(LastAirTail,
               (MaxN_of_LastAirTail+20)*sizeof(TLastAirTail));
if(LastAirTail==NULL)throw EIkoException ("Memory allocation error",
"airoplanelist.cpp","TAirplaneList","ModifySector",
"LastAirTail=(TLastAirTail*)realloc(LastAirTail,...");
               MaxN_of_LastAirTail+=20;
            }

             LastAirTail[N_of_LastAirTail].iNomerTrassy=
                apl_t2[iSize_apl_t2].lNomerTrassy;
             LastAirTail[N_of_LastAirTail].N_Sect30=Sect30;
             LastAirTail[N_of_LastAirTail].N_Indx=iSize_apl_t2;
             LastAirTail[N_of_LastAirTail].IsDelete=0;
             apl_t2[iSize_apl_t2].lIndexNomerTrassy=N_of_LastAirTail;
             N_of_LastAirTail++;
           }else{
             LastAirTail[FirstFree].iNomerTrassy=
                apl_t2[iSize_apl_t2].lNomerTrassy;
             LastAirTail[FirstFree].N_Sect30=Sect30;
             LastAirTail[FirstFree].N_Indx=iSize_apl_t2;
             LastAirTail[FirstFree].IsDelete=0;
             apl_t2[FirstFree].lIndexNomerTrassy=FirstFree;
           }
           apl_t2[iSize_apl_t2].lSectorPrev=apl_t2[iSize_apl_t2].lIndexPrev=-1;
        }


//���� ����� ������
        iSize_apl_t2++;
     }
   }

//������� �� �� �����
   if(iSect30_temp_apl[iCur_tempIndex]==Sect30)
   {
         iSize_temp_apl_tail[iCur_tempIndex]=iSize_apl_t2;
         iSize_temp_apl[iCur_tempIndex]=0;
   }else{
         iSize_apl_tail[Sect30]=iSize_apl_t2;
         iSize_apl[Sect30]=0;
    }
   return 1;
}

//��������
int sort_airplane( const void *a, const void *b)
{
  airplane **ar1=(airplane **)a;
  airplane **ar2=(airplane **)b;
  if((*ar1)->dalnost>(*ar2)->dalnost)return (-1);
  if((*ar1)->dalnost<(*ar2)->dalnost)return (1);
  return 0;
}



/*��������������, �� ���� ����������� �������������� ������ � ������� ������ */
int TAirplaneList::Swap_temp_apl(int i_tempIndex1)
{
   struct airplane **temp;
   struct airplane_light *temp_light;
   int i_temp;
   int k1,k2,k;
   long Naid_j;
   long Sct,Indx,Sct1,Indx1;   //����� ������� � ������
   airplane *temp1;
   long prev_circle_number;
   long Zapoln1,Zapoln2;
   int N_of_Prl=0;
   if(i_tempIndex1<0||i_tempIndex1>1||iSect30_temp_apl[i_tempIndex1]<0)return 0;


//���� ��� ��������� �������������� ������
   dLastRadian=(M_PI/6.0)*((iSect30_temp_apl[i_tempIndex1]+1)%12);
   k=iSect30_temp_apl[i_tempIndex1];
   k1=iSize_apl[k];


   for(int i=0;i<k1;i++)
   {
//������ ��� �������� ������� �� ������� � �������
       if(apl[k][i]->FD)
       {
//������ ���������
            apl[k][i]->FD->FreeFormulyar();
            apl[k][i]->FD=NULL;
       }

       if(!apl[k][i]->DeleteIt&&
          apl[k][i]->lNomerTrassy>0)
       {
//� ��� ���� ������� ���� �� ���� ���������
          if(apl[k][i]->N_of_Propusk==init.N_of_Potery)
          {
//������ ������������ ��� ������ ������ ������
//������ ���� � ������
            Naid_j=-1;
            for(int j=0;j<N_of_LastAirTail;j++)
            {
                if(LastAirTail[j].iNomerTrassy!=0&&
  LastAirTail[j].iNomerTrassy==apl[k][i]->lNomerTrassy)
                {
                  Naid_j=j;
                  break;
                }
            }
            if(Naid_j>=0)
            {
               LastAirTail[Naid_j].IsDelete=1;
            }
//������ ������ ���������� �� �����������
/*
            if(apl[k][i]->GetProbData)
            {
              delete ((TOcenkaVeroyatnosti*)(apl[k][i]->GetProbData));
              apl[k][i]->GetProbData=NULL;
            }
            ������ ����������� ����
*/
          }else{
            apl[k][i]->N_of_Propusk++;
/*
            if(apl[k][i]->GetProbData)
            {
       ((TOcenkaVeroyatnosti*)(apl[k][i]->GetProbData))->AddOtmetka(10);
            }
   ������ ����������� ����         
*/
//��������� ������ ������� ������
//������������� ������

//�������� ����� �������� ������� ������ ��
            if(iSize_temp_apl[i_tempIndex1]==
            iMaxSize_temp_apl[i_tempIndex1])
            {
               temp_apl[i_tempIndex1]=
                 (airplane**)realloc(temp_apl[i_tempIndex1],
                  sizeof(airplane*)*(iMaxSize_temp_apl[i_tempIndex1]+10));
if(temp_apl[i_tempIndex1]==NULL)throw EIkoException ("Memory allocation error",
"airoplanelist.cpp","TAirplaneList","Swap_temp_apl",
"temp_apl[i_tempIndex1]=(airplane**)realloc(temp_apl[i_tempIndex1],...");

//������� ������
                for(int kp=iMaxSize_temp_apl[i_tempIndex1];
                  kp<iMaxSize_temp_apl[i_tempIndex1]+10;
                  kp++)
                {
                    temp_apl[i_tempIndex1][kp]=(airplane*)malloc(sizeof(airplane));
if(temp_apl[i_tempIndex1][kp]==NULL)throw EIkoException ("Memory allocation error",
"airoplanelist.cpp","TAirplaneList","Swap_temp_apl",
"temp_apl[i_tempIndex1][kp]=(airplane*)malloc(sizeof(airplane));");
/*
temp_apl[i_tempIndex1][kp]->GetProbData=NULL;
������ ����������� ����
*/
                }
                iMaxSize_temp_apl[i_tempIndex1]+=10;
            }
            memcpy(
temp_apl[i_tempIndex1][iSize_temp_apl[i_tempIndex1]],
apl[k][i],sizeof(airplane));
            iSize_temp_apl[i_tempIndex1]++;
          }
       };
   }

//�������� ���������
   if(iSize_temp_apl[i_tempIndex1]>0)
   {
//���������� �������� ����� ����� �������� 1, 2 � 3

      if(iSize_temp_apl[i_tempIndex1]==1)
      {
         temp_apl[i_tempIndex1][0]->FD=
       TFormulyarDraw::AddFormulyar(
       temp_apl[i_tempIndex1][0]->i_x,
       temp_apl[i_tempIndex1][0]->i_y,
       temp_apl[i_tempIndex1][0]);
      }else if(iSize_temp_apl[i_tempIndex1]==2)
      {
         if(temp_apl[i_tempIndex1][0]->dalnost>
            temp_apl[i_tempIndex1][1]->dalnost)
         {
            temp_apl[i_tempIndex1][0]->FD=
         TFormulyarDraw::AddFormulyar(
           temp_apl[i_tempIndex1][0]->i_x,
           temp_apl[i_tempIndex1][0]->i_y,
           temp_apl[i_tempIndex1][0]);
            temp_apl[i_tempIndex1][1]->FD=
         TFormulyarDraw::AddFormulyar(
           temp_apl[i_tempIndex1][1]->i_x,
           temp_apl[i_tempIndex1][1]->i_y,
           temp_apl[i_tempIndex1][1]);
         }else{
          temp_apl[i_tempIndex1][1]->FD=
            TFormulyarDraw::AddFormulyar(
             temp_apl[i_tempIndex1][1]->i_x,
             temp_apl[i_tempIndex1][1]->i_y,
             temp_apl[i_tempIndex1][1]);
           temp_apl[i_tempIndex1][0]->FD=
            TFormulyarDraw::AddFormulyar(
             temp_apl[i_tempIndex1][0]->i_x,
             temp_apl[i_tempIndex1][0]->i_y,
             temp_apl[i_tempIndex1][0]);
             temp1=temp_apl[i_tempIndex1][0];
             temp_apl[i_tempIndex1][0]=temp_apl[i_tempIndex1][1];
             temp_apl[i_tempIndex1][1]=temp1;
         }
      }else if(iSize_temp_apl[i_tempIndex1]==3)
      {
//���� ��� �������
         if(temp_apl[i_tempIndex1][0]->dalnost>
            temp_apl[i_tempIndex1][1]->dalnost)
         {
            if(temp_apl[i_tempIndex1][2]->dalnost>
               temp_apl[i_tempIndex1][0]->dalnost)
            {
              temp_apl[i_tempIndex1][2]->FD=
               TFormulyarDraw::AddFormulyar(
                temp_apl[i_tempIndex1][2]->i_x,
                temp_apl[i_tempIndex1][2]->i_y,
                temp_apl[i_tempIndex1][2]);
              temp_apl[i_tempIndex1][0]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][0]->i_x,
                 temp_apl[i_tempIndex1][0]->i_y,
                 temp_apl[i_tempIndex1][0]);
              temp_apl[i_tempIndex1][1]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][1]->i_x,
                 temp_apl[i_tempIndex1][1]->i_y,
                 temp_apl[i_tempIndex1][1]);

                 temp1=temp_apl[i_tempIndex1][2];


            }else if(temp_apl[i_tempIndex1][2]->dalnost>
               temp_apl[i_tempIndex1][1]->dalnost)
            {
              temp_apl[i_tempIndex1][0]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][0]->i_x,
                 temp_apl[i_tempIndex1][0]->i_y,
                 temp_apl[i_tempIndex1][0]);
              temp_apl[i_tempIndex1][2]->FD=
               TFormulyarDraw::AddFormulyar(
                temp_apl[i_tempIndex1][2]->i_x,
                temp_apl[i_tempIndex1][2]->i_y,
                temp_apl[i_tempIndex1][2]);
              temp_apl[i_tempIndex1][1]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][1]->i_x,
                 temp_apl[i_tempIndex1][1]->i_y,
                 temp_apl[i_tempIndex1][1]);
            }else{
              temp_apl[i_tempIndex1][0]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][0]->i_x,
                 temp_apl[i_tempIndex1][0]->i_y,
                 temp_apl[i_tempIndex1][0]);
              temp_apl[i_tempIndex1][1]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][1]->i_x,
                 temp_apl[i_tempIndex1][1]->i_y,
                 temp_apl[i_tempIndex1][1]);
              temp_apl[i_tempIndex1][2]->FD=
               TFormulyarDraw::AddFormulyar(
                temp_apl[i_tempIndex1][2]->i_x,
                temp_apl[i_tempIndex1][2]->i_y,
                temp_apl[i_tempIndex1][2]);
            }
         }else{
//���� temp_apl[i_tempIndex1][0]->dalnost<=
//            temp_apl[i_tempIndex1][1]->dalnost
            if(temp_apl[i_tempIndex1][2]->dalnost>
               temp_apl[i_tempIndex1][1]->dalnost)
            {
              temp_apl[i_tempIndex1][2]->FD=
               TFormulyarDraw::AddFormulyar(
                temp_apl[i_tempIndex1][2]->i_x,
                temp_apl[i_tempIndex1][2]->i_y,
                temp_apl[i_tempIndex1][2]);
              temp_apl[i_tempIndex1][1]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][1]->i_x,
                 temp_apl[i_tempIndex1][1]->i_y,
                 temp_apl[i_tempIndex1][1]);
              temp_apl[i_tempIndex1][0]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][0]->i_x,
                 temp_apl[i_tempIndex1][0]->i_y,
                 temp_apl[i_tempIndex1][0]);
            }else if(temp_apl[i_tempIndex1][2]->dalnost>
               temp_apl[i_tempIndex1][0]->dalnost)
            {
              temp_apl[i_tempIndex1][1]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][1]->i_x,
                 temp_apl[i_tempIndex1][1]->i_y,
                 temp_apl[i_tempIndex1][1]);
              temp_apl[i_tempIndex1][2]->FD=
               TFormulyarDraw::AddFormulyar(
                temp_apl[i_tempIndex1][2]->i_x,
                temp_apl[i_tempIndex1][2]->i_y,
                temp_apl[i_tempIndex1][2]);
              temp_apl[i_tempIndex1][0]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][0]->i_x,
                 temp_apl[i_tempIndex1][0]->i_y,
                 temp_apl[i_tempIndex1][0]);
            }else{
              temp_apl[i_tempIndex1][1]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][1]->i_x,
                 temp_apl[i_tempIndex1][1]->i_y,
                 temp_apl[i_tempIndex1][1]);
              temp_apl[i_tempIndex1][0]->FD=
                TFormulyarDraw::AddFormulyar(
                 temp_apl[i_tempIndex1][0]->i_x,
                 temp_apl[i_tempIndex1][0]->i_y,
                 temp_apl[i_tempIndex1][0]);
              temp_apl[i_tempIndex1][2]->FD=
               TFormulyarDraw::AddFormulyar(
                temp_apl[i_tempIndex1][2]->i_x,
                temp_apl[i_tempIndex1][2]->i_y,
                temp_apl[i_tempIndex1][2]);
            }
         }
      }else{
//��� ��� ������ 3. ���� ����������� ������� ����� � ��������
        qsort(temp_apl[i_tempIndex1],iSize_temp_apl[i_tempIndex1],
               sizeof(airplane*),sort_airplane);


        for(int i=0;i<iSize_temp_apl[i_tempIndex1];i++)
        {
           temp_apl[i_tempIndex1][i]->FD=
             TFormulyarDraw::AddFormulyar(
                temp_apl[i_tempIndex1][i]->i_x,
                temp_apl[i_tempIndex1][i]->i_y,
                temp_apl[i_tempIndex1][i]);
        }
      }


   }

//�������� ����� �������

   temp=apl[iSect30_temp_apl[i_tempIndex1]];
   apl[iSect30_temp_apl[i_tempIndex1]]=temp_apl[i_tempIndex1];
   temp_apl[i_tempIndex1]=temp;
   i_temp=iMaxSize_apl[iSect30_temp_apl[i_tempIndex1]];
   iMaxSize_apl[iSect30_temp_apl[i_tempIndex1]]=iMaxSize_temp_apl[i_tempIndex1];
   iMaxSize_temp_apl[i_tempIndex1]=i_temp;
   iSize_apl[iSect30_temp_apl[i_tempIndex1]]=iSize_temp_apl[i_tempIndex1];
   temp_light=apl_tail[iSect30_temp_apl[i_tempIndex1]];
   apl_tail[iSect30_temp_apl[i_tempIndex1]]=temp_apl_tail[i_tempIndex1];
   temp_apl_tail[i_tempIndex1]=temp_light;
   i_temp=iMaxSize_apl_tail[iSect30_temp_apl[i_tempIndex1]];
   iMaxSize_apl_tail[iSect30_temp_apl[i_tempIndex1]]=iMaxSize_temp_apl_tail[i_tempIndex1];
   iMaxSize_temp_apl_tail[i_tempIndex1]=i_temp;
   iSize_apl_tail[iSect30_temp_apl[i_tempIndex1]]=iSize_temp_apl_tail[i_tempIndex1];
   iSize_temp_apl[i_tempIndex1]=0;
   iSize_temp_apl_tail[i_tempIndex1]=0;
   iSect30_temp_apl[i_tempIndex1]=-1; //�� ���� ���� �������
   iNewView=1;    //����� ��� ������

//��� ���� ��������� ��� ������ ������� ���� �������
//����� ������� ����������� ����������� �� ���������� � ���������� �������

   for(int i=0;i<N_of_LastAirTail;i++)
   {
      if(LastAirTail[i].iNomerTrassy>0)
      if(LastAirTail[i].IsDelete)
      {
         Sct=LastAirTail[i].N_Sect30;
         Indx=LastAirTail[i].N_Indx;
               N_of_Prl=0;
               while(Indx>=0&&
                     Sct<12&&Sct>=0)
               {
                  N_of_Prl++;
                  apl_tail[Sct][Indx].IsVisible=0;
                  Sct1=apl_tail[Sct][Indx].lSectorPrev;
                  Indx1=apl_tail[Sct][Indx].lIndexPrev;
                  if(Sct==Sct1&&Indx==Indx1)
                  {
                      break;
                  }
                  if(N_of_Prl>init.iTailLength+10)
                  {
                    break;
                  }
                  Sct=Sct1;
                  Indx=Indx1;
               };
         LastAirTail[i].iNomerTrassy=0;
      }else{
 //��� ���� ������� ����������� ����������� �� 10 ������� ���������� � �������
         if(init.IsOutVeroyatnostPervKanal||init.IsOutVeroyatnostVtorKanal)
         {
//���� ������� ����������� ����������� �� ���������� � ���������� �������

         }
      }
   }
//������� ��������� ��� ���� �������� � �������


   return 1;
}



//����� ������ � �� DC
int TAirplaneList::PaintData(HDC DC)  //����� ������ �� ���������� ����������
{
   int i,ict,j;
   int last_i[12]={0,0,0,0,0,0,0,0,0,0,0,0};    //��������� ������� ��� ���� �������
   unsigned char R1,G1,B1;
   int Rt1,Gt1,Bt1,Rt2,Gt2,Bt2;
   int rad[3]={init.radius1,init.radius2,init.radius3};
   HPEN OldPen;
   HBRUSH OldBrush;

/*����� �������� ��������*/
   LOGPEN CirclePen_t[3];
   LOGBRUSH CircleBrush_t[3];

/*����� ����� ��� ����������� ��������*/
   LOGPEN CirclePen_t_t[3];
   LOGBRUSH CircleBrush_t_t[3];


   HPEN Pen_t[3], Pen_t_t[3];
   HBRUSH Brush_t[3], Brush_t_t[3];




   int lft,rght,tp,dn;       //���������� ��������, ���� ����� ������ ���� �������



   long T1,T2;
   POINT Rhomb[4];

//������� ����� ��������
   memcpy(CirclePen_t,    &(init.CirclePen1),sizeof(LOGPEN));
   memcpy(CirclePen_t+1,  &(init.CirclePen2),sizeof(LOGPEN));
   memcpy(CirclePen_t+2,  &(init.CirclePen3),sizeof(LOGPEN));
   memcpy(CircleBrush_t,  &(init.CircleBrush1),sizeof(LOGBRUSH));
   memcpy(CircleBrush_t+1,&(init.CircleBrush2),sizeof(LOGBRUSH));
   memcpy(CircleBrush_t+2,&(init.CircleBrush3),sizeof(LOGBRUSH));

   memcpy(CirclePen_t_t,    &(init.CirclePen1),sizeof(LOGPEN));
   memcpy(CirclePen_t_t+1,  &(init.CirclePen2),sizeof(LOGPEN));
   memcpy(CirclePen_t_t+2,  &(init.CirclePen3),sizeof(LOGPEN));
   memcpy(CircleBrush_t_t,  &(init.CircleBrush1),sizeof(LOGBRUSH));
   memcpy(CircleBrush_t_t+1,&(init.CircleBrush2),sizeof(LOGBRUSH));
   memcpy(CircleBrush_t_t+2,&(init.CircleBrush3),sizeof(LOGBRUSH));

//������� ������� ����� ����� �������
   Rt2=GetRValue(init.Clrbackground);
   Gt2=GetGValue(init.Clrbackground);
   Bt2=GetBValue(init.Clrbackground);

   Rt1=GetRValue(CirclePen_t_t[0].lopnColor);
   Gt1=GetGValue(CirclePen_t_t[0].lopnColor);
   Bt1=GetBValue(CirclePen_t_t[0].lopnColor);
   CirclePen_t_t[0].lopnColor=RGB((Rt1+Rt2)/2,
                                  (Gt1+Gt2)/2,
                                  (Bt1+Bt2)/2);
   Rt1=GetRValue(CirclePen_t_t[1].lopnColor);
   Gt1=GetGValue(CirclePen_t_t[1].lopnColor);
   Bt1=GetBValue(CirclePen_t_t[1].lopnColor);
   CirclePen_t_t[1].lopnColor=RGB((Rt1+Rt2)/2,
                                  (Gt1+Gt2)/2,
                                  (Bt1+Bt2)/2);
   Rt1=GetRValue(CirclePen_t_t[2].lopnColor);
   Gt1=GetGValue(CirclePen_t_t[2].lopnColor);
   Bt1=GetBValue(CirclePen_t_t[2].lopnColor);
   CirclePen_t_t[2].lopnColor=RGB((Rt1+Rt2)/2,
                                  (Gt1+Gt2)/2,
                                  (Bt1+Bt2)/2);

   Rt1=GetRValue(CircleBrush_t_t[0].lbColor);
   Gt1=GetGValue(CircleBrush_t_t[0].lbColor);
   Bt1=GetBValue(CircleBrush_t_t[0].lbColor);
   CircleBrush_t_t[0].lbColor=RGB((Rt1+Rt2)/2,
                                  (Gt1+Gt2)/2,
                                  (Bt1+Bt2)/2);
   Rt1=GetRValue(CircleBrush_t_t[1].lbColor);
   Gt1=GetGValue(CircleBrush_t_t[1].lbColor);
   Bt1=GetBValue(CircleBrush_t_t[1].lbColor);
   CircleBrush_t_t[1].lbColor=RGB((Rt1+Rt2)/2,
                                  (Gt1+Gt2)/2,
                                  (Bt1+Bt2)/2);
   Rt1=GetRValue(CircleBrush_t_t[2].lbColor);
   Gt1=GetGValue(CircleBrush_t_t[2].lbColor);
   Bt1=GetBValue(CircleBrush_t_t[2].lbColor);
   CircleBrush_t_t[2].lbColor=RGB((Rt1+Rt2)/2,
                                  (Gt1+Gt2)/2,
                                  (Bt1+Bt2)/2);

    for(i=0;i<3;i++)
    {
      Pen_t[i]=CreatePenIndirect(&CirclePen_t[i]);
      if(Pen_t[i]==NULL)
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData","Pen_t[i]=CreatePenIndirect(...");

      Pen_t_t[i]=CreatePenIndirect(&CirclePen_t_t[i]);
      if(Pen_t_t[i]==NULL)
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData","Pen_t_[i]=CreatePenIndirect(...");

      Brush_t[i]=CreateBrushIndirect(&CircleBrush_t[i]);
      if(Brush_t[i]==NULL)
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData","Brush_t[i]=CreateBrushIndirect(...");

      Brush_t_t[i]=CreateBrushIndirect(&CircleBrush_t_t[i]);
      if(Brush_t_t[i]==NULL)
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData","Brush_t_[i]=CreateBrushIndirect(...");
    }

//���������� ��������� ��� �������

   OldPen=(HPEN)SelectObject(DC,Pen_t[0]);
   if(!OldPen)
   {
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData","OldPen=(HPEN)SelectObject(DC,Pen_t[0])");
   }
   OldBrush=(HBRUSH)SelectObject(DC,Brush_t[0]);
   if(!OldBrush)
   {
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData",
"   OldBrush=(HBRUSH)SelectObject(DC,Brush_t[0]);");
   }

//������ � ����������� ������
   EnterCriticalSection(&csForModifySwap_temp_apl);
//������� ������


//�������� �����, ������� �������� �������� �� ������
      for(i=0;i<12;i++)
      {
          last_i[i]=iSize_apl_tail[i];
          for(j=0;j<iSize_apl_tail[i];j++)
          {
              if(apl_tail[i][j].circle_number<=init.iTailLength)
              {
                   last_i[i]=j;
                   break;
              }
          }
      }


      if(init.iTail)
      {
       for(ict=init.iTailLength;ict>=1;ict--)
       {
          for(i=0;i<12;i++)
          {
            for(j=last_i[i];j<iSize_apl_tail[i];j++)
            {
              if(apl_tail[i][j].circle_number!=ict)
              {
                 last_i[i]=j;
                 break;
              }
//������� �������
//��� ������ ������ ����
              if(apl_tail[i][j].type<0||apl_tail[i][j].type>2)continue;

//��������, � �������� �� ������� � �������� �������, � ����� �� �� ������� ��
              if(apl_tail[i][j].i_x>=0&&apl_tail[i][j].i_x<szArea.cx&&
                 apl_tail[i][j].i_y>=0&&apl_tail[i][j].i_y<szArea.cy)
              {
                 if(!SelectObject(DC,Pen_t_t[apl_tail[i][j].type]))
                 {
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData",
"if(!SelectObject(DC,Pen_t_t[apl_tail[i][j].type]))");
                 }

//��� �������
                 if(!SelectObject(DC,Brush_t_t[apl_tail[i][j].type]))
                 {
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData",
"if(!SelectObject(DC,Brush_t_t[apl_tail[i][j].type]))");
                 }
//�������� ���� �������
                 if(apl_tail[i][j].IsVisible)
                 switch(apl_tail[i][j].type)
                 {
                   case 0:

                       MoveToEx(DC,
                        apl_tail[i][j].i_x-rad[0],
                        apl_tail[i][j].i_y-rad[0],NULL);
                       LineTo(DC,
                        apl_tail[i][j].i_x+rad[0],
                        apl_tail[i][j].i_y+rad[0]);
                       MoveToEx(DC,
                        apl_tail[i][j].i_x-rad[0],
                        apl_tail[i][j].i_y+rad[0],NULL);
                       LineTo(DC,
                        apl_tail[i][j].i_x+rad[0],
                        apl_tail[i][j].i_y-rad[0]);

                   break;
                   case 1:

                       MoveToEx(DC,
                        apl_tail[i][j].i_x-rad[1],
                        apl_tail[i][j].i_y,NULL);
                       LineTo(DC,
                        apl_tail[i][j].i_x+rad[1],
                        apl_tail[i][j].i_y);
                       MoveToEx(DC,
                        apl_tail[i][j].i_x,
                        apl_tail[i][j].i_y-rad[1],NULL);
                       LineTo(DC,
                        apl_tail[i][j].i_x,
                        apl_tail[i][j].i_y+rad[1]);
                   break;
                   case 2:
                        Rectangle(DC,
                        apl_tail[i][j].i_x-rad[2],
                        apl_tail[i][j].i_y-rad[2],
                        apl_tail[i][j].i_x+rad[2],
                        apl_tail[i][j].i_y+rad[2]);
                   break;
                 }

              }
            }
         }
      }
     }

//������� ������� ������
   for(i=0;i<12;i++)
   {
      for(j=0;j<iSize_apl[i];j++)
      {
          if(apl[i][j]->N_of_Propusk==0)
          {
              if(!SelectObject(DC,Pen_t[apl[i][j]->type]))
              {
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData",
"if(!SelectObject(DC,Pen_t[apl[i][j]->type]))");

              }
              if(!SelectObject(DC,Brush_t[apl[i][j]->type]))
              {
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData",
"if(!SelectObject(DC,Brush_t[apl[i][j]->type]))");
              }
           }else{
              if(!SelectObject(DC,Pen_t_t[apl[i][j]->type]))
              {
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData",
"if(!SelectObject(DC,Pen_t_t[apl[i][j]->type]))");

              }
              if(!SelectObject(DC,Brush_t_t[apl[i][j]->type]))
              {
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData",
"if(!SelectObject(DC,Brush_t_t[apl[i][j]->type]))");
              }
           }

//�������� ������� � ����� � ����� � ����������� �� ����

                 switch(apl[i][j]->type)
                 {
                   case 0:
                       MoveToEx(DC,
                        apl[i][j]->i_x-rad[0],
                        apl[i][j]->i_y-rad[0],NULL);
                       LineTo(DC,
                        apl[i][j]->i_x+rad[0],
                        apl[i][j]->i_y+rad[0]);
                       MoveToEx(DC,
                        apl[i][j]->i_x+rad[0],
                        apl[i][j]->i_y-rad[0],NULL);
                       LineTo(DC,
                        apl[i][j]->i_x-rad[0],
                        apl[i][j]->i_y+rad[0]);
                   break;
                   case 1:
                       MoveToEx(DC,
                        apl[i][j]->i_x-rad[1],
                        apl[i][j]->i_y,NULL);
                       LineTo(DC,
                        apl[i][j]->i_x+rad[1],
                        apl[i][j]->i_y);
                       MoveToEx(DC,
                        apl[i][j]->i_x,
                        apl[i][j]->i_y-rad[1],NULL);
                       LineTo(DC,
                        apl[i][j]->i_x,
                        apl[i][j]->i_y+rad[1]);
                   break;
                   case 2:
                        Rectangle(DC,
                        apl[i][j]->i_x-rad[2],
                        apl[i][j]->i_y-rad[2],
                        apl[i][j]->i_x+rad[2],
                        apl[i][j]->i_y+rad[2]);
                   break;
                 }

      }
   }
//�� �������
   if(!SelectObject(DC,OldPen))
   {
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData",
"if(!SelectObject(DC,OldPen))");
   }

   if(!SelectObject(DC,OldBrush))
   {
      throw EIkoException ("Error graphical output",
"airoplanelist.cpp","TAirplaneList","PaintData",
"if(!SelectObject(DC,OldBrush))");
   }

//������ ��� �������
   for(i=0;i<3;i++)
   {
      DeleteObject(Pen_t[i]);       DeleteObject(Pen_t_t[i]);
      DeleteObject(Brush_t[i]);     DeleteObject(Brush_t_t[i]);
   }


//������� ���������
   if(init.IsOutFormulyar)
   {
//   TFormulyarDraw::PaintAll(DC);
      for(i=0;i<12;i++)
      {
        for(j=0;j<iSize_apl[i];j++)
        {
           if(apl[i][j]->FD)
           {
             apl[i][j]->FD->PaintOne(DC);
           }

        }
      }
   }
   iNewView=0;   //��� �����  - �� ���������� ������
   LeaveCriticalSection(&csForModifySwap_temp_apl);
   return 1;
}




//��������� ��������, ��������� ��������� ��� ������� ������ - �������� ���� ���������
int TAirplaneList::SetNewAreaStates(
                POINT &_pntRLS,         //��������� ���
                SIZE & _szArea,         //������ ������
                initialization &_init,  //��������� �������������
                double _KoefMasshtab)   //����������
{
  int i,j;
  int iNotCur_tempIndex;


  EnterCriticalSection(&csForModifySwap_temp_apl);

//�������� ���������� ��������� - ��������, ����� �������������� ��� �������������
//����������
  if(memcmp(&pntRLS,&_pntRLS,sizeof(pntRLS))||
     memcmp(&szArea,&_szArea,sizeof(szArea))||
     ABS(_KoefMasshtab-dfKoefMasshtab)>0.00000001)
  {
//��������� ������
      memcpy(&pntRLS,&_pntRLS,sizeof(pntRLS));
      memcpy(&szArea,&_szArea,sizeof(szArea));
      memcpy(&init,&_init,sizeof(init));
      dfKoefMasshtab=_KoefMasshtab;
//��������� ��������
      TFormulyarDraw::SetInitialization(init, szArea.cx, szArea.cy);
      for(i=0;i<12;i++)
      {
        for(j=0;j<iSize_apl[i];j++)
        {
           apl[i][j]->i_x=pntRLS.x+Okrugl(apl[i][j]->x*dfKoefMasshtab);
           apl[i][j]->i_y=pntRLS.y-Okrugl(apl[i][j]->y*dfKoefMasshtab);
           apl[i][j]->FD=NULL;
        }
        for(j=0;j<iSize_apl_tail[i];j++)
        {
           apl_tail[i][j].i_x=pntRLS.x+Okrugl(apl_tail[i][j].x*dfKoefMasshtab);
           apl_tail[i][j].i_y=pntRLS.y-Okrugl(apl_tail[i][j].y*dfKoefMasshtab);
        }
      }
      for(j=0;j<iSize_temp_apl[0];j++)
      {
         temp_apl[0][j]->i_x=pntRLS.x+Okrugl(temp_apl[0][j]->x*dfKoefMasshtab);
         temp_apl[0][j]->i_y=pntRLS.y-Okrugl(temp_apl[0][j]->y*dfKoefMasshtab);
      }
      for(j=0;j<iSize_temp_apl[1];j++)
      {
         temp_apl[1][j]->i_x=pntRLS.x+Okrugl(temp_apl[1][j]->x*dfKoefMasshtab);
         temp_apl[1][j]->i_y=pntRLS.y-Okrugl(temp_apl[1][j]->y*dfKoefMasshtab);
      }
      for(j=0;j<iSize_temp_apl_tail[0];j++)
      {
         temp_apl_tail[0][j].i_x=pntRLS.x+Okrugl(temp_apl_tail[0][j].x*dfKoefMasshtab);
         temp_apl_tail[0][j].i_y=pntRLS.y-Okrugl(temp_apl_tail[0][j].y*dfKoefMasshtab);
      }
      for(j=0;j<iSize_temp_apl_tail[1];j++)
      {
         temp_apl_tail[1][j].i_x=pntRLS.x+Okrugl(temp_apl_tail[1][j].x*dfKoefMasshtab);
         temp_apl_tail[1][j].i_y=pntRLS.y-Okrugl(temp_apl_tail[1][j].y*dfKoefMasshtab);
      }
//������ ����������� ���������, ���� ��� ������ ����
      if(init.IsOutFormulyar)
      {
        iNotCur_tempIndex=(iCur_tempIndex+1)%2;
        for(i=0;i<12;i++)
        {
          for(j=0;j<iSize_apl[i];j++)
          {
             apl[i][j]->FD=TFormulyarDraw::AddFormulyar(apl[i][j]->i_x,
                                                        apl[i][j]->i_y,
                                                        apl[i][j]);

          }
        }
        for(i=0;i<iSize_temp_apl[iNotCur_tempIndex];i++)
        {
           temp_apl[iNotCur_tempIndex][i]->FD=TFormulyarDraw::
             AddFormulyar(temp_apl[iNotCur_tempIndex][i]->i_x,
                          temp_apl[iNotCur_tempIndex][i]->i_y,
                          temp_apl[iNotCur_tempIndex][i]);
        }
        for(i=0;i<iSize_temp_apl[iCur_tempIndex];i++)
        {
           temp_apl[iCur_tempIndex][i]->FD=TFormulyarDraw::
             AddFormulyar(temp_apl[iCur_tempIndex][i]->i_x,
                          temp_apl[iCur_tempIndex][i]->i_y,
                          temp_apl[iCur_tempIndex][i]);
        }

      }
      iNewView=1;
      LeaveCriticalSection(&csForModifySwap_temp_apl);
      return 1;
  }

//��������� ������ ���� �������� ������ ���� �������������
  if(memcmp(&init,&_init,sizeof(init)))
  {
//��������� ��������
     memcpy(&init,&_init,sizeof(init));
     TFormulyarDraw::SetInitialization(init, szArea.cx, szArea.cy);
//������ ����������� ���������, ���� ��� ������ ����
     if(init.IsOutFormulyar)
     {
        for(i=0;i<12;i++)
        {
          for(j=0;j<iSize_apl[i];j++)
          {
             apl[i][j]->FD=TFormulyarDraw::AddFormulyar(
                                                    apl[i][j]->i_x,
                                                    apl[i][j]->i_y,
                                                    apl[i][j]);
          }
        }
     }
     LeaveCriticalSection(&csForModifySwap_temp_apl);
     return 1;
  }
  iNewView=1;
  LeaveCriticalSection(&csForModifySwap_temp_apl);
  return 0;
}

//����������� �� ������� ������ ���� ���������
int TAirplaneList::HaveAnyFormulyarPixel(int Xm, int Ym)
{
  return TFormulyarDraw::AnyHavePixel(Xm,Ym);
}

//���������� 1 ������ � ��� ��������, ������� ������
int TAirplaneList::PaintOpaqueFormulyar(HDC DC, int Xm, int Ym)
{
  EnterCriticalSection(&csForModifySwap_temp_apl);

  airplane *aaa=TFormulyarDraw::GetOpaque(Xm,Ym);
  if(aaa)
  {
    if(aaa->FD)
    {
      aaa->FD->PaintOne(DC);
      aaa->TransFormulyar=1;
    }
  }
  LeaveCriticalSection(&csForModifySwap_temp_apl);

  return 1;
}


// ����� � ��������� ��������
int TAirplaneList::FindAndRotateFormuluar(int Xp, int Yp,
                      int Kuda)
{
 return TFormulyarDraw::NaitiIPovernut(Xp,Yp,Kuda);
}



