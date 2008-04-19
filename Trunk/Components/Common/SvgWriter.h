/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef SvgWriterH
#define SvgWriterH
#include "EmfParser.h"
#include <fstream>
//---------------------------------------------------------------------------
class TSvgWriter : public TGraphicWriter
{
  std::ostream &Stream;
  std::string FontName;
  int FontSize;
  int FontColor;

public:
  TSvgWriter(std::ostream &AStream) : Stream(AStream) {}
  void BeginFile(int Width, int Height);
  void EndOfFile();
  void Line(int X1, int Y1, int X2, int Y2);
  void SetPen(int Style, int Width, int Color);
  void Polygon(const POINTS *Points, int Count);
  void Polyline(const POINTS *Points, int Count);
  void Rectangle(const RECTL &Rect);
  void Text(int X, int Y, const char *Str, int Size);
  void SetFont(const char *Name, int Size, int Color);
};
#endif
