/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
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
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TAreaFrame : public TFrame
{
__published:	// IDE-managed Components
  TMyEdit *Edit1;
  TMyEdit *Edit2;
  TTntEdit *Edit3;
  TTntLabel *Label1;
  TTntLabel *Label2;
  TTntLabel *Label3;
private:	// User declarations
public:		// User declarations
  __fastcall TAreaFrame(TComponent* Owner);
  void EvalArea(TGraphElem *Elem);
  void EvalArc(TGraphElem *Elem);
};
//---------------------------------------------------------------------------
#endif
