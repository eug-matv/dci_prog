//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <IniFiles.hpp>
#include <stdio.h>
#include "structs.h"
#include "WorkWithIniFiles.h"
#include "win_str_dir_utils.h"

//---------------------------------------------------------------------------

#define MIN(x,y)  ((x)<(y)?(x):(y))

#define SAVE_TO_INI(SOURCE,KEY, DATA) \
MemIniFile->WriteString(SOURCE, KEY,String(DATA));

#define LOAD_FROM_INI(SOURCE,KEY, DATA, DEFAULT) \
{ \
  AnsiString RetString;                                          \
  RetString=MemIniFile->ReadString(SOURCE, KEY, String(DEFAULT));   \
  DATA=RetString.ToIntDef(0);   \
}


#define SAVE_COLORREF_TO_INI(SOURCE, KEY, DATA) \
{                                             \
   char Strka[20];                            \
   sprintf(Strka,"%08X",(unsigned long)DATA); \
   MemIniFile->WriteString(SOURCE, KEY,Strka);   \
}



#define LOAD_COLORREF_FROM_INI(SOURCE,KEY, DATA, U1) \
{                                         \
  AnsiString RetString;                   \
  RetString=MemIniFile->ReadString(SOURCE, KEY, String("-1"));  \
  if(RetString==String("-1"))DATA=U1;                             \
  else{                                                   \
     RetString=String("0x")+RetString.Trim();              \
     DATA=RetString.ToIntDef(U1);                         \
  }                                                       \
}






//Дополнительные данные для работы INI



#pragma package(smart_init)

//Работа с ини-файлами
// переменная содержит путь к файлу помощи
AnsiString HelpFilePaht;


static TMemIniFile *MemIniFile=NULL;

