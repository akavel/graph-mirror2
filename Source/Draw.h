/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef DrawH
#define DrawH
//---------------------------------------------------------------------------
#include "Data.h"
#include "Context.h"
#include <boost/function.hpp>

class TDrawThread;
typedef std::vector<TPoint>::iterator TPointIter;
typedef std::vector<TPoint>::const_iterator TConstPointIter;

typedef boost::function0<void> TOnCompleteEvent;

class TDraw
{
  friend TDrawThread;
  friend class TDrawLegend;
  ::TContext Context;
  double xScale, yScale;  //The scale of x-axis and y-axis
  int Width, Height;      //Width and height of image
  TRect AxesRect;         //Rectangle where the coordinate system is shown
  TData *Data;
  TAxes &Axes;
  TDrawThread *Thread;
  TRect LegendRect;
  bool ForceBlack;
  double SizeMul;
  TOnCompleteEvent OnComplete;

  void DrawPolyline(TConstPointIter Begin, TConstPointIter End, TPenStyle Style, int LineSize, TColor Color);
  void DrawPolydots(TConstPointIter Begin, TConstPointIter End, int LineSize, TColor Color);
  void DrawLegend();
  static double GetMinValue(double Unit, double Min, double Max, double AxisCross, bool Log);
  void ShowNumber(int X, int Y, double Number, bool MultiplyByPi);
  unsigned NumberWidth(double Number, bool MultiplyByPi);

  TDraw(const TDraw&); //Not implemented
  TDraw& operator=(const TDraw&); //Not implemented

public:
  TDraw(TCanvas *Canvas, TData *pData, bool AForceBlack, const std::string &ThreadName);
  ~TDraw();
  void DrawAll();
  void DrawFunc(const TGraphElem *F);
  int xPoint(long double x) const; //Return pixel coresponding to x value
  int yPoint(long double y) const; //Return pixel coresponding to y value
  TPoint xyPoint(long double x, long double y) const {return TPoint(xPoint(x), yPoint(y));}
  template<typename T>
  TPoint xyPoint(const Func32::TCoord<T> &Coord) const {return TPoint(xPoint(real(Coord.x)), yPoint(real(Coord.y)));}
  double xPointExact(long double x) const; //Same as xPoint(), but returns double and don't check for error
  double yPointExact(long double y) const;
  double xCoord(int x) const;
  double yCoord(int y) const;
  Func32::TDblPoint xyCoord(int x, int y) const {return Func32::TDblPoint(xCoord(x), yCoord(y));}
  bool AbortUpdate();
  bool Wait();
  bool Updating();
  void SetSize(int Width, int Height);
  int Size(int I) const {return I*SizeMul+0.5;}
  void SetArea(const TRect &Area);
  void RedrawAxes();
  void DrawAxes();
  void SetClippingRegion();
  double GetScaledYAxis() const;
  static bool CalcTan(TTan *Tan);
  void SetSizeMul(double xSizeMul, double ySizeMul);
  bool InsideLegend(int X, int Y) const {return Data->Axes.ShowLegend && InsideRect(LegendRect, TPoint(X, Y));}
  static void DrawPointLabel(TCanvas *Canvas, TPoint Pos, int PointSize, const std::string &Label, TLabelPosition LabelPosition);
  void SetOnComplete(TOnCompleteEvent AOnComplete) {OnComplete = AOnComplete;}
  const TRect& GetLegendRect() const {return LegendRect;}
  void SetCanvas(TCanvas *Canvas) {Context.SetCanvas(Canvas);}
  const TRect& GetAxesRect() const {return AxesRect;}
};

#endif
