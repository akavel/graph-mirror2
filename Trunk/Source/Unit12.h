/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit12H
#define Unit12H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include "LineSelect.h"
#include <ComCtrls.hpp>
#include "ExtColorBox.h"
#include "MyEdit.h"
#include "TntExtCtrls.hpp"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TForm12 : public TTntForm
{
__published:	// IDE-managed Components
    TTntButton *Button1;
    TTntButton *Button2;
    TMyEdit *Edit1;
    TTntLabel *Label1;
    TTntRadioGroup *RadioGroup1;
    TTntGroupBox *GroupBox1;
    TTntLabel *Label10;
    TMyEdit *Edit2;
    TTntGroupBox *GroupBox2;
    TTntLabel *Label7;
    TTntLabel *Label8;
    TTntLabel *Label9;
    TTntEdit *Edit3;
    TUpDown *UpDown1;
    TLineSelect *LineSelect1;
    TTntButton *Button3;
    TTntGroupBox *GroupBox3;
    TTntLabel *Label4;
    TTntLabel *Label5;
    TMyEdit *Edit4;
    TMyEdit *Edit5;
  TTntGroupBox *GroupBox5;
  TTntLabel *Label11;
  TTntLabel *Label12;
  TTntComboBox *ComboBox1;
  TTntComboBox *ComboBox2;
  TExtColorBox *ExtColorBox1;

    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Edit3KeyPress(TObject *Sender, char &Key);
    void __fastcall Button3Click(TObject *Sender);
  void __fastcall ComboBoxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);

private:	// User declarations
    TData &Data;
    boost::shared_ptr<TBaseFuncType> Func;
    int Index;

public:		// User declarations
    __fastcall TForm12(TComponent* Owner, TData &AData);
    int EditTan(const boost::shared_ptr<TBaseFuncType> &F, int AIndex);
    int InsertTan(const boost::shared_ptr<TBaseFuncType> &F);
};
//---------------------------------------------------------------------------
#endif
