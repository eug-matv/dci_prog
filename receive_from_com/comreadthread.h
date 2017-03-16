//---------------------------------------------------------------------------
/*����� ������ ������ � ����� COM1-COM9.
����� TComReadThread.
����� ���������� : C++Builder. ��: Windows 2k � ����.
����������: ����� �. � 2007
�������������: ��������� �.�. 09.2007
��������� �����������: 1.10.2007.
���������� ���������� ��������� �����, ������� ��������� ������ � COM �����
� ������� ASTERIX, � ��������� ���������� ������ paketstruct, ������� ���������
TPaketData::PaketData.

�������� ���������:
�������� ������ ���������� ������ ��� ���������� ������
static TComReadThread* TComReadThread::Connect(..)
���������: int N - ����� ����� �� 1 �� 9
           int Speed - ���-���� ����� (�������� ����������).
���������� ��������� �� ������ TComReadThread � ��������� ����.
������ ����� ��������� ����������: iErrorConnect - ��� ��� ������:
���� iErrorConnect=0, ������ �������� ����� ������ ��� ������.
����� �����  ���� �� �������� "���� ������ ���������� �����" (��. ����).
���� iErrorConnect!=0, ����� ����������� ���� ����������
iGetLastError, ������� ����� iGetLastError=GetLastError() � ������ �����������
������.

int __fastcall Disconnect() - ���������� ������ ������.
��������� ��� �������� �����������. ���������� ��������� �����.
����������:1 - ���� �� �������, ����� 0


void __fastcall Execute() - �������� ��� ���������� ������. ��������� ������ �
�����. � ������ ������ ��������� ���������� dwError, �������� ������� ����������
���� ��� "���� ������ �������� ����������"





*/


#ifndef ComReadThreadH
#define ComReadThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------

/*���� ������ �������� ����������*/
#define ERR_PEREPOLENIE_BUFFERA 0x0001
#define ERR_UNKNOWN_CLEARCOMERROR 0x0002
#define ERR_PAKET_NE_RASPOZNAN 0x0004
#define ERR_PAKET_PEREPOLNEN 0x0008

/*���� ������ ���������� �����*/
//������ ���������� CreateFile
#define ERR_CONNECT_CREATE       1
//������ ���������� GetCommState
#define ERR_CONNECT_GETCOMSTATE  2

//������ ���������� SetCommState
#define ERR_CONNECT_SETCOMSTATE  3

//������ �������� ���������� ������
#define ERR_CONNECT_CREATETHREAD 4

//������ �������� �������
#define ERR_CONNECT_CREATEEVENT  5


#include "structs.h"



/*������ ������ �� Com-�����. ������ ������ � ������ � �������� ������ � ��� ����������
��� ���������� ���������.

*/

class TComReadThread;
class TComReadThread : public TThread
{
private:


//��������� COM �����

 friend  TComReadThread;
 HANDLE hCom;       //���������� �����
 DCB dcb;           //��� �������� ���������


//������� �������� � ������ - ������������ ����� �������� �����
 long N_of_P;   //����� ������� ���� P
 long N_of_U;   //����� ���
 long N_of_R;   //����� RBS


 initialization init; //������� 16.03.2017 ��� ����
protected:
        void __fastcall Execute();
         __fastcall TComReadThread(const initialization &_init );
         int __fastcall Disconnect1();

public:
       HANDLE hEventNoEmpty; //��������� ����, ��� ������ ���� � ������

       DWORD dwError;        //��� ������ �� ����� ������

       static int iErrorConnect; //��� ������ ��� ���������� ������� Connect
       static int iGetLastError; //��� ��������� ������

       static TComReadThread* Connect(
                                const initialization &init
                             );  //���������� � �����������

            //������������ �� ����������

       int __fastcall Disconnect()
       {
           Terminate();
           Sleep(1000);     //������ ������� ������ ��� ���������� �����
           CloseHandle(hEventNoEmpty);
           hEventNoEmpty=NULL;
           return 1;
       };


};
//---------------------------------------------------------------------------
#endif
