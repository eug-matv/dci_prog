//---------------------------------------------------------------------------

#include <vcl.h>
//#include <Qt.h>
#pragma hdrstop

#include "lang_str.h"

#include "options.h"
#include "structs.h"
#include "MainRBZ.h"
//for Microsoft Help
#include <htmlhelp.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define RGB_TO_STRING(r,g,b) IntToHex(b,2)+IntToHex(g,2)+IntToHex(r,2)



TForm2 *Form2;
AnsiString text_pole;
extern struct initialization *init_struct;


//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
    : TForm(Owner)
{
text_pole="";
}
//---------------------------------------------------------------------------


void __fastcall TForm2::Button2Click(TObject *Sender)
{


// ����� �� �� ����� ��������
// �������� ������������  ��������� ����������
  int i,k;
  char tmp_port_name[100];


 // �����������-������������ �����
 if (StrToInt(Form2->Edit9->Text)>3 || StrToInt(Form2->Edit10->Text)>3
 || StrToInt(Form2->Edit13->Text)>3 || StrToInt(Form2->Edit15->Text)>3
 || StrToInt(Form2->Edit19->Text)>3)
  {ShowMessage(
	LSTR("Check the options. Thickness of band lines should not exceed 3",
"��������� ������������ �����! ������� ����� �� ������ ��������� 3")
);
  return;}


  //�����
 if (StrToInt(Form2->Edit7->Text)>200)
  {ShowMessage(
LSTR("Check the options. Track length shall not exceed 200",
"��������� ������������ �����! ����� ����� �� ������ ��������� 200")
);
  return;}

 if (StrToInt(Form2->RadCir1->Text)>10 || StrToInt(Form2->RadCir2->Text)>10 || StrToInt(Form2->RadCir3->Text)>10)
  {ShowMessage(LSTR("Check the options. The radius of the circle should not exceed 10",
	"��������� ������������ �����! ������ ���������� �� ������ ��������� 10")
);
  return;}

 if (StrToInt(Form2->WidthCir1->Text)>3 || StrToInt(Form2->WidthCir2->Text)>3 || StrToInt(Form2->WidthCir3->Text)>3)
  {ShowMessage(LSTR("Check the options. Thickness of band lines should not exceed 3",
	"��������� ������������ �����! ������� ����� �� ������ ��������� 3")
);
  return;}

 //��������
 if (StrToInt(Form2->SizeShrift->Text)>16)

  {ShowMessage(
LSTR("Check the options. Acceptable font size from 4 to 16 points",
"��������� ������������ �����! ���������� ������ ������ �� 4 �� 16 ��")
);
  return;}



 if  (StrToInt(Form2->fHeight->Text)>500
   || StrToInt(Form2->fWidth->Text)>500 )

  {ShowMessage(
LSTR("Check the options. Acceptable size of Label to 500x500",
"��������� ������������ �����! ���������� ������� ��������� 500�500")
);
  return;}


  // �����������-������������ �����
  // ����

  init.Clrbackground =  (COLORREF)(String("0x")+EClrbackground->Text).ToInt();
  // ���� ������ ����� ������������� ����� background
  Form1->FTime1->Color =  init.Clrbackground;
     //------������� ���� � ����-����� �� ������ ���� ����� ���
  Form1->FTime1->Label1->Font->Color=(~init.Clrbackground)&0xFFFFFF;

  init.DLines.lopnColor = (COLORREF)(String("0x")+ EDLines->Text).ToInt();
  init.Circle.lopnColor = (COLORREF)(String("0x")+ EClrcircle->Text).ToInt();
  init.DCircle.lopnColor = (COLORREF)(String("0x")+ EDClrcircle->Text).ToInt();
  init.Lines.lopnColor = (COLORREF)(String("0x")+ ELines->Text).ToInt();
  init.DLines.lopnColor = (COLORREF)(String("0x")+ EDLines->Text).ToInt();
 // init.Razvertka.lopnColor = (COLORREF)(String("0x")+ ERazvertka->Text).ToInt();
  init.Scale.lopnColor = (COLORREF)(String("0x")+ EScale->Text).ToInt();

  init.cAzimuthText =  (COLORREF)(String("0x")+ EAzimuth->Text).ToInt();
  init.cDalnostText =  (COLORREF)(String("0x")+ EDalnost->Text).ToInt();

  // �����
  SET_STYLE_STR_TO_PAR(ComboBox7->Text,init.Circle.lopnStyle );
  SET_STYLE_STR_TO_PAR(ComboBox8->Text,init.DCircle.lopnStyle );
  SET_STYLE_STR_TO_PAR(ComboBox9->Text,init.Lines.lopnStyle );
  SET_STYLE_STR_TO_PAR(ComboBox10->Text,init.DLines.lopnStyle );
  //SET_STYLE_STR_TO_PAR(ComboBox11->Text,init.Razvertka.lopnStyle );
  SET_STYLE_STR_TO_PAR(ComboBox12->Text,init.Scale.lopnStyle );

  // �������
  init.Circle.lopnWidth.x = init.Circle.lopnWidth.y = UpDown10->Position;
  init.DCircle.lopnWidth.x = init.DCircle.lopnWidth.y = UpDown11->Position;
  init.Lines.lopnWidth.x = init.Lines.lopnWidth.y = UpDown18->Position;
  init.DLines.lopnWidth.x = init.DLines.lopnWidth.y = UpDown19->Position;
  //init.Razvertka.lopnWidth.x = init.Razvertka.lopnWidth.y = UpDown20->Position;
  init.Scale.lopnWidth.x = init.Scale.lopnWidth.y = UpDown21->Position;

  // �������
 //yul// init.max_dalnost = StrToFloat(Edit1->Text);
  init.azimuth_delta = StrToInt(ComboBox2->Text);
 //yul//  init.dalnost_delta = StrToFloat(ComboBox4->Text);

  // �����
  // ��������� �����
  init.radius1 = UpDown3->Position; // ������
  init.CirclePen1.lopnColor =  (COLORREF)(String("0x") + Form2->EClrcircle1->Text).ToInt();
  init.CirclePen1.lopnWidth.x = init.CirclePen1.lopnWidth.y = UpDown4->Position;
  //SET_STYLE_STR_TO_PAR(Form2->StyleCir1->Text, init.CirclePen1.lopnStyle);
  //init.CircleBrush1.lbColor = (COLORREF)(String("0x") + Form2->EClrcirclebrush1->Text).ToInt();
  //SET_STYLE_STR_TO_PAR_BRUSH(Form2->StyleBrush1->Text, init.CircleBrush1.lbStyle);    // ����� �������

  // ��������� �����
  init.radius2 = UpDown5->Position; // ������
  init.CirclePen2.lopnColor =  (COLORREF)(String("0x") + Form2->EClrcircle2->Text).ToInt();
  init.CirclePen2.lopnWidth.x = init.CirclePen2.lopnWidth.y = UpDown6->Position;
 // SET_STYLE_STR_TO_PAR(Form2->StyleCir2->Text, init.CirclePen2.lopnStyle);
 // init.CircleBrush2.lbColor = (COLORREF)(String("0x") + Form2->EClrcirclebrush2->Text).ToInt();
  //SET_STYLE_STR_TO_PAR_BRUSH(Form2->StyleBrush2->Text, init.CircleBrush2.lbStyle);    // ����� �������

  // ������������ �����
  init.radius3 = UpDown7->Position; // ������
  init.CirclePen3.lopnColor =  (COLORREF)(String("0x") + Form2->EClrcircle3->Text).ToInt();
  init.CirclePen3.lopnWidth.x = init.CirclePen3.lopnWidth.y = Form2->UpDown8->Position;
  //SET_STYLE_STR_TO_PAR(Form2->StyleCir3->Text, init.CirclePen3.lopnStyle);
//  init.CircleBrush3.lbColor = (COLORREF)(String("0x") + Form2->EClrcirclebrush3->Text).ToInt();
  //SET_STYLE_STR_TO_PAR_BRUSH(Form2->StyleBrush3->Text, init.CircleBrush3.lbStyle);    // ����� �������

  init.iTail = CheckBox2->Checked;
  init.iTailLength = UpDown22->Position;


  // ��������� ���������
  init.szTextOut = Form2->UpDown9->Position;  // ������ ���������� ���������
  init.cText =  (COLORREF)(String("0x") + Form2->EClrShrift->Text).ToInt();
  init.sShir = Form2->UpDown16->Position;          // ������ ���������
  init.sVysot =Form2->UpDown17->Position;        // ������ ���������

//  init.thirdstring = Form2->RadioGroup2->ItemIndex;     // 3-�� ������ ���������

//��� ���������� �� ��������
  init.IsOutFormulyar=cbFormulyar->Checked;
  init.IsOutNomer=cbNomer->Checked;
  init.IsOutVysota=cbVysota->Checked;
  init.IsOutTime=cbTime->Checked;
  init.IsOutAzimutDalnost=cbAzimutDalnost->Checked;
  init.IsOutSkorost=cbSkorost->Checked;
  init.IsOutOstatokTopliva=cbOstatokTopliva->Checked;
  init.IsOutVeroyatnostPervKanal=cbVeroyatnostPervKanal->Checked;
  init.IsOutVeroyatnostVtorKanal=cbVeroyatnostVtorKanal->Checked;
  init.iRamkaFormulyar=CheckBox1->Checked;

 //Yul //init.iRamkaFormulyar=cbRamkaFormulyar->Checked;



//��������� COM-�����
  init.iBaudRate=RGBaudrate->Items->Strings[RGBaudrate->ItemIndex].ToInt();
  //strcpy(init.csComPortStr,CBPortName->Items->Strings[CBPortName->ItemIndex].c_str());
  strcpy(tmp_port_name,CBPortName->Text.Trim().c_str());
  k=0;
  for(i=0;i<strlen(tmp_port_name);i++)
  {
     if(tmp_port_name[i]=='\\'||tmp_port_name[i]=='.')
     {
       k++;
     }else{
        break;
     }
  }
  strcpy(init.csComPortStr,tmp_port_name+k);


  //��������� ���� ����
  init.iStopBits = RGChisloStopBits->ItemIndex;

//��������� �������� ��������
  if(cbFParity->Checked){
    init.fParity = 1;
  }else{
    init.fParity = 0;
  }

//��������� ����� ��������
  init.parityMode = RGParityMode->ItemIndex;


//��������� ������ � ����
  AnsiString AStr;
  AStr=EFullPathToData->Text.Trim();
  if(AStr.Length()<255)
  {
    strcpy(init.csFullPathToData, AStr.c_str());
  }else{
    AStr=AStr.SubString(1, 254);
    strcpy(init.csFullPathToData, AStr.c_str());
  }



  init.IsFilterDataToFile=CB_IsFilterDataToFile->Checked;
  init.IsSaveDataToFile=CB_IsSaveDataFile->Checked;
  init.IsSaveSectorsToFile=CB_IsSaveSectorsToFile->Checked;



  iModified=1;
  int formulyarGetSize(initialization &init);
  formulyarGetSize(init);



  Close();
}
//---------------------------------------------------------------------------


