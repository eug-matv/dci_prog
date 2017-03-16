//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "lang_str.h"
#include "MessageFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFMessage *FMessage;
//---------------------------------------------------------------------------
__fastcall TFMessage::TFMessage(TComponent* Owner)
        : TFrame(Owner)
{
}
//---------------------------------------------------------------------------



void __fastcall TFMessage::ShowText(AnsiString MessText)
{
   if(Visible)return;
   Label1->Caption=MessText;
   Label2->Caption="";
   BStartComdisable->Hide();
   Show();
}

void __fastcall TFMessage::ShowText(
             AnsiString MessText1,
             AnsiString MessText2)
{
   if(Visible)return;
   Label1->Caption=MessText1;
   Label2->Caption=MessText2;
  // Button1->Hide();
    BStartComdisable->Hide();
   Show();
}









