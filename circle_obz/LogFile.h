/*�����: ��������� �.�.  ��� ����� ��� ��� ��-29

*/
//---------------------------------------------------------------------------

#ifndef LogFileH
#define LogFileH
#include <stdlib.h>
#include "structs.h"

//---------------------------------------------------------------------------
class LOGFILE_IKO
{
  static LOGFILE_IKO *LogFile;   //���� �������


  char PathLogFileName[1000]; //���� � ���-�����
  char FileName[20];
  initialization init;
  FILE *fp;
  int LastSec,LastMin, LastHour;  //����

  int LastDay,LastMonth, LastYear;  //����
  int GetDate(int *Y, int *M, int *D);

//������������� ���� ��� ������ ������
  int DataOut(int IsData, char *Text);

  //��������, � �� ���� �� ��������� ������ ������
  int NewDay(int Year, int Month, int Day, int H, int M, int S); //������� �� ��������� ����


//����������� ������
  CRITICAL_SECTION CS;


public:
//���������� 1, ���� �������
  static int Init_StaticLog(
    char *FN,               //��� ����� � �����������, ������� ����� ������������� �� ����
    initialization &_init); //��������� �������������

  static int Init_StaticFree(void);



  static LOGFILE_IKO* GetStaticLog(void);

  LOGFILE_IKO();
  ~LOGFILE_IKO();



//��������� ���������
  int Start(
    char *FN,              //��� ����� � �����������, ������� ����� ������������� �� ����
    initialization &_init, //��������� �������������
    int View_init          //�������� �� ������ � ���������
  );

  int Stop();       //��������� ������



//������� ������ ����� �������������
  int Init_Out(void);


//������� ������ �� ������
  int OutCriticalError(
                 char *_TextIskl,
                 char *_ModuleName,
                 char *_ClassName,
                 char *_MethodName,
                 char *_RegExName); //��������� �� ����������� ������ - ������ ���������� � �������� ���������


//��������� ��������� �� ������
  int OutCriticalError(
                char *ClassName,
                char *Message);

//������� ������� ����������. ���������� � �������� �������� �� �����.
//� ������
  int OutWorkInfo(char *Mess);    //���������� � ������ ������� ����������

//���������� � ������������� ������
  int OutNotCriticalError(char *Mess);
                                     

//���������� � ����� ������ ����� - ���������� ������������� � ������.
  int OutChangeOption(const initialization &_init);


//��������, ������� ������ ����������� ������ 5-10 ������
//�������� �� ��������� �����
  int TestNextDay(void);

};

#endif
