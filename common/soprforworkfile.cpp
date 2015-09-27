//---------------------------------------------------------------------------


#pragma hdrstop



#include <dir.h>
#include <io.h>
#include <fcntl.h>
#include <math.h>

#ifdef linux
#include <sys\stat.h>
#else
#include <sys/stat.h>
#endif

#include <stdio.h>
#include <time.h>

#include "paketstruct.h"
//#include "soprOtmetki.h"
#include "soprforworkfile.h"
#include "win_str_dir_utils.h"



//---------------------------------------------------------------------------
#define ABS(x) ((x)>=0?(x):(-(x)))



#pragma package(smart_init)


static char FullPath[1024];  //������ ���� � ������

static int Cifra2=5;

static int FileName_d;  //���������� ����� ������� ���������� �����








//������� ���� �� �����, ������� �������� � ������ ������
//��������������� ���������
//�� ���������: char *FileName
//��� ���������: Y, M,D - ���, �����, ����
//���������� 0, ���� ��� �����. -1 ���� ���� �� ���� �� ��������, 1 - ���� ��������
int soprGetDate(char *FileName,
                int *Y,
                int *M,
                int *D)
{
  FILE *fp;
  char c1,c2;
  int Ret;
  fp=fopen(FileName,"r");
  if(!fp)return 0;
  Ret=fseek(fp,47,0);  //������� �� 47
  if(Ret)
  {
       fclose(fp);
       return (-1);
  }
  Ret=fscanf(fp,"%d%c%d%c%d", D, &c1, M, &c2, Y);
  fclose(fp);
  if(Ret==5)return 1;
  return (-1);
}


//�������� ������ �� �������� ����
//���������� ���� ��� ���������
int soprDelFiles(char *Path,    //���� � ������
                 char *Prefix,  //�������
                 char *Ext,     //����������
                 char Razdelit,
                 int Y, int M, int D)
{
  int i;
  char Stroka[2000];
  int M1,Y1,D1;
  int Ret;
  if(access(Path,00))return 0;


  for(i=0;i<24;i++)
  {
    sprintf(Stroka,"%s%c%s%02d.%s",Path,Razdelit,Prefix,i,Ext);
    Ret=soprGetDate(Stroka,&Y1,&M1,&D1);
    if(Ret==1)
    {
      if(Y1!=Y||M1!=M||D1!=D)
      {
         remove(Stroka);
      }
    }
    if(Ret==-1)
    {
      remove(Stroka);
    }
  }
  return 1;
}



//soprGetMinNomer
//������� ����������� ���� � ����� �� �� ������ ��������� �������
//���������� ����� ����� �� 0 �� 23, ����� -1
//������������ � ������
int soprGetMinNomer(char *Path,
                    char *Prefix,
                    char *Ext,
                    char Razdelit,
                    TMyDateTime DT)
{
   char Stroka[2000];
   int i;
   TMyDateTime MinDT;
   MinDT.God=2038;
   MinDT.Mesyac=1;
   MinDT.Den=1;
   MinDT.Time=0;
//
   for(i=0;i<24;i++)
   {

   }

}






/*���������� � ������ ������*/


