//---------------------------------------------------------------------------

#ifndef soprForWorkFileH
#define soprForWorkFileH
//---------------------------------------------------------------------------
/*������� ����� ������ � ������*/
  long __stdcall  soprSaveToFileString(
            char *Strka,         //������ ������ ������ ���� �� ������ 150 - ������������
            TPaketData &_PaketData,
            char *DopTextOut,//�������������� ����� ��� ������
            bool bVyvodAmpl, //�������� �������� ��� ���������� ������,
            bool bSkorostOut, //�������� ������ � ���������
            bool bDopDopInfo,      //�������� ���������� ���������� � ��������� ������ � �������
            int &SizeOfStroka      //������ ������
                          );




/*������������� ������ �� �����*/
  long __stdcall  soprGetFromStringToPaketData
                (
                char *Strka,
                TMyDateTime *LastDateTime,  //��������� ����� (�.�. ����� 0)
                TPaketData &_PaketData
                );




/*���������� ������*/


struct TSoprSaveSession
{
  char csPutKDannym[1200];  //���� � �������� �����, ��� �������� �����
                            //� ������� 01, 02, ..., 31
  char csPrefix[20];  //������� ����� ����� � ���� ������������� ����������� �����
                      //�������� plot ��� plotout ��� datasort
  char csExt[5];      //����������

  bool bDataOutDOS;   //������ ����

//����� ������
  bool bVyvodAmpl;    //�������� �������� ��� ���������� ������,
  bool bSkorostOut;   //�������� ������ � ���������
  bool bDopDopInfo;   //�������� ���������� ���������� � ��������� ������ � �������
  bool bSaveDataOtmetka;
  bool bSaveDataSever;
  bool bSaveDataSektor;
  char DopTextOut[200];   //�������������� ����� ��� ������

/*��������� �� ������� ������� �������� ������ ������������� ������� �� ������*/
   long __stdcall  (*DataToStr)(
            char *Strka,         //������ ������ ������ ���� �� ������ 150 - ������������
            TPaketData &_PaketData,
            char *DopTextOut,   //�������������� ����� ��� ������
            bool bVyvodAmpl,    //�������� �������� ��� ���������� ������,
            bool bSkorostOut,   //�������� ������ � ���������
            bool bDopDopInfo,
            int &SizeOfStroka      //������ ������
         );  //�������� ���������� ���������� � ��������� ������ � �������



/*��������� �� ��������� ������*/
   long __stdcall  (*DataFromStr)
                (
                char *Strka,            //������
                TMyDateTime *LastDateTime,  //��������� ����� (�.�. ����� 0)
                TPaketData &_PaketData
                );



/*�����������*/
  TSoprSaveSession()
  {
    lFD=-1;
    fOpen=0;
    DataToStr=soprSaveToFileString;
    DataFromStr=soprGetFromStringToPaketData;
    csPrefix[0]='o';csPrefix[1]=0;
    csExt[0]='t'; csExt[1]=0;
    DopTextOut[0]=0;
#ifdef linux
       cRazdelitel='//';
#else
       cRazdelitel='\\';
#endif

    bIsInit=false;
    bIsInitLoad=false;
    bSaveDataOtmetka=true;
    bSaveDataSever=true;
    bSaveDataSektor=false;
  };


/*����� ������� ����������� ������ �����. �� ������ ���������
  ����������: 0 - ���� ��� ��������, -1 - ���� �� ����������

���� csPutKDannym. -2 - ���� �� ������� ���������� � ����.
1- ���� ������� ��� ������
*/
  int Start(void);


/*��������� ������. ������ ������������ - �������� ��������� �� ���������
������ ������������ � ������� �����.
*/
  int Stop(void);

/* ���������� ������ � ����
*/
  int SaveDataToFile(TPaketData &_PaketData);



/*�������� ������ �� �����*/
//������ �������� �������� ������ �� �����
//  ������� ���������� 1, ���� ��  ��.
//0, ���� ��� �������� �������� ��� ������ � ����
//-1, ��� ��������� �����

  int StartLoad(char *csFileNamePath, //���� � �������� ��� �����
                TMyDateTime *FirstTime, //FirstTime - ����� ������ ��������� ������ �� �����
                                        //���� ����� ����, ��������� �����
                int iType,       //���� Type=1, �� FileNamePath - ��� ���� � ������.
                                 //             ����� ����� � ����� FileNamePath.
                                 //���� Type=2, �� FileNamePath - ��� ������ ���� � ������
                                 //             �����
                                 //� ��������� ������� ������� ������ �� �����
                                 //csPutKDannym
                TPaketData *PDO);   //������ �� �����


//������� ������ �� �����
//���������� 1 � ������ ������, 0 � ������ �� ������.
//������ ��������� ���������� ������ ����� ��������� ����������
//StartLoad. ���� ��������� ���������� � �� ������� � ������ ����� ������ ���
//������
  int GetDataFromFile(TPaketData *PDO);



  int StopLoad(void);


private:
  bool bIsInit;     //�������� ��������������� �� ������
  bool bIsInitLoad; //������ �������� �� �����
  long lFD;                 //����������� �������� �����
  FILE *fOpen;              //���������� �����

  char cRazdelitel; //����������� ���������
  int iLastHour;    //��������� ���

  int iTypeLoad;   //�������� ������

  char csFileNamePath1[1200];

  TMyDateTime LastDateTime;     //��������� ��������� ������


  TMyDateTime FirstDateTime;
  TMyDateTime LastFirstDateTime; //��������� ����� ������
                                 //����������� ����� ������� �������� ������

  TMyDateTime DTForEveryHour[32][24];  //����� � ���� ��� ������� ����
  TMyDateTime *pTempMyDateTime;        //��������� ������
  int FirstDen, FirstChas;             //������ ���� � ������ ���


//����� � ������� ����, ��� ���� ������ ������� �� ������ �������� �������
//���������� ��������� ���� FILE. ���� ������ � ���������� � �������� ����������
//������������ ����� ��������
  FILE *GetDataDirForFirstTime(char *TekPapka,
                            TMyDateTime *FirstTime,
                            int iType,         //1 - �� ����,
                                             //0 - �� ������ �������� �� �����
                            TPaketData *PDO   //����� � ������� ������� �����������
                           );

  FILE *GetDataFileForFirstTime(char *TekPapka,
                            TMyDateTime *FirstTime,
                            TPaketData *PDO   //����� � ������� ������� �����������
                           );

/*privGetDataFromFile - ������� ������ ������ �� �����*/
   int privGetDataFromFile(TPaketData *PDO);


/*��������� ������ � �������*/
   int GetDTForEveryHour(char *TekPapka, //������ ����
                         int iType);     //���� �������

};





#endif


