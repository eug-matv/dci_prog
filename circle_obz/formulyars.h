//---------------------------------------------------------------------------
/*Formulyars - предназначена для выводов, формирования и рисование формуляров*/


#ifndef formulyarsH
#define formulyarsH
//---------------------------------------------------------------------------


/*Формирование текста. Выравнивание по ширине. Определение сколько понадобится
символов, чтобы вывести текст*/
int formulyarGetText(HDC DC,               //Дексриптер экрана
                     airplane &air,        //Указатель на аэроплан
                     initialization &init, //Структура инициализации
                     char *Texts[8],       //Строки текста - не более 8 строк
                     int &N_of_Text,       // Число выведенных строк
                     HFONT &Font           //Фонт текста
                     );


int formulyarTextOut(HDC DC,               //Дескриптер экрана
                     int scrW, int scrH,   //Размеры экрана в пикселях
                     airplane &air,        //Указатель на аэроплан
                     initialization &init, //Структура инициализации
                     int iLeft, int iTop        //координаты левого верхнего угла
                     );


class TFormulyarDraw
{
   friend class TFormulyarDraw;   //Для доступа к жлементам класса
   static TFormulyarDraw **FD;    //Массив данных формуляра
   static int N_of_FD;            //Число элементов
   static int MaxN_of_FD;         //Максимальное число отметок
   static initialization init;   //Данные настроек для вывода формуляра
   static int Hscr, Wscr;
   static int lastFD;             //Последний индекс добавленного отсчета
   static double TO;              //Темп обзора
   int X0, Y0;                    //Координаты цели
   struct airplane *air_cust;     //Данные цели
   int Xc,Yc;                     //Сентор области вывода
   int Xugol, Yugol;              //Координаты угла с которым будет соединяться
                                  //отрезок от X0,Y0 до прямоугольника
   int Busy;                      // 1 - если занят 0 - свободен
   int iDirection;                //Указывает, где будет находится формуляр относительно
                                  //Текущего отсчета.

   int iLastDirection;            //Последнее выбранное направление формуляра

//Определение, не накладываются ли два формуляра друг на друга
//Возвращает 1, если есть наложение 2, если в этом напралении ничего не может быть

   int TestOverlap_One(TFormulyarDraw *OtherFD);


//Определение, не накладываются ли два формуляра друг на друга
//Возвращает 1, если есть наложение 2, если в этом напралении ничего не может быть
   int TestOverlap_All(void);

//Нахождение координат центра формуляра
   int FindXcYcByDirection(void);

//Определение принадлежит ли точка с координатами данной цели
   int HavePixel(int Xp, int Yp);


//Повернуть формуляр налево или направо по щелчку по отметке
   int PovorotFormulyar(int Xp, int Yp,
                      int Kuda); //0 - по часовой стрелке, 1 - против

 public:

//Начало ввода данных формуляров
  static int SetInitialization(
            initialization &_init,  //Структура инициализации
            int _Wscr, int _Hscr    //Ширина  и высота экрана
            );

//Добавить формуляр
  static TFormulyarDraw* AddFormulyar(
                   int X, int Y,  //Координаты отметки в области вывода (в пикселях)
                   airplane *_air_cust);

//Рисовать все объекты
  static int PaintAll(HDC DC);

//Очистить память формуляров
  static int Free(void);

//Освободить данный формуляр
  static int FreeAllFormulyar(void);

//Проверка попадания точки в один из формуляров
  static int AnyHavePixel(int Xm,int Ym);

//Рисование непрозрачного формуляра
  static airplane* GetOpaque( int Xm, int Ym);

//Найти  и повернуть формуляр
  static int NaitiIPovernut(int Xp, int Yp,
                      int Kuda);


//Рисовать текущий отсчет
  int PaintOne(HDC DC);


//Освободить данный формуляр
  int FreeFormulyar(void);



};





#endif
