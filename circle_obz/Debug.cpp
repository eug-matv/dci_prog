//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Debug.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFDebug *FDebug;
long DebugF1; //Период вывода

//---------------------------------------------------------------------------
__fastcall TFDebug::TFDebug(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFDebug::Timer1Timer(TObject *Sender)
{
   Label1->Caption=IntToStr((int)DebugF1);        
}
//---------------------------------------------------------------------------
