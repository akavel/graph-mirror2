/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit15H
#define Unit15H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "Grid.h"
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "ProgressForm.h"
#include "TntMenus.hpp"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TForm15 : public TTntForm
{
__published:	// IDE-managed Components
  TTntLabel *Label4;
  TTntEdit *Edit3;
  TTntButton *Button1;
  TTntButton *Button2;
  TTntLabel *Label1;
  TTntEdit *Edit1;
  TTntLabel *Label2;
  TTntEdit *Edit2;
  TTntPopupMenu *PopupMenu1;
  TTntMenuItem *Popup1_Copy;
  TTntMenuItem *Popup1_Select;
  TTntButton *Button3;
  TProgressForm *ProgressForm1;
  TGrid *Grid1;
  TTntMenuItem *Popup1_Export;
  TSaveDialog *SaveDialog1;
  TTntMenuItem *N1;
  TTntMenuItem *Popup1_Show_df;
  TTntMenuItem *Popup1_Show_ddf;
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall Popup1_CopyClick(TObject *Sender);
  void __fastcall Popup1_SelectClick(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall Popup1_ExportClick(TObject *Sender);
  void __fastcall Popup1_Show(TObject *Sender);
private:	// User declarations
  const TBaseFuncType *Func;

public:		// User declarations
  __fastcall TForm15(TComponent* Owner);
  void ShowTable(const TBaseFuncType *F);
};
//---------------------------------------------------------------------------
#endif
