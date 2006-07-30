//===========================================================================
// Copyright © 2005 Ivan Johansen
// SaveDialogEx.h
//===========================================================================
#ifndef SaveDialogExH
#define SaveDialogExH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class PACKAGE TSaveDialogEx : public TSaveDialog
{
private:
  AnsiString FHelpCaption;
  TNotifyEvent FOnHelp;
  void __fastcall WndProc(TMessage &Message);
  DYNAMIC void __fastcall DoShow();

protected:
public:
  __fastcall TSaveDialogEx(TComponent* Owner);

__published:
  __property AnsiString HelpCaption = {read=FHelpCaption, write=FHelpCaption};
  __property TNotifyEvent OnHelp = {read=FOnHelp, write=FOnHelp, default=NULL};
};
//---------------------------------------------------------------------------
#endif
