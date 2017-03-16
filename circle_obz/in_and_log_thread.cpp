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
           HANDLE hEvent, //Событие того что считано данное
           TMainCircleOut *_MCO,  //Для ввода данных
           TSoprSaveSession *_SSS,   //Сохранение в файл данных
           TSoprSaveSession *_SSS_Sopr, //Сохранение сопровождаемых данных
           initialization &_init        //структура инициализации
            ): TThread(false)
{
   FreeOnTerminate=true;   //Автоматически убирать проект
   MCO=_MCO;
   hPriem=hEvent;          //Передадим
   hStop=CreateEvent(NULL,true,false,NULL); //Создали
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
//Переходим в бесконечный цикл
  while(1)
  {
//Ждем 5 с какого нибудь события
    StatusWait=WaitForMultipleObjects(2,handles,false,5000);
    if(Terminated)break;
    switch(StatusWait)
    {
       case WAIT_OBJECT_0:
//Есть отсчеты
           i=TPaketData::OutIndex;
           k=TPaketData::InIndex;
           for(;i!=k;i=(i+1)%N_OF_FIFO_PAK_STR)
           {
//Обработать отметку
              if(STL)
              {
                 if(TPaketData::PaketData[i].TipPaketa==TipPakSever)
                    TPaketData::PaketData[i].Sever.Soprovojdenie=1;   
//Если режим привязки, то отметку надо обработать
//1 - пытаемся ее привязать
                 if(TPaketData::PaketData[i].TipPaketa==TipPakOtmetka)
                 {
   TPaketData::PaketData[i].Otmetka.lNomerTrassy=0;
   TPaketData::PaketData[i].Otmetka.iTail=0;
                 }
                 Ret=STL->AddOtschet(TPaketData::PaketData[i]);
                 if(Ret!=1)
                 {
//Отсчет не был привязан ни к какой трассе. Попытаемся завязать новую трассу
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
//Если не было привязки (сопровождения) то лучше обнулить номер трассы
              if(!STL&&TPaketData::PaketData[i].TipPaketa==TipPakOtmetka)
              {   

                TPaketData::PaketData[i].Otmetka.lNomerTrassy=0; 
              }
              SSS->SaveDataToFile(TPaketData::PaketData[i]);

           }
//Модифицировать данные
           TPaketData::OutIndex=k;

       break;

       case WAIT_OBJECT_0+1:
//Команда стоп поступила
          break;
       break;

       default:
          MCO->NoPackets=1;  //Ошибка нет пакетов
    };

  };
  if(STL)delete STL;

//Закроем
  CloseHandle(hStop);
}
//---------------------------------------------------------------------------
//Остановка работы потока
int __fastcall TInAndLogDataThread::Stop()
{
  SetEvent(hStop);   //Завершение работы
  Terminate();  //Завершим процесс
  return 1;
}



//Функция синхронизации. Вызывается для завершения
void __fastcall TInAndLogDataThread::SynhroExitForEcxeption(void)
{	
  //Экстренное завершение работы программы в связи с критическим исключением
   Form1->TreatExceptionInSecondThread(Exc,TypeOfExc);
//Тут она завершается
}	