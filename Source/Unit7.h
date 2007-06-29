/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit7H
#define Unit7H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include "LineSelect.h"
#include <ComCtrls.hpp>
#include "ExtColorBox.h"
#include "MyEdit.h"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TForm7 : public TTntForm
{
__published:	// IDE-managed Components
  TTntButton *Button1;
  TTntButton *Button2;
  TTntGroupBox *GroupBox2;
  TTntLabel *Label4;
  TTntLabel *Label5;
  TMyEdit *Edit1;
  TMyEdit *Edit2;
  TTntGroupBox *GroupBox4;
  TTntLabel *Label7;
  TTntLabel *Label8;
  TTntLabel *Label9;
  TTntEdit *Edit4;
  TUpDown *UpDown1;
  TLineSelect *LineSelect1;
  TTntGroupBox *GroupBox3;
  TTntLabel *Label10;
  TMyEdit *Edit3;
  TTntButton *Button3;
  TExtColorBox *ExtColorBox1;
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);

private:	// User declarations
  boost::shared_ptr<TBaseFuncType> Func;
  TData &Data;

public:		// User declarations
  __fastcall TForm7(TComponent* Owner, TData &AData);
  int InsertDif(const boost::shared_ptr<TBaseFuncType> &F);
};
//---------------------------------------------------------------------------
#endif

