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
#include "Unit20.h"
#include "vfw.h"
//---------------------------------------------------------------------------
#pragma link "TntMenus"
#pragma link "MediaPlayerEx"
#pragma resource "*.dfm"
//--------------------------------------------------------------------------
__fastcall TForm20::TForm20(TComponent* Owner)
  : TTntForm(Owner), BackwardDirection(false)
{
  ScaleForm(this);                              
  TranslateProperties(this);                        
  SetAccelerators(this);
        
  dwICValue = dwICValue; //Avoid stupid warning
  Panel1->DoubleBuffered;
}                                                   
//---------------------------------------------------------------------------
void TForm20::ShowAnimation(const AnsiString &FileName, unsigned ImageWidth, unsigned ImageHeight)
{
  Width = Width - Panel1->ClientWidth + ImageWidth;
  Height = Height - Panel1->ClientHeight + ImageHeight;
  MediaPlayer1->FileName = FileName;             
  MediaPlayer1->Open();
  TrackBar1->Max = MediaPlayer1->Length - 1;
  MediaPlayer1->TimeFormat = tfFrames;
  MediaPlayer1->SetSignal(0, 1);
  ShowModal();                                           
}                                                 
//---------------------------------------------------------------------------
void __fastcall TForm20::Save1Click(TObject *Sender)
{
  if(SaveDialog1->Execute())
    Win32Check(CopyFile(MediaPlayer1->FileName.c_str(), SaveDialog1->FileName.c_str(), false));
}
//---------------------------------------------------------------------------
void __fastcall TForm20::Open1Click(TObject *Sender)
{                                                      
  if(OpenDialog1->Execute())
  {
    MediaPlayer1->FileName = OpenDialog1->FileName;
    MediaPlayer1->Open();
    TrackBar1->Max = MediaPlayer1->Length - 1;
    MediaPlayer1->TimeFormat = tfFrames;
    MediaPlayer1->SetSignal(0, 1);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm20::MediaPlayer1Signal(TMediaPlayerEx *Sender,
      unsigned Position)               
{
  OutputDebugString(("Signal: Position = " + AnsiString(MediaPlayer1->Position)).c_str());

  TrackBar1->OnChange = NULL;
  TrackBar1->Position = MediaPlayer1->Reverse ? Position : Position - 1;
  TrackBar1->OnChange = &TrackBar1Change; 
}
//---------------------------------------------------------------------------
void __fastcall TForm20::ToolButton1Click(TObject *Sender)
{
  ToolButton1->Enabled = false;
  ToolButton2->Enabled = true;
  ToolButton3->Enabled = true;
  MediaPlayer1->Notify = true;

  if(!Reverse1->Checked)
    MediaPlayer1->Reverse = false;

  MediaPlayer1->Repeat = Repeat1->Checked && !Reverse1->Checked;
  MediaPlayer1->Play();
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
  TrackBar1->Position = 0;
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
    TrackBar1->OnChange = NULL;
    TrackBar1->Position = MediaPlayer1->Position;
    TrackBar1->OnChange = &TrackBar1Change;
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
    TrackBar1->OnChange = NULL;
    TrackBar1->Position = MediaPlayer1->Position;
    TrackBar1->OnChange = &TrackBar1Change;
  }
  catch(EMCIDeviceError &E)
  { //Ignore Boundary errors;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm20::MediaPlayer1Notify(TMediaPlayerEx *Sender,
      TMPNotifyValues NotifyValue)
{
  MediaPlayer1->Notify = false;
  if(NotifyValue != nvAborted)
  {
    if(Reverse1->Checked)
      MediaPlayer1->Reverse = !MediaPlayer1->Reverse;
    if(Repeat1->Checked)
    {
      MediaPlayer1->Notify = true;
      MediaPlayer1->Play();
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
}
//---------------------------------------------------------------------------

