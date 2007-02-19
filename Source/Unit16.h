/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit16H
#define Unit16H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ShadeSelect.h"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "MyRadioButton.h"
#include <ComCtrls.hpp>
#include "ExtColorBox.h"
#include "TntComCtrls.hpp"
#include "TntExtCtrls.hpp"
#include "TntStdCtrls.hpp"
#include "MyEdit.h"
//---------------------------------------------------------------------------
class TForm16 : public TTntForm
{
__published:	// IDE-managed Components
  TTntPageControl *PageControl1;
  TTntTabSheet *TabSheet1;
  TTntTabSheet *TabSheet2;
  TTntTabSheet *TabSheet3;
  TPanel *Panel1;
  TTntImage *Image1;
  TPanel *Panel2;
  TTntImage *Image2;
  TPanel *Panel3;
  TTntImage *Image3;
  TPanel *Panel4;
  TTntImage *Image4;
  TPanel *Panel5;
  TTntImage *Image5;
  TPanel *Panel6;
  TTntImage *Image6;
  TMyRadioButton *RadioButton3;
  TMyRadioButton *RadioButton2;
  TMyRadioButton *RadioButton1;
  TMyRadioButton *RadioButton6;
  TMyRadioButton *RadioButton4;
  TMyRadioButton *RadioButton5;
  TTntButton *Button1;
  TTntButton *Button2;
  TTntButton *Button3;
  TTntListBox *ListBox1;
  TTntGroupBox *GroupBox2;
  TMyEdit *Edit3;
  TTntLabel *Label8;
  TTntLabel *Label9;
  TMyEdit *Edit4;
  TTntCheckBox *CheckBox4;
  TTntCheckBox *CheckBox3;
  TTntLabel *Label11;
  TTntGroupBox *GroupBox1;
  TTntLabel *Label7;
  TTntLabel *Label6;
  TMyEdit *Edit1;
  TMyEdit *Edit2;
  TTntCheckBox *CheckBox2;
  TTntCheckBox *CheckBox1;
  TTntGroupBox *GroupBox3;
  TTntLabel *Label5;
  TTntLabel *Label4;
  TShadeSelect *ShadeSelect1;
  TTntLabel *Label12;
  TTntEdit *Edit5;
  TTntCheckBox *CheckBox5;
  TTntCheckBox *CheckBox6;
  TExtColorBox *ExtColorBox1;
  void __fastcall ImageClick(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall ExtColorBox1Change(TObject *Sender);
  void __fastcall RadioButton6CheckedChange(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);

private:	// User declarations
  boost::shared_ptr<TBaseFuncType> Func;
  std::vector<boost::shared_ptr<TBaseFuncType> > FuncList;
  boost::shared_ptr<TShade> OldShade;
  TData &Data;

  void ShowFuncList();
  void EnableGroupBox2(bool Enable);

public:		// User declarations
  __fastcall TForm16(TComponent* Owner, TData &AData);
  int EditShade(const boost::shared_ptr<TShade> &AShade);
  TModalResult InsertShade(const boost::shared_ptr<TBaseFuncType> &AFunc);
};
//---------------------------------------------------------------------------
#endif

