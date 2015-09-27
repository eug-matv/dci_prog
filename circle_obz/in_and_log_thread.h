//---------------------------------------------------------------------------

#ifndef in_and_log_threadH
#define in_and_log_threadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "main_circle.h"
#include "soprforworkfile.h"
#include "structs.h"
//---------------------------------------------------------------------------
class TInAndLogDataThread : public TThread
{
private:
  HANDLE hPriem;  //���� ������
  HANDLE hStop;   //������� ����, ��� ����������� ���������
  TMainCircleOut *MCO;
  TSoprSaveSession *SSS;
  TSoprSaveSession *SSS_Sopr;
  initialization init;
  
//��� �������� ���������  SynhroExitForEcxeption
  void *Exc;
  int TypeOfExc;

protected:
        void __fastcall Execute();
        void __fastcall SynhroExitForEcxeption(void);

public:
        __fastcall TInAndLogDataThread(
           HANDLE _hEvent,           //������� ���� ��� ������� ������
           TMainCircleOut *_MCO,     //��� ����� ������
           TSoprSaveSession *_SSS,   //���������� � ���� ������
           TSoprSaveSession *_SSS_Sopr, //���������� �������������� ������
           initialization &_init        //��������� �������������
                                   );

//��������� ������ ������
        int __fastcall Stop();
};
//---------------------------------------------------------------------------
#endif
