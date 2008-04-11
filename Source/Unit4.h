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
	TTntButton *Button1;
	TTntButton *Button2;
  TTntButton *Button3;
  TGroupBox *GroupBox1;
  TTntLabel *Label1;
  TTntEdit *Edit1;
  TUpDown *UpDown1;
  TTntRadioGroup *RadioGroup1;
  TUpDown *UpDown2;
  TTntEdit *Edit2;
  TTntLabel *Label2;
  TTntLabel *Label3;
  TTntEdit *Edit3;
  TUpDown *UpDown3;
  TTntLabel *Label5;
  TTntComboBox *ComboBox1;
  TTntComboBox *ComboBox2;
  TTntLabel *Label4;
  TTntCheckBox *CheckBox1;
  TTntCheckBox *CheckBox2;
  TTntCheckBox *CheckBox3;
  TTntCheckBox *CheckBox4;
	void __fastcall Button1Click(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall ComboBox1KeyPress(TObject *Sender, char &Key);
private:	// User declarations
  int OldLanguageIndex; //Saved language index in ComboBox
  TData &Data;

public:		// User declarations
	__fastcall TForm4(TComponent* Owner, TData &AData);
};
//---------------------------------------------------------------------------
#endif
