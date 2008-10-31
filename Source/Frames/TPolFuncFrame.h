/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
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
#include "TStdFuncFrame.h"
//---------------------------------------------------------------------------
class TPolFuncFrame : public TFrame, public TEvalFrame
{
__published:	// IDE-managed Components
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label1;
  TLabel *Label5;
  TLabel *Label6;
  TEdit *Edit2;
  TEdit *Edit3;
  TEdit *Edit4;
  TEdit *Edit5;
  TEdit *Edit6;
  TMyEdit *Edit1;
  TComboBox *ComboBox1;
  TLabel *Label7;
  void __fastcall ComboBox1Change(TObject *Sender);
private:	// User declarations
  void Clear();

public:		// User declarations
  __fastcall TPolFuncFrame(TComponent* Owner);
  void Eval(const TGraphElem *Elem);
  void SetPoint(const TGraphElem *Elem, int X, int Y);
  std::string GetErrorPrefix() {return std::string();}
  TFrame* GetFrame() {return this;} //Workaround for nasty compiler bug
};
//---------------------------------------------------------------------------
#endif
