//===========================================================================
// Copyright © 2005 Ivan Johansen
// ExtColorBox.cpp
//===========================================================================
#include "Config.h"
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "ExtColorBox.h"
#include "IColorSelect.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TExtColorBox *)
{
  new TExtColorBox(NULL);
}
//---------------------------------------------------------------------------
namespace Extcolorbox
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TExtColorBox)};
     RegisterComponents("IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
__fastcall TExtColorBox::TExtColorBox(TComponent* Owner)
  : TColorBox(Owner), FOnPickColor(NULL), FColorDialogType(cdtColorSelect), FDroppedWidth(0),
    FAutoDroppedWidth(true), DroppedWidthFound(false)
{
}
//---------------------------------------------------------------------------
void __fastcall TExtColorBox::Loaded(void)
{
  TColorBox::Loaded();
  if(Style.Contains(cbCustomColor))
    Items->Objects[0] = reinterpret_cast<TObject*>(clDefault);
}
//---------------------------------------------------------------------------
bool __fastcall TExtColorBox::PickCustomColor(void)
{
  if(OnPickColor)
  {
    TColor Color = Selected;
    OnPickColor(this, Color);
    Selected = Color;
    Invalidate();
    return true;
  }

  if(ColorDialogType == cdtColorSelect)
  {
    std::auto_ptr<TIColorSelect> ColorSelect(new TIColorSelect(NULL));
    ColorSelect->Color = Selected;
    if(ColorSelect->Execute())
    {
      Selected = ColorSelect->Color;
      Invalidate();
      return true;
    }
    return false;
  }

  return TColorBox::PickCustomColor();
}
//---------------------------------------------------------------------------
void __fastcall TExtColorBox::SetDroppedWidth(int AWidth)
{
  SendMessage(Handle, CB_SETDROPPEDWIDTH, AWidth, 0);
  FDroppedWidth = AWidth;
}
//---------------------------------------------------------------------------
void __fastcall TExtColorBox::SetExtendedUI(bool Value)
{
  SendMessage(Handle, CB_SETEXTENDEDUI, Value, 0);
}
//---------------------------------------------------------------------------
bool __fastcall TExtColorBox::GetExtendedUI()
{
  return SendMessage(Handle, CB_GETEXTENDEDUI, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TExtColorBox::DropDown(void)
{
  if(FAutoDroppedWidth && !DroppedWidthFound)
  {
    int MaxWidth = 0;
    for(int I = 0; I < Items->Count; I++)
    {
      int NewWidth = Canvas->TextWidth(Items->Strings[I]);
      if(NewWidth > MaxWidth)
        MaxWidth = NewWidth;
    }

    DroppedWidthFound = true;
    DroppedWidth = MaxWidth + Height + 10;
  }
  TColorBox::DropDown();
}
//---------------------------------------------------------------------------

