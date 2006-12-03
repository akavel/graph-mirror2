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
#include <float.h>
#include <complex>
#include <cmath>
#include "DrawThread.h"
#include "PointSelect.h"
//Windows 2000/XP uses signed 32 bit numbers (Says Windows SDK). It looks like only only 21 bits are used (signed).
//Windows 9x only uses signed 16 bit numbers for pixel positions
//Wierd lines might be shown under Windows 9x if lines are longer than a specific limit (2^14 I think) (Why do I hate Windows?)
//The following code should show a vertical line, but doesn't under windows 9x
//  Image1->Canvas->Pen->Width = 2;
//  Image1->Canvas->MoveTo(316, 242);
//  Image1->Canvas->LineTo(317, -16181);
//The same code will only show a dot under Windows 2000 if the BCB IDE is running. Just run the app with the IDE closed.
//WARNING: It looks like only 16 bit signed numbers are drawn when program are run from inside BCB
//http://groups.google.com/groups?threadm=e9j%23Z0cGEHA.3012%40TK2MSFTNGP12.phx.gbl

//The DrawPolyline() function requires the points to be below MAXINT/2
//Make some room to add pixels without overflowing
const int PixelLimit = MAXINT/2 - 200;
//---------------------------------------------------------------------------
//No declaration of abs(long) in std::
inline long abs(long x) {return std::abs(static_cast<int>(x));}
//---------------------------------------------------------------------------
struct TDrawLegend : public TGraphElemVisitor
{
  const int TextWidth, TextHeight;
  TDraw *Draw;
  int x,y;
  TDrawLegend(TDraw *ADraw, int ATextWidth, int ATextHeight, int X, int Y)
    : Draw(ADraw), TextWidth(ATextWidth), TextHeight(ATextHeight), x(X), y(Y) {}
  int Size(int I) const {return Draw->Size(I);}

