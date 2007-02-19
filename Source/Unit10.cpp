/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "Unit10.h"
//---------------------------------------------------------------------------
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm10::TForm10(TComponent* Owner, TProperty &AProperty)
  : TTntForm(Owner), Property(AProperty)
{
  ScaleForm(this);
  TranslateProperties(this);
  SetAccelerators(this);

  //Take care of range problems
  if(Property.NextTip < RES_FIRST_TIP || Property.NextTip > RES_LAST_TIP)
    Property.NextTip = RES_FIRST_TIP;

  CheckBox1->Checked = Property.ShowTipsAtStartup;
  Label2->Caption = LoadRes(Property.NextTip);

  Width = Width - Button3->Width + Canvas->TextWidth(Button3->Caption) + 20;
}
//---------------------------------------------------------------------------
void __fastcall TForm10::Button2Click(TObject *Sender)
{
  if(++Property.NextTip > RES_LAST_TIP)
    Property.NextTip = RES_FIRST_TIP;
  Label2->Caption = LoadRes(Property.NextTip);
}
//---------------------------------------------------------------------------
void __fastcall TForm10::Button3Click(TObject *Sender)
{
  if(--Property.NextTip < RES_FIRST_TIP)
    Property.NextTip = RES_LAST_TIP;
  Label2->Caption = LoadRes(Property.NextTip);
}
//---------------------------------------------------------------------------
void __fastcall TForm10::FormClose(TObject *Sender, TCloseAction &Action)
{
  Property.ShowTipsAtStartup = CheckBox1->Checked;
  if(++Property.NextTip > RES_LAST_TIP)
    Property.NextTip = RES_FIRST_TIP;
}
//---------------------------------------------------------------------------

