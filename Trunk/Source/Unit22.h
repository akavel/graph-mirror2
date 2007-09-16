//---------------------------------------------------------------------------
#ifndef Unit22H
#define Unit22H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm22 : public TForm
{
__published:	// IDE-managed Components
  TRichEdit *RichEdit1;
  void __fastcall FormHide(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm22(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TForm22 *Form22;
//---------------------------------------------------------------------------
#endif
