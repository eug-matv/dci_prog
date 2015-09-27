//---------------------------------------------------------------------------

#ifndef soprDopToolsH
#define soprDopToolsH

#define ABS(X) (((X) > 0) ? (X) : (-(X)))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))



//---------------------------------------------------------------------------

#define SET_NOMER_SEKTOR_30(A, S) S=(((short)(A))/30)



double doptools_DVremyaChas(double H1, double H2);

/*���������� ����� � ��������*/


double doptools_AbsRaznizaAzimutov(double Azmt1, double Azmt2);


/*�������� - ������ �� ������� ������� �� �������� �������.
���� ���� ����� �� ������� �� ������� ��� ������� - �����
���������� 1, ���� ����� �� �������. 0 - ���� � �������
*/
long doptools_TestVyhodZaSektor(
                                 double CurAzmt,double CurTime,    //������� �����
                                 double PrevAzmt, double PrevTime, //���������� �����
                                 double SektorAzmt,                //�������� ������� �������
                                 double TempObzora_ch);            //������ ������



/*��������� �������� �������� � ��������� �� ����������� �����������*/
long doptools_GetAzimutDalnostFromXY(
     long X_m, long Y_m,      //���������� ���������� � ������
     double &Azmt_gr, double &Dlnst_km //�������� ������� � ��������� � �������� � ��
                                    );



#endif