long __stdcall soprSaveToFileOtmetkaString(
            char *Strka,         //������ ������ ������ ���� �� ������ 150 - ������������
            TPaketData &_PaketData,
            bool bVyvodAmpl, //�������� �������� ��� ���������� ������,
            bool bSkorostOut, //�������� ������ � ���������
            bool bDopDopInfo,      //�������� ���������� ���������� � ��������� ������ � �������
	    int &SizeOfStroka      //������ ������

                                )
{
  char strMAURC[6];   //����, ���. ���, RBS, ����������� �������
  char strPS[3];
  char strNHNH[5];
  char strAO[3];
  int Index;
  long OstatokToplivaOut;
  short H,M;
  double S, Ostatok;

  H=(short)(_PaketData.DateTime.Time);
  Ostatok=(_PaketData.DateTime.Time-H)*60;
  M=(short)(Ostatok);
  S=(Ostatok-M)*60.0;



  if(_PaketData.Otmetka.bOstatokTopliva)
    OstatokToplivaOut=_PaketData.Otmetka.lOstatokTopliva;
  else OstatokToplivaOut=-1;

//�������� ������ strMAURC
  if(_PaketData.Otmetka.Kanal==Kanal_P)
  {
//�������� ��� ���������� ������
    if(_PaketData.Otmetka.bP_Mono)strMAURC[0]='M';
    else strMAURC[0]=' ';
    if(_PaketData.Otmetka.bP_LCHM)strMAURC[1]='A';
    else strMAURC[1]=' ';
    strMAURC[2]=strMAURC[3]=' ';
    if(_PaketData.Otmetka.bKO)strMAURC[4]='C'; else strMAURC[4]=' ';
    strMAURC[5]=0;
    strPS[0]='P'; strPS[1]=' '; strPS[2]=0;
    strAO[0]=strAO[1]=' '; strAO[2]=0;
  }else{
//�������� ��� ���������� ������
    if(_PaketData.Otmetka.Kanal==Kanal_PS)
      {strPS[0]='P'; strPS[1]='S'; strPS[2]=0;}
    else
      {strPS[0]=' '; strPS[1]='S'; strPS[2]=0;};
    strMAURC[0]=' ';     strMAURC[1]=' ';
    if(_PaketData.Otmetka.Rej==RejimUVD){strMAURC[2]='U';strMAURC[3]=' ';}
    else {strMAURC[2]=' ';strMAURC[3]='R';}
    if(_PaketData.Otmetka.bKO)strMAURC[4]='C'; else strMAURC[4]=' ';
    strMAURC[5]=0;

    if(_PaketData.Otmetka.bAbsVysota)
      {strAO[0]='A'; strAO[1]=' '; strAO[2]=0;}
      else
      {strAO[0]=' '; strAO[1]='O'; strAO[2]=0;};


  }

//�������� ������ � ���� ��������
  strNHNH[0]=' ';
  strNHNH[1]=' ';
  if(_PaketData.Otmetka.bNomer)strNHNH[2]='N';else strNHNH[2]=' ';
  if(_PaketData.Otmetka.bVysota)strNHNH[3]='H';else strNHNH[3]=' ';
  strNHNH[4]=0;

//���������� ��� ������������� ������

  

  sprintf(Strka, "\
 %c%c |\
%3d|\
%s|\
%s|\
%s|\
%s|\
%4d|\
%3d|\
%04X|\
%5d|\
%7d|\
%7d|\
%7d|\
 %6.02lf |\
 %6.02lf |\
 %02d:%02d:%06.03lf |",
  '5','0',   //� ������ �������
  _PaketData.Otmetka.IndexInObzor,
  strMAURC,
  strPS,
  strNHNH,
  strAO,
  _PaketData.Otmetka.sKodB,
  OstatokToplivaOut,
  _PaketData.Otmetka.lNomerTrassy%0x10000,
  _PaketData.Otmetka.lNomer,
  _PaketData.Otmetka.lVysota,
  _PaketData.Otmetka.X,
  _PaketData.Otmetka.Y,
  _PaketData.Azimut,
  _PaketData.Otmetka.Dalnost,
  H,M,S);

  SizeOfStroka=103;


//�������� ���� �� �������� �������������� ������

  if(bVyvodAmpl&&_PaketData.Otmetka.Kanal==Kanal_P)
  {
   //������� �������� ���������
       sprintf(Strka+103," %8.1e |",_PaketData.Otmetka.dAmpl);
       SizeOfStroka=114;
  }else{
     if(bSkorostOut||bDopDopInfo)
     {
//������� 10 ��������
       sprintf(Strka+103,"          |");
       SizeOfStroka=114;
     }
  }



  if(bSkorostOut)
  {
//������� ���������� � �������� ���������
     sprintf(Strka+114,"%5d|%5d|",_PaketData.Otmetka.Vx,
                                  _PaketData.Otmetka.Vy);  //������� �������� ��������
     SizeOfStroka=126;
  }else{
     if(bDopDopInfo)
     {
        sprintf(Strka+114,"     |     |");  //������� �������� ��������
        SizeOfStroka=126;
     }
  }


  if(bDopDopInfo)
  {
    sprintf(Strka+126, "%5d|%7d|",_PaketData.Otmetka.lDopNumber,
                                 _PaketData.Otmetka.lDopVysota);
    SizeOfStroka=140;
  }

//��������� 25.01.2008
//������� ��� �������������� ����������
  if(_PaketData.Otmetka.N_of_MustPO||
     _PaketData.Otmetka.N_of_MustSO)
  {
//���� ���� ���������� � ��������� ������
    if(_PaketData.Otmetka.N_of_MustPO>0)
    {
       sprintf(Strka+SizeOfStroka,"%4d|",_PaketData.Otmetka.N_of_PO);
       SizeOfStroka+=5;
       sprintf(Strka+SizeOfStroka,"%4d|",_PaketData.Otmetka.N_of_MustPO);
       SizeOfStroka+=5;
       if(_PaketData.Otmetka.P_PO>=0.0)sprintf(Strka+SizeOfStroka,"%4.02lf|",_PaketData.Otmetka.P_PO);
       else sprintf(Strka+SizeOfStroka,"   ?|");
       SizeOfStroka+=5;
    }else{
       sprintf(Strka+SizeOfStroka,"    |    |    |");
       SizeOfStroka+=15;
    }
    if(_PaketData.Otmetka.N_of_MustSO>0)
    {
       sprintf(Strka+SizeOfStroka,"%4d|",_PaketData.Otmetka.N_of_SO);
       SizeOfStroka+=5;
       sprintf(Strka+SizeOfStroka,"%4d|",_PaketData.Otmetka.N_of_MustSO);
       SizeOfStroka+=5;
       if(_PaketData.Otmetka.P_SO>=0.0)sprintf(Strka+SizeOfStroka,"%4.02lf|",_PaketData.Otmetka.P_SO);
       else sprintf(Strka+SizeOfStroka,"   ?|");
       SizeOfStroka+=5;
    }else{
       sprintf(Strka+SizeOfStroka,"    |    |    |");
       SizeOfStroka+=15;
    }
  }
  return 1;
}





/*���������� ������ � ������ Strka*/
long __stdcall soprSaveToFileSeverString
(
    char *Strka,         //������ ������ ������ ���� �� ������ 120 - ������������
    short God, short Mesyac, short Day,  //��� ����� ����
    char *DopText,
    short H, short M, double S   //����� � �����, ������� � ��������
)
{
  int i,j,k;
    
     sprintf(Strka, "NORD -- %02d.%02d.%04d -- ",Day,Mesyac,God);
  

  for(i=21,j=0;i<=85;i++,j++)
  {
    if(DopText[j]=='\0')
    {
      for(k=i;k<=85;k++)
      {
         Strka[k]=' ';
      }
      break;
    }
    Strka[i]=DopText[j];
  }
  Strka[86]=' '; Strka[87]='|';
  sprintf(Strka+88," %02d:%02d:%06.03lf |",H,M,S);


  return 1;
}

//����� ������ � �������
long __stdcall soprSaveToFileSektorString
(
  char *Strka,         //������ ������ ������ ���� �� ������ 150 - ������������
  double Azmt,
  short H, short M, double S   //����� � �����, ������� � ��������
)
{
   sprintf(Strka,"SECTOR %6.02lf | %02d:%02d:%06.03lf |" ,Azmt,H,M,S);
   return 1;
}


/*����� ������ � ������ Strka*/
long __stdcall  soprSaveToFileString(
            char *Strka,         //������ ������ ������ ���� �� ������ 200 - ������������
            TPaketData &_PaketData,
            char *DopTextOut,//�������������� ����� ��� ������
            bool bVyvodAmpl, //�������� �������� ��� ���������� ������,
            bool bSkorostOut, //�������� ������ � ���������
            bool bDopDopInfo,      //�������� ���������� ���������� � ��������� ������ � �������
	    int &SizeOfStroka      //������ ������
                          )
{
   short M,H;
   double S,Ostatok;
   char Strka1[250]=" ";
//��������� ���� ������ � �������

   switch(_PaketData.TipPaketa)
   {
      case TipPakSektor:
        H=(short)(_PaketData.DateTime.Time);
        Ostatok=(_PaketData.DateTime.Time-H)*60;
        M=(short)(Ostatok);
        S=(Ostatok-M)*60.0;
//������� ������
        soprSaveToFileSektorString(
          Strka,
          _PaketData.Azimut,
          H,M,S);
	  SizeOfStroka=30;
      break;

      case TipPakSever:
        H=(short)(_PaketData.DateTime.Time);
        Ostatok=(_PaketData.DateTime.Time-H)*60;
        M=(short)(Ostatok);
        S=(Ostatok-M)*60.0;
        if(_PaketData.Sever.Soprovojdenie)
        {
          Strka1[15]='�';
          Strka1[16]='�';
          Strka1[17]=' ';
        } 

        soprSaveToFileSeverString(
           Strka,
           _PaketData.DateTime.God,
           _PaketData.DateTime.Mesyac,
           _PaketData.DateTime.Den,
           Strka1,
           H, M, S);
	   SizeOfStroka=103;
      break;


      case TipPakOtmetka:
           soprSaveToFileOtmetkaString(Strka,
                    _PaketData,
                   bVyvodAmpl,
                   bSkorostOut,
                   bDopDopInfo,
		   SizeOfStroka);
      break;

      default:
        return 0;
   };


   return 1;
}

