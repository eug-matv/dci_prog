//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <string.h>
#include <stdlib.h>
#include "MainRBZ.h"
#include "in_and_log_thread.h"
#include "paketstruct.h"
#include "AnyTools.h"
#include "sopr_aorl_1a.h"
#include "LogFile.h"
#include "kio_exeptions.h"


#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TInAndLogDataThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TInAndLogDataThread::TInAndLogDataThread(
           HANDLE hEvent, //������� ���� ��� ������� ������
           TMainCircleOut *_MCO,  //��� ����� ������
           TSoprSaveSession *_SSS,   //���������� � ���� ������
           TSoprSaveSession *_SSS_Sopr, //���������� �������������� ������
           initialization &_init        //��������� �������������
            ): TThread(false)
{
   FreeOnTerminate=true;   //������������� ������� ������
   MCO=_MCO;
   hPriem=hEvent;          //���������
   hStop=CreateEvent(NULL,true,false,NULL); //�������
   SSS=_SSS;
   SSS_Sopr=_SSS_Sopr;
   memcpy(&init,&_init, sizeof(initialization));
}


//---------------------------------------------------------------------------
void __fastcall TInAndLogDataThread::Execute()
{
  int DebugKL=12;
  int Ret;

  DWORD StatusWait;
  HANDLE handles[2]={hPriem, hStop};
  int i,k,j;
  TSoprTrassaList *STL=NULL;
  if(init.IsSoprovojdenie)
  {
    STL=new TSoprTrassaList(init);
  }
//��������� � ����������� ����
  while(1)
  {
//���� 5 � ������ ������ �������
    StatusWait=WaitForMultipleObjects(2,handles,false,5000);
    if(Terminated)break;
    switch(StatusWait)
    {
       case WAIT_OBJECT_0:
//���� �������
           i=TPaketData::OutIndex;
           k=TPaketData::InIndex;
           for(;i!=k;i=(i+1)%N_OF_FIFO_PAK_STR)
           {
//���������� �������
              if(STL)
              {
                 if(TPaketData::PaketData[i].TipPaketa==TipPakSever)
                    TPaketData::PaketData[i].Sever.Soprovojdenie=1;   
//���� ����� ��������, �� ������� ���� ����������
//1 - �������� �� ���������
                 if(TPaketData::PaketData[i].TipPaketa==TipPakOtmetka)
                 {
   TPaketData::PaketData[i].Otmetka.lNomerTrassy=0;
   TPaketData::PaketData[i].Otmetka.iTail=0;
                 }
                 Ret=STL->AddOtschet(TPaketData::PaketData[i]);
                 if(Ret!=1)
                 {
//������ �� ��� �������� �� � ����� ������. ���������� �������� ����� ������
                    j=i-1;
                    if(j<0)j=N_OF_FIFO_PAK_STR-1;
                    while(j!=k)
                    {
                       Ret=STL->ZavyazatTrassu((TPaketData::PaketData[j]),
                                               (TPaketData::PaketData[i]));
                       if(Ret!=0)break;
                       j--;
                       if(j<0)j=N_OF_FIFO_PAK_STR-1;
                    }
                 }
              }
              try
              {
                MCO->AddTarget(TPaketData::PaketData[i]);
              }
              catch( EIkoException &EIE)
              {
                 Exc=(void*)(&EIE);
                 TypeOfExc=1;
                 Synchronize(SynhroExitForEcxeption);
              }
              catch(Exception &exception)
              {
                 Exc=(void*)(&exception);
                 TypeOfExc=2;
                 Synchronize(SynhroExitForEcxeption);
              }
              catch(...)
              {
                 TypeOfExc=0;
                 Synchronize(SynhroExitForEcxeption);
              }
//���� �� ���� �������� (�������������) �� ����� �������� ����� ������
              if(!STL&&TPaketData::PaketData[i].TipPaketa==TipPakOtmetka)
              {   

                TPaketData::PaketData[i].Otmetka.lNomerTrassy=0; 
              }
              SSS->SaveDataToFile(TPaketData::PaketData[i]);

           }
//�������������� ������
           TPaketData::OutIndex=k;

       break;

       case WAIT_OBJECT_0+1:
//������� ���� ���������
          break;
       break;

       default:
          MCO->NoPackets=1;  //������ ��� �������
    };

  };
  if(STL)delete STL;

//�������
  CloseHandle(hStop);
}
//---------------------------------------------------------------------------
//��������� ������ ������
int __fastcall TInAndLogDataThread::Stop()
{
  SetEvent(hStop);   //���������� ������
  Terminate();  //�������� �������
  return 1;
}



//������� �������������. ���������� ��� ����������
void __fastcall TInAndLogDataThread::SynhroExitForEcxeption(void)
{	
  //���������� ���������� ������ ��������� � ����� � ����������� �����������
   Form1->TreatExceptionInSecondThread(Exc,TypeOfExc);
//��� ��� �����������
}	