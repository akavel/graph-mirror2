/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit19H
#define Unit19H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TntStdCtrls.hpp"
#include <MPlayer.hpp>
#include "ProgressForm.h"
//---------------------------------------------------------------------------
struct TAnimationInfo
{
  std::string Constant;
  double Min;
  double Max;
  double Step;
  unsigned Width;
  unsigned Height;
  double FramesPerSecond;

  TAnimationInfo() : Min(1), Max(10), Step(1), Width(320), Height(320), FramesPerSecond(1) {}
};

class TForm19 : public TTntForm
{
__published:	// IDE-managed Components
  TTntLabel *Label1;
  TTntComboBox *ComboBox1;
  TTntButton *Button1;
  TTntButton *Button2;
  TTntButton *Button3;
  TProgressForm *ProgressForm1;
  TTntButton *Button4;
  TTntGroupBox *TntGroupBox1;
  TTntGroupBox *TntGroupBox2;
  TTntLabel *Label2;
  TTntEdit *Edit1;
  TTntLabel *Label3;
  TTntEdit *Edit2;
  TTntLabel *Label4;
  TTntEdit *Edit3;
  TTntLabel *Label5;
  TTntEdit *Edit4;
  TTntLabel *Label6;
  TTntEdit *Edit5;
  TTntLabel *Label7;
  TTntEdit *Edit6;
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall Button4Click(TObject *Sender);
  void __fastcall TntEditKeyPress(TObject *Sender, char &Key);
private:	// User declarations
  TData Data;
  static ::TAnimationInfo AnimationInfo;

public:		// User declarations
  __fastcall TForm19(TComponent* Owner, const TData &AData);
};
//---------------------------------------------------------------------------
#endif
