/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef DrawThreadH
#define DrawThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "IThread.h"
#pragma warn -8130
//---------------------------------------------------------------------------
namespace Graph
{
enum TDrawMessage {dmTerminate=WM_USER, dmDrawAll};

class TDrawThread : public Thread::TIThread, public TGraphElemVisitor
{
private:
  TData *Data;
  TDraw *Draw;
  TAxes &Axes;
  const TRect &AxesRect;
  TContext &Context;
  const double &SizeMul;
  volatile bool Aborted;

  int xPoint(long double x) const {return Draw->xPoint(x);}
  int yPoint(long double y) const {return Draw->yPoint(y);}
  TPoint xyPoint(const Func32::TDblPoint &P) const {return Draw->xyPoint(P);}

  template<typename T>
  void CalcFunc(TBaseFuncType &F, double sMin, double sMax, double ds, bool LogScl);

  void DrawAll();
  int SizeScale(int I){return I*SizeMul+0.5;};
  void __fastcall Execute();
  bool InsideImage(const TPoint &P);
  template<typename T> bool IsError(Func32::TErrorCode ErrorCode, const Func32::TCoord<T> &Coord);
  void PrepareFunction(TBaseFuncType *F);
  void ThreadTerminated(TObject*);
  TPoint GetFixedPoint(const TShading &Shade, const TPoint &P);

  void EquationLoop(TRelation &Relation, std::vector<TRect> &Points, bool Loop);
  static bool CheckResult1(const long double Result[3]);
  static bool CheckResult2(const long double Result[3]);

  //Draw elements
  void Visit(TBaseFuncType &Func);
  void Visit(TTan &Tan) {Visit(static_cast<TBaseFuncType&>(Tan));}
  void Visit(TShading &Shade);
  void Visit(TPointSeries &Series);
  void Visit(TTextLabel &Label);
  void Visit(TRelation &Relation);
  void Visit(TAxesView &AxesView);

  void CreateShade(TShading &Shade);
  void CreateEquation(TRelation &Relation);
  void CreateInequality(TRelation &Relation);

public:

  __fastcall TDrawThread(TDraw *ADraw);
  void AbortUpdate() {Aborted = true;}
  bool IsAborted() const {return Aborted;}
};
//---------------------------------------------------------------------------
} //namespace Graph
#endif
