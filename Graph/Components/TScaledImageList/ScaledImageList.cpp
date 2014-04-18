//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "ScaledImageList.h"
#pragma package(smart_init)
#pragma warn -8022 //Remove warning in gdiplus.h
#include "gdiplus.h"
#pragma warn .8022 //Set warning back to default
#pragma link "gdiplus.lib"
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TScaledImageList *)
{
  new TScaledImageList(NULL);
}
//---------------------------------------------------------------------------
namespace Scaledimagelist
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TScaledImageList)};
     RegisterComponents(L"IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
__fastcall TScaledImageList::TScaledImageList(TComponent* Owner)
  : TCustomImageList(Owner), FImages(NULL), FWidth(16), FHeight(16)
{
  Gdiplus::GdiplusStartupInput StartupInput;
  Gdiplus::GdiplusStartup(&Token, &StartupInput, NULL);
}
//---------------------------------------------------------------------------
__fastcall TScaledImageList::~TScaledImageList()
{
  Gdiplus::GdiplusShutdown(Token);
}
//---------------------------------------------------------------------------
void TScaledImageList::RecreateImageList()
{
  if(Images == NULL || Images->ColorDepth != cd32Bit)
    return;

  if(Images->Width == Width && Images->Height == Height)
    Assign(Images);

  BITMAP bm;
  Win32Check(GetObject(Images->GetImageBitmap(), sizeof(BITMAP), &bm));
  BYTE* sourceBytes = static_cast<BYTE*>(bm.bmBits);
  if(sourceBytes == NULL)
    return;

  Gdiplus::Bitmap B(bm.bmWidth, bm.bmHeight, PixelFormat32bppARGB);

  // Get access to the Gdiplus::Bitmap's pixel data
  Gdiplus::BitmapData bmd;
  Gdiplus::Rect R2(0, 0, bm.bmWidth, bm.bmHeight);
  B.LockBits(&R2, Gdiplus::ImageLockMode::ImageLockModeWrite, PixelFormat32bppARGB, &bmd);

  // Copy source pixel data to destination Bitmap (one is 'upside down' relative to the other)
  int lineSize = bm.bmWidth * sizeof(DWORD);
  byte* destBytes = static_cast<BYTE*>(bmd.Scan0);
  for(int y = 0; y < bm.bmHeight; y++)
    memcpy(destBytes + (y * lineSize), sourceBytes + ((bm.bmHeight - y - 1) * lineSize), lineSize);
  B.UnlockBits(&bmd);

//  CLSID pngClsid;
//  GetEncoderClsid(L"image/png", &pngClsid);
//  B.Save(L"C:\\Users\\Ivan Johansen\\Documents\\test.png", &pngClsid, NULL);

  int Count = Images->Count;
  int OldWidth = Images->Width;
  int OldHeight = Images->Height;
  ColorDepth = cd32Bit;

  Gdiplus::Bitmap B2(Width, Height, PixelFormat32bppARGB);
  Gdiplus::Graphics G(&B2);
//  G.SetInterpolationMode(Gdiplus::InterpolationModeHighQuality);
  Gdiplus::RectF R(0, 0, Width, Height);
  Gdiplus::Color ClearColor(0, 0, 0, 0);
  for(int I = 0; I < Count; I++)
  {
    G.Clear(ClearColor);
    //Subtract 0.5 to prevent some the top of the next icon to blend into this
    G.DrawImage(&B, R, 0, OldHeight * I, OldWidth, OldHeight - 0.5, Gdiplus::UnitPixel);
//    B2.Save((L"C:\\Users\\Ivan Johansen\\Documents\\test" + IntToStr(I) + ".png").c_str(), &pngClsid, NULL);
    HBITMAP H;
    B2.GetHBITMAP(ClearColor, &H);
    ImageList_Add(reinterpret_cast<HIMAGELIST>(Handle), H, NULL);
  }
}
//---------------------------------------------------------------------------
void __fastcall TScaledImageList::SetWidth(int Value)
{
  TCustomImageList::Width = Value;
  FWidth = Value;
  RecreateImageList();
}
//---------------------------------------------------------------------------
void __fastcall TScaledImageList::SetHeight(int Value)
{
  TCustomImageList::Height = Value;
  FHeight = Value;
  RecreateImageList();
}
//---------------------------------------------------------------------------
void TScaledImageList::SetSize(int W, int H)
{
  TCustomImageList::SetSize(W, H);
  FWidth = W;
  FHeight = H;
  RecreateImageList();
}
//---------------------------------------------------------------------------
void __fastcall TScaledImageList::SetImages(TCustomImageList *Value)
{
  FImages = Value;
  RecreateImageList();
}
//---------------------------------------------------------------------------