  void Visit(TBaseFuncType &Func);
  void Visit(TTan &Tan) {Visit(static_cast<TBaseFuncType&>(Tan));}
  void Visit(TShade &Shade);
  void Visit(TPointSeries &Series);
  void Visit(TTextLabel &Label) {};
  void Visit(TRelation &Relation);
  void Visit(TAxesView &AxesView) {};
  void Visit(TOleObjectElem &OleObjectElem) {};
};
//---------------------------------------------------------------------------
TDraw::TDraw(TCanvas *Canvas, TData *pData, bool AForceBlack, const std::string &ThreadName) :
  Context(Canvas), Data(pData), AxesRect(0, 0, 0, 0), OnComplete(NULL),
  SizeMul(1), ForceBlack(AForceBlack), Axes(Data->Axes), Thread(NULL), Width(0), Height(0)
{
  //Make sure everything is initialized before creating thread
  Thread = new TDrawThread(this);
  Wait(); //Wait for thread to be running before setting name
  Thread->SetName(ThreadName);
}
//---------------------------------------------------------------------------
TDraw::~TDraw()
{
  if(!AbortUpdate())
    return; //Return if thread is stuck
  Thread->IdleEvent.ResetEvent();
  Thread->PostMessage(dmTerminate);
  Thread->WaitFor();
  delete Thread;
}
//---------------------------------------------------------------------------
//Converts an x-coordinate to a pixel value
//Returns MAXINT on error
int TDraw::xPoint(long double x) const
{
  try
  {
    long double Result;
    if(std::_isnan(x))
      return -1;   //x=NAN may else throw an exception
    if(Axes.xAxis.LogScl)
      if(x <= 0) //Check for negative value
        return MAXINT; //Report error
      else
        Result = std::log(x / Axes.xAxis.Min) * xScale + AxesRect.Left + 0.5;
    else
      Result = (x - Axes.xAxis.Min) * xScale + AxesRect.Left + 0.5;

    if(Result > PixelLimit)
      return PixelLimit;
    if(Result < -PixelLimit)
      return -PixelLimit;
    return Result;
  }
  catch(EOverflow&)
  {
    //Handle very large numbers which causes overflow
    return x > 0 ? PixelLimit : -PixelLimit;
  }
}
//---------------------------------------------------------------------------
//Converts a y-coordinate to a pixel value
//Returns MAXINT on error
int TDraw::yPoint(long double y) const
{
  try
  {
    long double Result;
    if(std::_isnan(y))
      return -1;  //y=NAN may else throw an exception
    if(Axes.yAxis.LogScl)
      if(y <= 0) //Check for negative value
        return MAXINT; //Report error
      else
        Result = std::log(Axes.yAxis.Max / y) * yScale + AxesRect.Top + 0.5;
    else
      Result = (Axes.yAxis.Max - y) * yScale + AxesRect.Top + 0.5;

    if(Result > PixelLimit)
      return PixelLimit;
    if(Result < -PixelLimit)
      return -PixelLimit;
    return Result;
  }
  catch(EOverflow&)
  {
    //Handle very large numbers which causes overflow
    return y > 0 ? -PixelLimit : PixelLimit;
  }
}
//---------------------------------------------------------------------------
double TDraw::xPointExact(long double x) const
{
  if(Axes.xAxis.LogScl)
    return std::log(x / Axes.xAxis.Min) * xScale + AxesRect.Left;
  return (x - Axes.xAxis.Min) * xScale + AxesRect.Left;
}
//---------------------------------------------------------------------------
double TDraw::yPointExact(long double y) const
{
  if(Axes.yAxis.LogScl)
    return std::log(Axes.yAxis.Max / y) * yScale + AxesRect.Top;
  return (Axes.yAxis.Max - y) * yScale + AxesRect.Top;
}
//---------------------------------------------------------------------------
void TDraw::DrawFunc(const TGraphElem *F)
{
  DrawLegend();
  Thread->IdleEvent.ResetEvent();
  Thread->PostMessage(dmDrawFunc, reinterpret_cast<unsigned>(F));
}
//---------------------------------------------------------------------------
//Calculate data used to draw a tangent
//(x,y) is the point there the tangent crosses the function and a is the slope
bool TDraw::CalcTan(TTan *Tan)
{
  try
  {
    const Func32::TBaseFunc &Func = Tan->ParentFunc()->GetFunc();

    double x = Func.CalcX(Tan->t.Value); //Find x(t)
    double y = Func.CalcY(Tan->t.Value); //Find y(t)

    double a = Tan->ParentFunc()->GetFunc().CalcSlope(Tan->t.Value);

    if(Tan->TangentType == ttNormal)
      if(_finite(a))
        a = std::abs(a) < MIN_ZERO ? INF : -1/a;
      else
        a = 0;

    double q = _finite(a) ? y - a*x : x;
    Tan->UpdateTan(a, q);
    return true;
  }
  catch(Func32::EFuncError&)
  {
    Tan->UpdateTan(NAN, NAN);
    return false;
  }
}
//---------------------------------------------------------------------------
void TDraw::DrawAll()
{
  AbortUpdate();
  Context.DestroyClipRect(); //Remove all clipping regions; Must be done if printing more than one page

  for(unsigned I = 0; I < Data->ElemCount(); I++)
    for(unsigned N = 0; N < Data->GetElem(I)->ChildList.size(); N++)
      if(TTan *Tan = dynamic_cast<TTan*>(Data->GetElem(I)->ChildList[N].get()))
        CalcTan(Tan);

  RedrawAxes();

#ifdef LIMITED_EDITION
  Context.SetFont("Times New Roman", 10, clBlack);
  std::string Str = "Graph Limited School Edition";
  Context.DrawText(Str, 0, Height - Context.GetTextHeight(Str));
#endif

  if(Data->ElemCount() != 0)
  {
    Thread->IdleEvent.ResetEvent();
    Thread->PostMessage(dmDrawAll);
  }
}
//---------------------------------------------------------------------------
void TDraw::RedrawAxes()
{
  AxesRect = TRect(0, 0, Width, Height);

  if(!Axes.Title.empty())
  {
    Context.SetFont(Axes.TitleFont);
    Context.SetBrush(bsClear);
    if(ForceBlack)
      Context.SetFontColor(clBlack);
    TSize TextSize = Context.GetTextExtent(Axes.Title);
    Context.DrawText(Axes.Title, (AxesRect.Width() - TextSize.cx) / 2, 0);
    AxesRect.Top = TextSize.cy;
  }

  if(Axes.AxesStyle == asBoxed)
  {
    //Calculate font height for numbers
    Context.SetFont(Axes.NumberFont);
    int NumberHeight = Context.GetTextHeight("1");
    int MinWidth = NumberWidth(Axes.yAxis.Min, Axes.yAxis.MultiplyOfPi);
    int MaxWidth = NumberWidth(Axes.yAxis.Max, Axes.yAxis.MultiplyOfPi);

    AxesRect.Left = std::max(MinWidth, MaxWidth) + Size(7);
    AxesRect.Bottom = Height - NumberHeight - Size(4);

#ifdef LIMITED_EDITION
    AxesRect.Bottom -= Size(15); //Make space for "Graph Limited School Edition" text at the bottom
#endif
  }

  if(Axes.xAxis.LogScl)
    xScale = AxesRect.Width() / std::log(Axes.xAxis.Max / Axes.xAxis.Min);
  else
    xScale = AxesRect.Width() / (Axes.xAxis.Max - Axes.xAxis.Min);

  if(Axes.yAxis.LogScl)
    yScale = AxesRect.Height() / std::log(Axes.yAxis.Max / Axes.yAxis.Min);
  else
    yScale = AxesRect.Height() / (Axes.yAxis.Max - Axes.yAxis.Min);

  if(Axes.xAxis.AutoTick)
  {
    int TextDist = Size(40);
    int Ticks = AxesRect.Width() / TextDist;
    double Dist = (Axes.xAxis.LogScl ? std::log10(Axes.xAxis.Max / Axes.xAxis.Min) : Axes.xAxis.Max - Axes.xAxis.Min) / Ticks;
    if(Axes.xAxis.LogScl)
      Axes.xAxis.TickUnit = std::pow10(std::ceil(Dist));
    else
    {
      if(Axes.xAxis.MultiplyOfPi)
        Dist /= M_PI;
      Axes.xAxis.TickUnit = AdjustUnit(Dist);
      if(Axes.xAxis.MultiplyOfPi)
        Axes.xAxis.TickUnit *= M_PI;
    }
  }

  if(Axes.yAxis.AutoTick)
  {
    int TextDist = Size(40);
    int Ticks = AxesRect.Height() / TextDist;
    double Dist = (Axes.yAxis.LogScl ? std::log10(Axes.yAxis.Max / Axes.yAxis.Min) : Axes.yAxis.Max - Axes.yAxis.Min) / Ticks;
    if(Axes.yAxis.LogScl)
      Axes.yAxis.TickUnit = std::pow10(std::ceil(Dist));
    else
    {
      if(Axes.yAxis.MultiplyOfPi)
        Dist /= M_PI;
      Axes.yAxis.TickUnit = AdjustUnit(Dist);
      if(Axes.yAxis.MultiplyOfPi)
        Axes.yAxis.TickUnit *= M_PI;
    }
  }

  if(Axes.xAxis.AutoGrid)
    Axes.xAxis.GridUnit = Axes.xAxis.TickUnit;

  if(Axes.yAxis.AutoGrid)
    Axes.yAxis.GridUnit = Axes.yAxis.TickUnit;

  DrawLegend();
}
//---------------------------------------------------------------------------
void TDraw::SetSize(int AWidth, int AHeight)
{
  Width = AWidth;
  Height = AHeight;
}
//---------------------------------------------------------------------------
//This returns the actual x coordinate from the x-pixel coordinate
double TDraw::xCoord(int x) const
{
  x -= AxesRect.Left;
  if(Axes.xAxis.LogScl)
    return Axes.xAxis.Min / std::exp(x * std::log(Axes.xAxis.Min / Axes.xAxis.Max) / AxesRect.Width());
  else
    return (x / xScale) + Axes.xAxis.Min;
}
//---------------------------------------------------------------------------
//This returns the actual y coordinate from y-pixel coordinate
double TDraw::yCoord(int y) const
{
  y -= AxesRect.Top;
  if(Axes.yAxis.LogScl)
    return Axes.yAxis.Max / std::exp(y * std::log(Axes.yAxis.Max / Axes.yAxis.Min) / AxesRect.Height());
  else
    return Axes.yAxis.Max - (y / yScale);
}
//---------------------------------------------------------------------------
bool TDraw::AbortUpdate()
{
  Thread->AbortUpdate();
  return Wait();
}
//---------------------------------------------------------------------------
bool TDraw::Wait()
{
  static bool AbortWait = false;
  if(!AbortWait)
  {
    bool Loop;
    if(!Thread->IsAborted())
      Thread->IdleEvent.WaitFor(); //Wait infinitely
    else
      do
      {
        Loop = false;
        if(Thread->IdleEvent.WaitFor(1000) == wrTimeout)
          if(MessageBox("The program seems to be stuck in an infinite loop.\nIf you cancel the operation the program will become unstable.\nYou should therefore save your data under a new name and restart the application.", "Thread is not responding", MB_RETRYCANCEL|MB_ICONSTOP) == IDCANCEL)
            AbortWait = true;
          else
            Loop = true;
      } while(Loop);
  }
  return !AbortWait;
}
//---------------------------------------------------------------------------
bool TDraw::Updating()
{
  return !Thread->IdleEvent.TestEvent();
}
//---------------------------------------------------------------------------
//Selects the drawing area
//Changeing clipping area and set viewport
void TDraw::SetArea(const TRect &Rect)
{
  //Set viewport; (0,0) is top left corner of drawing region
  Context.SetDeviceOrigin(Rect.Left, Rect.Top);

  Width = Rect.Width();
  Height = Rect.Height();
}
//---------------------------------------------------------------------------
//Draws lines between the points in the vector from Begin to End
void TDraw::DrawPolyline(TConstPointIter Begin, TConstPointIter End, TPenStyle Style, int LineSize, TColor Color)
{
  if(End - Begin >= 2)
  {
    Context.SetBrush(bsClear); //Enable drawing of other than solid lines on win9x
    Context.SetPen(Style, ForceBlack ? clBlack : Color, Size(LineSize));
    Context.DrawPolyline(&*Begin, End - Begin, AxesRect);
  }
}
//---------------------------------------------------------------------------
//Draws dots at all points from Begin to End
void TDraw::DrawPolydots(TConstPointIter Begin, TConstPointIter End, int LineSize, TColor Color)
{
  int Width = Size(LineSize);
  if(Width > 1)
  {
    Context.SetBrush(bsSolid, Color);
    Context.SetPen(psSolid, ForceBlack ? clBlack : Color, 1);
    while(Begin != End)
    {
      Context.DrawEllipse(Begin->x - Width / 2, Begin->y - Width / 2, Begin->x + (Width+1) / 2, Begin->y + (Width+1) / 2);
      ++Begin;
    }
  }
  else
    Context.DrawPolydots(&*Begin, End - Begin, Color);
}
//---------------------------------------------------------------------------
//Find the position of the the first tick on the axes.
//It has to be done in user units instead of pixels to prevent rounding differences
double TDraw::GetMinValue(double Unit, double Min, double Max, double AxisCross, bool Log)
{
  if(Log) //Is log scale used?
    //Get the first number to be shown
    return std::exp(std::floor(std::log(Min / AxisCross * Unit) / std::log(Unit)) * std::log(Unit) + std::log(AxisCross));
  //Get the first number to be shown
  return std::floor((Min - AxisCross) / Unit) * Unit + AxisCross + Unit;
}
//---------------------------------------------------------------------------
void TDraw::DrawAxes()
{
  Context.DestroyClipRect(); //Remove all clipping regions; Must be done to draw boxed axes

  //Exclude the legend from the drawing area
  if(Axes.ShowLegend)
    Context.ExcludeClipRect(LegendRect);

  int xLabelWidth;
  AnsiString Str; //A generel used string

  //Calculate font height for numbers
  Context.SetFont(Axes.NumberFont);
  int NumberHeight = Context.GetTextHeight("1");

  //Get axis cross in pixels
  int xPixelCross = Axes.AxesStyle == asBoxed ? AxesRect.Left : xPoint(Axes.yAxis.AxisCross);
  int yPixelCross = Axes.AxesStyle == asBoxed ? AxesRect.Bottom : yPoint(Axes.xAxis.AxisCross);

  double xTickMin = GetMinValue(Axes.xAxis.TickUnit, Axes.xAxis.Min, Axes.xAxis.Max, Axes.yAxis.AxisCross, Axes.xAxis.LogScl);
  double yTickMin = GetMinValue(Axes.yAxis.TickUnit, Axes.yAxis.Min, Axes.yAxis.Max, Axes.xAxis.AxisCross, Axes.yAxis.LogScl);

  if(Axes.xAxis.ShowGrid)
  {
    //Show grid parallel with y-axis
    if(Axes.xAxis.LogScl)
      Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.GridColor, Size(Axes.GridSize));
    else
      Context.SetGridPen(ForceBlack ? clBlack : Axes.GridColor, Size(Axes.GridSize));

    double xGridMin = GetMinValue(Axes.xAxis.GridUnit, Axes.xAxis.Min, Axes.xAxis.Max, Axes.yAxis.AxisCross, Axes.xAxis.LogScl);
    double GridPixelMin = xPointExact(xGridMin);
    double GridPixelScl = xScale * (Axes.xAxis.LogScl ? std::log(Axes.xAxis.GridUnit) : Axes.xAxis.GridUnit);

    for(double x = GridPixelMin; x < AxesRect.Right - Size(5); x += GridPixelScl)
      Context.DrawLine(x, AxesRect.Top, x, AxesRect.Bottom);

    if(Axes.xAxis.LogScl)
    {
      Context.SetGridPen(ForceBlack ? clBlack : Axes.GridColor, Size(Axes.GridSize));
      for(double x = xGridMin / Axes.xAxis.GridUnit; x < Axes.xAxis.Max; x *= Axes.xAxis.GridUnit)
        for(unsigned n = 1; n < 9; n++)
        {
          double X = xPoint(x*(1+(Axes.xAxis.GridUnit-1)*n/9));
          if(X > AxesRect.Left) //Don't draw outside area (if Axes Style is Boxed)
            Context.DrawLine(X, AxesRect.Top, X, AxesRect.Bottom);
        }
    }
    else if(Axes.xAxis.ShowTicks && Axes.xAxis.TickUnit > Axes.xAxis.GridUnit)
    {
      //Draw solid lines instead of ticks
      Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.GridColor, Size(Axes.GridSize));
      double xPixelScl = (Axes.xAxis.LogScl ? std::log(Axes.xAxis.TickUnit) : Axes.xAxis.TickUnit) * xScale;
      for(double x = xPointExact(xTickMin); x < AxesRect.Right - Size(5); x += xPixelScl)
        if(std::abs(x - xPixelCross) > 1) //Don't show at or beside axis (when scaled it might be moved a pixel or two)
          Context.DrawLine(x, AxesRect.Top, x, AxesRect.Bottom);
    }
  }

  if(Axes.yAxis.ShowGrid)
  {
    if(Axes.yAxis.LogScl)
      Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.GridColor, Size(Axes.GridSize));
    else
      Context.SetGridPen(ForceBlack ? clBlack : Axes.GridColor, Size(Axes.GridSize));

    double yGridMin = GetMinValue(Axes.yAxis.GridUnit, Axes.yAxis.Min, Axes.yAxis.Max, Axes.xAxis.AxisCross, Axes.yAxis.LogScl);
    double GridPixelMin = yPointExact(yGridMin);
    double GridPixelScl = yScale * (Axes.yAxis.LogScl ? std::log(Axes.yAxis.GridUnit) : Axes.yAxis.GridUnit);

    for(double y = GridPixelMin; y > AxesRect.Top + Size(5); y -= GridPixelScl)
      Context.DrawLine(AxesRect.Left, y, AxesRect.Right, y);

    if(Axes.yAxis.LogScl)
    {
      Context.SetGridPen(ForceBlack ? clBlack : Axes.GridColor, Size(Axes.GridSize));
      for(double y = yGridMin / Axes.yAxis.GridUnit; y < Axes.yAxis.Max; y *= Axes.yAxis.GridUnit)
        for(unsigned n = 1; n < 9; n++)
        {
          double Y = yPoint(y*(1+(Axes.yAxis.GridUnit-1)*n/9));
          if(Y < AxesRect.Top)
            break;
          Context.DrawLine(AxesRect.Left, Y, AxesRect.Right, Y);
        }
    }
    else if(Axes.yAxis.ShowTicks && Axes.yAxis.TickUnit > Axes.yAxis.GridUnit)
    {
      //Draw solid lines instead of ticks
      Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.GridColor, Size(Axes.GridSize));
      double yPixelScl = (Axes.yAxis.LogScl ? std::log(Axes.yAxis.TickUnit) : Axes.yAxis.TickUnit) * yScale;
      for(double y = yPointExact(yTickMin); y > AxesRect.Top + Size(5); y -= yPixelScl)
        if(std::abs(y - yPixelCross) > 1) //Don't show at or beside axis (when scaled it might be moved a pixel or two)
          Context.DrawLine(AxesRect.Left, y, AxesRect.Right, y);
    }
  }

  //If axes are diabled, don't draw axes, numbers and labels
  if(Axes.AxesStyle == ::asNone)
    return;

  //Make sure texts are written on transperent background
  Context.SetBrush(bsClear);

  //Set pen width and style; Used when drawing axes
  Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.GridColor, Size(2));

  if(Axes.xAxis.ShowNumbers)
  {
    double x; //Current x-position
    int yPixel = yPixelCross + Size(4); //Pixel position to draw numbers
    if(yPixel >= AxesRect.Top) //Check that numbers are inside allowed view
    {
      if(Axes.xAxis.LogScl) //Is log scale used?
        //Get the first number to be shown
        x = std::exp(std::floor((std::log(Axes.xAxis.Min / Axes.yAxis.AxisCross * Axes.xAxis.TickUnit)) / std::log(Axes.xAxis.TickUnit)) *
          std::log(Axes.xAxis.TickUnit) + std::log(Axes.yAxis.AxisCross));
      else
        //Get the first number to be shown
        x = std::floor((Axes.xAxis.Min - Axes.yAxis.AxisCross) / Axes.xAxis.TickUnit) * Axes.xAxis.TickUnit + Axes.yAxis.AxisCross + Axes.xAxis.TickUnit;

      while(x < Axes.xAxis.Max)
      {
        int xPixel = xPoint(x); //Calc pixel position
        //Check that we are not showing a number at the axis
        if(std::abs(xPixel - xPixelCross) > 1)
        {
          int TextWidth = NumberWidth(x, Axes.xAxis.MultiplyOfPi);

          //Check if we are not too close to the right side of the window
          if(xPixel < AxesRect.Right - TextWidth / 2)
            ShowNumber(xPixel - TextWidth/2, yPixel, x, Axes.xAxis.MultiplyOfPi);
        }
        //Is axis shown in log scale
        if(Axes.xAxis.LogScl)
          x *= Axes.xAxis.TickUnit;
        else
          x += Axes.xAxis.TickUnit; //Add scale to position
      }
    }
  }

  if(Axes.yAxis.ShowNumbers)
  {
    int xPixel = xPixelCross - Size(7); //Pixel position to draw numbers
    double y = yTickMin; //Current y-position

    //Loop through all coordinates on y-axis
    while(y < Axes.yAxis.Max)
    {
      int yPixel = yPoint(y);//Get pixel position
      //Check if we are not drawing any number at the axis
      if(std::abs(yPixel - yPixelCross) > 1)
        //Check if we are not too close to the upper boundery of the window
        if(yPixel > AxesRect.Top + NumberHeight / 2)
        {
          int Width = NumberWidth(y, Axes.yAxis.MultiplyOfPi);
          ShowNumber(xPixel-Width, yPixel - NumberHeight / 2, y, Axes.yAxis.MultiplyOfPi);
        }

      //Is axis shown in log scale
      if(Axes.yAxis.LogScl)
        y *= Axes.yAxis.TickUnit;
      else
        y += Axes.yAxis.TickUnit; //Add scale to position
    }
  }

  //Set font for labels
  Context.SetFont(Axes.LabelFont);
  if(Axes.xAxis.ShowLabel)
  {
    xLabelWidth = Context.GetTextWidth(Axes.xAxis.Label);
    Context.DrawText(Axes.xAxis.Label, AxesRect.Right-xLabelWidth-3, yPixelCross-Context.GetTextHeight(Axes.xAxis.Label)-Size(6));
  }

  if(Axes.yAxis.ShowLabel)
    Context.DrawText(Axes.yAxis.Label, xPixelCross + Size(12), AxesRect.Top);

  //If x-axis is inside the view
  if(yPixelCross >= AxesRect.Top && yPixelCross <= AxesRect.Bottom)
  {
    int X = AxesRect.Right - 1;
    int Y = yPixelCross;
    double xPixelScl = (Axes.xAxis.LogScl ? std::log(Axes.xAxis.TickUnit) : Axes.xAxis.TickUnit) * xScale;
    Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.AxesColor, Size(2));
    Context.DrawLine(AxesRect.Left, Y, X - Size(3), Y);

    //Show filled arrow on x-axis
    TPoint Arrow[] = {TPoint(X-Size(5), Y-Size(5)-1), TPoint(X, Y-1), TPoint(X, Y), TPoint(X-Size(5), Y+Size(5))};
    Context.SetBrush(bsSolid, ForceBlack ? clBlack : Axes.AxesColor);
    Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.AxesColor, 1);
    Context.DrawPolygon(Arrow, 4);

    //Only show ticks if we have not already drawn a solid grid line instead
    if(Axes.xAxis.ShowTicks && (!Axes.xAxis.ShowGrid || Axes.xAxis.TickUnit <= Axes.xAxis.GridUnit))
      //Show coordinate points on x-axis
      for(double x = xPointExact(xTickMin); x < AxesRect.Right - Size(5); x += xPixelScl)
        if(std::abs(x - xPixelCross) > 1) //Don't show at or beside axis (when scaled it might be moved a pixel or two)
          Context.DrawLine(x + 0.5, Y + Size(5), x + 0.5, Y - Size(5)-2);
  }

  //If y-axis is inside the view
  if(xPixelCross >= AxesRect.Left && xPixelCross <= AxesRect.Right)
  {
    int X = xPixelCross;
    int Y = AxesRect.Top;
    Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.AxesColor, Size(2));
    Context.DrawLine(X, Y + Size(3),X, AxesRect.Bottom);

    //Show arrow on y-axis
    TPoint Arrow[] = {TPoint(X-Size(5)-1, Y+Size(5)), TPoint(X-1, Y), TPoint(X, Y), TPoint(X+Size(5), Y+Size(5))};
    Context.SetBrush(bsSolid, ForceBlack ? clBlack : Axes.AxesColor);
    Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.AxesColor, 1);
    Context.DrawPolygon(Arrow, 4);

    double yPixelScl = (Axes.yAxis.LogScl ? std::log(Axes.yAxis.TickUnit) : Axes.yAxis.TickUnit) * yScale;
    //Only show ticks if we have not already drawn a solid grid line instead
    if(Axes.yAxis.ShowTicks && (!Axes.yAxis.ShowGrid || Axes.yAxis.TickUnit <= Axes.yAxis.GridUnit))
      //Show coordinate points on the y-axis
      for(double y = yPointExact(yTickMin); y > AxesRect.Top + Size(5); y -= yPixelScl)
        if(std::abs(y - yPixelCross) > 1) //Don't show at or beside axis (when scaled it might be moved a pixel or two)
          Context.DrawLine(X - Size(5)-1, y + 0.5, X + Size(5)+1, y + 0.5);
  }
}
//---------------------------------------------------------------------------
void TDraw::DrawLegend()
{
  if(!Axes.ShowLegend)
    return;

  Context.SetFont(Axes.LegendFont);
  Context.SetBrush(bsClear); //Change brush style to enable drawing of other than solid lines in Win9x

  int LegendCount = 0;
  int TextHeight = Context.GetTextHeight("0");
  int TextWidth = Size(40);

  for(unsigned I = 0; I < Data->ElemCount(); I++)
  {
    TGraphElem *Elem = Data->GetElem(I).get();
    if(Elem->Visible && Elem->ShowInLegend)
    {
      LegendCount++;
      TextWidth = std::max(TextWidth, Context.GetTextWidth(Elem->GetLegendText()));
    }

    for(unsigned N = 0; N < Elem->ChildList.size(); N++)
      if(Elem->ChildList[N]->Visible && Elem->ChildList[N]->ShowInLegend)
      {
        LegendCount++;
        TextWidth = std::max(TextWidth, Context.GetTextWidth(Elem->ChildList[N]->GetLegendText()));
      }
  }

  if(!LegendCount || !Axes.ShowLegend)
  {
    LegendRect = TRect(0, 0, 0, 0);
    return;
  }

  unsigned LegendWidth = std::min(TextWidth + Size(10), AxesRect.Width());
  unsigned LegendHeight = (TextHeight + Size(6)) * LegendCount + Size(10);
  switch(Axes.LegendPlacement)
  {
    case lpTopLeft:
      LegendRect.Top = AxesRect.Top;
      LegendRect.Left =  AxesRect.Left;
      break;

    case lpTopRight:
      LegendRect.Top = AxesRect.Top;
      LegendRect.Left = AxesRect.Right - LegendWidth;
      break;

    case lpBottomLeft:
      LegendRect.Top = AxesRect.Bottom - LegendHeight;
      LegendRect.Left =  AxesRect.Left;
      break;

    case lpBottomRight:
      LegendRect.Top = AxesRect.Bottom - LegendHeight;
      LegendRect.Left = AxesRect.Right - LegendWidth;
      break;

    default:
      LegendRect.Top = yPoint(Axes.LegendPos.y);
      LegendRect.Left = xPoint(Axes.LegendPos.x);
  }

  LegendRect.Bottom = LegendRect.Top + LegendHeight;
  LegendRect.Right = LegendRect.Left + LegendWidth;

  int x = LegendRect.Left + Size(5);
  int y = LegendRect.Top + TextHeight + Size(6);

  TDrawLegend DrawLegendItems(this, TextWidth, TextHeight, x, y);
  for(unsigned I = 0; I < Data->ElemCount(); I++)
  {
    TGraphElem *Elem = Data->GetElem(I).get();
    if(Elem->Visible && Elem->ShowInLegend)
      Elem->Accept(DrawLegendItems);

    for(unsigned N = 0; N < Elem->ChildList.size(); N++)
      if(Elem->ChildList[N]->Visible && Elem->ChildList[N]->ShowInLegend)
        Elem->ChildList[N]->Accept(DrawLegendItems);
  }

  //Draw rectangle around legend
  Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.LegendFont->Color, Size(1));
  Context.SetBrush(bsClear);
  Context.DrawRectangle(LegendRect);
}
//---------------------------------------------------------------------------
void TDraw::SetClippingRegion()
{
  //Always set clipping region; Some applications (e.g. WordPad) may else draw outside the visibe area
  Context.IntersectClipRect(AxesRect);

  //Exclude the legend from the drawing area
  if(Axes.ShowLegend)
    Context.ExcludeClipRect(LegendRect);
}
//---------------------------------------------------------------------------
//Return the needed dy=yMax-yMin to make x- and y-axes scale equal
double TDraw::GetScaledYAxis() const
{
  HDC DC = GetDC(0);
  double xPixelsPerInch = GetDeviceCaps(DC, LOGPIXELSX);
  double yPixelsPerInch = GetDeviceCaps(DC, LOGPIXELSY);
  ReleaseDC(0, DC);
  return (AxesRect.Height() / yPixelsPerInch) / (AxesRect.Width() / xPixelsPerInch);
}
//---------------------------------------------------------------------------
//Draw Number at (X, Y)
//If MultiplyByPi is true, number is draw as a fraction multiplied by pi
void TDraw::ShowNumber(int X, int Y, double Number, bool MultiplyByPi)
{
  if(MultiplyByPi)
  {
    std::pair<int, int> Fract = FloatToFract(Number / M_PI);
    if(Fract.first == 0)
    {
      Context.DrawText("0", X, Y);
      return;
    }

    int xPos = X;
    if(Fract.first != 1)
    {
      std::string Str = Fract.first == -1 ? std::string("-") : ToString(Fract.first);
      Context.DrawText(Str, X, Y);
      xPos += Context.GetTextWidth(Str);
    }

    int NumberHeight = Context.GetTextHeight("0");
    Context.SetFontName("Symbol");
    Context.DrawText("p", xPos, Y - (Context.GetTextHeight("p") - NumberHeight)); //The pi is alligned at the bottom
    xPos += Context.GetTextWidth("p");
    Context.SetFontName(Axes.NumberFont->Name.c_str());

    if(Fract.second != 1)
      Context.DrawText("/" + ToString(Fract.second), xPos, Y);
    return;
  }

  std::string Str = std::abs(Number) < MIN_ZERO ? "0" : FloatToStrF(Number, ffGeneral, 8, 8).c_str();
  Context.DrawText(Str, X, Y);
}
//---------------------------------------------------------------------------
//Returns the width of the number in pixels
unsigned TDraw::NumberWidth(double Number, bool MultiplyByPi)
{
  if(MultiplyByPi)
  {
    std::pair<int, int> Fract = FloatToFract(Number / M_PI);
    if(Fract.first == 0)
      return Context.GetTextWidth("0");

    unsigned Width = 0;
    if(Fract.first != 1)
    {
      std::string Str = Fract.first == -1 ? std::string("-") : ToString(Fract.first);
      Width = Context.GetTextWidth(Str);
    }

    Context.SetFontName("Symbol");
    Width += Context.GetTextWidth("p");
    Context.SetFontName(Axes.NumberFont->Name.c_str());

    if(Fract.second != 1)
      Width += Context.GetTextWidth("/" + ToString(Fract.second));
    return Width;
  }

  return Context.GetTextWidth(std::abs(Number) < MIN_ZERO ? "0" : FloatToStrF(Number, ffGeneral, 8, 8).c_str());
}
//---------------------------------------------------------------------------
void TDraw::SetSizeMul(double xSizeMul, double ySizeMul)
{
  SizeMul = xSizeMul;

  //We need to update size of all text labels
  //This is only used for printing, and this is not a good way to find the size; To make it work I scaled by 1.1
  for(unsigned I = 0; I < Data->ElemCount(); I++)
    if(TTextLabel *Label = dynamic_cast<TTextLabel*>(Data->GetElem(I).get()))
      Label->Scale(xSizeMul, ySizeMul);
}
//---------------------------------------------------------------------------
void TDrawLegend::Visit(TBaseFuncType &Func)
{
  TPenStyle Style = Func.DrawType == dtDots ? psDot : Func.Style;
  Draw->Context.SetBrush(bsClear); //Enable drawing of other than solid lines on win9x
  Draw->Context.SetPen(Style, Draw->ForceBlack ? clBlack : Func.Color, Size(Func.Size));
  Draw->Context.DrawLine(TPoint(x, y), TPoint(x + TextWidth, y));

  Draw->Context.DrawText(Func.GetLegendText(), x, y - TextHeight - Size(1));
  y += TextHeight + Size(6);
}
//---------------------------------------------------------------------------
void TDrawLegend::Visit(TPointSeries &Series)
{
  if(Series.LineStyle != psClear)
  {
    std::vector<TPoint> LinePoints;
    LinePoints.push_back(TPoint(x,y));
    LinePoints.push_back(TPoint(x + TextWidth, y));
    Draw->DrawPolyline(LinePoints.begin(), LinePoints.end(), Series.LineStyle, Series.LineSize, Series.LineColor);
  }

  //Adjust point size in legend to max 6 (4 for arrow)
  unsigned PointSize = std::min(Series.Size, Series.Style == 7 ? 4 : 6);
  TColor FrameColor = Draw->ForceBlack ? clBlack : Series.FrameColor;
  TColor FillColor = Draw->ForceBlack ? clWhite : Series.FillColor;
  if(PointSize > 0)
    for(int X = x + Size(20); X < x + TextWidth - Size(10); X += Size(50))
     TPointSelect::DrawPoint(Draw->Context.GetCanvas(), TPoint(X, y), Series.Style, Series.Size > 2 ? FrameColor : FillColor, FillColor, Size(PointSize));

  Draw->Context.SetBrush(bsClear);
  Draw->Context.DrawText(Series.GetLegendText(), x, y - TextHeight - Size(1));
  y += TextHeight + Size(6);
}
//---------------------------------------------------------------------------
void TDrawLegend::Visit(TShade &Shade)
{
  //Don't Use FillRect() as it will not shade transparent
  Draw->Context.SetPen(psClear, clWhite, 1);
  Draw->Context.SetBrush(Shade.BrushStyle, Draw->ForceBlack ? clBlack : Shade.Color);
  TRect R(x, y - Size(3), x + TextWidth, y + Size(6));
  TPoint Points[] = {TPoint(R.Left, R.Top), TPoint(R.Right, R.Top), TPoint(R.Right, R.Bottom), TPoint(R.Left, R.Bottom)};
  Draw->Context.DrawPolygon(Points, 4);

  Draw->Context.DrawText(Shade.GetLegendText(), x, y - TextHeight - Size(1));
  y += TextHeight + Size(6);
}
//---------------------------------------------------------------------------
void TDrawLegend::Visit(TRelation &Relation)
{
  if(Relation.GetRelationType() == rtEquation)
  {
    std::vector<TPoint> LinePoints;
    LinePoints.push_back(TPoint(x, y));
    LinePoints.push_back(TPoint(x + TextWidth, y));
    Draw->DrawPolyline(LinePoints.begin(), LinePoints.end(), psSolid, Relation.GetSize(), Relation.GetColor());
  }
  else
  {
    //Draw relation as a shading. Don't Use FillRect() as it will not shade transparent
    Draw->Context.SetPen(psClear, clWhite, 1);
    Draw->Context.SetBrush(Relation.GetBrushStyle(), Draw->ForceBlack ? clBlack : Relation.GetColor());
    TRect R(x, y - Size(3), x + TextWidth, y + Size(6));
    TPoint Points[] = {TPoint(R.Left, R.Top), TPoint(R.Right, R.Top), TPoint(R.Right, R.Bottom), TPoint(R.Left, R.Bottom)};
    Draw->Context.DrawPolygon(Points, 4);
  }

  Draw->Context.SetBrush(bsClear); //Draw text transparent
  Draw->Context.DrawText(Relation.GetLegendText(), x, y - TextHeight - Size(1));
  y += TextHeight + Size(6);
}
//---------------------------------------------------------------------------
void TDraw::DrawPointLabel(TCanvas *Canvas, TPoint Pos, int PointSize, const std::string &Label, TLabelPosition LabelPosition)
{
  Canvas->Brush->Style = bsClear;
  TSize TextSize = Canvas->TextExtent(Label.c_str());

  switch(LabelPosition)
  {
    case lpAbove:
      Pos.x -= TextSize.cx / 2;
      Pos.y -= PointSize + TextSize.cy;
      break;

    case lpBelow:
      Pos.x -= TextSize.cx / 2;
      Pos.y += PointSize;
      break;

    case lpLeft:
      Pos.x -= TextSize.cx + PointSize;
      Pos.y -= TextSize.cy / 2;
      break;

    case lpRight:
      Pos.x += 2*PointSize;
      Pos.y -= TextSize.cy / 2;
      break;
  }
  Canvas->TextOut(Pos.x, Pos.y, Label.c_str());
}
//---------------------------------------------------------------------------

