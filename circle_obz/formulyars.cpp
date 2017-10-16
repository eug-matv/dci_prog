//---------------------------------------------------------------------------


#pragma hdrstop

#include <stdlib.h>

#include "lang_str.h"
#include "kio_exeptions.h"
#include "structs.h"
#include "AnyTools.h"
#include "formulyars.h"
#include "probality.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)
#define MIN(X,Y) (((X) > (Y)) ? (Y) : (X))
#define MAX(X,Y) (((X) < (Y)) ? (Y) : (X))
#define ABS(X) ((X)>=0?(X):(-(X)))

TFormulyarDraw** TFormulyarDraw::FD=NULL;
int TFormulyarDraw::N_of_FD=0;  //���� �������� ����� 0
int TFormulyarDraw::MaxN_of_FD=0;
initialization TFormulyarDraw::init;
int TFormulyarDraw::Hscr=0;
int TFormulyarDraw::Wscr=0;
int TFormulyarDraw::lastFD=0;
double TFormulyarDraw::TO=0.0;

/*������������ ������. ������������ �� ������. ����������� ������� �����������
��������, ����� ������� �����*/
int formulyarGetText(HDC DC,               //���������� ������
                     airplane &air,        //��������� �� ��������
                     initialization &init, //��������� �������������
                     char *Texts[8],       //������ ������ - �� ����� 8 ����� -  ������ ���� �������� ������ �� ���� 20
                     int &N_of_Text,       // ����� ���������� �����  (�� 0 �� 8)
                     HFONT &Font           //���� ������
                     )
{
   int i;


   return 1;
}


int formulyarGetSize(initialization &init)
{
  int N_of_Strk=0, N_of_Symb=0;
  if(init.IsOutNomer)
  {
    N_of_Symb=MAX(N_of_Symb,5);
    N_of_Strk++;
  }

  if(init.IsOutVysota)
  {
    N_of_Symb=MAX(N_of_Symb,4);
    N_of_Strk++;
  }

  if(init.IsOutTime)
  {
    N_of_Symb=MAX(N_of_Symb,8);
    N_of_Strk++;
  }

   if(init.IsOutAzimutDalnost)
   {
      N_of_Symb=MAX(N_of_Symb,9);
      N_of_Strk++;
   }

   if(init.IsOutSkorost)
   {
     N_of_Symb=MAX(N_of_Symb,7);
     N_of_Strk++;
   }

   if(init.IsOutVeroyatnostPervKanal)
   {
     N_of_Symb=MAX(N_of_Symb,10);
     N_of_Strk++;
   }

   if(init.IsOutVeroyatnostVtorKanal)
   {
     N_of_Symb=MAX(N_of_Symb,10);
     N_of_Strk++;
   }

//������ ��������� �������
   init.sShir=N_of_Symb*init.szTextOut*2/3;
   init.sVysot=(N_of_Strk+0.6)*init.szTextOut;
   return 1;
}



int formulyarTextOut(HDC DC,               //���������� ������
                     int scrW, int scrH,   //������� ������ � ��������
                     airplane &air,        //��������� �� ��������
                     initialization &init, //��������� �������������
                     int iLeft, int iTop        //���������� ������ �������� ����
                     )

