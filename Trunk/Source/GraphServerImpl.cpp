/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
// GRAPHSERVERIMPL : Implementation of TGraphServerImpl (CoClass: GraphServer, Interface: IGraphServer)
#include "Graph.h"
#pragma hdrstop
#include "GraphServerImpl.H"

TGraphServerImpl *GraphServerImpl = NULL;

/////////////////////////////////////////////////////////////////////////////
// TGraphServerImpl
TGraphServerImpl::TGraphServerImpl()
{
  //Data was not initialized when started as COM server

  Data.LoadDefault();
  Form1->Redraw();
  GraphServerImpl = this;
}
//---------------------------------------------------------------------------
TGraphServerImpl::~TGraphServerImpl()
{
  GraphServerImpl = NULL;
}
//---------------------------------------------------------------------------
STDMETHODIMP TGraphServerImpl::Show()
{
  OutputDebugString("Show()");
  Form1->Visible = true;
  return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP TGraphServerImpl::Hide()
{
  OutputDebugString("Hide()");
  Form1->Visible = false;
  return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP TGraphServerImpl::Load(BSTR FileName)
{
  Form1->LoadFromFile(FileName);
  Form1->Redraw();
  return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP TGraphServerImpl::ImportPointSeries(BSTR FileName)
{
  HRESULT Result = Data.ImportData(AnsiString(FileName).c_str()) ? S_OK : E_FAIL;
  if(Result == S_OK)
  {
    UndoList.Clear();
    Form1->UpdateTreeView();
    Form1->UpdateMenu();
    Form1->Redraw();
  }
  return Result;
}
//---------------------------------------------------------------------------
STDMETHODIMP TGraphServerImpl::Save(BSTR FileName, VARIANT_BOOL Remember)
{
  return Data.Save(AnsiString(FileName).c_str(), Remember) ? S_OK : E_FAIL;
}
//---------------------------------------------------------------------------
STDMETHODIMP TGraphServerImpl::SaveAsImage(BSTR FileName,
  TImageFileType ImageFileType)
{
  return Form1->SaveAsImage(FileName, ImageFileType) == seNoError ? S_OK : E_FAIL;
}
//---------------------------------------------------------------------------
STDMETHODIMP TGraphServerImpl::ZoomAllPoints()
{
  OutputDebugString("ZoomAllPoints()");
  Form1->ZoomAllPointsAction->Execute();
  return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP TGraphServerImpl::Refresh()
{
  OutputDebugString("Refresh()");
  Form1->Redraw();
  Form1->UpdateMenu();
  Form1->UpdateTreeView();
  return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP TGraphServerImpl::get_Axes(TAxesRecord* Value)
{
  Value->xAxis.Min = Data.Axes.xAxis.Min;
  Value->xAxis.Max = Data.Axes.xAxis.Max;
  Value->xAxis.TickUnit = Data.Axes.xAxis.TickUnit;
  Value->xAxis.GridUnit = Data.Axes.xAxis.GridUnit;
  Value->xAxis.LogScale = Data.Axes.xAxis.LogScl;
  Value->xAxis.ShowNumbers = Data.Axes.xAxis.ShowNumbers;
  Value->xAxis.AutoTick = Data.Axes.xAxis.AutoTick;
  Value->xAxis.AutoGrid = Data.Axes.xAxis.AutoGrid;
  Value->xAxis.ShowTicks = Data.Axes.xAxis.ShowTicks;
  Value->xAxis.ShowGrid = Data.Axes.xAxis.ShowGrid;
  Value->xAxis.Label = WideString(Data.Axes.xAxis.Label.c_str()).Copy();
  Value->xAxis.ShowLabel = Data.Axes.xAxis.ShowLabel;
  Value->xAxis.Cross = Data.Axes.xAxis.AxisCross;

  Value->yAxis.Min = Data.Axes.yAxis.Min;
  Value->yAxis.Max = Data.Axes.yAxis.Max;
  Value->yAxis.TickUnit = Data.Axes.yAxis.TickUnit;
  Value->yAxis.GridUnit = Data.Axes.yAxis.GridUnit;
  Value->yAxis.LogScale = Data.Axes.yAxis.LogScl;
  Value->yAxis.ShowNumbers = Data.Axes.yAxis.ShowNumbers;
  Value->yAxis.AutoTick = Data.Axes.yAxis.AutoTick;
  Value->yAxis.AutoGrid = Data.Axes.yAxis.AutoGrid;
  Value->yAxis.ShowTicks = Data.Axes.yAxis.ShowTicks;
  Value->yAxis.ShowGrid = Data.Axes.yAxis.ShowGrid;
  Value->yAxis.Label = WideString(Data.Axes.yAxis.Label.c_str()).Copy();
  Value->yAxis.ShowLabel = Data.Axes.yAxis.ShowLabel;
  Value->yAxis.Cross = Data.Axes.yAxis.AxisCross;

  Value->Title =  WideString(Data.Axes.Title.c_str()).Copy();
  Value->AxesStyle = Data.Axes.AxesStyle;
  Value->Trigonometry = Data.Axes.Trigonometry;
  Value->BackgroundColor = Data.Axes.BackgroundColor;
  Value->AxesColor = Data.Axes.AxesColor;
  Value->GridColor = Data.Axes.GridColor;

  return S_OK;
};
//---------------------------------------------------------------------------
STDMETHODIMP TGraphServerImpl::set_Axes(TAxesRecord Value)
{
  try
  {
    Draw->AbortUpdate();
    Data.Axes.xAxis.Min = Value.xAxis.Min;
    Data.Axes.xAxis.Max = Value.xAxis.Max;
    Data.Axes.xAxis.TickUnit = Value.xAxis.TickUnit;
    Data.Axes.xAxis.GridUnit = Value.xAxis.GridUnit;
    Data.Axes.xAxis.LogScl = Value.xAxis.LogScale;
    Data.Axes.xAxis.ShowNumbers = Value.xAxis.ShowNumbers;
    Data.Axes.xAxis.AutoTick = Value.xAxis.AutoTick;
    Data.Axes.xAxis.AutoGrid = Value.xAxis.AutoGrid;
    Data.Axes.xAxis.ShowTicks = Value.xAxis.ShowTicks;
    Data.Axes.xAxis.ShowGrid = Value.xAxis.ShowGrid;
    Data.Axes.xAxis.Label = AnsiString(Value.xAxis.Label).c_str();
    Data.Axes.xAxis.ShowLabel = Value.xAxis.ShowLabel;
    Data.Axes.xAxis.AxisCross = Value.xAxis.Cross;

    Data.Axes.yAxis.Min = Value.yAxis.Min;
    Data.Axes.yAxis.Max = Value.yAxis.Max;
    Data.Axes.yAxis.TickUnit = Value.yAxis.TickUnit;
    Data.Axes.yAxis.GridUnit = Value.yAxis.GridUnit;
    Data.Axes.yAxis.LogScl = Value.yAxis.LogScale;
    Data.Axes.yAxis.ShowNumbers = Value.yAxis.ShowNumbers;
    Data.Axes.yAxis.AutoTick = Value.yAxis.AutoTick;
    Data.Axes.yAxis.AutoGrid = Value.yAxis.AutoGrid;
    Data.Axes.yAxis.ShowTicks = Value.yAxis.ShowTicks;
    Data.Axes.yAxis.ShowGrid = Value.yAxis.ShowGrid;
    Data.Axes.yAxis.Label = AnsiString(Value.yAxis.Label).c_str();
    Data.Axes.yAxis.ShowLabel = Value.yAxis.ShowLabel;
    Data.Axes.yAxis.AxisCross = Value.yAxis.Cross;

    Data.Axes.Title = AnsiString(Value.Title).c_str();
    Data.Axes.AxesStyle = static_cast<TAxesStyle>(Value.AxesStyle);
    Data.Axes.Trigonometry = static_cast<Func32::TTrigonometry>(Value.Trigonometry);
    Data.Axes.BackgroundColor = static_cast<TColor>(Value.BackgroundColor);
    Data.Axes.AxesColor = static_cast<TColor>(Value.AxesColor);
    Data.Axes.GridColor = static_cast<TColor>(Value.GridColor);

    Refresh();
    return S_OK;
  }
  catch(...)
  {
    return E_FAIL;
  }
};
//---------------------------------------------------------------------------
STDMETHODIMP TGraphServerImpl::AddFunc(TFuncRecord Func)
{
  try
  {
    boost::shared_ptr<TStdFunc> F(new TStdFunc(AnsiString(Func.FuncText).c_str()));
    F->Color = static_cast<TColor>(Func.Color);
    F->Size = Func.Size;
    F->Style = static_cast<TPenStyle>(Func.LineStyle);
    F->From = Func.From;
    F->To = Func.To;
    F->Steps = Func.Steps;
    F->LegendText = AnsiString(Func.LegendText).c_str();
    F->StartPointStyle = Func.StartMarker;
    F->EndPointStyle = Func.EndMarker;
    Data.Add(F);
    Refresh();
    return S_OK;
  }
  catch(...)
  {
    return E_FAIL;
  }
}
//---------------------------------------------------------------------------





