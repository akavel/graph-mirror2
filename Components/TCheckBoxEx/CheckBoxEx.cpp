//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "CheckBoxEx.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TCheckBoxEx *)
{
  new TCheckBoxEx(NULL);
}
//---------------------------------------------------------------------------
namespace Checkboxex
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TCheckBoxEx)};
     RegisterComponents(L"IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
__fastcall TCheckBoxEx::TCheckBoxEx(TComponent* Owner)
  : TCheckBox(Owner)
{
  AutoSize = true;
}
//---------------------------------------------------------------------------
bool __fastcall TCheckBoxEx::CanAutoSize(int &NewWidth, int &NewHeight)
{
  HDC DC = GetDC(Handle);
  SelectObject(DC, Font->Handle);
  SIZE Size;
  bool Result = GetTextExtentPoint32(DC, Caption.c_str(), Caption.Length(), &Size);
  ReleaseDC(Handle, DC);
  int CheckWidth = GetSystemMetrics(SM_CXMENUCHECK);
//  int BorderWidth = GetSystemMetrics(SM_CXBORDER);
  NewWidth = Size.cx + CheckWidth + 6;
  return Result;
}
//---------------------------------------------------------------------------
void __fastcall TCheckBoxEx::CMTextChanged(TMessage &Message)
{
  Invalidate();
  AdjustSize();
}
//---------------------------------------------------------------------------
void __fastcall TCheckBoxEx::CMFontChanged(TMessage &Message)
{
  TCheckBox::Dispatch(&Message);
  AdjustSize();
}
//---------------------------------------------------------------------------

