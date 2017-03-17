//---------------------------------------------------------------------------
/*Читать paketstruct.h*/

#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include <stdlib.h>
#include "paketstruct.h"
#include "comreadthread.h"
#include "sopr_doptools.h"

#pragma package(smart_init)

int TComReadThread::iErrorConnect=0; //Код ошибки при выполнении функции Connect
int TComReadThread::iGetLastError=0; //Код последней ошибки





//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void _fastcall TComReadThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TComReadThread::TComReadThread(const initialization &_init)
        : TThread(false)
{
  memcpy(&init, &_init, sizeof(_init));
  FreeOnTerminate=true;   //Освобождаем при записи
  dwError=0;
}
//---------------------------------------------------------------------------



void __fastcall TComReadThread::Execute()
{
     SYSTEMTIME lp; //Для вычисления системного времени

     bool Ret,WasPaket;
     DWORD j=0;// Переменная, количество байт считанных с СОМ порта
     BYTE b;   // Переменная в которую считываются побайтно данные с СОМ порта
     bool start=false; //не начато чтение данных из пакета
     static BYTE paket[255];
     BYTE tmp;
     bool RetB;
     DWORD lEr=0;
     COMSTAT Comstat;
     TPaketData *PD=TPaketData::PaketData;
     DWORD dwError1;
     int Indx;    //Индекс
     int TI,N_of_TI=0;      //Рассматриваемый индекс
     int N_of_TI1;          //Рассчетное значение
     int N_of_Bytes;
     int IsUVD, IsRBS;          //УВД или RBS
     PurgeComm(hCom,PURGE_RXCLEAR); //Очистим входной буфер
     N_of_P=N_of_U=N_of_R=0;
  while(1)
  {
     start=false;
     dwError1=0;
     j=0;
     lEr=0;
     if(Terminated)goto End_while;
     WasPaket=false;    //Пакет не выявлен
     do{  // цикл считывания данных с СОМ порта
       ZeroMemory(&Comstat, sizeof(COMSTAT));     //Обнуление структуры Comstat
       lEr=0;
       RetB=ClearCommError(hCom,&lEr, &Comstat);  //возвращает false если прозошла ошибка считывания
       if(lEr&CE_RXOVER)
       {
//Переполнение
          dwError|=ERR_PEREPOLENIE_BUFFERA;
       }
       if(!RetB)
       {
          dwError|=ERR_UNKNOWN_CLEARCOMERROR;
       }
       if(Terminated)goto End_while;
       RetB=ReadFile(hCom,&b,1,&j,NULL);             // Читаем один байт данных из СОМ порта
       if(Terminated)goto End_while;
       if(RetB&&j!=0){                                // Буфер не был пуст и данные считали.
            if(b==0x10)
            {                        // проверка DLE

//Установим время получения начала пакета - вдруг это наш пакет
                GetLocalTime(&lp);
                if(Terminated)goto End_while;   //Проверка заверщения вторичного потока
                RetB=ReadFile(hCom,&b,1,&j,NULL);
                if(Terminated)goto End_while;
                if(RetB&&j!=0)
                    if(b==0x02)start=true;
            }

            if(Terminated)goto End_while;

                      //c этого момента начало пакета найдено
            if(start==true)
            {                   // Если начало пакета найдено
                int i=0;
                bool l= false;
                while(1){
                    if(Terminated)goto End_while;
                    RetB=ReadFile(hCom,&b,1,&j,NULL);
                    if(RetB&&j!=0){
                        if(b==0x10){                //Проверка условия конца пакета DLE=0x10
                            RetB=ReadFile(hCom,&b,1,&j,NULL);
                            if(RetB&&j!=0){

//Проверка, что новый элемент имеет значение 0x03 и поле длины указывает верную длину
                               if(b==0x03&&i>=9&&paket[1]==i){        //ETX = 0x03
//Проверим длину пакета. Может быть это середина пакета - просто 0x03
//Проверим длину пакета
                                     start = false;
                                     WasPaket=true;
                                     N_of_Bytes=i;
                                     goto loop;
                                }else{
                                     if(i>=250)break;
                                     paket[i]=0x10;
                                     i++;
                                     if(b==0x10)continue;
                                     break;
                                }
                            }
                        }
                    }
                    if(i>=250)break;
                    paket[i]=b;
                    i++;
                }
            }
       }// конец if(j!=0)
     }while(j!=0);//Читаем данные пока буфер СОМ порта не пуст    *******
// с этого момента пакет получен осталось распознать его
loop:
     if(Terminated)goto End_while;
     if(!WasPaket)continue;
     Indx=2;         //Начнем читать со второго байта
     TI=PD->InIndex;
     N_of_TI=0;
     while(Indx<N_of_Bytes)
     {
        switch (paket[0]){  //критерий распознования пакета его категория, первый байт пакета не счтая заголовочного
         case 0x01:
           if(paket[Indx+3]==0x10){
//Первичный канал
             PD[TI].TipPaketa=0;
             PD[TI].Otmetka.Rej=0;
             PD[TI].cKodStrany=paket[Indx+1];

             PD[TI].Otmetka.Kanal=0;
             PD[TI].Otmetka.bP_LCHM=true;
             PD[TI].Otmetka.bP_Mono=false;
             PD[TI].Otmetka.X = (long)((short)(paket[Indx+4]<<8 | paket[Indx+5]))*15;
             PD[TI].Otmetka.Y = (long)((short)(paket[Indx+6]<<8 | paket[Indx+7]))*15;
             PD[TI].Otmetka.lNomer=99999;
             PD[TI].Otmetka.lVysota=1999;

             PD[TI].DateTime.God=(short)(lp.wYear);
             PD[TI].DateTime.Mesyac=(short)(lp.wMonth);
             PD[TI].DateTime.Den=(short)(lp.wDay);
             PD[TI].DateTime.Time=(double)(lp.wHour+lp.wMinute/60.0+lp.wSecond/3600.0+lp.wMilliseconds/3600000.0);
             PD[TI].Otmetka.iTail=0;
             PD[TI].Otmetka.iFormulyar=0;
             PD[TI].Otmetka.apl=NULL;
             doptools_GetAzimutDalnostFromXY(
              PD[TI].Otmetka.X,
              PD[TI].Otmetka.Y,
              PD[TI].Azimut,
              PD[TI].Otmetka.Dalnost);
             PD[TI].Otmetka.lDopNumber=0;   //Нулевой номер
             PD[TI].Otmetka.lNomerTrassy=0;
             PD[TI].Otmetka.PrivyazanPredvaritelno=0;
             PD[TI].Otmetka.N_of_MustPO=PD[TI].Otmetka.N_of_MustSO=
             PD[TI].Otmetka.N_of_PO=PD[TI].Otmetka.N_of_SO=0;
             PD[TI].NomerSektoraRazmerom30=(int)(PD[TI].Azimut)/30;
             N_of_P++;
             PD[TI].Otmetka.IndexInObzor=N_of_P;
             TI=(TI+1)%N_OF_FIFO_PAK_STR;
             N_of_TI++;
             Indx+=10;
          }else{  //if(paket[5]==0x10)
//Проверка того, что это УВД
              IsUVD=0;

              if(N_of_Bytes==19)
              {
                 IsUVD=1;
              }else
              if(paket[Indx]==0x26)
              {
/*                 if(N_of_Bytes-Indx>17+1)
                 {
                    if(paket[Indx+17]==paket[Indx]%0xE0||
                       paket[Indx+18]==paket[Indx+1]%0xF0)
                    {
                       IsUVD=1;
                    }
                 }else{
                    if(N_of_Bytes-Indx==17)
                    {
                       IsUVD=1;
                    }
                 }
                 */
                 IsUVD=1;
              }
              if(IsUVD){

                  PD[TI].TipPaketa=0;
                  PD[TI].Otmetka.Rej=1;
                  PD[TI].Otmetka.bKodN=(bool)(paket[Indx]& 0x04);
                  PD[TI].Otmetka.bKodH=(bool)(paket[Indx]& 0x02);
                  PD[TI].cKodStrany=paket[Indx+1];
                  if((paket[Indx+3]&0x30)==0x30)
                 {
                       PD[TI].Otmetka.Kanal=2;
                 }else{
                       PD[TI].Otmetka.Kanal=1;
                 };
                 PD[TI].Otmetka.bKO=(bool)(paket[Indx+3] &0x02);
                 PD[TI].Otmetka.sKodB=(bool)(paket[Indx+15]& 0x40);
                 PD[TI].Otmetka.X = (long)((short)(paket[Indx+5]<<8 | paket[Indx+6]))*15;
                 PD[TI].Otmetka.Y = (long)((short)(paket[Indx+7]<<8 | paket[Indx+8]))*15;



//Проверим достоверность определения номера.
//параметры заданы в init.
//init.uvdNumberControlNByte - номер байта (считая с 1) в инф.части пакета.
//Если равен 0, то в пакете не ишется информация о достоверности - она считается
//достоверной. init.uvdNumberControlNBit - номер бита от 0 до 7.
//init.uvdNumberControlValueOfTrue - значение бита, которое соответствует
//достоверной информации о высоте, равно 0 или 1
                 if(init.uvdNumberControlNByte > 0){
                        unsigned char fg =
                 (paket[Indx+init.uvdNumberControlNByte-1] &
              (1<<init.uvdNumberControlNBit))>>init.uvdNumberControlNBit;
                        PD[TI].Otmetka.bNomer =
              (((int)fg) == init.uvdNumberControlValueOfTrue);
                 }else{
                        PD[TI].Otmetka.bNomer = true;
                 }



                 PD[TI].Otmetka.lNomer=((paket[Indx+11])&0x0F)*10000+
                 ((paket[Indx+12])>>4)*1000+((paket[Indx+12])&0x0F)*100+
                 ((paket[Indx+13])>>4)*10+((paket[Indx+13])&0x0F);
                 PD[TI].Otmetka.bAbsVysota=(bool)(paket[Indx+15]&0x80);
                 if((bool)(paket[Indx+15]&0x20))
                 {
                 //Дополнить единицами старший байт до inta
                        unsigned char tmp_uc[4]= {paket[Indx+16],
                          (paket[Indx+15])|0xc0, 0xFF,0xFF};
                        PD[TI].Otmetka.lVysota=(long)(*((int*)(tmp_uc)));
                 }else{
                     PD[TI].Otmetka.lVysota=(long)((((unsigned short)paket[Indx+15]&0x1F)<<8 | paket[Indx+16]));

                }
                 PD[TI].Otmetka.lVysota*=10;

//Проверим достоверность определения остатка топлива.
//параметры заданы в init.
//init.uvdFuelControlNByte - номер байта (считая с 1) в инф.части пакета.
//Если равен 0, то в пакете не ишется информация о достоверности - она считается
//достоверной. init.uvdFuelControlNBit - номер бита от 0 до 7.
//init.uvdFuelControlValueOfTrue - значение бита, которое соответствует
//достоверной информации об остатке топлива, равно 0 или 1
                 if(init.uvdFuelControlNByte > 0){
                        unsigned char fg =
                 (paket[Indx+init.uvdFuelControlNByte-1] &
              (1<<init.uvdFuelControlNBit))>>init.uvdFuelControlNBit;
                        PD[TI].Otmetka.bOstatokTopliva=
              (((int)fg) == init.uvdFuelControlValueOfTrue);
                 }else{
                        PD[TI].Otmetka.bOstatokTopliva = true;
                 }

//Проверим достоверность определения высоты.
//параметры заданы в init.
//init.uvdAltitudeControlNByte - номер байта (считая с 1) в инф.части пакета.
//Если равен 0, то в пакете не ишется информация о достоверности - она считается
//достоверной. init.uvdAltitudeControlNBit - номер бита от 0 до 7.
//init.uvdAltitudeControlValueOfTrue - значение бита, которое соответствует
//достоверной информации о высоте, равно 0 или 1
                 if(init.uvdAltitudeControlNByte > 0){
                        unsigned char fg =
                 (paket[Indx+init.uvdAltitudeControlNByte-1] &
              (1<<init.uvdAltitudeControlNBit))>>init.uvdAltitudeControlNBit;
                        PD[TI].Otmetka.bVysota =
              (((int)fg) == init.uvdAltitudeControlValueOfTrue);
                 }else{
                        PD[TI].Otmetka.bVysota = true;
                 }

                 PD[TI].Otmetka.lOstatokTopliva=(long)(paket[Indx+14]&0x0F);
                 PD[TI].Otmetka.lOstatokTopliva*=5;
                 if(PD[TI].Otmetka.lOstatokTopliva>100)PD[TI].Otmetka.lOstatokTopliva=100;
                 PD[TI].DateTime.God=(short)(lp.wYear);
                 PD[TI].DateTime.Mesyac=(short)(lp.wMonth);
                 PD[TI].DateTime.Den=(short)(lp.wDay);
                 PD[TI].DateTime.Time=(double)(lp.wHour+lp.wMinute/60.0+lp.wSecond/3600.0+lp.wMilliseconds/3600000.0);
                 PD[TI].Otmetka.iTail=1;
                 PD[TI].Otmetka.iFormulyar=1;
                 PD[TI].Otmetka.apl=NULL;
             doptools_GetAzimutDalnostFromXY(
                  PD[TI].Otmetka.X,
                   PD[TI].Otmetka.Y,
                  PD[TI].Azimut,
                  PD[TI].Otmetka.Dalnost);
                  PD[TI].Otmetka.PrivyazanPredvaritelno=0;
                 PD[TI].NomerSektoraRazmerom30=(int)(PD[TI].Azimut)/30;
                 PD[TI].Otmetka.lDopNumber=PD[TI].Otmetka.lNomer;
                 PD[TI].Otmetka.lNomerTrassy=0+PD[TI].Otmetka.lNomer;
                 N_of_U++;
                 PD[TI].Otmetka.IndexInObzor=N_of_U;
                 TI=(TI+1)%N_OF_FIFO_PAK_STR;
                 PD[TI].Otmetka.N_of_MustPO=PD[TI].Otmetka.N_of_MustSO=
                 PD[TI].Otmetka.N_of_PO=PD[TI].Otmetka.N_of_SO=0;
                 N_of_TI++;
                 Indx+=17;
                 continue;
             }

             IsRBS=0;
             if(N_of_Bytes==17)
             {
                 IsRBS=1;
             }else
             if(paket[Indx]==0x38)
             {
             /*
                if(N_of_Bytes-Indx>15+1)
                {
                    if(paket[Indx+15]==(paket[Indx]||
                       paket[Indx+16]%0xF0==(paket[Indx+1]%0xF0))
                    {
                       IsRBS=1;
                    }
               }else{
                   if(N_of_Bytes-Indx==15)
                   {
                       IsRBS=1;
                   }
               }
               */
               IsRBS=1;
            }

             if(IsRBS)
             {
                PD[TI].TipPaketa=0;
                PD[TI].Otmetka.Rej=2;
                PD[TI].Otmetka.bKodN=(bool)(paket[Indx]& 0x10);
                PD[TI].Otmetka.bKodH=(bool)(paket[Indx]& 0x08);
                PD[TI].cKodStrany=paket[Indx+1];
                if((paket[Indx+3]&0x30)==0x30){
                    PD[TI].Otmetka.Kanal=2;
                }else{
                     PD[TI].Otmetka.Kanal=1;
                };
                PD[TI].Otmetka.bKO=(bool)(paket[Indx+3] &0x02);
                switch(paket[Indx+4]>>5){
                   case 0x00:PD[TI].Otmetka.sKodB=0;break;
                   case 0x01:PD[TI].Otmetka.sKodB=7500;break;
                   case 0x02:PD[TI].Otmetka.sKodB=7600;break;
                   case 0x03:PD[TI].Otmetka.sKodB=7700;break;
                }
                PD[TI].Otmetka.X = (long)((short)(paket[Indx+5]<<8 | paket[Indx+6]))*15;
                PD[TI].Otmetka.Y = (long)((short)(paket[Indx+7]<<8 | paket[Indx+8]))*15;


//Проверим достоверность определения номера.
//параметры заданы в init.
//init.rbsNumberControlNByte - номер байта (считая с 1) в инф.части пакета.
//Если равен 0, то в пакете не ишется информация о достоверности - она считается
//достоверной. init.rbsNumberControlNBit - номер бита от 0 до 7.
//init.rbsNumberControlValueOfTrue - значение бита, которое соответствует
//достоверной информации о высоте, равно 0 или 1
                 if(init.rbsNumberControlNByte > 0){
                        unsigned char fg =
                 (paket[Indx+init.rbsNumberControlNByte-1] &
              (1<<init.rbsNumberControlNBit))>>init.rbsNumberControlNBit;
                        PD[TI].Otmetka.bNomer=
              (((int)fg) == init.rbsNumberControlValueOfTrue);
                 }else{
                        PD[TI].Otmetka.bNomer = true;
                 }




                PD[TI].Otmetka.lNomer=
(((((long)paket[Indx+11])&0x00000008)>>3)*4+((((long)paket[Indx+11])&0x00000004)>>2)*2+((((long)paket[Indx+11])&0x00000002)>>1))*1000+
(((((long)paket[Indx+11])&0x00000001))*4   +((((long)paket[Indx+12])&0x00000080)>>7)*2+((((long)paket[Indx+12])&0x00000040)>>6))*100+
(((((long)paket[Indx+12])&0x00000020)>>5)*4+((((long)paket[Indx+12])&0x00000010)>>4)*2+((((long)paket[Indx+12])&0x00000008)>>3))*10+
(((((long)paket[Indx+12])&0x00000004)>>2)*4+(((paket[Indx+12])&0x00000002)>>1)*2+ (((long)paket[Indx+12])&0x00000001));

//Проверим достоверность определения высоты.
//параметры заданы в init.
//init.rbsAltitudeControlNByte - номер байта (считая с 1) в инф.части пакета.
//Если равен 0, то в пакете не ишется информация о достоверности - она считается
//достоверной. init.rbsAltitudeControlNBit - номер бита от 0 до 7.
//init.rbsAltitudeControlValueOfTrue - значение бита, которое соответствует
//достоверной информации о высоте, равно 0 или 1
                 if(init.rbsAltitudeControlNByte > 0){
                        unsigned char fg =
                 (paket[Indx+init.rbsAltitudeControlNByte-1] &
              (1<<init.rbsAltitudeControlNBit))>>init.rbsAltitudeControlNBit;
                        PD[TI].Otmetka.bVysota=
              (((int)fg) == init.rbsAltitudeControlValueOfTrue);
                 }else{
                        PD[TI].Otmetka.bVysota = true;
                 }

                PD[TI].Otmetka.bAbsVysota=true;
                if(paket[Indx+13]&0x20)
                {
                        unsigned char tmp_uc[4]= {paket[Indx+14],
                        (paket[Indx+13])|0xc0,0xFF, 0xFF};
                        PD[TI].Otmetka.lVysota=(long)(*((int*)(tmp_uc)));
                }else{
                     PD[TI].Otmetka.lVysota=(long)((((unsigned short)paket[Indx+13]&0x1F)<<8 | paket[Indx+14]));
                }
                PD[TI].Otmetka.lVysota*=10;
                PD[TI].Otmetka.lOstatokTopliva=0;
                PD[TI].DateTime.God=(short)(lp.wYear);
                PD[TI].DateTime.Mesyac=(short)(lp.wMonth);
                PD[TI].DateTime.Den=(short)(lp.wDay);
                PD[TI].DateTime.Time=(double)(lp.wHour+lp.wMinute/60.0+lp.wSecond/3600.0+lp.wMilliseconds/3600000.0);
                PD[TI].Otmetka.iTail=1;
                PD[TI].Otmetka.iFormulyar=1;
                PD[TI].Otmetka.apl=NULL;
                doptools_GetAzimutDalnostFromXY(
                 PD[TI].Otmetka.X,
                 PD[TI].Otmetka.Y,
                 PD[TI].Azimut,
                PD[TI].Otmetka.Dalnost);
                PD[TI].Otmetka.lDopNumber=PD[TI].Otmetka.lNomer;
                PD[TI].Otmetka.PrivyazanPredvaritelno=0;
                PD[TI].Otmetka.N_of_MustPO=PD[TI].Otmetka.N_of_MustSO=
                PD[TI].Otmetka.N_of_PO=PD[TI].Otmetka.N_of_SO=0;


                PD[TI].NomerSektoraRazmerom30=(int)(PD[TPaketData::InIndex].Azimut)/30;
//Номер присвоем
//                 PD[TI].Otmetka.lDopNumber=
                 PD[TI].Otmetka.lNomerTrassy=0+PD[TI].Otmetka.lNomer;
                 N_of_R++;
                 PD[TI].Otmetka.IndexInObzor=N_of_R;

                 TI=(TI+1)%N_OF_FIFO_PAK_STR;
                 N_of_TI++;
                 Indx+=15;
                 continue;
             }
             dwError|=ERR_PAKET_NE_RASPOZNAN;
             dwError1=ERR_PAKET_NE_RASPOZNAN;
             Indx+=1000;
          }
          break;

         case 0x02:
              switch(paket[Indx+3]) //критерий распознования, тип сообщения
             {
                case 0x82:
//Сектор пришел
                  PD[TI].TipPaketa=1;
                  PD[TI].cKodStrany=paket[Indx+1];
                  PD[TI].Azimut=(paket[Indx+4]>>3)*11.25;//?????
                  PD[TI].DateTime.God=(short)(lp.wYear);
                  PD[TI].DateTime.Mesyac=(short)(lp.wMonth);
                  PD[TI].DateTime.Den=(short)(lp.wDay);
                  PD[TI].DateTime.Time=(double)(lp.wHour+lp.wMinute/60.0+lp.wSecond/3600.0+lp.wMilliseconds/3600000.0);
                  PD[TI].NomerSektoraRazmerom30=(int)(PD[TI].Azimut)/30;
                  TI=(TI+1)%N_OF_FIFO_PAK_STR;
                  N_of_TI++;
                  Indx+=8;
///////////////////////////////////////////////////////////////////////////*/
                break;       //Sektor

                case 0x81:
                  PD[TI].TipPaketa=2;
                  PD[TI].cKodStrany=paket[Indx+1];
                  PD[TI].DateTime.God=(short)(lp.wYear);
                  PD[TI].DateTime.Mesyac=(short)(lp.wMonth);
                  PD[TI].DateTime.Den=(short)(lp.wDay);
                  PD[TI].DateTime.Time=(double)(lp.wHour+lp.wMinute/60.0+lp.wSecond/3600.0+lp.wMilliseconds/3600000.0);
                  PD[TI].Azimut=0;
                  PD[TI].NomerSektoraRazmerom30=(int)(PD[TI].Azimut)/30;
                  PD[TI].Sever.Soprovojdenie=0;
                  N_of_P=N_of_U=N_of_R=0;
                  TI=(TI+1)%N_OF_FIFO_PAK_STR;
                  N_of_TI++;
                  Indx+=7;
                 break;    //Sever
                 default:
                  dwError|=ERR_PAKET_NE_RASPOZNAN;
                  dwError1=ERR_PAKET_NE_RASPOZNAN;
                  Indx+=1000;   //Выйдем из цикла
             }
             break;

         default:
//Ошибка - объект на распознан
        dwError|=ERR_PAKET_NE_RASPOZNAN;
        dwError1=ERR_PAKET_NE_RASPOZNAN;
        Indx+=1000;   //Выйдем из цикла
//структура заполнена
       } //switch (paket[0]){
     };  // while(Indx<i)

     if(Terminated)break;

//Проверка - не будет ли
//Найем разницу
     N_of_TI1=TPaketData::InIndex-TPaketData::OutIndex;
     if(N_of_TI1<=0)N_of_TI1+=N_OF_FIFO_PAK_STR;
     if(N_of_TI1<N_of_TI)
     {
        dwError|=ERR_PAKET_PEREPOLNEN;
        if(!dwError)
        {
          TPaketData::OutIndex=(TPaketData::OutIndex+N_of_TI-N_of_TI1)%N_OF_FIFO_PAK_STR;
          TPaketData::InIndex=TI;
        }
     }else{
       if(!dwError1)TPaketData::InIndex=TI;
     }
     if(TPaketData::InIndex!=TPaketData::OutIndex)
       SetEvent(hEventNoEmpty);  //Установим событие, что список не пуст

  };  //while (1)

End_while:
  Disconnect1();

}
//---------------------------------------------------------------------------


