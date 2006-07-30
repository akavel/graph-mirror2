//===========================================================================
// Copyright © 2005 Ivan Johansen
// SaveDialogEx.cpp
//===========================================================================
#include <vcl.h>
#pragma hdrstop
#include "SaveDialogEx.h"
#include "Dlgs.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TSaveDialogEx *)
{
  new TSaveDialogEx(NULL);
}
//---------------------------------------------------------------------------
namespace Savedialogex
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TSaveDialogEx)};
     RegisterComponents("IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
__fastcall TSaveDialogEx::TSaveDialogEx(TComponent* Owner)
  : TSaveDialog(Owner), FOnHelp(NULL)
{
}
//---------------------------------------------------------------------------
void __fastcall TSaveDialogEx::WndProc(TMessage &Message)
{
  if(Message.Msg == WM_NOTIFY && reinterpret_cast<POFNotify>(Message.LParam)->hdr.code == CDN_HELP)
    if(FOnHelp)
      FOnHelp(this);
  TSaveDialog::WndProc(Message);
}
//---------------------------------------------------------------------------
void __fastcall TSaveDialogEx::DoShow()
{
  if(!HelpCaption.IsEmpty())
    SendMessage(GetParent(Handle), CDM_SETCONTROLTEXT, pshHelp, reinterpret_cast<long>(HelpCaption.c_str()));
}
//---------------------------------------------------------------------------
