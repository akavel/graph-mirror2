/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "TntStdCtrls.hpp"
#include "TntExtCtrls.hpp"
#include "TntForms.hpp"
//---------------------------------------------------------------------------
class TForm4 : public TTntForm
{
__published:	// IDE-managed Components
	TTntEdit *Edit1;
	TTntLabel *Label1;
	TTntButton *Button1;
	TTntButton *Button2;
	TTntCheckBox *CheckBox1;
  TTntEdit *Edit2;
  TTntLabel *Label2;
  TTntCheckBox *CheckBox4;
  TTntButton *Button3;
  TBevel *Bevel1;
  TTntRadioGroup *RadioGroup1;
  TTntCheckBox *CheckBox2;
  TTntLabel *Label3;
  TTntEdit *Edit3;
  TTntCheckBox *CheckBox7;
  TTntLabel *Label4;
  TTntComboBox *ComboBox1;
  TUpDown *UpDown1;
  TUpDown *UpDown2;
  TUpDown *UpDown3;
  TTntLabel *Label5;
  TTntComboBox *ComboBox2;
	void __fastcall Button1Click(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall ComboBox2KeyPress(TObject *Sender, char &Key);
private:	// User declarations
  int OldLanguageIndex; //Saved language index in ComboBox
  TData &Data;

public:		// User declarations
	__fastcall TForm4(TComponent* Owner, TData &AData);
};
//---------------------------------------------------------------------------
#endif