{
   HFONT Font,OldFont; //�����
   COLORREF Colour,OldColour;    //���� ����� � ������ ��������� � ������ �������
   char Strka[20];
   int  N_of_Strk,N_of_Bukv,realN_of_Strka;
   int Hs,Ws;
   HPEN Pen,OldPen;
   HBRUSH Brush,OldBrush;
   LOGPEN lp;
   LOGBRUSH lb;
   int Xt,Yt;
   long OldBkMode;      //������ ����� �����������


   if(air.Formulyar==0 || !init.IsOutFormulyar)return 0;

//��������� ������� ���� ���������� ������
//   if(air.N_of_Propusk>0)return 1;


//�� ������ ������ ��������� ������ � ��������� ���������
   Ws=MIN(scrW-iLeft, init.sShir);
   Hs=MIN(scrH-iTop, init.sVysot);
   N_of_Strk=(Hs-4)/init.szTextOut;    //��������� ������� �����
   N_of_Bukv=2*(Ws-2-4)/init.szTextOut;

//��� Colour ����� ����  �����
   switch(air.type)
   {
      case 0:
        Colour=init.CirclePen1.lopnColor;
      break;

      case 1:
        Colour=init.CirclePen2.lopnColor;
      break;

      default:
        Colour=init.CirclePen3.lopnColor;
   };

   if(air.N_of_Propusk>0)
   {
       int R1,R2,G1,G2,B1,B2,Rs,Gs,Bs;
       R1=GetRValue(Colour);
       G1=GetGValue(Colour);
       B1=GetBValue(Colour);
       R2=GetRValue(Colour);
       G2=GetGValue(Colour);
       B2=GetBValue(Colour);
       Rs=(R1+R2)/2; Gs=(G1+G2)/2; Bs=(B1+B2)/2;
       Colour=RGB(Rs,Gs,Bs);
   }


//������� �����
   if(init.iRamkaFormulyar)
   {
// ����� �����
      lp.lopnStyle=PS_SOLID;
      lp.lopnColor=Colour;
      lp.lopnWidth.x=lp.lopnWidth.y=0;
      if(air.TransFormulyar)
      {
         lb.lbStyle=BS_HOLLOW;
      }else{
         lb.lbStyle=BS_SOLID;
         lb.lbColor=init.Clrbackground;
      }
      Pen=CreatePenIndirect(&lp);
      OldPen=(HPEN)SelectObject(DC,Pen);
      Brush=CreateBrushIndirect(&lb);
      OldBrush=(HBRUSH)SelectObject(DC,Brush);
      Rectangle(DC,iLeft,iTop,iLeft+Ws-1,iTop+Hs-1);
      SelectObject(DC,OldPen);
      SelectObject(DC,OldBrush);
      DeleteObject(Pen);
      DeleteObject(Brush);
   }else{
      if(!air.TransFormulyar)
      {
         lp.lopnStyle=PS_SOLID;
         lp.lopnColor=init.Clrbackground;
         lp.lopnWidth.x=lp.lopnWidth.y=0;
         lb.lbStyle=BS_SOLID;
         lb.lbColor=init.Clrbackground;
         Pen=CreatePenIndirect(&lp);
         OldPen=(HPEN)SelectObject(DC,Pen);
         Brush=CreateBrushIndirect(&lb);
         OldBrush=(HBRUSH)SelectObject(DC,Brush);
         Rectangle(DC,iLeft,iTop,iLeft+Ws-1,iTop+Hs-1);
         SelectObject(DC,OldPen);
         SelectObject(DC,OldBrush);
         DeleteObject(Pen);
         DeleteObject(Brush);
      }
   }

//������������ ����� �����
//��������� ����
   if(init.iRamkaFormulyar)Colour=init.cText;  //��������� ����
   if(air.N_of_Propusk>0&&init.iRamkaFormulyar)
   {
       int R1,R2,G1,G2,B1,B2,Rs,Gs,Bs;
       R1=GetRValue(Colour);
       G1=GetGValue(Colour);
       B1=GetBValue(Colour);
       R2=GetRValue(Colour);
       G2=GetGValue(Colour);
       B2=GetBValue(Colour);
       Rs=(R1+R2)/2; Gs=(G1+G2)/2; Bs=(B1+B2)/2;
       Colour=RGB(Rs,Gs,Bs);
   }


//�������� �����
   Font=CreateFont(init.szTextOut,init.szTextOut*2/3,GM_COMPATIBLE,
    0,FW_REGULAR,FALSE,FALSE,FALSE,RUSSIAN_CHARSET,0,0,DEFAULT_QUALITY,
    DEFAULT_PITCH ,"Courier New");

//������� ������
   OldFont=(HFONT)SelectObject(DC,Font);

//��������� ���� ������ � ������������
   OldColour=SetTextColor(DC,Colour);

//��������� ����� ����������� ������
   OldBkMode=SetBkMode(DC,TRANSPARENT);


//����� ��������� �������� ���� ��� �� ������� ��� ���� ��� �� ������
   Yt=iTop+2;
   Xt=iLeft+2;
   realN_of_Strka=0;

//������� ��� ������ �������
   if(init.IsOutNomer)
   {
//����� �����
       if(air.type==0)
       {
              sprintf(Strka,"%05d",air.lDopNumber);
       }else if(air.rejim==1)
       {
              sprintf(Strka,"%05d",air.lDopNumber);
       }
       else if(air.rejim==2)
       {
             sprintf(Strka,"%04d ",air.lDopNumber);
       }
       TextOut(DC,Xt,Yt,Strka,MIN(5,N_of_Bukv));
       Yt+=init.szTextOut;
       realN_of_Strka++;
       if(realN_of_Strka==N_of_Strk)
       {
           SetTextColor(DC,OldColour);
           SetBkMode(DC,OldBkMode);
           SelectObject(DC,OldFont);
           DeleteObject(Font);
           return 1;
       }
   }

   if(init.IsOutVysota)
   {
//������ �����
     if(air.abs_altitude)
     {
        sprintf(Strka,"%04d",air.altitude/10);
     }else{
        sprintf(Strka,"A%03d",air.altitude/10);
     }
     TextOut(DC,Xt,Yt,Strka,MIN(4,N_of_Bukv));
     Yt+=init.szTextOut;
     realN_of_Strka++;
     if(realN_of_Strka==N_of_Strk)
     {
           SetTextColor(DC,OldColour);
           SetBkMode(DC,OldBkMode);
           SelectObject(DC,OldFont);
           DeleteObject(Font);
           return 1;
     }
   }


   if(init.IsOutTime)
   {
//����� �����
     int H,M,S;
     double M1;
     H=(int)(air.Time);
     M1=(60*(air.Time-H));
     M=(int)M1;
     S=(M1-M)*60;
     sprintf(Strka,"%02d:%02d:%02d",H,M,S);
     TextOut(DC,Xt,Yt,Strka,MIN(8,N_of_Bukv));
     Yt+=init.szTextOut;
     realN_of_Strka++;
     if(realN_of_Strka==N_of_Strk)
     {
           SetTextColor(DC,OldColour);
           SetBkMode(DC,OldBkMode);
           SelectObject(DC,OldFont);
           DeleteObject(Font);
           return 1;
     }
   }

   if(init.IsOutAzimutDalnost)
   {
//������ � ���������
      sprintf(Strka,"A%03d �%03d",Okrugl(air.azimuth), Okrugl(air.dalnost));
      TextOut(DC,Xt,Yt,Strka,MIN(9,N_of_Bukv));
      Yt+=init.szTextOut;
      realN_of_Strka++;
      if(realN_of_Strka==N_of_Strk)
      {
            SetTextColor(DC,OldColour);
            SetBkMode(DC,OldBkMode);
            SelectObject(DC,OldFont);
            DeleteObject(Font);
            return 1;
      }
   }


   if(init.IsOutSkorost)
   {
//�������� ��������
      if(air.lNomerTrassy&&init.IsSoprovojdenie)
      {
         sprintf(Strka,"V: %4.0lf",air.velocity);
      }else{
         sprintf(Strka,"V:   ? ");
      }
      TextOut(DC,Xt,Yt,Strka,MIN(7,N_of_Bukv));
      Yt+=init.szTextOut;
      realN_of_Strka++;
      if(realN_of_Strka==N_of_Strk)
      {
            SetTextColor(DC,OldColour);
            SetBkMode(DC,OldBkMode);
            SelectObject(DC,OldFont);
            DeleteObject(Font);
            return 1;
      }
   }


   if(init.IsOutOstatokTopliva)
   {
//������� �������
      sprintf(Strka,"��:%4.0lf",air.fuel);
      TextOut(DC,Xt,Yt,Strka,MIN(7,N_of_Bukv));
      Yt+=init.szTextOut;
      realN_of_Strka++;
      if(realN_of_Strka==N_of_Strk)
      {
            SetTextColor(DC,OldColour);
            SetBkMode(DC,OldBkMode);
            SelectObject(DC,OldFont);
            DeleteObject(Font);
            return 1;
      }
   }



/*
    TOcenkaVeroyatnosti *OV=(TOcenkaVeroyatnosti *)air.GetProbData;
    ������ ����������� ����
*/
/*
   if(init.IsOutVeroyatnostPervKanal&&
     air.lNomerTrassy>0)
   {

//����������� ����������� �� ���������� ������
        double P=-1;
        if(OV)
        {
           P=OV->GetPropably(0);
        }
#ifdef ENG_LANG
        if(P<0)
        {
          sprintf(Strka,"P(PC)  ?  ");
        }else{
          sprintf(Strka,"P(PC) %4.2lf",P);
        }
#else
        if(P<0)
        {
          sprintf(Strka,"P(��)  ?  ");
        }else{
          sprintf(Strka,"P(��) %4.2lf",P);
        }
#endif
        TextOut(DC,Xt,Yt,Strka,MIN(10,N_of_Bukv));
        Yt+=init.szTextOut;
        realN_of_Strka++;
        if(realN_of_Strka==N_of_Strk)
        {
            SetTextColor(DC,OldColour);
            SetBkMode(DC,OldBkMode);
            SelectObject(DC,OldFont);
            DeleteObject(Font);
            return 1;
        }

   }
   ������ ������� ����
 */
/*
   ������ �����������
   if(init.IsOutVeroyatnostVtorKanal&&
     air.lNomerTrassy>0)
   {
//����������� ����������� �� ���������� ������
      double P=-1;
      if(OV)
      {
           P=OV->GetPropably(1);
      }
#ifdef ENG_LANG
      if(P<0)
      {
        sprintf(Strka,"P(SC)  ?  ");
      }else{
        sprintf(Strka,"P(SC) %4.2lf",P);
      }
#else
      if(P<0)
      {
        sprintf(Strka,"P(��)  ?  ");
      }else{
        sprintf(Strka,"P(��) %4.2lf",P);
      }
#endif
      TextOut(DC,Xt,Yt,Strka,MIN(10,N_of_Bukv));
      Yt+=init.szTextOut;
      realN_of_Strka++;
      if(realN_of_Strka==N_of_Strk)
      {
            SetTextColor(DC,OldColour);
            SetBkMode(DC,OldBkMode);
            SelectObject(DC,OldFont);
            DeleteObject(Font);
            return 1;
      }
   }
   ������ ����������� ����
*/
   SetTextColor(DC,OldColour);
   SetBkMode(DC,OldBkMode);
   SelectObject(DC,OldFont);
   DeleteObject(Font);
   return 1;
}



