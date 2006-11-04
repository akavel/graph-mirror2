/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit20H
#define Unit20H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <TntMenus.hpp>
#include <ImgList.hpp>
#include "MediaPlayerEx.h"
#include <MPlayer.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TForm20 : public TTntForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TSaveDialog *SaveDialog1;
  TTntPopupMenu *PopupMenu1;
  TTntMenuItem *Open1;
  TTntMenuItem *Save1;
  TOpenDialog *OpenDialog1;
  TImageList *ImageList1;
  TTntMenuItem *N1;
  TTntMenuItem *Repeat1;
  TTntMenuItem *Reverse1;
  TMediaPlayerEx *MediaPlayer1;
  TTntMenuItem *Speed1;
  TTrackBar *TrackBar1;
  TTntMenuItem *N2001;
  TTntMenuItem *N1501;
  TTntMenuItem *N1001;
  TTntMenuItem *N501;
  TTntMenuItem *N101;
  TTntMenuItem *N10001;
  TPanel *Panel2;
  TToolBar *ToolBar1;
  TToolButton *ToolButton1;
  TToolButton *ToolButton2;
  TToolButton *ToolButton3;
  TToolButton *ToolButton4;
  TToolButton *ToolButton5;
  TToolButton *ToolButton6;
  void __fastcall Save1Click(TObject *Sender);
  void __fastcall Open1Click(TObject *Sender);
  void __fastcall MediaPlayer1Signal(TMediaPlayerEx *Sender,
          unsigned Position);
  void __fastcall ToolButton1Click(TObject *Sender);
  void __fastcall ToolButton2Click(TObject *Sender);
  void __fastcall ToolButton3Click(TObject *Sender);
  void __fastcall ToolButton4Click(TObject *Sender);
  void __fastcall ToolButton5Click(TObject *Sender);
  void __fastcall ToolButton6Click(TObject *Sender);
  void __fastcall ToolButton7Click(TObject *Sender);
  void __fastcall MediaPlayer1Notify(TMediaPlayerEx *Sender,
          TMPNotifyValues NotifyValue);
  void __fastcall SpeedClick(TObject *Sender);
  void __fastcall TntFormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall TrackBar1Change(TObject *Sender);
  void __fastcall TrackBar1KeyPress(TObject *Sender, char &Key);
private:	// User declarations
  bool BackwardDirection;
public:		// User declarations
  __fastcall TForm20(TComponent* Owner);
  void ShowAnimation(const AnsiString &FileName);
};
//---------------------------------------------------------------------------
#endif
