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
#include <fstream>
#include <io.h>
#include "VersionInfo.h"
#include "ConfigFile.h"
#include "OleServer.h"
#include "Encode.h"
#include "ConfigRegistry.h"
#include "ICompCommon.h"
#include "HandleCsv.h"
//---------------------------------------------------------------------------
void TData::LoadFromFile(const std::wstring &FileName)
{
  TConfigFile IniFile;
  IniFile.LoadFromUtf8File(FileName);

  CheckIniInfo(IniFile);

  std::wstring SavedByVersion = IniFile.Section(L"Graph").Read(L"Version", L"NA");
  if(SavedByVersion == L"NA")
  {
    if(std::_waccess(FileName.c_str(), 0))
      throw EGraphError(LoadRes(RES_FILE_NOT_FOUND, FileName));
    throw EGraphError(LoadRes(RES_NOT_GRAPH_FILE, FileName));
  }

  try
  {
    GrfName = FileName;

    //Free mem in function lists
    Clear();

    if(SavedByVersion < TVersion(L"4.4.0.414"))
      IniFile.LoadFromAnsiFile(FileName);

    PreprocessGrfFile(IniFile);
    Axes.ReadFromIni(IniFile.Section(L"Axes"));

    //Adjust TickUnit and GridUnit if saved by an outdated version
    if(SavedByVersion < TVersion(L"3.2"))
    {
      if(Axes.xAxis.LogScl)
      {
        Axes.xAxis.TickUnit = std::pow(10, Axes.xAxis.TickUnit);
        Axes.xAxis.GridUnit = std::pow(10, Axes.xAxis.GridUnit);
      }
      if(Axes.yAxis.LogScl)
      {
        Axes.yAxis.TickUnit = std::pow(10, Axes.yAxis.TickUnit);
        Axes.yAxis.GridUnit = std::pow(10, Axes.yAxis.GridUnit);
      }
    }

    if(SavedByVersion < TVersion(L"4.0"))
    {
      std::swap(Axes.xAxis.AxisCross, Axes.yAxis.AxisCross);
    }

    CustomFunctions.ReadFromIni(IniFile.Section(L"CustomFunctions"));
    LoadData(IniFile);
    AnimationInfo.ReadFromIni(IniFile.Section(L"Animate"));
    LoadPluginData(IniFile.Section(L"PluginData"));
  }
  catch(Func32::EFuncError &Error)
  {
    throw;
  }
  catch(...)
  {
    throw EGraphError(LoadRes(RES_ERROR_READING_FILE, FileName));
  }

  Modified = false;
}
//---------------------------------------------------------------------------
void TData::Load(TConfigFile &IniFile)
{
  CheckIniInfo(IniFile);

  std::wstring SavedByVersion = IniFile.Section(L"Graph").Read(L"Version", L"NA");
  if(SavedByVersion == L"NA")
    throw EGraphError(LoadString(RES_INVALID_OBJECT));

  try
  {
    GrfName.clear();

    //Free mem in function lists
    Clear();

    PreprocessGrfFile(IniFile);
    Axes.ReadFromIni(IniFile.Section(L"Axes"));
    CustomFunctions.ReadFromIni(IniFile.Section(L"CustomFunctions"));
    LoadData(IniFile);
    AnimationInfo.ReadFromIni(IniFile.Section(L"Animate"));
    LoadPluginData(IniFile.Section(L"PluginData"));
  }
  catch(Func32::EFuncError &Error)
  {
    throw;
  }
  catch(...)
  {
    throw EGraphError(LoadString(RES_READING_OBJECT));
  }

  Modified = false;
}
//---------------------------------------------------------------------------
void TData::Import(const std::wstring &FileName)
{
  if(std::_waccess(FileName.c_str(), 0))
    throw EGraphError(LoadRes(RES_FILE_NOT_FOUND, FileName));
  TConfigFile ConfigFile;
  ConfigFile.LoadFromUtf8File(FileName);
  std::wstring SavedByVersion = ConfigFile.Section(L"Graph").Read(L"Version", L"NA");
  if(SavedByVersion == L"NA")
    throw EGraphError(LoadRes(RES_NOT_GRAPH_FILE, FileName));
  if(TVersion(SavedByVersion) <= L"2.4")
    throw EGraphError(LoadRes(RES_INVALID_VERSION, SavedByVersion, L"2.5"));

  try
  {
    if(SavedByVersion < TVersion(L"4.4.0.414"))
      ConfigFile.LoadFromAnsiFile(FileName);
    Import(ConfigFile);
  }
  catch(Func32::EFuncError &Error)
  {
    throw;
  }
  catch(...)
  {
    throw EGraphError(LoadRes(RES_ERROR_READING_FILE, FileName));
  }
}
//---------------------------------------------------------------------------
void TData::Import(TConfigFile &IniFile)
{
  CheckIniInfo(IniFile);

  //Save decimal separator
  char OldDecimalSeparator = DecimalSeparator;

  //Set decimal separator to '.' to make sure that file conversion are the
  //same over the whole world
  DecimalSeparator = '.';

  //ElemList must be empty when reading shades and tangents from file
  boost::shared_ptr<TTopGraphElem> Temp(new TTopGraphElem(this));
  Temp.swap(TopElem);
  try
  {
    PreprocessGrfFile(IniFile);
    CustomFunctions.ReadFromIni(IniFile.Section(L"CustomFunctions"));
    LoadData(IniFile);
    AnimationInfo.ReadFromIni(IniFile.Section(L"Animate"));
    LoadPluginData(IniFile.Section(L"PluginData"));
  }
  catch(...)
  {
    Temp.swap(TopElem); //Swap back on exception
    throw;
  }
  Temp.swap(TopElem);
  while(Temp->ChildCount() > 0)
    if(!dynamic_cast<TAxesView*>(Temp->GetChild(0).get())) //We only want 1 TAxesView
      TopElem->InsertChild(Temp->GetChild(0));
    else
      Temp->RemoveChild(0);

  //Set decimal separator back
  DecimalSeparator = OldDecimalSeparator;
  Modified = true;
}
//---------------------------------------------------------------------------
void TData::SaveImage(TConfigFile &IniFile, TCanvas *Canvas, int Width, int Height) const
{
  std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
  Bitmap->Width = Width;
  Bitmap->Height = Height;
  TRect Rect(0, 0, Bitmap->Width, Bitmap->Height);
  Bitmap->Canvas->CopyRect(Rect, Canvas, Rect);

  std::stringstream Stream;
  Bitmap->PixelFormat = pf8bit; //Change bitmap to 8 bit
//  if(SaveBitmapToPngStream(Bitmap->Handle, Stream))
//    IniFile.Section(L"Image").Write(L"Png", Base64Encode(Stream.str().data(), Stream.str().size()));
}
//---------------------------------------------------------------------------
//Saves data to file given in FileName; if empty the user is requested a file name
//If Remember is true this will be the new current file name
bool TData::Save(const std::wstring &FileName, bool Remember) const
{
  try
  {
    TConfigFile IniFile;
    std::wstring Comment = L"This file was created by Graph (http://www.padowan.dk)\nDo not change this file from other programs.";
    IniFile.SetComment(Comment);

    WriteInfoToIni(IniFile);
    SaveData(IniFile);
    CustomFunctions.WriteToIni(IniFile.Section(L"CustomFunctions"));
    AnimationInfo.WriteToIni(IniFile.Section(L"Animate"));
    TConfigFileSection &Section = IniFile.Section(L"PluginData");
    for(std::map<std::wstring,std::wstring>::const_iterator Iter = PluginData.begin(); Iter != PluginData.end(); ++Iter)
      Section.Write(Iter->first, Iter->second);
//    SaveImage(IniFile);

    if(!IniFile.SaveToUtf8File(FileName))
      return false;

    if(Remember)
    {
      GrfName = FileName;
      Application->MainForm->Caption = String(NAME) + " - " + GrfName.c_str();
      Application->Title = String(NAME) + " - " + ExtractFileName(GrfName.c_str());
      Modified = false;
    }

    SendOleAdvise(acDataSaved); //Notify OLE client that data has been saved
    return true;
  }
  catch(EFCreateError &E)
  {
    MessageBox(LoadRes(RES_FILE_ACCESS, FileName), LoadString(RES_WRITE_FAILED), MB_ICONSTOP);
    return false;
  }
}
//---------------------------------------------------------------------------
//Returns the saved data as a string
std::wstring TData::SaveToString(bool ResetModified) const
{
  TConfigFile IniFile;
  WriteInfoToIni(IniFile);
  Axes.WriteToIni(IniFile.Section(L"Axes"));
  SaveData(IniFile);
  CustomFunctions.WriteToIni(IniFile.Section(L"CustomFunctions"));
  AnimationInfo.WriteToIni(IniFile.Section(L"Animate"));

  if(ResetModified)
    Modified = false;
  return IniFile.GetAsString();
}
//---------------------------------------------------------------------------
void TData::SaveDefault() const
{
  TConfigRegistry Registry;
  if(Registry.CreateKey(REGISTRY_KEY))
  {
    TConfigFile IniFile;
    WriteInfoToIni(IniFile);
    Axes.WriteToIni(IniFile.Section(L"Axes"));
    Registry.Write(L"DefaultAxes", IniFile.GetAsString().c_str());
  }
}
//---------------------------------------------------------------------------
bool TData::ImportPointSeries(const std::wstring &FileName)
{
  const TColor Colors[] = {clRed, clGreen, clBlue, clYellow, clPurple, clAqua, clBlack, clGray, clSkyBlue	, clMoneyGreen, clDkGray};

  std::ifstream Stream(FileName.c_str());
  if(!Stream)
  {
    MessageBox(LoadRes(RES_NOT_GRAPH_FILE, FileName), LoadString(RES_FILE_ERROR), MB_ICONSTOP);
    return false;
  }

  TCsvGrid CsvGrid;
  ImportCsv(Stream, CsvGrid);
  std::vector<std::vector<TPointSeriesPoint> > Points;
  unsigned Col;
  unsigned ColCount = CsvGrid[0].size();
  try
  {
    for(Col = 1; Col < ColCount; Col++)
    {
      Points.push_back(std::vector<TPointSeriesPoint>());
      for(unsigned Row = 0; Row < CsvGrid.size(); Row++)
      {
        if(Property.DecimalSeparator != '.')
        {
          std::replace(CsvGrid[Row][0].begin(), CsvGrid[Row][0].end(), (char)Property.DecimalSeparator, '.');
          std::replace(CsvGrid[Row][Col].begin(), CsvGrid[Row][Col].end(), (char)Property.DecimalSeparator, '.');
        }
        Points.back().push_back(TPointSeriesPoint(ToWString(CsvGrid[Row][0]), ToWString(CsvGrid[Row][Col])));
      }
    }
  }
  catch(Func32::EParseError &E)
  {
    //Ignore errors in first line; This could be a text
    if(Col != 1)
    {
      MessageBox(LoadRes(526, FileName.c_str(), Col), LoadRes(RES_FILE_ERROR), MB_ICONSTOP);
      return false;
    }
  }

  unsigned ColorIndex = 0;
  unsigned Style = 6;
  unsigned LineStyle = psDashDotDot;

  UndoList.BeginMultiUndo();
  for(unsigned I = 0; I < Points.size(); I++)
  {
    boost::shared_ptr<TPointSeries> Series(new TPointSeries(
      clBlack,            //FrameColor
      Colors[ColorIndex], //FillColor
      Colors[ColorIndex], //LineColor
      Property.DefaultPoint.Size, //Size
      Property.DefaultPointLine.Size, //LineSize
      Style = ++Style % 7, //Style
      static_cast<TPenStyle>(++LineStyle % 5), //LineStyle
      iaLinear, //Onterpolation
      false, //ShowLabels
      Property.DefaultPointLabelFont, //Font
      lpBelow, //LabelPosition
      ptCartesian, //PointType
      ebtNone,  //xErrorBarType
      0, //xErrorValues
      ebtNone, //yErrorBarType
      0 //yErrorValue
    ));
    Series->Assign(Points[I]);
    Series->SetLegendText(CreatePointSeriesDescription());
    Insert(Series);
    UndoList.Push(TUndoAdd(Series));
    Series->Update();
    ColorIndex = ++ColorIndex % (sizeof(Colors)/sizeof(TColor));
  }

  UndoList.EndMultiUndo();
  Modified = true;
  return true;
}
//---------------------------------------------------------------------------
void TData::LoadDefault()
{
  GrfName.clear();
  TConfigFile IniFile;

  //Free mem in function lists
  Clear();

  Insert(boost::shared_ptr<TAxesView>(new TAxesView));

  Axes.ReadFromIni(IniFile.Section(L"Axes"));
  Modified = false;
}
//---------------------------------------------------------------------------
void TData::PreprocessGrfFile(TConfigFile &IniFile)
{
  std::wstring SavedByVersion = IniFile.Section(L"Graph").Read(L"Version", L"NA");
  if(SavedByVersion < TVersion(L"4.2"))
  {
    std::wstring Str = IniFile.GetAsString();
    unsigned Size = Str.size();
    for(unsigned I = 0; I < Str.size(); I++)
      if(Str[I] == '.' && !isdigit(Str[I+1]))
        Str.erase(I, 1);
    if(Size != Str.size())
      IniFile.LoadFromString(Str);
  }
}
//---------------------------------------------------------------------------


