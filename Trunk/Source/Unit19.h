/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit19H
#define Unit19H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TntStdCtrls.hpp"
#include <MPlayer.hpp>
#include "ProgressForm.h"
//---------------------------------------------------------------------------
class TForm19 : public TTntForm
{
__published:	// IDE-managed Components
  TTntLabel *Label1;
  TTntLabel *Label2;
  TTntLabel *Label3;
  TTntLabel *Label4;
  TTntLabel *Label5;
  TTntComboBox *ComboBox1;
  TTntEdit *Edit1;
  TTntEdit *Edit2;
  TTntEdit *Edit3;
  TTntEdit *Edit4;
  TTntButton *Button1;
  TTntButton *Button2;
  TTntButton *Button3;
  TProgressForm *ProgressForm1;
  TTntButton *Button4;
  TTntLabel *Label6;
  TTntLabel *Label7;
  TTntEdit *Edit5;
  TTntEdit *Edit6;
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall Button4Click(TObject *Sender);
  void __fastcall TntEditKeyPress(TObject *Sender, char &Key);
private:	// User declarations
  TData Data;

public:		// User declarations
  __fastcall TForm19(TComponent* Owner, const TData &AData);
};
//---------------------------------------------------------------------------
#endif
 