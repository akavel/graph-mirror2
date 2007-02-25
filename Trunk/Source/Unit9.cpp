/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "Unit9.h"
#include <complex>
#include <float.h>
//---------------------------------------------------------------------------
#pragma link "TStdFuncFrame"
#pragma link "TTanFrame"
#pragma link "TParFuncFrame"
#pragma link "TPolFuncFrame"
#pragma link "TAreaFrame"
#pragma link "TntStdCtrls"
#pragma link "TAreaFrame"
#pragma link "TParFuncFrame"
#pragma link "TPolFuncFrame"
#pragma link "TStdFuncFrame"
#pragma link "TTanFrame"
#pragma link "TAreaFrame"
#pragma link "TParFuncFrame"
#pragma link "TPolFuncFrame"
#pragma link "TStdFuncFrame"
#pragma link "TTanFrame"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm9::TForm9(TComponent* Owner)
    : TTntForm(Owner), VisibleFrame(NULL), SizeDif(Height - ClientHeight), EvalType(etEval)
{
  ScaleForm(this);
  StdFuncStrings->Assign(StdFuncFrame1->ComboBox1->Items);
  ParFuncStrings->Assign(ParFuncFrame1->ComboBox1->Items);
  PolFuncStrings->Assign(PolFuncFrame1->ComboBox1->Items);
  TranslateComponent(this);
  Translate();
}
//---------------------------------------------------------------------------
void TForm9::Translate()
{
  RetranslateComponent(this);
  StdFuncFrame1->ComboBox1->Items->Assign(StdFuncStrings);
  TranslateStrings(StdFuncFrame1->ComboBox1->Items);
  ParFuncFrame1->ComboBox1->Items->Assign(ParFuncStrings);
  TranslateStrings(ParFuncFrame1->ComboBox1->Items);
  PolFuncFrame1->ComboBox1->Items->Assign(PolFuncStrings);
  TranslateStrings(PolFuncFrame1->ComboBox1->Items);
  AreaFrame1->Label3->Caption = LoadRes(EvalType == etArea ? RES_AREA : RES_LENGTH) + WideString(L':');
  SetAccelerators(this);

  int Left = AreaFrame1->Label1->Left + TMaxWidth(AreaFrame1->Label1)(AreaFrame1->Label2)(Canvas->TextWidth(LoadRes(RES_AREA)))(Canvas->TextWidth(LoadRes(RES_LENGTH))) + 5;
  int Width = AreaFrame1->Width - Left - 9;
  AreaFrame1->Edit1->Left = Left;
  AreaFrame1->Edit1->Width = Width;
  AreaFrame1->Edit2->Left = Left;
  AreaFrame1->Edit2->Width = Width;
  AreaFrame1->Edit3->Left = Left;
  AreaFrame1->Edit3->Width = Width;

  ResizeControl(StdFuncFrame1->ComboBox1, StdFuncFrame1->Label5);
  ResizeControl(ParFuncFrame1->ComboBox1, ParFuncFrame1->Label7);
  ResizeControl(PolFuncFrame1->ComboBox1, PolFuncFrame1->Label7);

  StdFuncFrame1->ComboBox1->ItemIndex = 0;
  ParFuncFrame1->ComboBox1->ItemIndex = 0;
  PolFuncFrame1->ComboBox1->ItemIndex = 0;
}
//---------------------------------------------------------------------------
//User clicked on a (X,Y) pixel coordinate
void TForm9::StartValueChanged(int X, int Y)
{
  try
  {
    if(!Elem || !Visible)
      return;

    switch(EvalType)
    {
      case etArea:
      case etArc:
        if(TBaseFuncType *Func = dynamic_cast<TBaseFuncType*>(Elem.get()))
        {
          double t = FindNearestPoint(Func, X, Y); //Returns NAN if no point found
          if(_isnan(t))
          {
            Form1->Cross->Hide();
            AreaFrame1->Edit1->Text = "";
            AreaFrame1->Edit2->Text = "";
          }
          else
          {
            AreaFrame1->Edit1->Text = RoundToStr(t, Form1->Data);
            AreaFrame1->Edit2->Text = AreaFrame1->Edit1->Text;
          }
        }
        break;

      default:
        if(VisibleFrame)
          VisibleFrame->SetPoint(Elem.get(), X, Y);
    }
  }
  catch(Func32::EFuncError &Error)
  {
    if(Error.ErrorCode != Func32::ecEmptyString)
      Form1->ShowStatusError(GetErrorMsg(Error));
  }
}
//---------------------------------------------------------------------------
void TForm9::EndValueChanged(int X, int Y)
{
  if(TBaseFuncType *Func = dynamic_cast<TBaseFuncType*>(Elem.get()))
  {
    double t = FindNearestPoint(Func, X, Y);
    if(EvalType == etArea || EvalType == etArc)
      AreaFrame1->Edit2->Text = _isnan(t) ? AnsiString("") : RoundToStr(t, Form1->Data);
  }
}
//---------------------------------------------------------------------------
void TForm9::FuncChanged(const boost::shared_ptr<TGraphElem> &AElem)
{
  Elem = AElem;

  if(!Elem)
  {
    ShowFrame(NULL);
    return;
  }
  switch(EvalType)
  {
    case etEval:
      if(dynamic_cast<TStdFunc*>(Elem.get()))
        ShowFrame(StdFuncFrame1);
      else if(dynamic_cast<TParFunc*>(Elem.get()))
        ShowFrame(ParFuncFrame1);
      else if(dynamic_cast<TPolFunc*>(Elem.get()))
        ShowFrame(PolFuncFrame1);
      else if(dynamic_cast<TTan*>(Elem.get()) || dynamic_cast<TPointSeries*>(Elem.get()))
        ShowFrame(TanFrame1);
      else
        ShowFrame(NULL);

      Edit1Change(NULL);
      break;

    case etArea:
    case etArc:
      if(dynamic_cast<TBaseFuncType*>(Elem.get()))
      {
        ShowFrame(AreaFrame1); //Only show area frame for functions and tangents
        Edit1Change(NULL);
      }
      else
        ShowFrame(NULL);
      break;

    default:
      ShowFrame(NULL);
      break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm9::FormHide(TObject *Sender)
{
  Form1->Cross->Hide();
  Form1->EvalAction->Checked = false;
  Form1->PathAction->Checked = false;
  Form1->AreaAction->Checked = false;
  Form1->AnimateAction->Checked = false;
  Form1->Panel1->Height = 0;

  Form1->IPolygon1->Hide();
}
//---------------------------------------------------------------------------
void TForm9::Clear()
{
  Hide();
  Elem.reset();
}
//---------------------------------------------------------------------------
void __fastcall TForm9::Edit1Change(TObject *Sender)
{
  try
  {
    Form1->Cross->Hide();
    Form1->IPolygon1->Hide();
    if(!Visible)
      return;

    Form1->CancelStatusError();
    switch(EvalType)
    {
      case etEval:
        if(VisibleFrame)
          VisibleFrame->Eval(Elem.get());
        break;

      case etArea:
        AreaFrame1->EvalArea(Elem.get());
        break;

      case etArc:
        AreaFrame1->EvalArc(Elem.get());
        break;
    }
  }
  catch(Func32::EFuncError &Error)
  {
    if(Error.ErrorCode != Func32::ecEmptyString)
      Form1->ShowStatusError(ToWideString(VisibleFrame->GetErrorPrefix()) + GetErrorMsg(Error));
  }
  catch(EOverflow&)
  {
    Form1->ShowStatusError(LoadRes(RES_Overflow));
  }
  catch(EAbort&)
  { //Ignore
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm9::FormShortCut(TWMKey &Msg, bool &Handled)
{
  if(GetKeyState(VK_CONTROL))
    switch(Msg.CharCode)
    {
      case 'X':
        ActiveControl->Perform(WM_CUT, 0, 0);
        Handled = true;
        break;
      case 'C':
        ActiveControl->Perform(WM_COPY, 0, 0);
        Handled = true;
        break;
      case 'V':
        ActiveControl->Perform(WM_PASTE, 0, 0);
        Handled = true;
        break;
      case 'A':
        ActiveControl->Perform(EM_SETSEL, 0, -1);
        Handled = true;
        break;
      case 'Z':
        ActiveControl->Perform(WM_UNDO, 0, 0);
        Handled = true;
        break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm9::FormStartDock(TObject *Sender,
      TDragDockObject *&DragObject)
{
  //Graphics error when hints are shown when draging
  Form1->StatusBar1->AutoHint = false;

  //This is necessary to take care of a resize problem when first undocked.
  //The problem was introduced in Graph 2.7 with no obvius cause.
  //It may be Patch 4 for BCB6 that introduced it.
  if(VisibleFrame)
    GetFrame()->Width = ClientWidth;
}
//---------------------------------------------------------------------------
void __fastcall TForm9::FormEndDock(TObject *Sender, TObject *Target,
      int X, int Y)
{
  Form1->StatusBar1->AutoHint = true;
}
//---------------------------------------------------------------------------
void TForm9::ShowFrame(TEvalFrame *Frame)
{
  if(Frame != VisibleFrame)
  {
    if(VisibleFrame)
      GetFrame()->Hide();

    VisibleFrame = Frame;

    if(VisibleFrame)
    {
      GetFrame()->Show();
      ClientHeight = GetFrame()->Height;
    }
    if(Form1->Panel1->Height)
      Form1->Panel1->Height = VisibleFrame ? GetFrame()->Height+10 : 110;
    UndockHeight = ClientHeight + SizeDif;
  }

  Edit1Change(NULL);
}
//---------------------------------------------------------------------------
void TForm9::SetEvalType(TEvalType AEvalType)
{
  if(AEvalType != etArc && AEvalType != etArea)
    Form1->IPolygon1->Hide();

  EvalType = AEvalType;
  Visible = true;
  if(Form1->Panel4->VisibleDockClientCount)
    Form1->Panel1->Height = VisibleFrame ? GetFrame()->Height+10 : 100;
  switch(EvalType)
  {
    case etEval:
      Caption = LoadRes(527);
      break;

    case etArea:
    case etArc:
      AreaFrame1->Label3->Caption = LoadRes(EvalType == etArea ? RES_AREA : RES_LENGTH) + WideString(L':');
      Caption = LoadRes(EvalType == etArea ? 522 : 523);
      break;
  }
  FuncChanged(Elem);
}
//---------------------------------------------------------------------------
void __fastcall TForm9::FormResize(TObject *Sender)
{
  if(VisibleFrame)
    GetFrame()->Width = ClientWidth;
}
//---------------------------------------------------------------------------

