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
//---------------------------------------------------------------------------
#pragma link "TntMenus"
#pragma link "MediaPlayerEx"
#pragma resource "*.dfm"
//--------------------------------------------------------------------------
__fastcall TForm20::TForm20(TComponent* Owner, const ::TAnimationInfo &AAnimationInfo)
  : TTntForm(Owner), BackwardDirection(false), AnimationInfo(AAnimationInfo)
{
  ScaleForm(this);
  TranslateProperties(this);
  SetAccelerators(this);

  dwICValue = dwICValue; //Avoid stupid warning
  Panel1->DoubleBuffered;
  LabeledEdit1->EditLabel->Caption = AnimationInfo.Constant.c_str() + AnsiString("=");
}
//---------------------------------------------------------------------------
void TForm20::ShowAnimation(const AnsiString &FileName)
{
  MediaPlayer1->FileName = FileName;
  MediaPlayer1->Open();
  TRect Rect = MediaPlayer1->DisplayRect;
  Width = Width - Panel1->ClientWidth + Rect.Width();
  Height = Height - Panel1->ClientHeight + Rect.Height();
  TrackBar1->Max = MediaPlayer1->Length - 1;
  TrackBar1->Frequency = (MediaPlayer1->Length / 100) * 10;
  if(TrackBar1->Frequency == 0)
    TrackBar1->Frequency = 1;
  MediaPlayer1->TimeFormat = tfFrames;
  MediaPlayer1->SetSignal(0, 1);

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

  LabeledEdit1->Text = AnimationInfo.Min + AnimationInfo.Step * Position;
}
//---------------------------------------------------------------------------

