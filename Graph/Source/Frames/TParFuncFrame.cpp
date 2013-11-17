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
#include "TParFuncFrame.h"
//---------------------------------------------------------------------------
#pragma link "MyEdit"
#pragma link "UpDownEx"
#pragma link "TEvalFrame"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TParFuncFrame::TParFuncFrame(TComponent* Owner)
  : TEvalFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TParFuncFrame::Eval(const TGraphElem *Elem)
{
  if(const TParFunc *Func = dynamic_cast<const TParFunc*>(Elem))
  {
    Clear();

    ErrorPrefix = "t: ";
    bool UseReal = Property.ComplexFormat == cfReal;
    std::wstring xStr = ToWString(Edit1->Text);
    Func32::TComplex t = UseReal ? Func32::TComplex(Form1->Data.Calc(xStr)) : Form1->Data.CalcComplex(xStr);
    Func32::TComplex x, y;

    try
    {
      x = UseReal ? Func32::TComplex(Func->GetFunc().CalcX(real(t))) : Func->GetFunc().CalcX(t);
      Edit2->Text = ComplexToString(x);
    }
    catch(Func32::ECalcError&)
    {
    }

    try
    {
      y = UseReal ? Func32::TComplex(Func->GetFunc().CalcY(real(t))) : Func->GetFunc().CalcY(t);
      Edit3->Text = ComplexToString(y);
    }
    catch(Func32::ECalcError&)
    {
    }

    if(!Edit2->Text.IsEmpty() && !Edit3->Text.IsEmpty() &&
      std::abs(x.imag()) < MIN_ZERO && std::abs(y.imag()) < MIN_ZERO)
      Form1->SetCrossPos(real(x), real(y));

    Func32::TParamFunc Dif = Func->GetFunc().MakeDif();
    Func32::TComplex xDif = 0, yDif = 0;

    try
    {
      xDif = UseReal ? Func32::TComplex(Dif.CalcX(real(t))) : Dif.CalcX(t);
      if(!Edit2->Text.IsEmpty())
        Edit4->Text = ComplexToString(xDif);
    }
    catch(Func32::ECalcError&)
    {
    }

    try
    {
      yDif = UseReal ? Func32::TComplex(Dif.CalcY(real(t))) : Dif.CalcY(t);
      if(!Edit3->Text.IsEmpty())
        Edit5->Text = ComplexToString(yDif);
    }
    catch(Func32::ECalcError&)
    {
    }

    if(!Edit4->Text.IsEmpty() && !Edit5->Text.IsEmpty())
      if(xDif != Func32::TComplex(0,0))
        Edit6->Text = ComplexToString(yDif/xDif);
  }
}
//---------------------------------------------------------------------------
void TParFuncFrame::Clear()
{
  Edit2->Text = "";
  Edit3->Text = "";
  Edit4->Text = "";
  Edit5->Text = "";
  Edit6->Text = "";
  Form1->CancelStatusError();
}
//---------------------------------------------------------------------------
void TParFuncFrame::SetPoint(const TGraphElem *Elem, int X, int Y)
{
  if(const TParFunc *Func = dynamic_cast<const TParFunc*>(Elem))
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
      Edit1->Text = RoundToStr(t, ComboBox1->ItemIndex == 0 ? Property.RoundTo : std::max(8, Property.RoundTo));
  }
}
//---------------------------------------------------------------------------
void __fastcall TParFuncFrame::ComboBox1Change(TObject *Sender)
{
  Edit1->ReadOnly = ComboBox1->ItemIndex;
  UpDownEx1->Enabled = ComboBox1->ItemIndex == 0;
  Edit1->Color = ComboBox1->ItemIndex ? clBtnFace : clWindow;
}
//---------------------------------------------------------------------------

