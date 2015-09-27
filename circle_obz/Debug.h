//---------------------------------------------------------------------------

#ifndef DebugH
#define DebugH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFDebug : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TTimer *Timer1;
        void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFDebug(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFDebug *FDebug;
extern long DebugF1; //Период вывода

//---------------------------------------------------------------------------
#endif
