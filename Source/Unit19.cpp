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

::TAnimationInfo TForm19::AnimationInfo;
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

  Edit1->Text = ToWideString(AnimationInfo.Min);
  Edit2->Text = ToWideString(AnimationInfo.Max);
  Edit3->Text = ToWideString(AnimationInfo.Step);
  Edit4->Text = ToWideString(AnimationInfo.Width);
  Edit5->Text = ToWideString(AnimationInfo.Height);
  Edit6->Text = ToWideString(AnimationInfo.FramesPerSecond);
}
//---------------------------------------------------------------------------
void __fastcall TForm19::Button1Click(TObject *Sender)
{
  if(!CheckLimit(Edit4, LoadRes(RES_GREATER, Label6->Caption, 160), 160))
    return;
  if(!CheckLimit(Edit5, LoadRes(RES_GREATER, Label7->Caption, 160), 160))
    return;

  std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
  AnimationInfo.Width = ToInt(Edit4->Text);
  AnimationInfo.Height = ToInt(Edit5->Text);;
  Bitmap->Width = AnimationInfo.Width;
  Bitmap->Height = AnimationInfo.Height;
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
    StreamInfo.dwRate = AnimationInfo.FramesPerSecond * 100;
    StreamInfo.dwStart = 0;
    StreamInfo.dwLength = 0;
    StreamInfo.dwInitialFrames = 0;
    StreamInfo.dwSuggestedBufferSize = AnimationInfo.Height * AnimationInfo.Width * 4; //Should be changed later
    StreamInfo.dwQuality = -1;
    StreamInfo.dwSampleSize = 0;
    StreamInfo.rcFrame = TRect(0, 0, AnimationInfo.Width, AnimationInfo.Height);
    StreamInfo.dwEditCount = 0;
    StreamInfo.dwFormatChangeCount = 0;
    strcpy(StreamInfo.szName, "Graph animation");

    if(AVIFileCreateStream(pFile, &pStream, &StreamInfo) == AVIERR_OK)
    {
      if(AVIStreamSetFormat(pStream, 0, &BitmapInfo, sizeof(BitmapInfo)) == AVIERR_OK)
      {
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

          GetDIB(Bitmap->Handle, 0, &BitmapInfo, &ImageData[0]);
          if(AVIStreamWrite(pStream, I, 1, &ImageData[0], ImageSize, AVIIF_KEYFRAME, NULL, NULL) != AVIERR_OK)
            throw Exception("AVI write error");
          ProgressForm1->StepIt();     
        }
      }
      AVIStreamRelease(pStream);
    }

    AVIFileRelease(pFile);
  }

  AVIFileExit();

  ProgressForm1->Close();
  CreateForm<TForm20>()->ShowAnimation(TempFile);
  DeleteFile(TempFile);
}
//---------------------------------------------------------------------------
void __fastcall TForm19::Button4Click(TObject *Sender)
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

