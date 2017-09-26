//---------------------------------------------------------------------------


#pragma hdrstop

#include <math.h>
#include "lang_str.h"

#include "main_circle.h"
#include "AnyTools.h"
#include "kio_exeptions.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#define MIN(X,Y) (((X) > (Y)) ? (Y) : (X))
#define MAX(X,Y) (((X) < (Y)) ? (Y) : (X))

#define ABS(X) (((X) > 0) ? (X) : (-(X)))

//������ ������ ������
int TMainCircleOut::Start(
    HWND hwnd,            //���������� ����
    const SIZE &_szArea,         //������ ������� ������
    const initialization &_init //�������� ������������
    )
{
   int Ret;
   int iDlinaTail;
//�������� ������� ����������
   if(IsWork)return 0;
   ReinitializePaketData();
   bSetka=NULL;
   if(_szArea.cx<100||_szArea.cx>10000||
      _szArea.cy<100||_szArea.cy>10000)return (-1);
   HDC DC;
   IsSetka=0;
   NoPackets=0;

//���� ������� ����������
   memcpy(&init,&_init,sizeof(init));
   memcpy(&szArea1,&_szArea,sizeof(SIZE)); //��������
   if(init.max_dalnost<10||init.max_dalnost>10000)init.max_dalnost=400.0;

   szArea.cx=szArea1.cx-60;    szArea.cy=szArea1.cy-20;

//������ ���������
   pntRLS.x=szArea1.cx/2;
   pntRLS.y=szArea1.cy/2;
   dfMasshtab=(MIN(szArea.cx,szArea.cy))/(2.0*init.max_dalnost+2.0);

//��������� ��� ����� ��������� � ����������� �� ��������



//��������� ���������
   {
      int i;
      int M1[]={1, 2,  5,  10, 20,  50,  100};
      int M2[]={5, 10, 20, 50, 100, 200, 200};
      iShagDalnosti=1;
      iOpornShagDalnosti=5;
      iShagDalnosti=M1[sizeof(M1)/sizeof(int)-1];
      iOpornShagDalnosti=M2[sizeof(M1)/sizeof(int)-1];
      for(i=0;i<sizeof(M1)/sizeof(int);i++)
      {
         if(M1[i]*dfMasshtab>=30)
         {
           iShagDalnosti=M1[i];
           iOpornShagDalnosti=M2[i];
           break;
         }
      }

   };



//���������� ��������� ������ ��� �����, ��������� ����� �� ������ ���������
   Ret=MakeAndPaintSetka(hwnd);
   if(Ret<=0)
   {
//������
     return Ret;
   
   }


   if(init.iTail)iDlinaTail=init.iTailLength;
   else iDlinaTail=0;
   AL=new TAirplaneList(pntRLS, szArea1, init, dfMasshtab);

#ifdef ENG_LANG  
   if(AL==NULL)
    throw EIkoException ("Memory allocation error",
     "main_circle.cpp","TMainCircleOut","Start","AL=new TAirplaneList(...");
#else
if(AL==NULL)
    throw EIkoException ("������ ��������� ������",
     "main_circle.cpp","TMainCircleOut","Start","AL=new TAirplaneList(...");
#endif
   N_of_smPrev=0;
   iPaintSelected=0;

   DopustimPeriodV_ms=(long)(init.fTempObzora*1500);//���������� ������ � �������������

//��������� ����� � �������
   TimeOfLastAddTarget=GetTickCount();


   IsWork=1; //��������� ����� ������
   N_of_PD_Bedstvie=0;  //��� ���������� ��������

   
   return 1;
}


//����������
int TMainCircleOut::Restart(void)
{
   if(!IsWork)return 0;
   if(AL)delete AL;
   AL=new TAirplaneList(pntRLS, szArea1, init, dfMasshtab);

#ifdef ENG_LANG   
   if(AL==NULL)
    throw EIkoException ("Memory allocation error",
     "main_circle.cpp","TMainCircleOut","Restart","AL=new TAirplaneList(...");
#else
   if(AL==NULL)
    throw EIkoException ("������ ��������� ������",
     "main_circle.cpp","TMainCircleOut","Restart","AL=new TAirplaneList(...");
#endif
   N_of_PD_Bedstvie=0;
   
   ReinitializePaketData(); 
   return 1;

}






//���������� ������
int TMainCircleOut::Stop(void)
{
   if(!IsWork)return 0;
   if(bSetka)
   {
      SelectObject(DC_Setka,OldbSetka);
      DeleteDC(DC_Setka);
      DeleteObject(bSetka);
      SelectObject(DCPreOutput,bOldPreOutput);
      DeleteDC(DCPreOutput);
      DeleteObject(bPreOutput);
      SelectObject(DC_ScreenBack,bOldScreenBack);
      DeleteDC(DC_ScreenBack);
      DeleteObject(bScreenBack);
      bSetka=NULL;
   }
   delete AL;  AL=NULL;
   IsWork=0;
   return 1;
}



