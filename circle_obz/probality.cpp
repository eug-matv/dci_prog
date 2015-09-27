//---------------------------------------------------------------------------


#pragma hdrstop

#include "probality.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TOcenkaVeroyatnosti::TOcenkaVeroyatnosti()
{
      lNomerTrassy=0;
      N_of_MustP=N_of_P=N_of_PropuskP=WasP=
      N_of_MustS=N_of_S=N_of_PropuskS=WasS=0;
}

TOcenkaVeroyatnosti::TOcenkaVeroyatnosti(long _lNomerTrassy)
{
      lNomerTrassy=_lNomerTrassy;
      N_of_MustP=N_of_P=N_of_PropuskP=WasP=
      N_of_MustS=N_of_S=N_of_PropuskS=WasS=0;
      LastOP=LastOS=0;
}



int TOcenkaVeroyatnosti::
   AddOtmetka(int Kanal)   //�����
{
   int EstP=0, EstS=0;   //���� ������� ���������� ��� ���������� ������
   if(Kanal==0)EstP=1;
   else if(Kanal==1)EstS=1;
   else if(Kanal==2){EstP=1; EstS=1;};


   if(N_of_MustP==40)
   {
         N_of_MustP--;
         if(OP[LastOP])
         {
            N_of_P--;
            if(N_of_P<0)N_of_P=0;
         }
         if(EstP)
         {
            OP[LastOP]=1;
         }else{
            OP[LastOP]=0;
         }
         LastOP=(LastOP+1)%40;
      }else{
         if(EstP)
         {
            OP[LastOP]=1;
         }else{
            OP[LastOP]=0;
         }
         LastOP=(LastOP+1)%40;
   }
   if(EstP)
   {
//���� ������ � ��������� ������
      if(WasP)
      {
//���� ��� P
         N_of_PropuskP=0;
         N_of_P++;
         N_of_MustP++;
      }else{
         N_of_PropuskP=0;
         N_of_P=1;
         N_of_MustP=1;
         WasP=1;
      }
   }else{
//��� ������ � ��������� ������ � ������ �������
      if(WasP)
      {
         if(N_of_PropuskP==10)
         {
           N_of_PropuskP=0;
           N_of_P=0;
           N_of_MustP=0;
           WasP=0;
           LastOP=0;
         }else{
           N_of_PropuskP++;
           N_of_MustP++;
         }
      }
   }



   if(N_of_MustS==40)
   {
         N_of_MustS--;
         if(OS[LastOS])
         {
            N_of_S--;
            if(N_of_S<0)N_of_S=0;
         }
         if(EstS)
         {
            OS[LastOS]=1;
         }else{
            OS[LastOS]=0;
         }
         LastOS=(LastOS+1)%40;
   }else{
         if(EstS)
         {
            OS[LastOS]=1;
         }else{
            OS[LastOS]=0;
         }
         LastOS=(LastOS+1)%40;
   }


   if(EstS)
   {


//���� ������ � ��������� ������
      if(WasS)
      {
//���� ��� S
         N_of_PropuskS=0;
         N_of_S++;
         N_of_MustS++;
      }else{
         N_of_PropuskS=0;
         N_of_S=1;
         N_of_MustS=1;
         WasS=1;
      }
   }else{
//��� ������ � ��������� ������ � ������ �������
      if(WasS)
      {
         if(N_of_PropuskS==10)
         {
           N_of_PropuskS=0;
           N_of_S=0;
           N_of_MustS=0;
           WasS=0;
           LastOS=0;
         }else{
           N_of_PropuskS++;
           N_of_MustS++;
         }
      }
   }
   return 1;
}


//��������� ������ �����������
//���������� �� 0 �� 1 ���� ������ �����������, ����� -1
double TOcenkaVeroyatnosti::GetPropably(
                  int Kanal)   //����� 0 - ���������, 1 - ���������
{

   if(Kanal==0)
   {
      if(WasP&&N_of_MustP>=10)
      {
         return ((double)N_of_P)/N_of_MustP;
      }
      return (-1.0);
   }

   if(Kanal==1)
   {
      if(WasS&&N_of_MustS>=10)
      {
         return ((double)N_of_S)/N_of_MustS;
      }
   }
   return (-1.0);
}


//��������� ����� �������� �� ������� ����� ��� ���������
int TOcenkaVeroyatnosti::GetN_of_O(int Kanal) //����� 0 - ���������, 1 - ���������
{
  
   if(Kanal==1)return N_of_S;
   return N_of_P;
}


//��������� ����� ��������, ������� ������ ���� ����
int TOcenkaVeroyatnosti::GetN_of_MustO(int Kanal) //����� 0 - ���������, 1 - ���������
{
  
   if(Kanal==1)return N_of_MustS;
   return N_of_MustP;
}

