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
#include <io>
#include "VersionInfo.h"
#include "ConfigFile.h"
#include "OleServer.h"
#include "PngHelper.h"
#include "Encode.h"
//---------------------------------------------------------------------------
bool TData::LoadFromFile(const std::string &FileName, bool ShowErrorMessages)
{
  TConfigFile IniFile(FileName);

  if(!CheckIniInfo(IniFile, ShowErrorMessages))
    return false;

  std::string SavedByVersion = IniFile.Read("Graph", "Version", "NA");
  if(SavedByVersion == "NA")
  {
    if(ShowErrorMessages)
    {
      if(std::access(FileName.c_str(), 0))
        MessageBox(LoadRes(RES_FILE_NOT_FOUND, FileName), LoadRes(RES_FILE_DOESNT_EXIST), MB_ICONSTOP);
      else
        MessageBox(LoadRes(RES_NOT_GRAPH_FILE, FileName), LoadRes(RES_FILE_READ_ERROR), MB_ICONSTOP);
    }
    return false;
  }

  try
  {
    GrfName = FileName;

    //Free mem in function lists
    Clear();

    PreprocessGrfFile(IniFile);
    Axes.ReadFromIni(IniFile);

    //Adjust TickUnit and GridUnit if saved by an outdated version
    if(SavedByVersion < TVersion("3.2"))
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

    if(SavedByVersion < TVersion("4.0"))
    {
      std::swap(Axes.xAxis.AxisCross, Axes.yAxis.AxisCross);
    }

    CustomFunctions.ReadFromIni(IniFile);
    LoadData(IniFile);
    AnimationInfo.ReadFromIni(IniFile);
  }
  catch(Func32::EFuncError &Error)
  {
    if(ShowErrorMessages)
      ShowErrorMsg(Error);
    return false;
  }
  catch(...)
  {
    if(ShowErrorMessages)
      MessageBox(LoadRes(RES_ERROR_READING_FILE, FileName), "File read error", MB_ICONSTOP);
    return false;
  }

  Modified = false;
  return true;
}
//---------------------------------------------------------------------------
bool TData::LoadFromString(const std::string &Str)
{
  TConfigFile IniFile;
  IniFile.LoadFromString(Str);

  if(!CheckIniInfo(IniFile))
    return false;

  std::string SavedByVersion = IniFile.Read("Graph", "Version", "NA");
  if(SavedByVersion == "NA")
  {
    MessageBox(LoadRes(RES_INVALID_OBJECT), LoadRes(RES_OBJECT_ERROR), MB_ICONSTOP);
    return false;
  }

  try
  {
    GrfName.clear();

    //Free mem in function lists
    Clear();

    PreprocessGrfFile(IniFile);
    Axes.ReadFromIni(IniFile);
    CustomFunctions.ReadFromIni(IniFile);
    LoadData(IniFile);
    AnimationInfo.ReadFromIni(IniFile);
  }
  catch(Func32::EFuncError &Error)
  {
    ShowErrorMsg(Error);
  }
  catch(...)
  {
    MessageBox(LoadRes(RES_READING_OBJECT), LoadRes(RES_OBJECT_ERROR), MB_ICONSTOP);
  }

  Modified = false;
  return true;
}
//---------------------------------------------------------------------------
bool TData::Import(const std::string &FileName)
{
  TConfigFile IniFile(FileName);

  if(!CheckIniInfo(IniFile))
    return false;

  std::string SavedByVersion = IniFile.Read("Graph", "Version", "NA");
  if(SavedByVersion == "NA")
  {
    if(std::access(FileName.c_str(), 0))
      MessageBox(LoadRes(RES_FILE_NOT_FOUND, FileName), LoadRes(RES_FILE_DOESNT_EXIST), MB_ICONSTOP);
    else
      MessageBox(LoadRes(RES_NOT_GRAPH_FILE, FileName), LoadRes(RES_FILE_READ_ERROR), MB_ICONSTOP);
    return false;
  }

  //Save decimal separator
  char OldDecimalSeparator = DecimalSeparator;

  try
  {
    //Set decimal separator to '.' to make sure that file conversion are the
    //same over the whole world
    DecimalSeparator = '.';

    if(SavedByVersion != "NA" && TVersion(SavedByVersion) <= "2.4")
    {
      MessageBox(LoadRes(RES_INVALID_VERSION, SavedByVersion, "2.5"), "Invalid version");
      return false;
    }

    //ElemList must be empty when reading shades and tangents from file
    std::vector<boost::shared_ptr<TGraphElem> > Temp;
    Temp.swap(ElemList);
    PreprocessGrfFile(IniFile);
    CustomFunctions.ReadFromIni(IniFile);
    LoadData(IniFile);
    AnimationInfo.ReadFromIni(IniFile);
    for(unsigned I = 0; I < ElemList.size(); I++)
      if(!dynamic_cast<TAxesView*>(ElemList[I].get())) //We only want 1 TAxesView
        Temp.push_back(ElemList[I]);
    Temp.swap(ElemList);
  }
  catch(Func32::EFuncError &Error)
  {
    ShowErrorMsg(Error);
  }
  catch(...)
  {
    MessageBox(LoadRes(RES_ERROR_READING_FILE, FileName), "File read error", MB_ICONSTOP);
  }
  //Set decimal separator back
  DecimalSeparator = OldDecimalSeparator;
  Modified = true;
  return true;
}
//---------------------------------------------------------------------------
void TData::SaveImage(TConfigFile &IniFile)
{
  std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
  Bitmap->Width = Form1->Image1->Width;
  Bitmap->Height = Form1->Image1->Height;
  TRect Rect(0, 0, Bitmap->Width, Bitmap->Height);
  Bitmap->Canvas->CopyRect(Rect, Form1->Image1->Picture->Bitmap->Canvas, Rect);

  std::stringstream Stream;
  Bitmap->PixelFormat = pf8bit; //Change bitmap to 8 bit
  if(SaveBitmapToPngStream(Bitmap->Handle, Stream))
    IniFile.Write("Image", "Png", Base64Encode(Stream.str().data(), Stream.str().size()));
}
//---------------------------------------------------------------------------
//Saves data to file given in FileName; if empty the user is requested a file name
//If Remember is true this will be the new current file name
bool TData::Save(const std::string &FileName, bool Remember)
{
  try
  {
    TConfigFile IniFile;
    std::string Comment = "This file was created by Graph (http://www.padowan.dk)\nDo not change this file from other programs.";
    IniFile.SetComment(Comment);

    WriteInfoToIni(IniFile);
    SaveData(IniFile);
    CustomFunctions.WriteToIni(IniFile);
    AnimationInfo.WriteToIni(IniFile);
//    SaveImage(IniFile);

    if(!IniFile.SaveToFile(FileName))
      return false;

    if(Remember)
    {
      GrfName = FileName;
      Form1->Caption = AnsiString(NAME) + " - " + GrfName.c_str();
      Application->Title = AnsiString(NAME) + " - " + ExtractFileName(GrfName.c_str());
      Modified = false;
    }

    SendOleAdvise(acDataSaved); //Notify OLE client that data has been saved
    return true;
  }
  catch(EFCreateError &E)
  {
    MessageBox(LoadRes(RES_FILE_ACCESS, FileName), LoadRes(RES_WRITE_FAILED), MB_ICONSTOP);
    return false;
  }
}
//---------------------------------------------------------------------------
//Returns the saved data as a string
std::string TData::SaveToString(bool ResetModified)
{
  TConfigFile IniFile;
  WriteInfoToIni(IniFile);
  Axes.WriteToIni(IniFile);
  SaveData(IniFile);
  CustomFunctions.WriteToIni(IniFile);
  AnimationInfo.WriteToIni(IniFile);

  if(ResetModified)
    Modified = false;
  return IniFile.GetAsString();
}
//---------------------------------------------------------------------------
void TData::SaveDefault()
{
  std::auto_ptr<TRegistry> Registry(new TRegistry);
  if(Registry->OpenKey(REGISTRY_KEY, true))
  {
    TConfigFile IniFile;
    WriteInfoToIni(IniFile);
    Axes.WriteToIni(IniFile);
    Registry->WriteString("DefaultAxes", IniFile.GetAsString().c_str());
  }
}
//---------------------------------------------------------------------------
//Returns separator used for line ('\t', ' ', ';', ',')
char GetSeparator(const std::string &Str)
{
  const char *Separators = "\t; ,";
  for(const char* Ch = Separators; *Ch; Ch++)
    if(Str.find(*Ch) != std::string::npos)
      return *Ch;
  return 0;
}
//---------------------------------------------------------------------------
bool TData::ImportData(const std::string &FileName)
{
  const TColor Colors[] = {clRed, clGreen, clBlue, clYellow, clPurple, clAqua, clBlack, clGray, clSkyBlue	, clMoneyGreen, clDkGray};

  std::ifstream Stream(FileName.c_str());
  if(!Stream)
  {
    MessageBox(LoadRes(RES_NOT_GRAPH_FILE, FileName), LoadRes(RES_FILE_ERROR), MB_ICONSTOP);
    return false;
  }

  std::vector<std::vector<TPointSeriesPoint> > Points;
  std::string Str;
  unsigned Line = 1;

  while(std::getline(Stream, Str))
  {
    //Ignore empty lines
    if(Str.empty())
      continue;

    char Separator = GetSeparator(Str);
    if(Separator != ',')
      std::replace(Str.begin(), Str.end(), ',', '.');

    //Several separators after each other (eg. spaces) are ignored
    unsigned FirstPos = Str.find_first_not_of(Separator);
    unsigned Pos = Str.find(Separator, FirstPos);
    std::string xText = Str.substr(FirstPos, Pos - FirstPos);
    unsigned Col = 0;

    for(unsigned LastPos = Str.find_first_not_of(Separator, Pos);
        Pos != std::string::npos; LastPos = Pos + 1,
        Col++)
    {
      try
      {  
        Pos = Str.find(Separator, LastPos);

        //Ignore empty entries
        if(Pos == LastPos || LastPos == Str.size())
          continue;

        if(Line == 1)
          Points.push_back(std::vector<TPointSeriesPoint>());
        std::string yText = Str.substr(LastPos, Pos - LastPos);

        //Check if there are too many numbers on the line
        if(Col < Points.size())
          Points[Col].push_back(TPointSeriesPoint(*this, xText, yText));
        else
        {
          MessageBox(LoadRes(526, FileName.c_str(), Line), LoadRes(RES_FILE_ERROR), MB_ICONSTOP);
          return false;
        }
      }
      catch(Func32::EParseError &E)
      {
        //Ignore errors in first line; This could be a text
        if(Line != 1)
        { 
          MessageBox(LoadRes(526, FileName.c_str(), Line), LoadRes(RES_FILE_ERROR), MB_ICONSTOP);
          return false;
        }
      }
    }

    //Check if there are too few numbers in the line
    if(Col < Points.size())
    {
      MessageBox(LoadRes(526, FileName.c_str(), Line), LoadRes(RES_FILE_ERROR), MB_ICONSTOP);
      return false;
    }
    Line++;
  }

  unsigned ColorIndex = 0;
  unsigned Style = 6;
  unsigned LineStyle = psDashDotDot;

  UndoList.BeginMultiUndo();
  for(unsigned I = 0; I < Points.size(); I++)
  {
    boost::shared_ptr<TPointSeries> Series(new TPointSeries);
    Series->FrameColor = clBlack;
    Series->FillColor = Colors[ColorIndex];
    Series->LineColor = Colors[ColorIndex];
    ColorIndex = ++ColorIndex % (sizeof(Colors)/sizeof(TColor));
    Series->Size = Property.DefaultPoint.Size;
    Series->Style = (Style = ++Style % 7);
    Series->LineSize = Property.DefaultPointLine.Size;
    Series->LineStyle = static_cast<TPenStyle>(++LineStyle % 5);
    Series->Interpolation = iaLinear;
    Series->ShowLabels = false;
    Series->LabelPosition = lpBelow;
    Series->Font->Assign(Property.DefaultPointLabelFont);
    Series->PointList.swap(Points[I]);
    Series->SetLegendText(CreatePointSeriesDescription());
    ElemList.push_back(Series);
    UndoList.Push(TUndoAdd(Series));
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

  Add(boost::shared_ptr<TAxesView>(new TAxesView));

  Axes.ReadFromIni(IniFile);
  Modified = false;
}
//---------------------------------------------------------------------------
void TData::PreprocessGrfFile(TConfigFile &IniFile)
{
  std::string SavedByVersion = IniFile.Read("Graph", "Version", "NA");
  if(SavedByVersion < TVersion("4.2"))
  {
    std::string Str = IniFile.GetAsString();
    unsigned Size = Str.size();
    for(unsigned I = 0; I < Str.size(); I++)
      if(Str[I] == '.' && !isdigit(Str[I+1]))
        Str.erase(I, 1); 
    if(Size != Str.size())
      IniFile.LoadFromString(Str);
  }
}
//---------------------------------------------------------------------------


