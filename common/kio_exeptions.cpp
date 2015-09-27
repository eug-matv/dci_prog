//---------------------------------------------------------------------------


#pragma hdrstop

#include <string.h>
#include <stdio.h>
#include "kio_exeptions.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

EIkoException::EIkoException(
                 char *_TextIskl,
                 char *_ModuleName,
                 char *_ClassName,
                 char *_MethodName,
                 char *_RegExName)
{
   strcpy(TextIskl, _TextIskl);
   strcpy(ModuleName,_ModuleName);
   strcpy(ClassName,_ClassName);
   strcpy(MethodName,_MethodName);
   strcpy(RegExName,_RegExName);
}


void EIkoException::GetException(char *ExcText)
{
   sprintf(ExcText,"%s M:%s F:%s::%s %s ",
         TextIskl,ModuleName,ClassName,MethodName,RegExName);
}

