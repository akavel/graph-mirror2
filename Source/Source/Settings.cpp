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
TProperty Property;
//---------------------------------------------------------------------------
std::wistream& operator >>(std::wistream &Stream, const Func32::TDblPoint&)
{
  return Stream;
}
std::wostream& operator <<(std::wostream &Stream, const Func32::TDblPoint&)
{
  return Stream;
}
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
  LegendPos(0,0), AxesArrows(aaPositiveEnd), NumberPlacement(npCenter), GridStyle(gsLines)  
{
}
//---------------------------------------------------------------------------
void TAxis::WriteToIni(TConfigFileSection &Section, const std::wstring &Prefix) const
{
  Section.Write(Prefix + L"Min", Min);
  Section.Write(Prefix + L"Max", Max);
  Section.Write(Prefix + L"TickUnit", TickUnit);
  Section.Write(Prefix + L"GridUnit", GridUnit);
  Section.Write(Prefix + L"LogScl", LogScl, false);
  Section.Write(Prefix + L"ShowTicks", ShowTicks, true);
  Section.Write(Prefix + L"ShowGrid", ShowGrid, false);
  Section.Write(Prefix + L"ShowLabel", ShowLabel, true);
  Section.Write(Prefix + L"AutoTick", AutoTick, true);
  Section.Write(Prefix + L"AutoGrid", AutoGrid, true);
  Section.Write(Prefix + L"Label", Label, Prefix);
  Section.Write(Prefix + L"ShowNumbers", ShowNumbers, true);
  Section.Write(Prefix + L"AxisCross", AxisCross, 0.0);
  Section.Write(Prefix + L"MultiplyOfPi", MultiplyOfPi, false);
}
//---------------------------------------------------------------------------
void TAxes::WriteToIni(TConfigFileSection &Section) const
{
  xAxis.WriteToIni(Section, L"x");
  yAxis.WriteToIni(Section, L"y");

  Section.Write(L"AxesColor", AxesColor);
  Section.Write(L"GridColor", GridColor);
  Section.Write(L"BackgroundColor", BackgroundColor, clWhite);
  Section.Write(L"NumberFont", FontToStr(NumberFont), DEFAULT_NUMBER_FONT);
  Section.Write(L"LabelFont", FontToStr(LabelFont), DEFAULT_LABEL_FONT);
  Section.Write(L"LegendFont", FontToStr(LegendFont), DEFAULT_LEGEND_FONT);
  Section.Write(L"ShowLegend", ShowLegend);
  Section.Write(L"Radian", Trigonometry == Func32::Radian);
  Section.Write(L"Title", Title, std::wstring());
  Section.Write(L"TitleFont", FontToStr(TitleFont), DEFAULT_TITLE_FONT);
  Section.Write(L"AxesStyle", AxesStyle, asCrossed);
  Section.Write(L"LegendPlacement", LegendPlacement, lpTopRight);
  if(LegendPlacement == lpCustom)
    Section.Write(L"LegendPos", LegendPos);
  Section.Write(L"GridSize", GridSize, 1U);
  Section.Write(L"CalcComplex", CalcComplex, false);
  Section.Write(L"ZoomSquare", ZoomSquare, false);
  Section.Write(L"AxesArrows", AxesArrows, aaPositiveEnd);
  Section.Write(L"NumberPlacement", NumberPlacement, npCenter);
  Section.Write(L"GridStyle", GridStyle, gsLines);
}
//---------------------------------------------------------------------------
void TAxis::ReadFromIni(const TConfigFileSection &Section, const std::wstring &Prefix)
{
  Min = Section.Read(Prefix + L"Min", -10.0);
  Max = Section.Read(Prefix + L"Max", 10.0);

  LogScl = Section.Read(Prefix + L"LogScl", false);
  TickUnit = Section.Read(Prefix + L"TickUnit", 1.0);
  GridUnit = Section.Read(Prefix + L"GridUnit", 1.0);
  ShowTicks = Section.Read(Prefix + L"ShowTicks", true);
  ShowGrid = Section.Read(Prefix + L"ShowGrid", false);
  ShowLabel = Section.Read(Prefix + L"ShowLabel", true);
  AutoTick = Section.Read(Prefix + L"AutoTick", true);
  AutoGrid = Section.Read(Prefix + L"AutoGrid", true);
  ShowNumbers = Section.Read(Prefix + L"ShowNumbers", true);
  Label = Section.Read(Prefix + L"Label", Prefix);
  AxisCross = Section.Read(Prefix + L"AxisCross", 0.0);
  MultiplyOfPi = Section.Read(Prefix + L"MultiplyOfPi", false);
}
//---------------------------------------------------------------------------
void TAxes::ReadFromIni(const TConfigFileSection &Section)
{
  xAxis.ReadFromIni(Section, L"x");
  yAxis.ReadFromIni(Section, L"y");

  AxesColor = Section.Read(L"AxesColor", clBlue);
  GridColor = Section.Read(L"GridColor", static_cast<TColor>(0x00FF9999));
  BackgroundColor = Section.Read(L"BackgroundColor", clWhite);
  StrToFont(Section.Read(L"NumberFont", DEFAULT_NUMBER_FONT), NumberFont);
  StrToFont(Section.Read(L"LabelFont", DEFAULT_LABEL_FONT), LabelFont);
  StrToFont(Section.Read(L"LegendFont", DEFAULT_LEGEND_FONT), LegendFont);
  StrToFont(Section.Read(L"TitleFont", DEFAULT_TITLE_FONT),TitleFont);
  ShowLegend = Section.Read(L"ShowLegend", true);
  Title = Section.Read(L"Title", std::wstring());
  Trigonometry = Section.Read(L"Radian", true) ? Func32::Radian : Func32::Degree;
  AxesStyle = Section.Read(L"AxesStyle", asCrossed);
  LegendPlacement = Section.Read(L"LegendPlacement", lpTopRight);
  LegendPos = Section.Read(L"LegendPos", Func32::TDblPoint(0, 0));
  GridSize = Section.Read(L"GridSize", 1);
  CalcComplex = Section.Read(L"CalcComplex", false);
  ZoomSquare = Section.Read(L"ZoomSquare", false);
  AxesArrows = Section.Read(L"AxesArrows", aaPositiveEnd);
  NumberPlacement = Section.Read(L"NumberPlacement", npCenter);
  GridStyle = Section.Read(L"GridStyle", gsLines);
}
//---------------------------------------------------------------------------
void TAxes::HandleZoomSquare(double xyScale)
{
  if(ZoomSquare && xAxis.LogScl == yAxis.LogScl)
  {
    if(xAxis.LogScl)
    {
      double yMiddle = std::exp((std::log(yAxis.Max) + std::log(yAxis.Min)) / 2);
      double dy = std::exp(xyScale * (std::log(xAxis.Max) - std::log(xAxis.Min)) / 2);
      yAxis.Min = yMiddle / dy;
      yAxis.Max = yMiddle * dy;
    }
    else
    {
      double yMiddle = (yAxis.Min + yAxis.Max) /2;
      double dy = xyScale * (xAxis.Max - xAxis.Min);
      yAxis.Min = yMiddle - dy / 2;
      yAxis.Max = yMiddle + dy / 2;
    }
  }
  else
    ZoomSquare = false;
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
  DefaultFunction.FromString(L"", psSolid, clRed, 1);
  DefaultPoint.FromString(L"", 0, clRed, 5);
  DefaultTrendline.FromString(L"", psSolid, clBlue, 1);
  DefaultPointLine.FromString(L"", psClear, clBlue, 1);
  DefaultShade.FromString(L"", bsBDiagonal, clGreen, 0);
  DefaultRelation.FromString(L"", bsBDiagonal, clGreen, 1);
  DefaultTangent.FromString(L"", psSolid, clRed, 1);
  DefaultDif.FromString(L"", psSolid, clGreen, 1);
  StrToFont(DEFAULT_POINT_FONT, DefaultPointLabelFont);
  StrToFont(DEFAULT_TEXT_LABEL_FONT, DefaultLabelFont);
}
//---------------------------------------------------------------------------
void TProperty::Read(const TConfigRegistry &Registry)
{
  //Use current value as default
  RoundTo = Registry.Read("RoundTo", RoundTo);
  ComplexFormat = static_cast<TComplexFormat>(Registry.Read("ComplexFormat", ComplexFormat));
  SavePos = Registry.Read("SavePosition", SavePos);
  CheckForUpdate = Registry.Read("CheckForUpdate", CheckForUpdate);

  DefaultFunction.FromString(Registry.Read(L"DefaultFunction", L""), psSolid, clRed, 1);
  DefaultPoint.FromString(Registry.Read(L"DefaultPoint", L""), 0, clRed, 5);
  DefaultTrendline.FromString(Registry.Read(L"DefaultTrendline", L""), psSolid, clRed, 1);
  DefaultPointLine.FromString(Registry.Read(L"DefaultPointLine", L""), psClear, clBlue, 1);
  DefaultShade.FromString(Registry.Read(L"DefaultShade", L""), bsBDiagonal, clGreen, 0);
  DefaultRelation.FromString(Registry.Read(L"DefaultRelation", L""), bsBDiagonal, clGreen, 0);
  DefaultTangent.FromString(Registry.Read(L"DefaultTangent", L""), psSolid, clRed, 1);
  DefaultDif.FromString(Registry.Read(L"DefaultDif", L""), psSolid, clRed, 1);
  StrToFont(Registry.Read(L"DefaultPointLabelFont", L""), DefaultPointLabelFont);
  StrToFont(Registry.Read(L"DefaultLabelFont", L""), DefaultLabelFont);

  ShowTipsAtStartup = Registry.Read(L"ShowTipsAtStartup", ShowTipsAtStartup);
  NextTip = Registry.Read(L"NextTip", NextTip) + RES_FIRST_TIP; //Tips in registry count from 0
  FontScale = Registry.Read(L"FontScale", FontScale);
}
//---------------------------------------------------------------------------
void TProperty::Write(TConfigRegistry &Registry)
{
  Registry.Write("RoundTo", RoundTo);
  Registry.Write("ComplexFormat", ComplexFormat);
  Registry.Write("SavePosition", SavePos);
  Registry.Write("CheckForUpdate", CheckForUpdate);

  Registry.Write(L"DefaultFunction", DefaultFunction.ToString());
  Registry.Write(L"DefaultPoint", DefaultPoint.ToString());
  Registry.Write(L"DefaultPointLine", DefaultPointLine.ToString());
  Registry.Write(L"DefaultTrendline", DefaultTrendline.ToString());
  Registry.Write(L"DefaultShade", DefaultShade.ToString());
  Registry.Write(L"DefaultRelation", DefaultRelation.ToString());
  Registry.Write(L"DefaultTangent", DefaultTangent.ToString());
  Registry.Write(L"DefaultDif", DefaultDif.ToString());
  Registry.Write(L"DefaultPointLabelFont", FontToStr(DefaultPointLabelFont));
  Registry.Write(L"DefaultLabelFont", FontToStr(DefaultLabelFont));

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
void TCustomFunctions::Add(const std::string &Name, const Func32::TArgType &Args, const std::string &Text)
{
  TCustomFunction CustomFunction(Name, Args, Text);
  SymbolList.Add(Name, Text, CustomFunction.Arguments);
  std::string LowerName = ToLower(Name);
  for(TIterator Iter = Functions.begin(); Iter != Functions.end(); ++Iter)
    if(ToLower(Iter->Name) == LowerName)
    {
      *Iter = CustomFunction;
      return;
    }
  Functions.push_back(CustomFunction);
}
//---------------------------------------------------------------------------
void TCustomFunctions::Replace(const std::string &Name, const std::string &Value)
{
  for(TIterator Iter = Functions.begin(); Iter != Functions.end(); ++Iter)
    if(Iter->Name == Name)
    {
      Iter->Text = Value;
      SymbolList.Add(Name, Func32::TCustomFunc(Value, Iter->Arguments, SymbolList));
      return;
    }
  throw ECustomFunctionError(cfeSymbolUndefined, 0, Name);
}
//---------------------------------------------------------------------------
void TCustomFunctions::Replace(const std::string &Name, long double Value)
{
  for(TIterator Iter = Functions.begin(); Iter != Functions.end(); ++Iter)
    if(Iter->Name == Name)
    {
      Iter->Text = ToString(Value);
      SymbolList.Add(Name, Func32::TCustomFunc(Value));
      return;
    }
  throw ECustomFunctionError(cfeSymbolUndefined, 0, Name);
}
//---------------------------------------------------------------------------
void TCustomFunctions::Delete(const std::string &Name)
{
  for(TIterator Iter = Functions.begin(); Iter != Functions.end(); ++Iter)
    if(Iter->Name == Name)
    {
      Functions.erase(Iter);
      SymbolList.Erase(Name);
      return;
    }
  throw ECustomFunctionError(cfeSymbolUndefined, 0, Name);
}
//---------------------------------------------------------------------------
const TCustomFunction& TCustomFunctions::GetValue(const std::string &Name) const
{
  for(TConstIterator Iter = Functions.begin(); Iter != Functions.end(); ++Iter)
    if(Iter->Name == Name)
      return *Iter;
  throw ECustomFunctionError(cfeSymbolUndefined, 0, Name);
}
//---------------------------------------------------------------------------
void TCustomFunctions::Update()
{
  unsigned I;
  try
  {
    for(I = 0; I < Functions.size(); I++)
    {
      std::string Expression = Functions[I].Text.substr(0, Functions[I].Text.find("#"));
      SymbolList.Add(Functions[I].Name, Func32::TCustomFunc(Expression, Functions[I].Arguments, SymbolList));
    }
    SymbolList.Update();

    //Replace all constant expressions with their value
    std::vector<Func32::TComplex> DummyArgs;
    for(Func32::TSymbolList::TIterator Iter = SymbolList.Begin(); Iter != SymbolList.End(); ++Iter)
      if(Iter->second.GetArguments().size() == 0)
      {
        Iter->second.SetTrigonometry(Data.Axes.Trigonometry);
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
void TCustomFunctions::WriteToIni(TConfigFileSection &Section) const
{
  for(TConstIterator Iter = Functions.begin(); Iter != Functions.end(); ++Iter)
    Section.Write(ToWString(Iter->GetName()), ToWString(Iter->Text));
}
//---------------------------------------------------------------------------
void TCustomFunctions::ReadFromIni(const TConfigFileSection &Section)
{
  if(!Section.Empty())
  {
    for(TConfigFileSection::TIterator Iter = Section.Begin(); Iter != Section.End(); ++Iter)
      Add(ToString(Iter->first), ToString(Iter->second));
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
std::wstring TDefaultData::ToString() const
{
  return ToWString(Style) + L"," + ToWString(ColorToString(Color)) + L"," + ToWString(Size);
}
//---------------------------------------------------------------------------
void TDefaultData::FromString(const std::wstring &Str, unsigned AStyle, TColor AColor, unsigned ASize)
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
void TAnimationInfo::WriteToIni(TConfigFileSection &Section) const
{
  if(!Constant.empty())
  {
    Section.Write(L"Constant", ToWString(Constant));
    Section.Write(L"FramesPerSecond", FramesPerSecond);
    Section.Write(L"Width", Width, 0U);
    Section.Write(L"Height", Height, 0U);
  }
  for(std::map<std::string, TAnimationConstant>::const_iterator Iter = ConstantList.begin(); Iter != ConstantList.end(); ++Iter)
    Section.Write(ToWString('%' + Iter->first), ToWString(Iter->second.Min + ';' + Iter->second.Max + ';' + Iter->second.Step));
}
//---------------------------------------------------------------------------
void TAnimationInfo::ReadFromIni(const TConfigFileSection &Section)
{
  if(!Section.Empty())
  {
    Constant = ToString(Section.Read(L"Constant", L""));
    FramesPerSecond = Section.Read(L"FramesPerSecond", 1);
    Width = Section.Read(L"Width", 0);
    Height = Section.Read(L"Height", 0);
    for(TConfigFileSection::TIterator Iter = Section.Begin(); Iter != Section.End(); ++Iter)
      if(Iter->first[0] == '%')
      {
        TAnimationConstant &AnimationConstant = ConstantList[ToString(Iter->first.substr(1))];
        int Pos = Iter->second.find(';');
        AnimationConstant.Min = ToString(Iter->second.substr(0, Pos));
        int Pos2 = Iter->second.find(';', Pos+1);
        AnimationConstant.Max = ToString(Iter->second.substr(Pos+1, Pos2 - Pos - 1));
        AnimationConstant.Step = ToString(Iter->second.substr(Pos2+1));
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

