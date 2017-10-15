//---------------------------------------------------------------------------


//#pragma hdrstop
#include <windows.h>
#include <string.h>
#include <dir.h>
#include <io.h>
#include <fcntl.h>
#include "win_str_dir_utils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
/*��������� �������� ���� � ����� ��� �������, ��� �� ����� ��� ��� ��������� ���������
���������� ������ Strka ��� ������� ���� ��� �����, ����� ���������� 0
*/
char* GetFullPathFileWithProgram(
                       char *FileName,
                       char *Strka,
                       int SizeOfStrka)
{
   int Ret,i,Dlina;
   Ret=GetModuleFileName(0,Strka,SizeOfStrka);
   if(Ret==0)return 0;
   for(i=Ret-1;i>=0;i--)
   {
     if(Strka[i]=='\\')break;
   }
   if(i<0)return 0;
   Dlina=strlen(FileName);
   if(Dlina+Ret+1>SizeOfStrka)return 0;
   strcpy(Strka+i+1,FileName);
   return Strka;
}


/*�������� ���� ���� ��������*/

int winMakePath(const char *Path)
{
  int i,j=0;
  int D=1;
  char path1[3000];

  int Dlina=strlen(Path);
  if(Dlina > 2999)
  {
     return 0;
  }

  strcpy(path1, Path);

  char VremC;
  int Ret;
//1 - ������� �� ��� ����
  for(i=0;i<Dlina;i++)
  {
     if(path1[i]=='\\')
     {
        if(i==2)
        {
          if(path1[1]=':')
          {
             continue;
          }
        }
//�������� ������������� ����
        VremC=path1[i];
        path1[i]=0;
        if(access(Path,00))
        {
//��������� ��������
           Ret=mkdir(path1);
           if(Ret)
           {
//���� �� ������ ��� ������
              path1[i]=VremC;
              return 0;
           }
        }
        path1[i]=VremC;
     }
  }

//�������� ����
  if(access(Path,00))
  {
        Ret=mkdir(path1);
        if(Ret)
        {
//���� �� ������ ��� ������
              path1[i]=VremC;
              return 0;
        }
  }
  return 1;

};

