//---------------------------------------------------------------------------
/*������ paketstruct.h*/

#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include <stdlib.h>
#include "paketstruct.h"
#include "comreadthread.h"
#include "sopr_doptools.h"

#pragma package(smart_init)

int TComReadThread::iErrorConnect=0; //��� ������ ��� ���������� ������� Connect
int TComReadThread::iGetLastError=0; //��� ��������� ������





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
  FreeOnTerminate=true;   //����������� ��� ������
  dwError=0;
}
//---------------------------------------------------------------------------



void __fastcall TComReadThread::Execute()
{
     SYSTEMTIME lp; //��� ���������� ���������� �������

     bool Ret,WasPaket;
     DWORD j=0;// ����������, ���������� ���� ��������� � ��� �����
     BYTE b;   // ���������� � ������� ����������� �������� ������ � ��� �����
     bool start=false; //�� ������ ������ ������ �� ������
     static BYTE paket[255];
     BYTE tmp;
     bool RetB;
     DWORD lEr=0;
     COMSTAT Comstat;
     TPaketData *PD=TPaketData::PaketData;
     DWORD dwError1;
     int Indx;    //������
     int TI,N_of_TI=0;      //��������������� ������
     int N_of_TI1;          //���������� ��������
     int N_of_Bytes;
     int IsUVD, IsRBS;          //��� ��� RBS
     PurgeComm(hCom,PURGE_RXCLEAR); //������� ������� �����
     N_of_P=N_of_U=N_of_R=0;
  while(1)
  {
     start=false;
     dwError1=0;
     j=0;
     lEr=0;
     if(Terminated)goto End_while;
     WasPaket=false;    //����� �� �������
     do{  // ���� ���������� ������ � ��� �����
       ZeroMemory(&Comstat, sizeof(COMSTAT));     //��������� ��������� Comstat
       lEr=0;
       RetB=ClearCommError(hCom,&lEr, &Comstat);  //���������� false ���� �������� ������ ����������
       if(lEr&CE_RXOVER)
       {
//������������
          dwError|=ERR_PEREPOLENIE_BUFFERA;
       }
       if(!RetB)
       {
          dwError|=ERR_UNKNOWN_CLEARCOMERROR;
       }
       if(Terminated)goto End_while;
       RetB=ReadFile(hCom,&b,1,&j,NULL);             // ������ ���� ���� ������ �� ��� �����
       if(Terminated)goto End_while;
       if(RetB&&j!=0){                                // ����� �� ��� ���� � ������ �������.
            if(b==0x10)
            {                        // �������� DLE

//��������� ����� ��������� ������ ������ - ����� ��� ��� �����
                GetLocalTime(&lp);
                if(Terminated)goto End_while;   //�������� ���������� ���������� ������
                RetB=ReadFile(hCom,&b,1,&j,NULL);
                if(Terminated)goto End_while;
                if(RetB&&j!=0)
                    if(b==0x02)start=true;
            }

            if(Terminated)goto End_while;

                      //c ����� ������� ������ ������ �������
            if(start==true)
            {                   // ���� ������ ������ �������
                int i=0;
                bool l= false;
                while(1){
                    if(Terminated)goto End_while;
                    RetB=ReadFile(hCom,&b,1,&j,NULL);
                    if(RetB&&j!=0){
                        if(b==0x10){                //�������� ������� ����� ������ DLE=0x10
                            RetB=ReadFile(hCom,&b,1,&j,NULL);
                            if(RetB&&j!=0){

//��������, ��� ����� ������� ����� �������� 0x03 � ���� ����� ��������� ������ �����
                               if(b==0x03&&i>=9&&paket[1]==i){        //ETX = 0x03
//�������� ����� ������. ����� ���� ��� �������� ������ - ������ 0x03
//�������� ����� ������
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
       }// ����� if(j!=0)
     }while(j!=0);//������ ������ ���� ����� ��� ����� �� ����    *******
// � ����� ������� ����� ������� �������� ���������� ���
loop:
     if(Terminated)goto End_while;
     if(!WasPaket)continue;
     Indx=2;         //������ ������ �� ������� �����
     TI=PD->InIndex;
     N_of_TI=0;
     while(Indx<N_of_Bytes)
     {
        switch (paket[0]){  //�������� ������������� ������ ��� ���������, ������ ���� ������ �� ����� �������������
         case 0x01:
           if(paket[Indx+3]==0x10){
//��������� �����
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
             PD[TI].Otmetka.lDopNumber=0;   //������� �����
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
//�������� ����, ��� ��� ���
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



//�������� ������������� ����������� ������.
//��������� ������ � init.
//init.uvdNumberControlNByte - ����� ����� (������ � 1) � ���.����� ������.
//���� ����� 0, �� � ������ �� ������ ���������� � ������������� - ��� ���������
//�����������. init.uvdNumberControlNBit - ����� ���� �� 0 �� 7.
//init.uvdNumberControlValueOfTrue - �������� ����, ������� �������������
//����������� ���������� � ������, ����� 0 ��� 1
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
                 //��������� ��������� ������� ���� �� inta
                        unsigned char tmp_uc[4]= {paket[Indx+16],
                          (paket[Indx+15])|0xc0, 0xFF,0xFF};
                        PD[TI].Otmetka.lVysota=(long)(*((int*)(tmp_uc)));
                 }else{
                     PD[TI].Otmetka.lVysota=(long)((((unsigned short)paket[Indx+15]&0x1F)<<8 | paket[Indx+16]));

                }
                 PD[TI].Otmetka.lVysota*=10;

//�������� ������������� ����������� ������� �������.
//��������� ������ � init.
//init.uvdFuelControlNByte - ����� ����� (������ � 1) � ���.����� ������.
//���� ����� 0, �� � ������ �� ������ ���������� � ������������� - ��� ���������
//�����������. init.uvdFuelControlNBit - ����� ���� �� 0 �� 7.
//init.uvdFuelControlValueOfTrue - �������� ����, ������� �������������
//����������� ���������� �� ������� �������, ����� 0 ��� 1
                 if(init.uvdFuelControlNByte > 0){
                        unsigned char fg =
                 (paket[Indx+init.uvdFuelControlNByte-1] &
              (1<<init.uvdFuelControlNBit))>>init.uvdFuelControlNBit;
                        PD[TI].Otmetka.bOstatokTopliva=
              (((int)fg) == init.uvdFuelControlValueOfTrue);
                 }else{
                        PD[TI].Otmetka.bOstatokTopliva = true;
                 }

//�������� ������������� ����������� ������.
//��������� ������ � init.
//init.uvdAltitudeControlNByte - ����� ����� (������ � 1) � ���.����� ������.
//���� ����� 0, �� � ������ �� ������ ���������� � ������������� - ��� ���������
//�����������. init.uvdAltitudeControlNBit - ����� ���� �� 0 �� 7.
//init.uvdAltitudeControlValueOfTrue - �������� ����, ������� �������������
//����������� ���������� � ������, ����� 0 ��� 1
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


//�������� ������������� ����������� ������.
//��������� ������ � init.
//init.rbsNumberControlNByte - ����� ����� (������ � 1) � ���.����� ������.
//���� ����� 0, �� � ������ �� ������ ���������� � ������������� - ��� ���������
//�����������. init.rbsNumberControlNBit - ����� ���� �� 0 �� 7.
//init.rbsNumberControlValueOfTrue - �������� ����, ������� �������������
//����������� ���������� � ������, ����� 0 ��� 1
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

//�������� ������������� ����������� ������.
//��������� ������ � init.
//init.rbsAltitudeControlNByte - ����� ����� (������ � 1) � ���.����� ������.
//���� ����� 0, �� � ������ �� ������ ���������� � ������������� - ��� ���������
//�����������. init.rbsAltitudeControlNBit - ����� ���� �� 0 �� 7.
//init.rbsAltitudeControlValueOfTrue - �������� ����, ������� �������������
//����������� ���������� � ������, ����� 0 ��� 1
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
//����� ��������
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
              switch(paket[Indx+3]) //�������� �������������, ��� ���������
             {
                case 0x82:
//������ ������
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
                  Indx+=1000;   //������ �� �����
             }
             break;

         default:
//������ - ������ �� ���������
        dwError|=ERR_PAKET_NE_RASPOZNAN;
        dwError1=ERR_PAKET_NE_RASPOZNAN;
        Indx+=1000;   //������ �� �����
//��������� ���������
       } //switch (paket[0]){
     };  // while(Indx<i)

     if(Terminated)break;

//�������� - �� ����� ��
//����� �������
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
       SetEvent(hEventNoEmpty);  //��������� �������, ��� ������ �� ����

  };  //while (1)

End_while:
  Disconnect1();

}
//---------------------------------------------------------------------------


/*�������� ���������� ������, �������� ����� COM<N> � ���������
�������� speed*/
TComReadThread* TComReadThread::
                       Connect(
                          const initialization &init
                              )  //���������� � �����������
{
   TComReadThread* CurThread;
   char portname1[100],portname2[100];
   COMMTIMEOUTS comx; //
   bool fSuccess;      //������� ����������
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

   hCom1 = CreateFile(portname1,GENERIC_READ| GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL ); // ������� ���� ��� ��� ������ � ������
   if (hCom1 == INVALID_HANDLE_VALUE) {// ��������� �������� ��� ��� ���� ���
       iErrorConnect=ERR_CONNECT_CREATE;
       iGetLastError = GetLastError();
       return NULL;

   }

   SetCommMask(hCom1,EV_RXCHAR); //��������� �����
   SetupComm(hCom1,1500,1500);   //��������� �����

   GetCommTimeouts(hCom1,&comx);//�������� ������� Timeout
   comx.ReadIntervalTimeout=0;
   comx.ReadTotalTimeoutConstant = 400; // Timeout = 400    ����� �������� ������
   comx. ReadTotalTimeoutMultiplier=10; //��������� �������
   SetCommTimeouts(hCom1,&comx);//������������� Timeout=10

   DCB ComDCM;
   memset(&ComDCM,0,sizeof(ComDCM));
   ComDCM.DCBlength = sizeof(DCB);

   fSuccess = GetCommState(hCom1, &ComDCM); //  �������� ������� ��������� �����
   if (!fSuccess)
   {// ��������� �������� ��� ��� ������� ��������� �����
      iErrorConnect=ERR_CONNECT_GETCOMSTATE;
      iGetLastError = GetLastError();
      CloseHandle(hCom1);
      return NULL;
   }

   ComDCM.BaudRate = init.iBaudRate;        //��������
   ComDCM.ByteSize = 8;                     //����� ��� ������
   ComDCM.Parity = init.parityMode % 5;        //��� ��������(�� ��������)
   ComDCM.StopBits = init.iStopBits % 3;
   ComDCM.fAbortOnError = TRUE;             //������ ������ ���� ����� � ������ �� �������
   ComDCM.fDtrControl = DTR_CONTROL_DISABLE;//������ ���������� DTR ��������
   ComDCM.fRtsControl = RTS_CONTROL_DISABLE;//������ ���������� RTS ��������
//   ComDCM.fBinary = 0; // ?
    ComDCM.fBinary = 1; // ?
   ComDCM.fParity = (init.fParity != 0);
   ComDCM.fInX = ComDCM.fOutX = FALSE;      //
   ComDCM.XonChar = 0;                      //������ ��������
   ComDCM.XoffChar = 0xff;
   ComDCM.fErrorChar = FALSE;
   ComDCM.fNull = FALSE;
   ComDCM.fOutxCtsFlow = FALSE;
   ComDCM.fOutxDsrFlow = FALSE;
   ComDCM.XonLim = 200;                    //����������� �� ������ ������
   ComDCM.XoffLim = 200;
   fSuccess = SetCommState(hCom1, &ComDCM);// ����������� ���� ���
   if (!fSuccess)
   {// ��������� ������� �� ��������� ����
     iErrorConnect=ERR_CONNECT_SETCOMSTATE;
     iGetLastError = GetLastError();
     CloseHandle(hCom1);
     return NULL;
   }

   //�������� ������ �������
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

//�������
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