void __fastcall TForm2::EMaxdalnostKeyPress(TObject *Sender, char &Key)
{
  if (Key=='0' || Key=='1' || Key=='2' || Key=='3' || Key=='4' || Key=='5' || Key=='6' || Key=='7' || Key=='8' || Key=='9' || Key=='.')
     text_pole+=Key;



}
//---------------------------------------------------------------------------


void __fastcall TForm2::EMaxdalnostChange(TObject *Sender)
{
//  Edit1->Text=text_pole;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::SClrbackgroundMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  bool RetB;
  long i;
  AnsiString ShName, EdtName;
  TShape *Sh=NULL;
  TEdit *Edt=NULL;

  Sh=(TShape*)Sender;
  ShName=Sh->Name;
  EdtName=ShName;
  EdtName[1]='E';
  for (i=0;i<ComponentCount;i++)
  {
    if(Components[i]->Name==EdtName)
    {
       Edt=(TEdit*)Components[i];
       break;
    }
  }
  if(Edt==NULL)
  {
     MessageBox(Handle, 
LSTR("Error with Components!", "������ � ������������!") , 
LSTR("Attention! Error!", "��������, ������!") ,MB_OK);
     return;
  }

  ColorDialog1->Color=Sh->Brush->Color;
  RetB=ColorDialog1->Execute();
  if(RetB)
  {
//����������� ����

    Sh->Brush->Color=ColorDialog1->Color;
    Edt->Text=IntToHex(
              (0xFFFFFF)&((ColorDialog1->Color)),6);

  }
}
//---------------------------------------------------------------------------


