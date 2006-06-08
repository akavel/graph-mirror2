/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include "LineSelect.h"
#include "MyEdit.h"
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include "ExtColorBox.h"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TForm5 : public TTntForm
{
__published:	// IDE-managed Components
	TTntButton *Button1;
	TTntButton *Button2;
  TTntComboBox *ComboBox1;
  TTntGroupBox *GroupBox1;
  TMyEdit *Edit1;
  TTntLabel *Label2;
  TTntLabel *Label3;
  TTntGroupBox *GroupBox2;
  TTntGroupBox *GroupBox4;
  TTntLabel *Label7;
  TTntLabel *Label8;
  TTntLabel *Label9;
  TTntEdit *Edit6;
  TUpDown *UpDown1;
  TLineSelect *LineSelect1;
  TTntLabel *Label4;
  TTntLabel *Label5;
  TTntEdit *Edit3;
  TTntEdit *Edit4;
  TTntButton *Button3;
  TTntLabel *Label1;
  TMyEdit *Edit2;
  TTntLabel *Label6;
  TTntEdit *Edit5;
  TTntGroupBox *GroupBox3;
  TTntLabel *Label10;
  TTntEdit *Edit7;
  TTntGroupBox *GroupBox5;
  TTntComboBox *ComboBox2;
  TTntComboBox *ComboBox3;
  TTntLabel *Label11;
  TTntLabel *Label12;
  TTntComboBox *ComboBox4;
  TTntLabel *Label13;
  TExtColorBox *ExtColorBox1;
	void __fastcall Button1Click(TObject *Sender);
  void __fastcall EditKeyPress(TObject *Sender, char &Key);
  void __fastcall ComboBox1Change(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall ComboBox1KeyPress(TObject *Sender, char &Key);
  void __fastcall ComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
  void __fastcall ComboBox4Change(TObject *Sender);
private:	// User declarations
  struct TTempData
  {
    AnsiString Text1, Text2, From, To, Steps;
  };

  TTempData TempData[3];
  int OldItemIndex;
  boost::shared_ptr<TBaseFuncType> F;
  TData &Data;

  void Translate();

public:		// User declarations
	__fastcall TForm5(TComponent* Owner, TData &AData);
  int EditFunc(boost::shared_ptr<TBaseFuncType> Func);
};
//---------------------------------------------------------------------------
#endif
