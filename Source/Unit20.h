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
//---------------------------------------------------------------------------
class TForm20 : public TTntForm
{
__published:	// IDE-managed Components
  TMediaPlayerEx *MediaPlayer1;
  TPanel *Panel1;
  TSaveDialog *SaveDialog1;
  TPopupMenu *PopupMenu1;
  TMenuItem *Open1;
  TMenuItem *Save1;
  TOpenDialog *OpenDialog1;
  TPanel *Panel2;
  TToolBar *ToolBar1;
  TToolButton *ToolButton1;
  TImageList *ImageList1;
  TMenuItem *N1;
  TMenuItem *Repeat1;
  TScrollBar *ScrollBar1;
  TMenuItem *Reverse1;
  TToolButton *ToolButton2;
  TToolButton *ToolButton3;
  TToolButton *ToolButton4;
  TToolButton *ToolButton5;
  TToolButton *ToolButton6;
  TToolButton *ToolButton7;
  TToolButton *ToolButton8;
  void __fastcall Save1Click(TObject *Sender);
  void __fastcall Open1Click(TObject *Sender);
  void __fastcall MediaPlayer1PostClick(TObject *Sender,
          TMPBtnType Button);
  void __fastcall ScrollBar1Scroll(TObject *Sender, TScrollCode ScrollCode,
          int &ScrollPos);
  void __fastcall Reverse1Click(TObject *Sender);
  void __fastcall MediaPlayer1Signal(TMediaPlayerEx *Sender,
          unsigned Position);
  void __fastcall MediaPlayer1Hint(TMediaPlayerEx *Sender,
          TMPBtnType Button);
  void __fastcall ToolButton1Click(TObject *Sender);
  void __fastcall ToolButton2Click(TObject *Sender);
  void __fastcall ToolButton3Click(TObject *Sender);
  void __fastcall ToolButton4Click(TObject *Sender);
  void __fastcall ToolButton5Click(TObject *Sender);
  void __fastcall ToolButton6Click(TObject *Sender);
  void __fastcall ToolButton7Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm20(TComponent* Owner);
  void ShowAnimation(const AnsiString &FileName, unsigned ImageWidth, unsigned ImageHeight);
};
//---------------------------------------------------------------------------
#endif
