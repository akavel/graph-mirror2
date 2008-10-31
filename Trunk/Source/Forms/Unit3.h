/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
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
#include "MyEdit.h"
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
  TPageControl *PageControl1;
  TTabSheet *TabSheet1;
  TTabSheet *TabSheet2;
	TMyEdit *Edit1;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
	TMyEdit *Edit2;
	TMyEdit *Edit3;
  TCheckBox *CheckBox1;
  TCheckBox *CheckBox3;
  TMyEdit *Edit5;
  TCheckBox *CheckBox2;
  TLabel *Label4;
  TMyEdit *Edit6;
  TLabel *Label5;
  TLabel *Label6;
  TMyEdit *Edit7;
  TMyEdit *Edit8;
  TCheckBox *CheckBox14;
  TCheckBox *CheckBox8;
  TMyEdit *Edit9;
  TLabel *Label7;
  TCheckBox *CheckBox9;
  TCheckBox *CheckBox10;
  TMyEdit *Edit11;
  TMyEdit *Edit12;
  TLabel *Label8;
  TButton *Button1;
  TButton *Button2;
  TButton *Button3;
  TTabSheet *TabSheet3;
  TFontDialog *FontDialog1;
  TCheckBox *CheckBox15;
  TCheckBox *CheckBox16;
  TRadioGroup *RadioGroup1;
  TRadioGroup *RadioGroup2;
  TLabel *Label14;
  TMyEdit *Edit4;
  TCheckBox *CheckBox4;
  TCheckBox *CheckBox5;
  TTabSheet *TabSheet4;
  TGroupBox *GroupBox3;
  TLabel *Label17;
  TLabel *Label18;
  TLabel *Label19;
  TGroupBox *GroupBox4;
  TLabel *Label20;
  TLabel *Label21;
  TBitBtn *BitBtn2;
  TBitBtn *BitBtn3;
  TMyEdit *Edit13;
  TLabel *Label16;
  TBitBtn *BitBtn1;
  TMyEdit *Edit10;
  TLabel *Label9;
  TCheckBox *CheckBox11;
  TCheckBox *CheckBox12;
  TCheckBox *CheckBox6;
  TBitBtn *BitBtn4;
  TLabel *Label10;
  TCheckBox *CheckBox13;
  TCheckBox *CheckBox18;
  TCheckBox *CheckBox17;
  TCheckBox *CheckBox7;
  TRadioGroup *RadioGroup3;
  TCheckBox *CheckBox19;
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
