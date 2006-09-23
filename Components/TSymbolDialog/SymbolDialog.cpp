//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "SymbolDialog.h"
#include "SymbolForm.h"
#include <consts.hpp>
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TSymbolDialog *)
{
  new TSymbolDialog(NULL);
}
//---------------------------------------------------------------------------
namespace Symboldialog
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TSymbolDialog)};
     RegisterComponents("IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
__fastcall TSymbolDialog::TSymbolDialog(TComponent* Owner)
  : TComponent(Owner), FFontName("Symbol"), FCharacterSet(csAnsiSet),
    FOnShow(NULL), FOnClose(NULL), FOnInsert(NULL), SymbolFrm(NULL), FSymbol(0x20)
{
}
//---------------------------------------------------------------------------
__fastcall TSymbolDialog::~TSymbolDialog()
{
  Close();
}
//---------------------------------------------------------------------------
void TSymbolDialog::Execute()
{
  if(SymbolFrm == NULL)
  {
    SymbolFrm = new TSymbolFrm(NULL, CharacterSet == csUnicode && Win32Platform == VER_PLATFORM_WIN32_NT);
    DoShow();
  }
  SymbolFrm->ShowModal();
}
//---------------------------------------------------------------------------
void TSymbolDialog::Show()
{
  if(SymbolFrm == NULL)
  {
    SymbolFrm = new TSymbolFrm(NULL, CharacterSet == csUnicode && Win32Platform == VER_PLATFORM_WIN32_NT);
    DoShow();
    SymbolFrm->Show();
  }
}
//---------------------------------------------------------------------------
void TSymbolDialog::Close()
{
  if(SymbolFrm)
    SymbolFrm->Close();
}
//---------------------------------------------------------------------------
void __fastcall TSymbolDialog::Button2Click(TObject *Sender)
{
  if(OnInsert)                                
    OnInsert(this, SymbolFrm->GetSelected(), SymbolFrm->ComboBox1->Text);
}
//---------------------------------------------------------------------------
void __fastcall TSymbolDialog::FormClose(TObject *Sender, TCloseAction &Action)
{
  FFontName = SymbolFrm->ComboBox1->Text;
  FSymbol = SymbolFrm->GetSelected();
  if(OnClose)
    OnClose(this);
  SymbolFrm = NULL;
  Action = caFree;
}
//---------------------------------------------------------------------------
void TSymbolDialog::DoShow()
{
  SymbolFrm->Button2->OnClick = Button2Click;
  SymbolFrm->OnClose = FormClose;
  SymbolFrm->ComboBox1->ItemIndex = SymbolFrm->ComboBox1->Items->IndexOf(FontName);
  SymbolFrm->SetSelected(FSymbol);
  if(OnShow)
    OnShow(this);
}
//---------------------------------------------------------------------------

