/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "Unit19.h"
#include "Unit20.h"
#include <iterator>
#include "vfw.h"
#include "IGraphic.h"
//---------------------------------------------------------------------------
#pragma link "TntStdCtrls"
#pragma link "ProgressForm"
#pragma resource "*.dfm"                                                     

::TAnimationInfo TForm19::AnimationInfo;
const int MaxWidth = Screen->Width - 50;
const int MaxHeight = Screen->Height - 180;
//---------------------------------------------------------------------------
__fastcall TForm19::TForm19(TComponent* Owner, const TData &AData)
  : TTntForm(Owner), Data(AData)
{
  ScaleForm(this);
  TranslateProperties(this);
  SetAccelerators(this);

  int Left = Label7->Left + Label7->Width + 5;
  ResizeControl(ComboBox1, Left);
  ResizeControl(Edit1, Left);
  ResizeControl(Edit2, Left);
  ResizeControl(Edit3, Left);
  ResizeControl(Edit4, Left);
  ResizeControl(Edit5, Left);
  ResizeControl(Edit6, Left);

  dwICValue = dwICValue; //Avoid stupid warning

  //Add constants to the combo box
  for(TCustomFunctions::ConstIterator Iter = Data.CustomFunctions.Begin(); Iter != Data.CustomFunctions.End(); ++Iter)
    if(Iter->Arguments.empty())
      ComboBox1->Items->Add(Iter->Name.c_str());

  int Index = ComboBox1->Items->IndexOf(ToWideString(AnimationInfo.Constant));
  ComboBox1->ItemIndex = Index == -1 ? 0 : Index;

  AnimationInfo.Width = std::min(Form1->Image1->Width, MaxWidth);
  AnimationInfo.Height = std::min(Form1->Image1->Height, MaxHeight);

  Edit1->Text = ToWideString(AnimationInfo.Min);
  Edit2->Text = ToWideString(AnimationInfo.Max);
  Edit3->Text = ToWideString(AnimationInfo.Step);
  Edit4->Text = ToWideString(AnimationInfo.Width);
  Edit5->Text = ToWideString(AnimationInfo.Height);
  Edit6->Text = ToWideString(AnimationInfo.FramesPerSecond);

  EnumCompressors();
}
//---------------------------------------------------------------------------
void __fastcall TForm19::Button1Click(TObject *Sender)
{
  if(!CheckLimit(Edit4, LoadRes(RES_VALUE_RANGE, Label5->Caption, 160, MaxWidth), 160, MaxWidth))
    return;
  if(!CheckLimit(Edit5, LoadRes(RES_VALUE_RANGE, Label6->Caption, 160, MaxHeight), 160, MaxHeight))
    return;

  std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
  AnimationInfo.Width = ToInt(Edit4->Text);
  AnimationInfo.Height = ToInt(Edit5->Text);;
  Bitmap->Width = AnimationInfo.Width;
  Bitmap->Height = AnimationInfo.Height;
  Bitmap->PixelFormat = pf8bit;
  AnimationInfo.Constant = ToString(ComboBox1->Text);
  TDraw Draw(Bitmap->Canvas, &Data, false, "Animate thread");
  Draw.SetArea(TRect(0, 0, AnimationInfo.Width, AnimationInfo.Height));

  AnimationInfo.Min = MakeFloat(Edit1);
  AnimationInfo.Max = MakeFloat(Edit2);
  AnimationInfo.Step = MakeFloat(Edit3);
  AnimationInfo.FramesPerSecond = MakeFloat(Edit6, LoadRes(RES_GREATER_ZERO, Label5->Caption), 0.01);

  ProgressForm1->Max = std::ceil((AnimationInfo.Max - AnimationInfo.Min) / AnimationInfo.Step) + 1;
  ProgressForm1->Position = 0;
  ProgressForm1->Show();
  TCallOnRelease Dummy(&ProgressForm1->Close);

  TRect Rect(0, 0, AnimationInfo.Width, AnimationInfo.Height);
  std::vector<char> ImageData;
  ImageData.reserve(Rect.Width() * Rect.Height());

  //Get colors used and sort them. If other colors are used they will be replaced by the nearest from the list.
  //Unfortunately it is difficult to know what colors are used without looking at all the frames.
  std::vector<TColor> Colors = Form1->ColorSelect->ColorListVector;
  std::sort(Colors.begin(), Colors.end());

  std::vector<char> BitmapInfoData(Colors.size() * sizeof(TColor) + sizeof(BITMAPINFOHEADER));
  BITMAPINFO *BitmapInfo = reinterpret_cast<BITMAPINFO*>(&BitmapInfoData[0]);
  FillBitmapInfoHeader(BitmapInfo->bmiHeader, Bitmap.get(), Rect, Colors.size(), 0);
  std::copy(Colors.begin(), Colors.end(), reinterpret_cast<TColor*>(BitmapInfo->bmiColors));
/*  BITMAPINFOHEADER BitmapInfo;
  unsigned InfoSize;
  unsigned ImageSize;
  GetDIBSizes(Bitmap->Handle, InfoSize, ImageSize);
  ImageData.resize(ImageSize);
  GetDIB(Bitmap->Handle, 0, &BitmapInfo, &ImageData[0]);
*/
  AnsiString TempFile = GetTempFileName("Graph", "avi");
  AVIFileInit();
  TCallOnRelease Dummy2(AVIFileExit);
  TCallOnRelease Dummy3(DeleteFile, TempFile);

  //Inner scope needed to release the avi file when the creation is finished.
  {
    PAVIFILE pFile = NULL;
    OleCheck(AVIFileOpen(&pFile, TempFile.c_str(), OF_WRITE | OF_CREATE, NULL));
    TCallOnRelease Dummy4(AVIFileRelease, pFile);

    PAVISTREAM pStream = NULL;
    AVISTREAMINFO StreamInfo;

    StreamInfo.fccType = streamtypeVIDEO;
    StreamInfo.fccHandler = 0; //CompressionHandler[ComboBox2->ItemIndex];
    StreamInfo.dwFlags = 0;
    StreamInfo.dwCaps = 0;
    StreamInfo.wPriority = 0;
    StreamInfo.wLanguage = 0;
    StreamInfo.dwScale = 100;
    StreamInfo.dwRate = AnimationInfo.FramesPerSecond * 100;
    StreamInfo.dwStart = 0;
    StreamInfo.dwLength = std::ceil((AnimationInfo.Max - AnimationInfo.Min) / AnimationInfo.Step); //Number of frames, I think
    StreamInfo.dwInitialFrames = 0;
    StreamInfo.dwSuggestedBufferSize = AnimationInfo.Height * AnimationInfo.Width * 4; //Should be changed later
    StreamInfo.dwQuality = -1;
    StreamInfo.dwSampleSize = 0;
    StreamInfo.rcFrame = TRect(0, 0, AnimationInfo.Width, AnimationInfo.Height);
    StreamInfo.dwEditCount = 0;
    StreamInfo.dwFormatChangeCount = 0;
    strcpy(StreamInfo.szName, "Graph animation");

    OleCheck(AVIFileCreateStream(pFile, &pStream, &StreamInfo));
    TCallOnRelease Dummy5(AVIStreamRelease, pStream);

    OleCheck(AVIStreamSetFormat(pStream, 0, BitmapInfo, /*sizeof(BitmapInfo)*/BitmapInfoData.size()));

    int I = 0;
    for(double Value = AnimationInfo.Min; Value <= AnimationInfo.Max; Value += AnimationInfo.Step, I++)
    {
      Bitmap->Canvas->Brush->Color = Data.Axes.BackgroundColor;
      Bitmap->Canvas->FillRect(TRect(0, 0, AnimationInfo.Width, AnimationInfo.Height));
      Data.CustomFunctions.Replace(AnimationInfo.Constant, Value);
      Data.CustomFunctions.Update();
      Data.Update();
      Data.ClearCache();
      Draw.DrawAll();
      Draw.Wait();

      CompressBitmap(Bitmap.get(), Rect, Colors, ImageData);
//      GetDIB(Bitmap->Handle, 0, &BitmapInfo, &ImageData[0]);
      OleCheck(AVIStreamWrite(pStream, I, 1, &ImageData[0], ImageData.size(), AVIIF_KEYFRAME, NULL, NULL));
      ProgressForm1->StepIt();
      if(ProgressForm1->AbortProgress)
        return;
    }
  }

  ProgressForm1->Close();
  CreateForm<TForm20>()->ShowAnimation(TempFile);
}
//---------------------------------------------------------------------------
void __fastcall TForm19::Button3Click(TObject *Sender)
{
  Application->HelpContext(HelpContext);
}
//---------------------------------------------------------------------------
void __fastcall TForm19::TntEditKeyPress(TObject *Sender, char &Key)
{
  if(!isdigit(Key) && Key != '\b')
    Key = 0;
}
//---------------------------------------------------------------------------
void TForm19::EnumCompressors()
{
/*  ICINFO Info;
  unsigned I = 0;
  CompressionHandler.push_back(0); //The first is "Uncompressed"

  while(ICInfo(ICTYPE_VIDEO, I, &Info))
  {
    HIC hic = ICOpen(Info.fccType, Info.fccHandler, ICMODE_QUERY);
    ICGetInfo(hic, &Info, sizeof(Info));
    ICClose(hic);
    if(*Info.szName)
    {
      ComboBox2->Items->Add(Info.szDescription);
      CompressionHandler.push_back(Info.fccHandler);
    }
    I++;
  }

  ComboBox2->ItemIndex = 0;*/
}
//---------------------------------------------------------------------------
void __fastcall TForm19::Button5Click(TObject *Sender)
{
//  HIC hic = ICOpen(ICTYPE_VIDEO, CompressionHandler[ComboBox2->ItemIndex], ICMODE_QUERY);
//  ICAbout(hic, Handle);
//  ICClose(hic);
}
//---------------------------------------------------------------------------
void __fastcall TForm19::Button4Click(TObject *Sender)
{
//  HIC hic = ICOpen(ICTYPE_VIDEO, CompressionHandler[ComboBox2->ItemIndex], ICMODE_QUERY);
//  ICConfigure(hic, Handle);
//  ICClose(hic);
}
//---------------------------------------------------------------------------
void __fastcall TForm19::ComboBox2Change(TObject *Sender)
{
//  HIC hic = ICOpen(ICTYPE_VIDEO, CompressionHandler[ComboBox2->ItemIndex], ICMODE_QUERY);
//  Button4->Enabled = ICQueryConfigure(hic);
//  Button5->Enabled = ICQueryAbout(hic);
//  ICClose(hic);
}
//---------------------------------------------------------------------------

