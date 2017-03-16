//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
USEFORM("circle_obz\MainRBZ.cpp", Form1);
USEFORM("circle_obz\About.cpp", AboutBox);
USEFORM("circle_obz\options.cpp", Form2);
USEFORM("circle_obz\FrameTime.cpp", FTime); /* TFrame: File Type */
USEFORM("circle_obz\Navigation.cpp", Form4);
USEFORM("circle_obz\Debug.cpp", FDebug);
USEFORM("circle_obz\MessageFrame.cpp", FMessage); /* TFrame: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

extern HANDLE GlobalCntrlEvnt;


//ѕроверка существовани€ запущенного приложени€
         GlobalCntrlEvnt=OpenEvent( EVENT_ALL_ACCESS,
                FALSE,
                "AORLMatveyenko1979");
        if(GlobalCntrlEvnt)
        {
           CloseHandle(GlobalCntrlEvnt);
           ExitProcess(1);
        }
        GlobalCntrlEvnt=CreateEvent(NULL,false,false,"AORLMatveyenko1979");

        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TAboutBox), &AboutBox);
                 Application->CreateForm(__classid(TForm2), &Form2);
                 Application->CreateForm(__classid(TForm4), &Form4);
                 Application->CreateForm(__classid(TFDebug), &FDebug);
                 Application->Run();
        }
        catch (Exception &exception)
        {
            ExitProcess(1);
        }
        catch (...)
        {
            ExitProcess(1);
        }
        return 0;
}
//---------------------------------------------------------------------------
