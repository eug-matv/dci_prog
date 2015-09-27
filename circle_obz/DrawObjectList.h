//---------------------------------------------------------------------------
/*���������� �������� ������ ������ ���������*/



#ifndef DrawObjectListH
#define DrawObjectListH

#include "structs.h"
#include "formulyars.h"

//---------------------------------------------------------------------------


/*����� ��������
����������� �����. �� ���� ����������� ������
                        TDrawRect - �������� ��������������
                        TDrawLine - �������� �����
                        TDrawCircle - �������� ����������� � �����
*/

class TDrawObject
{
  public:
   virtual long Paint(HDC DC,
                   POINT pntRLS,
      SIZE szArea, double dfKoefMasshtab)=0;


   virtual long PaintOneMore(HDC DC)=0;

   virtual void GetClassName(char *ClassName, long szClassName)=0;
   virtual void GetLocation(double *xl,double *yl)=0;
};


// �������������
class TDrawRect:public TDrawObject
{
//  public:
  double X0,Y0;  //���������� ������������ ���  - �� ������
  double X1,Y1;  //���������� ������ ����� ������������ ��� - �� ������
  long iX0, iY0;    //����������� ���������� ������� ������� ������������ ���
                   //���� iX0 ��� iY0<0, �� ������ �� ��������� ������ �������

  long iX1, iY1;    //������ ����������
  COLORREF Colour;  //���� ��������� �����
  UINT lopnStyle;   //����� ���������� �������
  POINT lopnWidth;
  LOGBRUSH zalivka; // �������

public:
  TDrawRect(double x1, double y1, double x2, double y2,
                     COLORREF Cvet, UINT Stil, POINT Tolshina, LOGBRUSH Brush_transmit);

  virtual long Paint(HDC DC, POINT pntRLS,
     SIZE szArea, double dfKoefMasshtab);
  virtual long PaintOneMore(HDC DC);
  virtual void GetClassName(char *ClassName, long szClassName);
  virtual void GetLocation(double *xl,double *yl);



};



// �����
class TDrawLine:public TDrawObject
{
//  public:
  double X0,Y0;  //���������� ������������ ���
  double X1,Y1;  //���������� ������ ����� ������������ ���
  long iX0, iY0;    //����������� ���������� ������� ������� ������������ ���
                   //���� iX0 ��� iY0<0, �� ������ �� ��������� ������ �������

  long iX1, iY1;    //������ ����������
  COLORREF Colour;  //���� ��������� �����
  UINT lopnStyle;   //����� ���������� �������
  POINT lopnWidth;

public:
  TDrawLine(double x1, double y1, double x2, double y2,
                     COLORREF Cvet, UINT Stil, POINT Tolshina);

  virtual long Paint(HDC DC, POINT pntRLS,
     SIZE szArea, double dfKoefMasshtab);
  virtual long PaintOneMore(HDC DC);
  virtual void GetClassName(char *ClassName, long szClassName);
  virtual void GetLocation(double *xl,double *yl);

};

// ����������
class TDrawCircle : public TDrawObject
{
  double X0,Y0;   //���������� ������������ ���
  double Radius;  //������
  long lXl,lYt,lXr,lYb; //������������� ������� ��� ����� ���������� ����������
  LOGPEN LogPen;
  LOGBRUSH LogBrush;


  public:

//�������� ������ ��� ��������� ������� �� ��������� �� ������
  TDrawCircle (double x0, double y0,  //���������� ������ ����������
            double rad, LOGPEN CirclePen, LOGBRUSH CircleBrush);

  virtual long Paint(HDC DC, POINT pntRLS,
     SIZE szArea, double dfKoefMasshtab);
  virtual long PaintOneMore(HDC DC);
  virtual void GetClassName(char *ClassName, long szClassName);
  virtual void GetLocation(double *xl,double *yl);
};


// ����





/*������ �������� ��� ��������� �����*/
class TDrawObjectList
{
//  long N_of_Objects;     //����� ��������
//  long MaxN_of_Objects;  //����������� ���������� ����� ��������
//  TDrawObject **ArrayDrawObject; //������ �������� ���� TDrawObject
 bool FreeObjectsByDelete;
 public:
  TDrawObject **ArrayDrawObject; //������ �������� ���� TDrawObject
  long N_of_Objects;     //����� ��������
  long MaxN_of_Objects;  //����������� ���������� ����� ��������
  TDrawObjectList(bool isFreeObjectsByDelete)
  {
    N_of_Objects=0;
    MaxN_of_Objects=0;
    ArrayDrawObject=(TDrawObject **)0;
    FreeObjectsByDelete=isFreeObjectsByDelete;
  }
  ~TDrawObjectList();


//���������� ������ �������
  long AddDrawObject(TDrawObject *DrawObject);

//���������
  long Paint(HDC DC,
             POINT pntRLS,
             SIZE szArea,
             double dfKoefMasshtab);


//����������� ��������
  long PaintOneMore(HDC DC);

};






#endif
