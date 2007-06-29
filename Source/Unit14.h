/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit14H
#define Unit14H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <Grids.hpp>
#include <vcl\DBGrids.hpp>
#include <vcl\DB.hpp>
#include <vcl\DBTables.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include "Grid.h"
#include "PointSelect.h"
#include "LineSelect.h"
#include <Dialogs.hpp>
#include <Buttons.hpp>
#include "ExtColorBox.h"
#include "MyEdit.h"
#include "TntButtons.hpp"
#include "TntComCtrls.hpp"
#include "TntMenus.hpp"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TForm14 : public TTntForm
{
__published:	// IDE-managed Components
  TTntButton *Button1;
  TGrid *Grid;
  TTntPopupMenu *PopupMenu1;
  TTntMenuItem *Popup_Cut;
  TTntMenuItem *Popup_Copy;
  TTntMenuItem *Popup_Paste;
  TTntMenuItem *Popup_Delete;
  TTntMenuItem *N1;
  TTntMenuItem *Popup_Select;
  TTntButton *Button2;
  TMyEdit *Edit1;
  TTntLabel *Label3;
  TTntGroupBox *GroupBox4;
  TPaintBox *PaintBox1;
  TTntButton *Button3;
  TTntMenuItem *Popup_Insert;
  TTntMenuItem *N2;
  TTntMenuItem *Popup_Remove;
  TTntMenuItem *Popup_Import;
  TOpenDialog *OpenDialog1;
  TFontDialog *FontDialog1;
  TTntMenuItem *Popup_Export;
  TSaveDialog *SaveDialog1;
  TTntPageControl *PageControl1;
  TTntTabSheet *TabSheet1;
  TTntTabSheet *TabSheet2;
  TTntGroupBox *GroupBox1;
  TTntLabel *Label1;
  TTntLabel *Label2;
  TTntLabel *Label4;
  TTntEdit *Edit2;
  TUpDown *UpDown1;
  TPointSelect *PointSelect1;
  TTntGroupBox *GroupBox2;
  TTntLabel *Label5;
  TTntLabel *Label6;
  TTntLabel *Label7;
  TLineSelect *LineSelect1;
  TTntEdit *Edit3;
  TUpDown *UpDown2;
  TTntGroupBox *GroupBox3;
  TTntLabel *Label8;
  TTntCheckBox *CheckBox2;
  TTntBitBtn *BitBtn1;
  TTntComboBox *ComboBox1;
  TTntGroupBox *GroupBox5;
  TTntCheckBox *CheckBox3;
  TTntRadioButton *RadioButton1;
  TMyEdit *Edit4;
  TTntRadioButton *RadioButton2;
  TTntRadioButton *RadioButton3;
  TMyEdit *Edit5;
  TTntLabel *Label9;
  TTntGroupBox *GroupBox6;
  TTntLabel *Label10;
  TTntCheckBox *CheckBox4;
  TTntRadioButton *RadioButton4;
  TMyEdit *Edit6;
  TTntRadioButton *RadioButton5;
  TTntRadioButton *RadioButton6;
  TMyEdit *Edit7;
  TTntComboBox *ComboBox2;
  TTntLabel *Label11;
  TExtColorBox *ExtColorBox1;
  TExtColorBox *ExtColorBox2;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall Popup_CutClick(TObject *Sender);
  void __fastcall Popup_CopyClick(TObject *Sender);
  void __fastcall Popup_PasteClick(TObject *Sender);
  void __fastcall Popup_DeleteClick(TObject *Sender);
  void __fastcall Popup_SelectClick(TObject *Sender);
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall GridEditorKeyPress(TInplaceEdit *InplaceEdit,
    char &Key);
  void __fastcall PaintBox1Paint(TObject *Sender);
  void __fastcall Change(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall Popup_InsertClick(TObject *Sender);
  void __fastcall Popup_RemoveClick(TObject *Sender);
  void __fastcall Popup_ImportClick(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
  void __fastcall Popup_ExportClick(TObject *Sender);
  void __fastcall EditKeyPress(TObject *Sender, char &Key);
  void __fastcall RadioButtonClick(TObject *Sender);
  void __fastcall GridSetText(TObject *Sender, int ACol, int ARow,
          const AnsiString Value);
  void __fastcall GridGetText(TObject *Sender, int ACol, int ARow,
          AnsiString &Value);
  void __fastcall EditChange(TObject *Sender);

private:	// User declarations
  boost::shared_ptr<TPointSeries> Series;
  std::vector<TPointSeriesPoint> DataPoints;
  TData &Data;

  static HPEN SetPen(TColor Color, TPenStyle Style, int Width);
  void UpdateErrorBars();
  std::string& GetText(int ACol, int ARow);
  void Translate();

public:		// User declarations
  __fastcall TForm14(TComponent* Owner, TData &AData);
  int EditPointSeries(const boost::shared_ptr<TPointSeries> &P);
};
//---------------------------------------------------------------------------
#endif
