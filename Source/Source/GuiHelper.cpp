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
#include "Unit1.h"
#include <float.h>
//---------------------------------------------------------------------------
//////////////
// TAddView //
//////////////
TTreeNode* TAddView::AddNode(TGraphElem &Elem, int ImageIndex)
{
  //Replace Hyphen-Minus with Minus Sign
  std::wstring Str = Elem.MakeLegendText();
  std::replace(Str.begin(), Str.end(), L'-', L'\x2212');

  TTreeNode *Node = Elem.ParentFunc() ?
    Form1->TreeView->Items->AddChild(Form1->GetNode(Elem.ParentFunc()), Str.c_str()) :
    Form1->TreeView->Items->Add(NULL, Str.c_str());

  Node->ImageIndex = ImageIndex;
  Node->SelectedIndex = ImageIndex;
  switch(Elem.GetVisible())
  {
    case -1: Node->StateIndex =  iiGrayed; break;
    case 0:  Node->StateIndex =  iiUnChecked; break;
    case 1:  Node->StateIndex =  iiChecked; break;
  }
  return Node;
}
//---------------------------------------------------------------------------
void TAddView::Visit(TBaseFuncType &Func)
{
  AddNode(Func, Form1->AddImage(iiFuncNode, Func.Color));

  for(unsigned I = 0; I < Func.ChildList.size(); I++)
    Func.ChildList[I]->Accept(*this);
}
//---------------------------------------------------------------------------
void TAddView::Visit(TTan &Tan)
{
  AddNode(Tan, Tan.TangentType == ttTangent ? iiTanNode : iiNormalNode);
}
//---------------------------------------------------------------------------
void TAddView::Visit(TShade &Shade)
{
  AddNode(Shade, Form1->AddImage(Shade.Color, Shade.BrushStyle));
}
//---------------------------------------------------------------------------
void TAddView::Visit(TPointSeries &Series)
{

  std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
  Bitmap->Width = 16;
  Bitmap->Height = 16;
  TPointSelect::DrawPoint(
    Bitmap->Canvas,
    TPoint(8,8),
    Series.GetStyle(),
    Series.GetFrameColor(),
    Series.GetFillColor(),
    Series.GetStyle() == 7 ? 3 : 5);
  AddNode(Series, Form1->ImageList1->Add(Bitmap.get(), NULL));
}
//---------------------------------------------------------------------------
void TAddView::Visit(TTextLabel &Label)
{
  AddNode(Label, iiLabelNode);
}
//---------------------------------------------------------------------------
void TAddView::Visit(TRelation &Relation)
{
  int ImageIndex;
  if(Relation.GetRelationType() == rtInequality)
    ImageIndex = Form1->AddImage(Relation.GetColor(), Relation.GetBrushStyle());
  else
    ImageIndex = Form1->AddImage(iiFuncNode, Relation.GetColor());
  AddNode(Relation, ImageIndex);
}
//---------------------------------------------------------------------------
void TAddView::Visit(TAxesView &AxesView)
{
  AddNode(AxesView, iiAxesNode)->Text = LoadRes(RES_AXES);
}
//---------------------------------------------------------------------------
void TAddView::Visit(TOleObjectElem &OleObjectElem)
{
  AddNode(OleObjectElem, iiOleNode);
}
//---------------------------------------------------------------------------
//////////////
// TZoomFit //
//////////////
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
bool TZoomFit::IsChanged() const
{
  return xMin != xMax && yMin != yMax && _finite(xMin) && _finite(xMax) && _finite(yMin) && _finite(yMax);
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
  const TPointSeries::TPointList &PointList = Series.GetPointList();
  for(TPointSeries::TPointList::const_iterator Point = PointList.begin(); Point != PointList.end(); ++Point)
    //Check if point is valid
    if((!Data.Axes.xAxis.LogScl || Point->x > 0) && (!Data.Axes.yAxis.LogScl || Point->y > 0))
    {
      if(Point->x < xMin)
        xMin = Point->x;
      if(Point->x > xMax)
        xMax = Point->x;
      if(Point->y < yMin)
        yMin = Point->y;
      if(Point->y > yMax)
        yMax = Point->y;
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

