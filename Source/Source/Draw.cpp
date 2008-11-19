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
void TDraw::DrawAll()
{
  AbortUpdate();
  Context.DestroyClipRect(); //Remove all clipping regions; Must be done if printing more than one page

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
          if(MessageBox(LoadRes(559), LoadRes(560), MB_RETRYCANCEL|MB_ICONSTOP) == IDCANCEL)
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
//    return std::exp(std::floor(std::log(Min / AxisCross * Unit) / std::log(Unit)) * std::log(Unit) + std::log(AxisCross));
    return std::exp(std::floor(std::log(Min / AxisCross) / std::log(Unit)) * std::log(Unit) + std::log(AxisCross));
  //Get the first number to be shown
//  return std::floor((Min - AxisCross) / Unit) * Unit + AxisCross + Unit;
  return std::ceil((Min - AxisCross) / Unit) * Unit + AxisCross;
}
//---------------------------------------------------------------------------
//Fills yLabelInfo with list of labels and return the maximum with
unsigned TDraw::FindLabels()
{
  yLabelInfo.clear();
  unsigned MaxWidth = 0;

  if(Axes.yAxis.ShowNumbers)
  {
    //Calculate font height for numbers
    Context.SetFont(Axes.NumberFont);
    double y = yTickMin;

    //Loop through all coordinates on y-axis
    while(y < Axes.yAxis.Max)
    {
      int yPixel = yPoint(y);//Get pixel position
        //Check that we are not showing a number at the axis when they are crossed
      if(Axes.AxesStyle == asBoxed || std::abs(yPixel - yPixelCross) > 1)
        //Check if we are not too close to the boundery of the window
        if(yPixel + NumberHeight / 2 < Height && yPixel - NumberHeight / 2 > AxesRect.Top)
        {
          TLabelInfo LabelInfo;
          LabelInfo.Label = MakeNumber(y, Axes.yAxis.MultiplyOfPi);
          LabelInfo.Width = Context.GetTextWidth(LabelInfo.Label);
          LabelInfo.Pos = yPixel;;
          if(Axes.NumberPlacement == npCenter)
            LabelInfo.Pos -= NumberHeight / 2;
          yLabelInfo.push_back(LabelInfo);
          if(LabelInfo.Width > MaxWidth)
            MaxWidth = LabelInfo.Width;
        }

      //Is axis shown in log scale
      if(Axes.yAxis.LogScl)
        y *= Axes.yAxis.TickUnit;
      else
        y += Axes.yAxis.TickUnit; //Add scale to position
    }
  }
  return MaxWidth;
}
//---------------------------------------------------------------------------
void TDraw::PreCalcXAxis()
{
  unsigned MaxLabelWidth = FindLabels();
  if(Axes.AxesStyle == asBoxed)
    AxesRect.Left = MaxLabelWidth + Size(7);

  if(Axes.xAxis.LogScl)
    xScale = AxesRect.Width() / std::log(Axes.xAxis.Max / Axes.xAxis.Min);
  else
    xScale = AxesRect.Width() / (Axes.xAxis.Max - Axes.xAxis.Min);

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

  xPixelCross = Axes.AxesStyle == asBoxed ? AxesRect.Left : xPoint(yAxisCross);
  if(Axes.xAxis.AutoGrid)
    Axes.xAxis.GridUnit = Axes.xAxis.TickUnit;
  xTickMin = GetMinValue(Axes.xAxis.TickUnit, Axes.xAxis.Min, Axes.xAxis.Max, yAxisCross, Axes.xAxis.LogScl);
}
//---------------------------------------------------------------------------
void TDraw::PreCalcYAxis()
{
  if(Axes.AxesStyle == asBoxed)
  {
    AxesRect.Bottom = Height - NumberHeight - Size(4);
#ifdef LIMITED_EDITION
    AxesRect.Bottom -= Size(15); //Make space for "Graph Limited School Edition" text at the bottom
#endif
  }

  if(Axes.yAxis.LogScl)
    yScale = AxesRect.Height() / std::log(Axes.yAxis.Max / Axes.yAxis.Min);
  else
    yScale = AxesRect.Height() / (Axes.yAxis.Max - Axes.yAxis.Min);

  if(Axes.yAxis.AutoTick)
  {
    int TextDist = Size(40);
    int Ticks = AxesRect.Height() / TextDist;
    if(Ticks == 0)
      Ticks = 1; //Avoid division by zero, just in case.
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

  xAxisCross = Axes.AxesStyle == asBoxed ? (Axes.yAxis.LogScl ? 0.1 : 0) : Axes.xAxis.AxisCross;
  yAxisCross = Axes.AxesStyle == asBoxed ? (Axes.xAxis.LogScl ? 0.1 : 0) : Axes.yAxis.AxisCross;
  yTickMin = GetMinValue(Axes.yAxis.TickUnit, Axes.yAxis.Min, Axes.yAxis.Max, xAxisCross, Axes.yAxis.LogScl);
  yPixelCross = Axes.AxesStyle == asBoxed ? AxesRect.Bottom : yPoint(xAxisCross);

  if(Axes.yAxis.AutoGrid)
    Axes.yAxis.GridUnit = Axes.yAxis.TickUnit;
}
//---------------------------------------------------------------------------
void TDraw::RedrawAxes()
{
  AxesRect = TRect(0, 0, Width, Height);
  Context.SetFont(Axes.NumberFont);
  NumberHeight = Context.GetTextHeight("1");

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

  PreCalcYAxis();
  PreCalcXAxis();

  DrawLegend();
}
//---------------------------------------------------------------------------
void TDraw::DrawAxes()
{
  Context.DestroyClipRect(); //Remove all clipping regions; Must be done to draw boxed axes

  //Exclude the legend from the drawing area
  if(Axes.ShowLegend)
    Context.ExcludeClipRect(LegendRect);

  int xLabelWidth;
 
  //Calculate font height for numbers
  Context.SetFont(Axes.NumberFont);

  std::vector<int> xGridMajor, xGridMinor, yGridMajor, yGridMinor;

  if(Axes.xAxis.ShowGrid || Axes.GridStyle == gsDots)
  {
    //Show grid parallel with y-axis
    double GridMin = GetMinValue(Axes.xAxis.GridUnit, Axes.xAxis.Min, Axes.xAxis.Max, yAxisCross, Axes.xAxis.LogScl);
    double GridPixelScl = xScale * (Axes.xAxis.LogScl ? std::log(Axes.xAxis.GridUnit) : Axes.xAxis.GridUnit);

    bool ShowGridTick = !Axes.xAxis.LogScl && Axes.xAxis.ShowTicks && Axes.xAxis.TickUnit > Axes.xAxis.GridUnit;
    double MaxPixel = AxesRect.Right - Size(5);
    double MinTickPixel = ShowGridTick ? xPointExact(xTickMin) : MaxPixel;
    double TickPixelScl = Axes.xAxis.TickUnit * xScale;
    double x = xPointExact(GridMin);
    if(x < Size(5))
      x += GridPixelScl;

    //Draw dotted grid lines. If Tick unit is greater than the Grid unit, the ticks are drawn as solid
    //lines from one side to the other. A grid line is not drawn if it is within one pixel from a
    //grid line. This is done to avoid rounding problems where tick lines and grid lines are calculated
    //to have one pixel in difference while they actually should be on top of each other.
    for(double x2 = MinTickPixel; x <= MaxPixel; x2 += TickPixelScl)
    {
      for(; x < x2 + 1; x += GridPixelScl)
        //Don't show at or beside axis (when scaled it might be moved a pixel or two)
        if(std::abs(x - xPixelCross) > 1)
          if(Axes.xAxis.LogScl)
            xGridMajor.push_back(x + 0.5);
          else
            xGridMinor.push_back(x + 0.5);

      if(x < x2 + 1 && x> x2 - 1)
        x += GridPixelScl;

      if(ShowGridTick && x2 < MaxPixel)
        //Draw solid lines instead of ticks
        xGridMajor.push_back(x2 + 0.5);
    }

    if(Axes.xAxis.LogScl)
    {
      for(double x = GridMin / Axes.xAxis.GridUnit; x < Axes.xAxis.Max; x *= Axes.xAxis.GridUnit)
        for(unsigned n = 1; n < 9; n++)
        {
          int X = xPoint(x*(1+(Axes.xAxis.GridUnit-1)*n/9));
          //Don't draw outside area (if Axes Style is Boxed)
          if(X > AxesRect.Left)
            xGridMinor.push_back(X);
        }
    }
  }

  if(Axes.yAxis.ShowGrid || Axes.GridStyle == gsDots)
  {
    double GridMin = GetMinValue(Axes.yAxis.GridUnit, Axes.yAxis.Min, Axes.yAxis.Max, xAxisCross, Axes.yAxis.LogScl);
    double GridPixelScl = yScale * (Axes.yAxis.LogScl ? std::log(Axes.yAxis.GridUnit) : Axes.yAxis.GridUnit);

    bool ShowGridTick = !Axes.yAxis.LogScl && Axes.yAxis.ShowTicks && Axes.yAxis.TickUnit > Axes.yAxis.GridUnit;
    double MaxPixel = AxesRect.Top + Size(5);
    double MinTickPixel = ShowGridTick ? yPointExact(yTickMin) : MaxPixel;
    double TickPixelScl = Axes.yAxis.TickUnit * yScale;
    double y = yPointExact(GridMin);

    //Draw dotted grid lines. If Tick unit is greater than the Grid unit, the ticks are drawn as solid
    //lines from one side to the other. A grid line is not drawn if it is within one pixel from a
    //grid line. This is done to avoid rounding problems where tick lines and grid lines are calculated
    //to have one pixel in difference while they actually should be on top of each other.
    for(double y2 = MinTickPixel; y >= MaxPixel; y2 -= TickPixelScl)
    {
      for(; y > y2 + 1; y -= GridPixelScl)
        //Don't show at or beside axis (when scaled it might be moved a pixel or two)
        if(std::abs(y - yPixelCross) > 1)
          if(Axes.yAxis.LogScl)
            yGridMajor.push_back(y + 0.5);
          else
            yGridMinor.push_back(y + 0.5);

      if(y < y2 + 1 && y > y2 - 1)
        y -= GridPixelScl;

      if(ShowGridTick && y2 > MaxPixel)
        //Draw solid lines instead of ticks
        yGridMajor.push_back(y2 + 0.5);
    }

    if(Axes.yAxis.LogScl)
    {
      for(double y = GridMin / Axes.yAxis.GridUnit; y < Axes.yAxis.Max; y *= Axes.yAxis.GridUnit)
        for(unsigned n = 1; n < 9; n++)
        {
          double Y = yPoint(y*(1+(Axes.yAxis.GridUnit-1)*n/9));
          if(Y < AxesRect.Top || Y > AxesRect.Bottom)
            break;
          yGridMinor.push_back(Y);
        }
    }
  }

  if(Axes.GridStyle == gsLines)
  {
    //Draw solid lines
    Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.GridColor, Size(Axes.GridSize));
    for(std::vector<int>::const_iterator Iter = xGridMajor.begin(); Iter != xGridMajor.end(); ++Iter)
      Context.DrawLine(*Iter, AxesRect.Top, *Iter, AxesRect.Bottom);
    for(std::vector<int>::const_iterator Iter = yGridMajor.begin(); Iter != yGridMajor.end(); ++Iter)
      Context.DrawLine(AxesRect.Left, *Iter, AxesRect.Right, *Iter);

    //Draw dotted lines
    Context.SetGridPen(ForceBlack ? clBlack : Axes.GridColor, Size(Axes.GridSize));
    for(std::vector<int>::const_iterator Iter = xGridMinor.begin(); Iter != xGridMinor.end(); ++Iter)
      Context.DrawLine(*Iter, AxesRect.Top, *Iter, AxesRect.Bottom);
    for(std::vector<int>::const_iterator Iter = yGridMinor.begin(); Iter != yGridMinor.end(); ++Iter)
      Context.DrawLine(AxesRect.Left, *Iter, AxesRect.Right, *Iter);
  }
  else
  {
    int Width = Size(Axes.GridSize);
    Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.GridColor, 1);
    Context.SetBrush(bsSolid, ForceBlack ? clBlack : Axes.GridColor);
    for(std::vector<int>::const_iterator xIter = xGridMinor.begin(); xIter != xGridMinor.end(); ++xIter)
      for(std::vector<int>::const_iterator yIter = yGridMinor.begin(); yIter != yGridMinor.end(); ++yIter)
        Context.DrawEllipse(*xIter - Width, *yIter - Width, *xIter + Width, *yIter + Width);
    //Draw a dot where the grid lines would cross
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
    double x = xTickMin; //Current x-position
    int yPixel = yPixelCross + Size(4); //Pixel position to draw numbers
    if(yPixel >= AxesRect.Top) //Check that numbers are inside allowed view
    {
      while(x < Axes.xAxis.Max)
      {
        int xPixel = xPoint(x); //Calc pixel position
        //Check that we are not showing a number at the axis when they are crossed
        if(Axes.AxesStyle == asBoxed || std::abs(xPixel - xPixelCross) > 1)
        {
          std::wstring Str = MakeNumber(x, Axes.xAxis.MultiplyOfPi);
          int TextWidth = Context.GetTextWidth(Str);

          //Check if we are not too close to the sides of the window
          //Compare with 0 instead of AxesRect.Left because it is okay to write in the blank area
          if(xPixel - TextWidth / 2 >= 0 && xPixel + TextWidth / 2 <= AxesRect.Right)
          {                                                  
            xPixel -= Axes.NumberPlacement == npCenter ? TextWidth / 2 : TextWidth;
            Context.DrawText(Str, xPixel, yPixel);
          }
        }
        //Is axis shown in log scale
        if(Axes.xAxis.LogScl)
          x *= Axes.xAxis.TickUnit;
        else
          x += Axes.xAxis.TickUnit; //Add scale to position
      }
    }
  }

  //Draw number labels on the y-axis
  for(std::vector<TLabelInfo>::const_iterator Iter = yLabelInfo.begin(); Iter != yLabelInfo.end(); ++Iter)
    Context.DrawText(Iter->Label, xPixelCross - Iter->Width - Size(7), Iter->Pos);

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
    int X1 = AxesRect.Left;
    int X2 = AxesRect.Right - 1;
    int Y = yPixelCross;
    double xPixelScl = (Axes.xAxis.LogScl ? std::log(Axes.xAxis.TickUnit) : Axes.xAxis.TickUnit) * xScale;
    Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.AxesColor, Size(2));
    Context.DrawLine(X1, Y, X2 - Size(3), Y);

    Context.SetBrush(bsSolid, ForceBlack ? clBlack : Axes.AxesColor);
    Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.AxesColor, 1);
    const TPoint LeftArrow[] = {TPoint(X1+Size(5), Y-Size(5)-1), TPoint(X1, Y-1), TPoint(X1, Y), TPoint(X1+Size(5), Y+Size(5))};
    const TPoint RightArrow[] = {TPoint(X2-Size(5), Y-Size(5)-1), TPoint(X2, Y-1), TPoint(X2, Y), TPoint(X2-Size(5), Y+Size(5))};
    //Show filled arrow on x-axis
    if(Axes.AxesArrows == aaBothEnds)
      Context.DrawPolygon(LeftArrow, 4);
    if(Axes.AxesArrows != aaNone)
      Context.DrawPolygon(RightArrow, 4);

    //Only show ticks if we have not already drawn a solid grid line instead
    if(Axes.xAxis.ShowTicks && (!Axes.xAxis.ShowGrid || Axes.xAxis.TickUnit <= Axes.xAxis.GridUnit))
      //Show coordinate points on x-axis
      for(double x = xPointExact(xTickMin); x < AxesRect.Right - Size(5); x += xPixelScl)
        //Don't show at or beside axis (when scaled it might be moved a pixel or two)
        //Don't show tick at left side
        if(x > AxesRect.Left && std::abs(x - xPixelCross) > 1)
          Context.DrawLine(x + 0.5, Y + Size(5), x + 0.5, Y - Size(5)-2);
  }

  //If y-axis is inside the view
  if(xPixelCross >= AxesRect.Left && xPixelCross <= AxesRect.Right)
  {
    int X = xPixelCross;
    int Y1 = AxesRect.Top;
    int Y2 = AxesRect.Bottom;
    Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.AxesColor, Size(2));
    Context.DrawLine(X, Y1 + Size(3), X, Y2);

    Context.SetBrush(bsSolid, ForceBlack ? clBlack : Axes.AxesColor);
    Context.SetPen(psSolid, ForceBlack ? clBlack : Axes.AxesColor, 1);
    TPoint TopArrow[] = {TPoint(X-Size(5)-1, Y1+Size(5)), TPoint(X-1, Y1), TPoint(X, Y1), TPoint(X+Size(5), Y1+Size(5))};
    TPoint BottomArrow[] = {TPoint(X-Size(5)-1, Y2-Size(6)), TPoint(X-1, Y2-1), TPoint(X, Y2-1), TPoint(X+Size(5), Y2-Size(6))};

    //Show arrow on y-axis
    if(Axes.AxesArrows == aaBothEnds)
      Context.DrawPolygon(BottomArrow, 4);
    if(Axes.AxesArrows != aaNone)
      Context.DrawPolygon(TopArrow, 4);

    double yPixelScl = (Axes.yAxis.LogScl ? std::log(Axes.yAxis.TickUnit) : Axes.yAxis.TickUnit) * yScale;
    //Only show ticks if we have not already drawn a solid grid line instead
    if(Axes.yAxis.ShowTicks && (!Axes.yAxis.ShowGrid || Axes.yAxis.TickUnit <= Axes.yAxis.GridUnit))
      //Show coordinate points on the y-axis
      for(double y = yPointExact(yTickMin); y > AxesRect.Top + Size(5); y -= yPixelScl)
        if(std::abs(y - yPixelCross) > 1 &&  //Don't show at or beside axis (when scaled it might be moved a pixel or two)
            y < AxesRect.Bottom - 4) //Don't show too close to bottom
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
    if(Elem->GetVisible() && Elem->GetShowInLegend())
    {
      LegendCount++;
      TextWidth = std::max(TextWidth, Context.GetTextWidth(Elem->MakeLegendText()));
    }

    for(unsigned N = 0; N < Elem->ChildList.size(); N++)
      if(Elem->ChildList[N]->GetVisible() && Elem->ChildList[N]->GetShowInLegend())
      {
        LegendCount++;
        TextWidth = std::max(TextWidth, Context.GetTextWidth(Elem->ChildList[N]->MakeLegendText()));
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
    if(Elem->GetVisible() && Elem->GetShowInLegend())
      Elem->Accept(DrawLegendItems);

    for(unsigned N = 0; N < Elem->ChildList.size(); N++)
      if(Elem->ChildList[N]->GetVisible() && Elem->ChildList[N]->GetShowInLegend())
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
//Return the needed dy/dx for scaling the axes equal, i.e. d/dx is the visual difference in the pixels
double TDraw::GetScaledYAxis() const
{
  HDC DC = GetDC(0);
  double xPixelsPerInch = GetDeviceCaps(DC, LOGPIXELSX);
  double yPixelsPerInch = GetDeviceCaps(DC, LOGPIXELSY);
  ReleaseDC(0, DC);
  return (AxesRect.Height() / yPixelsPerInch) / (AxesRect.Width() / xPixelsPerInch);
}
//---------------------------------------------------------------------------
//If MultiplyByPi is true, the number is a fraction multiplied by pi
std::wstring TDraw::MakeNumber(double Number, bool MultiplyByPi)
{
  if(MultiplyByPi)
  {
    std::pair<int, int> Fract = FloatToFract(Number / M_PI);
    if(Fract.first == 0)
      return L"0";

    std::wstring Str;
    if(Fract.first != 1)
      Str = Fract.first == -1 ? std::wstring(L"-") : ToWString(Fract.first);

    Str += L'\x3C0'; // Pi

    if(Fract.second != 1)
      Str += L"/" + ToWString(Fract.second);
    return Str;
  }

  if(std::abs(Number) < MIN_ZERO)
    return L"0";
  return ToWString(FloatToStrF(Number, ffGeneral, 8, 8));
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

  Draw->Context.DrawText(Func.MakeLegendText(), x, y - TextHeight - Size(1));
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
  unsigned PointSize = std::min(Series.Size, Series.Style == 7 ? 4U : 6U);
  TColor FrameColor = Draw->ForceBlack ? clBlack : Series.FrameColor;
  TColor FillColor = Draw->ForceBlack ? clWhite : Series.FillColor;
  if(PointSize > 0)
    for(int X = x + Size(20); X < x + TextWidth - Size(10); X += Size(50))
     TPointSelect::DrawPoint(Draw->Context.GetCanvas(), TPoint(X, y), Series.Style, Series.Size > 2 ? FrameColor : FillColor, FillColor, Size(PointSize));

  Draw->Context.SetBrush(bsClear);
  Draw->Context.DrawText(Series.MakeLegendText(), x, y - TextHeight - Size(1));
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

  Draw->Context.DrawText(Shade.MakeLegendText(), x, y - TextHeight - Size(1));
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
  Draw->Context.DrawText(Relation.MakeLegendText(), x, y - TextHeight - Size(1));
  y += TextHeight + Size(6);
}
//---------------------------------------------------------------------------
void TDraw::DrawPointLabel(TCanvas *Canvas, TPoint Pos, int PointSize, const std::wstring &Label, TLabelPosition LabelPosition)
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

