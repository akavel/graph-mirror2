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
#include "Unit15.h"
#include <cmath>
//---------------------------------------------------------------------------
#pragma link "Grid"
#pragma link "ProgressForm"
#pragma link "TntMenus"
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm15::TForm15(TComponent* Owner)
        : TTntForm(Owner), Func(NULL)
{
  TranslateProperties(this);
  MoveControl(Edit1, Label1);
  MoveLabel(Edit2, Label2);
  Edit3->Left = Edit1->Left;

  //Initialize as dialog and change to sizeable to prevent change in client size when the border size changes
  BorderStyle = bsSizeable;
  ScaleForm(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm15::Button2Click(TObject *Sender)
{
  double ds = std::abs(MakeFloat(Edit3));
  double Min = MakeFloat(Edit1);
  double Max = MakeFloat(Edit2);
  int Digits = GetDecimals(ds);

  if(ds == 0)
  {
    Grid1->RowCount = 2;
    for(int I = 0; I < Grid1->ColCount; I++)
      Grid1->Cells[I][1] = "";
    return;
  }

  Grid1->RowCount = std::abs(Max - Min)/ds + 2.5;
  Grid1->FixedRows = 1;

  ProgressForm1->Max = Grid1->RowCount;
  ProgressForm1->Position = 0;
  ProgressForm1->Show();

  if(Max < Min)
    ds = -ds;

  if(const TStdFunc *F = dynamic_cast<const TStdFunc*>(Func))
  {
    Func32::TFunc Dif1;
    Func32::TFunc Dif2;
    try
    {
      Dif1 = F->GetFunc().MakeDif();
      Dif2 = Dif1.MakeDif();
    }
    catch(...)
    {
    }

    bool UseReal = Form1->Data.Property.ComplexFormat == cfReal;
    long double x = Min;
    for(int N = 1; N < Grid1->RowCount; ++N, x += ds)
    {
      AnsiString Str = DoubleToStr(x, (x >= 10000 || x <= -10000) ? 3 : Digits);
      //Calculate back to take care of rounding. What is written is also what is used for evaluation
      x = Str.ToDouble();
      Grid1->Cells[0][N] = Str;
      WideString y, dy, ddy; //Default to empty string
      const Func32::TFunc &StdFunc = F->GetFunc();
      try
      {
        y = ComplexToWideString(UseReal ? Func32::TComplex(StdFunc.CalcY(x)) : StdFunc.CalcY(Func32::TComplex(x)));
        dy = ComplexToWideString(UseReal ? Func32::TComplex(Dif1.CalcY(x)) : Dif1.CalcY(Func32::TComplex(x)));
        ddy = ComplexToWideString(UseReal ? Func32::TComplex(Dif2.CalcY(x)) : Dif2.CalcY(Func32::TComplex(x)));
      }
      catch(Func32::ECalcError&)
      { //Ignore errors and continue
      }

      Grid1->Cells[1][N] = y;
      Grid1->Cells[2][N] = dy;
      Grid1->Cells[3][N] = ddy;

      if(N%1000 == 0)
        ProgressForm1->StepIt();
      if(ProgressForm1->AbortProgress)
        break;
    }
  }
  else if(const TParFunc *ParFunc = dynamic_cast<const TParFunc*>(Func))
  {
    double t = Min;
    Func32::TParamFunc F = ParFunc->GetFunc();
    Func32::TParamFunc Dif = F.MakeDif();
    for(int N = 1; N < Grid1->RowCount; ++N, t += ds)
    {
      AnsiString Str = DoubleToStr(t, (t >= 10000 || t <= -10000) ? 3 : Digits);
      //Calculate back to take care of rounding. What is written is also what is used for evaluation
      t = Str.ToDouble();
      Grid1->Cells[0][N] = Str;
      long double xDif = 0;
      long double yDif = 0;
      try
      {
        Grid1->Cells[1][N] = DoubleToStr(F.CalcX(t));
        xDif = Dif.CalcX(t);
        Grid1->Cells[3][N] = DoubleToStr(xDif);
      }
      catch(Func32::EFuncError&)
      {
        Grid1->Cells[1][N] = "";
        Grid1->Cells[3][N] = "";
      }

      try
      {
        Grid1->Cells[2][N] = DoubleToStr(F.CalcY(t));
        yDif = Dif.CalcY(t);
        Grid1->Cells[4][N] = DoubleToStr(yDif);
      }
      catch(Func32::EFuncError&)
      {
        Grid1->Cells[2][N] = "";
        Grid1->Cells[4][N] = "";
      }

      if(xDif != 0)
        Grid1->Cells[5][N] = DoubleToStr(yDif / xDif);
      else
        Grid1->Cells[5][N] = "";

      if(N % 1000 == 0)
        ProgressForm1->StepIt();
      if(ProgressForm1->AbortProgress)
        break;
    }
  }
  else if(const TPolFunc *PolFunc = dynamic_cast<const TPolFunc*>(Func))
  {
    Func32::TPolarFunc F = PolFunc->GetFunc();
    Func32::TPolarFunc Dif = F.MakeDif();
    double t = Min;
    for(int N = 1; N < Grid1->RowCount; ++N, t += ds)
    {
      AnsiString Str = DoubleToStr(t, (t >= 10000 || t <= -10000) ? 3 : Digits);
      //Calculate back to take care of rounding. What is written is also what is used for evaluation
      t = Str.ToDouble();
      Grid1->Cells[0][N] = Str;
      AnsiString r, x, y, dr, dydx;
      try
      {
        r = DoubleToStr(F.CalcR(t));
        x = DoubleToStr(F.CalcX(t));
        y = DoubleToStr(F.CalcY(t));
        dr = DoubleToStr(Dif.CalcR(t));
        dydx = DoubleToStr(F.CalcSlope(t));
      }
      catch(Func32::ECalcError&)
      { //Ignore errors and continue
      }

      Grid1->Cells[1][N] = r;
      Grid1->Cells[2][N] = x;
      Grid1->Cells[3][N] = y;
      Grid1->Cells[4][N] = dr;
      Grid1->Cells[5][N] = dydx;

      if(N % 1000 == 0)
        ProgressForm1->StepIt();
      if(ProgressForm1->AbortProgress)
        break;
    }
  }
  ProgressForm1->Close();
}
//---------------------------------------------------------------------------
void TForm15::SetCaptions(const char*const* Captions)
{
  for(int I = 0; I < 5; I++)
  {
    Grid1->Cells[I+1][0] = ToWideString(Captions[I]);
    PopupMenu1->Items->Items[I]->Caption = LoadRes(RES_SHOW, Captions[I]);
    PopupMenu1->Items->Items[I]->Hint = AnsiString().sprintf(PopupMenu1->Items->Items[I]->Hint.c_str(), Captions[I]);
    PopupMenu1->Items->Items[I]->Visible = Captions[I][0] != '\0';
  }
}
//---------------------------------------------------------------------------
void TForm15::ShowTable(const TBaseFuncType *F)
{
  Func = F;

  if(dynamic_cast<const TStdFunc*>(Func))
  {
    const char *Captions[] = {"f(x)", "f'(x)", "f''(x)", "", ""};
    Grid1->ColCount = 4;
    Grid1->Cells[0][0] = "x";
    SetCaptions(Captions);
    Label4->Caption = L"\x394x=";
  }
  else if(dynamic_cast<const TParFunc*>(Func))
  {
    const char *Captions[] = {"x(t)", "y(t)", "dx/dt", "dy/dt", "dy/dx"};
    Grid1->ColCount = 6;
    Grid1->Cells[0][0] = "t";
    Label4->Caption = L"\x394t=";
    SetCaptions(Captions);
  }
  else if(dynamic_cast<const TPolFunc*>(Func))
  {
    const char *Captions[] = {"r(t)", "x(t)", "y(t)", "dr/dt", "dy/dx"};
    Grid1->ColCount = 6;
    Grid1->Cells[0][0] = "t";
    Label4->Caption = L"\x394t=";
    SetCaptions(Captions);
  }

  ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm15::Popup1_CopyClick(TObject *Sender)
{
  Grid1->CopyToClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TForm15::Popup1_SelectClick(TObject *Sender)
{
  Grid1->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TForm15::Button3Click(TObject *Sender)
{
  Application->HelpContext(HelpContext);
}
//---------------------------------------------------------------------------
void __fastcall TForm15::FormResize(TObject *Sender)
{
  Grid1->Width = ClientWidth - 2 * Grid1->Left;
  Grid1->Height = Button1->Top - Grid1->Top - 15;
}
//---------------------------------------------------------------------------
void __fastcall TForm15::Popup1_ExportClick(TObject *Sender)
{
  if(SaveDialog1->Execute())
    if(!Grid1->ExportToFile(SaveDialog1->FileName, SaveDialog1->FilterIndex == 1 ? ';' : '\t'))
      MessageBox(LoadRes(RES_FILE_ACCESS, SaveDialog1->FileName), LoadRes(RES_WRITE_FAILED), MB_ICONSTOP);
}
//---------------------------------------------------------------------------
void __fastcall TForm15::Popup1_Show(TObject *Sender)
{
  TMenuItem *MenuItem = static_cast<TMenuItem*>(Sender);
  int Index = MenuItem->MenuIndex;
  Grid1->ColWidths[Index+1] = MenuItem->Checked ? Grid1->DefaultColWidth : -1;
}
//---------------------------------------------------------------------------

