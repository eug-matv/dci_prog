//---------------------------------------------------------------------------
/*��� ����������� ���������������� ����������*/

#ifndef kio_exeptionsH
#define kio_exeptionsH
//---------------------------------------------------------------------------

class EIkoException
{
  public:

   char TextIskl[102];
   char ModuleName[52];     //��� ������
   char ClassName[52];      //��������� ����������
   char MethodName[52];     //��� ������ ��� ���������� ���������
   char RegExName[104];     //��� ���������
   EIkoException(char *_TextIskl,
                 char *_ModuleName,
                 char *_ClassName,
                 char *_MethodName,
                 char *_RegExName);
  void GetException(char *ExcText);
};

#endif
