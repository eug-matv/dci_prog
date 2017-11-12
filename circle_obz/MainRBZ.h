/*MainRBZ.cpp, MainRBZ.h, MainRBZ.dfm - главное окно программы.
Автор: Матвеенко Е.А.
Дата написания: конец 2007 года.
Дата исправления: 04.11.2017 - добавление этого заголовка.
Вызывается при запуске программы.

*/
//---------------------------------------------------------------------------

#ifndef MainRBZH
#define MainRBZH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
//#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include "airoplanelist.h"
#include "comreadthread.h"
#include "soprforworkfile.h"
#include "main_circle.h"
#include "in_and_log_thread.h"
#include <StdCtrls.hpp>
#include "MessageFrame.h"
//---------------------------------------------------------------------------
class TRZPDraw;

class TForm1 : public TForm
{
__published:	// IDE-managed Components
       TStatusBar *StatusBar1;
        TPanel *PanelUpravleniya;
        TTimer *PaintTimer;
        TMainMenu *MainMenu1;
        TMenuItem *N2;
        TMenuItem *N4;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *N8;
        TToolBar *ToolBar1;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TToolButton *ToolButton8;
        TToolButton *ToolButton7;
        TToolButton *ToolButton9;
        TImageList *ImageList1;
        TImageList *ImageList2;
        TMenuItem *N9;
        TMenuItem *N10;
        TMenuItem *N11;
        TMenuItem *N12;
        TMenuItem *N13;
        TMenuItem *N14;
        TTimer *Timer1;
        TMenuItem *N15;
        TPanel *Panel1;
        TScrollBar *ScrollBar1;
        TPanel *Panel2;
        TScrollBar *ScrollBar2;
        TToolButton *ToolButton10;
        TMenuItem *N18;
        TMenuItem *N19;
        TMenuItem *N20;
        TTimer *Timer2;
        TToolButton *ToolButton11;
        TMenuItem *N1;
        TFMessage *FMessage1;
        TTimer *TestKodBedstviaTimer;
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
    void __fastcall PaintTimerTimer(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall N6Click(TObject *Sender);
        void __fastcall N7Click(TObject *Sender);
        void __fastcall SpeedButton10Click(TObject *Sender);
        void __fastcall SpeedButton13Click(TObject *Sender);
        void __fastcall SpeedButton11Click(TObject *Sender);
        void __fastcall SpeedButton12Click(TObject *Sender);
        void __fastcall ToolButton7Click(TObject *Sender);
        void __fastcall ToolButton9Click(TObject *Sender);
        void __fastcall ToolButton2Click(TObject *Sender);
        void __fastcall ToolButton2MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton2MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton9MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton9MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton7MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton7MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton8MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton6MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton4Click(TObject *Sender);
        void __fastcall ToolButton4MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton4MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton1Click(TObject *Sender);
        void __fastcall ToolButton1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall ToolButton3Click(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall ToolButton5MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton5MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ScrollBar2Scroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
        void __fastcall N15Click(TObject *Sender);
        void __fastcall ToolButton5Click(TObject *Sender);
        void __fastcall ToolButton8Click(TObject *Sender);
        void __fastcall ToolButton8MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton6MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton10MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall ScrollBar1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall ToolButton6Click(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall ToolButton11MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton11MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton3MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton3MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ToolButton11Click(TObject *Sender);
        void __fastcall ToolButton10Click(TObject *Sender);
        void __fastcall N5Click(TObject *Sender);
        void __fastcall FMessage1Button1Click(TObject *Sender);
private:	// User declarations

        char DirectDir[500];
        char PreSetupIni[1024],  //Файл неизменяемых установок
               UserIni[1024],      //Файл изменяемых установок
            LogFile[1024],         //Лог-файл действий
            LogFileErr[1024];      //Логфайл ошибок

        bool IsStart;

/*Матвеенко 21.11.2007*/
        TMainCircleOut MCO;          //Отображение данных на экране
        TComReadThread *CRT;         //Считывание данных из КОМ порта
        initialization init;         //Данные инициализации (см.structs.h)
        TSoprSaveSession SSS;        //Данные для сохранения данных в файл не фильтрованных
        TSoprSaveSession SSS_Filter; //Данные для фильтра - пока не работает
        TInAndLogDataThread *IALDT;  //Вторичный поток ввода данных
        int iLastStateMouse;         //0 - не нажате кнопка
                                     //1 - нажата левая кнопка
                                     //2 - нажата правая кнопка



        bool isErrorControl;    //Ошибка контроля доступа



//Еще надо добавить фильтры




/*Вспомогательные процедуры*/
   void __fastcall StartWork();  //Запуск работы
   void __fastcall ModifyScrollBars();

   int __fastcall LoadControlLibrary();

//Освобождение библиотек
   void __fastcall ReleaseLibrary();

//Установить размеры FMessage1
   void __fastcall SetFMessage1Size(void);


   HINSTANCE HM;               //Дескриптер файла
   HANDLE __stdcall (*InitEvent)(char* IniName);
   void   __stdcall (*CloseEvent)(void);
   BOOL   __stdcall (*EventSet)(void);
   HANDLE HandleR;             //

   int WasOutLogError;         //Проверка того, был ли исключение в файд

   TStringList *slComPortsList; //Список портов
   TStringList *slStatesList;   //Список состояний портов

public:		// User declarations
        __fastcall TForm1(TComponent* Owner);

//Закрытые всех дынных в программе
   void __fastcall StopWork();  //Запуск работы


// Функция перемещения РЛС
   void __fastcall MoveRLS(double x,double y);



//Добавлена Матвеенко Е.А. 18.02.2008
//Оберточначная функция для вызова из вторичного потока 
//Процедура вызываемая в модуле in_and_log_trhead в Execute при возникновении критического
//исключения.  Сам вызов происходит непосредственно в методе SynhroExitForException
void __fastcall TreatExceptionInSecondThread(
                                         void *_Exception,  //Указатель на исключение
                                         int TypeOfExcept  //Тип исключения 1: EIkoException
              						   //2: Ecxeption
                                         );                 //Не было исключения 




};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