void __fastcall TForm2::EClrbackgroundKeyPress(TObject *Sender, char &Key)
{
   if (Key>='0' && Key<='9' ||
       Key>='a' && Key<='f' ||
       Key>='A' && Key<='F' ||Key==0x08)return;
   Key=0;
}
//---------------------------------------------------------------------------


void __fastcall TForm2::EClrbackgroundDblClick(TObject *Sender)
{
  bool RetB;  //
  long i;
  AnsiString ShName, EdtName;
  TShape *Sh=NULL;
  TEdit *Edt;

  Edt=(TEdit*)Sender;
  EdtName=Edt->Name;
  ShName=EdtName;
  ShName[1]='S';
  for (i=0;i<ComponentCount;i++)
  {
    if(Components[i]->Name==ShName)
    {
       Sh=(TShape*)Components[i];
       break;
    }
  }
  if(Sh==NULL)
  {
      MessageBox(Handle, 
LSTR("Error with Components!",  "������ � ������������!"),
LSTR("Attention! Error!", "��������, ������!"), MB_OK);
     return;
  }

  ColorDialog1->Color=Sh->Brush->Color;
  RetB=ColorDialog1->Execute();
  if(RetB)
  {
//����������� ����

    Sh->Brush->Color=ColorDialog1->Color;
    Edt->Text=IntToHex(
              (0xFFFFFF)&((ColorDialog1->Color)),6
                                      );

  }

}
//---------------------------------------------------------------------------

