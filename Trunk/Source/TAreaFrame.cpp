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
#include <algorithm>
#include "float.h"
#include "TAreaFrame.h"
//---------------------------------------------------------------------------
#pragma link "MyEdit"
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TAreaFrame::TAreaFrame(TComponent* Owner)
        : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TAreaFrame::EvalArea(TGraphElem *GraphElem)
{
  Edit3->Text = "";
  Form1->IPolygon1->Clear();

  double Min = Form1->Data.Calc(ToString(Edit1->Text));
  double Max = Form1->Data.Calc(ToString(Edit2->Text));

  if(!GraphElem->GetVisible())
    return;
    
  if(TTan *Tan = dynamic_cast<TTan*>(GraphElem))
  {
    try
    {
      double yMin = Tan->GetFunc().CalcY(Min);
      double yMax = Tan->GetFunc().CalcY(Max);
      Edit3->Text = RoundToStr(0.5 * (Max - Min) * (yMax + yMin), Form1->Data);

      Form1->IPolygon1->AddPoint(Form1->Draw.xyPoint(Min, Form1->Data.Axes.xAxis.AxisCross));
      Form1->IPolygon1->AddPoint(Form1->Draw.xyPoint(Min, yMin));
      Form1->IPolygon1->AddPoint(Form1->Draw.xyPoint(Max, yMax));
      Form1->IPolygon1->AddPoint(Form1->Draw.xyPoint(Max, Form1->Data.Axes.xAxis.AxisCross));
      Form1->IPolygon1->Pen->Width = Tan->Size;
    }
    catch(...)
    {
      if(Tan->IsValid())
        Edit3->Text = L"0";
    }
  }
  else if(TBaseFuncType *Func = dynamic_cast<TBaseFuncType*>(GraphElem))
  {
    unsigned N1 = std::lower_bound(Func->sList.begin(), Func->sList.end(), std::min(Min, Max), CompCoordSet1) - Func->sList.begin();
    unsigned N2 = std::lower_bound(Func->sList.begin() + N1, Func->sList.end(), std::max(Min, Max), CompCoordSet1) - Func->sList.begin();
    if(N1 != N2)
      Form1->IPolygon1->AddPoints(&Func->Points[N1], N2 - N1);

    Edit3->Text = RoundToStr(Func->GetFunc().CalcArea(Min, Max, 1000), Form1->Data);

    if(dynamic_cast<TStdFunc*>(GraphElem) || dynamic_cast<TParFunc*>(GraphElem))
    {
      if(N1 != N2 && Func->Points[N1] != Func->Points[N2-1])
      {
        TPoint P1(Func->Points[N1].x, Form1->Draw.yPoint(Form1->Data.Axes.xAxis.AxisCross));
        TPoint P2(Func->Points[N2-1].x, Form1->Draw.yPoint(Form1->Data.Axes.xAxis.AxisCross));

        //Make sure there is no line drawn on the right side if Max > xMax
        if(Max > Form1->Data.Axes.xAxis.Max)
        {
          P2.x += 10;
          Form1->IPolygon1->AddPoint(TPoint(P2.x, Func->Points[N2-1].y));
        }

        Form1->IPolygon1->AddPoint(P2);

        //Make sure there is no line drawn on the right side if Min < xMin
        if(Min < Form1->Data.Axes.xAxis.Min)
        {
          P1.x -= 10;
          Form1->IPolygon1->AddPoint(P1);
          Form1->IPolygon1->AddPoint(TPoint(P1.x, Func->Points[N1].y));
        }
        else
          Form1->IPolygon1->AddPoint(P1);
      }
    }
    else if(TPolFunc *PolFunc = dynamic_cast<TPolFunc*>(GraphElem))
    {
      if(N1 != N2)
        Form1->IPolygon1->AddPoint(TPoint(Form1->Draw.xyPoint(Form1->Data.Axes.yAxis.AxisCross, Form1->Data.Axes.xAxis.AxisCross)));
    }
    else
      Form1->IPolygon1->Clear();

    Form1->IPolygon1->Pen->Width = Func->Size;
  }

  Form1->IPolygon1->PolygonType = ptPolygon;
  Form1->IPolygon1->Visible = true;
}
//---------------------------------------------------------------------------
void TAreaFrame::EvalArc(TGraphElem *GraphElem)
{
  Edit3->Text = "";
  Form1->IPolygon1->Clear();

  if(!GraphElem->GetVisible())
    return;

  double Min = Form1->Data.Calc(ToString(Edit1->Text));
  double Max = Form1->Data.Calc(ToString(Edit2->Text));

  if(Max < Min)
    std::swap(Min, Max);

  if(TBaseFuncType *Func = dynamic_cast<TBaseFuncType*>(GraphElem))
  {
    Edit3->Text = RoundToStr(Func->GetFunc().CalcArc(Min, Max, 1000), Form1->Data);

    unsigned N1 = std::lower_bound(Func->sList.begin(), Func->sList.end(), Min, CompCoordSet1) - Func->sList.begin();
    unsigned N2 = std::upper_bound(Func->sList.begin() + N1, Func->sList.end(), Max, CompCoordSet2) - Func->sList.begin();
    if(N1 != N2)
      Form1->IPolygon1->AddPoints(&Func->Points[N1], N2 - N1);
    Form1->IPolygon1->Pen->Width = Func->Size;
  }
  else if(TTan *Tan = dynamic_cast<TTan*>(GraphElem))
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






