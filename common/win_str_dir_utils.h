//---------------------------------------------------------------------------
/*Модуль для работы со строками и путями к файлам*/


#ifndef win_str_dir_utilsH
#define win_str_dir_utilsH
//---------------------------------------------------------------------------
char* GetFullPathFileWithProgram(
                       char *FileName,
                       char *Strka,
                       int SizeOfStrka);


int winMakePath(char *Path);


#endif
