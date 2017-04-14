//===========================================================================
// Copyright � 2003 Ivan Johansen
// FocusPanel.h
//===========================================================================
#ifndef FocusPanelH
#define FocusPanelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFocusPanel : public TPanel
{
private:
  TNotifyEvent FOnMouseEnter;
  TNotifyEvent FOnMouseLeave;
  bool FCenterChildren;

  void __fastcall WndProc(TMessage &Message);
  void __fastcall WMGetDlgCode(TMessage &Message);
  void __fastcall MouseEnter(TMessage &Message)
  {
    if(FOnMouseEnter)
      FOnMouseEnter(this);
  }
  void __fastcall MouseLeave(TMessage &Message)
  {
    if(FOnMouseLeave)
      FOnMouseLeave(this);
  }
  BEGIN_MESSAGE_MAP
    VCL_MESSAGE_HANDLER(WM_GETDLGCODE, TMessage, WMGetDlgCode)
    VCL_MESSAGE_HANDLER(CM_MOUSEENTER, TMessage, MouseEnter)//Undocumented; Generated by VCL; Found in Controls.hpp
    VCL_MESSAGE_HANDLER(CM_MOUSELEAVE, TMessage, MouseLeave)
  END_MESSAGE_MAP(TPanel)
protected:
  DYNAMIC void __fastcall ControlsAligned(void);
public:
  __fastcall TFocusPanel(TComponent* Owner);
__published:
  __property OnKeyPress;
  __property OnKeyDown;
  __property OnKeyUp;
  __property OnMouseWheel;
  __property OnMouseWheelDown;
  __property OnMouseWheelUp;
  __property TNotifyEvent OnMouseEnter = {read=FOnMouseEnter, write=FOnMouseEnter, default=NULL};
  __property TNotifyEvent OnMouseLeave = {read=FOnMouseLeave, write=FOnMouseLeave, default=NULL};
  __property bool CenterChildren = {read=FCenterChildren, write=FCenterChildren, default=false};
};
//---------------------------------------------------------------------------
#endif