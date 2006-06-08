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
//---------------------------------------------------------------------------
#pragma link "TntStdCtrls"
#pragma link "ProgressForm"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm19::TForm19(TComponent* Owner)
  : TTntForm(Owner)
{
  ScaleForm(this);
  TranslateProperties(this);
  SetAccelerators(this);

  dwICValue = dwICValue; //Avoid stupid warning
  TData &Data = Form1->Data;

  //Add constants to the combo box
  for(TCustomFunctions::ConstIterator Iter = Data.CustomFunctions.Begin(); Iter != Data.CustomFunctions.End(); ++Iter)
    if(Iter->Arguments.empty())
      ComboBox1->Items->Add(Iter->Name.c_str());
  ComboBox1->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm19::Button1Click(TObject *Sender)
{
  std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
  unsigned ImageWidth = 320;
  unsigned ImageHeight = 320;
  Bitmap->Width = ImageWidth;
  Bitmap->Height = ImageHeight;
  TData &Data = Form1->Data;
  std::string ConstantName = ToString(ComboBox1->Text);
  std::string OldValue = Data.CustomFunctions.GetValue(ConstantName);
  TDraw Draw(Bitmap->Canvas, &Data, false, "Animate thread");
  Draw.SetArea(TRect(0, 0, ImageWidth, ImageHeight));

  double Min = MakeFloat(Edit1);
  double Max = MakeFloat(Edit2);
  double Step = MakeFloat(Edit3);
  double FramesPerSecond = MakeFloat(Edit4, LoadRes(RES_GREATER_ZERO, Label5->Caption), 0.01);

  ProgressForm1->Max = std::ceil((Max - Min) / Step) + 1;
  ProgressForm1->Position = 0;
  ProgressForm1->Show();
  TCallOnRelease Dummy(&ProgressForm1->Close);

  unsigned InfoSize;
  unsigned ImageSize;
  GetDIBSizes(Bitmap->Handle, InfoSize, ImageSize);
  std::vector<char> ImageData(ImageSize);
  BITMAPINFOHEADER BitmapInfo;
  GetDIB(Bitmap->Handle, 0, &BitmapInfo, &ImageData[0]);

  AnsiString TempFile = GetTempFileName("Graph", "avi");
  AVIFileInit();
  PAVIFILE pFile = NULL;
  if(AVIFileOpen(&pFile, TempFile.c_str(), OF_WRITE | OF_CREATE, NULL) == AVIERR_OK)
  {
    PAVISTREAM pStream = NULL;
    AVISTREAMINFO StreamInfo;

    StreamInfo.fccType = streamtypeVIDEO;
    StreamInfo.fccHandler = 0;
    StreamInfo.dwFlags = 0;
    StreamInfo.dwCaps = 0;
    StreamInfo.wPriority = 0;
    StreamInfo.wLanguage = 0;
    StreamInfo.dwScale = 100;
    StreamInfo.dwRate = FramesPerSecond * 100;
    StreamInfo.dwStart = 0;
    StreamInfo.dwLength = 0;
    StreamInfo.dwInitialFrames = 0;
    StreamInfo.dwSuggestedBufferSize = ImageHeight*ImageWidth*4; //Should be changed later
    StreamInfo.dwQuality = -1;
    StreamInfo.dwSampleSize = 0;
    StreamInfo.rcFrame = TRect(0, 0, ImageWidth, ImageHeight);
    StreamInfo.dwEditCount = 0;
    StreamInfo.dwFormatChangeCount = 0;
    strcpy(StreamInfo.szName, "Graph animation");

    if(AVIFileCreateStream(pFile, &pStream, &StreamInfo) == AVIERR_OK)
    {
      if(AVIStreamSetFormat(pStream, 0, &BitmapInfo, sizeof(BitmapInfo)) == AVIERR_OK)
      {
        int I = 0;
        for(double Value = Min; Value <= Max; Value += Step, I++)
        {
          Bitmap->Canvas->Brush->Color = Data.Axes.BackgroundColor;
          Bitmap->Canvas->FillRect(TRect(0, 0, ImageWidth, ImageHeight));
          Data.CustomFunctions.Replace(ConstantName, Value);
          Data.CustomFunctions.Update();
          Data.Update();
          Data.ClearCache();
          Draw.DrawAll();
          Draw.Wait();           

          GetDIB(Bitmap->Handle, 0, &BitmapInfo, &ImageData[0]);
          if(AVIStreamWrite(pStream, I, 1, &ImageData[0], ImageSize, AVIIF_KEYFRAME, NULL, NULL) != AVIERR_OK)
            throw Exception("AVI write error");
          ProgressForm1->StepIt();     
        }
        Data.CustomFunctions.Replace(ConstantName, OldValue);
      }
      AVIStreamRelease(pStream);
    }
    AVIFileRelease(pFile);
  }

  AVIFileExit();

  ProgressForm1->Close();
  CreateForm<TForm20>()->ShowAnimation(TempFile, ImageWidth, ImageHeight);
  DeleteFile(TempFile);
}
//---------------------------------------------------------------------------
void __fastcall TForm19::Button4Click(TObject *Sender)
{
  Application->HelpContext(HelpContext);
}
//---------------------------------------------------------------------------

