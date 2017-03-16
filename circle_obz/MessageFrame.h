//---------------------------------------------------------------------------
/*Ôð*/

#ifndef MessageFrameH
#define MessageFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFMessage : public TFrame
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TButton *BStartComdisable;
private:	// User declarations
public:		// User declarations
        __fastcall TFMessage(TComponent* Owner);
        void __fastcall ShowText(AnsiString MessText);
        void __fastcall ShowText(
             AnsiString MessText1,
             AnsiString MessText2);

};
//---------------------------------------------------------------------------
extern PACKAGE TFMessage *FMessage;
//---------------------------------------------------------------------------
#endif
