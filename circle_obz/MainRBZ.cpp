//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <math.h>

#include "lang_str.h"
#include "MainRBZ.h"
#include "DrawObjectList.h"
//#include "Crash_alarm.h"
#include "options.h"
#include "AnyTools.h"
#include "structs.h"
#include "paketstruct.h"
#include "airoplanelist.h"
#include "comreadthread.h"
#include "soprforworkfile.h"

#include "main_circle.h"
#include "WorkWithIniFiles.h"
#include "win_str_dir_utils.h"
#include "About.h"
#include "Navigation.h"
#include "LogFile.h"
#include "kio_exeptions.h"
//for Microsoft Help
#include <htmlhelp.h>
#include "scan_comport.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FrameTime"
#pragma link "MessageFrame"
#pragma resource "*.dfm"
#define ABS(X) (((X) > 0) ? (X) : (-(X)))
#define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))


#define MY_CATCH()   catch(EIkoException &EIE)     \
  {                                                 \
        Log_IKO=LOGFILE_IKO::GetStaticLog();        \
         if(Log_IKO)                                \
    Log_IKO->OutCriticalError(EIE.TextIskl,         \
    EIE.ModuleName, EIE.ClassName, EIE.MethodName, EIE.RegExName);   \
             ReleaseLibrary();                     \
             StopWork();                           \
             ExitProcess(1);                       \
                                                   \
  }                             		   \
  catch (Exception &exception)                     \
  {                                               \
   Log_IKO=LOGFILE_IKO::GetStaticLog();           \
   TempString=exception.ClassName();              \
   Log_IKO->OutCriticalError(TempString.c_str(),  \
            exception.Message.c_str());           \
            ReleaseLibrary();                     \
            StopWork();                           \
            ExitProcess(1);                       \
  }                                               \
  catch (...)                                     \
  {                                               \
   Log_IKO=LOGFILE_IKO::GetStaticLog();           \
   Log_IKO->OutCriticalError(\
LSTR("Unknown exception", "�� ��������� ����������"),  \
LSTR("Unknown exception","�� ��������� ����������"));           \
            StopWork();                           \
            ReleaseLibrary();                     \
            ExitProcess(1);                       \
  }



TForm1 *Form1;


HANDLE __stdcall InitEventDummy(char* IniName){return 0;}
void __stdcall CloseEventDummy(void){}
BOOL __stdcall  EventSetResetDummy(void){return 0;}

static LOGFILE_IKO *Log_IKO;
static AnsiString TempString;

HANDLE GlobalCntrlEvnt;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner),CRT(NULL), IALDT(NULL)
{
/*�������� ���� ������ �� �����*/

//��������� ������ � ���� ������������� ������
//������� ��� ��������, ��� ����� ���������
  int Ret;




  if(!GetFullPathFileWithProgram("IKOUser.ini",UserIni,511))
  {
     MessageBox(NULL,
LSTR("Search error file IKOUser.ini. Maybe the path is too long.\n Check and run the program again",
"������ ������ IKOUser.ini. �������� ���� ������� �������.\n��������� � ��������� ������!"),
LSTR("Error: TForm1::TForm1","������ TForm1::TForm1"), MB_OK);
  //������
     Sleep(10000);
     ExitProcess(1);
  }

  if(!GetFullPathFileWithProgram("IKOPreSetup.ini",PreSetupIni,511))
  {
     MessageBox(NULL,
LSTR("Search error file IKOPreSetup.ini. Maybe the path is too long.\n Check and run the program again",
"������ ������ IKOPreSetup.ini. �������� ���� ������� �������.\n��������� � ��������� ������!"),
LSTR("Error: TForm1::TForm1","������ TForm1::TForm1"), MB_OK);
  //������
     Sleep(10000);
     ExitProcess(1);
  }

  if(!FileExists(PreSetupIni))
  {
     MessageBox(NULL,
LSTR("Search error file IKOPreSetup.ini, File not found.\nCheck and run the program again",
"������ ������ IKOPreSetup.ini - ���� �� ������.\n��������� � ��������� ������"),
LSTR("Error: TForm1::TForm1", "������ TForm1::TForm1"), MB_OK);
     Sleep(10000);
     ExitProcess(1);
  }
  strcpy(LogFile,"IKO.log");

  Load_INI_f(&init,PreSetupIni, UserIni);  //�������� Ini-�����

  int formulyarGetSize(initialization &init);
  formulyarGetSize(init);

  WasOutLogError=0;
//�������� ���������
  LOGFILE_IKO::Init_StaticLog(LogFile,init);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
  SIZE szArea;
  ScrollBar1->Left=0;
  Panel1->Top=ClientHeight-Panel1->Height-
                  StatusBar1->Height;
  Panel1->Width=ClientWidth-Panel2->Width-
                    PanelUpravleniya->Width+20;

  Panel2->Left=Panel1->Width;
  Panel2->Top=0;
  Panel2->Height=Panel1->Top;
  FMessage1->Left=ClientWidth-PanelUpravleniya->Width-
                  FMessage1->Width;
  FMessage1->Top=0;


  if(MCO.GetWorking())
  {
     szArea.cx=Panel2->Left;
     szArea.cy=Panel1->Top;
     try
     {
       MCO.SetNewSize(szArea);
       ModifyScrollBars();
       MCO.Paint(Handle,0,0);
     }
     MY_CATCH()
  }
}
//---------------------------------------------------------------------------




 //---���������
void __fastcall TForm1::MoveRLS(double x,double y)
{
  try
  {
    if(MCO.GetWorking())
    {
      MCO.MoveRLS(x,y);
      ModifyScrollBars();
      MCO.Paint(Handle,0,0);
    }
  }
  MY_CATCH()
}



/*�������������� ���������*/
void __fastcall TForm1::StartWork()   //������ ������
{

}




void __fastcall TForm1::StopWork()  //��������� ������ ����������
{

//������� ��������� ����� ������� ��������� ������ ��
   if(IALDT)
   {
      IALDT->Stop();
      IALDT=NULL;
   }



   if(CRT)
   {
      CRT->Disconnect();
      CRT=NULL;
   }
   SSS.Stop();
   MCO.Stop();
   LOGFILE_IKO::Init_StaticFree();
   CloseHandle(GlobalCntrlEvnt);

}



