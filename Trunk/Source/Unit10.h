/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit10H
#define Unit10H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TForm10 : public TTntForm
{
__published:	// IDE-managed Components
  TTntButton *Button1;
  TTntButton *Button2;
  TTntButton *Button3;
  TTntCheckBox *CheckBox1;
  TPanel *Panel1;
  TTntLabel *Label2;
  TTntLabel *Label1;
  TImage *Image1;
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
  TProperty &Property;

public:		// User declarations
  __fastcall TForm10(TComponent* Owner, TProperty &AProperty);
};
//---------------------------------------------------------------------------
#endif

