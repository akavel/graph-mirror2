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
#include <float.h>
#include "Unit1.h"
#include "TPolFuncFrame.h"
//---------------------------------------------------------------------------
#pragma link "MyEdit"
#pragma link "UpDownEx"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TPolFuncFrame::TPolFuncFrame(TComponent* Owner)
  : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TPolFuncFrame::Eval(const TGraphElem *Elem)
{
  if(const TPolFunc *Func = dynamic_cast<const TPolFunc*>(Elem))
  {
    Clear();

    long double t = Form1->Data.Calc(ToWString(Edit1->Text));
    long double r = Func->GetFunc().CalcR(t);
    long double x = Func->GetFunc().CalcX(t);
    long double y = Func->GetFunc().CalcY(t);

    Edit2->Text = RoundToStr(r);
    Edit3->Text = RoundToStr(x);
    Edit4->Text = RoundToStr(y);

    Form1->SetCrossPos(x, y);

    Edit5->Text = RoundToStr(Func->GetFunc().MakeDif().CalcR(t));
    long double dydx = Func->GetFunc().CalcSlope(t);
    if(_finite(dydx))
      Edit6->Text = RoundToStr(dydx);
  }
}
//---------------------------------------------------------------------------
void TPolFuncFrame::SetPoint(const TGraphElem *Elem, int X, int Y)
{
  if(const TPolFunc *Func = dynamic_cast<const TPolFunc*>(Elem))
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
      Edit1->Text = RoundToStr(t, ComboBox1->ItemIndex == 0 ? Property.RoundTo : 8);
  }
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

