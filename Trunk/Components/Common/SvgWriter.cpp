/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2008 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Config.h"
#pragma hdrstop
#include "SvgWriter.h"
#include <cassert>
#include <vector>
#include <iomanip>
#include <algorithm>
//---------------------------------------------------------------------------
std::string Utf8Encode(const std::wstring &Str)
{
  std::string Result;
  for(unsigned I = 0; I < Str.size(); I++)
  {
    wchar_t ch = Str[I];
    if(ch == L'<')
      Result += "&lt;";
    else if(ch == L'&')
      Result += "&amp;";
    else if(ch <= 0x7F)
      Result.push_back(ch);
    else if(ch > 0x7FF)
    {
      Result.push_back(0xE0 | (ch >> 12));
      Result.push_back(0x80 | ((ch >> 6) & 0x3F));
      Result.push_back(0x80 | (ch & 0x3F));
    }
    else // 0x7F < ch <= 0x7FF
    {
      Result.push_back(0xC0 | (ch >> 6));
      Result.push_back(0x80 | (ch & 0x3F));
    }
  }
  return Result;
}
//---------------------------------------------------------------------------
#pragma argsused
void TSvgWriter::BeginFile(const RECTL &Rect, unsigned Width, unsigned Height)
{
  unsigned PixelWidth = Rect.right - Rect.left + 1;
  unsigned PixelHeight = Rect.bottom - Rect.top + 1;
  ViewBox = Rect;
  Stream <<
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
    "<svg " << "width=\"" << PixelWidth << "px\" height=\"" << PixelHeight << "px\" "  << "viewBox=\""
      << Rect.left << " " << Rect.top << " " << PixelWidth << " " << PixelHeight
      << "\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n";
  Stream << "  <g>\n";
  Stream << std::setfill('0');
}
//---------------------------------------------------------------------------
void TSvgWriter::EndOfFile()
{
  Stream << "  </g>\n";
  Stream << "</svg>\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Line(int X1, int Y1, int X2, int Y2)
{
  double x1 = X1, x2 = X2, y1 = Y1, y2 = Y2;
  if(Pen.Width % 2)
    x1 += 0.5, x2 += 0.5, y1 += 0.5, y2 += 0.5;
  Stream << "    <line";
  WritePen();
  Stream << " x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Polyline(const POINTS *Points, int Count)
{
  Stream << "    <polyline";
  WritePen();
  Stream << " fill=\"none\" points=\"";
  for(int I = 0; I < Count; I++)
    Stream << Points[I].x << "," << Points[I].y << " ";
  Stream << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Polygon(const POINTS *Points, int Count, TPolyFillMode PolyFillMode)
{
  bool OddPen = Pen.Width % 2;
  Stream << "    <polygon";
  WritePen();
  WriteBrush();
  if(PolyFillMode == pfmEvenOdd)
    Stream << " fill-rule=\"evenodd\"";
  Stream << " points=\"";
  for(int I = 0; I < Count; I++)
    Stream << (OddPen ? Points[I].x + 0.5 : Points[I].x) << "," <<
      (OddPen ? Points[I].y + 0.5 : Points[I].y) << " ";
  Stream << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Rectangle(const RECTL &Rect)
{
  Stream << "    <rect";
  WritePen();
  WriteBrush();
  double x = Pen.Width % 2 ? Rect.left + 0.5 : Rect.left;
  double y = Pen.Width % 2 ? Rect.top + 0.5 : Rect.top;
  Stream << " x=\"" << x << "\" y=\"" << y << "\" width=\"" <<
    (Rect.right - Rect.left) << "\" height=\"" << (Rect.bottom - Rect.top) << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Ellipse(const RECTL &Rect)
{
  bool Circle = Rect.right - Rect.left == Rect.bottom - Rect.top;
  Stream << (Circle ? "    <circle" : "    <ellipse");
  WritePen();
  WriteBrush();
  Stream << " cx=\"" << (Rect.right + Rect.left)/2 << "\" cy=\"" << (Rect.bottom + Rect.top)/2 << "\" ";
  if(Circle)
    Stream << "r=\"" << (Rect.right - Rect.left)/2;
  else
    Stream << "cx=\"" << (Rect.right - Rect.left)/2 << "\" cy=\"" << (Rect.bottom - Rect.top)/2;
  Stream << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Arc(const RECTL &Box, const POINTL &Start, const POINTL &End)
{
  Stream << "    <path fill=\"none\"";
  WritePen();
  Stream << " d=\"M" << Start.x << "," << Start.y;
  Stream << " A" << (Box.right - Box.left) / 2 << "," << (Box.bottom - Box.top) / 2;
  Stream << " 0 0 0";
  Stream << " " << End.x << "," << End.y << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Text(int X, int Y, const std::wstring &Str, const TFontInfo &Font)
{
  Stream << "    <text x=\"" << X << "\" y=\"" << Y << "\" font-family=\"" <<
    Font.Name << "\" font-size=\"" << Font.Size << "\"";
  if(Font.Weight != 400)
    Stream << " font-weight=\"" << Font.Weight << "\"";
  if(Font.Italic)
    Stream << " font-style=\"italic\"";
  if(Font.StrikeOut || Font.Underline)
  {
    Stream << " text-decoration=\"";
    if(Font.Underline) Stream << "underline";
    if(Font.Underline && Font.StrikeOut) Stream << ",";
    if(Font.StrikeOut) Stream << "line-through";
    Stream << "\"";
  }
  Stream << " fill=\"#" << std::hex << std::setw(6) << Font.Color << std::dec << "\"";
  Stream << ">" << Utf8Encode(Str) << "</text>\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::WritePen()
{
  if((Pen.Style &  PS_STYLE_MASK) == PS_NULL)
    Stream << " stroke=\"none\"";
  else
    Stream << " stroke=\"#" << std::hex << std::setw(6) << Pen.Color << "\" stroke-width=\"" << std::dec << Pen.Width << "\"";

  switch(Pen.Style &  PS_STYLE_MASK)
  {
    case PS_DASH:
      Stream << " stroke-dasharray=\"5,5\" ";
      break;

    case PS_DOT:
      Stream << " stroke-dasharray=\"1,5\" stroke-linecap=\"round\" ";
      break;

    case PS_DASHDOT:
      Stream << " stroke-dasharray=\"5,5,1,5\" stroke-linecap=\"round\" ";
      break;

    case PS_DASHDOTDOT:
      Stream << " stroke-dasharray=\"5,5,1,5,1,5\" stroke-linecap=\"round\" ";
      break;

    case PS_SOLID:
    default:
      break;
  }
  Stream << std::dec;
}
//---------------------------------------------------------------------------
void TSvgWriter::WriteBrush()
{
  switch(Brush.Style)
  {
    case BS_NULL:
      Stream << " fill=\"none\"";
      break;

    case BS_HATCHED:
      Stream << " fill=\"url(#pattern" << (1+(std::find(PatternList.begin(), PatternList.end(), Brush) - PatternList.begin())) << ")\"";
      break;

    case BS_SOLID:
    default:
      Stream << " fill=\"#" << std::hex << std::setw(6) << Brush.Color << "\"";
  }
  Stream << std::dec;
}
//---------------------------------------------------------------------------
void TSvgWriter::SetPen(const TPenInfo &APen)
{
  Pen = APen;
}
//---------------------------------------------------------------------------
void TSvgWriter::SetBrush(const TBrushInfo &ABrush)
{
  Brush = ABrush;
  if(Brush.Style == BS_HATCHED)
    CreatePattern();
}
//---------------------------------------------------------------------------
unsigned TSvgWriter::CreatePattern()
{
  std::vector<TBrushInfo>::iterator Iter = std::find(PatternList.begin(), PatternList.end(), Brush);
  unsigned Index = (Iter - PatternList.begin()) + 1;
  if(Iter == PatternList.end())
  {
    PatternList.push_back(Brush);
    Stream << "  <defs>\n";
    Stream << "    <pattern id=\"pattern" << Index << "\" x=\"0\" y=\"0\" width=\"10\" height=\"10\" patternUnits=\"userSpaceOnUse\">\n";
    Stream << std::hex;
    if(Brush.Hatch == HS_FDIAGONAL || Brush.Hatch == HS_DIAGCROSS)
      Stream << "      <line stroke=\"#" << std::setw(6) << Brush.Color << "\" stroke-width=\"1\" x1=\"0\" y1=\"0\" x2=\"10\" y2=\"10\" />\n";
    if(Brush.Hatch == HS_BDIAGONAL || Brush.Hatch == HS_DIAGCROSS)
      Stream << "      <line stroke=\"#" << std::setw(6) << Brush.Color << "\" stroke-width=\"1\" x1=\"0\" y1=\"10\" x2=\"10\" y2=\"0\" />\n";
    if(Brush.Hatch == HS_VERTICAL || Brush.Hatch == HS_CROSS)
      Stream << "      <line stroke=\"#" << std::setw(6) << Brush.Color << "\" stroke-width=\"1\" x1=\"5\" y1=\"0\" x2=\"5\" y2=\"10\" />\n";
    if(Brush.Hatch == HS_HORIZONTAL || Brush.Hatch == HS_CROSS)
      Stream << "      <line stroke=\"#" << std::setw(6) << Brush.Color << "\" stroke-width=\"1\" x1=\"0\" y1=\"5\" x2=\"10\" y2=\"5\" />\n";
    Stream << "    </pattern>\n";
    Stream << "  </defs>\n";
    Stream << std::dec;
  }
  return Index;
}
//---------------------------------------------------------------------------
void TSvgWriter::ExcludeClipRect(const RECTL &Rect)
{
  Stream << "  </g>\n";
  Stream << "  <clipPath id=\"clippath\">\n";
  Stream << "    <path d=\"M" << ViewBox.left-1 << "," << ViewBox.top-1 << " H" << ViewBox.right+1 << " V"
    << ViewBox.bottom << "H" << ViewBox.top-1 << " z M" << Rect.left-1 << "," << Rect.top-1 << " H" << Rect.right+1 << " V"
    << Rect.bottom+1 << " H" << Rect.left-1 << " z\" clip-rule=\"evenodd\" />\n";
  Stream << "  </clipPath>\n";

  Stream << "  <g clip-path=\"url(#clippath)\">\n";
}
//---------------------------------------------------------------------------
#pragma argsused
void TSvgWriter::SetWindowMapping(SIZEL WindowSize, SIZEL ViewportSize, POINTL WindowOrg)
{
  Stream << "  <svg width=\"" << ViewportSize.cx << "\" height=\"" << ViewportSize.cy <<
    "\" viewBox=\"0 0 " << WindowSize.cx << " " << WindowSize.cy << "\">\r";
}
//---------------------------------------------------------------------------
void TSvgWriter::PaintRegion(const RECT *Rect, unsigned Count)
{
  Pen.Style = PS_NULL;
  Stream << "    <g stroke=\"none\" fill-rule=\"evenodd\"";
  WriteBrush();
  Stream << " >\n";
  while(Count-- > 0)
  {
    Stream << "      <rect x=\"" << Rect->left << "\" y=\"" << Rect->top << "\" width=\"" <<
      (Rect->right - Rect->left) << "\" height=\"" << (Rect->bottom - Rect->top) << "\" />\n";
    Rect++;
  }
  Stream << "    </g>\n";
}
//---------------------------------------------------------------------------
#pragma argsused
void TSvgWriter::FrameRegion(const RECT *Rect, unsigned Count, const TBrushInfo &Brush, unsigned xSize, unsigned ySize)
{
/*  Pen.Style = PS_NULL;
  Stream << "    <g fill=\"none\" stroke=\"#" << std::hex << std::setw(6) <<
    Brush.Color << std::dec << "\" stroke-width=\"" << xSize << "\" >\n";
  while(Count-- > 0)
  {
    Stream << "      <line x1=\"" << Rect->left << "\" y1=\"" << Rect->top <<
      "\" x2=\"" << Rect->left << "\" y2=\"" << Rect->bottom << "\" />\n";
    Stream << "      <line x1=\"" << Rect->right << "\" y1=\"" << Rect->top <<
      "\" x2=\"" << Rect->right << "\" y2=\"" << Rect->bottom << "\" />\n";
    Rect++;
  }
  Stream << "    </g>\n";*/
}
//---------------------------------------------------------------------------