/*��������� ������ �� ������ � �����*/
long __stdcall  soprGetFromStringToPaketData
                (
                  char *Strka,                //����������� ������
                  TMyDateTime *LastDateTime,  //��������� ����� (�.�. ����� 0)
                  TPaketData &_PaketData
                )
{
  long Dlina;
  int Ret;
  double dfChislo;
  int M1,H1;
  double S1;
  int M2,Y2,D2;
  char c1,c2;
  struct tm tm1, *tm2;
  time_t time_t1;
  int ForTrek;  //�������� �� ����. ���� 0, ���� ����, ���� 1, ���� ����.
  char *VrStr;
  double Azimut, Dalnost;
//���������� ������
//��������� ������� ��� �������
  Dlina=strlen(Strka);
//��������� ���� �������
  if(Dlina>=30&&Strka[0]=='S'&&Strka[1]=='E'&&Strka[2]=='C'&&
     Strka[3]=='T'&&Strka[4]=='O'&&Strka[5]=='R')
  {
//��� �������� ������
//�������� �������
     Ret=sscanf(Strka+7,"%lf",&(_PaketData.Azimut));
     if(Ret==0||_PaketData.Azimut<-0.000001||
       _PaketData.Azimut>359.99999)
     {
        return 0;
     }

//�������� �����
     Ret=sscanf(Strka+16,"%d%c%d%c%lf",&H1,&c1,&M1,&c2,&S1);
     if(Ret!=5||H1<0||H1>23||M1<0||M1>59||S1<-0.0000000000001||
       S1>59.999999999999||c1!=':'||c2!=':')
     {
       return 0;
     }
     _PaketData.DateTime.Time=H1+M1/60.0+S1/3600.0;


//������ ����
     if(LastDateTime)
     {
//������� ���� � �����
       if(_PaketData.DateTime.Time<LastDateTime->Time-5.0/60.0)
       {
//���� ��������� ��������� ����
           tm1.tm_year=LastDateTime->God-1900;
           tm1.tm_mon=LastDateTime->Mesyac-1;
           tm1.tm_mday=LastDateTime->Den;
           tm1.tm_sec=tm1.tm_min=tm1.tm_hour=0;
           tm1.tm_isdst=-1;
           time_t1=mktime(&tm1);
           time_t1+=24*3600;
           tm2=localtime(&time_t1);
           _PaketData.DateTime.God=tm2->tm_year+1900;
           _PaketData.DateTime.Mesyac=tm2->tm_mon+1;
           _PaketData.DateTime.Den=tm2->tm_mday;
       }else{
           _PaketData.DateTime.God=LastDateTime->God;
           _PaketData.DateTime.Mesyac=LastDateTime->Mesyac;
           _PaketData.DateTime.Den=LastDateTime->Den;
       }
     }else{
       _PaketData.DateTime.God=0;
       _PaketData.DateTime.Mesyac=0;
       _PaketData.DateTime.Den=0;
     }
//�� ���������
     _PaketData.TipPaketa=TipPakSektor;
     *LastDateTime=_PaketData.DateTime;
     return 1;
  }

//��������� ��� ������� ���� �����
  if(Dlina>=101&&Strka[0]=='N'&&Strka[1]=='O'&&Strka[2]=='R'&&
     Strka[3]=='D')
  {
//�������� ����
      Ret=sscanf(Strka+8,"%d%c%d%c%d",&D2,&c1,&M2,&c2,&Y2);
      if(Ret!=5||D2>31||D2<0||M2>12||M2<1||Y2<1900||Y2>2038||
         c1!='.'||c2!='.')
      {
         return 0;
      }
      _PaketData.DateTime.God=Y2;
      _PaketData.DateTime.Mesyac=M2;
      _PaketData.DateTime.Den=D2;

//�������� �����
     Ret=sscanf(Strka+89,"%d%c%d%c%lf",&H1,&c1,&M1,&c2,&S1);
     if(Ret!=5||H1<0||H1>23||M1<0||M1>59||S1<-0.0000000000001||
       S1>59.999999999999||c1!=':'||c2!=':')
     {
       return 0;
     }
     _PaketData.DateTime.Time=H1+M1/60.0+S1/3600.0;
     _PaketData.TipPaketa=TipPakSever;

     _PaketData.Azimut=0;

//��������� ������ � ������
     *LastDateTime=_PaketData.DateTime;
     return 1;
  }

//� ������ ����� ������� - ��������� ������ � �������. ������ ���������� ����
//��� � MTA (����� � �����), ��� � ����� �� ��������� ���� (����-1�)
  if(Dlina>=101&&Strka[0]==' '&&Strka[1]>=0x30&&Strka[1]<=0x39&&
     Strka[2]>=0x30&&Strka[2]<=0x39&&Strka[3]==' ')
  {
//�������� ��� ������
     if(Dlina<101)return 0;
     if(Strka[2]=='3')ForTrek=1; else ForTrek=0;

//��������� ��� ����� �������
     if(Strka[16-ForTrek]=='S')
     {
        if(Strka[15-ForTrek]=='P')
        {
//������������ �������
          _PaketData.Otmetka.Kanal=Kanal_PS;
        }else{
          _PaketData.Otmetka.Kanal=Kanal_S;
        }

//������ ���������� � ��������
        if(Strka[11]=='U'||
           Strka[31-ForTrek]=='U')
        {
           _PaketData.Otmetka.Rej=RejimUVD;
        }else
        if (Strka[12]=='R'||
           Strka[30-ForTrek]=='I')
        {
           _PaketData.Otmetka.Rej=RejimRBS;
        }else{
           _PaketData.Otmetka.Rej=RejimNeIzvesten;
        }

     }else{
        if(Strka[15-ForTrek]=='P')
        {
           _PaketData.Otmetka.Kanal=Kanal_P;
           _PaketData.Otmetka.bP_LCHM=(Strka[10]=='A');
           _PaketData.Otmetka.bP_Mono=(Strka[9]=='M');
        }else{
           return 0;   //�� ������������ ������
        }
     }

//����������� �� ����� ���������� ���������� ������ ��� S
    _PaketData.Otmetka.bKodN=(Strka[18-ForTrek]=='N');
    _PaketData.Otmetka.bKodH=(Strka[19-ForTrek]=='H');
    _PaketData.Otmetka.bNomer=(Strka[20-ForTrek]=='N'||
                               Strka[20-ForTrek]=='B');
    _PaketData.Otmetka.bVysota=(Strka[21-ForTrek]=='H');

    _PaketData.Otmetka.bKO=(Strka[13]=='C');

//���������� ��������������� ������
    _PaketData.Otmetka.bAbsVysota=
       (Strka[22-ForTrek]=='A'||Strka[23-ForTrek]=='A');

//������� �����
    Ret=sscanf(Strka+40-ForTrek,"%ld",&(_PaketData.Otmetka.lNomer));
    if(!Ret)return 0;

//������� ������
    Ret=sscanf(Strka+47-ForTrek,"%ld",&(_PaketData.Otmetka.lVysota));
    if(!Ret)return 0;

//������� ���������� ����������
    Ret=sscanf(Strka+54-ForTrek,"%ld",&(_PaketData.Otmetka.X));
    if(!Ret)return 0;

    Ret=sscanf(Strka+62-ForTrek,"%ld",&(_PaketData.Otmetka.Y));
    if(!Ret)return 0;

//������� �����
    if(ForTrek)
    {
      if(Dlina<114)return 0;
      VrStr=Strka+102;
    }else{
      VrStr=Strka+89;
    }

    Ret=sscanf(VrStr,"%d%c%d%c%lf",&H1,&c1,&M1,&c2,&S1);
    if(Ret!=5||H1<0||H1>23||M1<0||M1>59||S1<-0.0000000000001||
      S1>59.999999999999||c1!=':'||c2!=':')
    {
      return 0;
    }
    _PaketData.DateTime.Time=H1+M1/60.0+S1/3600.0;

//������ ����
    if(LastDateTime)
    {
//������� ���� � �����
       if(_PaketData.DateTime.Time<LastDateTime->Time-5.0/60.0)
       {
//���� ��������� ��������� ����
           tm1.tm_year=LastDateTime->God-1900;
           tm1.tm_mon=LastDateTime->Mesyac-1;
           tm1.tm_mday=LastDateTime->Den;
           tm1.tm_sec=tm1.tm_min=tm1.tm_hour=0;
           tm1.tm_isdst=-1;
           time_t1=mktime(&tm1);
           time_t1+=24*3600;
           tm2=localtime(&time_t1);
           _PaketData.DateTime.God=tm2->tm_year+1900;
           _PaketData.DateTime.Mesyac=tm2->tm_mon+1;
           _PaketData.DateTime.Den=tm2->tm_mday;
       }else{
           _PaketData.DateTime.God=LastDateTime->God;
           _PaketData.DateTime.Mesyac=LastDateTime->Mesyac;
           _PaketData.DateTime.Den=LastDateTime->Den;
       }
    }else{
       _PaketData.DateTime.God=0;
       _PaketData.DateTime.Mesyac=0;
       _PaketData.DateTime.Den=0;
    }

//������ ������� �������
    Ret=sscanf(Strka+31-ForTrek,"%ld",
        &(_PaketData.Otmetka.lOstatokTopliva));
    if(!Ret||_PaketData.Otmetka.lOstatokTopliva<0)
    {
       _PaketData.Otmetka.lOstatokTopliva=0;
       _PaketData.Otmetka.bOstatokTopliva=false;
    }else{
       _PaketData.Otmetka.bOstatokTopliva=true;
    }

//������ ��� ��������
    long lKodB;
    Ret=sscanf(Strka+26-ForTrek,"%ld",&lKodB);
    if(Ret||lKodB<1000)
    {
      _PaketData.Otmetka.sKodB=0;
    }else{
      _PaketData.Otmetka.sKodB=lKodB;
    }

//������ � ������������ ������ � ���������
    Ret=sscanf(Strka+72-ForTrek, "%lf",&Azimut)+
        sscanf(Strka+81-ForTrek, "%lf",&Dalnost);

    long X1,Y1;
    if(Ret==2&&Azimut>-0.000001&&Azimut<359.99999&&Dalnost>=-0.000001)
    {
//�������� ��� � ������������ ����������
       X1=(long)(Dalnost*sin(Azimut/180.0*M_PI)*1000);
       Y1=(long)(Dalnost*cos(Azimut/180.0*M_PI)*1000);
//������� X1 � X, Y1 � Y
       if(ABS(X1-_PaketData.Otmetka.X)>10||ABS(Y1-_PaketData.Otmetka.Y)>10)
       {
//������������� ������� � ���������
          if(_PaketData.Otmetka.X==0&&_PaketData.Otmetka.Y==0)
          {
              Azimut=0;
              Dalnost=0;
          }else{
              Azimut=atan2(_PaketData.Otmetka.X/1000.0,_PaketData.Otmetka.Y/1000.0)/M_PI*180.0;
              Dalnost=sqrt(_PaketData.Otmetka.X/1000.0*_PaketData.Otmetka.X/1000.0+
                       _PaketData.Otmetka.Y/1000.0*_PaketData.Otmetka.Y/1000.0);
          }
       }
    }else{
       if(_PaketData.Otmetka.X==0&&_PaketData.Otmetka.Y==0)
       {
          Azimut=0;
          Dalnost=0;
       }else{
          Azimut=atan2(_PaketData.Otmetka.X/1000.0,_PaketData.Otmetka.Y/1000.0)/M_PI*180.0;
          Dalnost=sqrt(_PaketData.Otmetka.X/1000.0*_PaketData.Otmetka.X/1000.0+
                    _PaketData.Otmetka.Y/1000.0*_PaketData.Otmetka.Y/1000.0);
       }
    }
    if(Azimut<0)Azimut+=360.0;
    _PaketData.Otmetka.Dalnost=Dalnost;
    _PaketData.Azimut=Azimut;
    
    
//������ �������������� ���������� �� ������� ����.

    if(!ForTrek)
    {
      if(Dlina>=112)
      {
//�������� ������� �������
        if(_PaketData.Otmetka.Kanal==Kanal_P)
          Ret=sscanf(Strka+103,"%lf",
           &(_PaketData.Otmetka.dAmpl));
           if(!Ret)_PaketData.Otmetka.dAmpl=0.0;
        }else _PaketData.Otmetka.dAmpl=0.0;
      }else _PaketData.Otmetka.dAmpl=0.0;

//�������� ��������
      if(Dlina>=126)
      {
        Ret=sscanf(Strka+114,"%d",
              &(_PaketData.Otmetka.Vx));
        Ret+=sscanf(Strka+120,"%d",
              &(_PaketData.Otmetka.Vy));
        if(Ret!=2)
          _PaketData.Otmetka.Vx=_PaketData.Otmetka.Vy=100000;
      }else _PaketData.Otmetka.Vx=_PaketData.Otmetka.Vy=100000;

      if(Dlina>=140)
      {
        Ret=sscanf(Strka+126,"%d",
              &(_PaketData.Otmetka.lDopNumber));
        Ret+=sscanf(Strka+132,"%d",
              &(_PaketData.Otmetka.lDopVysota));

        if(Ret!=2)
_PaketData.Otmetka.lDopNumber=_PaketData.Otmetka.lDopVysota=-100;
      else
        _PaketData.Otmetka.lDopNumber=_PaketData.Otmetka.lDopVysota=-100;
    }//if(!ForTrek

    *LastDateTime=_PaketData.DateTime;
    _PaketData.TipPaketa=TipPakOtmetka;
    return 1;

  } //if(Strka[0]==' '&&Strka[1]>=0x...

  return 0;
}



