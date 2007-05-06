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
#include "DrawThread.h"
#include <complex>
#include <cfloat>
#include <values.h>
#include <algorithm>
#include <ctime>
#include <float.h>
#include "PointSelect.h"
#include "IGraphics.h"
#include "StackTrace.h"

const unsigned MaxPixelDist = 10;
const unsigned MaxExtraPoints = 100;
//---------------------------------------------------------------------------
using std::real;
inline long double real(long double t) {return t;}
//---------------------------------------------------------------------------
using std::imag;
inline long double imag(long double) {return 0;}
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
 : TIThread(false),
   Data(ADraw->Data), AxesRect(ADraw->AxesRect), Aborted(false),
   ForceBlack(ADraw->ForceBlack), SizeMul(ADraw->SizeMul),
   Draw(ADraw), Context(ADraw->Context), Axes(ADraw->Axes)
{
}
//---------------------------------------------------------------------------
void __fastcall TDrawThread::Execute()
{
  randomize();

  while(!Terminated)
  {
    try
    {
      if(Aborted)
        ClearMessageQueue();
      if(!HasMessage())
        IdleEvent.SetEvent();
      Aborted = false;
      TMessage Message;
      GetMessage(Message);
      IdleEvent.ResetEvent();

      //No need to prepare drawings if we are just terminating
      if(Message.Msg == dmTerminate)
        return;

      AxesRect = Draw->AxesRect;
      SizeMul = Draw->SizeMul;

      Synchronize(&BeginUpdate);

      switch(Message.Msg)
      {
        case dmDrawAll:
          DrawAll();
          break;

        case dmDrawFunc:
          reinterpret_cast<TGraphElem*>(Message.WParam)->Accept(*this);
          break;
      }
    }
    catch(Exception &E)
    {
      LogUncaughtException(this, &E);
      Synchronize(&Form1->ShowStatusError, AnsiString("Internal error. Log file Graph.err created."));
    }
    catch(std::exception &E)
    {
      LogUncaughtCppException(E.what(), "DrawThread");
      Synchronize(&Form1->ShowStatusError, AnsiString("Internal error. Log file Graph.err created."));
    }
    catch(...)
    {
      LogUncaughtCppException(NULL, "DrawThread");
      Synchronize(&Form1->ShowStatusError, AnsiString("Internal error. Log file Graph.err created."));
    }
    Synchronize(&EndUpdate);
  }
}
//---------------------------------------------------------------------------
void TDrawThread::DrawAll()
{
  for(unsigned I = 0; I < Data->ElemCount() && !Aborted; I++)
  {
    const boost::shared_ptr<TGraphElem> &Elem = Data->GetElem(I);
    for(unsigned N = 0; N < Elem->ChildList.size() && !Aborted; N++)
      if(Elem->ChildList[N]->GetVisible())
        Elem->ChildList[N]->Accept(*this);

    if(Elem->GetVisible() && !Aborted)
      Elem->Accept(*this);
  }
}
//---------------------------------------------------------------------------
void TDrawThread::PrepareFunction(TBaseFuncType *F)
{
  if(F->Points.empty())
  {
    //Use axes range for standard functions if the axes range is smaller than the function range
    //sMin = first; sMax = second
    std::pair<double,double> MinMax = F->GetCurrentRange();
    if(MinMax.first > MinMax.second)
      return; //No range

    int Steps = F->GetSteps().Value;
    bool LogScl = Axes.xAxis.LogScl && !Steps;
    double ds;
    if(Steps <= 0)
      ds = LogScl ? std::exp(std::log(Axes.xAxis.Max / Axes.xAxis.Min) / AxesRect.Width()) : 1/Draw->xScale;
    else if(Steps == 1)
      MinMax.second = MinMax.first, ds = 1;
    else
      ds = (MinMax.second - MinMax.first) / (Steps - 1); //Remove 1 so two steps only plots at Min and Max

    if(ds  <= 0)
      ds = 1; //ds must always be a positive number

    if(Data->Axes.CalcComplex)
      CalcFunc<std::complex<long double> >(*F, MinMax.first, MinMax.second, ds, LogScl);
    else
      CalcFunc<long double>(*F, MinMax.first, MinMax.second, ds, LogScl);
  }
}
//---------------------------------------------------------------------------
void TDrawThread::Visit(TBaseFuncType &Func)
{
  PrepareFunction(&Func);

  if(!Aborted)
    Synchronize(&DrawFunction, Func);
}
//---------------------------------------------------------------------------
void TDrawThread::DrawFunction(const TBaseFuncType &Func)
{
//  TContextLock ContextLock(Context);
  Draw->SetClippingRegion();

  if(Func.DrawType == dtDots)
    Draw->DrawPolydots(Func.Points.begin(), Func.Points.end(), Func.Size, Func.Color);
  else
  {
    std::vector<TPoint>::const_iterator iter = Func.Points.begin();
    for(unsigned I = 0; I < Func.PointNum.size(); I++)
    {
      Draw->DrawPolyline(iter, iter + Func.PointNum[I], Func.Style, Func.Size, Func.Color);
      iter += Func.PointNum[I];
    }
  }
  
  DrawEndPoints(Func);
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

            if(!Err)
            {
              F.Points.push_back(Pos);
              F.sList.push_back(Func32::TCoordSet(s2, real(Coord.x), real(Coord.y)));
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
            //We use virtual and horizontal dist to avoid overflow in square calculation
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
                F.sList.push_back(Func32::TCoordSet(sLast, real(Coord.x), real(Coord.y)));
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
             F.sList.push_back(Func32::TCoordSet(s - ds, real(Coord.x), real(Coord.y)));
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

            if(!Err)
            {
              F.Points.push_back(Pos2);
              F.sList.push_back(Func32::TCoordSet(s2, real(Coord2.x), real(Coord2.y)));
            }
          }

          //Add point to vector
          F.Points.push_back(Pos);
          F.sList.push_back(Func32::TCoordSet(s, real(Coord.x), real(Coord.y)));
          Err = false;            //Indicate no error
        }
        else //No errors
        {
          //Add point to vector
          if(Pos != LastPos)
          {
            F.Points.push_back(Pos);
            F.sList.push_back(Func32::TCoordSet(s, real(Coord.x), real(Coord.y)));
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
    Synchronize(&Form1->ShowStatusError, "Internal error. Unexpected exception: " + E.Message);
  }
  catch(std::exception &E)
  {
    Synchronize(&Form1->ShowStatusError, "Internal error. Unexpected exception: " + AnsiString(E.what()));
  }
  catch(...)
  {
    Synchronize(&Form1->ShowStatusError, AnsiString("Internal error. Unknown exception"));
  }
}
//---------------------------------------------------------------------------
HPEN TDrawThread::SetPen(TColor Color, TPenStyle Style, int Width)
{
  Width = Size(Width);
  if(IsWinNT && Width > 1 && Style != psSolid)
  {
    LOGBRUSH LogBrush;
    LogBrush.lbStyle = BS_SOLID;
    LogBrush.lbColor = ForceBlack ? clBlack : Color;
    return ExtCreatePen(PS_GEOMETRIC | Style, Width, &LogBrush, 0, NULL);
  }
  return CreatePen(Style, Width, ForceBlack ? clBlack : Color);
}
//---------------------------------------------------------------------------
struct TDrawShadeData
{
  const TShade &Shade;
  std::vector<TPoint> &Points;
  unsigned Pos;
  TPoint P1, P2;
  TDrawShadeData(const TShade &AShade, std::vector<TPoint> &APoints, unsigned APos, const TPoint &AP1, const TPoint &AP2)
    : Shade(AShade), Points(APoints), Pos(APos), P1(AP1), P2(AP2) {}
};
void TDrawThread::Visit(TShade &Shade)
{
  TBaseFuncType *F = Shade.ParentFunc().get();

  PrepareFunction(F);
  if(Shade.Func2)
    PrepareFunction(Shade.Func2.get());

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
    Min = Interval(sMin, Draw->xCoord(-1), Draw->xCoord(AxesRect.Right+1));
    Max = Interval(sMax, Draw->xCoord(-1), Draw->xCoord(AxesRect.Right+1));
  }
  else
  {
    Min = std::_finite(sMin) ? sMin : F->From.Value;
    Max = std::_finite(sMax) ? sMax : F->To.Value;
  }

  std::vector<TPoint> Points;

  //Find starting and ending points in point vector
  unsigned N1 = std::lower_bound(F->sList.begin(), F->sList.end() - 1, Min, CompCoordSet1) - F->sList.begin();
  unsigned N2 = std::upper_bound(F->sList.begin() + N1, F->sList.end(), Max, CompCoordSet2) - F->sList.begin() - 1;

  if(N2 < N1)
    N2 = N1; //This may happen because we subtracted 1 when we calculated N2

  //Position of x-axis on the y-axis
  int yAxisPixel = Interval<int>(yPoint(Axes.xAxis.AxisCross), AxesRect.Top, AxesRect.Bottom);

  //Position of y-axis on the x-axis
  int xAxisPixel = Interval<int>(xPoint(Axes.yAxis.AxisCross), AxesRect.Left, AxesRect.Right);

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

  switch(Shade.ShadeStyle)
  {
    case ssAbove:
      //Set start point on top of image
      Points.push_back(TPoint(x1, -1));
      break;
    case ssBelow:
      //Set start point on bottom of image
      Points.push_back(TPoint(x1, AxesRect.Bottom + 1));
      break;
    case ssXAxis:
      //Set start point on x-axis
      Points.push_back(TPoint(x1, yAxisPixel));
      break;
    case ssYAxis:
      //Set start point on y-axis
      Points.push_back(TPoint(xAxisPixel, y1));
      break;
    case ssBetween:
    case ssInside:
      break;
  }

  //Copy points used for drawing marked area
  Points.push_back(TPoint(x1, y1));

  Points.insert(Points.end(), F->Points.begin()+N1, F->Points.begin()+N2+1);

  Points.push_back(TPoint(x2, y2));

  switch(Shade.ShadeStyle)
  {
    case ssAbove:
      //Set end point on top of image
      Points.push_back(TPoint(x2,  -1));
      break;
    case ssBelow:
      //Set end point on bottom of image
      Points.push_back(TPoint(x2, AxesRect.Bottom + 1));
      break;
    case ssXAxis:
      //Set end point on x-axis
      Points.push_back(TPoint(x2, yAxisPixel));
      break;
    case ssYAxis:
      //Set end point on y-axis
      Points.push_back(TPoint(xAxisPixel, y2));
      break;
    case ssBetween:
    {
      //The intervals can go both ways. Make sure sMin is always less than sMax
      double sMin2 = std::min(Shade.sMin2.Value, Shade.sMax2.Value);
      double sMax2 = std::max(Shade.sMin2.Value, Shade.sMax2.Value);

      double Min2, Max2;

      if(dynamic_cast<const TStdFunc*>(Shade.Func2.get()))
      {
        Min2 = Interval(sMin2, Draw->xCoord(-1), Draw->xCoord(AxesRect.Right+1));
        Max2 = Interval(sMax2, Draw->xCoord(-1), Draw->xCoord(AxesRect.Right+1));
      }
      else
      {
        Min2 = std::_finite(sMin2) ? sMin2 : Shade.Func2->From.Value;
        Max2 = std::_finite(sMax2) ? sMax2 : Shade.Func2->To.Value;
      }

      //Find starting and ending points in point vector
      //If list is empty: M1=-1, M2=-1
      int M1 = std::lower_bound(Shade.Func2->sList.begin(), Shade.Func2->sList.end() - 1, Min2, CompCoordSet1) - Shade.Func2->sList.begin();
      int M2 = std::upper_bound(Shade.Func2->sList.begin()+M1, Shade.Func2->sList.end(), Max2, CompCoordSet2) - Shade.Func2->sList.begin() - 1;

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
      break;
    }
    case ssInside:
      break;
  }

