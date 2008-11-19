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
#include "Unit12.h"
#include <float.h>
//---------------------------------------------------------------------------
#pragma link "LineSelect"
#pragma link "ExtColorBox"
#pragma link "MyEdit"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm12::TForm12(TComponent* Owner, TData &AData)
    : TForm(Owner), Index(-1), Data(AData)
{
  TranslateProperties(this);
  TranslateStrings(ExtColorBox1->Items);
  SetAccelerators(this);
  Edit4->Left = Label4->Left + Label4->Width + 5;
  ResizeControl(Edit2, Label10);

  int Left = Label7->Left + TMaxWidth(Label7)(Label8)(Label9) + 5;
  LineSelect1->Left = Left;
  ExtColorBox1->Left = Left - 2;
  Edit3->Left = Left;

  MoveControl(ComboBox1, Label11);
  MoveLabel(ComboBox2, Label12);

  LineSelect1->ItemIndex = Property.DefaultTangent.Style;
  ExtColorBox1->Selected = Property.DefaultTangent.Color;
  UpDown1->Position = Property.DefaultTangent.Size;

  ScaleForm(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm12::Button1Click(TObject *Sender)
{
  boost::shared_ptr<TTan> Tan(new TTan);
  int Size = ToIntDef(Edit3->Text, 1);
  //check if size is less than 1
  if(Size <= 0)
  {
    MessageBox(LoadRes(513), LoadRes(RES_ERROR_IN_VALUE));
    Edit3->SetFocus();
    return;
  }

  Tan->t.Text = ToWString(Edit1->Text);
  Tan->t.Value = MakeFloat(Edit1);

  Data.AbortUpdate();

  Tan->SetParentFunc(Func);
  Tan->Size = Size;
  Tan->Style = LineSelect1->LineStyle;
  Tan->Color = ExtColorBox1->Selected;
  Tan->TangentType = RadioGroup1->ItemIndex == 0 ? ttTangent : ttNormal;
  Tan->SetLegendText(ToWString(Edit2->Text));

  Tan->From.Value = -INF;
  Tan->To.Value = INF;
  Tan->From.Text = ToWString(Edit4->Text);
  Tan->To.Text = ToWString(Edit5->Text);

  Tan->StartPointStyle = ComboBox1->ItemIndex;
  Tan->EndPointStyle = ComboBox2->ItemIndex;

  if(!Edit4->Text.IsEmpty())
    Tan->From.Value = MakeFloat(Edit4);
  if(!Edit5->Text.IsEmpty())
    Tan->To.Value = MakeFloat(Edit5);

  if(Tan->From.Value >= Tan->To.Value)
  {
    MessageBox(LoadRes(511), LoadRes(512));
    return;
  }

  if(!Tan->CalcTan())
  {
    MessageBox(LoadRes(518, ToWString(Tan->ParentFunc()->GetVariable()), Tan->t.Text), LoadString(519));
    return;
  }

  if(Index == -1)
  {
    Func->AddChild(Tan);
    UndoList.Push(TUndoAdd(Data, Func->ChildList.back()));
  }
  else
  {
    Tan->SetVisible(Func->ChildList[Index]->GetVisible());
    Tan->SetShowInLegend(Func->ChildList[Index]->GetShowInLegend());
    UndoList.Push(TUndoChange(Data, Func->ChildList[Index], Index));
    Func->ReplaceChild(Index, Tan);
  }

  Property.DefaultTangent.Set(LineSelect1->ItemIndex, ExtColorBox1->Selected, Edit3->Text.ToInt());

  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TForm12::Edit3KeyPress(TObject *Sender, char &Key)
{
  if(!isdigit(Key) && Key!='\b' || (Key=='0' && Edit3->Text.IsEmpty()))
    Key=0;//Allown only digits and backspace. 0 may not be placed first.
}
//---------------------------------------------------------------------------
int TForm12::EditTan(const boost::shared_ptr<TBaseFuncType> &F, int AIndex)
{
  Index = AIndex;
  Caption = LoadRes(525);
  Func = F;
  boost::shared_ptr<TTan> Tan = boost::static_pointer_cast<TTan>(Func->ChildList[Index]);
  Edit1->Text = Tan->t.Text.c_str();
  UpDown1->Position = Tan->Size;
  LineSelect1->LineStyle = Tan->Style;
  ExtColorBox1->Selected = Tan->Color;
  Label1->Caption = (Tan->ParentFunc()->GetVariable() + "=").c_str();
  Edit2->Text = ToUString(Tan->GetLegendText());
  RadioGroup1->ItemIndex = Tan->TangentType;
  Edit4->Text = Tan->From.Text.c_str();
  Edit5->Text = Tan->To.Text.c_str();
  ComboBox1->ItemIndex = Tan->StartPointStyle;
  ComboBox2->ItemIndex = Tan->EndPointStyle;

  return ShowModal();
}
//---------------------------------------------------------------------------
int TForm12::InsertTan(const boost::shared_ptr<TBaseFuncType> &F)
{
  Func = F;
  Label1->Caption = F->GetVariable().c_str() + String('=');
  return ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm12::Button3Click(TObject *Sender)
{
  Application->HelpContext(HelpContext);
}
//---------------------------------------------------------------------------
void __fastcall TForm12::ComboBoxDrawItem(TWinControl *Control, int Index,
      TRect &Rect, TOwnerDrawState State)
{
  if(TCustomComboBox *ComboBox = dynamic_cast<TCustomComboBox*>(Control))
    DrawComboBoxEndPoint(ComboBox->Canvas, Index, Rect);
}
//---------------------------------------------------------------------------

