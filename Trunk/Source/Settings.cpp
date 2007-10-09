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
#include "Settings.h"
#include "ConfigFile.h"
#include "ConfigRegistry.h"
//---------------------------------------------------------------------------
///////////
// TAxes //
///////////
TAxis::TAxis() : Min(-10), Max(10), LogScl(false), ShowGrid(false), MultiplyOfPi(false),
  ShowLabel(true), ShowNumbers(true), ShowTicks(true), AutoTick(true), AutoGrid(true),
  AxisCross(0), TickUnit(1), GridUnit(1)
{
}
//---------------------------------------------------------------------------
TAxes::TAxes() : ShowLegend(true), AxesColor(clBlue),
  GridColor(static_cast<TColor>(0x00FF9999)), BackgroundColor(clWhite), Trigonometry(Func32::Radian),
  AxesStyle(asCrossed), LegendPlacement(lpTopRight), GridSize(1), CalcComplex(false), ZoomSquare(false),
  LegendPos(0,0), AxesArrows(aaPositiveEnd), NumberPlacement(npCenter)  
{
}
//---------------------------------------------------------------------------
void TAxis::WriteToIni(TConfigFile &IniFile, const std::string &Prefix) const
{
  IniFile.Write("Axes", Prefix + "Min", Min);
  IniFile.Write("Axes", Prefix + "Max", Max);
  IniFile.Write("Axes", Prefix + "TickUnit", TickUnit);
  IniFile.Write("Axes", Prefix + "GridUnit", GridUnit);
  IniFile.Write("Axes", Prefix + "LogScl", LogScl, false);
  IniFile.Write("Axes", Prefix + "ShowTicks", ShowTicks, true);
  IniFile.Write("Axes", Prefix + "ShowGrid", ShowGrid, false);
  IniFile.Write("Axes", Prefix + "ShowLabel", ShowLabel, true);
  IniFile.Write("Axes", Prefix + "AutoTick", AutoTick, true);
  IniFile.Write("Axes", Prefix + "AutoGrid", AutoGrid, true);
  IniFile.Write("Axes", Prefix + "Label", ToString(Label), Prefix);
  IniFile.Write("Axes", Prefix + "ShowNumbers", ShowNumbers, true);
  IniFile.Write("Axes", Prefix + "AxisCross", AxisCross, 0.0);
  IniFile.Write("Axes", Prefix + "MultiplyOfPi", MultiplyOfPi, false);
}
//---------------------------------------------------------------------------
void TAxes::WriteToIni(TConfigFile &IniFile) const
{
  xAxis.WriteToIni(IniFile, "x");
  yAxis.WriteToIni(IniFile, "y");

  IniFile.Write("Axes", "AxesColor", AxesColor);
  IniFile.Write("Axes", "GridColor", GridColor);
  IniFile.Write("Axes", "BackgroundColor", BackgroundColor, clWhite);
  IniFile.Write("Axes", "NumberFont", FontToStr(NumberFont), std::string(DEFAULT_NUMBER_FONT));
  IniFile.Write("Axes", "LabelFont", FontToStr(LabelFont), std::string(DEFAULT_LABEL_FONT));
  IniFile.Write("Axes", "LegendFont", FontToStr(LegendFont), std::string(DEFAULT_LEGEND_FONT));
  IniFile.Write("Axes", "ShowLegend", ShowLegend);
  IniFile.Write("Axes", "Radian", Trigonometry == Func32::Radian);
  IniFile.Write("Axes", "Title", ToString(Title), std::string());
  IniFile.Write("Axes", "TitleFont", FontToStr(TitleFont), std::string(DEFAULT_TITLE_FONT));
  IniFile.Write("Axes", "AxesStyle", AxesStyle, asCrossed);
  IniFile.Write("Axes", "LegendPlacement", LegendPlacement, lpTopRight);
  if(LegendPlacement == lpCustom)
    IniFile.Write("Axes", "LegendPos", LegendPos);
  IniFile.Write("Axes", "GridSize", GridSize, 1U);
  IniFile.Write("Axes", "CalcComplex", CalcComplex, false);
  IniFile.Write("Axes", "ZoomSquare", ZoomSquare, false);
  IniFile.Write("Axes", "AxesArrows", AxesArrows, aaPositiveEnd);
  IniFile.Write("Axes", "NumberPlacement", NumberPlacement, npCenter);
}
//---------------------------------------------------------------------------
void TAxis::ReadFromIni(const TConfigFile &IniFile, const std::string &Prefix)
{
  Min = IniFile.Read("Axes", Prefix + "Min", -10.0);
  Max = IniFile.Read("Axes", Prefix + "Max", 10.0);

  LogScl = IniFile.Read("Axes", Prefix + "LogScl", false);
  TickUnit = IniFile.Read("Axes", Prefix + "TickUnit", 1.0);
  GridUnit = IniFile.Read("Axes", Prefix + "GridUnit", 1.0);
  ShowTicks = IniFile.Read("Axes", Prefix + "ShowTicks", true);
  ShowGrid = IniFile.Read("Axes", Prefix + "ShowGrid", false);
  ShowLabel = IniFile.Read("Axes", Prefix + "ShowLabel", true);
  AutoTick = IniFile.Read("Axes", Prefix + "AutoTick", true);
  AutoGrid = IniFile.Read("Axes", Prefix + "AutoGrid", true);
  ShowNumbers = IniFile.Read("Axes", Prefix + "ShowNumbers", true);
  Label = ToWString(IniFile.Read("Axes", Prefix + "Label", Prefix));
  AxisCross = IniFile.Read("Axes", Prefix + "AxisCross", 0.0);
  MultiplyOfPi = IniFile.Read("Axes", Prefix + "MultiplyOfPi", false);
}
//---------------------------------------------------------------------------
void TAxes::ReadFromIni(const TConfigFile &IniFile)
{
  xAxis.ReadFromIni(IniFile, "x");
  yAxis.ReadFromIni(IniFile, "y");

  AxesColor = IniFile.Read("Axes", "AxesColor", clBlue);
  GridColor = IniFile.Read("Axes", "GridColor", static_cast<TColor>(0x00FF9999));
  BackgroundColor = IniFile.Read("Axes", "BackgroundColor", clWhite);
  StrToFont(IniFile.Read("Axes", "NumberFont", DEFAULT_NUMBER_FONT), NumberFont);
  StrToFont(IniFile.Read("Axes", "LabelFont", DEFAULT_LABEL_FONT), LabelFont);
  StrToFont(IniFile.Read("Axes", "LegendFont", DEFAULT_LEGEND_FONT), LegendFont);
  StrToFont(IniFile.Read("Axes", "TitleFont", DEFAULT_TITLE_FONT),TitleFont);
  ShowLegend = IniFile.Read("Axes", "ShowLegend", true);
  Title = ToWString(IniFile.Read("Axes", "Title", std::string()));
  Trigonometry = IniFile.Read("Axes", "Radian", true) ? Func32::Radian : Func32::Degree;
  AxesStyle = IniFile.ReadEnum("Axes", "AxesStyle", asCrossed);
  LegendPlacement = IniFile.ReadEnum("Axes", "LegendPlacement", lpTopRight);
  LegendPos = IniFile.Read("Axes", "LegendPos", Func32::TDblPoint(0, 0));
  GridSize = IniFile.Read("Axes", "GridSize", 1);
  CalcComplex = IniFile.Read("Axes", "CalcComplex", false);
  ZoomSquare = IniFile.Read("Axes", "ZoomSquare", false);
  AxesArrows = IniFile.ReadEnum("Axes", "AxesArrows", aaPositiveEnd);
  NumberPlacement = IniFile.ReadEnum("Axes", "NumberPlacement", npCenter);
}
//---------------------------------------------------------------------------
///////////////
// TProperty //
///////////////
TProperty::TProperty() : RoundTo(4), SavePos(true), ComplexFormat(cfRectangular),
  CheckForUpdate(true), ShowTipsAtStartup(true), NextTip(RES_FIRST_TIP), Language("English"),
  FontScale(100)
{
  //Initialize with default settings
  DefaultFunction.FromString("", psSolid, clRed, 1);
  DefaultPoint.FromString("", 0, clRed, 5);
  DefaultTrendline.FromString("", psSolid, clBlue, 1);
  DefaultPointLine.FromString("", psClear, clBlue, 1);
  DefaultShade.FromString("", bsBDiagonal, clGreen, 0);
  DefaultRelation.FromString("", bsBDiagonal, clGreen, 1);
  DefaultTangent.FromString("", psSolid, clRed, 1);
  DefaultDif.FromString("", psSolid, clGreen, 1);
  StrToFont(DEFAULT_POINT_FONT, DefaultPointLabelFont);
  StrToFont(DEFAULT_TEXT_LABEL_FONT, DefaultLabelFont);
}
//---------------------------------------------------------------------------
void TProperty::Read(const TConfigRegistry &Registry)
{
  //Use current value as default
  RoundTo = Registry.Read("RoundTo", RoundTo);
  ComplexFormat = Registry.ReadEnum("ComplexFormat", ComplexFormat);
  SavePos = Registry.Read("SavePosition", SavePos);
  CheckForUpdate = Registry.Read("CheckForUpdate", CheckForUpdate);

  DefaultFunction.FromString(Registry.Read("DefaultFunction", ""), psSolid, clRed, 1);
  DefaultPoint.FromString(Registry.Read("DefaultPoint", ""), 0, clRed, 5);
  DefaultTrendline.FromString(Registry.Read("DefaultTrendline", ""), psSolid, clRed, 1);
  DefaultPointLine.FromString(Registry.Read("DefaultPointLine", ""), psClear, clBlue, 1);
  DefaultShade.FromString(Registry.Read("DefaultShade", ""), bsBDiagonal, clGreen, 0);
  DefaultRelation.FromString(Registry.Read("DefaultRelation", ""), bsBDiagonal, clGreen, 0);
  DefaultTangent.FromString(Registry.Read("DefaultTangent", ""), psSolid, clRed, 1);
  DefaultDif.FromString(Registry.Read("DefaultDif", ""), psSolid, clRed, 1);
  StrToFont(Registry.Read("DefaultPointLabelFont", ""), DefaultPointLabelFont);
  StrToFont(Registry.Read("DefaultLabelFont", ""), DefaultLabelFont);

  ShowTipsAtStartup = Registry.Read("ShowTipsAtStartup", ShowTipsAtStartup);
  NextTip = Registry.Read("NextTip", NextTip) + RES_FIRST_TIP; //Tips in registry count from 0
  FontScale = Registry.Read("FontScale", FontScale);
}
//---------------------------------------------------------------------------
void TProperty::Write(TConfigRegistry &Registry)
{
  Registry.Write("RoundTo", RoundTo);
  Registry.Write("ComplexFormat", ComplexFormat);
  Registry.Write("SavePosition", SavePos);
  Registry.Write("CheckForUpdate", CheckForUpdate);

  Registry.Write("DefaultFunction", DefaultFunction.ToString());
  Registry.Write("DefaultPoint", DefaultPoint.ToString());
  Registry.Write("DefaultPointLine", DefaultPointLine.ToString());
  Registry.Write("DefaultTrendline", DefaultTrendline.ToString());
  Registry.Write("DefaultShade", DefaultShade.ToString());
  Registry.Write("DefaultRelation", DefaultRelation.ToString());
  Registry.Write("DefaultTangent", DefaultTangent.ToString());
  Registry.Write("DefaultDif", DefaultDif.ToString());
  Registry.Write("DefaultPointLabelFont", FontToStr(DefaultPointLabelFont));
  Registry.Write("DefaultLabelFont", FontToStr(DefaultLabelFont));

  Registry.Write("ShowTipsAtStartup", ShowTipsAtStartup);
  Registry.Write("NextTip", NextTip - RES_FIRST_TIP); //Tips in registry count from 0

  Registry.Write("FontScale", FontScale);
}
//---------------------------------------------------------------------------
/////////////////////
// TCustomFunction //
/////////////////////
std::string TCustomFunction::CheckAndTrimName(const std::string &Str, unsigned Offset)
{
  unsigned Begin = Str.find_first_not_of(" ");
  unsigned End = Str.find_last_not_of(" ");
  if(Begin == std::string::npos)
    throw ECustomFunctionError(Offset ? cfeEmptyArg : cfeEmptyName, Offset);

  std::string Name = Str.substr(Begin, End - Begin + 1);
  if(Name[0] == '$')
    throw ECustomFunctionError(Offset ? cfeInvalidArg : cfeInvalidName, Begin + Offset, Name);

  if(!Func32::IsValidName(Name))
    throw ECustomFunctionError(Offset ? cfeInvalidArg : cfeInvalidName, Begin + Offset, Name);

  return Name;
}
//---------------------------------------------------------------------------
TCustomFunction::TCustomFunction(const std::string &Str, const std::string &AText)
  : Text(AText)
{
  unsigned Begin = Str.find('(');
  if(Begin == std::string::npos)
    Name = CheckAndTrimName(Str, 0);
  else
  {
    Name = CheckAndTrimName(Str.substr(0, Begin), 0);

    ++Begin;
    unsigned End;
    while((End = Str.find(',', Begin)) != std::string::npos)
    {
      Arguments.push_back(CheckAndTrimName(Str.substr(Begin, End - Begin), Begin));
      Begin = End + 1;
    }

    End = Str.find(')', Begin);
    if(End == std::string::npos)
      throw ECustomFunctionError(cfeEndParMissing, End);

    if(End != Str.size() - 1)
      throw ECustomFunctionError(cfeCharAfterEndPar, End);
    Arguments.push_back(CheckAndTrimName(Str.substr(Begin, End - Begin), Begin));
  }
}
//---------------------------------------------------------------------------
std::string TCustomFunction::GetName() const
{
  std::string Str = Name;
  if(!Arguments.empty())
  {
    Str += '(';
    for(unsigned I = 0; I < Arguments.size() - 1; I++)
      Str += Arguments[I] + ',';
    Str += Arguments.back() + ')';
  }
  return Str;
}
//---------------------------------------------------------------------------
//////////////////////
// TCustomFunctions //
//////////////////////
void TCustomFunctions::Add(const std::string &Str, const std::string &Value)
{
  TCustomFunction CustomFunction(Str, Value);
  if(SymbolList.Exists(CustomFunction.Name))
    throw ECustomFunctionError(cfeDoubleDefinedSymbol, 0, Str);
  Functions.push_back(CustomFunction);
  SymbolList.Add(CustomFunction.Name, "0", CustomFunction.Arguments);
}
//---------------------------------------------------------------------------
void TCustomFunctions::Replace(const std::string &Name, const std::string &Value)
{
  for(Iterator Iter = Functions.begin(); Iter != Functions.end(); ++Iter)
    if(Iter->Name == Name)
    {
      Iter->Text = Value;
      SymbolList.Add(Name, Value, Iter->Arguments);
      return;
    }
  throw ECustomFunctionError(cfeSymbolUndefined, 0, Name);
}
//---------------------------------------------------------------------------
void TCustomFunctions::Replace(const std::string &Name, long double Value)
{
  for(Iterator Iter = Functions.begin(); Iter != Functions.end(); ++Iter)
    if(Iter->Name == Name)
    {
      Iter->Text = ToString(Value);
      SymbolList.Add(Name, Func32::TCustomFunc(Value));
      return;
    }
  throw ECustomFunctionError(cfeSymbolUndefined, 0, Name);
}
//---------------------------------------------------------------------------
const std::string& TCustomFunctions::GetValue(const std::string &Name) const
{
  for(ConstIterator Iter = Functions.begin(); Iter != Functions.end(); ++Iter)
    if(Iter->Name == Name)
    {
      return Iter->Text;
    }
  throw ECustomFunctionError(cfeSymbolUndefined, 0, Name);
}
//---------------------------------------------------------------------------
void TCustomFunctions::Update()
{
  unsigned I;
  try
  {
    for(I = 0; I < Functions.size(); I++)
      SymbolList.Add(Functions[I].Name, Func32::TCustomFunc(Functions[I].Text, Functions[I].Arguments, SymbolList));
    SymbolList.Update();

    //Replace all constant expressions with their value 
    std::vector<Func32::TComplex> DummyArgs;
    for(Func32::TSymbolList::TIterator Iter = SymbolList.Begin(); Iter != SymbolList.End(); ++Iter)
      if(Iter->second.GetArguments().size() == 0)
      {
        Func32::TComplex Value = Iter->second.Calc(DummyArgs);
        Iter->second = Func32::TCustomFunc(Value);
      }
    SymbolList.Update();  
  }
  catch(const Func32::EParseError &E)
  {
    throw ECustomFunctionError(cfeParseError, I, E);
  }
}
//---------------------------------------------------------------------------
void TCustomFunctions::WriteToIni(TConfigFile &IniFile) const
{
  for(ConstIterator Iter = Functions.begin(); Iter != Functions.end(); ++Iter)
    IniFile.Write("CustomFunctions", Iter->GetName(), Iter->Text);
}
//---------------------------------------------------------------------------
void TCustomFunctions::ReadFromIni(const TConfigFile &IniFile)
{
  if(IniFile.SectionExists("CustomFunctions"))
  {
    std::pair<TConfigFile::TSectionIterator, TConfigFile::TSectionIterator> IterPair = IniFile.GetSectionData("CustomFunctions");
    for(TConfigFile::TSectionIterator Iter = IterPair.first; Iter != IterPair.second; ++Iter)
      Add(Iter->first, Iter->second);
    Update();
  }
}
//---------------------------------------------------------------------------
void TCustomFunctions::Clear()
{
  Functions.clear();
//  SymbolList.Clear();
  SymbolList = GlobalSymbolList;
}
//---------------------------------------------------------------------------
//////////////////
// TDefaultData //
//////////////////
std::string TDefaultData::ToString() const
{
  return ::ToString(Style) + "," + ColorToString(Color).c_str() + "," + ::ToString(Size);
}
//---------------------------------------------------------------------------
void TDefaultData::FromString(const std::string &Str, unsigned AStyle, TColor AColor, unsigned ASize)
{
  Style = AStyle;
  Color = AColor;
  Size = ASize;

  if(Str.empty())
    return;

  try
  {
    size_t Pos = Str.find(',');
    Style = ToIntDef(Str.substr(0, Pos), AStyle);
    size_t Pos2 = Str.find(',', Pos + 1);
    Color = StringToColor(Str.substr(Pos + 1, Pos2 - Pos - 1).c_str());
    Size = ToIntDef(Str.substr(Pos2 + 1), ASize);
  }
  catch(...)
  {
  }
}
//---------------------------------------------------------------------------
void TDefaultData::Set(unsigned AStyle, TColor AColor, unsigned ASize)
{
  Style = AStyle;
  Color = AColor;
  Size = ASize;
}
//---------------------------------------------------------------------------
////////////////////
// TAnimationInfo //
////////////////////
void TAnimationInfo::WriteToIni(TConfigFile &IniFile) const
{
  if(!Constant.empty())
    IniFile.Write("Animate", "Constant", Constant);
  for(std::map<std::string, TAnimationConstant>::const_iterator Iter = ConstantList.begin(); Iter != ConstantList.end(); ++Iter)
    IniFile.Write("Animate", '%' + Iter->first, Iter->second.Min + ';' + Iter->second.Max + ';' + Iter->second.Step);
}
//---------------------------------------------------------------------------
void TAnimationInfo::ReadFromIni(const TConfigFile &IniFile)
{
  if(IniFile.SectionExists("Animate"))
  {
    Constant = IniFile.Read("Animate", "Constant", "");
    std::pair<TConfigFile::TSectionIterator, TConfigFile::TSectionIterator> Section = IniFile.GetSectionData("Animate");
    for(TConfigFile::TSectionIterator Iter = Section.first; Iter != Section.second; ++Iter)
      if(Iter->first[0] == '%')
      {
        TAnimationConstant &AnimationConstant = ConstantList[Iter->first.substr(1)];
        int Pos = Iter->second.find(';');
        AnimationConstant.Min = Iter->second.substr(0, Pos);
        int Pos2 = Iter->second.find(';', Pos+1);
        AnimationConstant.Max = Iter->second.substr(Pos+1, Pos2 - Pos - 1);
        AnimationConstant.Step = Iter->second.substr(Pos2+1);
      }
  }
}
//---------------------------------------------------------------------------
void TAnimationInfo::Clear()
{
  Constant.clear();
  ConstantList.clear();
}
//---------------------------------------------------------------------------

