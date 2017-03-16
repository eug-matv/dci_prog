//---------------------------------------------------------------------------
#ifndef sopr_aorl_1aH
#define sopr_aorl_1aH
#include "paketstruct.h"
#include "structs.h"
//---------------------------------------------------------------------------
struct TSoprTrassa
{
   int Head;                 //������� ������. ������ �� �������� ������� ����������
   double Xe, Ye;            //������������������ �������� ��������� � ��
   double Ae, De;            //������������������ �������� ������� � ���������
   double V_Xe, V_Ye;        //������������������ �������� �������� � ��/�
   double Xi, Yi;            //����������������� �������� ��������� � ��
   double V_Xi, V_Yi;        //������������������ �������� �������� � ��/�
   double A_X, A_Y;          //������ ��������� � ��/�^2 ����������
   double Alfa_x, Beta_x;    //�������� Alfa � Beta ��� ���������� x
   double Alfa_y, Beta_y;    //�������� Alfa � Beta ��� ���������� y
   long Nomer;               //����� �����
   long Rejim;               //0 - ��, 1 - ���, 2 - RBS
   long NomerSektora;        //����� ������� �� 0 ��
   TMyDateTime DateTime;      //����� ������� ���������� ������������ �������
   TMyDateTime DateTimeSrav;  //  ��������� ������, ������� �����������
   double Koef;               //��������� �����������
   TPaketData *LastPD;        //��������� ����������� ����� ������
   long Predvaritelno;        //��������������� �� �������
   unsigned long NomerTrassy;

   struct TSoprTrassa *Next; //��������� �� ��������� ������� ������ �����
   struct TSoprTrassa *Prev; //��������� �� ���������� ������� ������ �����
//��������� �������� ������������
//���� ������ ���� �� ���� ��� ������� ��� ������. ���� ������ 100000 �� ���� � �� �������������,
//� � ��������� ��� ��������� ������������ � ����������
   double GetVesKoef(TPaketData &PD, initialization &_init, double TO);

//���������� ��������� ������ ��� ������
   int SetDataTrassy(double _Koef, TPaketData &PD, initialization &_init, double TO);


//��������� ��� ������� - ���������� ������ � ����
   int SaveDataToFileDebug(FILE *fp);


};

class TSoprTrassaList
{
   double TO;   //���� ������ � �����
   initialization init;
   TSoprTrassa *FirstTrassa;
   unsigned long N_of_Trass;

   bool ZavytazPervKanal;   //������������ ��������� �����
   int N_of_P;            //����� ������� ���������� ������

public:
   TSoprTrassaList(const initialization &_init);
   int Delete(TSoprTrassa *Trassa);
   int ZavyazatTrassu(TPaketData &PD1, TPaketData &PD2);
   int AddOtschet(TPaketData &PD);  //���������� �������
   ~TSoprTrassaList();
 //��������� ��� ������� - ���������� ������ � ����
   int SaveDataToFileDebug(FILE *fp, const TPaketData &PD);



};


#endif
