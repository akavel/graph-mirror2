/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef TAreaFrameH
#define TAreaFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MyEdit.h"
#include "TStdFuncFrame.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TAreaFrame : public TFrame, public TEvalFrame
{
__published:	// IDE-managed Components
  TGridPanel *GridPanel1;
  TLabel *Label1;
  TMyEdit *Edit1;
  TLabel *Label2;
  TMyEdit *Edit2;
  TLabel *Label3;
  TEdit *Edit3;
private:	// User declarations

public:		// User declarations
  __fastcall TAreaFrame(TComponent* Owner);
  void Eval(const TGraphElem *Elem) {} //Dummy
  void SetPoint(const TGraphElem *Elem, int X, int Y) {} //Dummy
  void EvalArea(const TGraphElem *Elem);
  void EvalArc(const TGraphElem *Elem);
  std::string GetErrorPrefix() {return std::string();}
  TFrame* GetFrame() {return this;} //Workaround for nasty compiler bug
};
//---------------------------------------------------------------------------
#endif