/*����� ������� ����������� ������ �����. �� ������ ���������
  ����������: 0 - ���� ��� ��������, -1 - ���� �� ����������

���� csPutKDannym. -2 - ���� �� ������� ���������� � ����.
-3 - ������ �������� �����
1- ���� ������� ��� ������
*/
int TSoprSaveSession::Start(void)
{
  int Ret;
  time_t cur_time;
  struct tm *cur_tm;
  char c1,c2;
  int Y1,Dy1,Mth1;
  FILE *fp;
  char sDatePapka[1250],
       sFileData[1270];
  char StrokaOut[100];


  if(bIsInit||bIsInitLoad)return 0;

//�������� ������������� �������� csPutKDannym
  Ret=access(csPutKDannym,00);

//��������� ������� �����
  if(winMakePath(csPutKDannym))
  {
     Ret=access(csPutKDannym,00);
  }

  if(Ret!=0)return (-1);

//������ ������� �����
  time(&cur_time);
  cur_tm=localtime(&cur_time);

//������ ��� ����� �� ���

  sprintf(sDatePapka,"%s%c%02d",
       csPutKDannym,cRazdelitel,cur_tm->tm_mday);

//�������� ������������� ������ �����
  Ret=access(sDatePapka,00);
  if(Ret)
  {
//������ ����� �� ���������� - �� ������� ��
     if(mkdir(sDatePapka))
     {
        return (-3);
     }
  }


//��������� ������� �� ���������� ����� � �����
  soprDelFiles(sDatePapka,csPrefix,csExt,cRazdelitel,
    cur_tm->tm_year+1900,    cur_tm->tm_mon+1, cur_tm->tm_mday);

//������� ���� �� ������
  sprintf(sFileData,"%s%c%s%02d%c%s",
       sDatePapka,cRazdelitel,csPrefix,cur_tm->tm_hour,'.',csExt);





//������� ����
  if(access(sFileData,00))
  {
     lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY|O_TRUNC|O_CREAT,S_IREAD|S_IWRITE);
  }else{
     lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY);
  }
  if(lFD==-1)return -4;

