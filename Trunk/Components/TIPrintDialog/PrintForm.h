//===========================================================================
// Copyright © 2004 Ivan Johansen
// PrintForm.h
//===========================================================================
#ifndef PrintFormH
#define PrintFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <TntForms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "TntExtCtrls.hpp"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TPrintFrm : public TTntForm
{
__published:	// IDE-managed Components
  TShape *Shape1;
  TShape *Shape2;
  TTntButton *Button1;
  TTntButton *Button2;
  TTntGroupBox *GroupBox1;
  TTntComboBox *ComboBox1;
  TTntLabel *Label1;
  TTntRadioGroup *RadioGroup1;
  TTntRadioGroup *RadioGroup2;
  TTntGroupBox *GroupBox2;
  TTntCheckBox *CheckBox1;
  TTntLabel *Label3;
  TTntLabel *Label4;
  TTntEdit *Edit1;
  TTntEdit *Edit2;
  TGroupBox *GroupBox3;
  TTntLabel *Label5;
  TTntEdit *Edit3;
  TUpDown *UpDown3;
  TUpDown *UpDown4;
  TTntEdit *Edit4;
  TTntLabel *Label6;
  TImage *Image1;
  TTntButton *Button3;
  TTntLabel *Label7;
  TTntLabel *Label8;
  TTntLabel *Label9;
  TLabel *Label11;
  TLabel *Label12;
  TLabel *Label13;
  TUpDown *UpDown1;
  TUpDown *UpDown2;
  void __fastcall EditKeyPress(TObject *Sender, char &Key);
  void __fastcall CheckBox1Click(TObject *Sender);
  void __fastcall RadioGroup1Click(TObject *Sender);
  void __fastcall Edit1Change(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall ComboBox1Change(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);

private:	// User declarations
  class TIPrintDialog *PrintDialog;

  void PrinterChanged();
  void ResizePaper();
  void DoPaintSample();

public:		// User declarations
  __fastcall TPrintFrm(TComponent* Owner, TIPrintDialog *APrintDialog);
};
//---------------------------------------------------------------------------
#endif
