//---------------------------------------------------------------------------

#ifndef soprForWorkFileH
#define soprForWorkFileH

#include <string>
#include <fstream>


using namespace std;

//---------------------------------------------------------------------------

/*���������� ������*/

struct TSoprSaveSession
{
//���� � �������� �����, ��� �������� �����
  string csPutKDannym;
                            //� ������� 01, 02, ..., 31

//������� ����� ����� � ���� ������������� ����������� �����
                      //�������� plot ��� plotout ��� datasort
  string csPrefix;
  string csExt;      //����������

  bool bDataOutDOS;   //������ ����

//����� ������
  bool bVyvodAmpl;    //�������� �������� ��� ���������� ������,
  bool bSkorostOut;   //�������� ������ � ���������
  bool bDopDopInfo;   //�������� ���������� ���������� � ��������� ������ � �������
  bool bSaveDataOtmetka;
  bool bSaveDataSever;
  bool bSaveDataSektor;
  string DopTextOut;   //�������������� ����� ��� ������ �



/*�����������*/
  TSoprSaveSession()
  {

#ifdef linux
       cRazdelitel='//';
#else
       cRazdelitel='\\';
#endif

    bIsInit=false;
    bSaveDataOtmetka=true;
    bSaveDataSever=true;
    bSaveDataSektor=false;
  };


/*����� ������� ����������� ������ �����. �� ������ ���������
  ����������: 0 - ���� ��� ��������, -1 - ���� �� ����������

���� csPutKDannym. -2 - ���� �� ������� ���������� � ����.
1- ���� ������� ��� ������
*/
  int Start(void);


/*��������� ������. ������ ������������ - �������� ��������� �� ���������
������ ������������ � ������� �����.
*/
  int Stop(void);

/* ���������� ������ � ����
*/
  int SaveDataToFile(TPaketData &_PaketData);


private:
  bool bIsInit;     //�������� ��������������� �� ������
  fstream fstrm;    //���������� �������� �����
  char cRazdelitel; //����������� ��������� - ������� � ������
  int iLastHour;    //��������� ���
  
};





#endif


