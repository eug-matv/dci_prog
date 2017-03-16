//---------------------------------------------------------------------------
/*


����� TAirplaneList �������������� ������ � ������.
����� ������ �� 12 ������ �������� �� 30 ��������. ������ ���� �������� ������ ��
0 �� 11.
TAirplaneList ������������ ���� ������ ��������
int AddTarget(const TPaketData &PD, //����� � �������
                 int IsTail);  //����� �� �����
� AddNord()

� ����� �� � ������ ���� TDrawObjectList *(��.)
SetDataToDrawObjectList

*/

#ifndef airoplanelistH
#define airoplanelistH
#include "DrawObjectList.h"
#include "paketstruct.h"
#include "probality.h"

//---------------------------------------------------------------------------
//#define SECOND_VARIANT





/*������������ ��� ����������� �����. �������� ���������� � ��������� �������. ������������ ��� �������� ������, �� ���� ������� ����
������� ������. ����� ����� ������ �����������, �� � ������ ������� ��������������� ������ LastAirTail. ���� */
struct TLastAirTail
{
  long iNomerTrassy;    //����� ������
  long N_Sect30;        //����� �������
  long N_Indx;          //������ ���������� ������� � ������ �������
  long IsDelete;        //�� �������� ���� �� ���
  int   FirstDirectionFormulyars; //����������� ��� ���������

//��� ������ �� ���� ��� ����
  long N_of_P;          //����� ������� ���������� ������
  long N_of_S;          //����� ������� ���������� ������
  long N_of_PS;         //����� ������������ �������
};


class TAirplaneList
{
//��������� ������� ������
   POINT pntRLS;          //���������� ��� � ����������� ������
   SIZE szArea;           //������ ������� ������
   double dfKoefMasshtab; //������� ������

//�������� ��������
   initialization init;  //�������� ��������


//��������� ��� ������� ������� � �����������
   double last_time[12];          //����� �������� - ����� ���������� �������
   int iMaxSize_apl[12];          //������������ ������ �������
   int iSize_apl[12];             //������� ������
   airplane **apl[12];            //������ ���������  �� ������ 30 ��������� ������ - �������� ������

   airplane **temp_apl[2];         //��������� �������, ������� ��������� ����������
   int iMaxSize_temp_apl[2];      //������������ ������ (�����������������)
   int iSize_temp_apl[2];         //������� �������
   int iSect30_temp_apl[2];     //������ �������� ������������� ������
   int iCur_tempIndex;            //������� ������ ����������. ����� 0 ��� 1.


   airplane_light *apl_tail[12];       //������ �������� � ������
   int iMaxSize_apl_tail[12];          //������������ ������ �������
   int iSize_apl_tail[12];             //������� ������
   airplane_light *temp_apl_tail[2];   //��������� ������� �������� � ������ 
   int iMaxSize_temp_apl_tail[2];      //������������ ������� ��������� ��������		
   int iSize_temp_apl_tail[2];	       //������� ��������

//������ �� ��������
   TLastAirTail  *LastAirTail;		//��������� ����� ������, ����� ������� �� 0 �� 11 ��� ������ � ������� apl ���
					//���������� ����� ��������
   long N_of_LastAirTail;		//������� ��������� � �������
   long MaxN_of_LastAirTail;		//�� ������� ��������� �������� ������ ������


   double TO;                 		//���� ������ (����� ������� ����� ������)
   int iFirstSect30NoOut;       	//������ ����� �������, ������� ��� �� ����������� � ������� ������


//����������� ������ ��� ������������� ModifySector � SetDataToDrawObjectList
   CRITICAL_SECTION csForModifySwap_temp_apl;


/*������� ������*/
//�����������
   int ModifyCircle(double Time, int Sect30);


   int ModifySector(int Sect30);  //��� ��������� � 1 ������� c ������ lastTime
                                  //������� �� 1 �����


//�������� ������� ������ � ������� 
//���������: i_tempIndex1 ��������� �� ��, ��� �������� �������
//������� apl[iSect30_temp_apl[ i_tempIndex1]] �  temp_apl[ i_tempIndex1],
//apl_tail[iSect30_temp_apl[ i_tempIndex1]] �  temp_apl_tail[ i_tempIndex1].
//�� ���� ������ ��������� ������������ ������� �� ���������� ������ � ������, �����
   int Swap_temp_apl(int i_tempIndex1);


//��������� ����������
   int SummaDebug;



   int iNewView;   //����� ��� ���� ������ �������


//��� ��������
   int iKodB;
   
//������ ������� ������ ��� ��������
   airplane Kod_bedstvia_apl;  //������ ������� ������ ��� ��������



public:
   double dLastRadian;       //��������� �������� ������� � �����
   TAirplaneList(
                POINT &_pntRLS,    //��������� ���
                SIZE & _szArea,    //������ ������
                initialization & _init, //��������� �������������
                double _KoefMasshtab); //���������� ��� �������� �� �������� �
                                       //������������ ������

   ~TAirplaneList();


//���������� ������ ������� � ������. ���������� �������������� � ������ TMainCircleOut::AddTarget 
//��������: PD - ������ �� ������ ���� TPaketData
   int AddTarget(
           TPaketData& PD); //����� � �������

//���������� ������ ������
   int AddNord(double Time);


   int PaintData(HDC DC);  //����� ������ �� ���������� ����������


//��������� ��������, ��������� ��������� ��� ������� ������ - �������� ���� ���������
   int SetNewAreaStates(
                POINT &_pntRLS,         //��������� ���
                SIZE & _szArea,         //������ ������
                initialization &_init,  //��������� �������������
                double _KoefMasshtab);  //

//����������� �� ������� ������ ���� ���������
   int HaveAnyFormulyarPixel(int Xm, int Ym);

//���������� 1 ������ � ��� ��������, ������� ������
   int PaintOpaqueFormulyar(HDC DC, int Xm, int Ym);

//�������� ��������, �� ������� ������� ��� ������
 //  TFormulyar * GetFormulyar(int Xm, int Ym);


//���������� ������ ���������� ����������
   int GetNewView(void)
   {
     return iNewView;
   };


// ����� � ��������� ��������
   int FindAndRotateFormuluar(int Xp, 
                              int Yp, 
                              int Kuda);



//���� �� ��� ��������
   int GetKodBedstvia(void)  //������� ����, ��� ���� ��� ��������
   {
     return iKodB;
   };

//����� ���� ��������
   void SbrosKodBekstvia(void)
   {
      iKodB=0;
   };





};

#endif
