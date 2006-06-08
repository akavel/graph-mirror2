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
#include <float.h>
#include "TPolFuncFrame.h"
//---------------------------------------------------------------------------
#pragma link "MyEdit"
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TPolFuncFrame::TPolFuncFrame(TComponent* Owner)
  : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TPolFuncFrame::EvalFunc(TPolFunc *Func)
{
  Clear();

  long double t = Form1->Data.Calc(ToString(Edit1->Text));
  long double r = Func->GetFunc().CalcR(t);
  long double x = Func->GetFunc().CalcX(t);
  long double y = Func->GetFunc().CalcY(t);

  Edit2->Text = RoundToStr(r, Form1->Data);
  Edit3->Text = RoundToStr(x, Form1->Data);
  Edit4->Text = RoundToStr(y, Form1->Data);

  Form1->SetCrossPos(x, y);

  Edit5->Text = RoundToStr(Func->GetFunc().MakeDif().CalcR(t), Form1->Data);
  long double dydx = Func->GetFunc().CalcSlope(t);
  if(_finite(dydx))
    Edit6->Text = RoundToStr(dydx, Form1->Data);
}
//---------------------------------------------------------------------------
void TPolFuncFrame::SetPoint(TPolFunc *Func, int X, int Y)
{
  TTraceType TraceType;
  switch(ComboBox1->ItemIndex)
  {
    case 0: TraceType = ttTrace;        break;
    case 1: TraceType = ttIntersection; break;
    case 2: TraceType = ttXAxis;        break;
    case 3: TraceType = ttYAxis;        break;
    case 4: TraceType = ttExtremeX;     break;
    case 5: TraceType = ttExtremeY;     break;
  }

  double t = TraceFunction(Func, TraceType, X, Y, Form1->Data, Form1->Draw);
  if(_isnan(t))
    Edit1->Text = "";
  else
    Edit1->Text = RoundToStr(t, ComboBox1->ItemIndex == 0 ? Form1->Data.Property.RoundTo : 8);
}
//---------------------------------------------------------------------------
void TPolFuncFrame::Clear()
{
  Edit2->Text = "";
  Edit3->Text = "";
  Edit4->Text = "";
  Edit5->Text = "";
  Edit6->Text = "";
  Form1->CancelStatusError();
}
//---------------------------------------------------------------------------
void __fastcall TPolFuncFrame::ComboBox1Change(TObject *Sender)
{
  Edit1->ReadOnly = ComboBox1->ItemIndex;
  Edit1->Color = ComboBox1->ItemIndex ? clBtnFace : clWindow;
}
//---------------------------------------------------------------------------

