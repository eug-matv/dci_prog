//---------------------------------------------------------------------------
/*Formulyars - ������������� ��� �������, ������������ � ��������� ����������*/


#ifndef formulyarsH
#define formulyarsH
//---------------------------------------------------------------------------


/*������������ ������. ������������ �� ������. ����������� ������� �����������
��������, ����� ������� �����*/
int formulyarGetText(HDC DC,               //���������� ������
                     airplane &air,        //��������� �� ��������
                     initialization &init, //��������� �������������
                     char *Texts[8],       //������ ������ - �� ����� 8 �����
                     int &N_of_Text,       // ����� ���������� �����
                     HFONT &Font           //���� ������
                     );


int formulyarTextOut(HDC DC,               //���������� ������
                     int scrW, int scrH,   //������� ������ � ��������
                     airplane &air,        //��������� �� ��������
                     initialization &init, //��������� �������������
                     int iLeft, int iTop        //���������� ������ �������� ����
                     );


class TFormulyarDraw
{
   friend class TFormulyarDraw;   //��� ������� � ��������� ������
   static TFormulyarDraw **FD;    //������ ������ ���������
   static int N_of_FD;            //����� ���������
   static int MaxN_of_FD;         //������������ ����� �������
   static initialization init;   //������ �������� ��� ������ ���������
   static int Hscr, Wscr;
   static int lastFD;             //��������� ������ ������������ �������
   static double TO;              //���� ������
   int X0, Y0;                    //���������� ����
   struct airplane *air_cust;     //������ ����
   int Xc,Yc;                     //������ ������� ������
   int Xugol, Yugol;              //���������� ���� � ������� ����� �����������
                                  //������� �� X0,Y0 �� ��������������
   int Busy;                      // 1 - ���� ����� 0 - ��������
   int iDirection;                //���������, ��� ����� ��������� �������� ������������
                                  //�������� �������.

   int iLastDirection;            //��������� ��������� ����������� ���������

//�����������, �� ������������� �� ��� ��������� ���� �� �����
//���������� 1, ���� ���� ��������� 2, ���� � ���� ���������� ������ �� ����� ����

   int TestOverlap_One(TFormulyarDraw *OtherFD);


//�����������, �� ������������� �� ��� ��������� ���� �� �����
//���������� 1, ���� ���� ��������� 2, ���� � ���� ���������� ������ �� ����� ����
   int TestOverlap_All(void);

//���������� ��������� ������ ���������
   int FindXcYcByDirection(void);

//����������� ����������� �� ����� � ������������ ������ ����
   int HavePixel(int Xp, int Yp);


//��������� �������� ������ ��� ������� �� ������ �� �������
   int PovorotFormulyar(int Xp, int Yp,
                      int Kuda); //0 - �� ������� �������, 1 - ������

 public:

//������ ����� ������ ����������
  static int SetInitialization(
            initialization &_init,  //��������� �������������
            int _Wscr, int _Hscr    //������  � ������ ������
            );

//�������� ��������
  static TFormulyarDraw* AddFormulyar(
                   int X, int Y,  //���������� ������� � ������� ������ (� ��������)
                   airplane *_air_cust);

//�������� ��� �������
  static int PaintAll(HDC DC);

//�������� ������ ����������
  static int Free(void);

//���������� ������ ��������
  static int FreeAllFormulyar(void);

//�������� ��������� ����� � ���� �� ����������
  static int AnyHavePixel(int Xm,int Ym);

//��������� ������������� ���������
  static airplane* GetOpaque( int Xm, int Ym);

//�����  � ��������� ��������
  static int NaitiIPovernut(int Xp, int Yp,
                      int Kuda);


//�������� ������� ������
  int PaintOne(HDC DC);


//���������� ������ ��������
  int FreeFormulyar(void);



};





#endif
