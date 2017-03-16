//---------------------------------------------------------------------------

#ifndef optionsH
#define optionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Dialogs.hpp>
#include "structs.h"
#include <Graphics.hpp>

/* перевод из строки в параметр и наоборот */
#define SET_STYLE_STR_TO_PAR(Strka, Par)      \
   Strka=Trim(Strka);     \
   if(Strka==String("PS_SOLID"))Par=PS_SOLID;    \
   else                                          \
   if(Strka==String("PS_DASH"))Par=PS_DASH;      \
   else                                          \
   if(Strka==String("PS_DOT"))Par=PS_DOT;        \
   else                                          \
   if(Strka==String("PS_DASHDOT"))Par=PS_DASHDOT; \
   else \
   if(Strka==String("PS_DASHDOTDOT"))Par=PS_DASHDOTDOT; \
   else  \
   if(Strka==String("PS_NULL"))Par=PS_NULL;  \
   else \
   if(Strka==String("PS_INSIDEFRAME"))Par=PS_INSIDEFRAME;


#define SET_STYLE_PAR_TO_STR(Par, Strka)      \
   switch(Par)                                \
   {                                          \
     case PS_SOLID: Strka=String("PS_SOLID"); break; \
     case PS_DASH: Strka=String("PS_DASH"); break; \
     case PS_DOT: Strka=String("PS_DOT"); break;   \
     case PS_DASHDOT: Strka=String("PS_DASHDOT"); break; \
     case PS_DASHDOTDOT: Strka=String("PS_DASHDOTDOT"); break; \
     case PS_NULL: Strka=String("PS_NULL"); break; \
     case PS_INSIDEFRAME: Strka=String("PS_INSIDEFRAME"); break; \
     default: \
       Strka=String("PS_SOLID"); break; \
   };

// заливка - перевод из строки и обратно
#define SET_STYLE_STR_TO_PAR_BRUSH(Strka, Par)      \
   Strka=Trim(Strka);     \
   if(Strka==String("BS_DIBPATTERN"))Par=BS_DIBPATTERN;    \
   else                                          \
   if(Strka==String("BS_DIBPATTERN8X8"))Par=BS_DIBPATTERN8X8;      \
   else                                          \
   if(Strka==String("BS_DIBPATTERNPT"))Par=BS_DIBPATTERNPT;        \
   else                                          \
   if(Strka==String("BS_HATCHED"))Par=BS_HATCHED; \
   else \
   if(Strka==String("BS_HOLLOW"))Par=BS_HOLLOW; \
   else \
   if(Strka==String("BS_PATTERN"))Par=BS_PATTERN; \
   else \
   if(Strka==String("BS_PATTERN8X8"))Par=BS_PATTERN8X8; \
   else \
   if(Strka==String("BS_SOLID"))Par=BS_SOLID;

// заливка - перевод из параметра в строку
#define SET_STYLE_PAR_TO_STR_BRUSH(Par, Strka)      \
   switch(Par)                                \
   {                                          \
     case BS_DIBPATTERN	: Strka=String("BS_DIBPATTERN"); break; \
     case BS_HATCHED : Strka=String("BS_HATCHED"); break; \
     case BS_DIBPATTERNPT : Strka=String("BS_DIBPATTERNPT"); break;   \
     case BS_HOLLOW	: Strka=String("BS_HOLLOW"); break; \
     case BS_PATTERN : Strka=String("BS_PATTERN"); break; \
     case BS_PATTERN8X8	 : Strka=String("BS_PATTERN8X8"); break; \
     case BS_DIBPATTERN8X8 : Strka=String("BS_DIBPATTERN8X8"); break; \
     case BS_SOLID : Strka=String("BS_SOLID"); break; \
     default: \
       Strka=String("BS_SOLID"); break; \
   };