int TFormulyarDraw::TestOverlap_One(TFormulyarDraw *OtherFD)
{
  int dX, dY;

//�������� ������� ����� ���� �������� ��� ������ ������� ������
//������ ������������� ��� ��������
  if(Xc<init.sShir/2||Xc>=Wscr-init.sShir/2||
     Yc<init.sVysot/2||Yc>=Wscr-init.sVysot/2)return 2;


  if(this==OtherFD)return 0;  //��� ����������� - ��� ��� �� ������

  if(!OtherFD->Busy)return 0;
  if(OtherFD->air_cust->sector30_number==air_cust->sector30_number&&
     air_cust->Time-OtherFD->air_cust->Time>TO/2)return 0;
  dX=ABS(Xc-OtherFD->Xc);
  dY=ABS(Yc-OtherFD->Yc);
  if(dX<init.sShir&&dY<init.sVysot)return 1;

//�������� ����� ������ ������ �� ������� �� �������
/*
  dX=ABS(Xc-OtherFD->X0);
  dY=ABS(Yc-OtherFD->Y0);
  if(dX<=init.sShir/2&&dY<=init.sVysot/2)return 1;

  dX=ABS(X0-OtherFD->Xc);
  dY=ABS(Y0-OtherFD->Yc);
  if(dX<=init.sShir/2&&dY<=init.sVysot/2)return 1;
*/

//�� ���������
  return 0;
}


