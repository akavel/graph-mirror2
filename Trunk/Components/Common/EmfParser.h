/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2008 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef EmfParserH
#define EmfParserH
#include <windows.h>
#include <string>
//---------------------------------------------------------------------------
class TGraphicWriter
{
public:
  virtual ~TGraphicWriter() {}
  virtual void BeginFile(int Width, int Height)=0;
  virtual void EndOfFile()=0;
  virtual void Line(int X1, int Y1, int X2, int Y2)=0;
  virtual void SetPen(int Style, int Width, int Color)=0;
  virtual void Polygon(const POINTS *Points, int Count)=0;
  virtual void Polyline(const POINTS *Points, int Count)=0;
  virtual void Rectangle(const RECTL &Rect)=0;
  virtual void Text(int X, int Y, const char *Str, int Size)=0;
  virtual void SetFont(const char *Name, int Size, int Color)=0;
};

class TEmfParser
{
  TGraphicWriter *Writer;
  POINTL Pos;

  static int CALLBACK EnhMetaFileProc(HDC hDC, HANDLETABLE *lpHTable, const ENHMETARECORD *lpEMFR, int nObj, LPARAM lpData);
  void HandleRecord(const ENHMETARECORD *lpEMFR);

public:
  TEmfParser();
  void Parse(HENHMETAFILE Handle, TGraphicWriter &AWriter);
  void Parse(const char *FileName, TGraphicWriter &AWriter);
};
#endif
