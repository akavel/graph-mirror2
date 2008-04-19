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
//---------------------------------------------------------------------------
void TSvgWriter::BeginFile(int Width, int Height)
{
  Stream <<
    "<?xml version=\"1.0\" standalone=\"no\"?>\n"
    "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
    "<svg width=\"" << Width << "\" height=\"" << Height << "\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::EndOfFile()
{
  Stream << "</svg>\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Line(int X1, int Y1, int X2, int Y2)
{
  Stream << "  <line x1=\"" << X1 << "\" y1=\"" << Y1 << "\" x2=\"" << X2 << "\" y2=\"" << Y2 << "\" stroke=\"blue\" stroke-width=\"2\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::SetPen(int Style, int Width, int Color)
{

}
//---------------------------------------------------------------------------
void TSvgWriter::Polyline(const POINTS *Points, int Count)
{
  Stream << "  <polyline stroke=\"red\" stroke-width=\"1\" fill=\"none\" points=\"";
  for(int I = 0; I < Count; I++)
    Stream << Points[I].x << "," << Points[I].y << " ";
  Stream << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Polygon(const POINTS *Points, int Count)
{
  Stream << "  <polygon points=\"";
  for(int I = 0; I < Count; I++)
    Stream << Points[I].x << "," << Points[I].y << " ";
  Stream << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Rectangle(const RECTL &Rect)
{
  Stream << "  <rect fill=\"none\" stroke=\"blue\" stroke-width=\"1\" x=\"" << Rect.left << "\" y=\"" << Rect.top << "\" width=\"" <<
    (Rect.right - Rect.left) << "\" height=\"" << (Rect.bottom - Rect.top) << "\" />\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::Text(int X, int Y, const char *Str, int Size)
{
  Stream << "  <text x=\"" << X << "\" y=\"" << Y << "\" font-family=\"" <<
    FontName << "\" font-size=\"" << Size << "\">" << Str << "</text>\n";
}
//---------------------------------------------------------------------------
void TSvgWriter::SetFont(const char *Name, int Size, int Color)
{
  FontName = Name;
  FontSize = Size;
  FontColor = Color;
}
//---------------------------------------------------------------------------

