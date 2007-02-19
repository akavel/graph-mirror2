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
#include "Unit7.h"
//---------------------------------------------------------------------------
#pragma link "LineSelect"
#pragma link "ExtColorBox"
#pragma link "MyEdit"
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm7::TForm7(TComponent* Owner, TData &AData)
    : TTntForm(Owner), Data(AData)
{
  ScaleForm(this);
  TranslateProperties(this);
  TranslateStrings(ExtColorBox1->Items);
  SetAccelerators(this);
  Edit1->Left = Label4->Left + Label4->Width + 5;
  Edit2->Left = Label5->Left + Label5->Width + 5;
  ResizeControl(Edit3, Label10);

  int Left = Label7->Left + Label7->Width + 5;
  LineSelect1->Left = Left;
  ExtColorBox1->Left = Left;

  LineSelect1->ItemIndex = Data.Property.DefaultDif.Style;
  ExtColorBox1->Selected = Data.Property.DefaultDif.Color;
  UpDown1->Position = Data.Property.DefaultDif.Size;
}
//---------------------------------------------------------------------------
void __fastcall TForm7::Button1Click(TObject *Sender)
{
  Data.AbortUpdate();

  try
  {
    boost::shared_ptr<TBaseFuncType> Dif(Func->MakeDifFunc());

    Dif->From.Text = ToString(Edit1->Text);
    Dif->To.Text = ToString(Edit2->Text);

    if(Edit1->Text.IsEmpty())
      Dif->From.Value = -INF;
    else
      Dif->From.Value = MakeFloat(Edit1);

    if(Edit2->Text.IsEmpty())
      Dif->To.Value = INF;
    else
      Dif->To.Value = MakeFloat(Edit2);

    Dif->Size = ToIntDef(Edit4->Text, 1);
    Dif->Style = LineSelect1->LineStyle;
    Dif->Color = ExtColorBox1->Selected;
    Dif->SetLegendText(ToWString(Edit3->Text));

    Data.Add(Dif);
    UndoList.Push(TUndoAdd(Dif));
    Data.Property.DefaultDif.Set(LineSelect1->ItemIndex, ExtColorBox1->Selected, ToIntDef(Edit4->Text, 1));
    ModalResult = mrOk;
  }
  catch(Func32::EFuncError &E)
  {
    ShowErrorMsg(E);
  }
}
//---------------------------------------------------------------------------
int TForm7::InsertDif(const boost::shared_ptr<TBaseFuncType> &F)
{
  Func = F;
  Edit1->Text = Func->From.Text.c_str();
  Edit2->Text = Func->To.Text.c_str();
  return ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm7::Button3Click(TObject *Sender)
{
  Application->HelpContext(HelpContext);
}
//---------------------------------------------------------------------------