//��������� �������� ������� ��� ��������� ������������
int TMainCircleOut::SetNewSizeAndMasshtab(
              POINT _pntRLS,        //��������� ���
              SIZE  _szArea,         //������ ������� ������
              double _Masshtab,      //�������
              int Backup             //����� Backup
             )
{
  int Koord1,Koord2;
  int dX,dY;
  if(!IsWork)return 0;  //�� �������� ���������


//������ ����, ����� ������� ������ �� �������� ���������
  if(_szArea.cx-60>_szArea.cy-20)
  {
     if(2*init.max_dalnost*_Masshtab<_szArea.cy-20)
     {
//��������� �������
       _Masshtab=(_szArea.cy-20)/(2.0*init.max_dalnost);
     }
  }else{
     if(2*init.max_dalnost*_Masshtab<_szArea.cx-60)
     {
//��������� �������
       _Masshtab=(_szArea.cx-60)/(2.0*init.max_dalnost);
     }
  }

//������������ �� ������������ ���������
//1 - �� ��� x
  Koord1=_pntRLS.x-_Masshtab*init.max_dalnost;
  Koord2=_pntRLS.x+_Masshtab*init.max_dalnost;

  if(Koord1>30)
  {
    if(Koord2<_szArea.cx-30)
    {

       _pntRLS.x=_szArea.cx/2;
    }else{
      dX=Koord1-29;
      if(Koord2-dX<_szArea.cx-31)  _pntRLS.x=_szArea.cx/2;
      else _pntRLS.x-=dX;
    }
  }else if(Koord2<_szArea.cx-31)
  {
    if(Koord1>=30)
    {
       _pntRLS.x=_szArea.cx/2;
    }else{
       dX=_szArea.cx-Koord2-30;
       if(Koord1+dX>30)_pntRLS.x=_szArea.cx/2;
       else _pntRLS.x+=dX;
    }
  }
//2 - �� ��� y
  Koord1=_pntRLS.y-_Masshtab*init.max_dalnost;
  Koord2=_pntRLS.y+_Masshtab*init.max_dalnost;

  if(Koord1>10)
  {
    if(Koord2<_szArea.cy-10)
    {
       _pntRLS.y=_szArea.cy/2;
    }else{
      dY=Koord1-10;
      if(Koord2-dY<_szArea.cy-10)  _pntRLS.y=_szArea.cy/2;
      else _pntRLS.y-=dY;
    }
  }else if(Koord2<_szArea.cy-10)
  {
    if(Koord1>10)
    {
       _pntRLS.y=_szArea.cy/2;
    }else{
       dY=_szArea.cy-Koord2-11;
       if(Koord1+dY>10)_pntRLS.y=_szArea.cy/2;
       else _pntRLS.y+=dY;
    }
  }


//��������, � ������ �������� �� ���������
  if(memcmp(&_szArea, &szArea1, sizeof(szArea))||
     memcmp(&_pntRLS, &pntRLS, sizeof(pntRLS))||
     ABS(dfMasshtab-_Masshtab)>0.00000001)
  {
     if(Backup)SaveMassharb_in_smPrev();  //���������� ���������
//���� �������� ��������
     memcpy(&szArea1, &_szArea, sizeof(szArea));
     memcpy(&pntRLS, &_pntRLS, sizeof(pntRLS));
     szArea.cx=szArea1.cx-60;      szArea.cy=szArea1.cy-20;
     dfMasshtab=_Masshtab;
//��������� ��� ����� ��������� � ����������� �� ��������
   {
      int i;
      int M1[]={1, 2,  5,  10, 20,  50,  100};
      int M2[]={5, 10, 20, 50, 100, 200, 200};
      iShagDalnosti=1;
      iOpornShagDalnosti=5;
      iShagDalnosti=M1[sizeof(M1)/sizeof(int)-1];
      iOpornShagDalnosti=M2[sizeof(M1)/sizeof(int)-1];
      for(i=0;i<sizeof(M1)/sizeof(int);i++)
      {
         if(M1[i]*dfMasshtab>=30)
         {
           iShagDalnosti=M1[i];
           iOpornShagDalnosti=M2[i];
           break;
         }
      }
   };

//���������� ��������� ������ ��� �����, ��������� ����� �� ������ ���������
//��������� ��� ����� ���
     IsSetka=0;

//������ ��� ����������� ���������� ��� ���� ��������
     AL->SetNewAreaStates(pntRLS, szArea1, init, dfMasshtab);
  }

  return 1;
}


//���������� ������ ������� - ������������ ��� ��������� ���������
int TMainCircleOut::SetNewSize(SIZE  _szArea)  //������� �������
{
  double dfNewMasshtab,Koef;
  double dX,dY;
  double k_X, k_Y;
  POINT pntNewRLS;   //����� ���������� ���
  if(!IsWork)return 0;

  k_X=((double)(_szArea.cx))/szArea1.cx;
  k_Y=((double)(_szArea.cy))/szArea1.cy;
  if(k_X>k_Y)Koef=k_Y;
  else Koef=k_X;

//������� ������ �������������� � �����������
  dX=szArea1.cx/2-pntRLS.x;
  dY=szArea1.cy/2-pntRLS.y;
  dX*=Koef; dY*=Koef;
  pntNewRLS.x=pntRLS.x-Okrugl(dX);
  pntNewRLS.y=pntRLS.y-Okrugl(dY);
  dfNewMasshtab=Koef*dfMasshtab;
  return SetNewSizeAndMasshtab(pntNewRLS,_szArea,dfNewMasshtab,0);
}


//��������� ����� �����
int TMainCircleOut::SetNewOption(initialization &_init)
{
  if(memcmp(&init,&_init,sizeof(init)))
  {
     memcpy(&init,&_init,sizeof(init));
     AL->SetNewAreaStates(pntRLS, szArea, init, dfMasshtab);
     IsSetka=0;
  }
  return 1;
}