//������� ������ � ����.
//���������� ������
  sprintf(StrokaOut,">>>   Start log : Time  %02d:%02d:%02d   Date %02d.%02d.%04d",    
   cur_tm->tm_hour,cur_tm->tm_min, cur_tm->tm_sec,
   cur_tm->tm_mday, cur_tm->tm_mon+1, cur_tm->tm_year+1900);

//�������� ��� ���� �������� � ������
  if(bDataOutDOS)
  {
     StrokaOut[57]=13;
     StrokaOut[58]=10;
     write(lFD,StrokaOut,59);

  }else{
     StrokaOut[57]=10;
     write(lFD,StrokaOut,58);
  }

  bIsInit=true;
  iLastHour=cur_tm->tm_hour;
  return 1;
}

/*��������� ������. ������ ������������ - �������� ��������� �� ���������
������ ������������ � ������� �����.
*/
int  TSoprSaveSession::Stop(void)
{
  int Ret;
  time_t cur_time;
  struct tm *cur_tm;
  char c1,c2;
  int Y1,Dy1,Mth1;
  FILE *fp;
  char sDatePapka[1250],
       sFileData[1270];
  char StrokaOut[100];


  if(!bIsInit)return 0;

//������ ������� �����
  time(&cur_time);
  cur_tm=localtime(&cur_time);


//�������� ������������� �������� csPutKDannym
  if(iLastHour!=cur_tm->tm_hour)
  {
     close(lFD);  //������� ������� ����������
     Ret=access(csPutKDannym,00);
     if(Ret!=0)return (-1);


//������ ��� ����� �� ���

     sprintf(sDatePapka,"%s%c%02d",
          csPutKDannym,cRazdelitel,cur_tm->tm_mday);

//�������� ������������� ������ �����
     Ret=access(sDatePapka,00);
     if(Ret)
     {
//������ ����� �� ���������� - �� ������� ��
        if(mkdir(sDatePapka))
        {
           return (-3);
        }
     }


     //��������� ������� �� ���������� ����� � �����
      soprDelFiles(sDatePapka,csPrefix,csExt,cRazdelitel,
    cur_tm->tm_year+1900,    cur_tm->tm_mon+1, cur_tm->tm_mday);

    //������� ���� �� ������
     sprintf(sFileData,"%s%c%s%02d%c%s",
          sDatePapka,cRazdelitel,csPrefix,cur_tm->tm_hour,'.',csExt);


//������� ����
    if(access(sFileData,00))
    {
       lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY|O_TRUNC|O_CREAT,S_IREAD|S_IWRITE);
    }else{
       lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY);
    }
    if(lFD==-1)return -4;
  }