int __fastcall TForm1::LoadControlLibrary()
{
  AnsiString Strng;
  if(init.SecurityEventControl)
  {
     HM=LoadLibrary(init.NameDllFileEvent);
     if(!HM)
     {
#ifdef ENG_LANG
        Strng=String("Error loading module ")+String(init.NameDllFileEvent);
//        MessageBox(NULL,Strng.c_str(),"������ �������� ����������",MB_OK);
          FMessage1->ShowText("Error control!",Strng);
#else
        Strng=String("������ �������� ")+String(init.NameDllFileEvent);
//        MessageBox(NULL,Strng.c_str(),"������ �������� ����������",MB_OK);
          FMessage1->ShowText("������ ��������!",Strng);
#endif
          SetFMessage1Size();
        goto End_1;
     }
     (void*)InitEvent=GetProcAddress(HM,"InitEvent");
     if(!InitEvent)
     {
        FreeLibrary(HM);
        HM=NULL;
#ifdef ENG_LANG
        Strng=String("in the module ")+String(init.NameDllFileEvent);
        FMessage1->ShowText("Error control! Not found the InitEvent procedure",
                  Strng);
#else
        Strng=String("� ������ ")+String(init.NameDllFileEvent);
        FMessage1->ShowText("������ ��������. �� ������� ��������� InitEvent",
                  Strng);
#endif
          SetFMessage1Size();
        goto End_1;
     }

     (void*)CloseEvent=GetProcAddress(HM,"CloseEvent");
     if(!CloseEvent)
     {
        FreeLibrary(HM);
        HM=NULL;
#ifdef ENG_LANG
        Strng=String("in the module ")+String(init.NameDllFileEvent);
        FMessage1->ShowText("Error control! Not found the CloseEvent procedure",
                    Strng);
#else
        Strng=String("� ������ ")+String(init.NameDllFileEvent);
        FMessage1->ShowText("������ ��������. �� ������� ��������� CloseEvent",
                    Strng);
#endif
          SetFMessage1Size();
        goto End_1;
     }

     (void*)EventSet=GetProcAddress(HM,"EventSet");
     if(!EventSet)
     {
        FreeLibrary(HM);
        HM=NULL;
#ifdef ENG_LANG
        Strng=String("in the module ")+String(init.NameDllFileEvent);
        FMessage1->ShowText("Error control! Not found the EventSet procedure",
                    Strng);
#else
        Strng=String("� ������ ")+String(init.NameDllFileEvent);
        FMessage1->ShowText("������ ��������. �� ������� ��������� EventSet",
                    Strng);
#endif
          SetFMessage1Size();
        goto End_1;
     }
   if(InitEvent&&CloseEvent&&EventSet)  return 1;
  }
End_1:
  InitEvent = InitEventDummy;
  CloseEvent = CloseEventDummy;
  EventSet = EventSetResetDummy;

  HM=NULL;


  return 0;

}


void __fastcall TForm1::ReleaseLibrary()
{
  if(HM)FreeLibrary(HM);
  HM=NULL;
}



void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
  Save_INI_f(&init,UserIni);
  StopWork();
}
//---------------------------------------------------------------------------
//��������� ������� ������ ����
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   int K=0;  //����� ������ ������
//������� ����
   if(MCO.GetWorking())
   {
      if(Button==mbLeft)
      {
         K=1;
      }
      if(Button==mbRight)
      {
         K+=2;
      }
      if(K)
      {

        try
        {
           MCO.MouseButtonDown(X,Y,K);
           MCO.Paint(Handle,0,0);
        }
        MY_CATCH()

      }

   }
}
//---------------------------------------------------------------------------

//��������� ����������� �������
void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
   int K=0;
//������ ������
   if(MCO.GetWorking())
   {
     if(Shift.Contains(ssLeft))K=1;
     if(Shift.Contains(ssRight))K+=2;
     try
     {
       MCO.MouseButtonDowned(X,Y,K);
       if(K)MCO.Paint(Handle,0,0);
     }
     MY_CATCH()
     iLastStateMouse=K;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  int K=0;  //����� ������ ������
  if(MCO.GetWorking())
  {
     if(Button==mbLeft)
     {
         K=1;
     }
     if(Button==mbRight)
     {
          K+=2;
     }
     try
     {
       if(K)
       {
          MCO.MouseButtonUp(X,Y,K);
          ModifyScrollBars();
       }
       if(K)MCO.Paint(Handle,0,0);
     }
     MY_CATCH()
     iLastStateMouse=K;
  }
}
//---------------------------------------------------------------------------