//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TColorDialog *ColorDialog1;
    TGroupBox *GroupBox3;
    TLabel *Label7;
    TLabel *Label8;
    TEdit *EClrcircle1;
    TShape *SClrcircle1;
    TLabel *Label11;
    TEdit *WidthCir1;
    TUpDown *UpDown3;
    TUpDown *UpDown4;
    TGroupBox *GroupBox4;
    TLabel *Label13;
    TLabel *Label14;
    TShape *SClrcircle2;
    TLabel *Label17;
    TEdit *EClrcircle2;
    TEdit *WidthCir2;
    TUpDown *UpDown5;
    TUpDown *UpDown6;
    TGroupBox *GroupBox5;
    TLabel *Label19;
    TLabel *Label20;
    TShape *SClrcircle3;
    TLabel *Label23;
    TEdit *EClrcircle3;
    TEdit *RadCir3;
    TEdit *WidthCir3;
    TUpDown *UpDown7;
    TUpDown *UpDown8;
    TTabSheet *TabSheet3;
    TGroupBox *GroupBox6;
    TLabel *Label25;
    TLabel *Label26;
    TEdit *EClrShrift;
    TShape *SClrShrift;
    TEdit *SizeShrift;
    TUpDown *UpDown9;
        TTabSheet *TabSheet4;
        TRadioGroup *RGBaudrate;
        TTabSheet *TabSheet5;
        TGroupBox *GroupBox8;
        TEdit *EFullPathToData;
        TPanel *Panel1;
        TButton *Button2;
        TCheckBox *CB_IsSaveDataFile;
        TCheckBox *CB_IsFilterDataToFile;
        TCheckBox *CB_IsSaveSectorsToFile;
        TGroupBox *gbDataOutOfFormulyar;
        TCheckBox *cbNomer;
        TCheckBox *cbVysota;
        TCheckBox *cbOstatokTopliva;
        TCheckBox *cbTime;
        TCheckBox *cbVeroyatnostPervKanal;
        TCheckBox *cbSkorost;
        TCheckBox *cbAzimutDalnost;
        TCheckBox *cbVeroyatnostVtorKanal;
        TLabel *Label38;
        TEdit *fWidth;
        TEdit *fHeight;
        TUpDown *UpDown16;
        TUpDown *UpDown17;
        TCheckBox *cbFormulyar;
        TStaticText *StaticText2;
        TGroupBox *GroupBox7;
        TCheckBox *CheckBox2;
        TLabel *Label29;
        TEdit *Edit7;
        TUpDown *UpDown22;
        TPageControl *PageControl2;
        TTabSheet *TabSheet6;
        TTabSheet *TabSheet7;
        TGroupBox *GroupBox10;
        TLabel *Label27;
        TShape *SClrcircle;
        TLabel *Label28;
        TLabel *Label39;
        TEdit *EClrcircle;
        TEdit *Edit9;
        TUpDown *UpDown10;
        TComboBox *ComboBox7;
        TGroupBox *GroupBox11;
        TLabel *Label40;
        TLabel *Label41;
        TLabel *Label42;
        TShape *SDClrcircle;
        TComboBox *ComboBox8;
        TUpDown *UpDown11;
        TEdit *Edit10;
        TEdit *EDClrcircle;
        TGroupBox *GroupBox13;
        TLabel *Label46;
        TShape *SDLines;
        TLabel *Label47;
        TLabel *Label48;
        TEdit *EDLines;
        TEdit *Edit15;
        TUpDown *UpDown19;
        TComboBox *ComboBox10;
        TGroupBox *GroupBox12;
        TLabel *Label43;
        TShape *SLines;
        TLabel *Label44;
        TLabel *Label45;
        TEdit *ELines;
        TEdit *Edit13;
        TUpDown *UpDown18;
        TComboBox *ComboBox9;
        TGroupBox *GroupBox16;
        TLabel *LClrbackground;
        TShape *SClrbackground;
        TEdit *EClrbackground;
        TGroupBox *GroupBox15;
        TLabel *Label52;
        TShape *SScale;
        TLabel *Label53;
        TLabel *Label54;
        TEdit *EScale;
        TEdit *Edit19;
        TUpDown *UpDown21;
        TComboBox *ComboBox12;
        TImage *Image1;
        TImage *Image2;
        TImage *Image3;
        TCheckBox *CheckBox1;
        TEdit *RadCir1;
        TEdit *RadCir2;
        TLabel *Label3;
        TEdit *EDalnost;
        TShape *SDalnost;
        TLabel *Label1;
        TEdit *EAzimuth;
        TShape *SAzimuth;
        TLabel *Label2;
        TComboBox *ComboBox2;
        TLabel *Label4;
        TLabel *Label5;
        TButton *Button1;
        TButton *Button3;
        TGroupBox *GBPort;
        TComboBox *CBPortName;
        TLabel *Label6;
        TRadioGroup *RGChisloStopBits;
        TCheckBox *cbFParity;
        TRadioGroup *RGParityMode;
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall EMaxdalnostKeyPress(TObject *Sender, char &Key);
    void __fastcall EMaxdalnostChange(TObject *Sender);
    void __fastcall SClrbackgroundMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall EClrbackgroundKeyPress(TObject *Sender, char &Key);
    void __fastcall EClrbackgroundDblClick(TObject *Sender);
    void __fastcall EClrbackgroundChange(TObject *Sender);
    void __fastcall ComboBox1Exit(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall RadCir1KeyPress(TObject *Sender, char &Key);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Button3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm2(TComponent* Owner);

    initialization init;   //Файл настроек
    int iModified;          //Изменем ли файл
    TStringList *slComPortsList; //Список портов
    TStringList *slStatesList;   //Список состояний портов




    
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