//������� ������ � ����.
//���������� ������
  sprintf(StrokaOut,">>>   End log : Time  %02d:%02d:%02d   Date %02d.%02d.%04d",
       cur_tm->tm_hour,cur_tm->tm_min, cur_tm->tm_sec,
    cur_tm->tm_mday, cur_tm->tm_mon+1, cur_tm->tm_year+1900);

//�������� ��� ���� �������� � ������
  if(bDataOutDOS)
  {
     StrokaOut[57]=13;
     StrokaOut[58]=10;
     write(lFD,StrokaOut,59);

  }else{
     StrokaOut[57]=10;
     write(lFD,StrokaOut,58);
  }

//������� � ���� ������
  close(lFD);
  bIsInit=false;
  return 1;
}



int TSoprSaveSession::SaveDataToFile(TPaketData &_PaketData)
{
  int Ret;
  time_t cur_time;
  struct tm *cur_tm;
  char c1,c2;
  int Y1,Dy1,Mth1;
  FILE *fp;
  char sDatePapka[1250],
       sFileData[1270];
  char StrokaOut[150];
  int SizeOfStrka;

  if(!bIsInit)return 0;

//�������� ��������� ������
  switch(_PaketData.TipPaketa)
  {
     case TipPakOtmetka:
         if(!bSaveDataOtmetka)return 1;
         break;

     case TipPakSever:
         if(!bSaveDataSever)return 1;
         break;

     case TipPakSektor:
         if(!bSaveDataSektor)return 1;
         break;

     default:
         return 1;
  };

//������ ������� �����
  time(&cur_time);
  cur_tm=localtime(&cur_time);


//�������� ������������� �������� csPutKDannym
  if(iLastHour!=cur_tm->tm_hour)
  {
     close(lFD);  //������� ������� ����������
     Ret=access(csPutKDannym,00);
     if(Ret!=0)return (-1);



//������ ��� ����� �� ���

     sprintf(sDatePapka,"%s%c%02d",
          csPutKDannym,cRazdelitel,cur_tm->tm_mday);


//�������� ������������� ������ �����
     Ret=access(sDatePapka,00);
     if(Ret)
     {
//������ ����� �� ���������� - �� ������� ��
        if(mkdir(sDatePapka))
        {
           return (-3);
        }
     }

  //��������� ������� �� ���������� ����� � �����
      soprDelFiles(sDatePapka,csPrefix,csExt,cRazdelitel,
    cur_tm->tm_year+1900,    cur_tm->tm_mon+1, cur_tm->tm_mday);



//������� ���� �� ������
     sprintf(sFileData,"%s%c%s%02d%c%s",
          sDatePapka,cRazdelitel,csPrefix,cur_tm->tm_hour,'.',csExt);


//������� ����
//������� ����
    if(access(sFileData,00))
    {
       lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY|O_TRUNC|O_CREAT,S_IREAD|S_IWRITE);
    }else{
       lFD=open(sFileData,O_APPEND|O_RDWR|O_BINARY);
    }

    if(lFD==-1)return -4;
     //������� ������ � ����.
//���������� ������
    sprintf(StrokaOut,">>>   Start log: Time  %02d:%02d:%02d   Date %02d.%02d.%04d",
      cur_tm->tm_hour,cur_tm->tm_min, cur_tm->tm_sec,
      cur_tm->tm_mday, cur_tm->tm_mon+1, cur_tm->tm_year+1900);

//�������� ��� ���� �������� � ������
       if(bDataOutDOS)
       {
          StrokaOut[57]=13;
          StrokaOut[58]=10;
          write(lFD,StrokaOut,59);
       }else{
          StrokaOut[57]=10;
          write(lFD,StrokaOut,58);
       }
  }

//������� ������ � ������
  Ret=DataToStr(StrokaOut,_PaketData, DopTextOut, bVyvodAmpl, bSkorostOut, bDopDopInfo, SizeOfStrka);


//��� �������������� ����� � ���� ����� ������ �������������� ������
  iLastHour=cur_tm->tm_hour;
  if(Ret==1)
  {
     if(bDataOutDOS)
     {
        StrokaOut[SizeOfStrka]=13;
        StrokaOut[SizeOfStrka+1]=10;
        write(lFD,StrokaOut,SizeOfStrka+2);
     }else{
        StrokaOut[SizeOfStrka]=10;
        write(lFD,StrokaOut,SizeOfStrka+1);
     }
  }else{
    return 0;
  }
  return 1;
}




/*���������� ������ �� �����.
*/
/*�������� ������ �� �����*/
//������ �������� �������� ������ �� �����
//  ������� ���������� 1, ���� ��  ��.
//0, ���� ��� �������� �������� ��� ������ � ����.
//-1, ��� ��������� �����, ��� �� ������ ������� �����


int TSoprSaveSession::
    StartLoad(char *csFileNamePath, //���� � �������� ��� �����
                TMyDateTime *FirstTime, //FirstTime - ����� ������ ��������� ������ �� �����
                                        //���� ����� ����, ��������� �����
                int iType,       //���� Type=1, �� FileNamePath - ��� ���� � ������.
                                 //             ����� �������������� ����� � ����� FileNamePath.
                                 //���� Type=2, �� FileNamePath - ��� ������ ���� � ������
                                 //             �����
                                 //� ��������� ������� ������� ������ �� �����
                                 //csPutKDannym � ������ ����

                TPaketData *PDO)   //������
{
//������� ��������
   if(bIsInit||bIsInitLoad)return 0;

 //������� ������



   switch(iType)
   {
      case 1:
        GetDTForEveryHour(csFileNamePath,1);
        pTempMyDateTime=DTForEveryHour[0]; //�������� ������� � �������
        FirstDen=0;                        //��� ��� ������ ����� ������ GetDataDirForFirstTime
//���� � ����� ��� ���� ������
        fOpen=GetDataDirForFirstTime(csFileNamePath,
                                     FirstTime,1,PDO);
        strcpy(csFileNamePath1,csFileNamePath);
        iTypeLoad=1;
      break;

      case 2:
        fOpen=GetDataFileForFirstTime(csFileNamePath,
                                      FirstTime,
                                      PDO);
        if(!fOpen)return (-1);
        bIsInitLoad=true;
        iTypeLoad=2;
      return 1;

      default:
        GetDTForEveryHour(csFileNamePath,0);
        fOpen=GetDataDirForFirstTime(csFileNamePath,
                                     FirstTime,0,PDO);
        strcpy(csFileNamePath1,csPutKDannym);
        iTypeLoad=0;

   };
   if(!fOpen)return (-1);
   bIsInitLoad=true;
   if(FirstTime)
   {
      FirstDateTime=*FirstTime;
   }else{
      memset(&FirstDateTime,0,sizeof(TMyDateTime));
   }
   //�������� ����� �������
   if(fOpen)
   {
       PDO->NomerSektoraRazmerom30=(short)(PDO->Azimut/30);
       if(PDO->NomerSektoraRazmerom30==12)PDO->NomerSektoraRazmerom30=0;
       return 1;
   }

   return 1;
}


