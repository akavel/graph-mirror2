//---------------------------------------------------------------------------
#ifndef Unit22H
#define Unit22H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "IRichEdit.h"
//---------------------------------------------------------------------------
class TForm22 : public TForm
{
__published:	// IDE-managed Components
  TIRichEdit *IRichEdit1;
  void __fastcall FormHide(TObject *Sender);
  void __fastcall IRichEdit1ProtectChange(TObject *Sender, int StartPos,
          int EndPos, bool &AllowChange);
  void __fastcall IRichEdit1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
  int LastIndex;
  bool FAllowChange;
  std::vector<AnsiString> TextCache;
  int CacheIndex;
  AnsiString Command;

public:		// User declarations
  __fastcall TForm22(TComponent* Owner);
  void WriteText(const AnsiString &Str, TColor Color = clBlack);
};
//---------------------------------------------------------------------------
extern TForm22 *Form22;
//---------------------------------------------------------------------------
#endif
