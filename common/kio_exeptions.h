//---------------------------------------------------------------------------
/*Для организации пользовательских исключений*/

#ifndef kio_exeptionsH
#define kio_exeptionsH
//---------------------------------------------------------------------------

class EIkoException
{
  public:

   char TextIskl[102];
   char ModuleName[52];     //Имя модуля
   char ClassName[52];      //Сообщение исключения
   char MethodName[52];     //Имя метода где исключение произошло
   char RegExName[104];     //Имя выражения
   EIkoException(char *_TextIskl,
                 char *_ModuleName,
                 char *_ClassName,
                 char *_MethodName,
                 char *_RegExName);
  void GetException(char *ExcText);
};

#endif