int TMainCircleOut::Paint(HWND hwnd,   //���������� ���������� - �� ������
            int left, int top) //������� ����� ����
{
   HDC DC;            //��������� ����������
   int Xr,Yr;
   int Ret;
   int IsSetka1;       //��������� ��������
   if(!IsWork)return 0;

//


//�������� ������� ���������� �����
   if(0==(IsSetka1=IsSetka))
   {
      Ret=MakeAndPaintSetka(hwnd);
//������
      if(Ret!=1)
      {
         return -10+Ret;
      }

   }


   DC=GetDC(hwnd);
   if(DC==NULL)
   {
//������
      return (-1);
   }



//������� ����� �� ��� ����������
   if(0==IsSetka1||AL->GetNewView())
   {
//������� ����� � DC_Setka �� DC_ScreenBack


       BitBlt(DC_ScreenBack, 0, 0, szArea1.cx, szArea1.cy, DC_Setka, 0, 0, SRCCOPY);
//�������� ������� � ��������� (���� ����� �����)
       AL->PaintData(DC_ScreenBack);
   }

   BitBlt(DCPreOutput, 0, 0, szArea1.cx, szArea1.cy, DC_ScreenBack, 0, 0, SRCCOPY);


 //���������� ������ � ����� ������������ �����
   if(iPaintSelected)
   {
      AL->PaintOpaqueFormulyar(DCPreOutput, X_mouse2, Y_mouse2);
   }



//������� ����� ��������� ���� ���� �� ����������
   if(iPaintSelected)
   {
      int X11,Y11,X22,Y22;
      HPEN Pen, OldPen;
      HBRUSH Brush, OldBrush;
      LOGBRUSH lb={BS_HOLLOW,0,0};
      int OldMode;

      X11=MAX(0,MIN(X_mouse1,X_mouse2));
      X22=MIN(szArea1.cx-1,MAX(X_mouse1,X_mouse2));
      Y11=MAX(0,MIN(Y_mouse1,Y_mouse2));
      Y22=MIN(szArea1.cy-1,MAX(Y_mouse1,Y_mouse2));
      Brush=CreateBrushIndirect(&lb);
      if(!Brush)
      {
#ifdef ENG_LANG
throw EIkoException ("Error graphical output",
"main_circle.cpp","TMainCircleOut","Paint",
"Brush=CreateBrushIndirect(&lb);");
#else
throw EIkoException ("������ ������������ ������",
"main_circle.cpp","TMainCircleOut","Paint",
"Brush=CreateBrushIndirect(&lb);");
#endif
      }
      OldBrush=(HBRUSH)SelectObject(DCPreOutput,Brush);
      if(!OldBrush)
      {
#ifdef ENG_LANG
throw EIkoException ("Error graphical output",
"main_circle.cpp","TMainCircleOut","Paint",
"OldBrush=(HBRUSH)SelectObject(DCPreOutput,Brush);");
#else
throw EIkoException ("������ ������������ ������",
"main_circle.cpp","TMainCircleOut","Paint",
"OldBrush=(HBRUSH)SelectObject(DCPreOutput,Brush);");
#endif
      }
      Pen=CreatePenIndirect(&(init.Scale));
      if(!Pen)
      {
#ifdef ENG_LANG
throw EIkoException ("Error graphical output",
"main_circle.cpp","TMainCircleOut","Paint",
"Pen=CreatePenIndirect(&(init.Scale));");
#else
throw EIkoException ("������ ������������ ������",
"main_circle.cpp","TMainCircleOut","Paint",
"Pen=CreatePenIndirect(&(init.Scale));");
#endif
      }

      OldPen=(HPEN)SelectObject(DCPreOutput,Pen);
      if(!OldPen)
      {
throw EIkoException ("Error graphical output",
"main_circle.cpp","TMainCircleOut","Paint",
"OldPen=(HPEN)SelectObject(DCPreOutput,Pen);");
      }

      OldMode=SetBkMode(DCPreOutput,TRANSPARENT);
      Rectangle(DCPreOutput,X11,Y11,X22,Y22);
//      RECT Rect={X11,Y11,X22,Y22};
//      InvertRect(DCPreOutput,&Rect);
//      Rectangle(DCPreOutput,X_mouse1,Y_mouse1,
//                            X_mouse2,Y_mouse2);
      SetBkMode(DCPreOutput,OldMode);
      if(!SelectObject(DCPreOutput,OldPen))
      {
#ifdef ENG_LANG
throw EIkoException ("Error graphical output",
"main_circle.cpp","TMainCircleOut","Paint",
"if(!SelectObject(DCPreOutput,OldPen))");
#else
throw EIkoException ("������ ������������ ������",
"main_circle.cpp","TMainCircleOut","Paint",
"if(!SelectObject(DCPreOutput,OldBrush))");
#endif
      }
      if(!SelectObject(DCPreOutput,OldBrush))
      {
#ifdef ENG_LANG
throw EIkoException ("Error graphical output",
"main_circle.cpp","TMainCircleOut","Paint",
"if(!SelectObject(DCPreOutput,OldBrush))");
#else
throw EIkoException ("������ ������������ ������",
"main_circle.cpp","TMainCircleOut","Paint",
"if(!SelectObject(DCPreOutput,OldBrush))");
#endif
      }
      DeleteObject(Pen);
      DeleteObject(Brush);
   }

//�������� ����� ���������
//���������� ���������� ���������
   Xr=init.max_dalnost*sin(AL->dLastRadian);
   Yr=init.max_dalnost*cos(AL->dLastRadian);

// ����� �������
   HFONT hFont, oldHFont;
   if(bTimeOutVisible)
   {
           hFont = CreateFont(24,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,
             DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
               CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,
               VARIABLE_PITCH,TEXT("Courier New"));
           if(hFont)
           {
                SetTextColor(DCPreOutput, RGB(255,255,255));
                SetBkMode(DCPreOutput, TRANSPARENT);
                oldHFont=(HFONT)SelectObject(DCPreOutput, hFont);
                TextOutA(DCPreOutput,10,10,timeOutStr,  strlen(timeOutStr));
                SelectObject(DCPreOutput, oldHFont);
                DeleteObject(hFont);
           }
    }
//���������� ���� �������� ����� �� �������
   BitBlt(DC,left,top,szArea1.cx,szArea1.cy,DCPreOutput,0,0,SRCCOPY);
//����

   if(NoPackets==1)
   {
//������� ���������
//      PaintTextError(DC,"�� ��������� ������!");

   }



  ReleaseDC(hwnd,DC);

  return 1;
}


int TMainCircleOut::MouseButtonDown(int Xm,int Ym,
                    unsigned long Key)   // 0 - �� ������ ������
                                // 1 - ����� ������
                                // 2 - ������ ������
                                // 3  - �� ������ ������
{
  if(!IsWork)return 0;  //�� �������� ������

//����������� - �������� �� ������ ��
  if(Key==1)
  {
    AL->FindAndRotateFormuluar(Xm,Ym,0);
  }else if(Key==2)
  {
    AL->FindAndRotateFormuluar(Xm,Ym,1);
  }


  if(Key==1)
  {
//���� ������ ����� ������
//     AL->HaveAnyFormulyarPixel(Xm,Ym);  ���� ������
     iPaintSelected=1;  //������ ���������
     X_mouse1=Xm;
     Y_mouse1=Ym;
     X_mouse2=Xm;
     Y_mouse2=Ym;
  }
  return 1;
}

//�������� ������� �������
int TMainCircleOut::MouseButtonDowned(int Xm,int Ym,
                    unsigned long Key)   // 0 - �� ������ ������
                                // 1 - ����� ������
                                // 2 - ������ ������
                                // 3  - �� ������ ������
{
  if(!IsWork)return 0;  //�� �������� ������

//��������� � �� ������ �� ����� ������
  if(Key&1)
  {
//���� ������ ����� ������
    AL->HaveAnyFormulyarPixel(Xm,Ym);
    if(iPaintSelected)
    {
       X_mouse2=Xm;
       Y_mouse2=Ym;
    }
  }
  return 1;
}


int TMainCircleOut::MouseButtonUp(int Xm,int Ym,
                    unsigned long Key)   // 0 - �� ������ ������
                                // 1 - ����� ������
                                // 2 - ������ ������
                                // 3  - �� ������ ������
{
  if(!IsWork)return 0;  //�� �������� ������

  if(Key==1)  //���� ����� ������ ���� ������
  {
    if(iPaintSelected)
    {
       X_mouse2=Xm;
       Y_mouse2=Ym;
       iPaintSelected=0;
       if(X_mouse1>=szArea1.cx)X_mouse1=szArea1.cx-1;
       if(X_mouse2>=szArea1.cx)X_mouse2=szArea1.cx-1;
       if(Y_mouse1>=szArea1.cy)Y_mouse1=szArea1.cy-1;
       if(Y_mouse2>=szArea1.cy)Y_mouse2=szArea1.cy-1;
       if(X_mouse1<0)X_mouse1=0;
       if(X_mouse2<0)X_mouse2=0;
       if(Y_mouse1<0)Y_mouse1=0;
       if(Y_mouse2<0)Y_mouse2=0;
       if(ABS(X_mouse1-X_mouse2)<10||
          ABS(Y_mouse1-Y_mouse2)<10)return 1;

       if(X_mouse1<X_mouse2&&Y_mouse1<Y_mouse2)
       {
          SetVnutriRamki(X_mouse1,Y_mouse1,X_mouse2,Y_mouse2);
       }else{
          RestoreMasshtab_from_smPrev();
       }
    }
  }
  return 1;  
}


//���������� ������� ������ ���������

