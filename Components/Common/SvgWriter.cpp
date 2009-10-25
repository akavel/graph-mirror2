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
    if(ch <= 0x7F)
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
void TSvgWriter::BeginFile(const RECTL &Rect, unsigned Width, unsigned Height)
{
  ViewBox = Rect;
  Stream <<
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
    "<svg " << "width=\"" << Width/1000.0 << "cm\" height=\"" << Height/1000.0 << "cm\" "  << "viewBox=\""
      << Rect.left << " " << Rect.top << " " << (Rect.right - Rect.left + 1) << " " << (Rect.bottom - Rect.top + 1)
      << "\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n";
  Stream << "  <g>\n";
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
  Stream << "    <line";
  WritePen();
  Stream << "x1=\"" << X1 << "\" y1=\"" << Y1 << "\" x2=\"" << X2 << "\" y2=\"" << Y2 << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Polyline(const POINTS *Points, int Count)
{
  Stream << "    <polyline";
  WritePen();
  WriteBrush(false);
  Stream << "points=\"";
  for(int I = 0; I < Count; I++)
    Stream << Points[I].x << "," << Points[I].y << " ";
  Stream << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Polygon(const POINTS *Points, int Count)
{
  Stream << "    <polygon";
  WritePen();
  WriteBrush();
  Stream << "points=\"";
  for(int I = 0; I < Count; I++)
    Stream << Points[I].x << "," << Points[I].y << " ";
  Stream << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Rectangle(const RECTL &Rect)
{
  Stream << "    <rect";
  WritePen();
  WriteBrush();
  Stream << "x=\"" << Rect.left << "\" y=\"" << Rect.top << "\" width=\"" <<
    (Rect.right - Rect.left) << "\" height=\"" << (Rect.bottom - Rect.top) << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Ellipse(const RECTL &Rect)
{
  bool Circle = Rect.right - Rect.left == Rect.bottom - Rect.top;
  Stream << (Circle ? "    <circle" : "    <ellipse");
  WritePen();
  WriteBrush();
  Stream << "cx=\"" << (Rect.right + Rect.left)/2 << "\" cy=\"" << (Rect.bottom + Rect.top)/2 << "\" ";
  if(Circle)
    Stream << "r=\"" << (Rect.right - Rect.left)/2;
  else
    Stream << "cx=\"" << (Rect.right - Rect.left)/2 << "\" cy=\"" << (Rect.bottom - Rect.top)/2;
  Stream << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Text(int X, int Y, const std::wstring &Str, const TFontInfo &Font)
{
  Stream << "    <text x=\"" << X << "\" y=\"" << Y << "\" font-family=\"" <<
    Font.Name << "\" font-size=\"" << Font.Size << "\">" << Utf8Encode(Str) << "</text>\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::WritePen()
{
  Stream << std::setfill('0');
  if((Pen.Style &  PS_STYLE_MASK) == PS_NULL)
    Stream << " stroke=\"none\" ";
  else
    Stream << " stroke=\"#" << std::hex << std::setw(6) << Pen.Color << "\" stroke-width=\"" << std::dec << Pen.Width << "\" ";

  switch(Pen.Style &  PS_STYLE_MASK)
  {
    case PS_DASH:
      Stream << "stroke-dasharray=\"5,5\" ";
      break;

    case PS_DOT:
      Stream << "stroke-dasharray=\"1,5\" stroke-linecap=\"round\" ";
      break;

    case PS_DASHDOT:
      Stream << "stroke-dasharray=\"5,5,1,5\" stroke-linecap=\"round\" ";
      break;

    case PS_DASHDOTDOT:
      Stream << "stroke-dasharray=\"5,5,5,5,1,5\" stroke-linecap=\"round\" ";
      break;

    case PS_SOLID:
    default:
      break;
  }
  Stream << std::dec;
}
//---------------------------------------------------------------------------
void TSvgWriter::WriteBrush(bool UseBrush)
{
  if(UseBrush)
    switch(Brush.Style)
    {
      case BS_NULL:
        Stream << "fill=\"none\" ";
        break;

      case BS_HATCHED:
        Stream << "fill=\"url(#pattern" << (1+(std::find(PatternList.begin(), PatternList.end(), Brush) - PatternList.begin())) << ")\" ";
        break;
        
      case BS_SOLID:
      default:
        Stream << "fill=\"#" << std::hex << std::setw(6) << Brush.Color << "\" ";
    }
  else
    Stream << "fill=\"none\" ";
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
    Stream << std::hex << std::setfill('0');
    if(Brush.Hatch == HS_FDIAGONAL || Brush.Hatch == HS_DIAGCROSS)
      Stream << "      <line stroke=\"#" << std::setw(6) << Brush.Color << "\" stroke-width=\"1\" x1=\"0\" y1=\"0\" x2=\"10\" y2=\"10\" />\n";
    if(Brush.Hatch == HS_BDIAGONAL || Brush.Hatch == HS_DIAGCROSS)
      Stream << "      <line stroke=\"#" << std::setw(6) << Brush.Color << "\" stroke-width=\"1\" x1=\"0\" y1=\"10\" x2=\"10\" y2=\"0\" />\n";
    if(Brush.Hatch == HS_VERTICAL || Brush.Hatch == HS_CROSS)
      Stream << "      <line stroke=\"#" << std::setw(6) << Brush.Color << "\" stroke-width=\"1\" x1=\5\" y1=\"0\" x2=\"5\" y2=\"10\" />\n";
    if(Brush.Hatch == HS_HORIZONTAL || Brush.Hatch == HS_CROSS)
      Stream << "      <line stroke=\"#" << std::setw(6) << Brush.Color << "\" stroke-width=\"1\" x1=\0\" y1=\"5\" x2=\"10\" y2=\"5\" />\n";
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
  Stream << "    <path d=\"M" << ViewBox.left << "," << ViewBox.top << " H" << ViewBox.right << " V"
    << ViewBox.bottom << "H" << ViewBox.top << " z M" << Rect.left << "," << Rect.top << " H" << Rect.right << " V"
    << Rect.bottom << " H" << Rect.left << " z\" clip-rule=\"evenodd\" />\n";
  Stream << "  </clipPath>\n";

  Stream << "  <g clip-path=\"url(#clippath)\">\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::SetWindowMapping(SIZEL WindowSize, SIZEL ViewportSize, POINTL WindowOrg)
{
  Stream << "  <svg width=\"" << ViewportSize.cx << "\" height=\"" << ViewportSize.cy <<
    "\" viewBox=\"0 0 " << WindowSize.cx << " " << WindowSize.cy << "\">\r";
}
//---------------------------------------------------------------------------