//  TContextLock ContextLock(Context);
  if(!Aborted)
    Synchronize(&DrawShade, TDrawShadeData(Shade, Points, N2 - N1 + 2, TPoint(x1, y1), TPoint(x2, y2)));
}
//---------------------------------------------------------------------------
void TDrawThread::DrawShade(const TDrawShadeData &ShadeData)
{
  const TShade &Shade = ShadeData.Shade;
  std::vector<TPoint> &Points = ShadeData.Points;
  unsigned Pos = ShadeData.Pos;

  Draw->SetClippingRegion();

  //Set pen and brush; No pen is used, to prevent line around shade
  Context.SetPen(psClear, clWhite, 1);
  Context.SetBrush(Shade.BrushStyle, ForceBlack ? clBlack : Shade.Color);

  //Draw marked area
  Context.DrawPolygon(Points);

  //Draw start and end of shading
  Context.SetPen(psSolid, ForceBlack ? clBlack : Shade.Color, 1);

  if(Shade.ShadeStyle == ssBetween)
  {
    if(MaxDist(Points.front(), Points[Pos]) > 1 || MaxDist(Points.back(), Points[Pos + 1]) > 1)
    {
      if((Points.front().x > 0 || Points.back().x > 0) && Shade.MarkStart)
        Context.DrawLine(Points.front(), Points.back());

      if(Shade.MarkEnd)
        Context.DrawLine(Points[Pos], Points[Pos + 1]);
    }
    else if(Shade.MarkStart || Shade.MarkEnd)
    {
      //If the points are only 1 pixel in distance, move them together
      //This takes care of some smalle rounding problems
      Points.front() = Points[Pos];
      Points.back() = Points[Pos+1];
    }
  }
  else if(Shade.ShadeStyle == ssInside)
  {
    if(Shade.MarkStart || Shade.MarkEnd)
      Context.DrawLine(Points.front(), Points.back());
  }
  else
  {
    if(Shade.MarkStart)
      Context.DrawLine(Points[0], ShadeData.P1);
    if(Shade.MarkEnd)
      Context.DrawLine(ShadeData.P2, Points.back());
  }
}
//---------------------------------------------------------------------------
void __fastcall TDrawThread::BeginUpdate()
{
  Form1->BeginUpdate();
  Form1->ShowStatusMessage(LoadRes(RES_UPDATE));
}
//---------------------------------------------------------------------------
//Must be synchronized
void __fastcall TDrawThread::EndUpdate()
{
  try
  {
    Form1->EndUpdate();
    Form1->ShowStatusMessage("");
    Form1->UpdateEval();
    if(Draw->OnComplete)
      Draw->OnComplete();
  }
  catch(Exception &E)
  {
    Form1->ShowStatusError("Internal error. Unexpected exception: " + E.Message);
  }
}
//---------------------------------------------------------------------------
void TDrawThread::DrawEndPoints(const TBaseFuncType &Func)
{
  if(Func.StartPointStyle != 0)
  {
    long double t = Func.From.Value;
    if(_finitel(t))
      DrawEndPoint(Func, t, Func.StartPointStyle, true);
    else
    {
      //If a start value has not been specified, search for the first point inside the visible area
      //that is close to the border, and show the end point here.
      for(unsigned I = 0; I < Func.sList.size(); I++)
        if(InsideRect(AxesRect, Func.Points[I]))
        {
          if(!IsInRange(Func.Points[I].x, AxesRect.Left + 5, AxesRect.Right - 5) ||
             !IsInRange(Func.Points[I].y, AxesRect.Top + 5, AxesRect.Bottom - 5))
          {
            t = Func.sList[I].t;
            TPoint Pos = Func.Points[I];
            if(I > 0)
              //This only works because we know the function is a standard function
              Pos = TContext::ClipLine(Func.Points[I-1], Func.Points[I], AxesRect);
            DrawEndPoint(Func, t, Pos, Func.StartPointStyle, true);
          }
          break;
        }
    }
  }

  if(Func.EndPointStyle != 0)
  {
    long double t = Func.To.Value;
    if(_finitel(t))
      DrawEndPoint(Func, t, Func.EndPointStyle, false);
    else
    {
      //If a start value has not been specified, search for the first point inside the visible area
      //that is close to the border, and show the end point here.
      for(unsigned I = Func.sList.size()-1; I > 0; I--)
        if(InsideRect(AxesRect, Func.Points[I]))
        {
          if(!IsInRange(Func.Points[I].x, AxesRect.Left + 5, AxesRect.Right - 5) ||
             !IsInRange(Func.Points[I].y, AxesRect.Top + 5, AxesRect.Bottom - 5))
          {
            t = Func.sList[I].t;
            TPoint Pos = Func.Points[I];
            if(I < Func.sList.size() - 1)
              //This only works because we know the function is a standard function
              Pos = TContext::ClipLine(Func.Points[I+1], Func.Points[I], AxesRect);
            DrawEndPoint(Func, t, Pos, Func.EndPointStyle, false);
          }
          break;
        }
    }
  }
}
//---------------------------------------------------------------------------
void TDrawThread::DrawEndPoint(const TBaseFuncType &Func, long double t, const TPoint &Pos, unsigned Style, bool InvertArrow)
{
  if(_finitel(t))
    try
    {
      Func32::TCoord<long double> Coord = Func.GetFunc().Calc(t);
      if(IsError(Func32::ecNoError, Coord))
        return; //Invalid coordinate because of log axes

      switch(Style)
      {
        case 1:
          TPointSelect::DrawPoint(Context.GetCanvas(), Pos, 0, Func.Color, Axes.BackgroundColor, Size(Func.Size + 3));
          break;

        case 2:
          TPointSelect::DrawPoint(Context.GetCanvas(), Pos, 0, Func.Color, Func.Color, Size(Func.Size + 3));
          break;

        case 3:
          if(InvertArrow)
            DrawArrow(Pos, Func.GetFunc().CalcAngleSlope(t) + M_PI, Func.Color, Func.Size);
          else
            DrawArrow(Pos, Func.GetFunc().CalcAngleSlope(t), Func.Color, Func.Size);
          break;

        case 4:
          DrawHalfCircle(Pos, Func.GetFunc().CalcAngleSlope(t), Func.Color, Func.Size);
          break;

        case 5:
          DrawHalfCircle(Pos, Func.GetFunc().CalcAngleSlope(t) + M_PI, Func.Color, Func.Size);
          break;
      }
    }
    catch(Func32::EFuncError&)
    {
    }
}
//---------------------------------------------------------------------------
void TDrawThread::DrawEndPoint(const TBaseFuncType &Func, long double t, unsigned Style, bool InvertArrow)
{
  if(_finitel(t))
    try
    {
      Func32::TCoord<long double> Coord = Func.GetFunc().Calc(t);
      if(IsError(Func32::ecNoError, Coord))
        return; //Invalid coordinate because of log axes

      DrawEndPoint(Func, t, Draw->xyPoint(Coord), Style, InvertArrow);
    }
    catch(Func32::EFuncError&)
    {
    }
}
//---------------------------------------------------------------------------
void TDrawThread::Visit(TTextLabel &Label)
{
  Synchronize(&DrawLabel, Label);
}
//---------------------------------------------------------------------------
void TDrawThread::DrawLabel(TTextLabel &Label)
{
  if(_isnan(Label.GetXPos().Value) || _isnan(Label.GetYPos().Value))
    return;

  TPoint Pos;
  switch(Label.GetPlacement())
  {
    case lpAboveX:
      Pos = TPoint(AxesRect.Right - Label.GetRect().Width() + 2, yPoint(Axes.xAxis.AxisCross) - Label.GetRect().Height() - 4);
      break;

    case lpBelowX:
      Pos = TPoint(AxesRect.Right - Label.GetRect().Width() + 2, yPoint(Axes.xAxis.AxisCross));
      break;

    case lpRightOfY:
      Pos = TPoint(xPoint(Axes.yAxis.AxisCross) + 12, AxesRect.Top + 1);
      break;

    case lpLeftOfY:
      Pos = TPoint(xPoint(Axes.yAxis.AxisCross) - Label.GetRect().Width(), AxesRect.Top + 1);
      break;

    case lpUserTopLeft:
    default:
      Pos = Draw->xyPoint(Label.GetXPos().Value, Label.GetYPos().Value);
      break;

    case lpUserTopRight:
      Pos = Draw->xyPoint(Label.GetXPos().Value, Label.GetYPos().Value);
      Pos.x -= Label.GetRect().Width();
      break;

    case lpUserBottomLeft:
      Pos = Draw->xyPoint(Label.GetXPos().Value, Label.GetYPos().Value);
      Pos.y -= Label.GetRect().Height();
      break;

    case lpUserBottomRight:
      Pos = Draw->xyPoint(Label.GetXPos().Value, Label.GetYPos().Value);
      Pos.x -= Label.GetRect().Width();
      Pos.y -= Label.GetRect().Height();
      break;
  }

  Label.UpdateRect(Pos.x, Pos.y);

  //Check for valid coordinate; The label may have a negative position on a log axis
  if(Pos.x != MAXINT && Pos.y != MAXINT)
  {
    Draw->SetClippingRegion();
    Context.GetCanvas()->Draw(Pos.x, Pos.y, Label.GetImage());
  }
}
//---------------------------------------------------------------------------
void TDrawThread::Visit(TPointSeries &PointSeries)
{
  Synchronize(&DrawPointSeries, PointSeries);
}
//---------------------------------------------------------------------------
void TDrawThread::DrawPointSeries(const TPointSeries &PointSeries)
{
//  TContextLock ContextLock(Context); Doesn't work; Synchronize() instead
  Draw->SetClippingRegion();

  std::vector<TPoint> PointArray;
  std::vector<TPointSeriesPoint>::const_iterator end = PointSeries.PointList.end();
  for(std::vector<TPointSeriesPoint>::const_iterator iter = PointSeries.PointList.begin(); iter != end; ++iter)
    //Check that point is valid (NAN is used to indicate invalid value)
    if((!Axes.xAxis.LogScl || iter->x.Value > 0) && (!Axes.yAxis.LogScl || iter->y.Value > 0) && _finite(iter->x.Value) && _finite(iter->y.Value))
      PointArray.push_back(TPoint(xPoint(iter->x.Value), yPoint(iter->y.Value)));

  if(PointSeries.LineStyle != psClear && PointSeries.LineSize > 0)
  {
    std::vector<TPoint> SplinePoints;
    Interpolate(SplinePoints, PointArray, PointSeries.Interpolation);
    Draw->DrawPolyline(SplinePoints.begin(), SplinePoints.end(), PointSeries.LineStyle, PointSeries.LineSize, PointSeries.LineColor);
  }

  if(PointSeries.ShowLabels)
  {
    Context.SetFont(PointSeries.Font);
    if(ForceBlack)
      Context.SetFontColor(clBlack);
  }

  //Only draw for point sizes >0
  if(PointSeries.Size > 0)
    for(std::vector<TPointSeriesPoint>::const_iterator iter = PointSeries.PointList.begin(); iter != end; ++iter)
    {
      //Check that point is valid (NAN is used to indicate invalid value)
      if((!Axes.xAxis.LogScl || iter->x.Value > 0) && (!Axes.yAxis.LogScl || iter->y.Value > 0) && _finite(iter->x.Value) && _finite(iter->y.Value))
      {
        unsigned I = iter - PointSeries.PointList.begin();
        TPoint Pos(Draw->xPoint(iter->x.Value), Draw->yPoint(iter->y.Value));
        if(!InsideRect(AxesRect, Pos)) //We might get rounding errors if the point is too much outside the visible area
          continue;

        //Draw x error bar
        Context.SetPen(psSolid, clBlack, Size(1));
        double Delta = PointSeries.GetXError(I);
        if(Delta)
        {
          int X1 = xPoint(iter->x.Value - Delta);
          int X2 = xPoint(iter->x.Value + Delta);
          Context.DrawLine(X1, Pos.y, X2, Pos.y);
          Context.DrawLine(X1, Pos.y - 4, X1, Pos.y +5);
          Context.DrawLine(X2, Pos.y - 4, X2, Pos.y +5);
        }

        //Draw y error bar
        Delta = PointSeries.GetYError(I);
        if(Delta)
        {
          int Y1 = yPoint(iter->y.Value - Delta);
          int Y2 = yPoint(iter->y.Value + Delta);
          Context.DrawLine(Pos.x, Y1, Pos.x, Y2);
          Context.DrawLine(Pos.x - 4, Y1, Pos.x +5, Y1);
          Context.DrawLine(Pos.x - 4, Y2, Pos.x +5, Y2);
        }

        int PointSize = Size(PointSeries.Size);
        TColor FillColor = ForceBlack ? clWhite : PointSeries.FillColor;
        TColor FrameColor = ForceBlack ? clBlack : (PointSeries.Size > 2 ? PointSeries.FrameColor : FillColor);

        if(PointSeries.Style != 7)
          TPointSelect::DrawPoint(Context.GetCanvas(), Pos, PointSeries.Style, FrameColor, FillColor, PointSize); //Draw normalt markers
        else if(I != 0) //Draw arrow
        {
          Context.SetPen(PointSeries.LineStyle == psClear ? psSolid : PointSeries.LineStyle, PointSeries.FillColor, PointSize);
          double Angle;
          Angle = InterpolationAngle(PointSeries.GetPoint(std::max((int)I-2, 0)), PointSeries.GetPoint(I-1),
            PointSeries.GetPoint(I), PointSeries.GetPoint(std::min(I+1, PointSeries.PointList.size()-1)), PointSeries.Interpolation);

          DrawArrow(Pos, Angle, PointSeries.FillColor, PointSeries.Size);
        }

        if(PointSeries.ShowLabels)
        {
          std::string Str = "(" + iter->x.Text + "," + iter->y.Text + ")";
          TDraw::DrawPointLabel(Context.GetCanvas(), Pos, PointSize, Str, PointSeries.LabelPosition);
        }
      }
    }
}
//---------------------------------------------------------------------------
void TDrawThread::DrawArrow(const TPoint &Point, long double Angle, TColor Color, unsigned Size)
{
  Context.SetPen(psSolid, Color, 1);
  Context.SetBrush(bsSolid, Color);

  //Adjust angle for visual coordinate system instead of real system
  long double Angle2 = std::atan(Draw->yScale/Draw->xScale * std::tan(Angle));
  if(std::abs(std::fmod(Angle, 2.0L*M_PI)) > M_PI_2)
    Angle2 -= M_PI;

  int Length = 13 + 2*Size;
  int dX1 = Length * std::cos(Angle2 + 2.8);
  int dY1 = Length * std::sin(Angle2 + 2.8);
  int dX2 = Length * std::cos(Angle2 - 2.8);
  int dY2 = Length * std::sin(Angle2 - 2.8);
  TPoint Arrow[] = {TPoint(Point.x + dX1, Point.y - dY1), Point, TPoint(Point.x + dX2, Point.y - dY2)};
  Context.DrawPolygon(Arrow, 3);
//  LineToAngle(Point.x, Point.y, Angle2 + 2.6, 15);
//  LineToAngle(Point.x, Point.y, Angle2 - 2.6, 15);
}
//---------------------------------------------------------------------------
void TDrawThread::LineToAngle(int X, int Y, double Angle, double Length)
{
  int dX = Length * std::cos(Angle);
  int dY = Length * std::sin(Angle);

  Context.DrawLine(X, Y, X + dX, Y - dY);
}
//---------------------------------------------------------------------------
void TDrawThread::DrawHalfCircle(const TPoint &Point, long double Angle, TColor Color, unsigned Size)
{
  Context.SetPen(psSolid, Color, Size);

  //Adjust angle for visual coordinate system instead of real system
  long double Angle2 = std::atan(Draw->yScale/Draw->xScale * std::tan(Angle));
  if(std::abs(std::fmod(Angle, 2.0L*M_PI)) > M_PI_2)
    Angle2 -= M_PI;

  int r = 8;
  int a = r * std::sin(Angle2);
  int b = r * std::cos(Angle2);
  TPoint C(Point.x - b, Point.y + a);

  Context.DrawArc(C.x - r, C.y - r, C.x + r, C.y + r, C.x + a, C.y + b, C.x - a, C.y - b);
}
//---------------------------------------------------------------------------
void TDrawThread::Visit(TAxesView &AxesView)
{
//  TContextLock ContextLock(Context);
//  Draw->DrawAxes();
  Synchronize(&Draw->DrawAxes);
}
//---------------------------------------------------------------------------
void TDrawThread::CreateInequality(TRelation &Relation)
{
  double dx = 1/Draw->xScale;
  double dy = 1/Draw->yScale;

  double y = Axes.yAxis.Max + dy;
  std::vector<TRect> Points;
  Points.reserve(500);
  int XStart;
  std::vector<long double> Args(2);
  Func32::ECalcError CalcError;

  for(int Y = AxesRect.Top - 1; Y < AxesRect.Bottom + 1; Y++, y -= dy)
  {
    Args[1] = y;
    bool LastResult = false;
    int X = AxesRect.Left - 1;
    for(double x = Axes.xAxis.Min - dx; X < AxesRect.Right + 1; X++, x += dx)
    {
      Args[0] = x;
      long double Temp = Relation.Eval(Args, CalcError);
      bool Result = !_isnanl(Temp) && Temp != 0;
      if(Result != LastResult)
      {
        if(Result)
          XStart = X;
        else
          Points.push_back(TRect(XStart, Y, X, Y + 1));
      }

      LastResult = Result;
    }

    if(Aborted)
      return;
    if(LastResult)
      Points.push_back(TRect(XStart, Y, AxesRect.Right + 1, Y + 1));
  }

  Relation.Region.reset(new TRegion(Points));

  if(Relation.GetBrushStyle() != bsSolid && Relation.GetSize() > 0)
  {
    //Create bounding region as a region 1 pixel large than the real region, and subtract the real region
    //to get the bounding region
    const std::vector<TRect>::iterator End = Points.end();
    for(std::vector<TRect>::iterator Iter = Points.begin(); Iter != End; ++Iter)
    {
      unsigned Delta = Size(Relation.GetSize());
      Iter->Left -= Delta;
      Iter->Right += Delta;
      Iter->Top -= Delta;
      Iter->Bottom += Delta;
    }

    Relation.BoundingRegion.reset(new TRegion(Points));
    *Relation.BoundingRegion -= *Relation.Region;
  }
  else
    Relation.BoundingRegion.reset(); //Don't draw frame when brush style is bsSolid
}
//---------------------------------------------------------------------------
void TDrawThread::CreateEquation(TRelation &Relation)
{
  double dx = 1/Draw->xScale;
  double dy = 1/Draw->yScale;
  int S1 = Size(Relation.GetSize()) / 2;
  int S2 = Size(Relation.GetSize() + 1) / 2;

  std::vector<TRect> Points;
  Points.reserve(500);

  std::vector<long double> Args(2);
  Func32::ECalcError CalcError;

  double y = Axes.yAxis.Max + dy;
  for(int Y = AxesRect.Top - 1; Y < AxesRect.Bottom + 1; Y++, y -= dy)
  {
    double Result[3] = {NAN, NAN};
    int X = AxesRect.Left - 1;
    for(double x = Axes.xAxis.Min - dx / 2; X < AxesRect.Right + 1; X++, x += dx)
    {
      Args[0] = x;
      Args[1] = y;
      Result[2] = Relation.Eval(Args, CalcError);

      //If the sign has changed, both last and current result is valid, and both last and current
      //value is increasing/decreasing (to avoid false vertical line at y=1/x)
      if((Result[1] < 0) == (Result[2] > 0) && !_isnan(Result[1]) && !_isnan(Result[2]) &&
        (Result[0] < Result[1]) == (Result[1] < Result[2]))
        Points.push_back(TRect(X - S1, Y - S1, X + S2, Y + S2));

      Result[0] = Result[1];
      Result[1] = Result[2];
    }

    if(Aborted)
      return;
  }

  double x = Axes.xAxis.Min - dx / 2;
  for(int X = AxesRect.Left - 1; X < AxesRect.Right + 1; X++, x += dx)
  {
    double Result[3] = {NAN, NAN};
    int Y = AxesRect.Top - 1;
    for(double y = Axes.yAxis.Max + dy; Y < AxesRect.Bottom + 1; Y++, y -= dy)
    {
      Args[0] = x;
      Args[1] = y;
      Result[2] = Relation.Eval(Args, CalcError);

      //If the sign has changed, both last and current result is valid, and both last and current
      //value is increasing/decreasing (to avoid false vertical line at y=1/x)
      if((Result[1] < 0) != (Result[2] < 0) && !_isnan(Result[1]) && !_isnan(Result[2]) &&
        (Result[0] < Result[1]) == (Result[1] < Result[2]))
        Points.push_back(TRect(X - S1, Y - S1, X + S2, Y + S2));

      Result[0] = Result[1];
      Result[1] = Result[2];
    }

    if(Aborted)
      return;
  }

  if(!IsWinNT && Points.size() > 4000)
  {
    Synchronize(&Form1->ShowStatusError, AnsiString("The relation is too complex to plot under Windows 9x"));
    Points.clear();
  }
  Relation.Region.reset(new TRegion(Points));
}
//---------------------------------------------------------------------------
void TDrawThread::Visit(TRelation &Relation)
{
  if(!Relation.Region)
  {
    if(Relation.GetRelationType() == rtEquation)
      CreateEquation(Relation);
    else
      CreateInequality(Relation);
  }

  if(!Aborted)
    Synchronize(&DrawRelation, Relation);
}
//---------------------------------------------------------------------------
void TDrawThread::DrawRelation(const TRelation &Relation)
{
//  TContextLock ContextLock(Context);
  Draw->SetClippingRegion();

  Context.SetBrush(Relation.GetRelationType() == rtInequality ? Relation.GetBrushStyle() : bsSolid, Relation.GetColor());
  Context.DrawRegion(*Relation.Region);

  //Draw a frame around inequalities using BoundingRegion
  if(Relation.BoundingRegion)
  {
    Context.SetBrush(bsSolid, Relation.GetColor());
    Context.DrawRegion(*Relation.BoundingRegion);
  }
}
//---------------------------------------------------------------------------


