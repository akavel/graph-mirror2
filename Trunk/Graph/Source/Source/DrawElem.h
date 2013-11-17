/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2013 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef DrawElemH
#define DrawElemH
//---------------------------------------------------------------------------
namespace Graph
{
class TDrawElem : public TGraphElemVisitor
{
private:
  TDraw *Draw;
  const TAxes &Axes;
  const TRect &AxesRect;
  TContext &Context;
  bool ForceBlack;
  unsigned PlotIndex; //Next elem to plot

  inline int xPoint(long double x) const;
  inline int yPoint(long double y) const;
  inline TPoint xyPoint(const Func32::TDblPoint &P) const;

  inline int SizeScale(int I);
  bool InsideImage(const TPoint &P);
  template<typename T> bool IsError(Func32::TErrorCode ErrorCode, const Func32::TCoord<T> &Coord);
  void DrawEndPoints(const TBaseFuncType &Func);
  void DrawEndPoint(const TBaseFuncType &Func, long double t, const TPoint &Pos, unsigned Style, bool InvertArrow);
  void DrawEndPoint(const TBaseFuncType &Func, long double t, unsigned Style, bool InvertArrow);
  void PrepareFunction(TBaseFuncType *F);
  void DrawArrow(const TPoint &Point, long double Angle, TColor Color, unsigned Size);
  void DrawHalfCircle(const TPoint &Point, long double Angle, TColor Color, unsigned Size);
  void DrawSquareBracket(const TPoint &Point, long double Angle, TColor Color, unsigned Size);
  void LineToAngle(int X, int Y, double Angle, double Length);
  void ThreadTerminated(TObject*);
  TPoint GetFixedPoint(const TShading &Shade, const TPoint &P);
  long double RealToVisualAngle(long double Angle);

  void DrawPointSeries(const TPointSeries &PointSeries);
  void DrawFunction(const TBaseFuncType &Func);
  void DrawShade(const TShading &Shade);
  void DrawLabel(TTextLabel &Label);
  void DrawRelation(const TRelation &Relation);

  //Draw elements
  void Visit(TBaseFuncType &Func);
  void Visit(TTan &Tan) {Visit(static_cast<TBaseFuncType&>(Tan));}
  void Visit(TShading &Shade);
  void Visit(TPointSeries &Series);
  void Visit(TTextLabel &Label);
  void Visit(TRelation &Relation);
  void Visit(TAxesView &AxesView);

public:
  TDrawElem::TDrawElem(TDraw *ADraw, bool AForceBlack, const TRect &AAxesRect, const TAxes &AAxes, TContext &Context);
  void DrawNext();
  void ClearPlotIndex() {PlotIndex = 0;}
};
//---------------------------------------------------------------------------
} //namespace Graph
#endif
