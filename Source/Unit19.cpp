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
#include "Unit19.h"
#include "Unit20.h"
#include <iterator>
#include "vfw.h"
#include "IGraphic.h"
#include <cmath>
//---------------------------------------------------------------------------
#pragma link "TntStdCtrls"
#pragma link "ProgressForm"
#pragma link "MyEdit"                       
#pragma resource "*.dfm"

const int MaxWidth = Screen->Width - 50;
const int MaxHeight = Screen->Height - 180;
//---------------------------------------------------------------------------
__fastcall TForm19::TForm19(TComponent* Owner, TData &AData)
  : TTntForm(Owner), Data(AData), AnimationInfo(AData.AnimationInfo)
{
  ScaleForm(this);
  TranslateProperties(this);
  SetAccelerators(this);

  int Left = Label7->Left + TMaxWidth(Label2)(Label3)(Label4)(Label5)(Label6)(Label7) + 5;
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
    {
      TAnimationConstant &AnimationConstant = AnimationInfo.ConstantList[Iter->Name];
      if(AnimationConstant.Step.empty())
      {
        double Value = Func32::Eval(Iter->Name, Data.CustomFunctions.SymbolList, Data.Axes.Trigonometry);
        double Min = (Value == 0) ? 10 : Value / 10;
        double Max = (Value == 0) ? 10 : Value * 10;
        AnimationConstant.Min = ToString(Min);
        AnimationConstant.Max = ToString(Max);
        AnimationConstant.Step = ToString(std::abs((Max - Min) / 9));
      }
      ComboBox1->Items->Add(Iter->Name.c_str());
    }

  unsigned ImageWidth = std::min(AnimationInfo.Width == 0 ? Form1->Image1->Width : AnimationInfo.Width, MaxWidth);
  unsigned ImageHeight = std::min(AnimationInfo.Height == 0 ? Form1->Image1->Height : AnimationInfo.Height, MaxHeight);

  int Index = ComboBox1->Items->IndexOf(ToWideString(AnimationInfo.Constant));
  ComboBox1->ItemIndex = Index == -1 ? 0 : Index;

  if(Index == -1)
    ComboBox1Change(ComboBox1);
  else
  {
    Edit1->Text = ToWideString(AnimationInfo.ConstantList[AnimationInfo.Constant].Min);
    Edit2->Text = ToWideString(AnimationInfo.ConstantList[AnimationInfo.Constant].Max);
    Edit3->Text = ToWideString(AnimationInfo.ConstantList[AnimationInfo.Constant].Step);
  }

  Edit4->Text = ToWideString(ImageWidth);
  Edit5->Text = ToWideString(ImageHeight);
  Edit6->Text = ToWideString(AnimationInfo.FramesPerSecond);
}
//---------------------------------------------------------------------------
void __fastcall TForm19::Button1Click(TObject *Sender)
{
  if(!CheckLimit(Edit4, LoadRes(RES_VALUE_RANGE, Label5->Caption, 160, MaxWidth), 160, MaxWidth))
    return;
  if(!CheckLimit(Edit5, LoadRes(RES_VALUE_RANGE, Label6->Caption, 160, MaxHeight), 160, MaxHeight))
    return;

  std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
  unsigned ImageWidth = ToInt(Edit4->Text);
  unsigned ImageHeight = ToInt(Edit5->Text);;
  if(Edit4->Modified || Edit5->Modified)
  {
    AnimationInfo.Width = ImageWidth;
    AnimationInfo.Height = ImageHeight;
  }

  Bitmap->Width = ImageWidth;
  Bitmap->Height = ImageHeight;

  AnimationInfo.Constant = ToString(ComboBox1->Text);
  TDraw Draw(Bitmap->Canvas, &Data, false, "Animate thread");
  Draw.SetArea(TRect(0, 0, ImageWidth, ImageHeight));

  ComboBox1Change(NULL); //Update AnimationInfo

  double Min = MakeFloat(Edit1);
  double Max = MakeFloat(Edit2);
  double Step = std::abs(MakeFloat(Edit3));
  if(Max < Min)
    Step = -Step;
  AnimationInfo.FramesPerSecond = MakeFloat(Edit6, LoadRes(RES_GREATER_ZERO, Label7->Caption), 0.01);

  unsigned StepCount = std::ceil((Max - Min) / Step) + 1;
  ProgressForm1->Max = StepCount;
  ProgressForm1->Position = 0;
  ProgressForm1->Show();
  TCallOnRelease Dummy(&ProgressForm1->Close);

  TRect Rect(0, 0, ImageWidth, ImageHeight);
  std::vector<char> ImageData;
  ImageData.reserve(Rect.Width() * Rect.Height());

  //Make space for 256 colors in palette
  std::vector<char> BitmapInfoData(256 * sizeof(RGBQUAD) + sizeof(BITMAPINFOHEADER));
  BITMAPINFO *BitmapInfo = reinterpret_cast<BITMAPINFO*>(&BitmapInfoData[0]);
  FillBitmapInfoHeader(BitmapInfo->bmiHeader, Bitmap.get(), Rect, 256, 0);
  AnsiString TempFile = GetTempFileName("Graph", "avi");
  std::string OriginalValue = Data.CustomFunctions.GetValue(AnimationInfo.Constant);

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
    StreamInfo.dwFlags = AVISTREAMINFO_FORMATCHANGES;
    StreamInfo.dwCaps = 0;
    StreamInfo.wPriority = 0;
    StreamInfo.wLanguage = 0;
    StreamInfo.dwScale = 100;
    StreamInfo.dwRate = AnimationInfo.FramesPerSecond * 100;
    StreamInfo.dwStart = 0;
    StreamInfo.dwLength = StepCount; //Number of frames, I think
    StreamInfo.dwInitialFrames = 0;
    StreamInfo.dwSuggestedBufferSize = ImageHeight * ImageWidth * 4; //Should be changed later
    StreamInfo.dwQuality = -1;
    StreamInfo.dwSampleSize = 0;
    StreamInfo.rcFrame = TRect(0, 0, ImageWidth, ImageHeight);
    StreamInfo.dwEditCount = 0;
    StreamInfo.dwFormatChangeCount = 0;                                 
    strcpy(StreamInfo.szName, "Graph animation");
                 
    OleCheck(AVIFileCreateStream(pFile, &pStream, &StreamInfo));
    TCallOnRelease Dummy5(AVIStreamRelease, pStream);

    unsigned I = 0;
    std::vector<RGBQUAD> Colors;
    for(double Value = Min; I < StepCount; Value += Step, I++)
    {
      Bitmap->Canvas->Brush->Style = bsSolid;
      Bitmap->Canvas->Brush->Color = Data.Axes.BackgroundColor;
      Bitmap->Canvas->FillRect(TRect(0, 0, ImageWidth, ImageHeight));
      Data.CustomFunctions.Replace(AnimationInfo.Constant, Value);
      Data.CustomFunctions.Update();
      Data.Update();
      Data.ClearCache();
      Draw.DrawAll();
      Draw.Wait();

      //As we don't know what colors will be used in all the frames, the palette is updated for each frame.
      //It seems to give problems when the palette is changed, at least with the MCI used in Graph,
      //so we only add to the palette. It also gives problems when the number of entries is
      //increased. Therefore we always store 256 colors, where the unused have dummy values.
      //Why is this so difficult?
      unsigned LastColorCount = Colors.size();
      CompressBitmap(Bitmap.get(), Rect, Colors, ImageData);
      if(Colors.size() > LastColorCount)
      {
        std::copy(Colors.begin(), Colors.end(), BitmapInfo->bmiColors);
        OleCheck(AVIStreamSetFormat(pStream, I, BitmapInfo, BitmapInfoData.size()));
      }

      OleCheck(AVIStreamWrite(pStream, I, 1, &ImageData[0], ImageData.size(), AVIIF_KEYFRAME, NULL, NULL));

/*
      //Save uncrompressed
      int BitmapInfoSize;
      int BitmapSize;
      Bitmap->HandleType = bmDIB;
      Bitmap->PixelFormat = pf8bit;
      InternalGetDIBSizes(Bitmap->Handle, BitmapInfoSize, BitmapSize, Bitmap->PixelFormat);
      std::vector<char> BitmapInfo(BitmapInfoSize);
      std::vector<char> BitmapBits(BitmapSize);
      InternalGetDIB(Bitmap->Handle, 0, &BitmapInfo[0], &BitmapBits[0], Bitmap->PixelFormat);
      if(I == 0)
        OleCheck(AVIStreamSetFormat(pStream, 0, &BitmapInfo[0], BitmapInfoSize));
      AVIStreamWrite(pStream, I, 1, &BitmapBits[0], BitmapSize, AVIIF_KEYFRAME, NULL, NULL);
*/

      ProgressForm1->StepIt();
      if(ProgressForm1->AbortProgress)
      {
        //Restore constant in case we want to create a new animation with another constant
        Data.CustomFunctions.Replace(AnimationInfo.Constant, OriginalValue);
        return;
      }
    }
  }

  //Restore constant in case we want to create a new animation with another constant
  Data.CustomFunctions.Replace(AnimationInfo.Constant, OriginalValue);
  ProgressForm1->Close();
  CreateForm<TForm20>(AnimationInfo.Constant, Min, Step)->ShowAnimation(TempFile);
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
void __fastcall TForm19::ComboBox1Change(TObject *Sender)
{
  if(!AnimationInfo.Constant.empty())
  {
    TAnimationConstant &AnimationConstant = AnimationInfo.ConstantList[AnimationInfo.Constant];
    AnimationConstant.Min = ToString(Edit1->Text);
    AnimationConstant.Max = ToString(Edit2->Text);
    AnimationConstant.Step = ToString(Edit3->Text);
  }

  AnimationInfo.Constant = ToString(ComboBox1->Text);
  TAnimationConstant &AnimationConstant = AnimationInfo.ConstantList[ToString(ComboBox1->Text)];
  Edit1->Text = ToWideString(AnimationConstant.Min);
  Edit2->Text = ToWideString(AnimationConstant.Max);
  Edit3->Text = ToWideString(AnimationConstant.Step);
}
//---------------------------------------------------------------------------