// загрузка параметров формруляра из ини-файла
void Load_INI_f(struct initialization *init_struct,
                      char *PreSetupIni,
                      char *UserIni)
{
 char *lpszMemIniFileName;
 char Strka[20];
 long lRet;
 unsigned char Rp,Gp,Bp;
 AnsiString DataString;
 char tmp_com_port_name[200];
 int i,k;

//Получить текущую папке

  MemIniFile=new TMemIniFile(PreSetupIni);
  DataString=MemIniFile->ReadString("MAIN","TempObzora","4.58");
  lRet=sscanf(DataString.c_str(),"%f",&(init_struct->fTempObzora));
  if(lRet!=1)init_struct->fTempObzora=4.58;
  DataString=MemIniFile->ReadString("App","NameDllFileEvent","execctrl.dll").Trim();
  strcpy(init_struct->NameDllFileEvent,DataString.c_str());
  DataString=MemIniFile->ReadString("App","EventName","SecuriryEventName").Trim();
  strcpy(init_struct->SecuriryEventName,DataString.c_str());
  init_struct->SecurityEventControl=
     MemIniFile->ReadInteger("App","EventControl",0);


  LOAD_FROM_INI("MAIN","max_dalnost", init_struct->max_dalnost, "400");

//Загрузка данных о сохранении файлов
  DataString=MemIniFile->ReadString("MAIN", "csFullPathToData","C:\\DATA_FROM_PORT").Trim();
  strcpy(init_struct->csFullPathToData,DataString.c_str());
  LOAD_FROM_INI("MAIN", "IsSaveDataToFile",init_struct->IsSaveDataToFile,"1");

  LOAD_FROM_INI("MAIN", "abuse_if_none",init_struct->abuse_if_none,"1");


//Загрузка данных для привязки
  LOAD_FROM_INI("ESCOUT","sigma_D_m",init_struct->sigma_D_m,"50");
  LOAD_FROM_INI("ESCOUT","sigma_A_min",init_struct->sigma_A_min,"20");
  LOAD_FROM_INI("ESCOUT","strob_dekkart_m",init_struct->strob_dekkart_m,"400");
  DataString=MemIniFile->ReadString("ESCOUT","strob_azimuth_g","4");
  lRet=sscanf(DataString.c_str(),"%lf",&(init_struct->strob_azimuth_g));
  if(lRet!=1)init_struct->strob_azimuth_g=4;
  LOAD_FROM_INI("ESCOUT","strob_dalnost_m",init_struct->strob_dalnost_m,"400");
  LOAD_FROM_INI("ESCOUT","N_of_Potery",init_struct->N_of_Potery,"3");
  LOAD_FROM_INI("ESCOUT","MaxV_km_ch",init_struct->MaxV_km_ch,"1200");
  LOAD_FROM_INI("ESCOUT","MinV_km_ch",init_struct->MinV_km_ch,"100");
// Загрузка пути файла помощи
  HelpFilePaht=MemIniFile->ReadString("MAIN", "HelpFile","help.chm").Trim();

//Использовать ли COMDisable
   LOAD_FROM_INI("COMDISABLE","isUseCOMDisable",init_struct->isUseCOMDisable,"0");

//Получить имя файла COMDisable
  DataString=MemIniFile->ReadString("COMDISABLE","csFullCOMDisablePath","").Trim();
  strncpy(init_struct->csFullComdisablePath,
        DataString.c_str(),
        sizeof(init_struct->csFullComdisablePath)-1);

  init_struct->csFullComdisablePath[sizeof(init_struct->csFullComdisablePath)-1]=0;


//Добавлено 16.03.2017
//байт 15 , бит 7 для кодограммы УВД.
   LOAD_FROM_INI("PROTOCOLS","uvdFuelControlNByte",
        init_struct->uvdFuelControlNByte,"0");
   if(init_struct->uvdFuelControlNByte < 0)
      init_struct->uvdFuelControlNByte = 0;

   if(init_struct->uvdFuelControlNByte > 17)
      init_struct->uvdFuelControlNByte = 17;

   LOAD_FROM_INI("PROTOCOLS","uvdFuelControlNBit",
        init_struct->uvdFuelControlNBit,"7");
   init_struct->uvdFuelControlNBit %= 8;

   LOAD_FROM_INI("PROTOCOLS","uvdFuelControlValueOfTrue",
                init_struct->uvdFuelControlValueOfTrue,"0");
   init_struct->uvdFuelControlValueOfTrue %= 2; 


   LOAD_FROM_INI("PROTOCOLS","uvdAltitudeControlNByte",
        init_struct->uvdAltitudeControlNByte,"15");
   if(init_struct->uvdAltitudeControlNByte < 0)
      init_struct->uvdAltitudeControlNByte = 0;

   if(init_struct->uvdAltitudeControlNByte > 17)
      init_struct->uvdAltitudeControlNByte = 17;

   LOAD_FROM_INI("PROTOCOLS","uvdAltitudeControlNBit",
        init_struct->uvdAltitudeControlNBit,"7");
   init_struct->uvdAltitudeControlNBit %= 8;
   LOAD_FROM_INI("PROTOCOLS","uvdAltitudeControlValueOfTrue",
                init_struct->uvdAltitudeControlValueOfTrue,"0");
   init_struct->uvdAltitudeControlValueOfTrue %= 2;


  delete MemIniFile;

//Получить текущую папке

 MemIniFile=new TMemIniFile(UserIni);

 // параметры азимутально-дальномерной шкалы
 // параметры азимутально-дальномерной шкалы
 LOAD_COLORREF_FROM_INI("ADSH","Clrbackground", init_struct->Clrbackground, RGB(0,0,0)); //черный
 LOAD_COLORREF_FROM_INI("ADSH","Circle_lopnColor", init_struct->Circle.lopnColor, RGB(0x0,0xc8,0x0)); //светло-зеленый
 LOAD_COLORREF_FROM_INI("ADSH","DCircle_lopnColor", init_struct->DCircle.lopnColor, RGB(0x0,0x96,0x0)); //зеленый
 LOAD_COLORREF_FROM_INI("ADSH","Lines_lopnColor", init_struct->Lines.lopnColor, RGB(0x0,0x64,0x0));   //темно-зеленый
 LOAD_COLORREF_FROM_INI("ADSH","DLines_lopnColor", init_struct->DLines.lopnColor, RGB(0x0,0x0,0xA4)); //
 LOAD_COLORREF_FROM_INI("ADSH","Razvertka_lopnColor", init_struct->Razvertka.lopnColor, RGB(0xff,0x0,0x0));  //красный
 LOAD_COLORREF_FROM_INI("ADSH","Scale_lopnColor", init_struct->Scale.lopnColor, RGB(0xe8,0xe8,0x0));  //желтый
 LOAD_FROM_INI("ADSH","Circle_lopnStyle", init_struct->Circle.lopnStyle, "0");
 LOAD_FROM_INI("ADSH","DCircle_lopnStyle", init_struct->DCircle.lopnStyle, "0");
 LOAD_FROM_INI("ADSH","Lines_lopnStyle", init_struct->Lines.lopnStyle, "0");
 LOAD_FROM_INI("ADSH","DLines_lopnStyle", init_struct->DLines.lopnStyle, "0");
 LOAD_FROM_INI("ADSH","Razvertka_lopnStyle", init_struct->Razvertka.lopnStyle, "0");
 LOAD_FROM_INI("ADSH","Scale_lopnStyle", init_struct->Scale.lopnStyle, "1");
 LOAD_FROM_INI("ADSH","Circle_lopnWidth", init_struct->Circle.lopnWidth.x=init_struct->Circle.lopnWidth.y, "1");
 LOAD_FROM_INI("ADSH","DCircle_lopnWidth", init_struct->DCircle.lopnWidth.x=init_struct->DCircle.lopnWidth.y, "1");
 LOAD_FROM_INI("ADSH","Lines_lopnWidth", init_struct->Lines.lopnWidth.x=init_struct->Lines.lopnWidth.y, "1");
 LOAD_FROM_INI("ADSH","DLines_lopnWidth", init_struct->DLines.lopnWidth.x=init_struct->DLines.lopnWidth.y, "1");

 LOAD_FROM_INI("ADSH","Razvertka_lopnWidth", init_struct->Razvertka.lopnWidth.x=init_struct->Razvertka.lopnWidth.y, "2");
 LOAD_FROM_INI("ADSH","Scale_lopnWidth", init_struct->Scale.lopnWidth.x=init_struct->Scale.lopnWidth.y, "1");

 LOAD_FROM_INI("ADSH","azimuth_delta", init_struct->azimuth_delta, "30");
 LOAD_COLORREF_FROM_INI("ADSH","cAzimuthText", init_struct->cAzimuthText, RGB(0x50,0x70,0x80));
 LOAD_COLORREF_FROM_INI("ADSH","cDalnostText", init_struct->cDalnostText, RGB(0x50,0x70,0x80));


 // Параметры отображения плотов
 LOAD_FROM_INI("PLOTS","radius1", init_struct->radius1, "3");
 init_struct->CirclePen1.lopnStyle=PS_SOLID;
 LOAD_FROM_INI("PLOTS","CirclePen1_lopnWidth", init_struct->CirclePen1.lopnWidth.x=init_struct->CirclePen1.lopnWidth.y, "1");
 LOAD_COLORREF_FROM_INI("PLOTS","CirclePen1_lopnColor", init_struct->CirclePen1.lopnColor, RGB(0xe8,0x64,0x08));// оранжевый
 init_struct->CircleBrush1.lbStyle=BS_SOLID;
 Rp=MIN(2*GetRValue(init_struct->CirclePen1.lopnColor),255);
 Gp=MIN(2*GetGValue(init_struct->CirclePen1.lopnColor),255);
 Bp=MIN(2*GetBValue(init_struct->CirclePen1.lopnColor),255);
 init_struct->CircleBrush1.lbColor=RGB(Rp,Gp,Bp);

 LOAD_FROM_INI("PLOTS","radius2", init_struct->radius2, "5");
 init_struct->CirclePen2.lopnStyle=PS_SOLID;
 LOAD_FROM_INI("PLOTS","CirclePen2_lopnWidth", init_struct->CirclePen2.lopnWidth.x=init_struct->CirclePen2.lopnWidth.y, "3");
 LOAD_COLORREF_FROM_INI("PLOTS","CirclePen2_lopnColor", init_struct->CirclePen2.lopnColor, RGB(0xe1,0xe1,0x0));// темно-желтый

 LOAD_FROM_INI("PLOTS","radius3", init_struct->radius3, "4");
 init_struct->CirclePen3.lopnStyle=PS_SOLID;
 LOAD_FROM_INI("PLOTS","CirclePen3_lopnWidth", init_struct->CirclePen3.lopnWidth.x=init_struct->CirclePen3.lopnWidth.y, "1");
 LOAD_COLORREF_FROM_INI("PLOTS","CirclePen3_lopnColor", init_struct->CirclePen3.lopnColor, RGB(0xc1,0x48,0x20));    //коричневый
 init_struct->CircleBrush3.lbStyle=BS_SOLID;
 Rp=MIN(2*GetRValue(init_struct->CirclePen3.lopnColor),255);
 Gp=MIN(2*GetGValue(init_struct->CirclePen3.lopnColor),255);
 Bp=MIN(2*GetBValue(init_struct->CirclePen3.lopnColor),255);
 init_struct->CircleBrush3.lbColor=RGB(Rp,Gp,Bp);

 LOAD_FROM_INI("PLOTS","iTailLength", init_struct->iTailLength,"50");
 LOAD_FROM_INI("PLOTS","iTail", init_struct->iTail, "1");




// параметры формуляра
 LOAD_FROM_INI("FORMULAR","HeightFormul", init_struct->sVysot, "50");
 LOAD_FROM_INI("FORMULAR","WidthFormul", init_struct->sShir, "100");
 LOAD_FROM_INI("FORMULAR","SizeOfFormulText", init_struct->szTextOut, "10");
 LOAD_COLORREF_FROM_INI("FORMULAR","ColorOfFormulText", init_struct->cText, RGB(0xff,0xff,0x80)); //бледно-желтый
 LOAD_FROM_INI("FORMULAR","number_symbol_form", init_struct->number_symbol_form, "15");
 LOAD_FROM_INI ("FORMULAR","iRamkaFormulyar",init_struct->iRamkaFormulyar,"1");

//Добавлено Матвеенко
 LOAD_FROM_INI("FORMULAR","IsOutFormulyar", init_struct->IsOutFormulyar, "1");
 LOAD_FROM_INI("FORMULAR","IsOutNomer", init_struct->IsOutNomer, "1");
 LOAD_FROM_INI("FORMULAR","IsOutVysota", init_struct->IsOutVysota, "1");
 LOAD_FROM_INI("FORMULAR","IsOutTime", init_struct->IsOutTime, "1");
 LOAD_FROM_INI("FORMULAR","IsOutAzimutDalnost", init_struct->IsOutAzimutDalnost, "1");
 LOAD_FROM_INI("FORMULAR","IsOutSkorost", init_struct->IsOutSkorost, "0");
 LOAD_FROM_INI("FORMULAR","IsOutOstatokTopliva", init_struct->IsOutOstatokTopliva, "0");
 LOAD_FROM_INI("FORMULAR","IsOutVeroyatnostPervKanal", init_struct->IsOutVeroyatnostPervKanal, "0");
 LOAD_FROM_INI("FORMULAR","IsOutVeroyatnostVtorKanal", init_struct->IsOutVeroyatnostVtorKanal, "0");
// LOAD_FROM_INI("FORMULAR","IsOutSKOPervKanal", init_struct->IsOutSKOPervKanal, "0");
// LOAD_FROM_INI("FORMULAR","IsOutSKOVtorKanal", init_struct->IsOutSKOVtorKanal, "0");



 init_struct->N_of_Obzoror_for_prob1=30;
 init_struct->N_of_Obzoror_for_prob2=30;




//Загрузка параметров COM-порта
 DataString=MemIniFile->ReadString("COMPORT","ComN","COM1").Trim();
 strcpy(tmp_com_port_name,DataString.c_str());
 k=0;
 for(i=0;i<strlen(tmp_com_port_name);i++)
 {
        if(tmp_com_port_name[i]==' '||tmp_com_port_name[i]=='\t'||
           tmp_com_port_name[i]=='\\'||tmp_com_port_name[i]=='.')
        {
             k=i+1;
        }else{
             break;
        }
 }
 strcpy(init_struct->csComPortStr,tmp_com_port_name+k);


 LOAD_FROM_INI("COMPORT", "iBaudRate",init_struct->iBaudRate,"9600");
 LOAD_FROM_INI("COMPORT", "iStopBits", init_struct->iStopBits, "0");
 LOAD_FROM_INI("COMPORT", "fParity",init_struct->fParity,"0");
 LOAD_FROM_INI("COMPORT", "parityMode", init_struct->parityMode, "0");



 LOAD_FROM_INI("PLOTOUTS", "IsSaveSectorsToFile",init_struct->IsSaveSectorsToFile,"0");
 LOAD_FROM_INI("PLOTOUTS", "IsFilterDataToFile",init_struct->IsFilterDataToFile,"0");


//Сохраним в ини информацию о числе обзоров для оценки вероятностей
  LOAD_FROM_INI("VER","N_of_Obzoror_for_prob1",init_struct->N_of_Obzoror_for_prob1,"20");
  LOAD_FROM_INI("VER","N_of_Obzoror_for_prob2",init_struct->N_of_Obzoror_for_prob2,"20");


//Сохранения данных о привязки
/*  LOAD_FROM_INI("ESCOUT","IsSoprovojdenie",init_struct->IsSoprovojdenie,"0");*/
//2014.12.15 Отключим сопровождение. принудительно
  init_struct->IsSoprovojdenie=0;      



 delete MemIniFile;

}

