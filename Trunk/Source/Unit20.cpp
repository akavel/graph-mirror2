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
#include "vfw.h"
#include "BMGLibPNG.h"                     //Used to save png files
#include <Jpeg.hpp>
//---------------------------------------------------------------------------
#pragma link "TntMenus"
#pragma link "MediaPlayerEx"
#pragma resource "*.dfm"
const double MaxWidth = Screen->Width - 50;
const double MaxHeight = Screen->Height - 180;
//--------------------------------------------------------------------------
__fastcall TForm20::TForm20(TComponent* Owner, const std::string &Constant, double AMin, double AStep)
  : TTntForm(Owner), BackwardDirection(false), Min(AMin), Step(AStep)
{
  ScaleForm(this);
  TranslateProperties(this);
  SetAccelerators(this);

  dwICValue = dwICValue; //Avoid stupid warning
  Panel1->DoubleBuffered;
  LabeledEdit1->EditLabel->Caption = Constant.c_str() + AnsiString("=");
  LabeledEdit1->Text = Min;
}
//---------------------------------------------------------------------------
void TForm20::ShowAnimation(const AnsiString &FileName)
{
  MediaPlayer1->FileName = FileName;
  MediaPlayer1->Open();
  DisplayRect = MediaPlayer1->SourceRect;
  double Zoom = std::max(std::max(DisplayRect.Height() / MaxHeight, DisplayRect.Width() / MaxWidth), 1.0);
  DisplayRect.Bottom /= Zoom;
  DisplayRect.Right /= Zoom;
  Width = Width - Panel1->ClientWidth + DisplayRect.Width();
  Height = Height - Panel1->ClientHeight + DisplayRect.Height();
  TrackBar1->Max = MediaPlayer1->Length - 1;
  TrackBar1->Frequency = (MediaPlayer1->Length / 100) * 10;
  if(TrackBar1->Frequency == 0)
    TrackBar1->Frequency = 1;
  MediaPlayer1->TimeFormat = tfFrames;
  MediaPlayer1->SetSignal(0, 1);
  MediaPlayer1->DisplayRect = DisplayRect;

  if(!Visible)
    ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm20::Save1Click(TObject *Sender)
{
  if(SaveDialog1->Execute())
    Win32Check(CopyFile(MediaPlayer1->FileName.c_str(), SaveDialog1->FileName.c_str(), false));
}
//---------------------------------------------------------------------------
void __fastcall TForm20::MediaPlayer1Signal(TMediaPlayerEx *Sender,
      unsigned Position)
{
  PosChanged(MediaPlayer1->Reverse ? Position : Position - 1);
}
//---------------------------------------------------------------------------
void __fastcall TForm20::ToolButton1Click(TObject *Sender)
{
  ToolButton1->Enabled = false;
  ToolButton2->Enabled = true;
  ToolButton3->Enabled = true;

  if(!Reverse1->Checked)
    MediaPlayer1->Reverse = false;

  MediaPlayer1->Repeat = Repeat1->Checked && !Reverse1->Checked;
  MediaPlayer1->Notify = true;
  MediaPlayer1->Play();
  MediaPlayer1->Notify = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm20::ToolButton2Click(TObject *Sender)
{
  MediaPlayer1->Pause();
  ToolButton1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm20::ToolButton3Click(TObject *Sender)
{
  MediaPlayer1->Stop();
  MediaPlayer1->Rewind();
  PosChanged(0);
  ToolButton1->Enabled = true;
  ToolButton2->Enabled = false;
  ToolButton3->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm20::ToolButton4Click(TObject *Sender)
{
  MediaPlayer1->Next();
}
//---------------------------------------------------------------------------
void __fastcall TForm20::ToolButton5Click(TObject *Sender)
{
  MediaPlayer1->Previous();
}
//---------------------------------------------------------------------------
void __fastcall TForm20::ToolButton6Click(TObject *Sender)
{
  try
  {
    MediaPlayer1->Step();
    PosChanged(MediaPlayer1->Position);
  }
  catch(EMCIDeviceError &E)
  { //Ignore Boundary errors;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm20::ToolButton7Click(TObject *Sender)
{
  try
  {
    MediaPlayer1->Back();
    PosChanged(MediaPlayer1->Position);
  }
  catch(EMCIDeviceError &E)
  { //Ignore Boundary errors;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm20::MediaPlayer1Notify(TMediaPlayerEx *Sender,
      TMPNotifyValues NotifyValue)
{
  if(NotifyValue != nvAborted)
  {
    if(Reverse1->Checked)
      MediaPlayer1->Reverse = !MediaPlayer1->Reverse;
    if(Repeat1->Checked)
    {
      MediaPlayer1->Notify = true;
      MediaPlayer1->Play();
      MediaPlayer1->Notify = false;
    }
  }

  if(MediaPlayer1->Mode == mpStopped)
  {
    ToolButton1->Enabled = true;
    ToolButton2->Enabled = false;
    ToolButton3->Enabled = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm20::SpeedClick(TObject *Sender)
{
  if(TMenuItem *Item = dynamic_cast<TMenuItem*>(Sender))
    MediaPlayer1->Speed = Item->Caption.SubString(1, Item->Caption.Length() - 1).ToInt() * 10;
}
//---------------------------------------------------------------------------
void __fastcall TForm20::TntFormClose(TObject *Sender,
      TCloseAction &Action)
{
  MediaPlayer1->Stop();
}
//---------------------------------------------------------------------------
void __fastcall TForm20::TrackBar1Change(TObject *Sender)
{
  MediaPlayer1->Position = TrackBar1->Position;
  PosChanged(MediaPlayer1->Position);
}
//---------------------------------------------------------------------------
void __fastcall TForm20::TrackBar1KeyPress(TObject *Sender, char &Key)
{
  if(Key == VK_ESCAPE)
    Close();
}
//---------------------------------------------------------------------------
void TForm20::PosChanged(unsigned Position)
{
  TrackBar1->OnChange = NULL;
  TrackBar1->Position = Position;
  TrackBar1->OnChange = &TrackBar1Change;

  LabeledEdit1->Text = Min + Step * Position;
}
//---------------------------------------------------------------------------
void __fastcall TForm20::Saveimageas1Click(TObject *Sender)
{
  if(SaveDialog2->Execute())
    SaveFrame(SaveDialog2->FileName, SaveDialog2->FilterIndex);
}
//---------------------------------------------------------------------------
void __fastcall TForm20::Saveimagesequence1Click(TObject *Sender)
{
  if(SaveDialog2->Execute())
  {
    int FrameCount = MediaPlayer1->Length;
    int Size = std::log10l(FrameCount) + 1;
    int OldPosition = MediaPlayer1->Position;
    for(int I = 0; I < FrameCount; I++)
    {
      AnsiString FileName = ChangeFileExt(SaveDialog2->FileName, AnsiString().sprintf(" %0*d%s", Size, I+1, ExtractFileExt(SaveDialog2->FileName).c_str()));
      MediaPlayer1->Position = I;
      SaveFrame(FileName, SaveDialog2->FilterIndex);
    }
    MediaPlayer1->Position = OldPosition;
  }
}
//---------------------------------------------------------------------------
void TForm20::SaveFrame(const AnsiString &FileName, int FilterIndex)
{
  std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
  MediaPlayer1->DisplayRect = MediaPlayer1->SourceRect;
  MediaPlayer1->GetBitmap(Bitmap.get());
  switch(FilterIndex)
  {
    case 1: //bmp
      Bitmap->SaveToFile(FileName);
      break;

    case 2: //png
     Bitmap->PixelFormat = pf8bit; //Change bitmap to 8 bit
     SaveBitmapToPNGFile(Bitmap->Handle, FileName.c_str());
     break;

    case 3: //jpeg
    {
      std::auto_ptr<TJPEGImage> Image(new TJPEGImage);
      Image->Assign(Bitmap.get());
//      Image->CompressionQuality = ImageOptions->Jpeg.Quality;
//      Image->ProgressiveEncoding = ImageOptions->Jpeg.ProgressiveEncoding;
      Image->Compress();
      Image->SaveToFile(FileName);
      break;
    }
  }
  MediaPlayer1->DisplayRect = DisplayRect; 
}
//---------------------------------------------------------------------------