int TMainCircleOut::SetVnutriRamki(
                     int X11, int Y11,   //������ �����
                     int X22, int Y22)   //������ �����
{
  double NewKoef,dfNewMasshtab;   //����� �����������
  double dX,dY;
  int Xc,Yc;
  POINT pntNewRLS;  //����� ���
  SIZE szNewArea;   //����� �������� ������� - ��� ����� �������� ������� � �������
  if(!IsWork)return 0;  //�� �������� ������

  if(X11>=X22||Y11>=Y22)return 0;
  if(ABS(X22-X11)<10||ABS(Y22-Y11)<10)return 0;
  dX=((double)szArea1.cx)/(X22-X11);
  dY=((double)szArea1.cy)/(Y22-Y11);
  if(dX>dY)NewKoef=dY;
  else NewKoef=dX;
  dfNewMasshtab=NewKoef*dfMasshtab;
  if(szArea1.cx/(dfNewMasshtab)<=10.0||
     szArea1.cy/(dfNewMasshtab)<=10.0)return 0;

//������ ����� ������������� ������� ���������
  Xc=(X11+X22)/2;   Yc=(Y11+Y22)/2;

//������ ���������� � �������� �� ��� �� Xc � Yc ��� ������ ����������
  dX=Xc-pntRLS.x;   dY=Yc-pntRLS.y;

//���������� ����� ������� ���������
  dX*=NewKoef;   dY*=NewKoef;

//��� ��� ����� ������ ��������� � ������ ���������� ����� ��������� ���
  pntNewRLS.x=szArea1.cx/2-Okrugl(dX);
  pntNewRLS.y=szArea1.cy/2-Okrugl(dY);
  szNewArea.cx=szArea1.cx;  szNewArea.cy=szArea1.cy;

//��������� ����� ����������
  SetNewSizeAndMasshtab(pntNewRLS, szNewArea,dfNewMasshtab,1);
  return 1;

}

//��������� ������ ������������ - �� ���� ��������� ������������ �� ��������
int TMainCircleOut::MultyScale(double Koef)
{
   double NewKoef,dfNewMasshtab;   //����� �����������
   double dX,dY;
   int Xc,Yc;
   POINT pntNewRLS;  //����� ���
   SIZE szNewArea;   //����� �������� ������� - ��� ����� �������� ������� � �������
   if(!IsWork)return 0;  //�� �������� ������

   if(Koef<=0.0000001)return 0;
   dfNewMasshtab=Koef*dfMasshtab;
   if(szArea1.cx/(dfNewMasshtab)<10.0||
      szArea1.cy/(dfNewMasshtab)<10.0)
   {
      if(szArea1.cy<szArea1.cx)Koef=szArea1.cy/10.0/dfMasshtab;
      else Koef=szArea1.cx/10.0/dfMasshtab;
      dfNewMasshtab=Koef*dfMasshtab;
   }
/*
   if(szArea1.cx/(dfNewMasshtab)>2*init.max_dalnost||
      szArea1.cy/(dfNewMasshtab)>2*init.max_dalnost)
   {
     if(szArea1.cy<szArea1.cx)Koef=1.0*szArea1.cy/(2*init.max_dalnost)/dfMasshtab;
      else Koef=1.0*szArea1.cx/(2*init.max_dalnost)/dfMasshtab;
      dfNewMasshtab=Koef*dfMasshtab;
   }
  */
  Xc=szArea1.cx/2;   Yc=szArea1.cy/2;

//������ ���������� � �������� �� ��� �� Xc � Yc ��� ������ ����������
  dX=Xc-pntRLS.x;   dY=Yc-pntRLS.y;

//���������� ����� ������� ���������
  dX*=Koef;   dY*=Koef;

  pntNewRLS.x=szArea1.cx/2-Okrugl(dX);
  pntNewRLS.y=szArea1.cy/2-Okrugl(dY);
  szNewArea.cx=szArea1.cx;  szNewArea.cy=szArea1.cy;

//��������� ����� ����������
  SetNewSizeAndMasshtab(pntNewRLS, szNewArea,dfNewMasshtab,1);
  return 1;

}

//�������� ����� ������������ ������� ����� ��� ����
int TMainCircleOut::MoveRLS(
            double K_Move_X,  //�������� �� �����������, >0 - RLS ������,  <0 - RLS ����� ��
            double K_Move_Y) //�������� �� �����������, >0 - RLS �����, <0 - RLS ����
{
   SIZE szNewArea;
   POINT pntNewRLS;
   if(!IsWork)return 0;  //�� �������� ������

   szNewArea.cx=szArea1.cx;  szNewArea.cy=szArea1.cy;
   pntNewRLS.x=pntRLS.x;    pntNewRLS.y=pntRLS.y;
   pntNewRLS.x+=szNewArea.cx*K_Move_X;
   pntNewRLS.y+=szNewArea.cy*K_Move_Y;

 //��������� ����� ����������
  SetNewSizeAndMasshtab(pntNewRLS, szNewArea,dfMasshtab,1);
  return 1;

}


//���������� ������ �������
int TMainCircleOut::AddTarget(
            TPaketData& PD //����� � �������
                             )
{
  long LastTime;
  if(!IsWork)return 0;  //�� �������� ������
  TimeOfLastAddTarget=GetTickCount(); //��������� ����� �������

  if(NoPackets)
  {
//��� �� ���� � ��� ���� ���� �������� - � ������������� ������
     NoPackets=0;
     Restart();   //������������
  }


//�������� ���� ��������
  if(PD.TipPaketa==TipPakOtmetka&&PD.Otmetka.sKodB>0)
  {
     if(N_of_PD_Bedstvie==10)
     {
        memmove(PD_Bedstvie,PD_Bedstvie+1,9*sizeof(TPaketData));
        memcpy(PD_Bedstvie+9,&PD,sizeof(TPaketData));
     }else{
        memcpy(PD_Bedstvie+N_of_PD_Bedstvie,&PD,sizeof(TPaketData));
        N_of_PD_Bedstvie++;
     }
//����� �������� ���� �� ��� ���� � �������� ������� ������
     if(N_of_PD_Bedstvie>1)
     {
        for(int i=0;i<N_of_PD_Bedstvie-1;i++)
        {
          if(PD_Bedstvie[i].Otmetka.lNomerTrassy>0&&
              PD_Bedstvie[i].Otmetka.lNomerTrassy==PD.Otmetka.lNomerTrassy)
          {
             memmove(PD_Bedstvie,PD_Bedstvie+1,(N_of_PD_Bedstvie-1-i)*sizeof(TPaketData));
             N_of_PD_Bedstvie--;
             break;
          }
        }
     }
  }
  AL->AddTarget(PD);
  return 1;
}

//�������� ���������� ��� � �������� ���������
//� ������� ������ �������� (� ��������)
int TMainCircleOut::GetScreenScaleState(POINT &_pntRLS,      //���������� ���
                           int &_Left, int &_Top,     //����� ������ �����
                           int &_Right, int &_Bottom) //������ ������ �����
{
   if(!IsWork)return 0;
   int Radius=Okrugl(init.max_dalnost*dfMasshtab);
   memcpy(&_pntRLS,&pntRLS,sizeof(POINT));
   _Left=+Radius;
   _Right=-Radius;
   _Top=+Radius;
   _Bottom=-Radius;

   return 1;
}

