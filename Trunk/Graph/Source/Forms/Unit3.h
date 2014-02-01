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
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "VclObject.h"
#include "ExtColorBox.h"
#include "MyEdit.h"
#include "CheckBoxEx.h"
#include "GridPanelEx.h"
#include "FocusPanel.h"
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TTabSheet *TabSheet3;
  TFontDialog *FontDialog1;
  TCheckBoxEx *CheckBox15;
  TCheckBoxEx *CheckBox16;
  TRadioGroup *RadioGroup1;
  TRadioGroup *RadioGroup2;
  TTabSheet *TabSheet4;
  TGroupBox *GroupBox3;
  TLabel *Label17;
  TLabel *Label18;
  TLabel *Label19;
  TGroupBox *GroupBox4;
  TLabel *Label20;
  TLabel *Label21;
  TMyEdit *Edit13;
  TLabel *Label16;
  TLabel *Label10;
  TRadioGroup *RadioGroup3;
  TCheckBoxEx *CheckBox19;
  TExtColorBox *ExtColorBox1;
  TExtColorBox *ExtColorBox2;
  TExtColorBox *ExtColorBox3;
  TGridPanelEx *GridPanel2;
  TLabel *Label5;
  TMyEdit *Edit7;
  TLabel *Label6;
  TMyEdit *Edit8;
  TLabel *Label7;
  TMyEdit *Edit9;
  TLabel *Label9;
  TMyEdit *Edit10;
  TGridPanelEx *GridPanel1;
  TLabel *Label1;
  TMyEdit *Edit1;
  TLabel *Label2;
  TMyEdit *Edit2;
  TLabel *Label3;
  TMyEdit *Edit3;
  TLabel *Label14;
  TMyEdit *Edit4;
  TCheckBoxEx *CheckBox4;
  TCheckBoxEx *CheckBox6;
  TCheckBoxEx *CheckBox5;
  TCheckBoxEx *CheckBox7;
  TCheckBoxEx *CheckBox1;
  TCheckBoxEx *CheckBox2;
  TCheckBoxEx *CheckBox17;
  TCheckBoxEx *CheckBox11;
  TCheckBoxEx *CheckBox13;
  TCheckBoxEx *CheckBox12;
  TCheckBoxEx *CheckBox14;
  TCheckBoxEx *CheckBox8;
  TCheckBoxEx *CheckBox9;
  TCheckBoxEx *CheckBox18;
  TButton *Button4;
  TButton *Button5;
  TButton *Button6;
  TButton *Button7;
  TGridPanelEx *GridPanelEx1;
  TLabel *Label8;
  TMyEdit *Edit12;
  TGridPanelEx *GridPanelEx2;
  TMyEdit *Edit11;
  TCheckBoxEx *CheckBox10;
  TGridPanelEx *GridPanelEx3;
  TMyEdit *Edit5;
  TCheckBoxEx *CheckBox3;
  TGridPanelEx *GridPanelEx4;
  TMyEdit *Edit6;
  TLabel *Label4;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
  void __fastcall Button7Click(TObject *Sender);
  void __fastcall Button4Click(TObject *Sender);
  void __fastcall Button5Click(TObject *Sender);
  void __fastcall CheckBoxClick(TObject *Sender);
  void __fastcall Button6Click(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall RadioGroup1Click(TObject *Sender);
  void __fastcall CheckBox1Click(TObject *Sender);
  void __fastcall CheckBox8Click(TObject *Sender);
  void __fastcall CheckBox15Click(TObject *Sender);
private:	// User declarations
  TVclObject<TFont> TitleFont, NumberFont, LabelFont, LegendFont;
  TData &Data;

  void RangeCheck(bool Condition, TWinControl *WinControl, const String &Str);
  void UpdateEnabledControls();
  void Translate();

public:		// User declarations
	__fastcall TForm3(TComponent* Owner, TData &AData);
};
//---------------------------------------------------------------------------
#endif
