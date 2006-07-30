//===========================================================================
// Copyright © 2003 Ivan Johansen
// MyEdit.h
//===========================================================================
#ifndef MyEditH
#define MyEditH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <TntStdCtrls.hpp>
//---------------------------------------------------------------------------
class TMyEdit : public TTntEdit
{
private:
  DYNAMIC void __fastcall KeyPress(char &Key);

  void __fastcall WMDeadChar(TMessage &Message);

  BEGIN_MESSAGE_MAP
    VCL_MESSAGE_HANDLER(WM_DEADCHAR, TMessage, WMDeadChar)
  END_MESSAGE_MAP(TTntEdit)
protected:
  WideString ConvertToGreek(WideString Str);
public:
  __fastcall TMyEdit(TComponent* Owner);
__published:
};
//---------------------------------------------------------------------------
#endif