//�������� ���������� ��� � �������� ���������
//� ������� ������ �������� (� ��������)
int TMainCircleOut::SetScreenScaleState(const POINT &_pntRLS) //���
{
   SIZE szNewArea;
   if(!IsWork)return 0;
   memcpy(&szNewArea, &szArea1, sizeof(szArea1));
 //��������� ����� ����������
   SetNewSizeAndMasshtab(_pntRLS, szNewArea,dfMasshtab,1);
   return 1;
}


//�������� ������
int TMainCircleOut::TestPeriod(void)
{
   long LastTimeMS;
   if(!IsWork)return 0;
   LastTimeMS=GetTickCount();
   if(LastTimeMS-TimeOfLastAddTarget>DopustimPeriodV_ms)
   {
      if(NoPackets==0)
      {

         NoPackets=1;
      }
   }
   if(NoPackets>0)
   {
    if(init.abuse_if_none)
    {
      Restart();
      FullScale();
    }  
   }
   return NoPackets;
}



/*����� ��������� �� ������*/





/*******************��������� �������************************************/

int TMainCircleOut::MakeAndPaintSetka(HWND hwnd)
{
  HDC DC;
  HDC CompDC;
  int i;
  TDrawObjectList *DOL_Setka;  //��� ������ �����
  LOGBRUSH lbEmpty={BS_HOLLOW, 0,0},
            lbArea={BS_SOLID,0,0};

  POINT pntRLS1;
  LOGPEN lpArea={PS_SOLID,{0,0},0};
  HBRUSH Brush, OldBrush;
  HPEN Pen, OldPen;
  int X1,Y1;  //���������� �����
  HBITMAP CompBitmap, OldCompBitmap;   //������������� � ������ ������ ��� ��������������

  if(bSetka)
  {
    SelectObject(DC_Setka,OldbSetka);
    DeleteDC(DC_Setka);
    DeleteObject(bSetka);
    SelectObject(DCPreOutput,bOldPreOutput);
    DeleteDC(DCPreOutput);
    DeleteObject(bPreOutput);
    SelectObject(DC_ScreenBack,bOldScreenBack);
    DeleteDC(DC_ScreenBack);
    DeleteObject(bScreenBack);
  }
  DC=GetDC(hwnd);
  if(DC==NULL)return (-3);  //������ �������� �����������
  bSetka=CreateCompatibleBitmap(DC,szArea1.cx,szArea1.cy);
  if(bSetka==NULL)
  {
//������
     ReleaseDC(hwnd,DC);
     return (-4);
  }
  DC_Setka=CreateCompatibleDC(DC);
  if(DC_Setka==NULL)
  {
//������
     ReleaseDC(hwnd,DC);
     DeleteObject(bSetka);
     return (-5);
  }



  OldbSetka=(HBITMAP)SelectObject(DC_Setka,bSetka);
  if(OldbSetka==NULL)
  {
//������
     DeleteObject(bSetka);
     DeleteDC(DC_Setka);
     ReleaseDC(hwnd,DC);
     
     return (-6);
  }


   bPreOutput=CreateCompatibleBitmap(DC,szArea1.cx,szArea1.cy);
   if(bPreOutput==NULL)
   {
      SelectObject(DC_Setka, OldbSetka);
      DeleteObject(bSetka);
      DeleteDC(DC_Setka);
      ReleaseDC(hwnd,DC);
      return (-7);
   }

   DCPreOutput=CreateCompatibleDC(DC);  //�������� ������������ ����������
   if(bPreOutput==NULL)
   {
      DeleteObject(bPreOutput);
      SelectObject(DC_Setka, OldbSetka);
      DeleteObject(bSetka);
      DeleteDC(DC_Setka);
      ReleaseDC(hwnd,DC);
      return (-8);
   }


   bOldPreOutput=(HBITMAP)SelectObject(DCPreOutput,bPreOutput);
   if(bOldPreOutput==NULL)
   {
//������
      DeleteDC(DCPreOutput);
      DeleteObject(bPreOutput);
      SelectObject(DC_Setka, OldbSetka);
      DeleteObject(bSetka);
      DeleteDC(DC_Setka);
      ReleaseDC(hwnd,DC);
      return (-9);
   }


//� ������ ����� ���������� �� ������
//������ ����������
  DOL_Setka=new   TDrawObjectList(true);  //����������� ����� ���������� ���������
  if(!DOL_Setka)
  {
//������
     SelectObject(DC_Setka,OldbSetka);
     DeleteObject(bSetka);
     DeleteDC(DC_Setka);
     ReleaseDC(hwnd,DC);
     return (-7);
  }

//���������� ��� ������ ����������� ������
  CompBitmap=CreateCompatibleBitmap(DC,szArea.cx,szArea.cy);
  if(!CompBitmap)
  {
//������
     delete DOL_Setka;
     SelectObject(DC_Setka,OldbSetka);
     DeleteObject(bSetka);
     DeleteDC(DC_Setka);
     ReleaseDC(hwnd,DC);
     return (-8);
  }

  CompDC=CreateCompatibleDC(DC);
  if(!CompDC)
  {
//������
     DeleteObject(CompBitmap);
     delete DOL_Setka;
     SelectObject(DC_Setka,OldbSetka);
     DeleteObject(bSetka);
     DeleteDC(DC_Setka);
     ReleaseDC(hwnd,DC);
     return (-9);
  }

  OldCompBitmap=(HBITMAP)SelectObject(CompDC, CompBitmap);
  if(!OldCompBitmap)
  {
     DeleteDC(CompDC);
     DeleteObject(CompBitmap);
     delete DOL_Setka;
     SelectObject(DC_Setka,OldbSetka);
     DeleteObject(bSetka);
     DeleteDC(DC_Setka);
     ReleaseDC(hwnd,DC);
     return (-10);
  }

//�������� ����� �������� ��������� ������ ��� ������ ������
  DC_ScreenBack=CreateCompatibleDC(DC);
  if(!DC_ScreenBack)
  {
     SelectObject(CompDC,OldCompBitmap);
     DeleteDC(CompDC);
     DeleteObject(CompBitmap);
     delete DOL_Setka;
     SelectObject(DC_Setka,OldbSetka);
     DeleteObject(bSetka);
     DeleteDC(DC_Setka);
     ReleaseDC(hwnd,DC);
     return (-11);
  }

//�������� Bitmap ��� ������ ������
  bScreenBack=CreateCompatibleBitmap(DC,szArea1.cx,szArea1.cy);
  if(!bScreenBack)
  {
     DeleteDC(DC_ScreenBack);
     SelectObject(CompDC,OldCompBitmap);
     DeleteDC(CompDC);
     DeleteObject(CompBitmap);
     delete DOL_Setka;
     SelectObject(DC_Setka,OldbSetka);
     DeleteObject(bSetka);
     DeleteDC(DC_Setka);
     ReleaseDC(hwnd,DC);
     return (-12);
  }

//������ ������� ������� bScreenBack � �������� ��������� DC_ScreenBack
  bOldScreenBack=(HBITMAP)SelectObject(DC_ScreenBack,bScreenBack);
  if(!bOldScreenBack)
  {
     DeleteObject(bScreenBack);
     DeleteDC(DC_ScreenBack);
     SelectObject(CompDC,OldCompBitmap);
     DeleteDC(CompDC);
     DeleteObject(CompBitmap);
     delete DOL_Setka;
     SelectObject(DC_Setka,OldbSetka);
     DeleteObject(bSetka);
     DeleteDC(DC_Setka);
     ReleaseDC(hwnd,DC);
     return (-13);

  }



  ReleaseDC(hwnd,DC);

//������ ����������
  for(i=iShagDalnosti;i<=init.max_dalnost;i+=iShagDalnosti)
  {
//������ ����������
     if(i%iOpornShagDalnosti)
     {
//������ ������� �����
        DOL_Setka->AddDrawObject(new TDrawCircle(0,0,i,init.DCircle,lbEmpty));
     }else{
//������ ������� �����
        DOL_Setka->AddDrawObject(new TDrawCircle(0,0,i,init.Circle,lbEmpty));
     }
  }

//������ ������ ������������ �����
  for(i=0;i<360;i+=init.azimuth_delta)
  {
     X1=Okrugl(2*init.max_dalnost*sin(i/180.0*M_PI));
     Y1=Okrugl(2*init.max_dalnost*cos(i/180.0*M_PI));
     if(i%90)
     {
//�������������� ������
       DOL_Setka->AddDrawObject(new TDrawLine(0,0,X1,Y1,
               init.DLines.lopnColor,
               init.DLines.lopnStyle,
               init.DLines.lopnWidth));
     }else{
//�������� �����
        DOL_Setka->AddDrawObject(new TDrawLine(0,0,X1,Y1,
               init.Lines.lopnColor,
               init.Lines.lopnStyle,
               init.Lines.lopnWidth));
     }
  }

//�������� ������� � ��������� ���� init.Clrbackground;
  lpArea.lopnColor=lbArea.lbColor=init.Clrbackground;
  Brush=CreateBrushIndirect(&lbArea);
  Pen=CreateBrushIndirect(&lbArea);
  OldBrush=(HBRUSH)SelectObject(DC_Setka, Brush);
  OldPen=(HBRUSH)SelectObject(DC_Setka, Pen);
  Rectangle(DC_Setka,0,0,szArea1.cx, szArea1.cy);
  SelectObject(DC_Setka, OldBrush);
  SelectObject(DC_Setka, OldPen);

//�������� ������� � ��������� ���� init.Clrbackground;
  lpArea.lopnColor=lbArea.lbColor=init.Clrbackground;
  OldBrush=(HBRUSH)SelectObject(CompDC, Brush);
  OldPen=(HBRUSH)SelectObject(CompDC, Pen);
  Rectangle(CompDC,0,0,szArea.cx, szArea.cy);
  SelectObject(CompDC, OldBrush);
  SelectObject(CompDC, OldPen);
  DeleteObject(Pen);
  DeleteObject(Brush);

  pntRLS1.x=pntRLS.x-30;
  pntRLS1.y=pntRLS.y-10;


//����� �����
  DOL_Setka->Paint(CompDC, pntRLS1, szArea, dfMasshtab);
  delete DOL_Setka;  //������ �����

//����� ����� �� DC_Setka
  BitBlt(DC_Setka, 30, 10, szArea.cx, szArea.cy, CompDC, 0, 0, SRCCOPY);

//������� ����� ��� �����
  PaintSetkaText(DC_Setka);

  IsSetka=1;
//�������� �������� ������ ��������
  SelectObject(CompDC,OldCompBitmap);
  DeleteObject(CompBitmap);
  DeleteDC(CompDC);

  return 1;
}





