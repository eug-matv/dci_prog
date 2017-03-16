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
void __fastcall TAboutBox::FormCreate(TObject *Sender)
{
#ifndef ENG_LANG
	AboutBox->Caption="О программе";
        AboutBox->Label1->Caption="(ИКО АОРЛ)";
	AboutBox->Label2->Caption="Индикатор кругового обзора АОРЛ";
	AboutBox->Label3->Caption="2007 г.";
       //	AboutBox->Label4->Caption="(с) НТЦ  ОКБ  ОАО \"ЧРЗ \"ПОЛЕТ\"";
	AboutBox->Label5->Caption="Pentium IV и выше";
	AboutBox->Label6->Caption="Видеопамять от 64 Мб";
	AboutBox->Label7->Caption="Частота от 1ГГц";
	AboutBox->Label8->Caption="ОЗУ не менее 256 Мб";
	AboutBox->Label9->Caption="Рекомендуемые требования:";
#endif
}
//---------------------------------------------------------------------------

