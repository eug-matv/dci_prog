//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include "scan_comport.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)




int scanComPorts(TStringList *comPortsList, TStringList *statesList)
{
        HANDLE hCom;
        AnsiString com_port,com_port1;
        int iGetLastError;
        comPortsList->Clear();
        statesList->Clear();
        for(int i=1;i<1000;i++)
        {
            com_port=String("COM")+IntToStr(i);
            com_port1=String("\\\\.\\")+com_port;
            hCom = CreateFile(com_port1.c_str(),GENERIC_READ| GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL ); // открыли порт СОМ для чтения и записи
            if (hCom == INVALID_HANDLE_VALUE)
            {// проверяем открылся или нет порт СОМ
                   iGetLastError = GetLastError();
                   if(iGetLastError==ERROR_ACCESS_DENIED)
                   {
                        comPortsList->Add(com_port);
                        statesList->Add("Buzy");
                   }
            }else{
                   comPortsList->Add(com_port);
                   statesList->Add("Free");
                   CloseHandle(hCom);
            }
        }
        return comPortsList->Count;
}