int TSoprSaveSession::GetDataFromFile(TPaketData *PDO)
{
  if(!bIsInitLoad)return 0;
  if(fOpen==NULL)return 0;
  int Ret;
//����� ������ ������ ���� �� ����� ������ ��� ���� �� ��������, ��� ��� �� ����� ����
    Ret=privGetDataFromFile(PDO);
    if(Ret==1)
    {
      PDO->NomerSektoraRazmerom30=(short)(PDO->Azimut/30);
      if(PDO->NomerSektoraRazmerom30==12)PDO->NomerSektoraRazmerom30=0;

      return 1;
    }
    fclose(fOpen);
    fOpen=NULL;
    if(iTypeLoad==2)return 0;   //��� ���������� ����� ��� - ���������

//���� ����������� ����� ��������� ������ � �����
//���������� �� ��������
    TMyDateTime tempMyDateTime;   //���������

    tempMyDateTime.God=LastDateTime.God;
    tempMyDateTime.Mesyac=LastDateTime.Mesyac;
    tempMyDateTime.Den=LastDateTime.Den;
    if(LastDateTime.Time<23)
    {
       tempMyDateTime.Time=(int)(LastDateTime.Time)+1;
    }else{
       tempMyDateTime.Time=23+59.0/60.0+59.9995/3600.0;
    }

//��������� ������� � ������� ��������� ������ �� �����
    fOpen=GetDataDirForFirstTime(csFileNamePath1,
                        &tempMyDateTime,iTypeLoad,PDO);

//�������� ����� �������
    if(fOpen)
    {
       PDO->NomerSektoraRazmerom30=(short)(PDO->Azimut/30);
       if(PDO->NomerSektoraRazmerom30==12)PDO->NomerSektoraRazmerom30=0;

       return 1;
    }
    return 0;
}



int TSoprSaveSession::StopLoad(void)
{
  if(!bIsInitLoad)return 0;
  if(fOpen)
  {
     fclose(fOpen);
     fOpen=NULL;
  }
  bIsInitLoad=false;
  return 1;
}




/*��������� ���� private*/

//����� � ������� ����, ��� ���� ������ ������� �� ������ �������� �������
//���������� ��������� ���� FILE.
FILE* TSoprSaveSession::GetDataDirForFirstTime
                           (
                             char *TekPapka,  //����� ������
                             TMyDateTime *FirstTime, //������ �����
                             int iType, //1 - �� ����,
                                             //0 - �� ������ �������� �� �����
                             TPaketData *PDO   //����� � ������� ������� �����������
                           )
{
   int Chislo,i,j;
   FILE *fp;
   char Strka[1200];

   TMyDateTime MDT,minMDT;

   if(iType==0)
   {
       if(FirstTime&&FirstTime->God>0&&FirstTime->Mesyac>0&&
          FirstTime->Den>0)
       {
          MDT.Time=(int)(FirstTime->Time); //�������� �� �����������
          MDT.God=FirstTime->God;
          MDT.Mesyac=FirstTime->Mesyac;
          MDT.Den=FirstTime->Den;
       }else{
          MDT.God=1901;
          MDT.Den=1;
          MDT.Mesyac=1;
          MDT.Time=0;
       }
       minMDT.God=2038;
       minMDT.Mesyac=12;
       minMDT.Den=31;
       Chislo=-1;
       FirstDen=0;
//��������� ������ ���� ����� � ����� �����, ������� ����������
       for(i=1;i<=31;i++)
       {
          for(j=0;j<24;j++)
          {
              if(DTForEveryHour[i][j].God!=0&&
                 DTForEveryHour[i][j]>=MDT&&DTForEveryHour[i][j]<minMDT)
              {
                 minMDT=DTForEveryHour[i][j];
                 FirstDen=i;
                 FirstChas=j;
              }

          }
       }

       if(FirstDen==0)return NULL;  //�� ������� ������ �����
       

//���� �� ���� ������ ������� � �������...
       i=FirstDen;
       do
       {
         sprintf(Strka,"%s%c%02d",TekPapka,cRazdelitel,i);
         pTempMyDateTime=DTForEveryHour[i];  //��������� ���������
         fOpen=GetDataDirForFirstTime(Strka,FirstTime,1,PDO);
         if(fOpen)
         {
            return fOpen;
         }
         i=(i+1)%31;
       }while(i!=FirstDen);

       return NULL;  //�� ������ ������
   };



//����� ����� ������� ����, �� ������ ������


//��������� ��������� ����� ������ ���������
//���������� �� ����
//��������� ������ ��� ������ ���������

   if(FirstDen==0)
   {
      if(FirstTime&&FirstTime->God>0&&FirstTime->Mesyac>0&&
         FirstTime->Den>0)
      {
          MDT.Time=(int)(FirstTime->Time); //�������� �� �����������
          MDT.God=FirstTime->God;
          MDT.Mesyac=FirstTime->Mesyac;
          MDT.Den=FirstTime->Den;
      }else{
          MDT.God=1901;
          MDT.Den=1;
          MDT.Mesyac=1;
          MDT.Time=0;
      }
      minMDT.God=2038;
      minMDT.Mesyac=12;
      minMDT.Den=31;
      Chislo=-1;
      for(i=0;i<24;i++)
      {
         if(pTempMyDateTime[i].God!=0)
         {
            if(pTempMyDateTime[i]>=MDT&&pTempMyDateTime[i]<minMDT)
            {
               Chislo=i;
               minMDT=pTempMyDateTime[i];
            }
         }
      }
      if(Chislo==-1)
      {
         return NULL;
      }
   }else{
     Chislo=FirstChas;
   }
   i=Chislo; //�������� �� ���� ������(�����)
//�������� ������ ��� ������ ����� ���� <�������><����>.<ext>
   do
   {
       sprintf(Strka,"%s%c%s%02d.%s",
       TekPapka,cRazdelitel,csPrefix,i,csExt);
       fOpen=GetDataFileForFirstTime(Strka,FirstTime,PDO);
       if(fOpen)
       {
          break;
       }
       i=(i+1)%24;
   }while(i!=Chislo);
   if(!fOpen)return NULL;
   return fOpen;
}


