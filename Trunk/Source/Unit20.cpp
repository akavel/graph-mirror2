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
void __fastcall TForm20::Copy1Click(TObject *Sender)
{
/*typedef struct  {
    DWORD dwCallback;     // see below    DWORD dwFrom;         // starting position for copy    DWORD dwTo;           // ending position for copy    RECT  rc;             // see below    DWORD dwAudioStream;  // audio stream    DWORD dwVideoStream;  // video stream} MCI_DGV_COPY_PARMS;
  MCI_DGV_COPY_PARMS Copy = {0};
  Copy.dwCallback = (DWORD)Handle;
  Copy.dwFrom = 1;
  Copy.dwTo = 5;

  DWORD Result = mciSendCommand(MediaPlayer1->DeviceID, MCI_COPY, MCI_NOTIFY | MCI_FROM | MCI_TO, (DWORD)&Copy);
  char ErrorStr[256];
  if(Result != 0)
  {
    mciGetErrorString(Result, ErrorStr, sizeof(ErrorStr));
    MessageBox(ErrorStr, "MCI ERROR");
  } */

  OleCheck(OleInitialize(NULL));
  FORMATETC FormatEtc = {0, NULL, DVASPECT_CONTENT, -1, TYMED_NULL};
  IDataObject *DataObject;
  IOleObject *OleObject;
  IStorage *Storage;
  OleCheck(StgCreateDocfile(NULL, STGM_DELETEONRELEASE | STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE, 0, &Storage));
//  OleCheck(OleCreateLinkToFile(WideString(MediaPlayer1->FileName), IID_IDataObject, OLERENDER_NONE, &FormatEtc, NULL, Storage, reinterpret_cast<void**>(&DataObject)));

  OleCheck(OleCreateLinkToFile(WideString(MediaPlayer1->FileName), IID_IOleObject, OLERENDER_NONE, &FormatEtc, NULL, Storage, reinterpret_cast<void**>(&OleObject)));
  TRect Rect= Panel1->ClientRect;
  OleObject->DoVerb(OLEIVERB_SHOW, NULL, NULL, 0, Handle, &Rect);

/*  OleCheck(OleSetClipboard(DataObject));
  OleCheck(OleFlushClipboard());
  OleCheck(DataObject->Release());
  OleCheck(Storage->Release());
  OleUninitialize();*/
/*
  PAVIFILE pFile = NULL;
  AVIFileInit();
  if(AVIFileOpen(&pFile, MediaPlayer1->FileName.c_str(), OF_READ, NULL) == AVIERR_OK)
  {
    AVIPutFileOnClipboard(pFile);
    AVIFileRelease(pFile);
  }
  AVIFileExit(); */
}
//---------------------------------------------------------------------------
void __fastcall TForm20::Button1Click(TObject *Sender)
{
  DWORD Result = mciSendCommand(MediaPlayer1->DeviceID, MCI_UPDATE, 0, NULL);
  char ErrorStr[256];
  if(Result != 0)
  {
    mciGetErrorString(Result, ErrorStr, sizeof(ErrorStr));
    MessageBox(ErrorStr, "MCI ERROR");
  }
}
//---------------------------------------------------------------------------

