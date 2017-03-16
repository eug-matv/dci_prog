/*
  �����: ��������� �.�. ��-29
������� � ����������:  KOI_AORL_1AC.exe, �������� Analisis.exe
����: 10.09.2007 �����������: 28.11.2007
� ������ ������ ������� ���������
1) TMyDateTime - ��� �������� �������,
2) TPaketData - ��� �������� ������, ����������� � COM-�����, ����� ������
3) ��� ��������������� ������ � ���������
  �������� ��������� �������� ����� � ������������ � ������� ���������� ����

  ���������� ������� TPaketData::PaketData ����� ������. ������ ������� N_OF_FIFO_PAK_STR
  TPaketData::InIndex - ��������� ������ ������� ������� ������ ���� �������
  ������ ������ ��� ���������� (TPaketData::InIndex+1)%N_OF_FIFO_PAK_STR

  TPaketData::OutIndex - ��������� ������ ������� ������� ������ ���� ������.
  ������ ������ ��� ���������� (TPaketData::OutIndex+1)%N_OF_FIFO_PAK_STR




*/


//---------------------------------------------------------------------------


#ifndef PaketStructH
#define PaketStructH

//---------------------------------------------------------------------------
#define N_OF_FIFO_PAK_STR 1000



/*�������� ���� ������*/

typedef enum
{
   TipPakOtmetka=0,   //���� ��� ������ �������
   TipPakSektor=1,    //���� ��� ������ ������
   TipPakSever=2,      //���� ��� ������ �����
   TipPakNeOpredelen=3      //���� ��� ������ �����
} TTipPaketa;



typedef enum
{
    RejimNeIzvesten=0,   //�� ��������� �����
    RejimUVD=1,          //����� ���
    RejimRBS=2          //����� RBS
} TRejimRaboty;


/*����� �� ������� ��� ������� ������ ������*/
typedef enum
{
    Kanal_P=0,      //��������� �����
    Kanal_S=1,      //��������� �����
    Kanal_PS=2      //��������� � ��������� ������
} TKanal;



struct TMyDateTime
{
   short God, Mesyac, Den;   //��� ����� ����
   double Time;              //����� � ����� �� 0 �� 24 �� �������

   int operator <(TMyDateTime mdt);
   int operator >(TMyDateTime mdt);
   int operator <=(TMyDateTime mdt);
   int operator >=(TMyDateTime mdt);

   int operator ==(TMyDateTime mdt);
   int operator !=(TMyDateTime mdt);
   TMyDateTime& operator =(const TMyDateTime &mdt);
   TMyDateTime& operator =(TMyDateTime mdt);


   TMyDateTime operator +(double dTime);
   TMyDateTime operator -(double dTime);


};



  struct TIshodDannye
  {
    long FileNumber;    //����� ����� - ���� ������ �� �����

    long Smesh;
  };



struct TPaketDataOtmetka
{

//��� �� ��� false //��� RBS               //��� ���
  bool bKodN;      //����� 3� - ���� 5_5   ���� 5_3
  bool bKodH;      //����� � - ���� 5_4    ���� 5_2



  TKanal Kanal;
                     //���� 7. ���� 1 � 2 (<����7>&0x3)
                     //��� �� ��������� ���� 5,6 ����� 8, ��� ��� ��� �����������
                     //�� ����  (7,1)=(8,5) � (7,2)=(8,6)

  TRejimRaboty Rej;   //����� ������ RejimUVD (���), RejimRBS(RBS)
                      //����:

  bool bP_LCHM;        //����� ��� ��� ���������� ������ (��� ���������� �����)
  bool bP_Mono;        //����� ���� ��� ���������� ������ (��� ���������� �����)
                       //���� ���� �� ��������

  bool bKO;           //����������� ������� (��� ��� � RBS)
                      //���� 8, ��� 2

  short sKodB;  //��� ��������, ��� RBS ��������: 0(���), 7500, 7600,7700
                //���� 9 - ���� 6 � 7.
//                ��� ��� ���� 20 ��� 7. ���� ��� ���������� �� sKodB=1 (������ ��������)



  long X,Y;     //���������� ����������. Xb - ����  10,11 �� ������.
                //Yb - ���� 12, 13 �� ������. X=Xb*15, Y=Yb*15
//��� �����
  bool bNomer;   //����������� �� ����� ���� 16, ��� 8 (��� ��� � RBS)
  long lNomer;   //����� �����. ������ ��� ���������� ������
                 //RBS: ((16,4)*4+(16,3)*2+(16,2))*1000+
                 //     ((16,1)*4+(17,8)*2+(17,7))*100+
                 //     ((17,6)*4+(17,5)*2+(17,4))*10+
                 //     (17,3)*4+(17,2)*2+(17,1)