void __fastcall TForm2::EClrbackgroundChange(TObject *Sender)
{
  long i;
  AnsiString ShName, EdtName;
  TShape *Sh=NULL;
  TEdit *Edt;

  Edt=(TEdit*)Sender;
  EdtName=Edt->Name;
  ShName=EdtName;
  ShName[1]='S';
  for (i=0;i<ComponentCount;i++)
  {
    if(Components[i]->Name==ShName)
    {
       Sh=(TShape*)Components[i];
       break;
    }
  }
  if(Sh==NULL)
  {
     MessageBox(Handle, 
LSTR("Error with Components!","������ � ������������!") , 
LSTR("Attention! Error!","��������, ������!"), MB_OK);
     return;
  }
  Sh->Brush->Color=StrToInt(String("0x")+Edt->Text.Trim());

    
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ComboBox1Exit(TObject *Sender)
{
 int i;
  TComboBox *CB;
  CB=(TComboBox*)Sender;
  CB->Text=Trim(CB->Text);
  for(i=0;i<CB->Items->Count;i++)
  {
     if(CB->Text==CB->Items->Strings[i])
     {
        break;
     }
  }
  if(i==CB->Items->Count)
  {
     CB->Text="PS_SOLID";
  }
}
//---------------------------------------------------------------------------







void __fastcall TForm2::FormShow(TObject *Sender)
{
  int i;
 //
    Form2->OnKeyDown;
   // SHOW �����������-������������ �����
   iModified=0;


  Form2->EClrbackground->Text=IntToHex(0xFFFFFF&(int)(init.Clrbackground),6);
  Form2->EClrcircle->Text = IntToHex(0xFFFFFF&(int)(init.Circle.lopnColor),6);
  Form2->EDClrcircle->Text = IntToHex(0xFFFFFF&(int)(init.DCircle.lopnColor),6);
  Form2->ELines->Text = IntToHex(0xFFFFFF&(int)(init.Lines.lopnColor),6);
  Form2->EDLines->Text = IntToHex(0xFFFFFF&(int)(init.DLines.lopnColor),6);
  //Form2->ERazvertka->Text = IntToHex(0xFFFFFF&(int)(init.Razvertka.lopnColor),6);
  Form2->EScale->Text = IntToHex(0xFFFFFF&(int)(init.Scale.lopnColor),6);

   Form2->EAzimuth->Text =  IntToHex(0xFFFFFF&(int)(init.cAzimuthText),6);
   Form2->EDalnost->Text =  IntToHex(0xFFFFFF&(int)(init.cDalnostText),6);



  // �����
  SET_STYLE_PAR_TO_STR(init.Circle.lopnStyle,Form2->ComboBox7->Text);
  SET_STYLE_PAR_TO_STR(init.DCircle.lopnStyle,Form2->ComboBox8->Text);
  SET_STYLE_PAR_TO_STR(init.Lines.lopnStyle,Form2->ComboBox9->Text);
  SET_STYLE_PAR_TO_STR(init.DLines.lopnStyle,Form2->ComboBox10->Text);
  //SET_STYLE_PAR_TO_STR(init.Razvertka.lopnStyle,Form2->ComboBox11->Text);
  SET_STYLE_PAR_TO_STR(init.Scale.lopnStyle,Form2->ComboBox12->Text);

  // �������
  //Form2->Edit9->Text =FloatToStr(init.Circle.lopnWidth.x);
  //Form2->Edit10->Text =FloatToStr(init.DCircle.lopnWidth.x);
  //Form2->Edit13->Text =FloatToStr(init.Lines.lopnWidth.x);
  //Form2->Edit15->Text =FloatToStr(init.DLines.lopnWidth.x);
  //Form2->Edit17->Text =FloatToStr(init.Razvertka.lopnWidth.x);
  //Form2->Edit19->Text =FloatToStr(init.Scale.lopnWidth.x);

  Form2->UpDown10->Position = init.Circle.lopnWidth.x;
  Form2->UpDown11->Position = init.DCircle.lopnWidth.x;
  Form2->UpDown18->Position = init.Lines.lopnWidth.x;
  Form2->UpDown19->Position = init.DLines.lopnWidth.x;
  //Form2->UpDown20->Position = init.Razvertka.lopnWidth.x;
  Form2->UpDown21->Position = init.Scale.lopnWidth.x;




  // �������
  //Form2->Edit1->Text = FloatToStr(init.max_dalnost);
  Form2->ComboBox2->Text = FloatToStr(init.azimuth_delta);
 // Form2->ComboBox4->Text = FloatToStr(init.dalnost_delta);

  // �����
  // ��������� �����

  //RadCir1->Text    = IntToStr(init.radius1); // ������

  Form2->UpDown3->Position = init.radius1;  // ������
  Form2->EClrcircle1->Text =IntToHex(0xFFFFFF&(int)(init.CirclePen1.lopnColor),6); // ����

 // Form2->WidthCir1->Text   = FloatToStr(init.CirclePen1.lopnWidth.x); // ������� �����
  Form2->UpDown4->Position = init.CirclePen1.lopnWidth.x; // ������� �����
  //SET_STYLE_PAR_TO_STR(init.CirclePen1.lopnStyle, Form2->StyleCir1->Text); // ����� �����
  //Form2->EClrcirclebrush1->Text = IntToHex(0xFFFFFF&(int)(init.CircleBrush1.lbColor),6); // ���� �������
 // SET_STYLE_PAR_TO_STR_BRUSH(init.CircleBrush1.lbStyle, Form2->StyleBrush1->Text);    // ����� �������



  // ��������� �����
  //Form2->RadCir2->Text     = FloatToStr(init.radius2); // ������

  Form2->UpDown5->Position = init.radius2; // ������
  Form2->EClrcircle2->Text =IntToHex(0xFFFFFF&(int)(init.CirclePen2.lopnColor),6); // ����

 // Form2->WidthCir2->Text   = FloatToStr(init.CirclePen2.lopnWidth.x); // ������� �����

  Form2->UpDown6->Position = init.CirclePen2.lopnWidth.x; // ������� �����
 // SET_STYLE_PAR_TO_STR(init.CirclePen2.lopnStyle, Form2->StyleCir2->Text); // ����� �����
 // Form2->EClrcirclebrush2->Text = IntToHex(0xFFFFFF&(int)(init.CircleBrush2.lbColor),6); // ���� �������
 // SET_STYLE_PAR_TO_STR_BRUSH(init.CircleBrush2.lbStyle, Form2->StyleBrush2->Text);    // ����� �������

  // ������������ �����
  //Form2->RadCir3->Text     = FloatToStr(init.radius3); // ������

  Form2->UpDown7->Position = init.radius3; // ������
  Form2->EClrcircle3->Text =IntToHex(0xFFFFFF&(int)(init.CirclePen3.lopnColor),6); // ����

 // Form2->WidthCir3->Text   = FloatToStr(init.CirclePen3.lopnWidth.x); // ������� �����

  Form2->UpDown8->Position = init.CirclePen3.lopnWidth.x; // ������� �����
  //SET_STYLE_PAR_TO_STR(init.CirclePen3.lopnStyle, Form2->StyleCir3->Text); // ����� �����
  //Form2->EClrcirclebrush3->Text = IntToHex(0xFFFFFF&(int)(init.CircleBrush3.lbColor),6); // ���� �������
 // SET_STYLE_PAR_TO_STR_BRUSH(init.CircleBrush3.lbStyle, Form2->StyleBrush3->Text);    // ����� �������

  Form2-> CheckBox2->Checked = init.iTail;      //���������� ����
  Form2->UpDown22->Position = init.iTailLength; //����� �����


  // ��������� ���������
  Form2->UpDown9->Position  = init.szTextOut;  // ������ ���������� ���������
  Form2->EClrShrift->Text = IntToHex(0xFFFFFF&(int)(init.cText),6); // ���� �������
  Form2->UpDown16->Position = init.sShir;      // ������ ���������
  Form2->UpDown17->Position = init.sVysot;     // ������ ���������
//  Form2->RadioGroup2->ItemIndex = init.thirdstring;      // 3-�� ������ ���������
  CheckBox1->Checked      = init.iRamkaFormulyar;

  cbFormulyar->Checked=init.IsOutFormulyar;
  cbNomer->Checked=init.IsOutNomer;
  cbVysota->Checked=init.IsOutVysota;
  cbTime->Checked=init.IsOutTime;
  cbAzimutDalnost->Checked=init.IsOutAzimutDalnost;
  cbSkorost->Checked=init.IsOutSkorost;
  cbOstatokTopliva->Checked=init.IsOutOstatokTopliva;
  cbVeroyatnostPervKanal->Checked=init.IsOutVeroyatnostPervKanal;
  cbVeroyatnostVtorKanal->Checked=init.IsOutVeroyatnostVtorKanal;
  //yul//cbRamkaFormulyar->Checked=init.iRamkaFormulyar;


//��������� �����
  for( i=0;i<RGBaudrate->Items->Count;i++)
  {
     if(RGBaudrate->Items->Strings[i].ToInt()==
        init.iBaudRate)
     {
        RGBaudrate->ItemIndex=i;
        break;
     }
  }
  if(i==RGBaudrate->Items->Count)RGBaudrate->ItemIndex=6;

  CBPortName->Items->Clear();

  for(i=0;i<slComPortsList->Count;i++)
  {

        CBPortName->Items->Add(slComPortsList->Strings[i]);
  }

  CBPortName->Text=String(init.csComPortStr).Trim();
  CBPortName->ItemIndex=-1;
  for(i=0;i<slComPortsList->Count;i++)
  {
      if(CBPortName->Text==slComPortsList->Strings[i].Trim())
      {
        CBPortName->ItemIndex=i;
        break;
      }
  }

//��������� ���� ����
  RGChisloStopBits->ItemIndex = init.iStopBits % 3;

//��������� �������� ��������
  cbFParity->Checked = (init.fParity != 0);

//��������� ����� ��������
  RGParityMode->ItemIndex = init.parityMode % 5;   


//���������� ������
  EFullPathToData->Text=String(init.csFullPathToData);
  CB_IsSaveDataFile->Checked=init.IsSaveDataToFile;
  CB_IsFilterDataToFile->Checked=init.IsFilterDataToFile;
  CB_IsSaveSectorsToFile->Checked=init.IsSaveSectorsToFile;

//----yul
  Form2->WindowState = wsMaximized;
  Form2->PageControl1->ActivePage = TabSheet1;
}
//---------------------------------------------------------------------------

 //-----Yul------------------------------------------------------------------

void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
     Form1->ToolButton3->Down = false;
}
//---------------------------------------------------------------------------


     ///////----ESC
