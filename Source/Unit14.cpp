/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include <Clipbrd.hpp>
#include "Unit14.h"
//---------------------------------------------------------------------------
#pragma link "Grid"
#pragma link "PointSelect"
#pragma link "LineSelect"
#pragma link "ExtColorBox"
#pragma link "MyEdit"
#pragma link "TntButtons"
#pragma link "TntComCtrls"
#pragma link "TntMenus"
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm14::TForm14(TComponent* Owner, TData &AData)
    : TTntForm(Owner), Data(AData)
{
  ScaleForm(this);
  Translate();            

  FontDialog1->Font->Assign(Data.Property.DefaultPointLabelFont);
  Edit1->Text = Data.CreatePointSeriesDescription().c_str();

  PointSelect1->ItemIndex = Data.Property.DefaultPoint.Style;
  ExtColorBox1->Selected = Data.Property.DefaultPoint.Color;
  UpDown1->Position = Data.Property.DefaultPoint.Size;

  LineSelect1->LineStyle = static_cast<TPenStyle>(Data.Property.DefaultPointLine.Style);
  ExtColorBox2->Selected = Data.Property.DefaultPointLine.Color;
  UpDown2->Position = Data.Property.DefaultPointLine.Size;

  ComboBox1->ItemIndex = 1;
  UpdateErrorBars();
  PageControl1->ActivePage = TabSheet1;
}
//---------------------------------------------------------------------------
void TForm14::Translate()
{
  TranslateProperties(this);
  TranslateStrings(ExtColorBox1->Items);
  TranslateStrings(ExtColorBox2->Items);
  SetAccelerators(this);

  ResizeControl(Edit1, Label3);
  ResizeControl(ComboBox1, Label8);
  int LabelWidth = TMaxWidth(Label1)(Label2)(Label4)(Label5)(Label6)(Label7)(Label11);
  if(LabelWidth > 61)
    Width = Width + LabelWidth - 61;

  ComboBox2->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm14::FormShow(TObject *Sender)
{
  //Grid needs do receive focus from something else
  Button1->SetFocus();
  Grid->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Popup_CutClick(TObject *Sender)
{
  Grid->CutToClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Popup_CopyClick(TObject *Sender)
{
  Grid->CopyToClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Popup_PasteClick(TObject *Sender)
{
  Grid->PasteFromClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Popup_DeleteClick(TObject *Sender)
{
  Grid->ClearSelection();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Popup_SelectClick(TObject *Sender)
{
  Grid->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::PopupMenu1Popup(TObject *Sender)
{
  Popup_Paste->Enabled = Clipboard()->HasFormat(CF_TEXT);
  Popup_Copy->Enabled = Grid->CanCopy();
  Popup_Cut->Enabled = Grid->CanCopy();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::GridEditorKeyPress(TInplaceEdit *InplaceEdit,
      char &Key)
{
  if(Key == '\n')
    Button1->Click();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Button1Click(TObject *Sender)
{
  boost::shared_ptr<TPointSeries> PointSeries(new TPointSeries);
  PointSeries->LegendText = ToWString(Edit1->Text);
  PointSeries->FrameColor = clBlack;
  PointSeries->FillColor = ExtColorBox1->Selected;
  PointSeries->LineColor = ExtColorBox2->Selected;
  PointSeries->Size = ToInt(Edit2->Text);
  PointSeries->LineSize = ToInt(Edit3->Text);
  PointSeries->Style = PointSelect1->ItemIndex;
  PointSeries->LineStyle = LineSelect1->LineStyle;
  PointSeries->Interpolation = static_cast<TInterpolationAlgorithm>(ComboBox2->ItemIndex);
  PointSeries->ShowLabels = CheckBox2->Checked;
  PointSeries->Font->Assign(FontDialog1->Font);
  PointSeries->LabelPosition = static_cast<TLabelPosition>(ComboBox1->ItemIndex);

  if(!CheckBox3->Checked)
    PointSeries->xErrorBarType = ebtNone;
  else if(RadioButton1->Checked)
  {
    PointSeries->xErrorValue = MakeFloat(Edit4, LoadRes(RES_POSITIVE, RadioButton1->Caption), 0);
    PointSeries->xErrorBarType = ebtFixed;
  }
  else if(RadioButton2->Checked)
  {
    PointSeries->xErrorValue = MakeFloat(Edit5, LoadRes(RES_POSITIVE, RadioButton2->Caption), 0);
    PointSeries->xErrorBarType = ebtRelative;
  }
  else
    PointSeries->xErrorBarType = ebtCustom;

  if(!CheckBox4->Checked)
    PointSeries->yErrorBarType = ebtNone;
  else if(RadioButton4->Checked)
  {
    PointSeries->yErrorValue = MakeFloat(Edit6, LoadRes(RES_POSITIVE, RadioButton4->Caption), 0);
    PointSeries->yErrorBarType = ebtFixed;
  }
  else if(RadioButton5->Checked)
  {
    PointSeries->yErrorValue = MakeFloat(Edit7, LoadRes(RES_POSITIVE, RadioButton5->Caption), 0);
    PointSeries->yErrorBarType = ebtRelative;
  }
  else
    PointSeries->yErrorBarType = ebtCustom;

  for(int Row = 1; Row < Grid->RowCount; Row++)
  {
    if(DataPoints[Row-1].x.Text.empty() && DataPoints[Row-1].y.Text.empty())
      continue;

    if(DataPoints[Row-1].x.Text.empty() || DataPoints[Row-1].y.Text.empty())
    {
      Grid->Col = DataPoints[Row-1].y.Text.empty();
      Grid->Row = Row;
      Grid->SetFocus();
      MessageBox(LoadRes(534), LoadRes(533));
      return;
    }

    DataPoints[Row-1].x.Value = CellToDouble(Grid, 0, Row);
    DataPoints[Row-1].y.Value = CellToDouble(Grid, 1, Row);

    if(PointSeries->xErrorBarType == ebtCustom && !Grid->Cells[2][Row].IsEmpty())
      DataPoints[Row-1].xError.Value = CellToDouble(Grid, 2, Row);

    if(PointSeries->yErrorBarType == ebtCustom)
    {
      int Col = PointSeries->xErrorBarType == ebtCustom ? 3 : 2;
      if(!Grid->Cells[Col][Row].IsEmpty())
        DataPoints[Row-1].yError.Value = CellToDouble(Grid, Col, Row);
    }

    PointSeries->PointList.push_back(DataPoints[Row-1]);
  }

  if(PointSeries->PointList.empty())
  {
    MessageBox(LoadRes(536), LoadRes(533), MB_ICONWARNING);
    return;
  }

  if(Series)
  {
    PointSeries->Visible = Series->Visible;
    PointSeries->ShowInLegend = Series->ShowInLegend;
    int Index = Data.GetIndex(Series);
    UndoList.Push(TUndoChange(Series, Index));
    Data.Replace(Index, PointSeries);
  }
  else
  {
    UndoList.Push(TUndoAdd(PointSeries));
    Data.Add(PointSeries);
  }

  Data.Property.DefaultPoint.Set(PointSelect1->ItemIndex, ExtColorBox1->Selected, ToInt(Edit2->Text));
  Data.Property.DefaultPointLine.Set(LineSelect1->LineStyle, ExtColorBox2->Selected, ToInt(Edit3->Text));
  Data.Property.DefaultPointLabelFont->Assign(FontDialog1->Font);

  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
int TForm14::EditPointSeries(const boost::shared_ptr<TPointSeries> &P)
{
  Series = P;
  if(P)
  {
    Caption = LoadRes(537);
    Edit1->Text = P->LegendText.c_str();
    ExtColorBox1->Selected = P->FillColor;
    UpDown1->Position = P->Size;
    PointSelect1->ItemIndex = P->Style;
    ExtColorBox2->Selected = P->LineColor;
    UpDown2->Position = P->LineSize;
    LineSelect1->LineStyle = P->LineStyle;
    ComboBox2->ItemIndex = P->Interpolation;
    Grid->RowCount = P->PointList.size() + 2;
    CheckBox2->Checked = P->ShowLabels;
    ComboBox1->Enabled = CheckBox2->Checked;
    FontDialog1->Font->Assign(P->Font);
    ComboBox1->ItemIndex = P->LabelPosition;

    DataPoints = P->PointList; //Create a working copy of all data

    CheckBox3->Checked = P->xErrorBarType != ebtNone;
    switch(P->xErrorBarType)
    {
      case ebtFixed:
        RadioButton1->Checked = true;
        Edit4->Text = ToWideString(P->xErrorValue);
        break;

      case ebtRelative:
        RadioButton2->Checked = true;
        Edit5->Text = ToWideString(P->xErrorValue);
        break;

      case ebtCustom:
        RadioButton3->Checked = true;
        break;
    }

    CheckBox4->Checked = P->yErrorBarType != ebtNone;
    switch(P->yErrorBarType)
    {
      case ebtFixed:
        RadioButton4->Checked = true;
        Edit6->Text = ToWideString(P->yErrorValue);
        break;

      case ebtRelative:
        RadioButton5->Checked = true;
        Edit7->Text = ToWideString(P->yErrorValue);
        break;

      case ebtCustom:
        RadioButton6->Checked = true;
        break;
    }
  }
  return ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::PaintBox1Paint(TObject *Sender)
{
  try
  {
    int X = PaintBox1->Width / 2;
    int Y = PaintBox1->Height / 2;
    int PointSize = ToInt(Edit2->Text);

    PaintBox1->Canvas->Pen->Width = 1;
    PaintBox1->Canvas->Pen->Color = clBlack;
    if(CheckBox3->Checked && PointSize)
    {
      PaintBox1->Canvas->MoveTo(X - 10, Y);
      PaintBox1->Canvas->LineTo(X + 10, Y);
      PaintBox1->Canvas->MoveTo(X - 10, Y - 4);
      PaintBox1->Canvas->LineTo(X - 10, Y + 5);
      PaintBox1->Canvas->MoveTo(X + 10, Y - 4);
      PaintBox1->Canvas->LineTo(X + 10, Y + 5);
    }

    if(CheckBox4->Checked && PointSize)
    {
      PaintBox1->Canvas->MoveTo(X, Y - 10);
      PaintBox1->Canvas->LineTo(X, Y + 10);
      PaintBox1->Canvas->MoveTo(X - 4, Y - 10);
      PaintBox1->Canvas->LineTo(X + 5, Y - 10);
      PaintBox1->Canvas->MoveTo(X - 4, Y + 10);
      PaintBox1->Canvas->LineTo(X + 5, Y + 10);
    }

    PaintBox1->Canvas->Pen->Handle = SetPen(ExtColorBox2->Selected, LineSelect1->LineStyle, ToInt(Edit3->Text));
    PaintBox1->Canvas->MoveTo(0, Y);
    PaintBox1->Canvas->LineTo(PaintBox1->Width, Y);

    TPointSelect::DrawPoint(PaintBox1->Canvas, TPoint(X, Y), PointSelect1->ItemIndex, clBlack, ExtColorBox1->Selected, PointSize);

    if(CheckBox2->Checked)
    {
      std::string Str = "(2.37,9.53)";
      PaintBox1->Canvas->Font->Assign(FontDialog1->Font);
      TDraw::DrawPointLabel(PaintBox1->Canvas, TPoint(X, Y), PointSize, Str, static_cast<TLabelPosition>(ComboBox1->ItemIndex));
    }
  }
  catch(...)
  {
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Change(TObject *Sender)
{
  PaintBox1->Invalidate();
  PointSelect1->FillColor = ExtColorBox1->Selected;
  ComboBox1->Enabled = CheckBox2->Checked;
}
//---------------------------------------------------------------------------
HPEN TForm14::SetPen(TColor Color, TPenStyle Style, int Width)
{
  if(Win32Platform == VER_PLATFORM_WIN32_NT && Width > 1)
  {
    unsigned long DashStyle[] = {16, 8};
    LOGBRUSH LogBrush;
    LogBrush.lbStyle = BS_SOLID;
    LogBrush.lbColor = Color;
    if(Style == PS_DASH)
      return ExtCreatePen(PS_GEOMETRIC | PS_USERSTYLE, Width, &LogBrush, sizeof(DashStyle)/sizeof(*DashStyle), DashStyle);
    return ExtCreatePen(PS_GEOMETRIC | Style, Width, &LogBrush, 0, NULL);
  }
  return CreatePen(Style, Width, Color);
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Button3Click(TObject *Sender)
{
  Application->HelpContext(HelpContext);
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Popup_InsertClick(TObject *Sender)
{
  Grid->InsertRows(Grid->Row, 1);
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Popup_RemoveClick(TObject *Sender)
{
  Grid->RemoveRows(Grid->Selection.Top, Grid->Selection.Bottom - Grid->Selection.Top + 1);
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Popup_ImportClick(TObject *Sender)
{
  OpenDialog1->Filter = LoadRes(RES_DATA_FILTER);
  if(OpenDialog1->Execute())
  {
    Grid->ImportFromFile(OpenDialog1->FileName);
    int Row = Grid->Selection.Top;
    if(Grid->Cells[0][Row] == "X" && Grid->Cells[1][Row] == "Y")
      Grid->RemoveRows(Row, 1);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm14::BitBtn1Click(TObject *Sender)
{
  if(FontDialog1->Execute())
    PaintBox1->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Popup_ExportClick(TObject *Sender)
{
  SaveDialog1->Filter = LoadRes(RES_EXPORT_DATA_FILTER);
  if(SaveDialog1->Execute())
    if(!Grid->ExportToFile(SaveDialog1->FileName, SaveDialog1->FilterIndex == 1 ? ';' : '\t'))
      MessageBox(LoadRes(RES_FILE_ACCESS, SaveDialog1->FileName), LoadRes(RES_WRITE_FAILED), MB_ICONSTOP);
}
//---------------------------------------------------------------------------
void __fastcall TForm14::EditKeyPress(TObject *Sender, char &Key)
{
  if(!isdigit(Key) && Key!='\b')
    Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm14::RadioButtonClick(TObject *Sender)
{
  UpdateErrorBars();
  PaintBox1->Invalidate();
}
//---------------------------------------------------------------------------
void TForm14::UpdateErrorBars()
{
  unsigned Cols = 2;
  Cols += CheckBox3->Checked && RadioButton3->Checked;
  Cols += CheckBox4->Checked && RadioButton6->Checked;
  Width = Width + (Cols - Grid->ColCount) * Grid->DefaultColWidth;
  Edit1->Width = Edit1->Width + (Cols - Grid->ColCount) * Grid->DefaultColWidth;
  Grid->ColCount = Cols;

  RadioButton1->Enabled = CheckBox3->Checked;
  RadioButton2->Enabled = CheckBox3->Checked;
  RadioButton3->Enabled = CheckBox3->Checked;
  Edit4->Enabled = CheckBox3->Checked;
  Edit5->Enabled = CheckBox3->Checked;

  RadioButton4->Enabled = CheckBox4->Checked;
  RadioButton5->Enabled = CheckBox4->Checked;
  RadioButton6->Enabled = CheckBox4->Checked;
  Edit6->Enabled = CheckBox4->Checked;
  Edit7->Enabled = CheckBox4->Checked;
}
//---------------------------------------------------------------------------
std::string& TForm14::GetText(int ACol, int ARow)
{
  DataPoints.resize(std::max(Grid->RowCount - 1, ARow));
  switch(ACol)
  {
    case 0: return DataPoints[ARow-1].x.Text;
    case 1: return DataPoints[ARow-1].y.Text;
    case 2: return CheckBox3->Enabled && RadioButton3->Checked ? DataPoints[ARow-1].xError.Text : DataPoints[ARow-1].yError.Text;
    case 3: return DataPoints[ARow-1].yError.Text;
  }
  throw Exception("Invalid Coloumn");
}
//---------------------------------------------------------------------------
void __fastcall TForm14::GridSetText(TObject *Sender, int ACol, int ARow,
      const AnsiString Value)
{
  if(ARow > 0)
    GetText(ACol, ARow) = Value.c_str();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::GridGetText(TObject *Sender, int ACol, int ARow,
      AnsiString &Value)
{
  if(ARow == 0)
    switch(ACol)
    {
      case 0: Value = "X"; break;
      case 1: Value = "Y"; break;
      case 2: Value = LoadRes(RES_UNCERTAINTY, (CheckBox3->Checked && RadioButton3->Checked) ? "X" : "Y"); break;
      case 3: Value = LoadRes(RES_UNCERTAINTY, "Y"); break;
    }
  else
    Value = GetText(ACol, ARow).c_str();
}
//---------------------------------------------------------------------------
void __fastcall TForm14::EditChange(TObject *Sender)
{
  if(Sender == Edit4)
    RadioButton1->Checked = true;
  else if(Sender == Edit5)
    RadioButton2->Checked = true;
  else if(Sender == Edit6)
    RadioButton4->Checked = true;
  else if(Sender == Edit7)
    RadioButton5->Checked = true;
}
//---------------------------------------------------------------------------

