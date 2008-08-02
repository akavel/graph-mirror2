/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit18H
#define Unit18H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "TntComCtrls.hpp"
#include "TntExtCtrls.hpp"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
enum TImageFormat {ifMetafile = 1, /*ifPostScript,*/ ifSvg, ifBitmap, ifPng, ifJpeg, ifPdf};

class TForm18 : public TTntForm
{
__published:	// IDE-managed Components
  TTntPageControl *PageControl1;
  TTntTabSheet *TabSheet1;
  TTntTabSheet *TabSheet2;
  TTntButton *Button1;
  TTntButton *Button2;
  TTntLabel *Label1;
  TTntEdit *Edit1;
  TTntLabel *Label2;
  TTntEdit *Edit2;
  TTntRadioGroup *RadioGroup1;
  TTntGroupBox *GroupBox1;
  TTrackBar *TrackBar1;
  TEdit *Edit3;
  TTntTabSheet *TabSheet3;
  TTntRadioGroup *RadioGroup2;
  TTntLabel *Label3;
  TTntComboBox *ComboBox1;
  TTntCheckBox *CheckBox1;
  TTntRadioGroup *RadioGroup3;
  void __fastcall EditKeyPress(TObject *Sender, char &Key);
  void __fastcall TrackBar1Change(TObject *Sender);
  void __fastcall Edit3Change(TObject *Sender);
  void __fastcall RadioGroup3Click(TObject *Sender);
private:	// User declarations
  int ScreenWidth;
  int ScreenHeight;
  int CustomWidth;
  int CustomHeight;

public:		// User declarations
  __fastcall TForm18(TComponent* Owner);
  void EditOptions(struct TImageOptions &Options, TImageFormat ImageFormat, int AWidth, int AHeight);
};

enum TPaperSize {psA2, psA3, psA4, psA5, psA6, psLetter, psLedger, psLegal};
struct TImageOptions
{
  int CustomWidth;
  int CustomHeight;
  bool UseCustomSize;

  struct TJpeg
  {
    unsigned Quality;
    bool ProgressiveEncoding;
    TJpeg() : Quality(90), ProgressiveEncoding(false) {}
  } Jpeg;

  struct TPdf
  {
    TPrinterOrientation Orientation;
    TPaperSize PaperSize;
    TPdf() : Orientation(poPortrait), PaperSize(psA4) {}
  } Pdf;

  TImageOptions(unsigned AWidth, unsigned AHeight)
    : CustomWidth(AWidth), CustomHeight(AHeight), UseCustomSize(false) {}
  void LoadSettings();
  void SaveSettings() const;
};
//---------------------------------------------------------------------------
#endif
