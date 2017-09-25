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
/*Получение путлного пути к файлу при условии, что он лежит там где программа находится
Возвращает строку Strka при условии того что нашел, иначе возврашает 0
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


/*Создание пути если возможно*/

int winMakePath(char *Path)
{
  int i,j=0;
  int D=1;
  int Dlina=strlen(Path);
  char VremC;
  int Ret;
//1 - разбить на под пути
  for(i=0;i<Dlina;i++)
  {
     if(Path[i]=='\\')
     {
        if(i==2)
        {
          if(Path[1]=':')
          {
             continue;
          }
        }
//Проверим существование пути
        VremC=Path[i];
        Path[i]=0;
        if(access(Path,00))
        {
//Попробуем создатть
           Ret=mkdir(Path);
           if(Ret)
           {
//Если не удачно был создан
              Path[i]=VremC;
              return 0;
           }
        }
        Path[i]=VremC;
     }
  }
  
//Создадим путь  
  if(access(Path,00))
  {
        Ret=mkdir(Path);
        if(Ret)
        {
//Если не удачно был создан
              Path[i]=VremC;
              return 0;
        }
  }
  return 1;

};