void __fastcall TForm1::FormShow(TObject *Sender)
{
  int Ret;
  SIZE szArea;
  int _Left,_Top,_Right,_Bottom;
  POINT pntRLS;
  int i,tmp_i;
  if(isErrorControl)return;

  Log_IKO=LOGFILE_IKO::GetStaticLog();

//������ ������ ������������ ����������� ������


  ScrollBar1->Left=0;
  Panel1->Top=ClientHeight-Panel1->Height-
                  StatusBar1->Height;
  Panel1->Width=ClientWidth-Panel2->Width-
                    PanelUpravleniya->Width+20;

  Panel2->Left=Panel1->Width;
  Panel2->Top=0;
  Panel2->Height=Panel1->Top;

  szArea.cx=Panel2->Left;
  szArea.cy=Panel1->Top;
//��������� ���������� � ��������� � ������������ �� ����������
  if(init.IsOutFormulyar)
  {
    N18->Checked = true;
    ToolButton2->Hint = LSTR("Hide Label","������ ��������");
    ToolButton2->Tag = 0;
    ToolButton2->Down = true;
    ToolButton11->Enabled = true;
     N1->Enabled = true;
  }else{
    N18->Checked = false;
    ToolButton2->Hint = LSTR("Show Label", "���������� ��������") ;
    ToolButton2->Tag = 1;
    ToolButton2->Down = false;
    ToolButton11->Enabled = false;
     N1->Enabled = false;
  }

  if(init.iRamkaFormulyar)
  {
   N1->Checked = true;
   ToolButton11->Hint = LSTR("Hide Label frame","������ ����� ���������");
   ToolButton11->Tag = 0;
   ToolButton11->Down = true;
  }else{ N1->Checked = false;
   ToolButton11->Hint = LSTR("Show Label frame","���������� ����� ���������");
   ToolButton11->Tag = 1;
   ToolButton11->Down = false;
  }




  if(init.iTail)
  {
     N19->Checked = true;
     ToolButton6->Hint = LSTR("Don't show the track","�� �������� ����") ;
     ToolButton6->Tag = 0;
     ToolButton6->Down = true;

  }else{
     N19->Checked = false;
     ToolButton6->Hint = LSTR("Show the track","�������� ����") ;
     ToolButton6->Tag = 1;
     ToolButton6->Down = false;
  }

   if (init.IsSoprovojdenie)
   {
     N20->Checked = true;
     ToolButton10->Hint = LSTR("Disable tracking","��������� �������������");
     ToolButton10->Tag = 0;
     ToolButton10->Down = true;
   }else{
     N20->Checked = false;
//     ToolButton10->Hint = "�������� �������������" ;  //��������������� 2014.12.15
     ToolButton10->Hint = 
LSTR("The opportunity tracking is disabled",
"����������� ������������� ����������� ������� ���������") ;
     ToolButton10->Tag = 1;
     ToolButton10->Down = false;
     ToolButton10->Enabled=false;
   }




  try
  {
    MCO.Start(Handle,szArea,init);
    ModifyScrollBars();
    strcpy(SSS.csPutKDannym, init.csFullPathToData);
    strcpy(SSS_Filter.csPutKDannym, init.csFullPathToData);
    strcpy(SSS.csPrefix,"plotout");
    strcpy(SSS_Filter.csPrefix,"filtout");
    strcpy(SSS.csExt,"txt");
    strcpy(SSS_Filter.csExt, "txt");
    SSS.bDataOutDOS=true;
    SSS.bVyvodAmpl=false;
    SSS.bSkorostOut=false;
    SSS.bDopDopInfo=false;
    SSS.bSaveDataSever=true;
    SSS.bSaveDataOtmetka=init.IsSaveDataToFile;
    SSS.bSaveDataSektor=init.IsSaveSectorsToFile;

//������� ���������
    SSS_Filter.bDataOutDOS=true;
    SSS_Filter.bVyvodAmpl=false;
    SSS_Filter.bSkorostOut=true;
    SSS_Filter.bDopDopInfo=true;
    SSS_Filter.bSaveDataSever=true;
    SSS_Filter.bSaveDataOtmetka=false;  //���� �� �����������
    SSS_Filter.bSaveDataSektor=false;

    Ret=SSS.Start();
    if(Ret==-1)
    {
#ifdef ENG_LANG      
	  AnsiString Put=String("Not found directory ")+SSS.csPutKDannym+String(" to write data");
#else
      AnsiString Put=String("�� ������ ������� ")+SSS.csPutKDannym+String(" ��� ������ ������");
#endif
      FMessage1->ShowText(Put);
      SetFMessage1Size();
    }

//�������� ����� ����������� ������ �� ��� �����
    CRT=TComReadThread::Connect(init);
    if(!CRT)
    {
//�������� ������������� �����
        tmp_i=-1;
        for(i=0;i<slComPortsList->Count;i++)
        {
                if(slComPortsList->Strings[i].Trim().LowerCase()==
                    String(init.csComPortStr).Trim().LowerCase())
                {
                        tmp_i=i;
                        break;
                }
        }
        if(tmp_i>=0)
        {
#ifdef ENG_LANG
                AnsiString Put=String("Error opening port ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
                        "Change port settings or run the program ComDisable to disable the port");
#else
                AnsiString Put=String("������ �������� ����� ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
                        "�������� ��������� COM-����� ��� ��������� COMDisable ��� ���������� �����!");
#endif
                 SetFMessage1Size();
                MCO.NoPackets=3;
                Log_IKO=LOGFILE_IKO::GetStaticLog();
                if(Log_IKO)Log_IKO->OutNotCriticalError(Put.c_str());
                WasOutLogError=1;
                FMessage1->BStartComdisable->Show();
        }else{
#ifdef ENG_LANG
                AnsiString Put=String("Error opening port ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
                        "The port is not detected. Try opening another port");
#else
				AnsiString Put=String("������ �������� ����� ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
                        "���� �� ���������, ���������� ������� ������ ����!");				
#endif               
				  SetFMessage1Size();
                MCO.NoPackets=2;
                Log_IKO=LOGFILE_IKO::GetStaticLog();
                if(Log_IKO)Log_IKO->OutNotCriticalError(Put.c_str());
                WasOutLogError=1;

        }
   }else{
      WasOutLogError=0;
      MCO.NoPackets=0;
   }
  }
  MY_CATCH()

//�������� ��������� ����� �������� ������
   if(CRT)
   {
       IALDT=new TInAndLogDataThread(CRT->hEventNoEmpty,&MCO,&SSS,&SSS_Filter,init);
   }else{
       IALDT=NULL;
   }
   PaintTimer->Interval=(int)(init.fTempObzora*1000)/12/10*10;
   Timer1Timer(NULL);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::PaintTimerTimer(TObject *Sender)
{
  AnsiString Strka;
  int Ret;
  AnsiString Put;

  try
  {
    if(MCO.GetWorking())
    {
//�������� ��� ��������

          Ret=MCO.TestPeriod(); //���� ��� ���������, ������ ������� 0
          MCO.Paint(Handle,0,0);
          if(Ret==2)
          {
#ifdef ENG_LANG
                AnsiString Put=String("Error opening port ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
                        "The port is not detected. Try opening another port");
#else
                AnsiString Put=String("������ �������� ����� ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
                        "���� �� ���������, ���������� ������� ������ ����!");
#endif

                SetFMessage1Size();

                if(!WasOutLogError)
                {
                   Log_IKO=LOGFILE_IKO::GetStaticLog();
                   if(Log_IKO)Log_IKO->OutNotCriticalError(Put.c_str());
                   WasOutLogError=1;
                }

          }else if(Ret==3)
          {
#ifdef ENG_LANG
                AnsiString Put=String("Error opening port  ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
                        "Change port settings or run the program ComDisable to disable the port");
                 SetFMessage1Size();
#else
                AnsiString Put=String("������ �������� �����  ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
                        "�������� ��������� COM-����� ��� ��������� COMDisable ��� ���������� �����!");
                 SetFMessage1Size();

#endif

                 if(!WasOutLogError)
                {
                   Log_IKO=LOGFILE_IKO::GetStaticLog();
                   if(Log_IKO)Log_IKO->OutNotCriticalError(Put.c_str());
                   WasOutLogError=1;
                }
                FMessage1->BStartComdisable->Show();
          }else  if(Ret==1)
          {
//             AnsiString Put=String("������ �������� ����� ")+String(init.csComPortStr);
             if(FMessage1->Visible)FMessage1->Hide();
             Put=String(
LSTR("No packets from port ", "��� ������� � ����� "))+
	String(init.csComPortStr);
             if(init.abuse_if_none)
             {
               FMessage1->ShowText(Put);
               SetFMessage1Size();
             }
             if(!WasOutLogError)
             {
                Log_IKO=LOGFILE_IKO::GetStaticLog();
                if(Log_IKO)Log_IKO->OutNotCriticalError(Put.c_str());
                WasOutLogError=1;
             }
          }else{
             if(FMessage1->Visible)FMessage1->Hide();
             if(WasOutLogError!=0)
             {
               if(Log_IKO)Log_IKO->OutWorkInfo(LSTR("Packets arrive","������ ���������"));
               WasOutLogError=0;
             }
          }
    }
  }
  MY_CATCH()

}
//---------------------------------------------------------------------------


void __fastcall TForm1::N7Click(TObject *Sender)
{

   Close();
}
//---------------------------------------------------------------------------




void __fastcall TForm1::N6Click(TObject *Sender)
{
  AboutBox->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton10Click(TObject *Sender)
{

  if(MCO.GetWorking())
  {
    try
    {
      MCO.MoveRLS(0,0.4);
      ModifyScrollBars();
      MCO.Paint(Handle,0,0);
    }
    MY_CATCH()
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton13Click(TObject *Sender)
{
  if(MCO.GetWorking())
  {
    try
    {
      MCO.MoveRLS(0,-0.4);
      ModifyScrollBars();
      MCO.Paint(Handle,0,0);
    }
    MY_CATCH()
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton11Click(TObject *Sender)
{
  if(MCO.GetWorking())
  {
    try
    {
      MCO.MoveRLS(0.4,0);
      ModifyScrollBars();
      MCO.Paint(Handle,0,0);
    }
    MY_CATCH()
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton12Click(TObject *Sender)
{
  if(MCO.GetWorking())
  {
    try
    {
      MCO.MoveRLS(-0.4,0);
      ModifyScrollBars();
      MCO.Paint(Handle,0,0);
    }
    MY_CATCH()
  }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ToolButton7Click(TObject *Sender)
{
   try
   {
     if(MCO.GetWorking())
     {
       MCO.MultyScale(1.5);
       ModifyScrollBars();
       MCO.Paint(Handle,0,0);
     }
   }
   MY_CATCH()
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton9Click(TObject *Sender)
{
  try
  {
     if(MCO.GetWorking())
     {
        MCO.MultyScale(1.0/1.5);
        ModifyScrollBars();
        MCO.Paint(Handle,0,0);
     }
   }
   MY_CATCH()
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ToolButton2Click(TObject *Sender)
{
   if (ToolButton2->Tag == 0)
   {N18->Checked = false;
    ToolButton2->Hint = LSTR("Show Label", "���������� ��������")  ;
    ToolButton2->Tag = 1;
    ToolButton2->Down = false;
    ToolButton11->Enabled = false;
    N1->Enabled = false;
//������ ��������
    init.IsOutFormulyar=0;
   }
else
   {N18->Checked = true;
    ToolButton2->Hint = LSTR("Hide Label", "������ ��������");
    ToolButton2->Tag = 0;
    ToolButton2->Down = true;
    init.IsOutFormulyar=1;
    ToolButton11->Enabled = true;
    N1->Enabled = true;
    }
    try
    {
      MCO.SetNewOption(init);
      Save_INI_f_VisibleFormulyar(&init,UserIni);
      MCO.Paint(Handle,0,0);
    }
    MY_CATCH()

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
       StatusBar1->SimpleText = LSTR( "Show or hide Label","  �������� ��� ������ ��������") ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton2MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
     StatusBar1->SimpleText = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton9MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   StatusBar1->SimpleText = "";
   ToolButton9->ImageIndex = 5;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton9MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    StatusBar1->SimpleText = LSTR("Scale down ", "  ��������� �������") ;
    ToolButton9->ImageIndex = 6;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton7MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   StatusBar1->SimpleText = LSTR("  Scale up ","  ��������� �������") ;
   ToolButton7->ImageIndex = 4;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton7MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
          StatusBar1->SimpleText = "";
          ToolButton7->ImageIndex = 3;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton8MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
     StatusBar1->SimpleText = "";
     ToolButton8->ImageIndex = 8;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton6MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
      StatusBar1->SimpleText = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton4Click(TObject *Sender)
{
 if (ToolButton4->Tag == 1)
   {ToolButton4->Down = true;
    Form4->Show();
    ToolButton4->Hint = LSTR("Hide the additional window Navigation",
"������ ������ ���������");
    ToolButton4->Tag = 0;}
 else
   {ToolButton4->Down = false;
    Form4->Hide();
    ToolButton4->Hint =
LSTR("Show the additional window Navigation",
"�������� ������ ���������");
    ToolButton4->Tag = 1;}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton4MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
     StatusBar1->SimpleText = 
LSTR(" Show or hide the additional window Navigation",
"  �������� ��� ������ ������ ���������");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton4MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
     StatusBar1->SimpleText = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton1Click(TObject *Sender)
{
 if (ToolButton1->Tag == 0)

   { N14->Checked = true;
    ToolButton2->Visible = true; // ��������
    ToolButton3->Visible = true; // �����
    ToolButton4->Visible = true; // ���������
    ToolButton5->Visible = true; //��������
    ToolButton6->Visible = true; //����
    ToolButton8->Visible = true; //���������
    ToolButton7->Visible = true; // �������� �������
    ToolButton9->Visible = true; // ���������
    ToolButton10->Visible = true; // �������������
    ToolButton11->Visible = true; //����� ���������
    ToolButton1->Down = true;

    ToolButton1->Hint = LSTR("Hide Panel", "������ ������") ;
    ToolButton1->Tag = 1;
   }
else
   { N14->Checked = false;
    ToolButton2->Visible = false;
    ToolButton3->Visible = false;
    ToolButton4->Visible = false;
    ToolButton5->Visible = false;
    ToolButton6->Visible = false;
    ToolButton8->Visible = false;
    ToolButton7->Visible = false;
    ToolButton9->Visible = false;
    ToolButton10->Visible = false;
    ToolButton11->Visible = false;
    ToolButton1->Down = false;
    ToolButton1->Hint = LSTR("Show Panel","�������� ������")  ;
    ToolButton1->Tag = 0;}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    StatusBar1->SimpleText = 
LSTR("  Show or hide toolbar", "  �������� ��� ������ ������ ������������") ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    StatusBar1->SimpleText = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    int Ret;
#ifndef ENG_LANG
        Form1->Caption = "��� ����";
        Form1->ToolButton1->Caption="";
	Form1->ToolButton1->Hint="������ ������";
	Form1->ToolButton2->Caption="ToolButton2";
	Form1->ToolButton2->Hint="������ ��������";
	Form1->ToolButton11->Caption="ToolButton11";
	Form1->ToolButton11->Hint="�������� ��� ������ ����� ���������";
	Form1->ToolButton3->Caption="ToolButton3";
	Form1->ToolButton3->Hint="�������� �����";
	Form1->ToolButton4->Caption="ToolButton4";
	Form1->ToolButton4->Hint="�������� ������ ���������";
	Form1->ToolButton10->Caption="ToolButton10";
	Form1->ToolButton10->Hint="�������� ��� ������ �������������";
	Form1->ToolButton5->Caption="ToolButton5";
	Form1->ToolButton5->Hint="�������� ��������� �������";
	Form1->ToolButton6->Caption="ToolButton6";
	Form1->ToolButton6->Hint="���������� ����";
	Form1->ToolButton8->Caption="ToolButton8";
	Form1->ToolButton8->Hint="�������� �������";
	Form1->ToolButton7->Caption="ToolButton7";
	Form1->ToolButton7->Hint="��������� �������";
	Form1->ToolButton9->Caption="ToolButton9";
	Form1->ToolButton9->Hint="��������� �������";
	Form1->N13->Caption="���";
	Form1->N15->Caption="����� � ����";
	Form1->N14->Caption="������ ������������";
	Form1->N19->Caption="����";
	Form1->N20->Caption="�������������";
	Form1->N18->Caption="��������";
	Form1->N1->Caption="����� ���������";
	Form1->N2->Caption="�����";
	Form1->N8->Caption="�����";
	Form1->N9->Caption="�������";
	Form1->N10->Caption="���������       Ctrl  +  \"+\"";
	Form1->N11->Caption="���������      Ctrl  +  \"-\"";
	Form1->N12->Caption="�������� �������    F5";
	Form1->N4->Caption="������";
	Form1->N5->Caption="������ �� \"��� ����\"";
	Form1->N6->Caption="� ���������";

#endif


//-----������ ��������� �� ��������� ��� ������ �� ������ ������������
    Form1->ToolButton1->Down =true;  // ������ ��������-������ ������ ������������
    Form1->ToolButton2->Down = true; //������ ��������-������ ��������
    ToolButton1->Tag = 1;
    ToolButton4->Down = false;  //������ ��������-������ ���� ��������
    ToolButton6->Down = true; //������ ��������-������ ����
    ToolButton10->Down = true; //������ ��������-������ �������������
    ToolButton4->Tag = 1;
    ToolButton11->Tag = 1;

    Form1->WindowState=wsMaximized;
  //�������� ���� ������ � ����� � ��������
    Form1->FTime1->Color =(TColor) init.Clrbackground;
    Form1->FTime1->Label1->Font->Color=(~init.Clrbackground)&0xFFFFFF;
    isErrorControl=0;

 //�������� ���������� ��������
    Ret=LoadControlLibrary();
    if(Ret)
    {
//
       HandleR=InitEvent(PreSetupIni);
       if(!HandleR)
       {
#ifdef ENG_LANG
          AnsiString Strng=String("There is no object-events ")+
            String(init.SecuriryEventName);
          FMessage1->ShowText("Error Access Control",
               Strng);
#else
          AnsiString Strng=String("��� ������ �������-������� � ������� ��� ")+
            String(init.SecuriryEventName);
          FMessage1->ShowText("������ �������� �������!",
               Strng);
#endif
          SetFMessage1Size();
          isErrorControl=1;
          return;
       }
    }

//�������� ������ ������
    slComPortsList=new TStringList;
    slStatesList=new TStringList;


    scanComPorts(slComPortsList, slStatesList); //�������� ������  � ��������� �����

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton3Click(TObject *Sender)
{
   ToolButton3->Down = true;
   int State=0;
   int i,tmp_i;
   LOGFILE_IKO *LogFile_IKO=LOGFILE_IKO::GetStaticLog();
//����� �����
  memcpy(&(Form2->init), &init,sizeof(init));
  Form2->slComPortsList=slComPortsList;
  Form2->slStatesList=slStatesList;

  Form2->ShowModal();


  if(Form2->iModified)
  {

//���� ���� ��������� - ���� ��������� �� � ��� �����
//��������, � �� ���������� �� ������ �� COM-�����
     if(strcmp(Form2->init.csComPortStr, init.csComPortStr)||
        Form2->init.iBaudRate!=init.iBaudRate ||
        Form2->init.fParity != init.fParity ||
        Form2->init.parityMode != init.parityMode ||
        Form2->init.iStopBits != init.iStopBits)State=1;


     memcpy(&init,&Form2->init,sizeof(init));
     Log_IKO=LOGFILE_IKO::GetStaticLog();
     Log_IKO->OutChangeOption(init);

     if(State&1)
     {
//���� ���������� �������������, �� ����� ���� ��� ����������
       try
       {

        if(IALDT)IALDT->Stop();
        if(CRT)CRT->Disconnect();
       }
       MY_CATCH()
       LogFile_IKO->Stop();
       CRT=TComReadThread::Connect(init);
       if(!CRT)
       {
           tmp_i=-1;
        for(i=0;i<slComPortsList->Count;i++)
        {
                if(slComPortsList->Strings[i].Trim().LowerCase()==
                    String(init.csComPortStr).Trim().LowerCase())
                {
                        tmp_i=i;
                        break;
                }
        }
        if(tmp_i>=0)
        {
#ifdef ENG_LANG
                AnsiString Put=String("Error opening port ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
                        "Change port settings or run the program ComDisable to disable the port");
#else
				AnsiString Put=String("������ �������� ����� ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
 "�������� ��������� COM-����� ��� ��������� COMDisable ��� ���������� ������ PNP ��� �����!");
#endif
                 SetFMessage1Size();
                MCO.NoPackets=3;
                Log_IKO=LOGFILE_IKO::GetStaticLog();
                if(Log_IKO)Log_IKO->OutNotCriticalError(Put.c_str());
                WasOutLogError=1;
                FMessage1->BStartComdisable->Show();
        }else{
#ifdef ENG_LANG
                AnsiString Put=String("Error opening port ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
                        "The port is not detected. Try opening another port");
#else
                AnsiString Put=String("������ �������� ����� ")+String(init.csComPortStr);
                if(FMessage1->Visible)FMessage1->Hide();
                FMessage1->ShowText(Put,
                        "���� �� ���������, ���������� ������� ������ ����!");
#endif
                 SetFMessage1Size();
                MCO.NoPackets=2;
                Log_IKO=LOGFILE_IKO::GetStaticLog();
                if(Log_IKO)Log_IKO->OutNotCriticalError(Put.c_str());
                WasOutLogError=1;

        }
       }else{
          WasOutLogError=0;
          MCO.NoPackets=0;
          FMessage1->Hide();
       }
//�������� ��������� ����� �������� ������
       MCO.Restart();
       if(CRT)
       {
          IALDT=new TInAndLogDataThread(CRT->hEventNoEmpty,&MCO,&SSS,&SSS_Filter,init);
       }else{
          IALDT=NULL;
       }
      }
      try
      {

        MCO.SetNewOption(init);
      }
      MY_CATCH()
       Save_INI_f(&init,UserIni);

//��������� ���������� � ��������� � ������������ �� ����������
     if(init.IsOutFormulyar)
     {
       N18->Checked = true;
       ToolButton2->Hint = LSTR("Hide Label","������ ��������")  ;
       ToolButton2->Tag = 0;
       ToolButton2->Down = true;
       ToolButton11->Enabled = true;
        N1->Enabled = true;
     }else{
       N18->Checked = false;
       ToolButton2->Hint = LSTR("Show Label", "���������� ��������")  ;
       ToolButton2->Tag = 1;
       ToolButton2->Down = false;
       ToolButton11->Enabled = false;
       N1->Enabled = false;
     }
     if(init.iRamkaFormulyar)
     {
       N1->Checked = true;
       ToolButton11->Hint = LSTR("Hide Label frame", "������ ����� ���������") ;
       ToolButton11->Tag = 0;
       ToolButton11->Down = true;
     }else{
       N1->Checked = false;
       ToolButton11->Hint = LSTR("Show Label frame","���������� ����� ���������") ;
       ToolButton11->Tag = 1;
       ToolButton11->Down = false;
     }

     if(init.iTail)
     {
       N19->Checked = true;
       ToolButton6->Hint = LSTR("Don't show the track", "�� �������� ����")  ;
       ToolButton6->Tag = 0;
       ToolButton6->Down = true;
     }else{
       N19->Checked = false;
       ToolButton6->Hint = LSTR("Show the track","�������� ����")  ;
       ToolButton6->Tag = 1;
       ToolButton6->Down = false;
     }
     if (init.IsSoprovojdenie)
     {
       N20->Checked = true;
       ToolButton10->Hint = LSTR("Disable tracking", "��������� �������������") ;
       ToolButton10->Tag = 0;
       ToolButton10->Down = true;
     }else{
       N20->Checked = false;
       ToolButton10->Hint = LSTR("Enable tracking", "�������� �������������");
       ToolButton10->Tag = 1;
       ToolButton10->Down = false;
     }
     FMessage1->Hide();
     try
     {
        MCO.Paint(Handle,0,0);
     }
     MY_CATCH()
  }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Timer1Timer(TObject *Sender)
{

  //����������� �������
   TDateTime DateTime = Time();  // store the current date and time
   AnsiString str = TimeToStr(DateTime)+ "   " + DateToStr(Date()); // convert the time to a string
   FTime1->Label1->Caption = str;



}
//---------------------------------------------------------------------------




void __fastcall TForm1::ToolButton5MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    StatusBar1->SimpleText = LSTR("Undo", "  �������� ��������");
     ToolButton5->ImageIndex = 10;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton5MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    StatusBar1->SimpleText = "";
     ToolButton5->ImageIndex = 9;
}
//---------------------------------------------------------------------------





void __fastcall TForm1::ScrollBar2Scroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
//
  POINT pntRLS;
  pntRLS.x=-ScrollBar1->Position;
  pntRLS.y=-ScrollBar2->Position;
  try
  {
    MCO.SetScreenScaleState(pntRLS);
//  ModifyScrollBars();
    MCO.Paint(Handle,0,0);
  }
  MY_CATCH()
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ModifyScrollBars()
{
  int _Left,_Top,_Right,_Bottom;
  POINT pntRLS;
  ScrollBar1->OnScroll = 0;
  ScrollBar2->OnScroll = 0;
  MCO.GetScreenScaleState(pntRLS,_Left,_Top,_Right,_Bottom);
  ScrollBar2->Min =-2000000000L;
  ScrollBar2->Max =2000000000L;
  ScrollBar1->Min =-2000000000L;
  ScrollBar1->Max =2000000000L;

  ScrollBar2->Min =_Bottom-(_Top-_Bottom)/50;
  ScrollBar2->Max =_Top+(_Top-_Bottom)/50;
  ScrollBar1->Min =_Right-(_Left-_Right)/50;
  ScrollBar1->Max =_Left+(_Left-_Right)/50;
  ScrollBar2->Position = -pntRLS.y;
  ScrollBar1->Position = -pntRLS.x;
  ScrollBar1->SmallChange=(ScrollBar1->Max-ScrollBar1->Min)/100;
  ScrollBar2->SmallChange=(ScrollBar2->Max-ScrollBar2->Min)/100;
  ScrollBar1->LargeChange=(ScrollBar1->Max-ScrollBar1->Min)/20;
  ScrollBar2->LargeChange=(ScrollBar2->Max-ScrollBar2->Min)/20;

  ScrollBar1->OnScroll = ScrollBar2Scroll;
  ScrollBar2->OnScroll = ScrollBar2Scroll;

}







void __fastcall TForm1::N15Click(TObject *Sender)
{
 if (FTime1->Visible == false)
    {
     FTime1->Visible = true;
     N15->Checked = true;
    }
    else
    {
     FTime1->Visible = false;
     N15->Checked = false;
    }
 }
//---------------------------------------------------------------------------


void __fastcall TForm1::ToolButton5Click(TObject *Sender)
{
 try
 {
   if(MCO.GetWorking())
   {
     MCO.RestoreMasshtab_from_smPrev();
     ModifyScrollBars();
     MCO.Paint(Handle,0,0);
   }
 }
 MY_CATCH()
}
//---------------------------------------------------------------------------


//-----------�������� �������
void __fastcall TForm1::ToolButton8Click(TObject *Sender)
{
  try
  {
    if(MCO.GetWorking())
    {
       MCO.FullScale();
       ModifyScrollBars();
       MCO.Paint(Handle,0,0);
    }
  }
  MY_CATCH()
}
//---------------------------------------------------------------------------











void __fastcall TForm1::ToolButton8MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
      StatusBar1->SimpleText = LSTR(" Full scale","  �������� �������") ;
      ToolButton8->ImageIndex = 13;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton6MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
      StatusBar1->SimpleText = 
LSTR("  Show or hide track","  ���������� ��� �� ���������� ����") ;
}
//---------------------------------------------------------------------------





void __fastcall TForm1::ToolButton10MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
      StatusBar1->SimpleText = 
  LSTR("Show or hide tracking","  �������� ��� ������ �������������") ;
}
//---------------------------------------------------------------------------






//-----��� �������� �����-��� �������������
void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{

// CanClose = false;
// Form1->WindowState = wsMinimized;

}
//---------------------------------------------------------------------------




void __fastcall TForm1::ScrollBar1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
//----------���������� �� ����������(Ctrl + "+")
if(Shift.Contains(ssCtrl))
   {if ( Key==0xBB || Key==0x6B)

    {
       try
       {
        if(MCO.GetWorking())
        {
         MCO.MultyScale(1.5);
         ModifyScrollBars();
         MCO.Paint(Handle,0,0);
         }
        }
        MY_CATCH()
      }
    }



//----------���������� �� ����������(Ctrl + "-")
if(Shift.Contains(ssCtrl))
{if ( Key==0xBD||Key==0x6D)

    {
     try
     {

      if(MCO.GetWorking())
      {
       MCO.MultyScale(1.0/1.5);
       ModifyScrollBars();
       MCO.Paint(Handle,0,0);
      }
     }
     MY_CATCH()
   }
 }

//----------�������� ������� �� ����������(F5)
if (  Key == VK_F5 )
   {
     try
     {
      if(MCO.GetWorking())
      {
       MCO.FullScale();
       ModifyScrollBars();
       MCO.Paint(Handle,0,0);
      }
     }
     MY_CATCH()
    }
// ������������ �� ����� ��������� �������
// ��������� 21.01.08 ����
    float mh;
    if(Shift.Contains(ssCtrl)) mh=0.4; else mh=0.08;
    if (  Key == VK_LEFT )  {  Form1->MoveRLS(mh,0); Key=0;}
    if (  Key == VK_RIGHT)  {  Form1->MoveRLS (-mh,0); Key=0;}
    if (  Key == VK_UP )    {  Form1->MoveRLS(0,mh); Key=0;}
    if (  Key == VK_DOWN)   {  Form1->MoveRLS (0,-mh); Key=0;}

}
//---------------------------------------------------------------------------




void __fastcall TForm1::ToolButton6Click(TObject *Sender)
{
  try
  {
   if (ToolButton6->Tag == 0)
   {
      N19->Checked = false;
      ToolButton6->Hint = LSTR("Show track", "�������� ����")  ;
      ToolButton6->Tag = 1;
      ToolButton6->Down = false;
//������ ��������
      init.iTail=0;
   }
   else
   {
      N19->Checked = true;
      ToolButton6->Hint = LSTR("Do not show track","�� �������� ����")  ;
      ToolButton6->Tag = 0;
      ToolButton6->Down = true;
      init.iTail=1;
    }
    MCO.SetNewOption(init);
    Save_INI_f_VisibleTail(&init,UserIni);

    MCO.Paint(Handle,0,0);
  }
  MY_CATCH()
}
//---------------------------------------------------------------------------









void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
//�������� ��������
  if(HM)
  {
    EventSet();
  }

//��� �� ����� ��������� ������� �� ���� �����
  Log_IKO=LOGFILE_IKO::GetStaticLog();
  if(Log_IKO)Log_IKO->TestNextDay();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton11MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        StatusBar1->SimpleText = LSTR("  Show or hide Label frame",
"  �������� ��� ������ ����� ���������");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton11MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        StatusBar1->SimpleText = " ";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton3MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        StatusBar1->SimpleText = LSTR("Show options",
"  �������� �����");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton3MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        StatusBar1->SimpleText = "  ";
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ToolButton11Click(TObject *Sender)
{
if (ToolButton11->Tag == 0)
   {N1->Checked = false;
    ToolButton11->Hint = LSTR("Show Label frame",
 "���������� ����� ���������");
    ToolButton11->Tag = 1;
    ToolButton11->Down = false;
//������ ��������
    init.iRamkaFormulyar=0;
   }
else
   {N1->Checked = true;
    ToolButton11->Hint = LSTR("Hide Label frame",
"������ ����� ��������");
    ToolButton11->Tag = 0;
    ToolButton11->Down = true;
    init.iRamkaFormulyar=1;
    }
    try
    {
      MCO.SetNewOption(init);
      Save_INI_f_VisibleFormulyar(&init,UserIni);
      MCO.Paint(Handle,0,0);
    }
    MY_CATCH()
}
//---------------------------------------------------------------------------


//���������� ������� FMessage1
void __fastcall TForm1::SetFMessage1Size(void)
{
   int W1=FMessage1->Label1->Width+FMessage1->Label1->Left;
   int W2=FMessage1->Label2->Width+FMessage1->Label1->Left;
   FMessage1->Width=MAX(W1,W2);
   FMessage1->Left=ClientWidth-PanelUpravleniya->Width-
                  FMessage1->Width-Panel2->Width;
   FMessage1->Top=0;
}






void __fastcall TForm1::ToolButton10Click(TObject *Sender)
{
   LOGFILE_IKO *LogFile_IKO=LOGFILE_IKO::GetStaticLog();

   if (ToolButton10->Tag == 0)
   {
     N20->Checked = false;
     ToolButton10->Hint = LSTR("Enable tracking","�������� �������������");
     ToolButton10->Tag = 1;
     ToolButton10->Down = false;
     init.IsSoprovojdenie=0;
   }else{
     N20->Checked = true;
     ToolButton10->Hint = LSTR("Disable tracking","��������� �������������")  ;
     ToolButton10->Tag = 0;
     ToolButton10->Down = true;
     init.IsSoprovojdenie=1;
   }
   try
   {
//������������� ��
      if(IALDT)IALDT->Stop();
      if(CRT)CRT->Disconnect();
      LogFile_IKO->Stop();
      CRT=TComReadThread::Connect(init);
      if(!CRT)
      {
#ifdef ENG_LANG
          AnsiString Put=String("Error opening port  ")+String(init.csComPortStr);
          if(FMessage1->Visible)FMessage1->Hide();
          FMessage1->ShowText(Put, "Change port settings");
#else
          AnsiString Put=String("������ �������� ����� ")+String(init.csComPortStr);
          if(FMessage1->Visible)FMessage1->Hide();
          FMessage1->ShowText(Put, "�������� ��������� COM-�����!");
#endif
          SetFMessage1Size();
          Log_IKO=LOGFILE_IKO::GetStaticLog();
          if(Log_IKO)Log_IKO->OutNotCriticalError(Put.c_str());
          WasOutLogError=1;
          MCO.NoPackets=2;
      }else{
          MCO.NoPackets=0;
          WasOutLogError=0;
      }
//�������� ��������� ����� �������� ������
       MCO.Restart();
       if(CRT)
       {
          IALDT=new TInAndLogDataThread(CRT->hEventNoEmpty,&MCO,&SSS,&SSS_Filter,init);
       }else{
          IALDT=NULL;
       }
      MCO.SetNewOption(init);
      MCO.Paint(Handle,0,0);
   }
   MY_CATCH()

}
//---------------------------------------------------------------------------


//��������� ���������� � ������ in_and_log_trhead � Execute ��� ������������� ������������
//����������.  ��� ����� ���������� ��������������� � ������ SynhroExitForException
void __fastcall TForm1::TreatExceptionInSecondThread(
                                         void *_Exception,  //��������� �� ����������
                                         int TypeOfExcept  //��� ���������� 1: EIkoException
              						   //2: Exception
                                         )                 //�� ���� ����������
{
    EIkoException  *EIE;
    Exception *exc;
 
     Log_IKO=LOGFILE_IKO::GetStaticLog();        

     if(Log_IKO)switch(TypeOfExcept)
     {
         case 1:
           EIE=(EIkoException  *)_Exception;   
           Log_IKO->OutCriticalError(EIE->TextIskl,         
    EIE->ModuleName, EIE->ClassName, EIE->MethodName, EIE->RegExName);   
         break;

         case 2:
           exc=(Exception  *)_Exception;
           TempString=exc->ClassName();              
           Log_IKO->OutCriticalError(TempString.c_str(),  
                              exc->Message.c_str());                  
         break;

         default:
#ifdef ENG_LANG
           Log_IKO->OutCriticalError("No notable exception",  
            "No notable exception");           
#else
           Log_IKO->OutCriticalError("�� ��������� ����������! ",  
            "�� ��������� ����������");           
#endif

    }              
      
    ReleaseLibrary();                     
    StopWork();                           
    ExitProcess(1);                          
}
extern AnsiString HelpFilePaht;
void __fastcall TForm1::N5Click(TObject *Sender)
{
    AnsiString HhpStr;
    HhpStr = HelpFilePaht + "::/main window iko.htm";
    HtmlHelp(GetDesktopWindow(),
    HhpStr.c_str(),
    HH_DISPLAY_TOPIC,NULL);      
        
}
//---------------------------------------------------------------------------






void __fastcall TForm1::FMessage1Button1Click(TObject *Sender)
{
        AnsiString prog,comName;
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        int i;

//1 ������ ��������� ����� CreteProccess
        ZeroMemory(&si, sizeof(si));
        si.cb=sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        i=String(init.csComPortStr).Length()-1;
        for(;;i--)
        {
                if(i<0)
                {
                    break;
                }
                if(init.csComPortStr[i]=='\\')
                {
                        i++;
                        break;
                }
                if(i==0)
                {
                        break;
                }
        }
        if(i>=0)
        {
                comName=String(init.csComPortStr+i);

                prog=String(init.csFullComdisablePath)+String(" /disable")+
                        String(" ")+comName;
                if(CreateProcess( NULL, prog.c_str(),
                        NULL,NULL,FALSE,0,NULL,NULL,&si,&pi ))
                {
                        Sleep(1000);
                        ExitWindowsEx(EWX_REBOOT,0);
                }else{
#ifdef ENG_LANG
                    MessageBox(NULL,
                "Error starting program comdisable.exe. Perhaps wrongly set the program path comdisable.exe",
                  "Error starting program comdisable.exe",MB_OK);
#else
                    MessageBox(NULL,
                "������ ������� ��������� comdisable.exe. ��������, �� ��������� ����� ���� � comdisable.exe",
                  "������ ������� comdisable.exe",MB_OK);
#endif
                }



        }
}
//---------------------------------------------------------------------------