//�����������, �� ������������� �� ��� ��������� ���� �� �����
//���������� 1, ���� ���� ��������� 2, ���� � ���� ���������� ������ �� ����� ����
int TFormulyarDraw::TestOverlap_All(void)
{
  int i,Ret;
  for(i=0;i<N_of_FD;i++)
  {
     Ret=TestOverlap_One(FD[i]);
     if(Ret)
     {
       return Ret;
     }

  }
  return 0;
}



int TFormulyarDraw::FindXcYcByDirection(void)
{
  int DX,DY;
  int P,O;

  P=iDirection/8+1,O=iDirection%8;
  if(air_cust)
  {
     O=(O+air_cust->FirstDirectionFormulyars)%8;
  }

  iLastDirection=O;

  switch(O)
  {
    case 0:
//������ �����
      Xugol=X0+20*P; Yugol=Y0+20*P;
      Xc=Xugol+init.sShir/2;
      Yc=Yugol+init.sVysot/2;
    break;

    case 1:
//����� ���
      Xugol=X0; Yugol=Y0+20*P;
      Xc=Xugol;
      Yc=Yugol+init.sVysot/2;
    break;

    case 2:
//����� �����
      Xugol=X0-20*P; Yugol=Y0+20*P;
      Xc=Xugol-init.sShir/2;
      Yc=Yugol+init.sVysot/2;
    break;

    case 3:
//�����
      Xugol=X0-20*P; Yugol=Y0;
      Xc=Xugol-init.sShir/2;
      Yc=Yugol;
    break;


    case 4:
//����� ������
      Xugol=X0-20*P; Yugol=Y0-20*P;
      Xc=Xugol-init.sShir/2;
      Yc=Yugol-init.sVysot/2;
    break;

    case 5:
//������
      Xugol=X0; Yugol=Y0-20*P;
      Xc=Xugol;
      Yc=Yugol-init.sVysot/2;
    break;

    case 6:
//������ - ������
      Xugol=X0+20*P; Yugol=Y0-20*P;
      Xc=Xugol+init.sShir/2;;
      Yc=Yugol-init.sVysot/2;
    break;

    default:
//���� ����� 7 - ������
     Xugol=X0+20*P; Yugol=Y0;
     Xc=Xugol+init.sShir/2;;
     Yc=Yugol;
  };
  return 1;
}