//�������� ����� �� ������ ����� �� ������ ������� ������
int TMainCircleOut::PaintTextError(HDC DC,  //����������
                               char *csTextError) //��������� ���������
{
   HFONT Font,OldFont;
   COLORREF OldCol,OldBkColor,W_Symb, H_Symb;
   int OldBkMode;
   int DlinaText;
   DlinaText=strlen(csTextError);

   W_Symb=szArea1.cx/(DlinaText+1);
   H_Symb=W_Symb/2*3;
   Font=CreateFont(H_Symb,W_Symb,GM_COMPATIBLE,
    0,FW_REGULAR,FALSE,FALSE,FALSE,RUSSIAN_CHARSET,0,0,DEFAULT_QUALITY,
    DEFAULT_PITCH ,"Courier New");

   OldFont=(HFONT)SelectObject(DC,Font);
   if(!OldFont)
   {
      DeleteObject(Font);
      return 0;
   }

   OldCol=SetTextColor(DC,RGB(255,0,0));
   OldBkMode=SetBkMode(DC,OPAQUE);
   OldBkColor=SetBkColor(DC,init.Clrbackground);
   
//������� �����
   TextOut(DC,0,szArea1.cx/2-H_Symb,csTextError,DlinaText);
   SetTextColor(DC,OldCol);
   SetBkMode(DC,OldCol);
   SetBkColor(DC,OldBkColor);
   return 1;
}




//���������� �������� � ����
int TMainCircleOut::SaveMassharb_in_smPrev(void)
{
   if(N_of_smPrev==20)
   {
      memmove(smPrev,smPrev+1,19*sizeof(stack_mashtab));
      smPrev[N_of_smPrev-1].RLS.x=pntRLS.x;
      smPrev[N_of_smPrev-1].RLS.y=pntRLS.y;
      smPrev[N_of_smPrev-1].Koef=dfMasshtab;
   }else{
      smPrev[N_of_smPrev].RLS.x=pntRLS.x;
      smPrev[N_of_smPrev].RLS.y=pntRLS.y;
      smPrev[N_of_smPrev].Koef=dfMasshtab;
      N_of_smPrev++;
   }
   return 1;
}


//�������� ����� ��������
int TMainCircleOut::PaintRazvertka(HDC DC, double Azmt_Radian, int Left, int Top)
{
   double X1,Y1,X11,Y11;
   double X2,Y2,X22,Y22;
   double Xk,Yk;

   X1=pntRLS.x; Y1=pntRLS.y;
   Xk=init.max_dalnost*sin(Azmt_Radian); Yk=init.max_dalnost*cos(Azmt_Radian);
   X2=pntRLS.x+Xk*dfMasshtab; Y2=pntRLS.y-Yk*dfMasshtab;

//������� ���, ����� �� ������� ������ ����� �������

}


