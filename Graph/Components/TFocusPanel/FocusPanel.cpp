//===========================================================================
// Copyright © 2003 Ivan Johansen
// FocusPanel.cpp
//===========================================================================
#include <vcl.h>
#pragma hdrstop
#include "FocusPanel.h"
#include "ICompCommon.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
static TRegisterClass Dummy(__classid(TFocusPanel));
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TFocusPanel *)
{
  new TFocusPanel(NULL);
}
//---------------------------------------------------------------------------
namespace Focuspanel
{
  void __fastcall PACKAGE Register()
  {
           TComponentClass classes[1] = {__classid(TFocusPanel)};
           RegisterComponents("IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
__fastcall TFocusPanel::TFocusPanel(TComponent* Owner)
        : TPanel(Owner), FOnMouseEnter(NULL), FOnMouseLeave(NULL), FCenterChildren(false)
{
}
//---------------------------------------------------------------------------
void __fastcall TFocusPanel::WMGetDlgCode(TMessage &Message)
{
  Message.Result = DLGC_WANTARROWS;
}
//---------------------------------------------------------------------------
void __fastcall TFocusPanel::WndProc(TMessage &Message)
{
  TPanel::WndProc(Message);
  if(TabStop && Message.Msg == WM_LBUTTONDOWN && !ComponentState.Contains(csDesigning))
    SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFocusPanel::ControlsAligned()
{
  TPanel::ControlsAligned();
  if(FCenterChildren)
    for(int I = 0; I < ControlCount; I++)
    {
      TControl *AControl = Controls[I];
      AControl->Top = (ClientHeight - AControl->Height) / 2;
    }
}
//---------------------------------------------------------------------------

