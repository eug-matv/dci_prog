//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "lang_str.h"

#include "MainRBZ.h"
#include "Navigation.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm4::SpeedButton10Click(TObject *Sender)
{

  Form1->MoveRLS(0,0.4);

}
//---------------------------------------------------------------------------

void __fastcall TForm4::SpeedButton11Click(TObject *Sender)
{
  Form1->MoveRLS(0.4,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm4::SpeedButton13Click(TObject *Sender)
{
  Form1->MoveRLS(0,-0.4);
}
//---------------------------------------------------------------------------

void __fastcall TForm4::SpeedButton12Click(TObject *Sender)
{
 Form1->MoveRLS (-0.4,0);
}
//---------------------------------------------------------------------------


void __fastcall TForm4::FormCreate(TObject *Sender)
{
GroupBox5->Tag = 1;
}
//---------------------------------------------------------------------------


void __fastcall TForm4::FormHide(TObject *Sender)
{
     Form1->ToolButton4->Down = false;
     Form1->ToolButton4->Tag=1;
     Form1->ToolButton4->Hint =
LSTR("Show the additional window Navigation",
"Показать панель навигации");
        
}
//---------------------------------------------------------------------------

void __fastcall TForm4::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
// передвижение по экрну клавишами курсора
// добавлено 21.01.08 Стас
    float mh;
    if(Shift.Contains(ssCtrl)) mh=0.4; else mh=0.08;
    if (  Key == VK_LEFT )  {  Form1->MoveRLS(mh,0); Key=0;}
    if (  Key == VK_RIGHT)  {  Form1->MoveRLS (-mh,0); Key=0;}
    if (  Key == VK_UP )    {  Form1->MoveRLS(0,mh); Key=0;}
    if (  Key == VK_DOWN)   {  Form1->MoveRLS (0,-mh); Key=0;}
// Стас окончание    
   if(Key == VK_ESCAPE)
   Form4->Close();
}
//---------------------------------------------------------------------------

