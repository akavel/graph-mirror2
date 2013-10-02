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
#include "Unit1.h"
#include "DrawThread.h"
#include <complex>
#include <cfloat>
#include <values.h>
#include <algorithm>
#include <ctime>
#include <cfloat>
#include <set>
#include "IGraphic.h"
#include "StackTrace.h"
#pragma warn -8072 //Disable warning: Suspicous pointer arithmetic
#include <boost/tr1/complex.hpp>

namespace Graph
{
const unsigned MaxPixelDist = 10;
const unsigned MaxExtraPoints = 100;
//---------------------------------------------------------------------------
using std::real;
using std::imag;
//---------------------------------------------------------------------------
template<typename T> inline bool TDrawThread::IsError(Func32::TErrorCode ErrorCode, const Func32::TCoord<T> &Coord)
{
  return ErrorCode != Func32::ecNoError || std::abs(imag(Coord.x)) > MIN_ZERO || std::abs(imag(Coord.y)) > MIN_ZERO ||
    (Data->Axes.xAxis.LogScl && real(Coord.x) <= 0) || (Data->Axes.yAxis.LogScl && real(Coord.y) <= 0);
}
//---------------------------------------------------------------------------
inline bool TDrawThread::InsideImage(const TPoint &P)
{
  return P.x >= AxesRect.Left && P.y >= AxesRect.Top && P.x <= AxesRect.Right && P.y <= AxesRect.Bottom;
}
//---------------------------------------------------------------------------
__fastcall TDrawThread::TDrawThread(TDraw *ADraw)
 : Thread::TIThread(true),
   Data(ADraw->Data), AxesRect(ADraw->AxesRect), Aborted(false),
   SizeMul(ADraw->SizeMul), Draw(ADraw), Context(ADraw->Context), Axes(ADraw->Axes)
{
}
//---------------------------------------------------------------------------
void __fastcall TDrawThread::Execute()
{
	randomize();
  bool Init = true;

  while(!Terminated)
  {
    try
    {
      if(Aborted)
        ClearMessageQueue();
      Aborted = false;
      if(!HasMessage())
        Draw->IncThreadInIdle(Init);
      Init = false;
      TMessage Message;
      GetMessage(Message);

      //No need to prepare drawings if we are just terminating
      if(Message.Msg == dmTerminate)
        return;

      switch(Message.Msg)
      {
        case dmDrawAll:
          DrawAll();
          break;
      }
    }
    catch(Exception &E)
    {
      LogUncaughtException(this, &E);
      ShowStatusError("Internal error. Log file Graph.err created.");
    }
    catch(std::exception &E)
    {
      LogUncaughtCppException(E.what(), "DrawThread");
      ShowStatusError("Internal error. Log file Graph.err created.");
    }
    catch(...)
    {
      LogUncaughtCppException(NULL, "DrawThread");
      ShowStatusError("Internal error. Log file Graph.err created.");
    }
  }
}
//---------------------------------------------------------------------------
bool TDrawThread::EvalNext()
{
  TGraphElemPtr Elem = Draw->GetNextEvalElem();
  if(!Elem)
    return false;
  if(Elem->GetVisible() && !Aborted)
    Elem->Accept(*this);

  unsigned Count = Elem->ChildCount();
  for(unsigned N = 0; N < Count && !Aborted; N++)
  {
    const TGraphElemPtr &Child = Elem->GetChild(N);
    if(Child->GetVisible())
      Child->Accept(*this);
  }

  if(!Aborted)
    Elem->SetUpdateFinished();
  return !Aborted && !Terminated;
}
//---------------------------------------------------------------------------
void TDrawThread::DrawAll()
{
  while(EvalNext())
    Queue(&Draw->DrawElem.DrawNext);
}
//---------------------------------------------------------------------------
void TDrawThread::PrepareFunction(TBaseFuncType *F)
{
  if(F->Points.empty())
  {
    //Use axes range for standard functions if the axes range is smaller than the function range
    double Min, Max, ds;
    F->GetCurrentRange(Min, Max, ds);
    if(Min > Max)
      return; //No range

    bool LogScl = Axes.xAxis.LogScl && ds == 0;
    if(ds == 0)
      ds = LogScl ? std::exp(1/Draw->xScale) : 1/Draw->xScale;

    if(ds <= 0)
      ds = 1; //ds must always be a positive number

    //Avoid calculations on an empty range  
    if(!IsZero(Max - Min))
    {
      if(Data->Axes.CalcComplex)
        CalcFunc<Func32::TComplex>(*F, Min, Max, ds, LogScl);
      else
        CalcFunc<long double>(*F, Min, Max, ds, LogScl);
    }
  }
}
//---------------------------------------------------------------------------
void TDrawThread::Visit(TBaseFuncType &Func)
{
  PrepareFunction(&Func);
}
//---------------------------------------------------------------------------
//This function draws a mathematical function on the canvas
//Func is the function to draw; sMin and sMax are the range
//ds is the increment between the calculations
template<typename T>
void TDrawThread::CalcFunc(TBaseFuncType &F, double sMin, double sMax, double ds, bool LogScl)
{
  try
  {
    Func32::ECalcError CalcError;
    const Func32::TBaseFunc &Func = F.GetFunc();
    Func32::TCoord<T> Coord = Func.Calc(T(sMin), CalcError); //Calc first x- and y-coordinate
    TPoint Pos;

    //Calculate position in pixel coordinates
    TPoint LastPos;
    if(!CalcError.ErrorCode)
      LastPos = TPoint(xPoint(real(Coord.x)), yPoint(real(Coord.y)));
    unsigned OldSize = 0;
    int Err = 4; //0=no error; 1=calc error; 2=croped; 3=crossing image boundary; 4=start (first point)

    bool Finished = false;
    double sLast = sMin;
    //WARNING: Because of bug in Bcc 5.6.4 we need to put sLast = s inside the ?: operator instead of before
    for(double s = sMin; !Finished && !Aborted; LogScl ? (sLast = s, s*=ds) : (sLast = s, s+=ds))
    {
      //Last point will always be exactly at sMax
      if(s > sMax)
      {
        s = sMax;
        Finished = true;
      }

      try
      {
        Coord = Func.Calc(T(s), CalcError);    //Calc x- and y-coordinate

        Pos.x = xPoint(real(Coord.x));
        Pos.y = yPoint(real(Coord.y));

        //If an error occurred or x or y have an imaginary part
        if(IsError(CalcError.ErrorCode, Coord))
        {
          //If we just went outside defined range
          //Use binary search to find the place between the last defined point and
          //the undefined point where the pixel position no longer changes.
          if(!Err && F.DrawType == dtAuto)
          {
            double s2 = sLast;   //The last defined point
            Pos = LastPos;       //Current pixel position

            do
            {
              double sMiddle = LogScl ? std::sqrt(s*s2) : (s + s2) / 2; // Average between s and s2
              Coord = Func.Calc(T(sMiddle), CalcError);
              Err = IsError(CalcError.ErrorCode, Coord);
              if(Err)
                s = sMiddle;
              else
              {
                s2 = sMiddle;
                LastPos = Pos;
                Pos.x = xPoint(real(Coord.x));
                Pos.y = yPoint(real(Coord.y));
             }
            }
            while((Err || (LastPos != Pos && InsideImage(Pos))) && std::abs(s-s2) > 1E-12);

            //If we have a new position, add it. We do not check for error here.
            //Even if the last calculation was an error, we may have a better position.
            if(Pos != F.Points.back())
            {
              F.Points.push_back(Pos);
              F.sList.push_back(Func32::TCoordSet<>(s2, real(Coord.x), real(Coord.y)));
            }
          }
          Err = true;
          continue;
        }

        //If no error occured: Check if Pos and LastPos are not outside the image on the same side
        if(!Err && F.DrawType == dtAuto)
          if(!((Pos.x < 0 && LastPos.x < 0) || (Pos.x > AxesRect.Right && LastPos.x > AxesRect.Right) || (Pos.y < 0 && LastPos.y < 0) || (Pos.y > AxesRect.Bottom && LastPos.y > AxesRect.Bottom)))
          {
            unsigned Count = MaxExtraPoints; //Use a max loop count to prevent the algorithm from running wild
            //If there is more than 10 pixels between the two points, calculate a point in the middle.
            //We use virtical and horizontal dist to avoid overflow in square calculation
            while(--Count && MaxDist(Pos, LastPos) > MaxPixelDist)
            {
              double sMiddle = LogScl ? std::sqrt(s*sLast) : (s + sLast) / 2; // Average between s and sLast
              Coord = Func.Calc(T(sMiddle), CalcError);
              Err = IsError(CalcError.ErrorCode, Coord);
              if(Err)
                break;

              //Cosider the new point to be part of the line it is closest to
              TPoint P = Draw->xyPoint(Coord);
              if(MaxDist(P, LastPos) < MaxDist(P, Pos))
              {
                if(P == LastPos)
                {
                  Err = 3; //Simulate error at LastPos to make Pos part of a new line
                  break;
                }
                sLast = sMiddle;
                LastPos = P;
                F.Points.push_back(LastPos);
                F.sList.push_back(Func32::TCoordSet<>(sLast, real(Coord.x), real(Coord.y)));
              }
              else
              {
                if(P == Pos)
                {
                  Err = 3; //Simulate error at LastPos to make Pos part of a new line
                  break;
                }
                s = sMiddle;
                Pos = P;
                Finished = false;
              }

              //If both points are outside the image, simulate an error at LastPos to break the line between LastPos and Pos
              if(!InsideRect(AxesRect, LastPos) && !InsideRect(AxesRect, Pos))
              {
                Err = 3; //Simulate error at LastPos to make Pos part of a new line
                break;
              }
            }
          }

        if(Err)             //If an error happened last but not now
        {
          //If points has been added since last calculation
          if(F.Points.size() > OldSize)
            //Add number of points in piece of line
            F.PointNum.push_back(F.Points.size() - OldSize);

          OldSize = F.Points.size();

          //If line was split because of we were out of image area
          if(Err == 2)
          {
             //Add last point
             F.Points.push_back(LastPos);
             Func32::TCoord<T> Coord = Func.Calc(T(s-ds), CalcError);
             F.sList.push_back(Func32::TCoordSet<>(s - ds, real(Coord.x), real(Coord.y)));
          }
          else if(Err == 1 && F.DrawType == dtAuto)   //If calculation error
          {
            //If we just went inside defined range
            //Use binary tree to find the place between the last undefined point and
            //the defined point where the pixel position no longer changes.
            double s1 = sLast;  //The last undefined point
            double s2 = s;      //The first defined point
            TPoint Pos2 = Pos;  //Pixel position at s2
            LastPos = Pos2;
            Func32::TCoord<T> Coord2;

            do
            {
              double sMiddle = LogScl ? std::sqrt(s1 * s2) : (s1 + s2) / 2; //Average between s1 and s2
              Coord2 = Func.Calc(T(sMiddle), CalcError);

              Err = IsError(CalcError.ErrorCode, Coord2);
              if(Err)
                s1 = sMiddle;
              else
              {
                s2 = sMiddle;
                LastPos = Pos2;
                Pos2.x = xPoint(real(Coord2.x));
                Pos2.y = yPoint(real(Coord2.y));
              }
            }
            while((Err || (LastPos != Pos2 && InsideImage(LastPos))) && std::abs(s1-s2) > 1E-12);

            //Add point if we have a new one. Don't check Err. Even if the last calculation
            //was an error, Pos2 might be better than Pos
            if(Pos2 != Pos)
            {
              F.Points.push_back(Pos2);
              F.sList.push_back(Func32::TCoordSet<>(s2, real(Coord2.x), real(Coord2.y)));
            }
          }

          //Add point to vector
          F.Points.push_back(Pos);
          F.sList.push_back(Func32::TCoordSet<>(s, real(Coord.x), real(Coord.y)));
          Err = false;            //Indicate no error
        }
        else //No errors
        {
          //Add point to vector
          if(Pos != LastPos)
          {
            F.Points.push_back(Pos);
            F.sList.push_back(Func32::TCoordSet<>(s, real(Coord.x), real(Coord.y)));
          }
        }
        LastPos = Pos; //Save point
      }
      catch(EOverflow&)         //"e^1000" results in EOverflow when casting from long double to double
      {                         //Note: The exception is not thrown until xPoint(x)
        Err = true;
        continue;
      }
    }
    if(F.Points.size() > OldSize)
      F.PointNum.push_back(F.Points.size() - OldSize);

    //Clear points if aborted
    if(Aborted)
      F.ClearCache();
  }
  catch(Exception &E)
  {
    ShowStatusError("Internal error. Unexpected exception: " + E.Message);
  }
  catch(std::exception &E)
  {
    ShowStatusError("Internal error. Unexpected exception: " + String(E.what()));
  }
  catch(...)
  {
    ShowStatusError("Internal error. Unknown exception");
  }
}
//---------------------------------------------------------------------------
void TDrawThread::Visit(TShading &Shade)
{
  if(Shade.Region.get() == NULL)
    CreateShade(Shade);
}
//---------------------------------------------------------------------------
int ComparePoint(const TPoint &P1, const TPoint &P2, TShadeStyle Style)
{
  switch(Style)
  {
    case ssAbove:
    case ssBelow:
    case ssXAxis:
      return P2.x == P1.x ? 0 : (P2.x > P1.x ? 1 : -1);

    case ssYAxis:
      return P2.y == P1.y ? 0 : (P2.y > P1.y ? 1 : -1);

    default:
      return 0;
  }
}
//---------------------------------------------------------------------------
TPoint TDrawThread::GetFixedPoint(const TShading &Shade, const TPoint &P)
{
  switch(Shade.ShadeStyle)
  {
    case ssAbove:
      return TPoint(P.x, AxesRect.Top - 1);
    case ssBelow:
      return TPoint(P.x, AxesRect.Bottom + 1);
    case ssXAxis:
      return TPoint(P.x, Draw->yPixelCross);
    case ssYAxis:
      return TPoint(Draw->xPixelCross, P.y);
    default:
      return P;
  }
}
//---------------------------------------------------------------------------
void TDrawThread::CreateShade(TShading &Shade)
{
  TBaseFuncType *F = dynamic_cast<TBaseFuncType*>(Shade.GetParent().get());

  //If the shading is attached to a Func2, we continue evaluating until
  //Func2 has been evaluated so we can use the data from it.
  if(Shade.Func2)
    while(!Shade.Func2->IsUpdateFinished())
      if(!EvalNext())
        return;

	if(F->sList.empty() || (Shade.Func2.get() && Shade.Func2->sList.empty()))
    return;

  //The intervals can go both ways. Make sure sMin is always less than sMax
  double sMin = std::min(Shade.sMin.Value, Shade.sMax.Value);
  double sMax = std::max(Shade.sMin.Value, Shade.sMax.Value);
  double Min, Max;

  //If shade is outside the function area, nothing to draw
  if(sMax < F->sList.front().t || sMin > F->sList.back().t)
    return;

  if(dynamic_cast<const TStdFunc*>(F))
  {
    //If Min or Max is outside coordinate system: Make sure they are just one pixel outside
    Min = Range(std::max(Draw->xCoord(-1), F->From.Value), sMin, Draw->xCoord(AxesRect.Right+1));
    Max = Range(Draw->xCoord(-1), sMax, std::min(Draw->xCoord(AxesRect.Right+1), F->To.Value));
  }
  else
  {
    Min = std::_finite(sMin) ? sMin : F->From.Value;
    Max = std::_finite(sMax) ? sMax : F->To.Value;
  }

  //Find starting and ending points in point vector
  unsigned N1 = std::lower_bound(F->sList.begin(), F->sList.end() - 1, Min, TCompCoordSet()) - F->sList.begin();
  unsigned N2 = std::upper_bound(F->sList.begin() + N1, F->sList.end(), Max, TCompCoordSet()) - F->sList.begin() - 1;

	if(N2 <= N1)
		return; //This may happen because we subtracted 1 when we calculated N2

  //Position of x-axis on the y-axis
  int yAxisPixel = Range<int>(AxesRect.Top, yPoint(Axes.xAxis.AxisCross), AxesRect.Bottom);

  //Position of y-axis on the x-axis
  int xAxisPixel = Range<int>(AxesRect.Left, xPoint(Axes.yAxis.AxisCross), AxesRect.Right);

  if(Shade.ExtendMinToIntercept)
  {
    switch(Shade.ShadeStyle)
    {
      case ssAbove:
      case ssBelow:
      case ssXAxis:
        int CrossLine;
        if(Shade.ShadeStyle == ssAbove)
          CrossLine = 0;
        else if(Shade.ShadeStyle == ssBelow)
          CrossLine = AxesRect.Bottom;
        else
          CrossLine = yAxisPixel;

        //Decrease N1 until F->Points[N1] intercepts with the x-axis, top or bottom of image
        bool Above;
        Above = F->Points[N1].y < CrossLine;
        while(N1 && (Above ? F->Points[N1].y < CrossLine : F->Points[N1].y > CrossLine))
          --N1;
        break;
      case ssYAxis:
        //Decrease N1 until F->Points[N1] intercepts with the y-axis
        bool Left;
        Left = F->Points[N1].x < xAxisPixel;
        while(N1 && (Left ? F->Points[N1].x < xAxisPixel : F->Points[N1].x > xAxisPixel))
          --N1;
        break;
      case ssBetween:
        N1 = F->Points.rend() - FindCrossing(F->Points.rend() - (N1+1), F->Points.rend(), Shade.Func2->Points.begin(), Shade.Func2->Points.end());
        if(N1)
          N1--;
        break;
      case ssInside:
        N1 = F->Points.rend() - FindCrossing(F->Points.rend() - (N1+1), F->Points.rend(), F->Points.begin() + N2, F->Points.end());
        if(N1)
          N1--;
    }
    Min = F->sList[N1].t;
  }

  if(Shade.ExtendMaxToIntercept)
  {
    switch(Shade.ShadeStyle)
    {
      case ssAbove:
      case ssBelow:
      case ssXAxis:
        int CrossLine;
        if(Shade.ShadeStyle == ssAbove)
          CrossLine = 0;
        else if(Shade.ShadeStyle == ssBelow)
          CrossLine = AxesRect.Bottom;
        else
          CrossLine = yAxisPixel;

        //Increase N2 until F->Points[N2] intercepts with the x-axis, top or bottom of image
        bool Above;
        Above = F->Points[N2].y < CrossLine;
        while(N2 < F->Points.size() && (Above ? F->Points[N2].y < CrossLine : F->Points[N2].y > CrossLine))
          ++N2;
        break;

      case ssYAxis:
        //Increase N2 until F->Points[N2] intercepts with the y-axis
        bool Left;
        Left = F->Points[N2].x < xAxisPixel;
        while(N2 < F->Points.size() && (Left ? F->Points[N2].x < xAxisPixel : F->Points[N2].x > xAxisPixel))
          ++N2;
        break;

      case ssBetween:
        N2 = FindCrossing(F->Points.begin() + N2, F->Points.end(), Shade.Func2->Points.begin(), Shade.Func2->Points.end()) - F->Points.begin();
        break;

      case ssInside:
        N2 = FindCrossing(F->Points.begin() + N2, F->Points.end(), F->Points.rend() - N1, F->Points.rend()) - F->Points.begin();
    }
    if(N2)
      N2--;
    Max = F->sList[N2].t;
  }

  //Find start and end points of function; May not have been drawn, and may not be valid
  int x1, y1, x2, y2;
  try
  {
    x1 = xPoint(F->GetFunc().CalcX(Min));
    y1 = yPoint(F->GetFunc().CalcY(Min));
  }
  catch(...)
  {
    x1 = F->Points[N1].x;
    y1 = F->Points[N1].y;
  }

  try
  {
    x2 = xPoint(F->GetFunc().CalcX(Max));
    y2 = yPoint(F->GetFunc().CalcY(Max));
  }
  catch(...)
  {
    x2 = F->Points[N2].x;
    y2 = F->Points[N2].y;
  }

  if(x1 == AxesRect.Left)
    x1--;
  if(x2 == AxesRect.Right)
    x2++;
  if(y1 == AxesRect.Bottom)
    y1++;
  if(y2 == AxesRect.Top)
    y2--;

  boost::shared_ptr<TRegion> Region(new TRegion(TRect(0,0,0,0)));
  unsigned CountIndex = 0;
  unsigned Sum = 0;
  while(Sum <= N1)
    Sum += F->PointNum[CountIndex++];

  switch(Shade.ShadeStyle)
  {
    case ssAbove:
    case ssBelow:
    case ssXAxis:
    case ssYAxis:
    {
      std::vector<TPoint> Points;
      //Set start point
      Points.push_back(GetFixedPoint(Shade, TPoint(x1, y1)));
      Points.push_back(TPoint(x1, y1));
      int y = F->Points[N1].y;
      unsigned I = N1+1;
      bool Pos = ComparePoint(F->Points[N1], F->Points[I], Shade.ShadeStyle) >= 0;
      while(I < N2+1)
      {
        while(I < N2+1 && I < Sum)
        {
          int Pos2 = ComparePoint(F->Points[I-1], F->Points[I], Shade.ShadeStyle);
          if(Pos ? Pos2 < 0 : Pos2 > 0)
            break;
          I++;
        }
        if(I == Sum)
        {
          if(CountIndex < F->PointNum.size())
            Sum += F->PointNum[CountIndex++];
        }
        else
          Pos = !Pos;
        Points.insert(Points.end(), F->Points.begin()+N1, F->Points.begin()+I);
        if(I < N2+1)
        {
          Points.push_back(GetFixedPoint(Shade, Points.back()));
          *Region |= TRegion(Points);
          Points.clear();
          N1 = I;
          Points.push_back(GetFixedPoint(Shade, F->Points[I]));
        }
      }
      Points.push_back(TPoint(x2, y2));
      Points.push_back(GetFixedPoint(Shade, TPoint(x2, y2)));
      *Region |= TRegion(Points);
      Region->Widen(0, 0, 1, 1);
      Points.clear();
      break;
    }

    case ssInside:
      if(N1 != N2)
        *Region |= TRegion(&F->Points[N1], N2-N1);
      break;

    case ssBetween:
    {
      std::vector<TPoint> Points;
      Points.push_back(TPoint(x1, y1));
      Points.insert(Points.end(), F->Points.begin()+N1, F->Points.begin()+N2+1);
      Points.push_back(TPoint(x2, y2));

      //The intervals can go both ways. Make sure sMin is always less than sMax
      double sMin2 = std::min(Shade.sMin2.Value, Shade.sMax2.Value);
      double sMax2 = std::max(Shade.sMin2.Value, Shade.sMax2.Value);

      double Min2, Max2;

      if(dynamic_cast<const TStdFunc*>(Shade.Func2.get()))
      {
        Min2 = Range(Draw->xCoord(-1), sMin2, Draw->xCoord(AxesRect.Right+1));
        Max2 = Range(Draw->xCoord(-1), sMax2, Draw->xCoord(AxesRect.Right+1));
      }
      else
      {
        Min2 = std::_finite(sMin2) ? sMin2 : Shade.Func2->From.Value;
        Max2 = std::_finite(sMax2) ? sMax2 : Shade.Func2->To.Value;
      }

      //Find starting and ending points in point vector
      //If list is empty: M1=-1, M2=-1
      int M1 = std::lower_bound(Shade.Func2->sList.begin(), Shade.Func2->sList.end() - 1, Min2, TCompCoordSet()) - Shade.Func2->sList.begin();
      int M2 = std::upper_bound(Shade.Func2->sList.begin()+M1, Shade.Func2->sList.end(), Max2, TCompCoordSet()) - Shade.Func2->sList.begin() - 1;

      //Find start and end points of function; May not have been drawn, and may not be valid
      int X1, Y1, X2, Y2;
      try
      {
        X1 = xPoint(Shade.Func2->GetFunc().CalcX(Min2));
        Y1 = yPoint(Shade.Func2->GetFunc().CalcY(Min2));
      }
      catch(...)
      {
        X1 = Shade.Func2->Points[M1].x;
        Y1 = Shade.Func2->Points[M1].y;
      }

      try
      {
        X2 = xPoint(Shade.Func2->GetFunc().CalcX(Max2));
        Y2 = yPoint(Shade.Func2->GetFunc().CalcY(Max2));
      }
      catch(...)
      {
        X2 = Shade.Func2->Points[M2].x;
        Y2 = Shade.Func2->Points[M2].y;
      }

      //Check if Max is actually less than min for one of the functions
      bool SwapMinMax = (Shade.sMin.Value < Shade.sMax.Value) != (Shade.sMin2.Value < Shade.sMax2.Value);

      if(X1 == AxesRect.Left)
        X1--;
      if(X2 == AxesRect.Right)
        X2++;
      if(Y1 == AxesRect.Bottom)
        Y1++;
      if(Y2 == AxesRect.Top)
        Y2--;

      //Add start point to the polygon points list
      if(!Shade.ExtendMax2ToIntercept)
        Points.push_back(SwapMinMax ? TPoint(X1, Y1) : TPoint(X2, Y2));

      if(Shade.ExtendMin2ToIntercept)
      {
        M1 = Shade.Func2->Points.rend() -
          FindCrossing(Shade.Func2->Points.rend() - (M1+1), Shade.Func2->Points.rend(), F->Points.begin(), F->Points.end());
        if(M1)
          M1--;
      }

      if(Shade.ExtendMax2ToIntercept)
      {
        M2 = FindCrossing(Shade.Func2->Points.begin() + M2, Shade.Func2->Points.end(), F->Points.begin(), F->Points.end()) - Shade.Func2->Points.begin();
        if(M2)
          M2--;
      }

      //Add points on the second function to the polygon points list
      if(M2 > M1)
      {
        if(SwapMinMax)
          Points.insert(Points.end(), Shade.Func2->Points.begin()+M1, Shade.Func2->Points.begin()+M2);
        else
          Points.insert(Points.end(), Shade.Func2->Points.rend()-M2-1, Shade.Func2->Points.rend()-M1);
      }

      //Add end point to the polygon points list
      //Add start point to the polygon points list
      if(!Shade.ExtendMin2ToIntercept)
        Points.push_back(SwapMinMax ? TPoint(X2, Y2) : TPoint(X1, Y1));
      *Region |= TRegion(Points);
      break;
    }
  }

  Shade.Region = Region;
}
//---------------------------------------------------------------------------
void TDrawThread::Visit(TTextLabel &Label)
{
}
//---------------------------------------------------------------------------
void TDrawThread::Visit(TPointSeries &PointSeries)
{
}
//---------------------------------------------------------------------------
void TDrawThread::Visit(TAxesView &AxesView)
{
}
//---------------------------------------------------------------------------
struct TCompY
{
  inline bool operator()(const TPoint &P1, const TPoint &P2) const
  {
    return P1.y == P2.y ? P1.x < P2.x : P1.y < P2.y;
  }
};
//---------------------------------------------------------------------------
struct TCompX
{
  inline bool operator()(const TPoint &P1, const TPoint &P2) const
  {
    return P1.x == P2.x ? P1.y < P2.y : P1.x < P2.x;
  }
};
//---------------------------------------------------------------------------
void RemoveSharedVertice(std::set<TPoint,TCompY> &Points, const TPoint &P)
{
  std::set<TPoint,TCompY>::iterator Iter = Points.find(P);
  if(Iter != Points.end())
    Points.erase(Iter);
  else
    Points.insert(P);
}
//---------------------------------------------------------------------------
//Converted from Python script found at
//http://stackoverflow.com/questions/13746284/merging-multiple-adjacent-rectangles-into-one-polygon
void RegionToPolygons(const std::vector<TRect> &Region, std::vector<TPoint> &PolygonPoints, std::vector<int> &PolygonCount)
{
  std::set<TPoint,TCompY> yPoints;
  for(std::vector<TRect>::const_iterator RectIter = Region.begin(); RectIter != Region.end(); ++RectIter)
  {
    RemoveSharedVertice(yPoints, RectIter->TopLeft());
    RemoveSharedVertice(yPoints, RectIter->BottomRight());
    RemoveSharedVertice(yPoints, TPoint(RectIter->Right, RectIter->Top));
    RemoveSharedVertice(yPoints, TPoint(RectIter->Left, RectIter->Bottom));
  }

  std::map<TPoint,TPoint,TCompY> hEdges;
  std::set<TPoint,TCompY>::const_iterator Iter = yPoints.begin();
  while(Iter != yPoints.end())
  {
    int Y = Iter->y;
    while(Iter != yPoints.end() && Iter->y == Y)
    {
      const TPoint &P = *Iter;
      ++Iter;
      hEdges.insert(std::make_pair(P, *Iter));
      hEdges.insert(std::make_pair(*Iter, P));
      ++Iter;
    }
  }

  std::map<TPoint,TPoint,TCompX> vEdges;
  std::set<TPoint,TCompX> xPoints(yPoints.begin(), yPoints.end());
  std::set<TPoint,TCompX>::const_iterator Iter2 = xPoints.begin();
  while(Iter2 != xPoints.end())
  {
    int X = Iter2->x;
    while(Iter2 != xPoints.end() && Iter2->x == X)
    {
      const TPoint &P = *Iter2;
      ++Iter2;
      vEdges.insert(std::make_pair(P, *Iter2));
      vEdges.insert(std::make_pair(*Iter2, P));
      ++Iter2;
    }
  }

  //Get all the polygons
  while(!hEdges.empty())
  {
    std::vector<std::pair<TPoint,int> > Polygon;
    Polygon.push_back(std::make_pair(hEdges.begin()->first,0)); //We can start with any point.
    hEdges.erase(hEdges.begin());

    while(true)
    {
      if(Polygon.back().second == 0)
      {
        std::map<TPoint,TPoint,TCompX>::iterator NextVertex = vEdges.find(Polygon.back().first);
        Polygon.push_back(std::make_pair(NextVertex->second, 1));
        vEdges.erase(NextVertex);
      }
      else
      {
        std::map<TPoint,TPoint,TCompY>::iterator NextVertex = hEdges.find(Polygon.back().first);
        Polygon.push_back(std::make_pair(NextVertex->second, 0));
        hEdges.erase(NextVertex);
      }

      if(Polygon.front() == Polygon.back())
      {
        //Closed polygon
        Polygon.erase(Polygon.end()-1);
        break;
      }
    }
    //Remove implementation-markers from the polygon.
    PolygonCount.push_back(Polygon.size());
    for(unsigned I = 0; I < Polygon.size(); I++)
    {
      PolygonPoints.push_back(TPoint(Polygon[I].first.x / 2, Polygon[I].first.y));
      hEdges.erase(Polygon[I].first);
      vEdges.erase(Polygon[I].first);
    }
  }
}
//---------------------------------------------------------------------------
void TDrawThread::CreateInequality(TRelation &Relation)
{
  bool xLogScl = Axes.xAxis.LogScl;
  bool yLogScl = Axes.yAxis.LogScl;
  int dX = (Draw->Width > 1200) ? (Draw->Width > 2400 ? Draw->Width / 120 : 2 ) : 1;
  std::exp(1.0); //Workaround for stupid bug in bcc 5.6.4
  double dx = xLogScl ? std::exp(dX/Draw->xScale) : dX/Draw->xScale;
  double dx2 = xLogScl ? std::exp(1/Draw->xScale) : 1/Draw->xScale;
  double dy = yLogScl ? std::exp(-1/Draw->yScale) : -1/Draw->yScale;
  std::exp(1.0); //Workaround for stupid bug in bcc 5.6.4

  std::vector<TRect> Points;
  Points.reserve(500);
  int XStart;
  std::vector<long double> Args(2);
  Func32::ECalcError CalcError;

  double y = Axes.yAxis.Max;
  for(int Y = AxesRect.Top; Y < AxesRect.Bottom + 1; Y++)
  {
    Args[1] = y;
    bool LastResult = false;
    double x;
    x = xLogScl ? Axes.xAxis.Min / std::sqrt(dx2) : Axes.xAxis.Min - dx2/2;
    for(int X = AxesRect.Left - 1; X < AxesRect.Right + dX; X += dX)
    {
      Args[0] = x;
      long double Temp = Relation.Eval(Args, CalcError);
      bool Result = !_isnanl(Temp) && Temp != 0;
      if(Result != LastResult)
      {
        double x2 = xLogScl ? x / dx * dx2 : x - dx + dx2;
        for(int X2 = X - dX + 1; X2 <= X; X2++)
        {
          Args[0] = x2;
          Temp = Relation.Eval(Args, CalcError);
          Result = !_isnanl(Temp) && Temp != 0;
          if(Result != LastResult)
          {
            if(Result)
              XStart = X2;
            else
              Points.push_back(TRect(
                XStart <= AxesRect.Left ? -100 : XStart*2,
                Y <= AxesRect.Top ? -100 : Y,
                X2*2 - 1,
                Y >= AxesRect.Bottom ? AxesRect.Bottom + 100 : Y + 1));
            break;
          }
          xLogScl ? x2 *= dx2 : x2 += dx2; //Don't place inside for() because of bug in CB2009
        }
      }
      LastResult = Result;
      xLogScl ? x *= dx : x += dx;
    }

    if(Aborted)
      return;
    if(LastResult)
      Points.push_back(TRect(
        XStart <= AxesRect.Left ? -100 : XStart*2,
        Y <= AxesRect.Top ? -100 : Y,
        AxesRect.Right * 2+ 100,
        Y >= AxesRect.Bottom ? AxesRect.Bottom + 100 : Y + 1));
    yLogScl ? y *= dy : y += dy;
  }

  std::vector<TPoint> PolygonPoints;
  std::vector<int> PolygonCounts;
  RegionToPolygons(Points, PolygonPoints, PolygonCounts);
  Relation.SetPoints(PolygonPoints, PolygonCounts);
}
//---------------------------------------------------------------------------
//Check if there is a possibility for a zero point in f(x,y).
//It may not be certain
bool TDrawThread::CheckResult1(const long double Result[3])
{
  //If the sign has changed, at least one of the current and last result is valid, or
  //the increase/decrease state has changed, which might indicate an asymptote.
  return (Result[1] < 0) == (Result[2] > 0) && !(_isnanl(Result[1]) && _isnanl(Result[2])) ||
    (Result[0] < Result[1]) != (Result[1] < Result[2]);
  if((Result[0] < Result[1]) != (Result[1] < Result[2]))
    return true;
  return false;
}
//---------------------------------------------------------------------------
//Check if there actually is a zero point in f(x,y) for certain.
bool TDrawThread::CheckResult2(const long double Result[3])
{
  //If the sign has changed, both last and current result is valid, and both last and current
  //value is increasing/decreasing (to avoid false vertical line at y=1/x)
  return (Result[1] < 0) == (Result[2] > 0) && !_isnanl(Result[1]) && !_isnanl(Result[2]) &&
    (Result[0] < Result[1]) == (Result[1] < Result[2]);
}
//---------------------------------------------------------------------------
void TDrawThread::EquationLoop(TRelation &Relation, std::vector<TRect> &Points, bool Loop)
{
  std::vector<long double> Args(2);
  Func32::ECalcError CalcError;
  double ds1 = Loop ? 1/Draw->xScale : -1/Draw->yScale;
  double ds2 = Loop ? -1/Draw->yScale : 1/Draw->xScale;
  bool LogScl1 = Loop ? Axes.xAxis.LogScl : Axes.yAxis.LogScl;
  bool LogScl2 = Loop ? Axes.yAxis.LogScl : Axes.xAxis.LogScl;

  int M1 = SizeScale(Relation.GetSize()) / 2;
  int M2 = std::max((SizeScale(Relation.GetSize()) + 1) / 2, 1);

  double s1Min = Loop ? Axes.xAxis.Min : Axes.yAxis.Max;
  double s2Min = Loop ? Axes.yAxis.Max : Axes.xAxis.Min;

  int S1Min = Loop ? AxesRect.Left - 1 : AxesRect.Top - 1;
  int S2Min = Loop ? AxesRect.Top : AxesRect.Left;

  int S1Max = Loop ? AxesRect.Right + 1 : AxesRect.Bottom + 1;
  int S2Max = Loop ? AxesRect.Bottom + 1 : AxesRect.Right + 1;

  int dS1 = Draw->Width > 1200 ? Draw->Width / 120 : 5;
  int dS2 = M2;

  LogScl1 ? s1Min /= std::exp(ds1/2) : s1Min -= ds1/2;

  double ds3 = LogScl1 ? std::exp(ds1) : ds1;
  ds1 *= dS1;
  ds2 *= dS2;
  if(LogScl1)
    ds1 = std::exp(ds1);
  if(LogScl2)
    ds2 = std::exp(ds2);

  double s2 = s2Min;
  for(int S2 = S2Min; S2 < S2Max && !Aborted; S2 += dS2)
  {
    Args[Loop] = s2;
    long double Result[3] = {NAN, NAN}; //Don't use plain double as it might cause overflow with large numbers
    double s1 = s1Min;
    for(int S1 = S1Min; S1 < S1Max; S1 += dS1)
    {
      Args[!Loop] = s1;
      Result[2] = Relation.Eval(Args, CalcError);
      if(CheckResult1(Result))
      {
        double s3 = LogScl1 ? s1 / ds1 * ds3 : s1 - ds1 + ds3;
        for(int S3 = S1 - dS1 + 1; S3 <= S1; S3++)
        {
          Args[!Loop] = s3;
          Result[2] = Relation.Eval(Args, CalcError);
          if(CheckResult2(Result))
            if(Loop)
              Points.push_back(TRect(S3 - M1, S2 - M1, S3 + M2, S2 + M2));
            else
              Points.push_back(TRect(S2 - M1, S3 - M1, S2 + M2, S3 + M2));
          Result[0] = Result[1];
          Result[1] = Result[2];
          if(LogScl1)
            s3 *= ds3;
          else
            s3 += ds3;
        }
      }
      else
        Result[0] = Result[1], Result[1] = Result[2];

      if(LogScl1)
        s1 *= ds1;
      else
        s1 += ds1;
    }
    if(LogScl2)
      s2 *= ds2;  //Don't integrate in for loop. It will trigger a bug in CB2009
    else
      s2 += ds2;
  }
}
//---------------------------------------------------------------------------
void TDrawThread::CreateEquation(TRelation &Relation)
{
  std::vector<TRect> Points;
  Points.reserve(500);

  EquationLoop(Relation, Points, 1);
  EquationLoop(Relation, Points, 0);

  if(!Aborted)
    Relation.Region.reset(new TRegion(Points));
}
//---------------------------------------------------------------------------
void TDrawThread::Visit(TRelation &Relation)
{
  if(Relation.GetRelationType() == rtEquation)
  {
    if(!Relation.Region)
      CreateEquation(Relation);
  }
  else
    if(Relation.GetPolygonPoints().empty())
      CreateInequality(Relation);
}
//---------------------------------------------------------------------------
void TDrawThread::SynchronizedShowStatusError(const String &Str)
{
  Form1->ShowStatusError(Str);
}
//---------------------------------------------------------------------------
void TDrawThread::ShowStatusError(const String &Str)
{
  //Synchronizing directly to Form1->ShowStatusError will sometimes
  //cause an ICE on C++ Builder XE
  Synchronize(&SynchronizedShowStatusError, Str);
}
//---------------------------------------------------------------------------
} //namespace Graph