void __fastcall TForm2::RadCir1KeyPress(TObject *Sender, char &Key)
{
 if (Key==0x08 || Key>=0x30 && Key<=0x39)return;
 Key=0;


}
//---------------------------------------------------------------------------




void __fastcall TForm2::FormCreate(TObject *Sender)
{
 Caption = LSTR("Options", "�����");

#ifndef ENG_LANG
        Caption="�����";
        TabSheet1->Caption="�����������-������������ �����";
	TabSheet7->Caption="������";
	GroupBox13->Caption="�������������� ������������ �����";
	Label46->Caption="����";
	Label47->Caption="�������";
	Label48->Caption="�����";
	GroupBox12->Caption="������� ������������ �����";
	Label43->Caption="����";
	Label44->Caption="�������";
	Label45->Caption="�����";
	Label1->Caption="�������� ";
	Label2->Caption="��� �������";
	Label4->Caption="�������";
	TabSheet6->Caption="���������";
	GroupBox10->Caption="�������  ������  ���������";
	Label27->Caption="����";
	Label28->Caption="�������";
	Label39->Caption="�����";
	GroupBox11->Caption="�������������� ������ ���������";
	Label40->Caption="����";
	Label41->Caption="�����";
	Label42->Caption="�������";
	Label3->Caption="��������";
	Label5->Caption=" ���������";
	GroupBox16->Caption="";
	LClrbackground->Caption="���� ����";
	GroupBox15->Caption="������� ��������";
	Label52->Caption="����";
	Label53->Caption="�������";
	Label54->Caption="�����";
	TabSheet2->Caption="�����";
	GroupBox3->Caption="       ��������� ����";
	Label7->Caption="������";
	Label8->Caption="����";
	Label11->Caption="������� �����";
	GroupBox4->Caption="       ��������� ����";
	Label13->Caption="������";
	Label14->Caption="���� ";
	Label17->Caption="������� �����";
	GroupBox5->Caption="       ������������ �����";
	Label19->Caption="������";
	Label20->Caption="���� ";
	Label23->Caption="������� �����";
	GroupBox7->Caption="��������� �����";
	Label29->Caption="����� �����";
	CheckBox2->Caption="���������� ����";
	TabSheet3->Caption="��������";
	GroupBox6->Caption="��������� ���������";
	Label25->Caption="������ ������";
	Label26->Caption="����";
	Label38->Caption="������";
	CheckBox1->Caption="�������� �����";
	gbDataOutOfFormulyar->Caption="���������� ������ � ���������";
	cbNomer->Caption="����� �����";
	cbVysota->Caption="������";
	cbOstatokTopliva->Caption="������� �������";
	cbTime->Caption="�����";
	cbVeroyatnostPervKanal->Caption="����������� ����������� �� ��";
	cbSkorost->Caption="��������";
	cbAzimutDalnost->Caption="������ � ���������";
	cbVeroyatnostVtorKanal->Caption="����������� ����������� �� ��";
	cbFormulyar->Caption="���������� ��������";
	TabSheet4->Caption="��������� �����";
	RGBaudrate->Caption="";
	GBPort->Caption="����";
	Label6->Caption="��� �����";
	RGChisloStopBits->Caption="����� �������� ���";
	TabSheet5->Caption="��������� ���������� � ����";
	GroupBox8->Caption="���� ���������� ������";
	CB_IsSaveDataFile->Caption="��������� ������";
	CB_IsFilterDataToFile->Caption="������";
	CB_IsSaveSectorsToFile->Caption="��������� ������ � ���������� ��������";
	Button2->Caption="OK";
	Button1->Caption="������";
	Button3->Caption="�� ���������... ";
        cbFParity->Caption = "����.����";
        RGBaudrate->Caption = "�������� �����";
        RGParityMode->Caption = "��.�����.��������";
        RGParityMode->Items->Strings[0] = "��� ���� ����.";
        RGParityMode->Items->Strings[1] = "������.�� ������.";
        RGParityMode->Items->Strings[2] = "������.�� ����.";
        RGParityMode->Items->Strings[3] = "��� ����.������ 1";
        RGParityMode->Items->Strings[4] = "��� ����.������ 0";
        StaticText2->Caption = "�������� �����";
#endif
}
//---------------------------------------------------------------------------











