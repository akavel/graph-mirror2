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
//---------------------------------------------------------------------------
__fastcall TForm20::TForm20(TComponent* Owner)
  : TTntForm(Owner)
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
  ScrollBar1->Max = MediaPlayer1->Length - 1;
  MediaPlayer1->Notify = true;
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
    MediaPlayer1->FileName = OpenDialog1->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TForm20::MediaPlayer1PostClick(TObject *Sender,
      TMPBtnType Button)
{
  ScrollBar1->Position = MediaPlayer1->Position;
}
//---------------------------------------------------------------------------
void __fastcall TForm20::ScrollBar1Scroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
  MediaPlayer1->Position = ScrollBar1->Position;
}
//---------------------------------------------------------------------------
void __fastcall TForm20::Reverse1Click(TObject *Sender)
{
  MediaPlayer1->Reverse = Reverse1->Checked;
  MediaPlayer1->Repeat = Repeat1->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TForm20::MediaPlayer1Signal(TMediaPlayerEx *Sender,
      unsigned Position)
{
  ScrollBar1->Position = MediaPlayer1->Position - 1;
}
//---------------------------------------------------------------------------
void __fastcall TForm20::MediaPlayer1Hint(TMediaPlayerEx *Sender,
      TMPBtnType Button)
{
  switch(Button)
  {
    case btPlay:  Sender->Hint = "Play"; break;
    case btPause: Sender->Hint = "Pause"; break;
    case btStop:  Sender->Hint = "Stop"; break;
    case Mplayer::btNext:
    case Mplayer::btPrev:
    case btStep:
    case btBack:

    default:
      Sender->Hint = "";
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm20::ToolButton1Click(TObject *Sender)
{
  MediaPlayer1->Play();
  ToolButton1->Enabled = false;
  ToolButton2->Enabled = true;
  ToolButton3->Enabled = true;
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
  MediaPlayer1->Step();
}
//---------------------------------------------------------------------------
void __fastcall TForm20::ToolButton7Click(TObject *Sender)
{
  MediaPlayer1->Back();
}
//---------------------------------------------------------------------------

