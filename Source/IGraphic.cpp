/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "ComCtrls.hpp"
#include "IGraphic.h"
#include "IRichEdit.h"
#include "RichEditOle.h"
#include <fstream>
#include <algorithm>
#include <Jpeg.hpp>

//---------------------------------------------------------------------------
inline bool operator==(RGBQUAD Color1, RGBQUAD Color2)
{
  return *reinterpret_cast<unsigned*>(&Color1) == *reinterpret_cast<unsigned*>(&Color2);
}
//---------------------------------------------------------------------------
inline bool operator!=(RGBQUAD Color1, RGBQUAD Color2)
{
  return *reinterpret_cast<unsigned*>(&Color1) != *reinterpret_cast<unsigned*>(&Color2);
}
//---------------------------------------------------------------------------

//==  CountColors  =====================================================
// Count number of unique R-G-B triples in a pf24bit Bitmap.
//
// Use 2D array of TBits objects -- when (R,G) combination occurs
// for the first time, create 256-bit array of bits in blue dimension.
// So, overall this is a fairly sparse matrix for most pictures.
// Tested with pictures created with a known number of colors, including
// a specially constructed image with 1024*1024 = 1,048,576 colors.
struct pRGBTripleArray
{
  unsigned char rgbtRed;
  unsigned char rgbtGreen;
  unsigned char rgbtBlue;
};

