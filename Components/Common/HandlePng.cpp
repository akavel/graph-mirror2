/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Config.h"
#pragma hdrstop
#include <windows.h>
#include "HandlePng.h"
#include <memory>
#pragma warn -8022 //Disable warning in gdiplus.h
#include <gdiplus.h>
//---------------------------------------------------------------------------
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

   Gdiplus::GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}
//---------------------------------------------------------------------------
bool CheckGdiPlus()
{
  static int Result = -1;
  if(Result == -1)
  {
    HINSTANCE hGDIPlus = LoadLibrary(L"Gdiplus.dll");
    Result = hGDIPlus != NULL;
    FreeLibrary(hGDIPlus);
  }
  return Result;
}
//---------------------------------------------------------------------------
bool SaveBitmapToPNGFile(HBITMAP HBitmap, const wchar_t *FileName)
{
  if(!CheckGdiPlus())
    return false;

  // Initialize GDI+.
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  CLSID   encoderClsid;
  std::auto_ptr<Gdiplus::Bitmap> Bitmap(new Gdiplus::Bitmap(HBitmap, NULL));
  // Get the CLSID of the PNG encoder.
  GetEncoderClsid(L"image/png", &encoderClsid);
  Gdiplus::Status Stat = Bitmap->Save(FileName, &encoderClsid, NULL);
  Bitmap.reset(); //Bitmap must be deleted before GDI+ is shut down
  Gdiplus::GdiplusShutdown(gdiplusToken);
  return Stat == Gdiplus::Ok;
}
//---------------------------------------------------------------------------
bool SaveBitmapToPngStream(HBITMAP hBitmap, IStream *Stream)
{
  if(!CheckGdiPlus())
    return false;

  // Initialize GDI+.
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  CLSID   encoderClsid;
  std::auto_ptr<Gdiplus::Bitmap> Bitmap(new Gdiplus::Bitmap(hBitmap, NULL));
  // Get the CLSID of the PNG encoder.
  GetEncoderClsid(L"image/png", &encoderClsid);
  Gdiplus::Status Stat = Bitmap->Save(Stream, &encoderClsid, NULL);
  Bitmap.reset(); //Bitmap must be deleted before GDI+ is shut down
  Gdiplus::GdiplusShutdown(gdiplusToken);
  return Stat == Gdiplus::Ok;
}
//---------------------------------------------------------------------------

