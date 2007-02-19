/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit17H
#define Unit17H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include "Grid.h"
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "TntMenus.hpp"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TForm17 : public TTntForm
{
__published:	// IDE-managed Components
  TTntButton *Button1;
  TTntButton *Button2;
  TTntButton *Button4;
  TTntButton *Button3;
  TGrid *Grid1;
  TTntPopupMenu *PopupMenu1;
  TTntMenuItem *Popup_Insert;
  TTntMenuItem *Popup_Remove;
  TTntMenuItem *Popup_Cut;
  TTntMenuItem *Popup_Copy;
  TTntMenuItem *Popup_Paste;
  TTntMenuItem *Popup_Delete;
  TTntMenuItem *N1;
  TTntMenuItem *N2;
  TTntMenuItem *Popup_Import;
  TTntMenuItem *Popup_Export;
  TTntMenuItem *Popup_Select;
  TOpenDialog *OpenDialog1;
  TSaveDialog *SaveDialog1;
  void __fastcall Button4Click(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall Popup_InsertClick(TObject *Sender);
  void __fastcall Popup_RemoveClick(TObject *Sender);
  void __fastcall Grid1EditorKeyPress(TInplaceEdit *InplaceEdit,
          char &Key);
  void __fastcall Popup_CutClick(TObject *Sender);
  void __fastcall Popup_CopyClick(TObject *Sender);
  void __fastcall Popup_PasteClick(TObject *Sender);
  void __fastcall Popup_DeleteClick(TObject *Sender);
  void __fastcall Popup_SelectClick(TObject *Sender);
  void __fastcall Popup_ImportClick(TObject *Sender);
  void __fastcall Popup_ExportClick(TObject *Sender);
private:	// User declarations
  TData &Data;

public:		// User declarations
  __fastcall TForm17(TComponent* Owner, TData &AData);
};
//---------------------------------------------------------------------------
#endif
