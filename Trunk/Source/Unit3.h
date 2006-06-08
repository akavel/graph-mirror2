/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Menus.hpp>
#include <Dialogs.hpp>
#include "VclObject.h"
#include "ExtColorBox.h"
#include "TntStdCtrls.hpp"
#include "TntButtons.hpp"
#include "TntComCtrls.hpp"
#include "TntExtCtrls.hpp"
//---------------------------------------------------------------------------
class TForm3 : public TTntForm
{
__published:	// IDE-managed Components
	TTntPageControl *PageControl1;
	TTntTabSheet *TabSheet1;
	TTntTabSheet *TabSheet2;
	TTntEdit *Edit1;
	TTntLabel *Label1;
	TTntLabel *Label2;
	TTntLabel *Label3;
	TTntEdit *Edit2;
	TTntEdit *Edit3;
	TTntCheckBox *CheckBox1;
  TTntCheckBox *CheckBox3;
  TTntEdit *Edit5;
  TTntCheckBox *CheckBox2;
	TTntLabel *Label4;
  TTntEdit *Edit6;
	TTntLabel *Label5;
	TTntLabel *Label6;
  TTntEdit *Edit7;
  TTntEdit *Edit8;
  TTntCheckBox *CheckBox14;
  TTntCheckBox *CheckBox8;
  TTntEdit *Edit9;
	TTntLabel *Label7;
  TTntCheckBox *CheckBox9;
  TTntCheckBox *CheckBox10;
  TTntEdit *Edit11;
  TTntEdit *Edit12;
	TTntLabel *Label8;
	TTntButton *Button1;
	TTntButton *Button2;
  TTntButton *Button3;
  TTntTabSheet *TabSheet3;
  TFontDialog *FontDialog1;
  TTntCheckBox *CheckBox15;
  TTntCheckBox *CheckBox16;
  TTntRadioGroup *RadioGroup1;
  TTntRadioGroup *RadioGroup2;
  TTntLabel *Label14;
  TTntEdit *Edit4;
  TTntCheckBox *CheckBox4;
  TTntCheckBox *CheckBox5;
  TTntTabSheet *TabSheet4;
  TTntGroupBox *GroupBox3;
  TTntLabel *Label17;
  TTntLabel *Label18;
  TTntLabel *Label19;
  TTntGroupBox *GroupBox4;
  TTntLabel *Label20;
  TTntLabel *Label21;
  TTntBitBtn *BitBtn2;
  TTntBitBtn *BitBtn3;
  TTntEdit *Edit13;
  TTntLabel *Label16;
  TTntBitBtn *BitBtn1;
  TTntEdit *Edit10;
  TTntLabel *Label9;
  TTntCheckBox *CheckBox11;
  TTntCheckBox *CheckBox12;
  TTntCheckBox *CheckBox6;
  TTntBitBtn *BitBtn4;
  TTntLabel *Label10;
  TTntCheckBox *CheckBox13;
  TTntCheckBox *CheckBox18;
  TTntCheckBox *CheckBox17;
  TTntCheckBox *CheckBox7;
  TTntRadioGroup *RadioGroup3;
  TTntCheckBox *CheckBox19;
  TExtColorBox *ExtColorBox1;
  TExtColorBox *ExtColorBox2;
  TExtColorBox *ExtColorBox3;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
  void __fastcall BitBtn2Click(TObject *Sender);
  void __fastcall BitBtn3Click(TObject *Sender);
  void __fastcall CheckBoxClick(TObject *Sender);
  void __fastcall BitBtn4Click(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall RadioGroup1Click(TObject *Sender);
  void __fastcall CheckBox1Click(TObject *Sender);
  void __fastcall CheckBox8Click(TObject *Sender);
  void __fastcall CheckBox15Click(TObject *Sender);
private:	// User declarations
  TVclObject<TFont> TitleFont, NumberFont, LabelFont, LegendFont;
  TData &Data;

  void RangeCheck(bool Condition, TWinControl *WinControl, const AnsiString &Str);
  void UpdateEnabledControls();
  void Translate();

public:		// User declarations
	__fastcall TForm3(TComponent* Owner, TData &AData);
};
//---------------------------------------------------------------------------
#endif
