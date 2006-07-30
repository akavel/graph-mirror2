//===========================================================================
// Copyright � 2003 Ivan Johansen
// Cross.cpp
//===========================================================================
#include <vcl.h>
#pragma hdrstop
#include "Cross.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TCross *)
{
  new TCross(NULL);
}
//---------------------------------------------------------------------------
namespace Cross
{
  void __fastcall PACKAGE Register()
  {
    TComponentClass classes[1] = {__classid(TCross)};
    RegisterComponents("IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
__fastcall TCross::TCross(TComponent* Owner)
        : TGraphicControl(Owner), FX(30), FY(30)
{
  Width = 60;
  Height = 60;
  Canvas->Pen->Style = psDash;
  Canvas->Brush->Style = bsClear;
}
//---------------------------------------------------------------------------
void __fastcall TCross::Paint()
{
  Canvas->MoveTo(0, Y);
  Canvas->LineTo(Width, Y);
  Canvas->MoveTo(X, 0);
  Canvas->LineTo(X, Height);
}
//---------------------------------------------------------------------------

