//---------------------------------------------------------------------------

#ifndef AnyToolsH
#define AnyToolsH
//---------------------------------------------------------------------------
int GetTochkiNaGranicePriam(double X1,double Y1,double X2, double Y2, //���������� �����q
                       long X1r, long Y1r, long X2r, long Y2r,  //���������� ���������������
                       long *X1ret,long *Y1ret, long *X2ret, long *Y2ret); //���������� ������������

int ObrezatPriamyeNaGranice(double X1,double Y1,double X2, double Y2, //���������� �����q
                       long X1r, long Y1r, long X2r, long Y2r,  //���������� ���������������
                       long *X1ret,long *Y1ret, long *X2ret, long *Y2ret); //���������� ������������


//����� ����������� ���������� � �����
int GetPixelsForCircleAndLine(
             double X1, double Y1, double X2, double Y2,
             double X0,  double Y0,  double radius,  //������ ����������
             double X[], double Y[], int *N_of_R);


//���������� ����� �����������
int GetPixelsForCircleAndRect(
       double X1r, double Y1r, double X2r, double Y2r, //���������� �����
       double X0,  double Y0,  double radius,  //������ ����������
       double X[], double Y[], int *N_of_R);

             


long Okrugl(double Chislo1);

//����������� �� ����� �������
int PixelAndLine(int X1,int Y1,  //������ ���������� �������
                 int X2,int Y2,  //������ ����������� �������
                 int Xp, int Yp, //���������� �����
                 int Tochnost);  //�������

#endif
