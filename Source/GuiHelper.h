/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef GuiHelperH
#define GuiHelperH
//---------------------------------------------------------------------------
struct TAddView : public TGraphElemVisitor
{
  void Visit(TBaseFuncType &Func);
  void Visit(TTan &Tan);
  void Visit(TShade &Shade);
  void Visit(TPointSeries &Series);
  void Visit(TTextLabel &Label);
  void Visit(TRelation &Relation);
  void Visit(TAxesView &AxesView);
  void Visit(TOleObjectElem &OleObjectElem);
};

struct TZoomFit : public TGraphElemVisitor
{
  const TData &Data;
  const TDraw &Draw;
  double xMin, xMax, yMin, yMax;
  TZoomFit(const TData &AData, const TDraw &ADraw);
  void Visit(TBaseFuncType &Func);
  void Visit(TTan &Tan) {} //Not used
  void Visit(TShade &Shade) {} //Not used
  void Visit(TPointSeries &Series);
  void Visit(TTextLabel &Label) {}  //Not used
  void Visit(TRelation &Relation);
  void Visit(TAxesView &AxesView) {} //Not used
  void Visit(TOleObjectElem &OleObjectElem) {}
};
#endif
