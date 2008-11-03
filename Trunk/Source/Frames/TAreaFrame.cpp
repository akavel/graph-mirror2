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
#include <algorithm>
#include "float.h"
#include "Unit1.h"
#include "TAreaFrame.h"
//---------------------------------------------------------------------------
#pragma link "MyEdit"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TAreaFrame::TAreaFrame(TComponent* Owner)
        : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TAreaFrame::EvalArea(const TGraphElem *GraphElem)
{
  Edit3->Text = "";
  Form1->IPolygon1->Clear();

  if(!GraphElem->GetVisible())
    return;

  if(const TBaseFuncType *Func = dynamic_cast<const TBaseFuncType*>(GraphElem))
  {
    long double From = Form1->Data.Calc(::ToString(Edit1->Text));
    long double To = Form1->Data.Calc(::ToString(Edit2->Text));

    Edit3->Text = RoundToStr(Func->CalcArea(From, To), Form1->Data);

    if(From > To)
      std::swap(From, To);

    Func32::TCoord<long double> Min = Func->Eval(From);
    Func32::TCoord<long double> Max = Func->Eval(To);

    unsigned N1 = std::lower_bound(Func->sList.begin(), Func->sList.end(), From, TCompCoordSet()) - Func->sList.begin();
    unsigned N2 = std::lower_bound(Func->sList.begin() + N1, Func->sList.end(), To, TCompCoordSet()) - Func->sList.begin();
    if(N1 != N2)
    {
      Form1->IPolygon1->AddPoint(Form1->Draw.xyPoint(Min.x, Min.y));
      Form1->IPolygon1->AddPoints(&Func->Points[N1], N2 - N1);
      Form1->IPolygon1->AddPoint(Form1->Draw.xyPoint(Max.x, Max.y));
    }

    if(const TPolFunc *PolFunc = dynamic_cast<const TPolFunc*>(GraphElem))
    {
      if(N1 != N2)
        Form1->IPolygon1->AddPoint(TPoint(Form1->Draw.xyPoint(Form1->Data.Axes.yAxis.AxisCross, Form1->Data.Axes.xAxis.AxisCross)));
    }
    else
    {
      Form1->IPolygon1->AddPoint(Form1->Draw.xyPoint(Max.x, Form1->Data.Axes.xAxis.AxisCross));
      Form1->IPolygon1->AddPoint(Form1->Draw.xyPoint(Min.x, Form1->Data.Axes.xAxis.AxisCross));
    }

    Form1->IPolygon1->Pen->Width = Func->Size;
  }

  Form1->IPolygon1->PolygonType = ptPolygon;
  Form1->IPolygon1->Visible = true;
}
//---------------------------------------------------------------------------
void TAreaFrame::EvalArc(const TGraphElem *GraphElem)
{

  Edit3->Text = "";
  Form1->IPolygon1->Clear();

  if(!GraphElem->GetVisible())
    return;

  double Min = Form1->Data.Calc(::ToString(Edit1->Text));
  double Max = Form1->Data.Calc(::ToString(Edit2->Text));

  if(Max < Min)
    std::swap(Min, Max);

  if(const TBaseFuncType *Func = dynamic_cast<const TBaseFuncType*>(GraphElem))
  {
    Edit3->Text = RoundToStr(Func->GetFunc().CalcArc(Min, Max, 1000), Form1->Data);

    unsigned N1 = std::lower_bound(Func->sList.begin(), Func->sList.end(), Min, TCompCoordSet()) - Func->sList.begin();
    unsigned N2 = std::upper_bound(Func->sList.begin() + N1, Func->sList.end(), Max, TCompCoordSet()) - Func->sList.begin();
    if(N1 != N2)
      Form1->IPolygon1->AddPoints(&Func->Points[N1], N2 - N1);
    Form1->IPolygon1->Pen->Width = Func->Size;
  }
  else if(const TTan *Tan = dynamic_cast<const TTan*>(GraphElem))
  {
    try
    {
      double dx = Max - Min;
      double yMin = Tan->GetFunc().CalcY(Min);
      double yMax = Tan->GetFunc().CalcY(Max);
      double dy = yMax - yMin;

      //Length = sqrt(dx^2+dy^2) = sqrt(dx^2+^(a*dx)^2)
      Edit3->Text = RoundToStr(std::sqrt(dx*dx + dy*dy), Form1->Data);

      Form1->IPolygon1->AddPoint(TPoint(Form1->Draw.xPoint(Min), Form1->Draw.yPoint(yMin)));
      Form1->IPolygon1->AddPoint(TPoint(Form1->Draw.xPoint(Max), Form1->Draw.yPoint(yMax)));
      Form1->IPolygon1->Pen->Width = Tan->Size;
    }
    catch(...)
    {
    }
  }

  Form1->IPolygon1->PolygonType = ptPolyline;
  Form1->IPolygon1->Visible = true;
}
//---------------------------------------------------------------------------






