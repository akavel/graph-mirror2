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
#include "GuiHelper.h"
#include "PointSelect.h"
#include "OleObjectElem.h"
//---------------------------------------------------------------------------
//////////////
// TAddView //
//////////////
void TAddView::Visit(TBaseFuncType &Func)
{
  TTntTreeNode *Node = Form1->TreeView->Items->Add(NULL, ToWideString(Func.MakeLegendText()));
  int ImageIndex = Form1->AddImage(iiFuncNode, Func.Color);
  Node->ImageIndex = ImageIndex;
  Node->SelectedIndex = ImageIndex;
  Node->StateIndex = Func.GetVisible() ? iiChecked : iiUnChecked;

  for(unsigned I = 0; I < Func.ChildList.size(); I++)
    Func.ChildList[I]->Accept(*this);
}
//---------------------------------------------------------------------------
void TAddView::Visit(TTan &Tan)
{
  TTntTreeNode *Node = Form1->TreeView->Items->AddChild(Form1->GetNode(Tan.ParentFunc()), ToWideString(Tan.MakeLegendText()));

  Node->ImageIndex = Tan.TangentType == ttTangent ? iiTanNode : iiNormalNode;
  Node->SelectedIndex = Node->ImageIndex;
  Node->StateIndex = Tan.GetVisible() ? iiChecked : iiUnChecked;
}
//---------------------------------------------------------------------------
void TAddView::Visit(TShade &Shade)
{
  WideString Str = ToWideString(Shade.MakeLegendText());
  TTntTreeNode *Node = Form1->TreeView->Items->AddChild(Form1->GetNode(Shade.ParentFunc()), Str);
  int ImageIndex = Form1->AddImage(Shade.Color, Shade.BrushStyle);

  Node->ImageIndex = ImageIndex;
  Node->SelectedIndex = ImageIndex;
  Node->StateIndex = Shade.GetVisible() ? iiChecked : iiUnChecked;
}
//---------------------------------------------------------------------------
void TAddView::Visit(TPointSeries &Series)
{
  TTreeNode *Node = Form1->TreeView->Items->Add(NULL, ToWideString(Series.MakeLegendText()));

  std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
  Bitmap->Width = 16;
  Bitmap->Height = 16;
  TPointSelect::DrawPoint(Bitmap->Canvas, TPoint(8,8), Series.Style, Series.FrameColor, Series.FillColor, Series.Style == 7 ? 3 : 5);
  int ImageIndex = Form1->ImageList1->Add(Bitmap.get(), NULL);
  Node->ImageIndex = ImageIndex;
  Node->SelectedIndex = ImageIndex;
  Node->StateIndex = Series.GetVisible() ? iiChecked : iiUnChecked;
}
//---------------------------------------------------------------------------
void TAddView::Visit(TTextLabel &Label)
{
  TTreeNode *Node = Form1->TreeView->Items->Add(NULL, ToWideString(Label.MakeText()));

  Node->ImageIndex = iiLabelNode;
  Node->SelectedIndex = iiLabelNode;
  Node->StateIndex = Label.GetVisible() ? iiChecked : iiUnChecked;
}
//---------------------------------------------------------------------------
void TAddView::Visit(TRelation &Relation)
{
  TTreeNode *Node = Form1->TreeView->Items->Add(NULL, ToWideString(Relation.MakeLegendText()));

  if(Relation.GetRelationType() == rtInequality)
    Node->ImageIndex = Form1->AddImage(Relation.GetColor(), Relation.GetBrushStyle());
  else
    Node->ImageIndex = Form1->AddImage(iiFuncNode, Relation.GetColor());

  Node->SelectedIndex = Node->ImageIndex;
  Node->StateIndex = Relation.GetVisible() ? iiChecked : iiUnChecked;
}
//---------------------------------------------------------------------------
void TAddView::Visit(TAxesView &AxesView)
{
  TTreeNode *Node = Form1->TreeView->Items->Add(NULL, LoadRes(RES_AXES));
  Node->ImageIndex = iiAxesNode;
  Node->SelectedIndex = iiAxesNode;
  switch(AxesView.GetVisible())
  {
    case -1: Node->StateIndex =  iiGrayed; break;
    case 0:  Node->StateIndex =  iiUnChecked; break;
    case 1:  Node->StateIndex =  iiChecked; break;
  }
}
//---------------------------------------------------------------------------
void TAddView::Visit(TOleObjectElem &OleObjectElem)
{
  TTreeNode *Node = Form1->TreeView->Items->Add(NULL, ToWideString(OleObjectElem.MakeText()));
  Node->ImageIndex = iiOleNode;
  Node->SelectedIndex = iiOleNode;
  Node->StateIndex = OleObjectElem.GetVisible() ? iiChecked : iiUnChecked;
}
//---------------------------------------------------------------------------
///////////////
// TZoomView //
///////////////
TZoomFit::TZoomFit(const TData &AData, const TDraw &ADraw)
 : Data(AData), Draw(ADraw)
{
  //Always show axes when Axes style is Crossed
  xMin = Data.Axes.AxesStyle == asCrossed ? Data.Axes.yAxis.AxisCross : INF;
  xMax = Data.Axes.AxesStyle == asCrossed ? Data.Axes.yAxis.AxisCross : -INF;
  yMin = Data.Axes.AxesStyle == asCrossed ? Data.Axes.xAxis.AxisCross : INF;
  yMax = Data.Axes.AxesStyle == asCrossed ? Data.Axes.xAxis.AxisCross : -INF;
}
//---------------------------------------------------------------------------
void TZoomFit::Visit(TBaseFuncType &Func)
{
  if(TStdFunc *StdFunc = dynamic_cast<TStdFunc*>(&Func))
  {
    Func32::ECalcError E;
    if(std::_finite(StdFunc->From.Value))
      if(Func.GetFunc().Calc(StdFunc->From.Value, E), E.ErrorCode == Func32::ecNoError) //Note comma operator
        xMin = StdFunc->From.Value;
    if(std::_finite(StdFunc->To.Value))
      if(Func.GetFunc().Calc(StdFunc->To.Value, E), E.ErrorCode == Func32::ecNoError) //Note comma operator
        xMax = StdFunc->To.Value;
  }

  for(std::vector<Func32::TCoordSet>::const_iterator Iter = Func.sList.begin(); Iter != Func.sList.end(); ++Iter)
  {
    if(Iter->x < xMin)
      xMin = Iter->x;
    if(Iter->x > xMax)
      xMax = Iter->x;
    if(Iter->y < yMin)
      yMin = Iter->y;
    if(Iter->y > yMax)
      yMax = Iter->y;
  }
}
//---------------------------------------------------------------------------
void TZoomFit::Visit(TPointSeries &Series)
{
  for(std::vector<TPointSeriesPoint>::const_iterator Point = Series.PointList.begin(); Point != Series.PointList.end(); ++Point)
    //Check if point is valid
    if((!Data.Axes.xAxis.LogScl || Point->x > 0) && (!Data.Axes.yAxis.LogScl || Point->y > 0))
    {
      if(Point->x < xMin)
        xMin = Point->x.Value;
      if(Point->x > xMax)
        xMax = Point->x.Value;
      if(Point->y < yMin)
        yMin = Point->y.Value;
      if(Point->y > yMax)
        yMax = Point->y.Value;
    }
}
//---------------------------------------------------------------------------
void TZoomFit::Visit(TRelation &Relation)
{
  if(!Relation.Region)
    return;

  TRect Rect = Relation.Region->GetBoundingRect();
  xMin = Draw.xCoord(Rect.Left);
  xMax = Draw.xCoord(Rect.Right);
  yMin = Draw.yCoord(Rect.Bottom);
  yMax = Draw.yCoord(Rect.Top);
}
//---------------------------------------------------------------------------

