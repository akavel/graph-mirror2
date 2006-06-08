/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef TTanFrameH
#define TTanFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MyEdit.h"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TTanFrame : public TFrame
{
__published:	// IDE-managed Components
  TTntEdit *Edit2;
  TLabel *Label1;
  TLabel *Label2;
  TMyEdit *Edit1;
private:	// User declarations
public:		// User declarations
  __fastcall TTanFrame(TComponent* Owner);
  void EvalTan(const TTan *Tan);
  void SetPoint(const TTan *Tan, int X);
  void SetPoint(const TPointSeries *Series, int X);
  void EvalSeries(const TPointSeries *Series);
};
//---------------------------------------------------------------------------
#endif
