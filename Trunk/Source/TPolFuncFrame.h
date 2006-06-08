/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef TPolFuncFrameH
#define TPolFuncFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MyEdit.h"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TPolFuncFrame : public TFrame
{
__published:	// IDE-managed Components
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label1;
  TLabel *Label5;
  TLabel *Label6;
  TTntEdit *Edit2;
  TTntEdit *Edit3;
  TTntEdit *Edit4;
  TTntEdit *Edit5;
  TTntEdit *Edit6;
  TMyEdit *Edit1;
  TTntComboBox *ComboBox1;
  TTntLabel *Label7;
  void __fastcall ComboBox1Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TPolFuncFrame(TComponent* Owner);
  void EvalFunc(TPolFunc *Func);
  void Clear();
  void SetPoint(TPolFunc *Func, int X, int Y);
};
//---------------------------------------------------------------------------
#endif
