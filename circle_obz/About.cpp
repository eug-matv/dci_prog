//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "About.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//--------------------------------------------------------------------- 
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------

void __fastcall TAboutBox::SpeedButton1Click(TObject *Sender)
{
        AboutBox->Close();        
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::Image1Click(TObject *Sender)
{
        AboutBox->Close();
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{  if(Key == VK_ESCAPE)
   AboutBox->Close();
}
//---------------------------------------------------------------------------




