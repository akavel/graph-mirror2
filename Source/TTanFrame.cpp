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
#include "TTanFrame.h"
#include <float.h>
//---------------------------------------------------------------------------
#pragma link "MyEdit"
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TTanFrame::TTanFrame(TComponent* Owner)
  : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TTanFrame::EvalTan(const TTan *Tan)
{
  Edit2->Text = "";

  double x = Form1->Data.Calc(ToString(Edit1->Text));
  double y = Tan->GetFunc().CalcY(x);
  Edit2->Text = RoundToStr(y, Form1->Data);

  Form1->SetCrossPos(x, y);
}
//---------------------------------------------------------------------------
void TTanFrame::EvalSeries(const TPointSeries *Series)
{
  Edit2->Text = "";

  Func32::TDblPoint Coord = Series->FindCoord(Form1->Data.Calc(ToString(Edit1->Text)));
  Edit2->Text = RoundToStr(Coord.y, Form1->Data);

  Form1->SetCrossPos(Coord.x, Coord.y);
}
//---------------------------------------------------------------------------
void TTanFrame::SetPoint(const TTan *Tan, int X)
{
  Edit1->Text = RoundToStr(Range(Tan->From.Value, Form1->Draw.xCoord(X), Tan->To.Value), Form1->Data);
}
//---------------------------------------------------------------------------
void TTanFrame::SetPoint(const TPointSeries *Series, int X)
{
  Edit1->Text = RoundToStr(Form1->Draw.xCoord(X), Form1->Data);
}
//---------------------------------------------------------------------------

