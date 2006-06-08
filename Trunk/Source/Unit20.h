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
#include <MPlayer.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <TntMenus.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TForm20 : public TTntForm
{
__published:	// IDE-managed Components
  TMediaPlayer *MediaPlayer1;
  TPanel *Panel1;
  TSaveDialog *SaveDialog1;
  TPopupMenu *PopupMenu1;
  TMenuItem *Open1;
  TMenuItem *Save1;
  TMenuItem *Copy1;
  TOpenDialog *OpenDialog1;
  TPanel *Panel2;
  TToolBar *ToolBar1;
  TToolButton *ToolButton1;
  TButton *Button1;
  TImageList *ImageList1;
  void __fastcall Save1Click(TObject *Sender);
  void __fastcall Open1Click(TObject *Sender);
  void __fastcall Copy1Click(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm20(TComponent* Owner);
  void ShowAnimation(const AnsiString &FileName, unsigned ImageWidth, unsigned ImageHeight);
};
//---------------------------------------------------------------------------
#endif