//�������������� ��������
int TMainCircleOut::RestoreMasshtab_from_smPrev(void)
{
  initialization _init;
  SIZE _szArea;
  if(N_of_smPrev==0)return 0;

  memcpy(&_szArea,&szArea1,sizeof(SIZE));
  memcpy(&_init,&init,sizeof(init));
  SetNewSizeAndMasshtab(smPrev[N_of_smPrev-1].RLS,
                    _szArea,smPrev[N_of_smPrev-1].Koef,0);

  N_of_smPrev--;
  return 1;
}




//��������� ��� �������������� ���������
int TMainCircleOut::ScreenToWorld(int Xs, int Ys,  //������
                             double &Xw, double &Yw)
{
  Xw =(Xs-pntRLS.x)/dfMasshtab;
  Yw=(pntRLS.y-Ys)/dfMasshtab;
  return 1;
}

//��������� ��� �������������� ���������
int TMainCircleOut::WorldToScreen(double Xw, double Yw, //������
                    int &Xs,   int &Ys)
{
  Xs=pntRLS.x+Okrugl(Xw*dfMasshtab);
  Ys=pntRLS.y-Okrugl(Yw*dfMasshtab);
  return 1;
}


/*��������������� �������, ������� ������ �������� �� ����� ������� ������. */
//���������� ��� ���������� �����
int TMainCircleOut::PaintTextCircle(HDC DC, double Radius)  //������ ����������
{
  double Xr[2],Yr[2];
  int iXr, iYr;
  int N_of_R;
  double f_left, f_top, f_right, f_bottom;
  char StrkaOut[100];
  int DlinaStrkaOut;

//1 ���� -- ��������� �� ����� �������


//������� ���������� � �������� �����������
  ScreenToWorld(30,10,f_left,f_top);
  ScreenToWorld(szArea1.cx-30,szArea1.cy-10,f_right,f_bottom);
  sprintf(StrkaOut,"%d", Okrugl(Radius));
  DlinaStrkaOut=strlen(StrkaOut);

//����� ������
  N_of_R=GetPixelsForCircleAndLine(f_left, f_top, f_right, f_top,
                                   0,0,Radius,Xr,Yr,NULL);
  for(int i=0;i<N_of_R;i++)
  {
      WorldToScreen(Xr[i],Yr[i],iXr, iYr);
//������� ������ - � �������
      TextOut(DC,iXr-DlinaStrkaOut*7/2,0,StrkaOut,DlinaStrkaOut);
  }

//����� �����
  N_of_R=GetPixelsForCircleAndLine(f_left,f_bottom, f_right, f_bottom,
                                   0,0,Radius,Xr,Yr,NULL);
  for(int i=0;i<N_of_R;i++)
  {
      WorldToScreen(Xr[i],Yr[i],iXr, iYr);
//������� ������ - � �������
      TextOut(DC,iXr-DlinaStrkaOut*7/2,szArea1.cy-10,StrkaOut,DlinaStrkaOut);
  }


//������
  N_of_R=GetPixelsForCircleAndLine(f_right,f_top, f_right, f_bottom,
                                   0,0,Radius,Xr,Yr,NULL);

  for(int i=0;i<N_of_R;i++)
  {
      WorldToScreen(Xr[i],Yr[i],iXr, iYr);
//������� ������ - � �������
      TextOut(DC,szArea1.cx-30,iYr-5,StrkaOut,DlinaStrkaOut);
  }

//����� �����
  sprintf(StrkaOut,"%4d", Okrugl(Radius));
  DlinaStrkaOut=strlen(StrkaOut);
  N_of_R=GetPixelsForCircleAndLine(f_left,f_top, f_left, f_bottom,
                                   0,0,Radius,Xr,Yr,NULL);

   for(int i=0;i<N_of_R;i++)
  {
      WorldToScreen(Xr[i],Yr[i],iXr, iYr);
//������� ������ - � �������
      TextOut(DC,0,iYr-5,StrkaOut,DlinaStrkaOut);
  }


//2 - �� ����������� ����� �� ������� ���������
  sprintf(StrkaOut,"%d", Okrugl(Radius));
  DlinaStrkaOut=strlen(StrkaOut);
  WorldToScreen(0,Radius,iXr,iYr);
  if(iXr>=30&&iXr<=szArea1.cx-30&&
     iYr>=10&&iYr<=szArea1.cy-10)
  {
     TextOut(DC,iXr-DlinaStrkaOut*7/2,iYr-5,StrkaOut,DlinaStrkaOut);
  }

  WorldToScreen(Radius,0,iXr,iYr);
  if(iXr>=30&&iXr<=szArea1.cx-30&&
     iYr>=10&&iYr<=szArea1.cy-10)
  {
     TextOut(DC,iXr-DlinaStrkaOut*7/2,iYr-5,StrkaOut,DlinaStrkaOut);
  }

  WorldToScreen(0,-Radius,iXr,iYr);
  if(iXr>=30&&iXr<=szArea1.cx-30&&
     iYr>=10&&iYr<=szArea1.cy-10)
  {
     TextOut(DC,iXr-DlinaStrkaOut*7/2,iYr-5,StrkaOut,DlinaStrkaOut);
  }

  WorldToScreen(-Radius,0,iXr,iYr);
  if(iXr>=30&&iXr<=szArea1.cx-30&&
     iYr>=10&&iYr<=szArea1.cy-10)
  {
     TextOut(DC,iXr-DlinaStrkaOut*7/2,iYr-5,StrkaOut,DlinaStrkaOut);
  }
  return 1;
}


