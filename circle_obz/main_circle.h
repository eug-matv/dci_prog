//---------------------------------------------------------------------------
/*��������� �.�  ��� ����� ��� ��� ��-29
  TMainCircleOut.
   ������� ����� ��������� ������� ������. ������������:
   1)  ����� ������ �������
   2) �������� ���������
   3) ���������� ����� � ������ �����

   �������� ������ � ������ �������:
   1) ��������� ��� ������� Start
   2) ����� ������� ������ � ������� ������ �� ������� ��� �� ��������� ���������
   ������� AddTarget
   3) ����� ������� Paint
   4) ��������� ����� ������� ������� SetNewOption.
   5) ����������� �������� ������� SetNewSizeAndMasshtab,
   � ��� �� ������������� �������:
     SetNewSize,   SetVnutriRamki, MultyScale, MoveRLS
   6) ��������� ������� �������� ����
MouseButtonDown, MouseButtonDowned,MouseButtonUp.
   7) ����� �������� ���� �������� �������� GetKodBedstvia, ������ ��������� ���������� �� ������� �� �������� ���� ���������, ��� ������������.  ����� ���� �������� SbrosKodBedstvia. 
   8) ��������� ������ � ������������ ������ Stop.
 */


#ifndef main_circleH
#define main_circleH

#include <windows.h>
#include "DrawObjectList.h"
#include "structs.h"
#include "airoplanelist.h"
#include "paketstruct.h"



class TMainCircleOut
{
  int IsWork;          //������ ������ ��������������� � ��������� � ������
  POINT pntRLS;        //��������� RLS �� ������

  SIZE szArea1;         //������ ������� ������ �������
  SIZE szArea;          //������ ������� ������ ����������
  double dfMasshtab;   //������� ������
  initialization init; //��������� ������������� structs.h
  TAirplaneList *AL;   //������ �������� ����� (airoplanelist.h)



//����������� ����� - ������ ��� ������� �����
  HDC DC_Setka;     //���������� ��� ������ �����
                    //����������� ����� ����� ���������� ��������
  HBITMAP bSetka,OldbSetka;   //������� ���� ��� ��������� ����� ����� � ������
                    //�������� ������ ���� �������� ������� ��� �������� ������

  HDC DCPreOutput;    //���������� �����

  HBITMAP bPreOutput,bOldPreOutput;  //������� ���� ��� �������������� ������ �����

  int IsSetka;      //���� �� �����

//��� ������ ������ �� ��� ��������� ��������� � ����� ���������
  HBITMAP bScreenBack, bOldScreenBack;
  HDC DC_ScreenBack;



//��� �� ������� ���������� ����� ��������� - ����������� � ����������� �� ��������
  int iShagDalnosti;
  int iOpornShagDalnosti;




  TDrawObjectList *DOL_rect;
                   //��� ������ ����� - ����� �������� �����

//��� ������ ��������
  stack_mashtab smPrev[20];   //��������
  int N_of_smPrev;            //����� ���������


//����� - ��������� �����
  int iPaintSelected;           //������ ��������� �����
  int X_mouse1,Y_mouse1;      //���������� ���������� ����
  int X_mouse2,Y_mouse2;      //���������� ����������� ����


  long TimeOfLastAddTarget;   //����� ���������� ���������� �������


  long DopustimPeriodV_ms;   //���������� ������ � �������������


  int N_of_PD_Bedstvie;      //���� ��������  
                             //���� ���� ��������, �� ��� �����
			     //����� ��������
                             //����� ����� 0 
                             //��������������� ������� AddTarget,
                             //���� ����� ������� � ���������
     	  		     //����� ��� �������� ����� 0
	                     //�������� ����������� ����� ������� 
                             //PaintBedstvie ������ ������� ������� 
			     //� ���������. 
                             //������������ ������� ResetBedstvie

  long flashBedstvie;	     //�������� ������� ��������. ���� 0, �� 
			     //���c��� ����, ����� - �������� ���
        
  TPaketData PD_Bedstvie[10];//����� ������ ��������
			     //����� �������� ����� N_of_PD_Bedstvie					

  char timeOutStr[100];      //������ � ������������ �������� ������ 

  bool bTimeOutVisible;      //���������� �� ����� � ����

