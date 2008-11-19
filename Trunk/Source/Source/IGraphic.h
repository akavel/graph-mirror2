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
int CountColors(Graphics::TBitmap *Bitmap);
void GetColors(Graphics::TBitmap *Bitmap, const TRect &Rect, std::vector<RGBQUAD> &Colors);
void CompressBitmap(Graphics::TBitmap *Bitmap, const TRect &Rect, std::vector<RGBQUAD> &Colors, std::vector<char> &Data);
bool SaveCompressedBitmap(Graphics::TBitmap *Bitmap, const TRect &Rect, const String &FileName);
void FillBitmapInfoHeader(BITMAPINFOHEADER &BitmapHeader, Graphics::TBitmap *Bitmap, const TRect &Rect, unsigned Colors, unsigned DataSize);

void RenderRichText(const String &Str, TCanvas *Canvas, const TPoint &Pos, int width = MAXSHORT, TColor BackgroundColor = Graphics::clNone, const TData *Data = NULL);
TPoint RichTextSize(const std::string &Str, const TData *Data = NULL);

void Interpolate(std::vector<TPoint> &Points, const std::vector<TPoint> &P, TInterpolationAlgorithm Algorithm);
double InterpolationAngle(Func32::TDblPoint p1, Func32::TDblPoint p2, Func32::TDblPoint p3, Func32::TDblPoint p4, TInterpolationAlgorithm Algorithm);

TRect Rotate(TMetafile *Metafile, int Degrees);

void InternalGetDIBSizes(HBITMAP Bitmap, int &InfoHeaderSize, int &ImageSize, TPixelFormat PixelFormat);
bool InternalGetDIB(HBITMAP Bitmap, HPALETTE Palette, void *BitmapInfo, void *Bits, TPixelFormat PixelFormat);

#endif
