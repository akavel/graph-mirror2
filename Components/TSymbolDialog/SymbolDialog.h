//---------------------------------------------------------------------------
#ifndef SymbolDialogH
#define SymbolDialogH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TSymbolDialog;
typedef void __fastcall (__closure *TInsertEvent)(TSymbolDialog *Sender, wchar_t Symbol, const AnsiString &FontName);
enum TCharacterSet {csAnsiSet, csUnicodeSet};

class PACKAGE TSymbolDialog : public TComponent
{
private:
  TFontName FFontName;
  TNotifyEvent FOnShow;
  TNotifyEvent FOnClose;
  TInsertEvent FOnInsert;
  TCharacterSet FCharacterSet;
  wchar_t FSymbol;
  class TSymbolFrm *SymbolFrm;

  void __fastcall Button2Click(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void DoShow();
  void DoClose();

protected:
public:
  __fastcall TSymbolDialog(TComponent* Owner);
  __fastcall ~TSymbolDialog();
  void Execute();
  void Show();
  void Close();

  __property wchar_t Symbol = {read=FSymbol, write=FSymbol};
  
__published:
  __property TFontName FontName = {read=FFontName, write=FFontName};
  __property TCharacterSet CharacterSet = {read=FCharacterSet, write=FCharacterSet};
  __property TNotifyEvent OnShow = {read=FOnShow, write=FOnShow, default=NULL};
  __property TNotifyEvent OnClose = {read=FOnClose, write=FOnClose, default=NULL};
  __property TInsertEvent OnInsert = {read=FOnInsert, write=FOnInsert, default=NULL};
};
//---------------------------------------------------------------------------
#endif
