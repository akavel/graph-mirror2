/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "TStdFuncFrame.h"
#include <float.h>
//---------------------------------------------------------------------------
#pragma link "MyEdit"
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TStdFuncFrame::TStdFuncFrame(TComponent* Owner)
  : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TStdFuncFrame::Clear()
{
  Edit2->Text = "";
  Edit3->Font->Color = clWindowText;
  Edit3->Text = "";
  Edit4->Font->Color = clWindowText;
  Edit4->Text = "";
  Form1->CancelStatusError();
}
//---------------------------------------------------------------------------
void TStdFuncFrame::EvalFunc(TStdFunc *Func)
{
  Clear();
  Func32::TComplex x = Form1->Data.CalcComplex(ToString(Edit1->Text));
  Func32::TComplex y = Func->GetFunc().CalcY(x);

  if(!x.imag() && std::abs(y.imag()) < MIN_ZERO)
    Form1->SetCrossPos(x.real(), y.real());

  Edit2->Text = ComplexToWideString(y);
  Func32::TFunc Dif1 = Func->GetFunc().MakeDif();
  Func32::TComplex yDif1 = Dif1.CalcY(x);

  Edit3->Text = ComplexToWideString(yDif1);
  Func32::TFunc Dif2 = Dif1.MakeDif();

  Func32::TComplex yDif2 = Dif2.CalcY(x);
  Edit4->Text = ComplexToWideString(yDif2);
}
//---------------------------------------------------------------------------
void TStdFuncFrame::SetPoint(TStdFunc *Func, int X, int Y)
{
  Edit1->Text = "";
  TTraceType TraceType;
  switch(ComboBox1->ItemIndex)
  {
    case 0: TraceType = ttTrace;        break;
    case 1: TraceType = ttIntersection; break;
    case 2: TraceType = ttXAxis;        break;
    case 3: TraceType = ttExtremeY;     break;
  }

  double t = TraceFunction(Func, TraceType, X, Y, Form1->Data, Form1->Draw);
  if(_isnan(t))
    Edit1->Text = "";
  else
    Edit1->Text = RoundToStr(t, ComboBox1->ItemIndex == 0 ? Form1->Data.Property.RoundTo : 8);
}
//---------------------------------------------------------------------------
void __fastcall TStdFuncFrame::ComboBox1Change(TObject *Sender)
{
  Edit1->ReadOnly = ComboBox1->ItemIndex;
  Edit1->Color = ComboBox1->ItemIndex ? clBtnFace : clWindow;
}
//---------------------------------------------------------------------------

