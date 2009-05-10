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
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TAreaFrame : public TFrame, public TEvalFrame
{
__published:	// IDE-managed Components
  TGridPanel *GridPanel1;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TEdit *Edit3;
  TPanel *Panel1;
  TMyEdit *Edit1;
  TUpDown *UpDown1;
  TPanel *Panel2;
  TMyEdit *Edit2;
  TUpDown *UpDown2;
private:	// User declarations
  void EvalArea(const TBaseFuncType *Func, long double From, long double To);
  void EvalArea(const TPointSeries *PointSeries, long double From, long double To);

public:		// User declarations
  __fastcall TAreaFrame(TComponent* Owner);
  void Eval(const TGraphElem *Elem) {} //Dummy
  void SetPoint(const TGraphElem *Elem, int X, int Y);
  void SetEndPoint(const TGraphElem *Elem, int X, int Y);
  void EvalArea(const TGraphElem *Elem);
  void EvalArc(const TGraphElem *Elem);
  std::string GetErrorPrefix() {return std::string();}
  TFrame* GetFrame() {return this;} //Workaround for nasty compiler bug
};
//---------------------------------------------------------------------------
#endif