int CountColors(Graphics::TBitmap *Bitmap)
{
  int i;
  int j;
  int k;
  pRGBTripleArray *rowIn;
  TBits* Flags[256][256];

  // Be sure bitmap is 24-bits/pixel
  if(Bitmap->PixelFormat != pf24bit)
    return -1;

  // Clear 2D array of TBits objects
  for(j = 0; j <= 255;j++)
    for(i = 0;i <= 255;i++)
      Flags[i][j] = NULL;

  // Step through each scanline of image
  for(j = 0;j < Bitmap->Height;j++)
  {
    rowIn = (pRGBTripleArray*)Bitmap->ScanLine[j];
    for(i = 0;i < Bitmap->Width;i++)
    {
      if(!Flags[rowIn[i].rgbtRed][rowIn[i].rgbtGreen])
      {
          // Create 3D column when needed
          Flags[rowIn[i].rgbtRed][rowIn[i].rgbtGreen] = new TBits;
          Flags[rowIn[i].rgbtRed][rowIn[i].rgbtGreen]->Size = 256;
      }

        // Mark this R-G-B triple
        Flags[rowIn[i].rgbtRed][rowIn[i].rgbtGreen]->Bits[rowIn[i].rgbtBlue] = true;
    }
  }

  int RESULT = 0;
  // Count and Free TBits objects
  for(j = 0;j <= 255;j++)
    for(i = 0;i<= 255;i++)
      if(Flags[i,j])
        if(Flags[i][j])
        {
          for(k = 0;k <= 255;k++)
            if(Flags[i][j]->Bits[k])
              ++RESULT;
          delete Flags[i,j];
        }
  return RESULT;
}
//---------------------------------------------------------------------------
void GetColors(Graphics::TBitmap *Bitmap, const TRect &Rect, std::vector<RGBQUAD> &Colors)
{
  Colors.clear();
  Bitmap->HandleType = bmDIB;
  Bitmap->PixelFormat = pf32bit;
  RGBQUAD LastColor = {0xFF, 0xFF, 0xFF, 0xFF}; //Used as cache

  for(int Row = Rect.Top; Row < Rect.Bottom; Row++)
  {
    RGBQUAD *ScanLine = static_cast<RGBQUAD*>(Bitmap->ScanLine[Row]);
    for(int Col = Rect.Left; Col < Rect.Right; Col++)
      if(!(ScanLine[Col] == LastColor) && std::find(Colors.begin(), Colors.end(), ScanLine[Col]) == Colors.end())
      {
        LastColor = ScanLine[Col]; //Store in cache
        Colors.push_back(ScanLine[Col]);
      }
  }
}
//---------------------------------------------------------------------------
bool SaveCompressedBitmap(Graphics::TBitmap *Bitmap, const TRect &Rect, const AnsiString &FileName)
{
  BITMAPFILEHEADER FileHeader;
  BITMAPINFOHEADER BitmapHeader;
  std::vector<RGBQUAD> Colors;
  std::vector<char> Data; //Area with compressed bitmap data
  CompressBitmap(Bitmap, Rect, Colors, Data);
  FillBitmapInfoHeader(BitmapHeader, Bitmap, Rect, Colors.size(), Data.size());

  FileHeader.bfType = 0x4D42; //Initialize file header; must be 'BM'; Remember little endian
  FileHeader.bfSize = sizeof(FileHeader)+sizeof(BitmapHeader)+Colors.size()*sizeof(RGBQUAD)+Data.size(); //File size
  FileHeader.bfReserved1 = 0;
  FileHeader.bfReserved2 = 0;
  FileHeader.bfOffBits = sizeof(FileHeader)+sizeof(BitmapHeader)+Colors.size()*sizeof(RGBQUAD);//Offset to compressed data

  //Create new text file; Erase if exists
  std::ofstream out(FileName.c_str(),std::ios::out|std::ios::binary);
  //If open not succesfull
  if(!out)
    return false;
  out.write((char*)&FileHeader, sizeof(FileHeader)); //Save Save file header
  out.write((char*)&BitmapHeader, sizeof(BitmapHeader)); //Save bitmap header
  out.write((char*)&Colors[0], Colors.size()*sizeof(TColor)); //Save palette
  out.write((char*)&Data[0], Data.size()); //Save compressed data
  out.close(); //Close file
  return true; //Return sucess
}
//---------------------------------------------------------------------------
void FillBitmapInfoHeader(BITMAPINFOHEADER &BitmapHeader, Graphics::TBitmap *Bitmap, const TRect &Rect, unsigned Colors, unsigned DataSize)
{
  BitmapHeader.biSize = sizeof(BitmapHeader);  //Size of header
  BitmapHeader.biWidth = Rect.Width();         //Width of bitmap
  BitmapHeader.biHeight = Rect.Height();       //Height of bitmap
  BitmapHeader.biPlanes = 1;                   //Planes; Must be 1
  BitmapHeader.biBitCount = 8;                 //Color depth; Bits per pixel
  BitmapHeader.biCompression = BI_RLE8;        //Compression format
  BitmapHeader.biSizeImage = DataSize;       //Size of bitmap in bytes
  //Calculate the x- and y-resolution in pixels per meter
  BitmapHeader.biXPelsPerMeter = GetDeviceCaps(Bitmap->Canvas->Handle,HORZRES)*1000/GetDeviceCaps(Bitmap->Canvas->Handle,HORZSIZE);
  BitmapHeader.biYPelsPerMeter = GetDeviceCaps(Bitmap->Canvas->Handle,VERTRES)*1000/GetDeviceCaps(Bitmap->Canvas->Handle,VERTSIZE);
  BitmapHeader.biClrUsed = Colors;      //Used indexes in the color table
  BitmapHeader.biClrImportant = Colors; //Required indexes
}
//---------------------------------------------------------------------------
unsigned AddToPalette(RGBQUAD Color, std::vector<RGBQUAD> &Colors)
{
  std::vector<RGBQUAD>::iterator Iter = std::find(Colors.begin(), Colors.end(), Color);
  if(Iter == Colors.end())
  {
    Colors.push_back(Color);
    return Colors.size() - 1;
  }
  return Iter - Colors.begin();
}
//---------------------------------------------------------------------------
//Colors must be sorted
//Notice that the Colors are not real TColor values, as DIB use inverted colors.
void CompressBitmap(Graphics::TBitmap *Bitmap, const TRect &Rect, std::vector<RGBQUAD> &Colors, std::vector<char> &Data)
{
  Bitmap->HandleType = bmDIB;
  Bitmap->PixelFormat = pf32bit;

  Data.clear();

  //Loop through scanlines from bottom to top
  for(int y = Rect.Bottom - 1; y >= Rect.Top; y--)
  {
    const RGBQUAD *ScanLine = static_cast<RGBQUAD*>(Bitmap->ScanLine[y]); //Get pointer to scanline
    unsigned Count = 1;  //Number of equal pixels following each other
    RGBQUAD Color = ScanLine[Rect.Left]; //Get color of first pixel in scanline
    unsigned ColorIndex = AddToPalette(Color, Colors);

    //Loop through pixels in scanline
    for(int x = Rect.Left + 1; x < Rect.Right; x++)
      //Check if we are in a block of equal colors
      if(ScanLine[x] == Color && Count < 255)
        Count++;
      else
      {
        //No more equal colors
        Data.push_back(Count);      //Add length of equal pixels
        Data.push_back(ColorIndex); //Add the color
        Count = 1;                  //Set count to 1; The one we are looking at now
        Color = ScanLine[x];        //Save the color
        ColorIndex = AddToPalette(Color, Colors);
      }
    Data.push_back(Count);          //Add length of last block of equal colors
    Data.push_back(ColorIndex);     //Add the color
    Data.push_back(0);              //Add escape sequense telling:
    Data.push_back(0);              //End of scanline
  }
  Data.push_back(0);                //Add escape sequense telling:
  Data.push_back(1);                //End of bitmap
}
//---------------------------------------------------------------------------
unsigned FindEndPar(const std::string &Str, unsigned Pos)
{
  unsigned Count = 0;
  for(unsigned I = Pos; I <= Str.size(); I++)
    if(Str[I] == '(')
      Count++;
    else if(Str[I] == ')')
      if(--Count == 0)
        return I;
  return std::string::npos;
}
//---------------------------------------------------------------------------
void ReplaceExpression(TIRichEdit *RichEdit, const TData &Data)
{
  std::string Str = RichEdit->Text.c_str();
  for(int I = Str.size()-1; I >=0; I--)
    if(Str[I] == '\r')
      Str.erase(I, 1);

  unsigned Pos = std::string::npos;
  while((Pos = Str.rfind("%(", Pos-1)) != std::string::npos)
  {
    unsigned Pos2 = FindEndPar(Str, Pos);
    if(Pos2 == std::string::npos)
      return;
    unsigned Length = Pos2 - Pos + 1;

    try
    {
      std::string Expression = Str.substr(Pos+2, Pos2-Pos-2);
      bool UseReal = Form1->Data.Property.ComplexFormat == cfReal;
      WideString Value = ComplexToWideString(UseReal ? Func32::TComplex(Data.Calc(Expression)) : Data.CalcComplex(Expression));

      //If %() is preceded by a '+' and the value is negative, the '+' will be removed to avoid such as "2x+-3"
      if(Value[1] == '-' && Pos > 0 && Str[Pos - 1] == '+')
      {
        Pos--;
        Length++;
      }

      RichEdit->SelStart = Pos;
      RichEdit->SelLength = Length;
      RichEdit->SelText = Value;                 
    }
    catch(Func32::EFuncError &Error)
    {
      WideString ErrorMessage = GetErrorMsg(Error);
      RichEdit->SelStart = Pos;
      RichEdit->SelLength = Length;
      RichEdit->SelText = ErrorMessage;

      RichEdit->SelStart = Pos;
      RichEdit->SelLength = ErrorMessage.Length();
      RichEdit->TextFormat.SetBold(true);
      RichEdit->TextFormat.SetColor(clRed);
    }

    if(Pos == 0)
      break; //Special case
  }
}
//---------------------------------------------------------------------------
void RenderRichText(const AnsiString &Str, TCanvas *Canvas, const TPoint &Pos, int Width, TColor BackgroundColor, const TData *Data)
{
  //We need a parent window; just use main form
  std::auto_ptr<TIRichEdit> RichEdit(new TIRichEdit(Application->MainForm));
  RichEdit->Visible = false;
  RichEdit->Transparent = BackgroundColor == clNone;
  RichEdit->Parent = Application->MainForm;
  RichEdit->WordWrap = false;
  RichEdit->BackgroundColor = BackgroundColor;
  TRichEditOle RichEditOle(RichEdit.get());

  RichEdit->SetRichText(Str);
  if(Data)
    ReplaceExpression(RichEdit.get(), *Data);
  RichEdit->Render(Canvas, Pos, Width);
}
//---------------------------------------------------------------------------
TPoint RichTextSize(const std::string &Str, const TData *Data)
{
  //We need a parent window; just use main form
  std::auto_ptr<TIRichEdit> RichEdit(new TIRichEdit(Application->MainForm));
  RichEdit->Visible = false;
  RichEdit->Transparent = true;
  RichEdit->WordWrap = false; //Must be false for LineIndex() to find the correct line

  RichEdit->Parent = Application->MainForm;
  TRichEditOle RichEditOle(RichEdit.get());
  RichEdit->SetRichText(Str.c_str());
  if(Data)
    ReplaceExpression(RichEdit.get(), *Data);

  //We need to add a space to the end of each lines. Else the width will be too
  //small if the line ends with an italic character
  int LineCount = RichEdit->LineCount();
  for(int I = 0; I < LineCount; I++)
  {
    int Start = RichEdit->LineIndex(I);
    RichEdit->SelStart = Start + RichEdit->LineLength(Start);
    RichEdit->SelText = " ";
  }

  //We need to add an empty line to the end, because GetTextSize doesn't count
  //the last line in the height
  RichEdit->SelText = "\n";

  //We need to left align the text because there is no specific right side
  RichEdit->SelectAll();
  RichEdit->Paragraph->Alignment = pfaLeft;

  return RichEdit->GetTextSize();
}
//---------------------------------------------------------------------------
struct TSplineParam
{
  TPoint Param[4];
};