void __fastcall TForm2::Button1Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

extern AnsiString HelpFilePaht;
void __fastcall TForm2::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
 //���� F1 �� ������� �������
{  if (Key==VK_F1)
    {
        AnsiString HhpStr;
        HhpStr = HelpFilePaht + "::/options IKO AORL.htm";
        HtmlHelp(GetDesktopWindow(),
        HhpStr.c_str(),
        HH_DISPLAY_TOPIC,NULL);
    }
//���� ESC �� ������� ���� �����
  if(Key==0x1B)
  {
     Close();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button3Click(TObject *Sender)
{
//��������� ���������� �� ���������
//�����������-������������ �����



  EClrbackground->Text=RGB_TO_STRING(0,0,0);
  EScale->Text=RGB_TO_STRING(232,232,0);
  Edit19->Text="1";
  ComboBox12->Text="PS_DASH";
  ELines->Text=RGB_TO_STRING(62,62,62);
  Edit13->Text="2";
  ComboBox9->Text="PS_SOLID";
  EDLines->Text=RGB_TO_STRING(46,46,46);
  Edit15->Text="1";
  ComboBox10->Text="PS_SOLID";
  EAzimuth->Text=RGB_TO_STRING(80,112,168);
  ComboBox2->Text="10";
  EClrcircle->Text=RGB_TO_STRING(62,62,62);
  Edit9->Text="1";
  ComboBox7->Text="PS_SOLID";
  EDClrcircle->Text=RGB_TO_STRING(46,46,46);
  Edit10->Text="1";
  ComboBox8->Text="PS_SOLID";
  EDalnost->Text=RGB_TO_STRING(80,112,128);

 // Form1->FTime1->Label1->Font->Color=clWhite;
 //�����
   RadCir1->Text="5";
   WidthCir1->Text="3";
   EClrcircle1->Text=RGB_TO_STRING(0,0,255);
   RadCir2->Text="5";
   WidthCir2->Text="3";
   EClrcircle2->Text=RGB_TO_STRING(225,225,0);
   RadCir3->Text="5";
   WidthCir3->Text="3";
   EClrcircle3->Text=RGB_TO_STRING(193,72,32);
   CheckBox2->Checked=true;
   Edit7->Text=50;
   cbFormulyar->Checked=true;
   SizeShrift->Text="13";
   CheckBox1->Checked=false;
   EClrShrift->Text=RGB_TO_STRING(255,255,128);
   fHeight->Text="13";
   cbNomer->Checked=true;
   cbVysota->Checked=true;
   cbTime->Checked=false;
   cbAzimutDalnost->Checked=true;
   cbVeroyatnostPervKanal->Checked=false;
   cbVeroyatnostVtorKanal->Checked=false;
   cbSkorost->Checked=false;

 //��������� �����
   if(CBPortName->Items->Count<1)
   {
        CBPortName->Text=String(LSTR("No port","��� �����") );
        CBPortName->ItemIndex=-1;
   }else{
        CBPortName->ItemIndex=0;
        CBPortName->Text=slComPortsList->Strings[0];
   }
   RGBaudrate->ItemIndex=6;

   CB_IsSaveSectorsToFile->Checked=false;


}
//---------------------------------------------------------------------------













