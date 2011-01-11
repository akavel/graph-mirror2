//===========================================================================
// Copyright © 2003 Ivan Johansen
// MyEdit.cpp
//===========================================================================
#include <vcl.h>
#pragma hdrstop
#include "MyEdit.h"
#include "ICompCommon.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
static TRegisterClass Dummy(__classid(TMyEdit));
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TMyEdit *)
{
        new TMyEdit(NULL);
}
//---------------------------------------------------------------------------
namespace Myedit
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TMyEdit)};
                 RegisterComponents("IComp", classes, 0);
        }
}
//---------------------------------------------------------------------------
__fastcall TMyEdit::TMyEdit(TComponent* Owner)
		: TEdit(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMyEdit::KeyPress(Char &Key)
{
  if(Key == 0x07) //Ctrl+G
  {
    if(SelLength == 0)
    {
      int OldSelStart = SelStart;
      if(SelStart > 0)
        Text = Text.SubString(1, SelStart-1) + ConvertToGreek(Text.SubString(SelStart, 1)) + Text.SubString(SelStart+1, Text.Length()-SelStart-1);
      SelStart = OldSelStart;
    }
    else
      SelText = ConvertToGreek(SelText);
    Key = 0;
  }
  else if(Key == L'\xB2') //Superscript 2
  {
    SendMessage(Handle, WM_CHAR, '^', 0);
    Key = '2';
  }
  else
    TEdit::KeyPress(Key);
}
//---------------------------------------------------------------------------
void __fastcall TMyEdit::WMDeadChar(TMessage &Message)
{
  //I am not sure why this works, but it does
  if(Message.WParam == '^')
    PostMessage(Handle, WM_KEYUP, VK_SPACE, 0);
}
//---------------------------------------------------------------------------
String TMyEdit::ConvertToGreek(String Str)
{
  for(int I = 1; I <= Str.Length(); I++)
    if(Str[I] >= L'a' && Str[I] <= L'z')
      Str[I] += L'\x3B1' - L'a'; //'\x3B1' is the Greek capital lette alpha
    else if(Str[I] >= L'A' && Str[I] <= L'Z')
      Str[I] += L'\x391' - L'A'; //'\x391' is the Greek small letter alpha
  return Str;
}
//---------------------------------------------------------------------------

