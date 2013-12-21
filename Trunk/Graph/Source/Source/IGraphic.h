/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef IGraphicH
#define IGraphicH
#include <vector>
//---------------------------------------------------------------------------
void RenderRichText(const String &Str, TCanvas *Canvas, const TPoint &Pos, int width = MAXSHORT, TColor BackgroundColor = Graphics::clNone);
TPoint RichTextSize(const String &Str);
String UpdateRichTextOleObjects(const String &Str);
String ReplaceExpression(const String &Str, const TData &Data);

void Interpolate(std::vector<TPoint> &Points, const std::vector<TPoint> &P, TInterpolationAlgorithm Algorithm);
double InterpolationAngle(Func32::TDblPoint p1, Func32::TDblPoint p2, Func32::TDblPoint p3, Func32::TDblPoint p4, TInterpolationAlgorithm Algorithm);

TRect Rotate(TMetafile *Metafile, int Degrees);

void InternalGetDIBSizes(HBITMAP Bitmap, int &InfoHeaderSize, int &ImageSize, TPixelFormat PixelFormat);
bool InternalGetDIB(HBITMAP Bitmap, HPALETTE Palette, void *BitmapInfo, void *Bits, TPixelFormat PixelFormat);

#endif
