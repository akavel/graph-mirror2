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
  ScaleForm(this);

  Popup1_Show_df->Caption = LoadRes(RES_SHOW, "f'(x)");
  Popup1_Show_ddf->Caption = LoadRes(RES_SHOW, "f''(x)");

  TranslateProperties(this);
  MoveControl(Edit1, Label1);
  MoveLabel(Edit2, Label2);
  Edit3->Left = Edit1->Left;

  //Initialize as dialog and change to sizeable to prevent change in client size when the border size changes
  BorderStyle = bsSizeable;
}
//---------------------------------------------------------------------------
void __fastcall TForm15::Button2Click(TObject *Sender)
{
  double ds = MakeFloat(Edit3, LoadRes(RES_GREATER_ZERO, "d" + Func->GetVariable()), 0.00001);
  double Min = MakeFloat(Edit1);
  double Max = MakeFloat(Edit2, LoadRes(511), Min);
  int Digits = GetDecimals(ds);

  Grid1->RowCount = (Max - Min)/ds + 2.5;
  Grid1->FixedRows = 1;

  ProgressForm1->Max = Grid1->RowCount;
  ProgressForm1->Position = 0;
  ProgressForm1->Show();

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

    long double x = Min;
    for(int N = 1; N < Grid1->RowCount; ++N, x += ds)
    {
      Grid1->Cells[0][N] = DoubleToStr(x, (x >= 10000 || x <= -10000) ? 3 : Digits);
      //Calculate back to take care of rounding. What is written is also what is used for evaluation
      x = Grid1->Cells[0][N].ToDouble();
      AnsiString y, dy, ddy; //Default to empty string
      try
      {
        y = DoubleToStr(F->GetFunc().CalcY(x));
        dy = DoubleToStr(Dif1.CalcY(x));
        ddy = DoubleToStr(Dif2.CalcY(x));
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
    for(int N = 1; N < Grid1->RowCount; ++N, t += ds)
    {
      Grid1->Cells[0][N] = DoubleToStr(t, (t >= 10000 || t <= -10000) ? 3 : Digits);
      //Calculate back to take care of rounding. What is written is also what is used for evaluation
      t = Grid1->Cells[0][N].ToDouble();

      try
      {
        Grid1->Cells[1][N] = DoubleToStr(ParFunc->GetFunc().CalcX(t));
      }
      catch(Func32::EFuncError&)
      {
      }
      try
      {
        Grid1->Cells[2][N] = DoubleToStr(ParFunc->GetFunc().CalcY(t));
      }
      catch(Func32::EFuncError&)
      {
      }
      if(N%1000 == 0)
        ProgressForm1->StepIt();
      if(ProgressForm1->AbortProgress)
        break;
    }
  }
  else if(const TPolFunc *PolFunc = dynamic_cast<const TPolFunc*>(Func))
  {
    double t = Min;
    for(int N = 1; N < Grid1->RowCount; ++N, t += ds)
    {
      Grid1->Cells[0][N] = DoubleToStr(t, (t >= 10000 || t <= -10000) ? 3 : Digits);
      //Calculate back to take care of rounding. What is written is also what is used for evaluation
      t = Grid1->Cells[0][N].ToDouble();
      AnsiString r, x, y;
      try
      {
        r = DoubleToStr(PolFunc->GetFunc().CalcR(t));
        x = DoubleToStr(PolFunc->GetFunc().CalcX(t));
        y = DoubleToStr(PolFunc->GetFunc().CalcY(t));
      }
      catch(Func32::ECalcError&)
      { //Ignore errors and continue
      }

      Grid1->Cells[1][N] = r;
      Grid1->Cells[2][N] = x;
      Grid1->Cells[3][N] = y;

      if(N%1000 == 0)
        ProgressForm1->StepIt();
      if(ProgressForm1->AbortProgress)
        break;
    }
  }
  ProgressForm1->Close();
}
//---------------------------------------------------------------------------
void TForm15::ShowTable(const TBaseFuncType *F)
{
  Func = F;

  if(dynamic_cast<const TStdFunc*>(Func))
  {
    Grid1->ColCount = 4;
    Grid1->Cells[0][0] = "x";
    Grid1->Cells[1][0] = "f(x)";
    Grid1->Cells[2][0] = "f'(x)";
    Grid1->Cells[3][0] = "f''(x)";
  }
  else if(dynamic_cast<const TParFunc*>(Func))
  {
    Popup1_Show_df->Visible = false;
    Popup1_Show_ddf->Visible = false;
    Grid1->ColCount = 3;
    Grid1->Cells[0][0] = "t";
    Grid1->Cells[1][0] = "x(t)";
    Grid1->Cells[2][0] = "y(t)";
    Label4->Caption = "dt=";
  }
  else if(dynamic_cast<const TPolFunc*>(Func))
  {
    Popup1_Show_df->Visible = false;
    Popup1_Show_ddf->Visible = false;
    Grid1->ColCount = 4;
    Grid1->Cells[0][0] = "t";
    Grid1->Cells[1][0] = "r(t)";
    Grid1->Cells[2][0] = "x(t)";
    Grid1->Cells[3][0] = "y(t)";
    Label4->Caption = "dt=";
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
  Grid1->ColWidths[2] = Popup1_Show_df->Checked ? Grid1->DefaultColWidth : 0;
  Grid1->ColWidths[3] = Popup1_Show_ddf->Checked ? Grid1->DefaultColWidth : 0;
}
//---------------------------------------------------------------------------

