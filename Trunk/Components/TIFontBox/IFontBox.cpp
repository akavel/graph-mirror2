//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "IFontBox.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TIFontBox *)
{
  new TIFontBox(NULL);
}
//---------------------------------------------------------------------------
namespace Ifontbox
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TIFontBox)};
     RegisterComponents("IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
__fastcall TIFontBox::TIFontBox(TComponent* Owner)
  : inherited(Owner), FSamplePos(100), OldItemIndex(-1)
{
  Width = 200;
  //Add list of fonts; Take care of duplicates, that for some strange reason has happened
  if(!ComponentState.Contains(csDesigning))
    for(int I = 0; I < Screen->Fonts->Count; I++)
      if(Items->IndexOf(Screen->Fonts->Strings[I]) == -1)
        Items->Add(Screen->Fonts->Strings[I]);
}
//---------------------------------------------------------------------------
void __fastcall TIFontBox::CreateParams(TCreateParams &Params)
{
  inherited::CreateParams(Params);
  Params.Style = Params.Style | CBS_OWNERDRAWFIXED;
}
//---------------------------------------------------------------------------
void __fastcall TIFontBox::DrawItem(int Index, const Types::TRect &Rect, TOwnerDrawState State)
{
  int TextHeight = Canvas->TextHeight('M');
  Canvas->TextRect(Rect, Rect.Left + 3, Rect.Top + (Rect.Height() - TextHeight) / 2, Items->Strings[Index]);

  String Str = FSample.IsEmpty() ? Items->Strings[Index] : FSample;
  Canvas->Font->Name = Items->Strings[Index];
  Canvas->Font->Size = 12;
  TextHeight = Canvas->TextHeight(Str);
  TRect Rect2(Rect);
  Rect2.Left += FSamplePos;
  Canvas->TextRect(Rect2, Rect2.Left, Rect2.Top + (Rect2.Height() - TextHeight) / 2, Str);
}
//---------------------------------------------------------------------------
void __fastcall TIFontBox::MeasureItem(int Index, int &H)
{
  if(Index != -1)
    H = 20;
}
//---------------------------------------------------------------------------
String __fastcall TIFontBox::GetFontName()
{
  return ItemIndex != -1 ? Items->Strings[ItemIndex] : String();
}
//---------------------------------------------------------------------------
void __fastcall TIFontBox::SetFontName(const String &Str)
{
  ItemIndex = Items->IndexOf(Str);
}
//---------------------------------------------------------------------------
void __fastcall TIFontBox::DoExit()
{
  ItemIndex = OldItemIndex;
  Text = Items->Strings[ItemIndex];
  inherited::DoExit();
}
//---------------------------------------------------------------------------
void __fastcall TIFontBox::Select()
{
  OldItemIndex = ItemIndex;
  inherited::Select();
}
//---------------------------------------------------------------------------
void __fastcall TIFontBox::ChangeScale(int M, int D)
{
  inherited::ChangeScale(M, D); // Call inherited
  ItemHeight = (ItemHeight * M) / D;
  FSamplePos = (FSamplePos * M) / D;
}
//---------------------------------------------------------------------------