                 //���: A, B, C, D, E ������ � �������� ������ �� 0000 �� 1001
                 //����
//      (��������<9) ((16,4)*8+(16,3)*4+(16,2)*2+(16,1))*10000+
//      (��������<9) ((17,8)*8+(17,7)*4+(17,6)*2+(17,5))*1000+
//      (��������<9) ((17,4)*8+(17,3)*4+(17,2)*2+(17,1))*100+
//      (��������<9) ((18,8)*8+(18,7)*4+(18,6)*2+(18,5))*10+
//      (��������<9) (18,4)*8+(18,3)*4+(18,2)*2+(18,1)




//��� ������
  bool bVysota;     //������������� ������
                    //��� ��� = true. ��� RBS ���� 18 ��� 8
  bool bAbsVysota;  //������� ������ (��H)
                    //��� RBS ������ true, ��� ��� ���� 20 ��� 8

  long lVysota;      //�������� ������
                    //RBS: ������� ������� ���� - 18, ���� 1 - 6(6 -��������)
                    //������� ������� ���� 19 ���� 1-8
                    //���: ������� ������� ���� - 20, ���� 1 - 6(6 -��������)
                    //������� ������� ���� 21

  bool bOstatokTopliva; //������ ��� ���. ���� 19. ���
  long lOstatokTopliva; //������� �������  - ������ ��� ���,
                       //��� RBS = 0.
                       //��� ��� ���� 19 - ���� 1 - 4


//���������������� ������
  double Dalnost;      //��������� � ��. ������������� �� X,Y
  double Vx, Vy;         //���������� �������� �������� ���������
                         //� ��/�



//�������������� ������
  double dAmpl;      //�������� ��������� ��� ���������� ������

//������������ ����� �����
  long lDopNumber, lDopVysota;  //���������� �������� ������ � ������

  long  lNomerTrassy;  //����� ������

  short IndexInObzor;         //������ ������� � ������


//������ ������� 22.11.2007
  int iTail;      //����� �� �����
  int iFormulyar; //����� �� ��������
  void *apl;     //��������� �� ��������, ���� ������������� ������
                 //�������� � ������

//��������� 20.01.2008
  long PrivyazanPredvaritelno;


//��� ������ ����������� � ����
  long N_of_PO;        //����� ������� ���������� ������
  long N_of_MustPO;    //����� ������� ���������� ������ ������� ������ ���� 
  double P_PO;
  long N_of_SO;        //����� ������� ���������� ������
  long N_of_MustSO;    //����� ������� ���������� ������ ������� ������ ����
  double P_SO;

};


/*����� ������*/
struct TPaketSektor
{


  int Dummy;



};



struct TPaketSever
{

  int Soprovojdenie;   //��������� �� ��, ��� ������������� ���� ��������. 
                       // ������������� ��� ������

};



struct TPaketData;

//�������� ������
struct TPaketData
{


  static TPaketData PaketData[N_OF_FIFO_PAK_STR];   //������ ������� ������
  static long InIndex;                 //������� ������ - ���������� ��� �����
  static long OutIndex;                //�������� ������
                                       //�� ��������

  TTipPaketa TipPaketa;  //��� ������. ����� TipPakOtmetka,
                         //                  TipPakSektor ���
                         //                  TipPakSever

//������
  union
  {
     TPaketDataOtmetka Otmetka;   //������� - �������� ����.
                               //������������� ���� TipPaketa=TipPakOtmetka

     TPaketSektor Sektor;         //������ - �������� ����
                               //������������� ���� TipPaketa=TipPakSektor

     TPaketSever  Sever;          //�����- �������� ����
                               //������������� ���� TipPaketa=TipPakSever
  };

//����� ������ ��� ���� - �����
  TMyDateTime  DateTime;  //���� � ����� - �������� ����
  unsigned char cKodStrany;      //��� ������ ���� 6
  double Azimut;                 //������
  short NomerSektoraRazmerom30;  //����� ������� ��������
                                 //���������� �� 0 �� 11
  TIshodDannye IshodDannye;   //�������� ������ - ��������� ������ �������� ������


};



//������ ���������������� ������
void ReinitializePaketData(void);


#endif