//���������� ����� ��� �����
int TMainCircleOut::PaintTextLine(HDC DC, double Ugol)  //��� ����� ���������� ��� � ��������
{
   long tX1,tY1, tX2, tY2;         //������������ �����
   double ftX1, ftY1, ftX2, ftY2;  //
   int StrkaOutDlina;
   char StrkaOut[100];
   double MaxDlina=1.5*init.max_dalnost*dfMasshtab;
   int Ret;
   ftX1=pntRLS.x; ftY1=pntRLS.y;

   ftX2=ftX1+1.5*MaxDlina*sin(Ugol/180.0*M_PI);
   ftY2=ftY1-1.5*MaxDlina*cos(Ugol/180.0*M_PI);

//������ ����������� � ������
   Ret=ObrezatPriamyeNaGranice( ftX1,ftY1,ftX2, ftY2,
                                30,10, szArea1.cx-30,szArea1.cy-10,
                                &tX1,&tY1,&tX2,&tY2);

   if(!Ret)return 0;

//���������� � ������ ������
//��������� ��� ��� ���������
   if(tX1<=30+1&&tY1>=10&&tY1<=szArea1.cy-10)
   {
//������� ����� �����
      sprintf(StrkaOut,"%4d�", Okrugl(Ugol));
      StrkaOutDlina=strlen(StrkaOut);
      TextOut(DC,0,tY1-5,StrkaOut,StrkaOutDlina);
   }else if(tY1<=10+1&&tX1>=30&&tX1<=szArea1.cx-30)
   {
//������� ����� ������
     sprintf(StrkaOut,"%d�", Okrugl(Ugol));
     StrkaOutDlina=strlen(StrkaOut);
     TextOut(DC,tX1-StrkaOutDlina*7/2,0,StrkaOut,StrkaOutDlina);
   }else if(tX1>=szArea1.cx-30-1&&tY1>=10&&tY1<=szArea1.cy-10)
   {
//������� ����� ������
     sprintf(StrkaOut,"%d�", Okrugl(Ugol));
     StrkaOutDlina=strlen(StrkaOut);
     TextOut(DC,szArea1.cx-30,tY1-5,StrkaOut,StrkaOutDlina);
   }else if(tY1>=szArea1.cy-10-1&&tX1>=30&&tX1<=szArea1.cx-30)
   {
//������� ����� �����
     sprintf(StrkaOut,"%d�", Okrugl(Ugol));
     StrkaOutDlina=strlen(StrkaOut);
     TextOut(DC,tX1-StrkaOutDlina*7/2,szArea1.cy-10,StrkaOut,StrkaOutDlina);
   }

//������� ������ �����
   if(tX2<=30+1&&tY2>=10&&tY2<=szArea1.cy-10)
   {
//������� ����� �����
      sprintf(StrkaOut,"%4d�", Okrugl(Ugol));
      StrkaOutDlina=strlen(StrkaOut);
      TextOut(DC,0,tY2-5,StrkaOut,StrkaOutDlina);
   }else if(tY2<=10+1&&tX2>=30&&tX2<=szArea1.cx-30)
   {
//������� ����� ������
     sprintf(StrkaOut,"%d�", Okrugl(Ugol));
     StrkaOutDlina=strlen(StrkaOut);
     TextOut(DC,tX2-StrkaOutDlina*7/2,0,StrkaOut,StrkaOutDlina);
   }else if(tX2>=szArea1.cx-30-1&&tY2>=10&&tY2<=szArea1.cy-10)
   {
//������� ����� ������
     sprintf(StrkaOut,"%d�", Okrugl(Ugol));
     StrkaOutDlina=strlen(StrkaOut);
     TextOut(DC,szArea1.cx-30,tY2-5,StrkaOut,StrkaOutDlina);
   }else if(tY2>=szArea1.cy-10-2&&tX2>=30&&tX2<=szArea1.cx-30)
   {
//������� ����� �����
     sprintf(StrkaOut,"%d�", Okrugl(Ugol));
     StrkaOutDlina=strlen(StrkaOut);
     TextOut(DC,tX2-StrkaOutDlina*7/2,szArea1.cy-10,StrkaOut,StrkaOutDlina);
   }

   return 1;
}


//�������� ����� �����
int TMainCircleOut::PaintSetkaText(HDC DC)
{
  HFONT Font,OldFont;
  int OldMode,i;
  COLORREF OldColor;

//�������� �����
  Font=CreateFont(12,8,GM_COMPATIBLE,
    0,FW_REGULAR,FALSE,FALSE,FALSE,RUSSIAN_CHARSET,0,0,DEFAULT_QUALITY,
    DEFAULT_PITCH ,"Courier New");

  OldFont=(HFONT)SelectObject(DC,Font);
  OldMode=SetBkMode(DC, TRANSPARENT);

  OldColor=SetTextColor(DC,init.cDalnostText);
//�������� ������ ��� �����������
  for(i=iOpornShagDalnosti;i<=init.max_dalnost+0.1;i+=iOpornShagDalnosti)
  {
    PaintTextCircle(DC,i);
  }

//�������� ������ ��� ���������� ������������ �����
  SetTextColor(DC,init.cAzimuthText);
  for(i=0;i<360;i+=init.azimuth_delta)
  {
     PaintTextLine(DC,i);
  }
  SetTextColor(DC,OldColor);
  SetBkMode(DC, OldMode);
  SelectObject(DC,OldFont);
  DeleteObject(Font);
  return 1;
}

//������ ��������
int TMainCircleOut::FullScale(void)
{
  SIZE szNewArea;   //������
  POINT pntNewRLS;  //
  double dfNewMasshtab;
  if(!IsWork)return 0;
  memcpy(&szNewArea, &szArea1, sizeof(SIZE));
  pntNewRLS.x=szNewArea.cx/2;
  pntNewRLS.y=szNewArea.cy/2;
  dfNewMasshtab=(MIN(szArea.cx,szArea.cy))/(2.0*init.max_dalnost+2.0);

//��������� ����� ����������
  SetNewSizeAndMasshtab(pntNewRLS, szNewArea,dfNewMasshtab,1);
  return 1;
}




//��������� ���������� � ���� ��������
//������ ��������� ������ ���������� �� �������. ��� �������������
//
int TMainCircleOut::PaintBedstvie(HWND hwnd,  //���������� ����������
          int left, int top)	 //����� ������� ����
{
   HDC DC;            //��������� ����������

   LOGPEN lp={PS_SOLID,{0,0},RGB(175,0,0)};        //�����
   LOGBRUSH lb={BS_SOLID,0,RGB(175,0,0)};
   HPEN Pen, OldPen;
   HBRUSH Brush, OldBrush;
   int X_paint, Y_paint;  //���������� ������ ��������� �����
   if(N_of_PD_Bedstvie<=0) //���� ����� �������� �� ������ 0, �� �����
   {
     return 0;
   }
   DC=GetDC(hwnd);
   if(DC==NULL)
   {
//������
//
     return (-1);
   }
   if(flashBedstvie)
   {
      lp.lopnColor=RGB(0, 0, 175);
      lb.lbColor=RGB(0, 0, 175);
   }

   flashBedstvie=(flashBedstvie+1)%2;
   Pen=CreatePenIndirect(&lp);
   Brush=CreateBrushIndirect(&lb);
   OldPen=(HPEN)SelectObject(DC, Pen);
   OldBrush=(HBRUSH)SelectObject(DC, Brush);
   for(int i=0;i<N_of_PD_Bedstvie;i++)
   {
//�������� ���������� ������ ������ �������, ������� �������� ��� ��������
     X_paint=pntRLS.x+Okrugl(dfMasshtab*PD_Bedstvie[i].Otmetka.X*0.001*dfMasshtab);
     Y_paint=pntRLS.y-Okrugl(dfMasshtab*PD_Bedstvie[i].Otmetka.Y*0.001*dfMasshtab);
     Ellipse(DC, X_paint-10, Y_paint-10, X_paint+10, Y_paint+10);
   }

   SelectObject(DC,OldPen);
   SelectObject(DC,OldBrush);
   DeleteObject(Pen);
   DeleteObject(Brush);
   ReleaseDC(hwnd,DC);
   return 1;
}






