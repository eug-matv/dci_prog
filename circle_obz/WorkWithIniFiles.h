/*������ ��� �������� � ����������*/

//---------------------------------------------------------------------------


#ifndef WorkWithIniFilesH
#define WorkWithIniFilesH
//---------------------------------------------------------------------------
// �������� ���������� ���������� �� ���-�����
void Load_INI_f(struct initialization *init_struct,
                      char *PreSetupIni,
                      char *UserIni);

//
void Save_INI_f(struct initialization *init_struct,
                 char *UserIni);

void Save_INI_f_VisibleFormulyar(
   struct initialization *init_struct,
                 char *UserIni);

void Save_INI_f_VisibleTail(
   struct initialization *init_struct,
                 char *UserIni);


#endif