  int MakeAndPaintSetka(HWND hwnd);

//���������� ��������
  int SaveMassharb_in_smPrev(void);



//��������� ��� �������������� ���������
  int ScreenToWorld(int Xs, int Ys,  //������
                    double &Xw, double &Yw);

//��������� ��� �������������� ���������
  int WorldToScreen(double Xw, double Yw, //������
                    int &Xs,   int &Ys);


//���������� ����� ��� �����������
  int PaintTextCircle(HDC DC, double Radius);  //������ ���������� � ��


//���������� ����� ��� �����
  int PaintTextLine(HDC DC, double Ugol);  //��� ����� ���������� ��� � ��������


//�������� ����� ��� �����
  int PaintSetkaText(HDC DC);

//�������� ����� ��������
  int PaintRazvertka(HDC DC,  double Azmt_Radian, int Left, int Top);


//�������� ����� �� ������ ����� �� ������ ������� ������
  int PaintTextError(HDC DC, char *csTextError);


public:
   int NoPackets;       //1 - ������� ����, ��� ����� ������ �� ���� �������
                        //2  - ������ �������� ����� -- ���� �� ����������
                        //3  - ������ �������� ����� - ���� �����
   int MustRestarting;  //����� ����������  - ��� ��� ������� ���������,
  // ��� ���� ���������




   TMainCircleOut():IsWork(0),bSetka(NULL),
        bTimeOutVisible(true){
        timeOutStr[0] = 0;
   };
   ~TMainCircleOut(){Stop();};
   int GetWorking()
   {
     return IsWork;
   };

//������ ������ ������
   int Start(
    HWND hwnd,            //���������� ����
    const SIZE &_szArea,         //������ ������� ������
    const initialization &_init //�������� ������������
            );

//����������
   int Restart(void);


   int Stop(void); //���������� ������

//�������� ������
   int TestPeriod(void);


//��������� �������� ������� ��� ��������� ������������
  int SetNewSizeAndMasshtab(
              POINT _pntRLS,         //��������� ���
              SIZE  _szArea,         //������ ������� ������
              double _Masshtab,      //�������
              int Backup             //����� Backup ����������
                           );


//���������� ������ �������
  int SetNewSize(SIZE  _szArea);  //������� �������


//��������� ����� �����
  int SetNewOption(initialization &_init);



//����� ������ �� �����
  int Paint(HWND hwnd,   //���������� ���������� - �� ������
            int left, int top); //������� ����� ����




//Move Down
  int MouseButtonDown(int Xm,int Ym,
                    unsigned long Key);   // 0 - �� ������ ������
                                // 1 - ����� ������
                                // 2 - ������ ������
                                // 3  - �� ������ ������

  int MouseButtonDowned(int Xm,int Ym,
                    unsigned long Key);   // 0 - �� ������ ������
                                // 1 - ����� ������
                                // 2 - ������ ������
                                // 3  - �� ������ ������

  int MouseButtonUp(int Xm, int Ym,
                       unsigned long Key);   // 0 - �� ������ ������
                                // 1 - ����� ������
                                // 2 - ������ ������
                                // 3  - �� ������ ������

//���������� ������� ������ ���������
  int SetVnutriRamki(int X11, int Y11,
                     int X22, int Y22);

//��������� ������ ������������ - �� ���� ��������� ������������ �� ��������
  int MultyScale(double Koef);


//�������� ����� ������������ ������� ����� ��� ����
  int MoveRLS(double K_Move_X,  //�������� �� �����������, >0 - RLS ������,  <0 - RLS ����� ��
              double K_Move_Y); //�������� �� �����������, >0 - RLS �����, <0 - RLS ����

//���������� ������ �������
   int AddTarget(
           TPaketData& PD //����� � �������
                );

//�������� ���������� ��� � �������� ���������
//� ������� ������ �������� (� ��������)
   int GetScreenScaleState(POINT &_pntRLS,      //���������� ���
                           int &_Left, int &_Top,      //����� ������ �����
                           int &_Right, int &_Bottom); //������ ������ �����

//�������� ���������� ��� � �������� ���������
//� ������� ������ �������� (� ��������)
   int SetScreenScaleState(const POINT &_pntRLS); //���

//�������������� ���������
  int RestoreMasshtab_from_smPrev(void);


//������ ��������
  int FullScale(void);


//��������� ���������� � ���� ��������
  int PaintBedstvie(HWND hwnd,  //���������� ����������
          int left, int top);	 //����� ������� ����	
   
//�������� ��� ��������
   void ResetBedstvia(void)
   {
      N_of_PD_Bedstvie=0;
   }


//���������� ������ �� �������� � �����
   void SetTimeOutStr(char *str)
   {
        strcpy(timeOutStr,str);
   }

//����� ������������?
   bool GetTimeOutVisisble()
   {
        return bTimeOutVisible;
   }


   void SetTimeOutVisible(bool timeOut)
   {
       bTimeOutVisible = timeOut;
   }

};





//---------------------------------------------------------------------------
#endif
