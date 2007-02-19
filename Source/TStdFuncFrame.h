/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef TStdFuncFrameH
#define TStdFuncFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "MyEdit.h"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TStdFuncFrame : public TFrame
{
__published:	// IDE-managed Components
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label1;
  TMyEdit *Edit1;
  TTntComboBox *ComboBox1;
  TTntLabel *Label5;
  TTntEdit *Edit2;
  TTntEdit *Edit3;
  TTntEdit *Edit4;
  void __fastcall ComboBox1Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TStdFuncFrame(TComponent* Owner);
  void EvalFunc(TStdFunc *Func);
  void Clear();
  void SetPoint(TStdFunc *Func, int X, int Y);
};
//---------------------------------------------------------------------------
#endif
