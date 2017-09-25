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
	AboutBox->Caption="� ���������";
        AboutBox->Label1->Caption="(��� ����)";
	AboutBox->Label2->Caption="��������� ��������� ������ ����";
	AboutBox->Label3->Caption="2007 �.";
       //	AboutBox->Label4->Caption="(�) ���  ���  ��� \"��� \"�����\"";
	AboutBox->Label5->Caption="Pentium IV � ����";
	AboutBox->Label6->Caption="����������� �� 64 ��";
	AboutBox->Label7->Caption="������� �� 1���";
	AboutBox->Label8->Caption="��� �� ����� 256 ��";
	AboutBox->Label9->Caption="������������� ����������:";
#endif
}
//---------------------------------------------------------------------------

