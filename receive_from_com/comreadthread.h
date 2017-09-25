//---------------------------------------------------------------------------
/*Поток приема данных с порта COM1-COM9.
Класс TComReadThread.
Среда разработки : C++Builder. ОС: Windows 2k и выше.
Разработан: Ларин К. в 2007
Модифицирован: Матвеенко Е.А. 09.2007
Последняя модификация: 1.10.2007.
Назначение организует вторичный поток, который считывает данные с COM порта
в формате ASTERIX, и заполняет глобальный массив paketstruct, который назывется
TPaketData::PaketData.

Основные процедуры:
Создание нового вторичного потока для считывания данных
static TComReadThread* TComReadThread::Connect(..)
Параметры: int N - номер порта от 1 до 9
           int Speed - бит-рейт порта (скорость считывания).
Возвращает указатель на объект TComReadThread и открывает порт.
Данный метод заполняет переменные: iErrorConnect - это код ошибки:
если iErrorConnect=0, значит открытие порта прощло без ошибки.
Иначе имеет  одно из значений "Коды ощибок соединения порта" (см. ниже).
Если iErrorConnect!=0, тогда заполняется стат переменная
iGetLastError, которая равна iGetLastError=GetLastError() в момент обнаружения
ошибки.

int __fastcall Disconnect() - заверщение работы потока.
Закрывает все открытые дискрипторы. Уничтожает вторичный поток.
Возвращает:1 - если всё успешно, иначе 0


void __fastcall Execute() - содержит код вторичного потока. Считывает данные с
порта. В случае ошибки заполняет переменную dwError, значения которой определыны
ниже как "Коды ошибок текущего выполнения"





*/


#ifndef ComReadThreadH
#define ComReadThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------

/*Коды ошибок текущего выполнения*/
#define ERR_PEREPOLENIE_BUFFERA 0x0001
#define ERR_UNKNOWN_CLEARCOMERROR 0x0002
#define ERR_PAKET_NE_RASPOZNAN 0x0004
#define ERR_PAKET_PEREPOLNEN 0x0008

/*Коды ощибок соединения порта*/
//Ощибка выполнения CreateFile
#define ERR_CONNECT_CREATE       1
//Ошибка выполнения GetCommState
#define ERR_CONNECT_GETCOMSTATE  2

//Ошибка выполнения SetCommState
#define ERR_CONNECT_SETCOMSTATE  3

//Ощибка создания вторичного потока
#define ERR_CONNECT_CREATETHREAD 4

//Ошибка создания события
#define ERR_CONNECT_CREATEEVENT  5


#include "structs.h"



/*Чтение данных из Com-порта. Запись данных в строку и передача данных в две переменные
для дальнейшей обработки.

*/

class TComReadThread;
class TComReadThread : public TThread
{
private:


//Параметры COM порта

 friend  TComReadThread;
 HANDLE hCom;       //Дескриптор файла
 DCB dcb;           //для рписания устройств


//Индексы отсчетов в обзоре - сбрасываются когда приходит север
 long N_of_P;   //Число отметок типа P
 long N_of_U;   //Число УВД
 long N_of_R;   //Число RBS


 initialization init; //Внесена 16.03.2017 Код инит
protected:
        void __fastcall Execute();
         __fastcall TComReadThread(const initialization &_init );
         int __fastcall Disconnect1();

public:
       HANDLE hEventNoEmpty; //Указатель того, что данные есть в списке

       DWORD dwError;        //Код ощибки во время работы

       static int iErrorConnect; //Код ошибки при выполнении функции Connect
       static int iGetLastError; //Код последней ошибки

       static TComReadThread* Connect(
                                const initialization &init
                             );  //Соединение с устройством

            //Отсоединение от устройства

       int __fastcall Disconnect()
       {
           Terminate();
           Sleep(1000);     //Выждеи секунду прежде чем завершится потом
           CloseHandle(hEventNoEmpty);
           hEventNoEmpty=NULL;
           return 1;
       };


};
//---------------------------------------------------------------------------
#endif