/* GetDataFileForFirstTime
  ��������������� ���������. ������������ � �������: GetDataDirForFirstTime.
���������� ������ ���� � �������� ����� TekFile,
� ��������� ����� TMyDateTime *FirstTime. �������� FirstTime ����� ����
����� NULL, ����� ������ ������ ������ � ������ �����. ����
FirstTime->Den, FirstTime->Mesyac ��� FirstTime->God ����� 0, ������ ���� ������ �� �������.
��� �������� ������ ����������� ������ ������ PDO ���� TPaketData.
������������ ��������:
��������� ����� ���� FILE ��� �������� ������ ��������, ������� ������ FirstTime
��� ������� - �������� NULL.
*/
FILE* TSoprSaveSession::GetDataFileForFirstTime(
                            char *TekFile,           //������� ����
                            TMyDateTime *FirstTime,  //��������� �����
                            TPaketData *PDO   //����� � ������� ������� �����������
                           )
{
   short M1,S1,H1;
   long lRet;
   int Y2,M2,D2;
   int Ret,i;
   char Strka[300],*RetStr;
   int iStrLen;
   if(fOpen)return NULL;
   bool bDataBolshe=false;  //������� ����, ��� ���� ������ ��� �������.
   //���� ��� �� ����� ��������������� �� �����
//�������� ������������� �����
   if(access(TekFile,00))
   {
//������ ������ ���������
     return NULL;
   }



//�������� ���� - ������ � ������, ���� ��������� ���� ����� � �����
   if(FirstTime&&FirstTime->Den&&FirstTime->God&&FirstTime->Mesyac)
   {
     Ret=soprGetDate(TekFile,&Y2,&M2,&D2);
     if(Ret==1)
     {
//������� ����
         if(Y2==FirstTime->God)
         {
             if(M2==FirstTime->Mesyac)
             {
                if(D2<FirstTime->Den)return 0;
                if(D2>FirstTime->Den)bDataBolshe=true;

             }else
               if(M2<FirstTime->Mesyac)return 0; else bDataBolshe=true;
         }else
            if(Y2<FirstTime->God)return 0; else bDataBolshe=true;
     }else{
        return 0;
     }
     LastDateTime=*FirstTime; //���������
   }else{
//��������� ������ �����
     LastDateTime=*FirstTime;
     LastDateTime.God=1901;
     LastDateTime.Mesyac=1;
     LastDateTime.Den=1;
   }


   fOpen=fopen(TekFile,"rb");
   if(!fOpen)return NULL;

//����� ��������������� ���������
   do
   {
//������� ������ � ��������
      PDO->IshodDannye.FileNumber=(int)(FirstTime->Time);
      PDO->IshodDannye.Smesh=ftell(fOpen);

//������� ������
      RetStr=fgets(Strka, 299, fOpen);
      if(!RetStr)
      {
         fclose(fOpen);
         return NULL;  //������ ��� � ������ �����
      }
//��������� ������ �� �������� 13,10
      for(i=0;Strka[i]!='\0';i++)if(Strka[i]==13||Strka[i]==10){Strka[i]=0; break;}

//�������� ������
      lRet=DataFromStr(Strka,&LastDateTime,*PDO);
      if(lRet)
      {
//��������. ���� ����� ������ �� ������
         if(bDataBolshe)return fOpen;

//� ��� �������� �����
         if(FirstTime->Time>PDO->DateTime.Time)
         {
            fclose(fOpen);
            return NULL;
         }
         LastFirstDateTime=PDO->DateTime;
         return fOpen;  //���������� �������� ����
      }
   }while(!feof(fOpen));
   fclose(fOpen);
   return NULL;
}


/*privGetDataFromFile - ������� ������ ������ �� �����*/
int TSoprSaveSession::privGetDataFromFile(TPaketData *PDO)
{
  char Strka[300],*RetStr;
  int Ret;
  int i;
  while(!feof(fOpen))
  {
     RetStr=fgets(Strka,299,fOpen);
     if(!RetStr) return 0;
     for(i=0;Strka[i]!='\0';i++)if(Strka[i]==13||Strka[i]==10){Strka[i]=0; break;}
     Ret=DataFromStr(Strka,&LastDateTime, *PDO);
     if(Ret==1)return 1;
  };
  return 0;
}

/*���������� ��������� DTForEveryHour �� ������ ����������*/
/*��������� ������ � �������*/
int TSoprSaveSession::GetDTForEveryHour(char *TekPapka, //������ ����
                                         int iType)     //���� �������
{
   int i,j,Ret;
   int M2,D2,Y2;
   char Strka[1500];
   if(iType==0)
   {
//��������� �� ���� ���������
       for(i=1;i<=31;i++)
       {
          for(j=0;j<24;j++)
          {
//������ ���� � �����
             sprintf(Strka,"%s%c%02d%c%s%02d.%s",
    TekPapka, cRazdelitel,i,cRazdelitel,csPrefix,j,csExt);
             Ret=soprGetDate(Strka,&Y2,&M2,&D2);
             if(Ret==1)
             {
                DTForEveryHour[i][j].God=Y2;
                DTForEveryHour[i][j].Mesyac=M2;
                DTForEveryHour[i][j].Den=D2;
                DTForEveryHour[i][j].Time=j;
             }else{
                DTForEveryHour[i][j].God=0;
                DTForEveryHour[i][j].Mesyac=0;
                DTForEveryHour[i][j].Den=0;
                DTForEveryHour[i][j].Time=0;
             }
          }
       }
       return 1;
   }

   if(iType==1)
   {
          for(j=0;j<24;j++)
          {
//������ ���� � �����
             sprintf(Strka,"%s%c%s%02d.%s",
    TekPapka, cRazdelitel,csPrefix,j,csExt);
             Ret=soprGetDate(Strka,&Y2,&M2,&D2);
             if(Ret==1)
             {
                DTForEveryHour[0][j].God=Y2;
                DTForEveryHour[0][j].Mesyac=M2;
                DTForEveryHour[0][j].Den=D2;
                DTForEveryHour[0][j].Time=j;
             }else{
                DTForEveryHour[0][j].God=0;
                DTForEveryHour[0][j].Mesyac=0;
                DTForEveryHour[0][j].Den=0;
                DTForEveryHour[0][j].Time=0;
             }
          }
       return 1;   
   }
   return 0;
}

