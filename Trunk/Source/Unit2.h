/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include "TntStdCtrls.hpp"
#include "SymbolDialog.h"
//---------------------------------------------------------------------------
class TForm2 : public TTntForm
{
__published:	// IDE-managed Components
	TTntButton *Button1;
	TPanel *Panel1;
	TTntLabel *ProductName;
	TTntLabel *Version;
	TTntLabel *Copyright;
	TTntLabel *Comments;
  TTntLabel *Label1;
  TTntLabel *Label2;
  TTntLabel *Label3;
  TTntLabel *Label4;
  TTntLabel *Label5;
  TTntLabel *Label6;
  TAnimate *Animate1;
  TImage *ProgramIcon;
  void __fastcall Label2Click(TObject *Sender);
  void __fastcall Label5Click(TObject *Sender);
  void __fastcall ProgramIconDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
