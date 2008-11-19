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
//---------------------------------------------------------------------------
class TForm15 : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label4;
  TEdit *Edit3;
  TButton *Button1;
  TButton *Button2;
  TLabel *Label1;
  TEdit *Edit1;
  TLabel *Label2;
  TEdit *Edit2;
  TPopupMenu *PopupMenu1;
  TMenuItem *Popup1_Copy;
  TMenuItem *Popup1_Select;
  TButton *Button3;
  TProgressForm *ProgressForm1;
  TGrid *Grid1;
  TMenuItem *Popup1_Export;
  TSaveDialog *SaveDialog1;
  TMenuItem *N1;
  TMenuItem *Popup1_Show1;
  TMenuItem *Popup1_Show5;
  TMenuItem *Popup1_Show3;
  TMenuItem *Popup1_Show4;
  TMenuItem *Popup1_Show2;
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall Popup1_CopyClick(TObject *Sender);
  void __fastcall Popup1_SelectClick(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall Popup1_ExportClick(TObject *Sender);
  void __fastcall Popup1_Show(TObject *Sender);
private:	// User declarations
  const TBaseFuncType *Func;

  void SetCaptions(const wchar_t*const* Captions);

public:		// User declarations
  __fastcall TForm15(TComponent* Owner);
  void ShowTable(const TBaseFuncType *F);
};
//---------------------------------------------------------------------------
#endif