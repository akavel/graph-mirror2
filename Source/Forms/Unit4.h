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
#include "CheckBoxEx.h"
//---------------------------------------------------------------------------
class TForm4 : public TForm
{
__published:	// IDE-managed Components
  TButton *Button1;
  TButton *Button2;
  TButton *Button3;
  TGroupBox *GroupBox1;
  TLabel *Label1;
  TEdit *Edit1;
  TUpDown *UpDown1;
  TRadioGroup *RadioGroup1;
  TUpDown *UpDown2;
  TEdit *Edit2;
  TLabel *Label2;
  TLabel *Label3;
  TEdit *Edit3;
  TUpDown *UpDown3;
  TLabel *Label5;
  TComboBox *ComboBox1;
  TComboBox *ComboBox2;
  TLabel *Label4;
  TCheckBoxEx *CheckBox1;
  TCheckBoxEx *CheckBox2;
  TCheckBoxEx *CheckBox3;
  TCheckBoxEx *CheckBox4;
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