//����������� ����������� �� ����� � ������������ ������ ����
int TFormulyarDraw::HavePixel(int Xp, int Yp)
{
  int dX,dY;
  if(!Busy)return 0;
//���������
  dX=ABS(Xc-Xp); dY=ABS(Yc-Yp);
  if(dX<=init.sShir/2&&dY<=init.sVysot/2)return 1;

//���������
  return PixelAndLine(X0,Y0,Xugol,Yugol,Xp,Yp,1);
}



//��������� ���������
int TFormulyarDraw::PaintOne(HDC DC)
{
  LOGPEN lp;
  HPEN Pen,OldPen;
  int Xugol1, Yugol1;
  if(!Busy)return 0;
//�������� �����, ����������� ������� � ������
//��������� ������ �����
  lp.lopnWidth.x=lp.lopnWidth.y=0;
  lp.lopnStyle=PS_SOLID;

  if(air_cust->type==0)
  {
     lp.lopnColor=init.CirclePen1.lopnColor;
  }else if(air_cust->type==1)
  {
     lp.lopnColor=init.CirclePen2.lopnColor;
  }else{
     lp.lopnColor=init.CirclePen3.lopnColor;
  }

  if(air_cust->N_of_Propusk>0)
  {
    return 1;  //��������, ����� �� ���� ���������
/*
      int R1,R2,G1,G2,B1,B2,Rs,Gs,Bs;
      R1=GetRValue(lp.lopnColor);
      G1=GetGValue(lp.lopnColor);
      B1=GetBValue(lp.lopnColor);
      R2=GetRValue(init.Clrbackground);
      G2=GetGValue(init.Clrbackground);
      B2=GetBValue(init.Clrbackground);
      Rs=(R1+R2)/2; Gs=(G1+G2)/2; Bs=(B1+B2)/2;
      lp.lopnColor=RGB(Rs,Gs,Bs);
*/
  }


  Pen=CreatePenIndirect(&lp);
  OldPen=(HANDLE)SelectObject(DC,Pen);
  MoveToEx(DC,X0,Y0,NULL);
  LineTo(DC,Xugol,Yugol);
  SelectObject(DC,OldPen);
  DeleteObject(Pen);
  Xugol1=Xc-init.sShir/2;
  Yugol1=Yc-init.sVysot/2;

  formulyarTextOut(DC, Wscr, Hscr, *air_cust, init, Xugol1, Yugol1);
  if(0==air_cust->TransFormulyar)air_cust->TransFormulyar=1; //������� ������ ����� ����������
  return 1;
}