TSplineParam CalcSingleSpline(TPoint p1, TPoint p2, TPoint p3, TPoint p4)
{
  TSplineParam Spline;
  Spline.Param[0].x = -p1.x + 3*p2.x -3*p3.x + p4.x;
  Spline.Param[1].x = 2*p1.x -5*p2.x + 4*p3.x - p4.x;
  Spline.Param[2].x = -p1.x+p3.x;
  Spline.Param[3].x = 2*p2.x;
  Spline.Param[0].y = -p1.y + 3*p2.y -3*p3.y + p4.y;
  Spline.Param[1].y = 2*p1.y -5*p2.y + 4*p3.y - p4.y;
  Spline.Param[2].y = -p1.y+p3.y;
  Spline.Param[3].y = 2*p2.y;
  return Spline;
}
//---------------------------------------------------------------------------
//Calculates points used to draw a cubic spline
//The spline will go from p2 to p3; p1 and p4 are used as control points
void CreateSingleSpline(std::vector<TPoint> &Points, TPoint p1, TPoint p2, TPoint p3, TPoint p4)
{
  //If the two points are identical there is nothing to draw, and we prevent
  //a division by zero in calculation of dt.
  if(p2.x == p3.x && p2.y == p3.y)
    return;

  TPoint q;
  //Find step point from the horizontal or vertical differense in the points
  double dt = 2.0/std::max(abs(p2.x-p3.x), abs(p2.y-p3.y));

  TSplineParam Spline = CalcSingleSpline(p1, p2, p3, p4);

  //Calculate points on spline
  for(double t = 0; t < 1; t += dt)
  {
    //Add 0.5 to take care of rounding error. We assume the coordinates are always positive.
    q.x = 0.5 * (Spline.Param[0].x * t*t*t
               + Spline.Param[1].x * t*t
               + Spline.Param[2].x * t
               + Spline.Param[3].x) + 0.5;
    q.y = 0.5 * (Spline.Param[0].y * t*t*t
               + Spline.Param[1].y * t*t
               + Spline.Param[2].y * t
               + Spline.Param[3].y) + 0.5;

    //No need to add point if it is equal to the last one
    if(Points.empty() || q != Points.back())
      Points.push_back(q);
  }
}
//---------------------------------------------------------------------------
//Calculate points used to draw a series of cubic splines between points in P
void CreateCubicSplines(std::vector<TPoint> &Points, const std::vector<TPoint> &P)
{
  if(P.size() == 2)
  {
    //Draw a straight line if only two points exists
    Points.push_back(P.front());
    Points.push_back(P.back());
  }

  if(P.size() <= 2)
    return;

  std::vector<TPoint>::const_iterator p1 = P.begin();
  std::vector<TPoint>::const_iterator p2 = P.begin();
  std::vector<TPoint>::const_iterator p3 = p2 + 1;
  std::vector<TPoint>::const_iterator p4 = p3 + 1;

  //Clear points, calculate the number of points needed to draw the splines and
  //allocate memory
  Points.clear();
  int PointNum = 0;
  for(unsigned I = 1; I < P.size(); I++)
    PointNum += std::max(abs(P[I-1].x-P[I].x), abs(P[I-1].y-P[I].y));
  Points.reserve(PointNum);

  //Draw the first spline from p2 to p3. The first point is also used as the first control point
  if(P.front() == P.back()) //WARNING: Don't use operator ?:  because of code generation bug in Bcc 5.6.4
    p1 = P.end()-2;
  CreateSingleSpline(Points, *p1, *p2++, *p3++, *p4++);
  p1 = P.begin();

  //Draw splines from p2 to p3. The point before and the point after are used as control points
  for(unsigned n = 0; n < P.size() - 3; n++)
    CreateSingleSpline(Points, *p1++, *p2++, *p3++, *p4++);

  //Draw the last spline. The last point is also used as the last control point
  if(P.front() == P.back()) //WARNING: Don't use operator ?: beacuse of code generation bug in Bcc 5.6.4
    p4 = P.begin()+1;
  else
    p4 = p3;
  CreateSingleSpline(Points, *p1, *p2, *p3, *p4);
}
//---------------------------------------------------------------------------
// vt = (v1 + v2)/2 + (v1 - v2)/2 * cos(pi * (t - t1)/(t2-t1))
// where, vt is the value you're looking for (at time t), v1 and v2 are the start and end values,
// t1 and t2 are the start and end times.
// half-cosine interpolation
enum TCurveDirection {cdHorizontal, cdVertical};
TCurveDirection CreateSingleHalfCosine(std::vector<TPoint> &Points, TCurveDirection Direction, TPoint p2, TPoint p3, TPoint p4)
{
  //Continue horizontal if the points don't start to move backwards
  if((Direction == cdHorizontal && p2.x < p3.x && p3.x <= p4.x) || (Direction == cdHorizontal && p2.x > p3.x && p3.x >= p4.x))
  {
    bool Sign = p2.x < p3.x;
    for(double t = p2.x; (t <= p3.x) == Sign; Sign ? t++ : t--)
      Points.push_back(TPoint(t, (p2.y + p3.y)/2 + (p2.y - p3.y)/2 * std::cos(M_PI * (t - p2.x)/(p3.x - p2.x))));
    return cdHorizontal;
  }
  //Continue vertical if the points don't change up/down direction
  else if((Direction == cdVertical && p2.y < p3.y && p3.y <= p4.y) || (Direction == cdVertical && p2.y > p3.y && p3.y >= p4.y))
  {
    bool Sign = p2.y < p3.y;
    for(double t = p2.y; (t <= p3.y) == Sign; Sign ? t++ : t--)
      Points.push_back(TPoint((p2.x + p3.x)/2 + (p2.x - p3.x)/2 * std::cos(M_PI * (t - p2.y)/(p3.y - p2.y)), t));
    return cdVertical;
  }
  else
  {
    //Draw an quarter ellipsis if we need to change horizontal/vertical direction
    double dt = M_PI/std::max(abs(p2.x-p3.x), abs(p2.y-p3.y));
    for(double t = 0; t <= M_PI/2; t += dt)
    {
      TPoint q;
      if(Direction == cdHorizontal)
      {
        q.x = p2.x + (p3.x-p2.x) * std::sin(t);
        q.y = p3.y - (p3.y-p2.y) * std::cos(t);
      }
      else
      {
        q.x = p3.x - (p3.x-p2.x) * std::cos(t);
        q.y = p2.y + (p3.y-p2.y) * std::sin(t);
      }
      if(Points.empty() || Points.back() != q)
        Points.push_back(q);
    }
    return Direction == cdHorizontal ? cdVertical : cdHorizontal;
  }
}
//---------------------------------------------------------------------------
void CreateHalfCosineInterpolation(std::vector<TPoint> &Points, const std::vector<TPoint> &P)
{
  //Start the curve horizontally; This might not always be the best but works in most cases
  TCurveDirection Direction = cdHorizontal;
  if(P.size()>2)
    Direction = CreateSingleHalfCosine(Points, Direction, P[0], P[1], P[2]);
  for(unsigned n = 1; n < P.size() - 2; n++)
    Direction = CreateSingleHalfCosine(Points, Direction, P[n], P[n+1], P[n+2]);
  if(P.size()>2)
    Direction = CreateSingleHalfCosine(Points, Direction, *(P.end()-2), P.back(), P.back());
}
//---------------------------------------------------------------------------
//Calculate points used to draw a series of cubic splines between points in P
void Interpolate(std::vector<TPoint> &Points, const std::vector<TPoint> &P, TInterpolationAlgorithm Algorithm)
{
  switch(Algorithm)
  {
    case iaCubicSpline:
      CreateCubicSplines(Points, P);
      break;

    case iaHalfCosine:
      CreateHalfCosineInterpolation(Points, P);
      break;

    case iaLinear:
    default:
      Points.insert(Points.end(), P.begin(), P.end());
  }
}
//---------------------------------------------------------------------------
//Returns the angle (slope) of the spline at p3. The spline goes from p2 to p3. p1 and p4 are control points
double InterpolationAngle(Func32::TDblPoint p1, Func32::TDblPoint p2, Func32::TDblPoint p3, Func32::TDblPoint p4, TInterpolationAlgorithm Algorithm)
{
  switch(Algorithm)
  {
    case iaCubicSpline:
    {
      double dx = 1.5 * (-p1.x + 3*p2.x -3*p3.x + p4.x) +
                        (2*p1.x -5*p2.x + 4*p3.x - p4.x) +
                  0.5 * (-p1.x+p3.x);

      double dy = 1.5 * (-p1.y + 3*p2.y -3*p3.y + p4.y) +
                        (2*p1.y -5*p2.y + 4*p3.y - p4.y) +
                  0.5 * (-p1.y+p3.y);
      return std::atan2(dy, dx);
    }
    case iaHalfCosine:
      return 0;

    case iaLinear:
    default:
      return std::atan2(p3.y - p2.y, p3.x - p2.x);
  }
}
//---------------------------------------------------------------------------
TRect Rotate(TMetafile *Metafile, int Degrees)
{
  if(Degrees == 0)
    return TRect(0, 0, Metafile->Width, Metafile->Height);

  unsigned Width = Metafile->Width;
  unsigned Height = Metafile->Height;
  double Angle = Degrees * M_PI / 180;

  unsigned NewWidth = std::abs(Width * std::cos(Angle)) + std::abs(Height * std::sin(Angle));
  unsigned NewHeight = std::abs(Width * std::sin(Angle)) + std::abs(Height * std::cos(Angle));

  std::auto_ptr<TMetafile> TempMetafile(new TMetafile);
  TempMetafile->Width = NewWidth;
  TempMetafile->Height = NewHeight;
  std::auto_ptr<TMetafileCanvas> Canvas(new TMetafileCanvas(TempMetafile.get(), 0));

  int dx = 0;
  int dy = 0;
  if(std::sin(Angle) > 0)
    dy += Width * std::sin(Angle);
  else
    dx -= Height * std::sin(Angle);
  if(std::cos(Angle) < 0)
  {
    dy -= Height * std::cos(Angle);
    dx -= Width * std::cos(Angle);
  }

  XFORM Xform = {std::cos(Angle), -std::sin(Angle), std::sin(Angle), std::cos(Angle), dx, dy};
  Win32Check(SetGraphicsMode(Canvas->Handle, GM_ADVANCED));
  Win32Check(SetWorldTransform(Canvas->Handle, &Xform));
  Canvas->Draw(0, 0, Metafile);
  Canvas.reset();
  Metafile->Assign(TempMetafile.get());
  return TRect(0, 0, NewWidth, NewHeight);
}
//---------------------------------------------------------------------------
unsigned AlignBit(unsigned Bits, unsigned BitsPerPixel, unsigned Alignment)
{
  Alignment--;
  return (((Bits * BitsPerPixel) + Alignment) & ~Alignment) >> 3;
}
//---------------------------------------------------------------------------
void InitializeBitmapInfoHeader(HBITMAP Bitmap, BITMAPINFOHEADER &Info, TPixelFormat PixelFormat)
{
  DIBSECTION DIB;
  DIB.dsBmih.biSize = 0;
  int Bytes = GetObject(Bitmap, sizeof(DIB), &DIB);
  if(Bytes == 0)
    throw Exception("Invalid bitmap");

  if(Bytes >= (sizeof(DIB.dsBm) + sizeof(DIB.dsBmih)) &&
    DIB.dsBmih.biSize >= sizeof(DIB.dsBmih))
    Info = DIB.dsBmih;
  else
  {
    memset(&Info, 0, sizeof(Info));
    Info.biSize = sizeof(Info);
    Info.biWidth = DIB.dsBm.bmWidth;
    Info.biHeight = DIB.dsBm.bmHeight;
  }
  switch(PixelFormat)
  {
    case pf1bit: Info.biBitCount = 1;   break;
    case pf4bit: Info.biBitCount = 4;   break;
    case pf8bit: Info.biBitCount = 8;   break;
    case pf24bit: Info.biBitCount = 24; break;
    case pf32bit: Info.biBitCount = 32; break;
    default:
      throw Exception("Invalid pixel foramt");
  }
  Info.biPlanes = 1;
  Info.biCompression = BI_RGB; // Always return data in RGB format
  Info.biSizeImage = AlignBit(Info.biWidth, Info.biBitCount, 32) * abs(Info.biHeight);
}
//---------------------------------------------------------------------------
bool InternalGetDIB(HBITMAP Bitmap, HPALETTE Palette, void *BitmapInfo, void *Bits, TPixelFormat PixelFormat)
{
  HPALETTE OldPal = 0;
  BITMAPINFOHEADER &Info = *static_cast<BITMAPINFOHEADER*>(BitmapInfo);
  InitializeBitmapInfoHeader(Bitmap, Info, PixelFormat);
  HDC DC = CreateCompatibleDC(0);
  if(Palette != 0)
  {
    OldPal = SelectPalette(DC, Palette, False);
    RealizePalette(DC);
  }
  bool Result = GetDIBits(DC, Bitmap, 0, abs(Info.biHeight), Bits,
    static_cast<BITMAPINFO*>(BitmapInfo), DIB_RGB_COLORS);
  if(OldPal != 0)
    SelectPalette(DC, OldPal, false);
  DeleteDC(DC);
  return Result;
}
//---------------------------------------------------------------------------
void InternalGetDIBSizes(HBITMAP Bitmap, int &InfoHeaderSize, int &ImageSize, TPixelFormat PixelFormat)
{
  BITMAPINFOHEADER Info;
  InitializeBitmapInfoHeader(Bitmap, Info, PixelFormat);
  // Check for palette device format
  if(Info.biBitCount > 8)
  {
    // Header but no palette
    InfoHeaderSize = sizeof(Info);
    if((Info.biCompression & BI_BITFIELDS) != 0)
      InfoHeaderSize += 12;
  }
  else
    // Header and palette
    InfoHeaderSize = sizeof(BITMAPINFOHEADER) + sizeof(TRGBQuad) * (1 << Info.biBitCount);
  ImageSize = Info.biSizeImage;
}
//---------------------------------------------------------------------------

