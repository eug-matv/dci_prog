//---------------------------------------------------------------------------


#ifndef FrameTimeH
#define FrameTimeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFTime : public TFrame
{
__published:	// IDE-managed Components
        TLabel *Label1;
private:	// User declarations
public:		// User declarations
        __fastcall TFTime(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFTime *FTime;
//---------------------------------------------------------------------------
#endif
