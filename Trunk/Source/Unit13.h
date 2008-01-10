/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit13H
#define Unit13H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Graphics.hpp>
#include "LineSelect.h"
#include "MyRadioButton.h"
#include "MyEdit.h"
#include <Grids.hpp>
#include <ValEdit.hpp>
#include "ExtColorBox.h"
#include "TntComCtrls.hpp"
#include "TntExtCtrls.hpp"
#include "TntStdCtrls.hpp"
#include "TntDialogs.hpp"
#include "TntMenus.hpp"
#include <Dialogs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm13 : public TTntForm
{
__published:	// IDE-managed Components
  TTntButton *Button1;
  TTntButton *Button2;
  TTntButton *Button3;
  TTntGroupBox *GroupBox1;
  TTntLabel *Label5;
  TTntLabel *Label6;
  TTntLabel *Label7;
  TTntEdit *Edit2;
  TLineSelect *LineSelect1;
  TUpDown *UpDown1;
  TTntPageControl *PageControl1;
  TTntTabSheet *TabSheet1;
  TTntTabSheet *TabSheet2;
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
  TTntEdit *Edit1;
  TTntStaticText *StaticText1;
  TUpDown *UpDown2;
  TMyRadioButton *RadioButton3;
  TMyRadioButton *RadioButton2;
  TMyRadioButton *RadioButton1;
  TMyRadioButton *RadioButton4;
  TMyRadioButton *RadioButton5;
  TListBox *ListBox1;
  TTntButton *Button4;
  TTntButton *Button5;
  TTntLabel *Label1;
  TTntButton *Button6;
  TMyEdit *Edit3;
  TPanel *Panel6;
  TTntImage *Image6;
  TMyRadioButton *RadioButton6;
  TTntStaticText *StaticText2;
  TTntEdit *Edit4;
  TUpDown *UpDown3;
  TExtColorBox *ExtColorBox1;
  TMyEdit *Edit5;
  TTntCheckBox *CheckBox1;
        TTntPopupMenu *TntPopupMenu1;
        TTntMenuItem *Popup_Export;
        TTntMenuItem *Popup_Import;
        TSaveDialog *SaveDialog1;
        TOpenDialog *OpenDialog1;
  void __fastcall ImageClick(TObject *Sender);
  void __fastcall EditKeyPress(TObject *Sender, char &Key);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall ListBox1Click(TObject *Sender);
  void __fastcall Button4Click(TObject *Sender);
  void __fastcall Button6Click(TObject *Sender);
  void __fastcall Button5Click(TObject *Sender);
  void __fastcall Edit3Change(TObject *Sender);
  void __fastcall RadioButtonClick(TObject *Sender);
  void __fastcall Edit5Change(TObject *Sender);
  void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall Popup_ImportClick(TObject *Sender);
        void __fastcall Popup_ExportClick(TObject *Sender);

private:	// User declarations
  boost::shared_ptr<TPointSeries> Series;
  TData &Data;

  void ShowUserModels(const std::wstring &Selected);

public:		// User declarations
  __fastcall TForm13(TComponent* Owner, TData &AData);
  int InsertTrendline(const boost::shared_ptr<TPointSeries> &ASeries);
};
//---------------------------------------------------------------------------
#endif

