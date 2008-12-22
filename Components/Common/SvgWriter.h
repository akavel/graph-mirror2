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
#include <vector>
//---------------------------------------------------------------------------
class TSvgWriter : public TGraphicWriter
{
  std::ostream &Stream;
  RECTL ViewBox;
  TPenInfo Pen;
  TBrushInfo Brush;
  std::vector<TBrushInfo> PatternList;

  void WritePen();
  void WriteBrush(bool UseBrush=true);
  unsigned CreatePattern();

public:
  TSvgWriter(std::ostream &AStream) : Stream(AStream) {}
  void BeginFile(const RECTL &Rect, unsigned Width, unsigned Height);
  void EndOfFile();
  void Line(int X1, int Y1, int X2, int Y2);
  void Polygon(const POINTS *Points, int Count);
  void Polyline(const POINTS *Points, int Count);
  void Rectangle(const RECTL &Rect);
  void Ellipse(const RECTL &Rect);
  void Text(int X, int Y, const std::wstring &Str, const TFontInfo &Font);
  void SetPen(const TPenInfo &APen);
  void SetBrush(const TBrushInfo &ABrush);
  void ExcludeClipRect(const RECTL &Rect);
  void SetWindowMapping(SIZEL WindowSize, SIZEL ViewportSize, POINTL WindowOrg);
};
#endif