//������ ����� ������ ����������
int TFormulyarDraw::SetInitialization(
            initialization &_init,  //��������� �������������
            int _Wscr, int _Hscr    //������  � ������ ������
            )
{
   if(_Wscr<=0||_Hscr<0)return 0;
   FreeAllFormulyar();
   N_of_FD=0;
   memcpy(&init,&_init,sizeof(init));
   Wscr=_Wscr;
   Hscr=_Hscr;
   TO=init.fTempObzora/3600.0;
   return 1;
}

//�������� ��������
TFormulyarDraw* TFormulyarDraw::AddFormulyar(
                   int X, int Y,  //���������� ������� � ������� ������ (� ��������)
                   airplane *_air_cust)
{
  TFormulyarDraw **FD1, *FDRet=NULL;
  int i,j,Ret;
  if(X<0||X>=Wscr||Y<0||Y>=Hscr||_air_cust==NULL)return NULL;
  if(!init.IsOutFormulyar || !_air_cust->Formulyar)return NULL;

//������ �������� ������� ���� ����������
  if(N_of_FD)for(int i=lastFD;;)
  {
    if(!FD[i]->Busy)  //���� �� ����� ��������, ��� ���� ������
    {
       FD[i]->Busy=1;
       FDRet=FD[i];
       lastFD=i;
       break;
    }
    i=(i+1)%N_of_FD;
    if(i==lastFD)break;
  }

  if(FDRet==NULL)
  {
//�� ����� �� ����� ��������� ��������� ����������
     if(MaxN_of_FD==N_of_FD)
     {
//���� ��������� ����� ������ �� 10
        FD1=new TFormulyarDraw*[MaxN_of_FD+10];
        if(FD1)
        {
          memcpy(FD1,FD, sizeof(TFormulyarDraw*)*MaxN_of_FD);
          delete []FD;
          FD=FD1;
        }
        for(i=MaxN_of_FD,j=0;j<10;j++,i++)
        {
           FD[i]=new TFormulyarDraw;
           FD[i]->Busy=0;
        }
        MaxN_of_FD+=10;
        FD=FD1;
     }
     FDRet=FD[N_of_FD];
     lastFD=N_of_FD;
     N_of_FD++;
     FDRet->Busy=1;
     FDRet->iDirection=0;
  }
//��� ���� ��� ���������
  FDRet->X0=X; FDRet->Y0=Y;
  FDRet->air_cust=_air_cust;
  for(FDRet->iDirection=0;
         FDRet->iDirection<80;
         FDRet->iDirection+=1)
  {
    FDRet->FindXcYcByDirection();
    Ret=FDRet->TestOverlap_All();
    if(Ret==0)
    {
       _air_cust->FirstDirectionFormulyars=FDRet->iLastDirection;
       return FDRet;
    }
  }

  for(FDRet->iDirection=0;
         FDRet->iDirection<80;
         FDRet->iDirection+=1)
  {
    FDRet->FindXcYcByDirection();
    Ret=FDRet->TestOverlap_All();
    if(Ret!=2)
    {
       _air_cust->FirstDirectionFormulyars=FDRet->iLastDirection;
       return FDRet;
    }
  }
  _air_cust->FirstDirectionFormulyars=0;

  return NULL;
}