// загрузка параметров формруляра из ини-файла
void Save_INI_f(struct initialization *init_struct,
                 char *UserIni)
{
 char* lpszMemIniFileName;
 char Strka[20];

 long lRet;

  MemIniFile=new TMemIniFile(UserIni);

   // запись в INI-файл параметров АДШ
  // Цвет
  SAVE_COLORREF_TO_INI("ADSH","Clrbackground", init_struct->Clrbackground);
  SAVE_COLORREF_TO_INI("ADSH","Circle_lopnColor", init_struct->Circle.lopnColor);
  SAVE_COLORREF_TO_INI("ADSH","Lines_lopnColor", init_struct->Lines.lopnColor);
  SAVE_COLORREF_TO_INI("ADSH","Razvertka_lopnColor", init_struct->Razvertka.lopnColor);
  SAVE_COLORREF_TO_INI("ADSH","Scale_lopnColor", init_struct->Scale.lopnColor);
  // Стиль
  SAVE_TO_INI("ADSH","Circle_lopnStyle", init_struct->Circle.lopnStyle);
  SAVE_TO_INI("ADSH","Lines_lopnStyle", init_struct->Lines.lopnStyle);
  SAVE_TO_INI("ADSH","Razvertka_lopnStyle", init_struct->Razvertka.lopnStyle);
  SAVE_TO_INI("ADSH","Scale_lopnStyle", init_struct->Scale.lopnStyle);
  // Толщина
  SAVE_TO_INI("ADSH","Circle_lopnWidth", init_struct->Circle.lopnWidth.x);
  SAVE_TO_INI("ADSH","Lines_lopnWidth", init_struct->Lines.lopnWidth.x);
  SAVE_TO_INI("ADSH","Razvertka_lopnWidth", init_struct->Razvertka.lopnWidth.x);
  SAVE_TO_INI("ADSH","Scale_lopnWidth", init_struct->Scale.lopnWidth.x);
  // Размеры
  SAVE_TO_INI("ADSH","azimuth_delta", init_struct->azimuth_delta);
  SAVE_COLORREF_TO_INI("ADSH","cAzimuthText", init_struct->cAzimuthText);
  SAVE_COLORREF_TO_INI("ADSH","cDalnostText", init_struct->cDalnostText);


  // Параметры отображения плотов
  SAVE_TO_INI("PLOTS","radius1", init_struct->radius1);
  SAVE_TO_INI("PLOTS","CirclePen1_lopnWidth", init_struct->CirclePen1.lopnWidth.x);
  SAVE_COLORREF_TO_INI("PLOTS","CirclePen1_lopnColor", init_struct->CirclePen1.lopnColor);

  SAVE_TO_INI("PLOTS","radius2", init_struct->radius2);
  SAVE_TO_INI("PLOTS","CirclePen2_lopnWidth", init_struct->CirclePen2.lopnWidth.x);
  SAVE_COLORREF_TO_INI("PLOTS","CirclePen2_lopnColor", init_struct->CirclePen2.lopnColor);

  SAVE_TO_INI("PLOTS","radius3", init_struct->radius3);
  SAVE_TO_INI("PLOTS","CirclePen3_lopnWidth", init_struct->CirclePen3.lopnWidth.x);
  SAVE_COLORREF_TO_INI("PLOTS","CirclePen3_lopnColor", init_struct->CirclePen3.lopnColor);

  
 // Параметры следа
  SAVE_TO_INI("PLOTS","iTailLength", init_struct->iTailLength );
  SAVE_TO_INI("PLOTS","iTail", init_struct->iTail);


  // параметры формуляра
  SAVE_TO_INI("FORMULAR","HeightFormul", init_struct->sVysot);
  SAVE_TO_INI("FORMULAR","WidthFormul",  init_struct->sShir);
  SAVE_TO_INI("FORMULAR","SizeOfFormulText", init_struct->szTextOut);
  SAVE_COLORREF_TO_INI("FORMULAR","ColorOfFormulText", init_struct->cText);

//Добавлено матвеенко
  SAVE_TO_INI("FORMULAR","IsOutFormulyar", init_struct->IsOutFormulyar);
  SAVE_TO_INI("FORMULAR","IsOutNomer", init_struct->IsOutNomer);
  SAVE_TO_INI("FORMULAR","IsOutVysota", init_struct->IsOutVysota);
  SAVE_TO_INI("FORMULAR","IsOutTime", init_struct->IsOutTime);
  SAVE_TO_INI("FORMULAR","IsOutAzimutDalnost", init_struct->IsOutAzimutDalnost);
  SAVE_TO_INI("FORMULAR","IsOutSkorost", init_struct->IsOutSkorost);
  SAVE_TO_INI("FORMULAR","IsOutOstatokTopliva", init_struct->IsOutOstatokTopliva);
  SAVE_TO_INI("FORMULAR","IsOutVeroyatnostPervKanal", init_struct->IsOutVeroyatnostPervKanal);
  SAVE_TO_INI("FORMULAR","IsOutVeroyatnostVtorKanal", init_struct->IsOutVeroyatnostVtorKanal);
  SAVE_TO_INI ("FORMULAR","iRamkaFormulyar",init_struct->iRamkaFormulyar);

  //  SAVE_TO_INI("FORMULAR","IsOutSKOPervKanal", init_struct->IsOutSKOPervKanal);
//  SAVE_TO_INI("FORMULAR","IsOutSKOVtorKanal", init_struct->IsOutSKOVtorKanal);


//Сохраним данные о настройках COM-порта
  MemIniFile->WriteString("COMPORT","ComN",init_struct->csComPortStr);
  SAVE_TO_INI("COMPORT", "iBaudRate",init_struct->iBaudRate);
  SAVE_TO_INI("COMPORT", "iStopBits",init_struct->iStopBits);
  SAVE_TO_INI("COMPORT", "fParity",init_struct->fParity);
  SAVE_TO_INI("COMPORT", "parityMode", init_struct->parityMode);



//Сохраним данные о настроках сохранения в файле
  SAVE_TO_INI("PLOTOUTS", "IsSaveSectorsToFile",init_struct->IsSaveSectorsToFile);
  SAVE_TO_INI("PLOTOUTS", "IsFilterDataToFile",init_struct->IsFilterDataToFile);

//Сохраним в ини информацию о числе обзоров для оценки вероятностей
  SAVE_TO_INI("VER","N_of_Obzoror_for_prob1",init_struct->N_of_Obzoror_for_prob1);
  SAVE_TO_INI("VER","N_of_Obzoror_for_prob2",init_struct->N_of_Obzoror_for_prob2);


//Сохранения данных о привязки
  SAVE_TO_INI("ESCOUT","IsSoprovojdenie",init_struct->IsSoprovojdenie);

  MemIniFile->UpdateFile();
  delete MemIniFile;
}


/*Сохранить свойства о формуляре в файл*/
void Save_INI_f_VisibleFormulyar(
   struct initialization *init_struct,
                 char *UserIni)
{
  MemIniFile=new TMemIniFile(UserIni);
  SAVE_TO_INI("FORMULAR","IsOutFormulyar", init_struct->IsOutFormulyar);
  MemIniFile->UpdateFile();
  delete MemIniFile;

}

/*СОхранить в файл данные об отображении следа*/
void Save_INI_f_VisibleTail(
   struct initialization *init_struct,
                 char *UserIni)
{
  MemIniFile=new TMemIniFile(UserIni);
  SAVE_TO_INI("PLOTS","iTail", init_struct->iTail);
  MemIniFile->UpdateFile();
  delete MemIniFile;
}                 