/*Создание вторичного потока, открытие порта COM<N> и установка
скорости speed*/
TComReadThread* TComReadThread::
                       Connect(
                          const initialization &init
                              )  //Соединение с устройством
{
   TComReadThread* CurThread;
   char portname1[100],portname2[100];
   COMMTIMEOUTS comx; //
   bool fSuccess;      //Удачное соединение
   HANDLE hCom1;
   int i,k,s;

   iErrorConnect=0;
   iGetLastError=0;
   k=0; s=0;
   strcpy(portname2,init.csComPortStr);
   for(i=0;i<strlen(portname2);i++)
   {
        if(portname2[i]==' '||portname2[i]=='\t'||portname2[i]=='\\'||
           portname2[i]=='.')
        {
            if(s==0)
            {
                k++;
                continue;
            }else{
                portname2[i]=0;
                break;
            }
        }else{
            s=1;
        }
   }
   strcpy(portname1,"\\\\.\\");
   strcpy(portname1+strlen("\\\\.\\"),portname2+k);

   hCom1 = CreateFile(portname1,GENERIC_READ| GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL ); // открыли порт СОМ для чтения и записи
   if (hCom1 == INVALID_HANDLE_VALUE) {// проверяем открылся или нет порт СОМ
       iErrorConnect=ERR_CONNECT_CREATE;
       iGetLastError = GetLastError();
       return NULL;

   }

   SetCommMask(hCom1,EV_RXCHAR); //Установим маску
   SetupComm(hCom1,1500,1500);   //Установим буфер

   GetCommTimeouts(hCom1,&comx);//Получаем текущей Timeout
   comx.ReadIntervalTimeout=0;
   comx.ReadTotalTimeoutConstant = 400; // Timeout = 400    время ожидания данных
   comx. ReadTotalTimeoutMultiplier=10; //Получение времени
   SetCommTimeouts(hCom1,&comx);//Устанавливаем Timeout=10

   DCB ComDCM;
   memset(&ComDCM,0,sizeof(ComDCM));
   ComDCM.DCBlength = sizeof(DCB);

   fSuccess = GetCommState(hCom1, &ComDCM); //  Получаем текущие настройки порта
   if (!fSuccess)
   {// проверяем получили или нет текущие настройки порта
      iErrorConnect=ERR_CONNECT_GETCOMSTATE;
      iGetLastError = GetLastError();
      CloseHandle(hCom1);
      return NULL;
   }

   ComDCM.BaudRate = init.iBaudRate;        //скорость
   ComDCM.ByteSize = 8;                     //число бит данных
   ComDCM.Parity = init.parityMode % 5;        //бит паритета(до четности)
   ComDCM.StopBits = init.iStopBits % 3;
   ComDCM.fAbortOnError = TRUE;             //выдает ошибку если битов в пакете не хватает
   ComDCM.fDtrControl = DTR_CONTROL_DISABLE;//сигнал готовности DTR выключен
   ComDCM.fRtsControl = RTS_CONTROL_DISABLE;//сигнал готовности RTS выключен
//   ComDCM.fBinary = 0; // ?
    ComDCM.fBinary = 1; // ?
   ComDCM.fParity = (init.fParity != 0);
   ComDCM.fInX = ComDCM.fOutX = FALSE;      //
   ComDCM.XonChar = 0;                      //фильтр символов
   ComDCM.XoffChar = 0xff;
   ComDCM.fErrorChar = FALSE;
   ComDCM.fNull = FALSE;
   ComDCM.fOutxCtsFlow = FALSE;
   ComDCM.fOutxDsrFlow = FALSE;
   ComDCM.XonLim = 200;                    //ограничение на размер буфера
   ComDCM.XoffLim = 200;
   fSuccess = SetCommState(hCom1, &ComDCM);// Настраиваем порт СОМ
   if (!fSuccess)
   {// проверяем удалось ли настроить порт
     iErrorConnect=ERR_CONNECT_SETCOMSTATE;
     iGetLastError = GetLastError();
     CloseHandle(hCom1);
     return NULL;
   }

   //Создадим объект событие
//   hEventNoEmpty=0;
   HANDLE hEventNoEmpty1=CreateEvent(NULL,false,false,NULL);
   if(hEventNoEmpty1==NULL)
   {
     iErrorConnect=ERR_CONNECT_CREATEEVENT;
     iGetLastError = ::GetLastError();
     CloseHandle(hCom1);
     return NULL;
   };
   Sleep(500);

//Попроюе
   CurThread=new TComReadThread(init);
   if(!CurThread)
   {
     iErrorConnect=ERR_CONNECT_CREATETHREAD;
     iGetLastError = 0;
     CloseHandle(hCom1);
     CloseHandle(hEventNoEmpty1);
     return NULL;
   }

   CurThread->hEventNoEmpty=hEventNoEmpty1;
   CurThread->hCom=hCom1;

   return CurThread;
}



int __fastcall TComReadThread::Disconnect1()
{
   CloseHandle(hCom);
   ResetEvent(hEventNoEmpty);

   return 1;
}

