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
  HANDLE hPriem;  //Есть данные
  HANDLE hStop;   //Событие того, что остановлена прогармма
  TMainCircleOut *MCO;
  TSoprSaveSession *SSS;
  TSoprSaveSession *SSS_Sopr;
  initialization init;
  
//Для передачи процедуре  SynhroExitForEcxeption
  void *Exc;
  int TypeOfExc;

protected:
        void __fastcall Execute();
        void __fastcall SynhroExitForEcxeption(void);

public:
        __fastcall TInAndLogDataThread(
           HANDLE _hEvent,           //Событие того что считано данное
           TMainCircleOut *_MCO,     //Для ввода данных
           TSoprSaveSession *_SSS,   //Сохранение в файл данных
           TSoprSaveSession *_SSS_Sopr, //Сохранение сопровождаемых данных
           initialization &_init        //структура инициализации
                                   );

//Остановка работы потока
        int __fastcall Stop();
};
//---------------------------------------------------------------------------
#endif