//�������� ��� �������
int TFormulyarDraw::PaintAll(HDC DC)
{
  int i;
  TFormulyarDraw *PromFD=NULL;    //������������� ��������
  if(N_of_FD)for(i=(lastFD+1)%N_of_FD;;)
  {
    if(0==FD[i]->air_cust->TransFormulyar)
    {
       if(PromFD)
       {
          PromFD->air_cust->TransFormulyar=1;
          PromFD->PaintOne(DC);
       }
       PromFD=FD[i];
    }
    FD[i]->PaintOne(DC);
    if(i==lastFD)break;
    i=(i+1)%N_of_FD;
  }
  if(PromFD)PromFD->PaintOne(DC);
  return 1;
}




//�������� ������ ����������
int TFormulyarDraw::Free(void)
{

  if(FD)
  {
    for(int i=0;i<MaxN_of_FD;i++)delete FD[i];
    delete []FD;
    FD=NULL;
  }

  N_of_FD=MaxN_of_FD=lastFD=0;
  return 1;
}


//���������� ������ ��������
int TFormulyarDraw::FreeAllFormulyar(void)
{
  for(int i=0; i<N_of_FD;i++)
  {
    FD[i]->FreeFormulyar();
  }
  return 1;
}


//�������� ��������� ����� � ���� �� ����������
int TFormulyarDraw::AnyHavePixel(int Xm,int Ym)
{
  int i,k;
  if(N_of_FD==0)return (-1);
  k=lastFD-1;
  if(k<0)k=N_of_FD-1;
  for(;;)
  {
     if(FD[k]->HavePixel(Xm,Ym))
     {
       FD[k]->air_cust->TransFormulyar=0;  //������� ��������
       return k;
     }
     if(k==lastFD)break;
     if((--k)<0)k=N_of_FD-1;

  }
  return (-1);
}


//��������� ������������� ���������
airplane* TFormulyarDraw::
               GetOpaque( int Xm, int Ym)
{
   int K=AnyHavePixel(Xm,Ym);
   if(K>=0)return FD[K]->air_cust;
   return NULL;
}



//���������� ������ ��������
int TFormulyarDraw::FreeFormulyar(void)
{
   if(!Busy)return 0;
   Busy=0;
   return 1;
}




int TFormulyarDraw::PovorotFormulyar(int Xp, int Yp,
                      int Kuda) //0 - �� ������� �������, 1 - ������
{
   int iD1,i;
   int Rad;
   if(!Busy)return 0;

   if(air_cust==NULL)return 0;

 //�������� ��������� �����
   if(air_cust->type==0)Rad=init.radius1;
   else  if(air_cust->type==1)Rad=init.radius2;
   else  if(air_cust->type==2)Rad=init.radius3;

   if(Xp<air_cust->i_x-Rad||Xp>air_cust->i_x+Rad||
      Yp<air_cust->i_y-Rad||Yp>air_cust->i_y+Rad)
   {
      return 0;
   }

   iD1=iDirection%8;
   if(Kuda==0)
   {
     air_cust->FirstDirectionFormulyars=(air_cust->FirstDirectionFormulyars+1)%8;
   }else{
     air_cust->FirstDirectionFormulyars=(air_cust->FirstDirectionFormulyars+7)%8;
   }



   for(iDirection=0;iDirection<80;iDirection+=8)
   {
     FindXcYcByDirection();
     if(!TestOverlap_All())
     {
        return 1;
     }
   }
   iDirection=0;
   FindXcYcByDirection();
   return 1;
}


//�����  � ��������� ��������
int TFormulyarDraw::NaitiIPovernut(int Xp, int Yp,
                      int Kuda)
{
  int i;
  for(i=0;i<N_of_FD;i++)
  {
    if(FD[i]->PovorotFormulyar(Xp,Yp,Kuda))return 1;
  }
  return 0;
}

