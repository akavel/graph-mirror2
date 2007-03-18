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
#include "GraphElem.h"
#include <float.h>
#include "ConfigFile.h"
#include "IGraphic.h"
#include "Tokenizer.h"
#include "VersionInfo.h"
#include <boost/lexical_cast.hpp>
#include <boost/mem_fn.hpp>
//---------------------------------------------------------------------------
////////////////
// TTextValue //
////////////////
TTextValue::TTextValue(double AValue) : Value(AValue)
{
  if(_finite(Value))
    Text = AnsiString(AValue).c_str();
}
//---------------------------------------------------------------------------
void TTextValue::Update(const TData &Data)
{
  try
  {
    if(Text.empty())
      return; //Nothing to update. It is already +/-INF
    Value = Data.Calc(Text);
  }
  catch(Func32::EFuncError &E)
  {
    Value = NAN;
  }
}
//---------------------------------------------------------------------------
void TTextValue::Set(const std::string AText, const TData &Data, bool IgnoreErrors)
{
  Text = AText;
  if(Text == "INF" || Text == "+INF")
  {
    Text = "";
    Value = INF;
  }
  else if(Text == "-INF")
  {
    Text = "";
    Value = -INF;
  }
  else
    try
    {
      Value = Data.Calc(Text);
    }
    catch(Func32::EFuncError &E)
    {
      if(!IgnoreErrors)
        throw;
      Value = NAN;
    }
}
//---------------------------------------------------------------------------
std::ostream& operator<<(std::ostream &Stream, const TTextValue &TextValue)
{
  if(TextValue.Value == INF)
    return Stream << "+INF";
  else if(TextValue.Value == -INF)
    return Stream << "-INF";
  return Stream << TextValue.Text;
}
//---------------------------------------------------------------------------
////////////////
// TGraphElem //
////////////////
TGraphElem::TGraphElem(const TGraphElem &Elem)
  : Visible(Elem.Visible), ShowInLegend(Elem.ShowInLegend), Data(NULL), LegendText(Elem.LegendText)
{
  //Do not copy ChildList; It must be copyed from the derived class to be able to call SetParentFunc()
}
//---------------------------------------------------------------------------
void TGraphElem::WriteToIni(TConfigFile &IniFile, const std::string &Section) const
{
  IniFile.Write(Section, "Visible", Visible, true);
  IniFile.Write(Section, "ShowInLegend", ShowInLegend, true);
  IniFile.Write(Section, "LegendText", ToString(LegendText), std::string());
}
//---------------------------------------------------------------------------
void TGraphElem::ReadFromIni(const TConfigFile &IniFile, const std::string &Section)
{
  Visible = IniFile.Read(Section, "Visible", true);
  ShowInLegend = IniFile.Read(Section, "ShowInLegend", true);
  LegendText = ToWString(IniFile.Read(Section, "LegendText", ""));
}
//---------------------------------------------------------------------------
void TGraphElem::AddChild(const TGraphElemPtr &Elem)
{
  ChildList.push_back(Elem);
  Elem->SetData(&GetData());
}
//---------------------------------------------------------------------------
void TGraphElem::ReplaceChild(unsigned Index, const TGraphElemPtr &Elem)
{
  BOOST_ASSERT(Index < ChildList.size());
  ChildList[Index] = Elem;
  Elem->SetData(&GetData());
}
//---------------------------------------------------------------------------
unsigned TGraphElem::GetChildIndex(const TGraphElemPtr &Elem) const
{
  return IndexOf(ChildList, Elem);
}
//---------------------------------------------------------------------------
void TGraphElem::SetData(const TData *AData)
{
  Data = AData;
  for(unsigned I = 0; I < ChildList.size(); I++)
    ChildList[I]->SetData(AData);
}
//---------------------------------------------------------------------------
///////////////////
// TBaseFuncType //
///////////////////
TBaseFuncType::TBaseFuncType() :
  Color(clRed), Size(1), Style(psSolid), Steps(0), DrawType(dtAuto),
  StartPointStyle(0), EndPointStyle(0), From(-INF), To(INF)
{
}
//---------------------------------------------------------------------------
//NOTE: This does not copy tangents; Use CopyTangents()
TBaseFuncType::TBaseFuncType(const TBaseFuncType &F) : TGraphElem(F), DrawType(F.DrawType),
  Color(F.Color), Size(F.Size), Style(F.Style),
  From(F.From), To(F.To), Steps(F.Steps), StartPointStyle(F.StartPointStyle), EndPointStyle(F.EndPointStyle)
{
}
//---------------------------------------------------------------------------
void TBaseFuncType::WriteToIni(TConfigFile &IniFile, const std::string &Section) const
{
  TGraphElem::WriteToIni(IniFile, Section);

  IniFile.Write(Section, "From", From.Text, std::string());
  IniFile.Write(Section, "To", To.Text, std::string());
  IniFile.Write(Section, "Steps", Steps.Text, std::string());
  IniFile.Write(Section, "Style", Style, psSolid);
  IniFile.Write(Section, "Color", Color);
  IniFile.Write(Section, "Size", Size, 1U);
  IniFile.Write(Section, "StartPoint", StartPointStyle, 0U);
  IniFile.Write(Section, "EndPoint", EndPointStyle, 0U);
  IniFile.Write(Section, "DrawType", DrawType, dtAuto);

  //Write tangents
  unsigned TanCount = 0;
  for(unsigned N = 0; N < ChildList.size(); N++)
    if(dynamic_cast<TTan*>(ChildList[N].get()))
    ChildList[N]->WriteToIni(IniFile, Section + "Tan" + ToString(++TanCount));
  IniFile.Write(Section, "TanCount", TanCount, 0U);
}
//---------------------------------------------------------------------------
void TBaseFuncType::ReadFromIni(const TConfigFile &IniFile, const std::string &Section)
{
  GetFunc().SetTrigonometry(GetData().Axes.Trigonometry);
  Style = IniFile.ReadEnum(Section, "Style", psSolid);
  Color = IniFile.Read(Section, "Color", clRed);
  Size = IniFile.Read(Section, "Size", 1);
  if(IniFile.KeyExists(Section, ">From"))
    From.Set(IniFile.Read(Section, ">From", "-INF"), GetData()); //Caused by bug in MS Outlook Express
  else
    From.Set(IniFile.Read(Section, "From", "-INF"), GetData());
  To.Set(IniFile.Read(Section, "To", "+INF"), GetData());
  Steps.Set(IniFile.Read(Section, "Steps", ""), GetData(), true);
  int TanCount = IniFile.Read(Section, "TanCount", 0);
  StartPointStyle = IniFile.Read(Section, "StartPoint", 0);
  EndPointStyle = IniFile.Read(Section, "EndPoint", 0);
  DrawType = IniFile.ReadEnum(Section, "DrawType", dtAuto);

  //Create list of tangents
  for(int I = 0; I < TanCount; I++)
  {
    boost::shared_ptr<TTan> Tan(new TTan);
    Tan->SetData(&GetData());
    Tan->ReadFromIni(IniFile, Section + "Tan" + ToString(I+1));
    ChildList.push_back(Tan);
  }

  TGraphElem::ReadFromIni(IniFile, Section);
}
//---------------------------------------------------------------------------
const boost::shared_ptr<TBaseFuncType> TBaseFuncType::CloneWithTangents(TBaseFuncType *Dest, const TBaseFuncType *Src)
{
  boost::shared_ptr<TBaseFuncType> Func(Dest);
  for(unsigned I = 0; I < Src->ChildList.size(); I++)
  {
    Func->ChildList.push_back(Src->ChildList[I]->Clone());
    Func->ChildList.back()->SetParentFunc(Func);
  }
  return Func;
}
//---------------------------------------------------------------------------
void TBaseFuncType::ClearCache()
{
  Points.clear();
  PointNum.clear();
  sList.clear();

  for(unsigned N = 0; N < ChildList.size(); N++)
    ChildList[N]->ClearCache();
}
//---------------------------------------------------------------------------
void TBaseFuncType::Update()
{
  GetFunc().SetTrigonometry(GetData().Axes.Trigonometry);
  GetFunc().Update(GetData().CustomFunctions.SymbolList);
  From.Update(GetData());
  To.Update(GetData());
  Steps.Update(GetData());
  std::for_each(ChildList.begin(), ChildList.end(), boost::mem_fn(&TGraphElem::Update));
}
//---------------------------------------------------------------------------
Func32::TCoord<long double> TBaseFuncType::Eval(long double t) const
{
  if(!GetData().Axes.CalcComplex)
    return GetFunc().Calc(t);

  Func32::TCoord<Func32::TComplex> Result = GetFunc().Calc(Func32::TComplex(t));
  if(imag(Result.x) || imag(Result.y))
    throw Func32::ECalcError(Func32::ecComplexError);
  return Func32::TCoord<long double>(real(Result.x), real(Result.y));
}
//---------------------------------------------------------------------------
long double TBaseFuncType::CalcArea(long double From, long double To) const
{
  return GetFunc().CalcArea(From, To, 1000);
}
//---------------------------------------------------------------------------

//////////////
// TStdFunc //
//////////////
TStdFunc::TStdFunc(const std::string &AText, const Func32::TSymbolList &SymbolList, Func32::TTrigonometry Trig)
  : Text(AText), Func(AText, "x", SymbolList, Trig)
{
}
//---------------------------------------------------------------------------
void TStdFunc::WriteToIni(TConfigFile &IniFile, const std::string &Section) const
{
  IniFile.Write(Section, "FuncType", ftStdFunc);
  IniFile.Write(Section, "y", Text);

  TBaseFuncType::WriteToIni(IniFile, Section);
}
//---------------------------------------------------------------------------
void TStdFunc::ReadFromIni(const TConfigFile &IniFile, const std::string &Section)
{
  TBaseFuncType::ReadFromIni(IniFile, Section);
  Text = IniFile.Read(Section, "y", "");

  try
  {
    Func.SetFunc(Text, "x", GetData().CustomFunctions.SymbolList);
  }
  catch(Func32::EParseError &E)
  {
    Form1->ShowStatusError(GetErrorMsg(E));
  }
}
//---------------------------------------------------------------------------
std::wstring TStdFunc::MakeText() const
{
  return ToWString("f(x)=" + Text);
}
//---------------------------------------------------------------------------
boost::shared_ptr<TBaseFuncType> TStdFunc::MakeDifFunc()
{
  boost::shared_ptr<TStdFunc> DifFunc(new TStdFunc);
  DifFunc->Func = Func.MakeDif();
  DifFunc->Text = DifFunc->Func.MakeText();
  return DifFunc;
}
//---------------------------------------------------------------------------
std::pair<double,double> TStdFunc::GetCurrentRange() const
{
  return std::make_pair(std::max(GetData().Axes.xAxis.Min, From.Value), std::min(GetData().Axes.xAxis.Max, To.Value));
}
//---------------------------------------------------------------------------
//////////////
// TParFunc //
//////////////
TParFunc::TParFunc(const std::string &AxText, const std::string &AyText, const Func32::TSymbolList &SymbolList, Func32::TTrigonometry Trig)
  : xText(AxText), yText(AyText), Func(AxText, AyText, "t", SymbolList, Trig)
{
}
//---------------------------------------------------------------------------
void TParFunc::WriteToIni(TConfigFile &IniFile, const std::string &Section) const
{
  IniFile.Write(Section, "FuncType", ftParFunc);
  IniFile.Write(Section, "x", xText);
  IniFile.Write(Section, "y", yText);

  TBaseFuncType::WriteToIni(IniFile, Section);
}
//---------------------------------------------------------------------------
void TParFunc::ReadFromIni(const TConfigFile &IniFile, const std::string &Section)
{
  try
  {
    xText = IniFile.Read(Section, "x", "");
    yText = IniFile.Read(Section, "y", "");
    Func.SetFunc(xText, yText, "t", GetData().CustomFunctions.SymbolList);
  }
  catch(Func32::EParseError &E)
  {
    Form1->ShowStatusError(GetErrorMsg(E));
  }
  TBaseFuncType::ReadFromIni(IniFile, Section);
}
//---------------------------------------------------------------------------
std::wstring TParFunc::MakeText() const
{
  return ToWString("x(t)=" + xText + " , y(t)=" + yText);
}
//---------------------------------------------------------------------------
boost::shared_ptr<TBaseFuncType> TParFunc::MakeDifFunc()
{
  boost::shared_ptr<TParFunc> DifFunc(new TParFunc);
  DifFunc->Func = Func.MakeDif();
  DifFunc->xText = DifFunc->Func.MakeXText();
  DifFunc->yText = DifFunc->Func.MakeYText();
  return DifFunc;
}
//---------------------------------------------------------------------------
//////////////
// TPolFunc //
//////////////
TPolFunc::TPolFunc(const std::string &AText, const Func32::TSymbolList &SymbolList, Func32::TTrigonometry Trig)
  : Text(AText), Func(AText, "t", SymbolList, Trig)
{
}
//---------------------------------------------------------------------------
void TPolFunc::WriteToIni(TConfigFile &IniFile, const std::string &Section) const
{
  IniFile.Write(Section, "FuncType", ftPolFunc);
  IniFile.Write(Section, "r", Text);

  TBaseFuncType::WriteToIni(IniFile, Section);
}
//---------------------------------------------------------------------------
void TPolFunc::ReadFromIni(const TConfigFile &IniFile, const std::string &Section)
{
  try
  {
    Text = IniFile.Read(Section, "r", "");
    Func.SetFunc(Text, "t", GetData().CustomFunctions.SymbolList);
  }
  catch(Func32::EParseError &E)
  {
    Form1->ShowStatusError(GetErrorMsg(E));
  }

  TBaseFuncType::ReadFromIni(IniFile, Section);
}
//---------------------------------------------------------------------------
std::wstring TPolFunc::MakeText() const
{
  return ToWString("r(t)=" + Text);
}
//---------------------------------------------------------------------------
boost::shared_ptr<TBaseFuncType> TPolFunc::MakeDifFunc()
{
  boost::shared_ptr<TPolFunc> DifFunc(new TPolFunc);
  DifFunc->Func = Func.MakeDif();
  DifFunc->Text = DifFunc->Func.MakeText();
  return DifFunc;
}
//---------------------------------------------------------------------------
//////////
// TTan //
//////////
TTan::TTan()
{
  DrawType = dtLines;
}
//---------------------------------------------------------------------------
void TTan::WriteToIni(TConfigFile &IniFile, const std::string &Section) const
{
  IniFile.Write(Section, "t", t.Text);
  IniFile.Write(Section, "From", From, TTextValue(-INF));
  IniFile.Write(Section, "To", To, TTextValue(+INF));
  IniFile.Write(Section, "Style", Style, psSolid);
  IniFile.Write(Section, "Color", Color);
  IniFile.Write(Section, "Size", Size, 1U);
  IniFile.Write(Section, "TangentType", TangentType, ttTangent);

  TBaseFuncType::WriteToIni(IniFile, Section);
}
//---------------------------------------------------------------------------
void TTan::ReadFromIni(const TConfigFile &IniFile, const std::string &Section)
{
  t.Text = IniFile.Read(Section, "t", "0");
  t.Value = GetData().Calc(t.Text);
  TangentType = IniFile.ReadEnum(Section, "TangentType", ttTangent);

  TBaseFuncType::ReadFromIni(IniFile, Section);
  DrawType = dtLines; //Overwrite the default from TBaseFuncType::ReadFromIni()
}
//---------------------------------------------------------------------------
std::wstring TTan::MakeText() const
{
  return ToWString(Func.lock()->GetVariable() + "=" + t.Text);
}
//---------------------------------------------------------------------------
std::wstring TTan::MakeLegendText() const
{
  if(!GetLegendText().empty())
    return GetLegendText();

  if(_isnan(a))
    return L"";
  if(!_finite(a))
    return ToWString("x=" + RoundToString(q, GetData()));
  return ToWString("y=" + RoundToString(a, GetData()) + "x" + (q < 0 ? '-' : '+') + RoundToString(std::abs(q), GetData()));
}
//---------------------------------------------------------------------------
const TTextValue& TTan::GetSteps() const
{
  return _finite(a) ? TTextValue(0, "") : TTextValue(2, "");
}
//---------------------------------------------------------------------------
void TTan::UpdateTan(double a1, double q1)
{
  a = a1;
  q = q1;
  if(_finite(a))
    TanFunc.SetFunc("t", (AnsiString(a) + "t+" + q).c_str());
  else if(_finite(q))
    TanFunc.SetFunc(AnsiString(q).c_str(), "t");
  else
    TanFunc.Clear();
  Points.clear();
}
//---------------------------------------------------------------------------
bool TTan::IsValid() const
{
  return !_isnan(a);
}
//---------------------------------------------------------------------------
std::pair<double,double> TTan::GetCurrentRange() const
{
  return std::make_pair(_finite(a) ? std::max(GetData().Axes.xAxis.Min, From.Value) : GetData().Axes.yAxis.Min,
    _finite(a) ? std::min(GetData().Axes.xAxis.Max, To.Value) : GetData().Axes.yAxis.Max);
}
//---------------------------------------------------------------------------
void TTan::Update()
{
  TBaseFuncType::Update();
  t.Update(GetData());
  CalcTan(); //Update a and q
}
//---------------------------------------------------------------------------
long double TTan::CalcArea(long double From, long double To) const
{
  if(_isnanl(a))
    return NAN; //The tangent is not valid, i.e. it is touching a undefined function
  if(_finitel(a))
    return (To - From) * a * 0.5 * (To + From);
  return 0;    //The tangent is vertical
}
//---------------------------------------------------------------------------
//Calculate data used to draw a tangent
//(x,y) is the point there the tangent crosses the function and a is the slope
bool TTan::CalcTan()
{
  try
  {
    const Func32::TBaseFunc &Func = ParentFunc()->GetFunc();

    double x = Func.CalcX(t.Value); //Find x(t)
    double y = Func.CalcY(t.Value); //Find y(t)

    double a = Func.CalcSlope(t.Value);

    if(TangentType == ttNormal)
      if(_finite(a))
        a = std::abs(a) < MIN_ZERO ? INF : -1/a;
      else
        a = 0;

    double q = _finite(a) ? y - a*x : x;
    UpdateTan(a, q);
    return true;
  }
  catch(Func32::EFuncError&)
  {
    UpdateTan(NAN, NAN);
    return false;
  }
}
//---------------------------------------------------------------------------
////////////
// TShade //
////////////
TShade::TShade(TShadeStyle AShadeStyle, TBrushStyle ABrushStyle, TColor AColor,
  const boost::shared_ptr<TBaseFuncType> &AFunc, const boost::shared_ptr<TBaseFuncType> &AFunc2,
  double AsMin, double AsMax, double AsMin2, double AsMax2, bool AExtendMinToIntercept, bool AExtendMaxToIntercept,
  bool AExtendMin2ToIntercept, bool AExtendMax2ToIntercept) : ShadeStyle(AShadeStyle), BrushStyle(ABrushStyle), Color(AColor),
  Func(AFunc), Func2(AFunc2), ExtendMinToIntercept(AExtendMinToIntercept), ExtendMaxToIntercept(AExtendMaxToIntercept),
    ExtendMin2ToIntercept(ExtendMin2ToIntercept), ExtendMax2ToIntercept(ExtendMax2ToIntercept)
{
  sMin.Value = AsMin;
  sMax.Value = AsMax;
  sMin2.Value = AsMin2;
  sMax2.Value = AsMax2;
}
//---------------------------------------------------------------------------
void TShade::WriteToIni(TConfigFile &IniFile, const std::string &Section) const
{
  TGraphElem::WriteToIni(IniFile, Section);

  IniFile.Write(Section, "ShadeStyle", ShadeStyle);
  IniFile.Write(Section, "BrushStyle", BrushStyle);
  IniFile.Write(Section, "Color", Color);

  unsigned FuncCount = 0;
  unsigned FuncNo = 0;
  unsigned Func2No = 0;
  for(unsigned I = 0; I < GetData().ElemCount(); I++)
  {
    if(dynamic_cast<TBaseFuncType*>(GetData().GetElem(I).get()))
      FuncCount++;
    if(GetData().GetElem(I) == Func.lock())
      FuncNo = FuncCount;
    if(GetData().GetElem(I) == Func2)
      Func2No = FuncCount;
  }

  BOOST_ASSERT(!Func2 || Func2No); //If Func2 is defined we must have a number for it

  IniFile.Write(Section, "FuncNo", FuncNo);
  IniFile.Write(Section, "Func2No", Func2No, 0U);
  IniFile.Write(Section, "sMin", sMin, TTextValue(-INF));
  IniFile.Write(Section, "sMax", sMax, TTextValue(+INF));
  IniFile.Write(Section, "sMin2", sMin2, TTextValue(-INF));
  IniFile.Write(Section, "sMax2", sMax2, TTextValue(+INF));
  IniFile.Write(Section, "ExtendMinToIntercept", ExtendMinToIntercept, false);
  IniFile.Write(Section, "ExtendMaxToIntercept", ExtendMaxToIntercept, false);
  IniFile.Write(Section, "ExtendMin2ToIntercept", ExtendMin2ToIntercept, false);
  IniFile.Write(Section, "ExtendMax2ToIntercept", ExtendMax2ToIntercept, false);
  IniFile.Write(Section, "MarkStart", MarkStart, true);
  IniFile.Write(Section, "MarkEnd", MarkEnd, true);
}
//---------------------------------------------------------------------------
void TShade::ReadFromIni(const TConfigFile &IniFile, const std::string &Section)
{
  TGraphElem::ReadFromIni(IniFile, Section);

  //For backward compatibility
  if(GetLegendText().empty())
    SetLegendText(LoadString(RES_SHADE));

  ShadeStyle = IniFile.ReadEnum(Section, "ShadeStyle", ssXAxis);
  BrushStyle = IniFile.ReadEnum(Section, "BrushStyle", bsBDiagonal);
  Color = IniFile.Read(Section, "Color", clGreen);

  Func = GetData().GetFuncFromIndex(IniFile.Read(Section, "FuncNo", 0) - 1);

  int Func2No = IniFile.Read(Section, "Func2No", 0) - 1;
  Func2.reset();
  if(Func2No != -1)
    Func2 = GetData().GetFuncFromIndex(Func2No);

  sMin.Set(IniFile.Read(Section, "sMin", "-INF"), GetData());
  sMax.Set(IniFile.Read(Section, "sMax", "+INF"), GetData());
  sMin2.Set(IniFile.Read(Section, "sMin2", "-INF"), GetData());
  sMax2.Set(IniFile.Read(Section, "sMax2", "+INF"), GetData());
  ExtendMinToIntercept = IniFile.Read(Section, "ExtendMinToIntercept", false);
  ExtendMaxToIntercept = IniFile.Read(Section, "ExtendMaxToIntercept", false);
  ExtendMin2ToIntercept = IniFile.Read(Section, "ExtendMin2ToIntercept", false);
  ExtendMax2ToIntercept = IniFile.Read(Section, "ExtendMax2ToIntercept", false);
  MarkStart = IniFile.Read(Section, "MarkStart", true);
  MarkEnd = IniFile.Read(Section, "MarkEnd", true);
}
//---------------------------------------------------------------------------
std::wstring TShade::MakeText() const
{
  return GetLegendText();
}
//---------------------------------------------------------------------------
void TShade::Update()
{
  sMin.Update(GetData());
  sMax.Update(GetData());
  sMin2.Update(GetData());
  sMax2.Update(GetData());
}
//////////////////
// TPointSeries //
//////////////////
TPointSeriesPoint::TPointSeriesPoint(const TData &Data, const std::string &X, const std::string &Y, const std::string &XError, const std::string &YError, bool IgnoreErrors)
  : xError(XError.empty() ? 0 : Data.Calc(XError), XError), yError(YError.empty() ? 0 : Data.Calc(YError), YError)
{
  x.Text = X;
  y.Text = Y;

  // Optimize for the case of numbers. If the text is not a valid number, fall back to calling Calc() to parse an expression
  try
  {
    //Make sure there is no 'e' (Euler's constant) as it will be interpretted as 'E'
    if(X.find("e") == std::string::npos && Y.find("e") == std::string::npos)
    {
      x.Value = boost::lexical_cast<double>(X);
      y.Value = boost::lexical_cast<double>(Y);
      return;
    }
  }
  catch(boost::bad_lexical_cast &E)
  {
  }

  //Allow calculation errors but not parse errors
  try
  {
    x.Value = Data.Calc(X);
    y.Value = Data.Calc(Y);
  }
  catch(Func32::ECalcError &E)
  {
    x.Value = NAN;
    y.Value = NAN;
  }
  catch(...)
  {
    if(!IgnoreErrors)
      throw;
    x.Value = NAN;
    y.Value = NAN;
  }
}
//---------------------------------------------------------------------------
TPointSeries::TPointSeries()
  : xErrorValue(0), yErrorValue(0), xErrorBarType(ebtNone), yErrorBarType(ebtNone)
{
}
//---------------------------------------------------------------------------
void TPointSeries::WriteToIni(TConfigFile &IniFile, const std::string &Section) const
{
  IniFile.Write(Section, "FrameColor", FrameColor, clBlack);
  IniFile.Write(Section, "FillColor", FillColor);
  IniFile.Write(Section, "LineColor", LineColor);
  IniFile.Write(Section, "Size", Size);
  IniFile.Write(Section, "Style", Style);
  IniFile.Write(Section, "LineSize", LineSize, 1U);
  IniFile.Write(Section, "LineStyle", LineStyle, psClear);
  IniFile.Write(Section, "Interpolation", Interpolation, iaLinear);
  IniFile.Write(Section, "ShowLabels", ShowLabels, false);
  IniFile.Write(Section, "Font", FontToStr(Font), std::string(DEFAULT_POINT_FONT));
  IniFile.Write(Section, "LabelPosition", LabelPosition);

  std::ostringstream Str;
  for(unsigned N = 0; N < PointList.size(); N++)
  {
    if(PointList[N].x.Text.find(',') == std::string::npos)
      Str << PointList[N].x.Text << ',';
    else
      Str << '"' << PointList[N].x.Text << "\",";

    if(PointList[N].y.Text.find(',') == std::string::npos)
      Str << PointList[N].y.Text << ';';
    else
      Str << '"' << PointList[N].y.Text << "\";";
  }
  IniFile.Write(Section, "Points", Str.str());

  IniFile.Write(Section, "xErrorBarType", xErrorBarType, ebtNone);
  IniFile.Write(Section, "xErrorBarValue", xErrorValue, 0.0);
  if(xErrorBarType == ebtCustom)
  {
    Str.str("");
    for(unsigned N = 0; N < PointList.size(); N++)
      Str << PointList[N].xError << ';';
    IniFile.Write(Section, "xErrorBarData", Str.str(), std::string());
  }

  IniFile.Write(Section, "yErrorBarType", yErrorBarType, ebtNone);
  IniFile.Write(Section, "yErrorBarValue", yErrorValue, 0.0);
  if(yErrorBarType == ebtCustom)
  {
    Str.str("");
    for(unsigned N = 0; N < PointList.size(); N++)
        Str << PointList[N].yError << ';';
    IniFile.Write(Section, "yErrorBarData", Str.str(), std::string());
  }

  TGraphElem::WriteToIni(IniFile, Section);
}
//---------------------------------------------------------------------------
void TPointSeries::ReadFromIni(const TConfigFile &IniFile, const std::string &Section)
{
  TGraphElem::ReadFromIni(IniFile, Section);
  //For backward compatibility
  if(GetLegendText().empty())
    SetLegendText(ToWString(IniFile.Read(Section, "Text", Section)));

  FrameColor = IniFile.Read(Section, "FrameColor", clBlack);
  FillColor = IniFile.Read(Section, "FillColor", clRed);
  LineColor = IniFile.Read(Section, "LineColor", clBlue);
  Size = IniFile.Read(Section, "Size", 5);
  Style = IniFile.Read(Section, "Style", 0);
  LineSize = IniFile.Read(Section, "LineSize", 1);
  LineStyle = IniFile.ReadEnum(Section, "LineStyle", psClear);
  if(IniFile.KeyExists(Section, "SmoothLine"))
    Interpolation = IniFile.ReadEnum(Section, "SmoothLine", iaLinear);
  else
    Interpolation = IniFile.ReadEnum(Section, "Interpolation", iaLinear);
  ShowLabels = IniFile.Read(Section, "ShowLabels", false);
  StrToFont(IniFile.Read(Section, "Font", DEFAULT_POINT_FONT), Font);
  LabelPosition = IniFile.ReadEnum(Section, "LabelPosition", lpBelow);

  TTokenizer Tokens(IniFile.Read(Section, "Points", ""), ';');
  std::istringstream xStream(IniFile.Read(Section, "xErrorBarData", ""));
  std::istringstream yStream(IniFile.Read(Section, "yErrorBarData", ""));
  TTokenizer Token("", ',', '"');

  std::string SavedByVersion = IniFile.Read("Graph", "Version", "NA");
  bool BackwardCompatibility = SavedByVersion < TVersion("4.0"); //Before 4.0 the number 3,000,000 would be saved as "3e6"

  while(Token = Tokens.Next(), Tokens)
  {
    std::string x, y, xError, yError;
    Token >> x >> y;

    getline(xStream, xError, ';');
    getline(yStream, yError, ';');

    try
    {
      using boost::lexical_cast;
      if(BackwardCompatibility)
        PointList.push_back(TPointSeriesPoint(lexical_cast<double>(x), lexical_cast<double>(y), xError.empty() ? 0.0 : lexical_cast<double>(yError), xError.empty() ? 0.0 : lexical_cast<double>(yError)));
      else
        PointList.push_back(TPointSeriesPoint(GetData(), x, y, xError, yError));
    }
    catch(Func32::EParseError &E)
    {
      Form1->ShowStatusError(GetErrorMsg(E));
      PointList.push_back(TPointSeriesPoint(GetData(), x, y, xError, yError, true));
    }
  }

  xErrorBarType = IniFile.ReadEnum(Section, "xErrorBarType", ebtNone);
  xErrorValue = IniFile.Read(Section, "xErrorBarValue", 0.0);

  yErrorBarType = IniFile.ReadEnum(Section, "yErrorBarType", ebtNone);
  yErrorValue = IniFile.Read(Section, "yErrorBarValue", 0.0);
}
//---------------------------------------------------------------------------
double TPointSeries::GetXError(unsigned Index) const
{
  switch(xErrorBarType)
  {
    case ebtFixed:
      return xErrorValue;

    case ebtRelative:
      return xErrorValue * PointList[Index].x.Value / 100;

    case ebtCustom:
      return PointList[Index].xError.Value;
  }
  return 0;
}
//---------------------------------------------------------------------------
double TPointSeries::GetYError(unsigned Index) const
{
  switch(yErrorBarType)
  {
    case ebtFixed:
      return yErrorValue;

    case ebtRelative:
      return yErrorValue * PointList[Index].y.Value / 100;

    case ebtCustom:
      return PointList[Index].yError.Value;
  }
  return 0;
}
//---------------------------------------------------------------------------
Func32::TDblPoint TPointSeries::FindCoord(double x) const
{
  if(Interpolation == iaLinear)
    for(unsigned I = 0; I < PointList.size() - 1; I++)
      if((PointList[I].x.Value < x && PointList[I+1].x.Value > x) ||
         (PointList[I].x.Value > x && PointList[I+1].x.Value < x))
      {
        double a = (PointList[I+1].y.Value - PointList[I].y.Value) / (PointList[I+1].x.Value - PointList[I].x.Value);
        double b = PointList[I].y.Value - a * PointList[I].x.Value;
        return Func32::TDblPoint(x, a*x+b);
      }

  throw EAbort("");
}
//---------------------------------------------------------------------------
void TPointSeries::Update()
{
  for(unsigned I = 0; I < PointList.size(); I++)
  {
    try
    {
      PointList[I].x.Value = GetData().Calc(PointList[I].x.Text);
      PointList[I].y.Value = GetData().Calc(PointList[I].y.Text);
      if(!PointList[I].xError.Text.empty())
        PointList[I].xError.Value = GetData().Calc(PointList[I].xError.Text);
      if(!PointList[I].yError.Text.empty())
        PointList[I].yError.Value = GetData().Calc(PointList[I].yError.Text);
    }
    catch(Func32::EFuncError &E)
    {
      PointList[I].x.Value = NAN;
      PointList[I].y.Value = NAN;
      PointList[I].xError.Value = NAN;
      PointList[I].yError.Value = NAN;
    }
  }
}
//---------------------------------------------------------------------------
////////////////
// TTextLabel //
////////////////
TTextLabel::TTextLabel(const std::string &Str, TLabelPlacement Placement, const TTextValue &AxPos, const TTextValue &AyPos, TColor Color, unsigned ARotation)
  : Text(Str), LabelPlacement(Placement), xPos(AxPos), yPos(AyPos),
    BackgroundColor(Color), Rotation(ARotation)
{
  SetShowInLegend(false);
  StatusText = ToWString(RtfToPlainText(Str));
  //Update() must be called after Label is added to Data
}
//---------------------------------------------------------------------------
void TTextLabel::WriteToIni(TConfigFile &IniFile, const std::string &Section) const
{
  IniFile.Write(Section, "Placement", LabelPlacement);
  if(LabelPlacement == lpUserTopLeft || LabelPlacement >= lpUserTopRight)
    IniFile.Write(Section, "Pos", xPos.Text + ";" + yPos.Text);
  IniFile.Write(Section, "Rotation", Rotation, 0U);
  IniFile.Write(Section, "Text", EncodeEscapeSequence(Text));
  IniFile.Write(Section, "BackgroundColor", BackgroundColor);

  TGraphElem::WriteToIni(IniFile, Section);
}
//---------------------------------------------------------------------------
void TTextLabel::ReadFromIni(const TConfigFile &IniFile, const std::string &Section)
{
  LabelPlacement = IniFile.ReadEnum(Section, "Placement", lpUserTopLeft);
  std::string Temp = IniFile.Read(Section, "Pos", "0;0");
  unsigned n = Temp.find(";");
  if(n == std::string::npos)
  {
    //For backwards compatibility
    Func32::TDblPoint Pos = IniFile.Read(Section, "Pos", Func32::TDblPoint(0,0));
    xPos = TTextValue(Pos.x);
    yPos = TTextValue(Pos.y);
  }
  else
  {
    xPos.Set(Temp.substr(0, n), GetData(), true);
    yPos.Set(Temp.substr(n+1), GetData(), true);
  }

  Rotation = IniFile.Read(Section, "Rotation", 0U);
  Text = DecodeEscapeSequence(IniFile.Read(Section, "Text", "ERROR"));
  BackgroundColor = IniFile.Read(Section, "BackgroundColor", clNone);

  Update();
  StatusText = ToWString(RtfToPlainText(Text));

  TGraphElem::ReadFromIni(IniFile, Section);
  SetShowInLegend(false); //Overwrite data; Label is never shown in legend
}
//---------------------------------------------------------------------------
std::wstring TTextLabel::MakeText() const
{
  return StatusText.empty() ? LoadString(RES_LABEL) : StatusText;
}
//---------------------------------------------------------------------------
void TTextLabel::Scale(double xSizeMul, double ySizeMul)
{
  Rect.Right = Rect.Left + Rect.Width() * xSizeMul;
  Rect.Bottom = Rect.Top + Rect.Height() * ySizeMul;
  Metafile->MMWidth = Metafile->MMWidth * xSizeMul;
  Metafile->MMHeight = Metafile->MMHeight * ySizeMul;
}
//---------------------------------------------------------------------------
void TTextLabel::Update()
{
  TPoint Size = RichTextSize(Text, &GetData());
  Metafile->Width = Size.x;
  Metafile->Height = Size.y;
  std::auto_ptr<TMetafileCanvas> Canvas(new TMetafileCanvas(Metafile, 0));

  RenderRichText(Text.c_str(), Canvas.get(), TPoint(0, 0), Size.x, BackgroundColor, &GetData());
  Canvas.reset();
  Rect = Rotate(Metafile, Rotation);

  xPos.Update(GetData());
  yPos.Update(GetData());
}
//---------------------------------------------------------------------------
///////////////
// TRelation //
///////////////
TRelation::TRelation()
  : Color(clGreen), BrushStyle(bsBDiagonal), Size(1)
{
}
//---------------------------------------------------------------------------
TRelation::TRelation(const std::string &AText, const Func32::TSymbolList &SymbolList, TColor AColor, TBrushStyle Style, unsigned ASize, Func32::TTrigonometry Trig)
  : Text(AText), Color(AColor), BrushStyle(Style), Size(ASize)
{
  std::vector<std::string> Args;
  Args.push_back("x");
  Args.push_back("y");
  Func.SetTrigonometry(Trig);
  Constraints.SetTrigonometry(Trig);
  Func.SetFunc(Text, Args, SymbolList);
  if(Func.GetFunctionType() != Func32::ftInequality && Func.GetFunctionType() != Func32::ftEquation)
    throw EGraphError(geInvalidRelation);

  RelationType = Func.GetFunctionType() == Func32::ftInequality ? rtInequality : rtEquation;
  if(RelationType == rtEquation && Size == 0)
    Size = 1;

  if(Func.GetFunctionType() == Func32::ftEquation)
    Func.RemoveRelation();
}
//---------------------------------------------------------------------------
TRelation::TRelation(const TRelation &Relation)
  : TGraphElem(Relation), Text(Relation.Text), ConstraintsText(Relation.ConstraintsText),
    Func(Relation.Func), Constraints(Relation.Constraints), RelationType(Relation.RelationType),
    Color(Relation.Color), BrushStyle(Relation.BrushStyle), Size(Relation.Size)
{
}
//---------------------------------------------------------------------------
void TRelation::WriteToIni(TConfigFile &IniFile, const std::string &Section) const
{
  IniFile.Write(Section, "Relation", Text);
  IniFile.Write(Section, "Constraints", ConstraintsText, std::string());
  IniFile.Write(Section, "Style", BrushStyle);
  IniFile.Write(Section, "Color", Color);
  IniFile.Write(Section, "Size", Size, 1U);

  TGraphElem::WriteToIni(IniFile, Section);
}
//---------------------------------------------------------------------------
void TRelation::ReadFromIni(const TConfigFile &IniFile, const std::string &Section)
{
  Func.SetTrigonometry(GetData().Axes.Trigonometry);
  Constraints.SetTrigonometry(GetData().Axes.Trigonometry);
  Text = IniFile.Read(Section, "Relation", "");
  ConstraintsText = IniFile.Read(Section, "Constraints", "");
  BrushStyle = IniFile.ReadEnum(Section, "Style", bsBDiagonal);
  Color = IniFile.Read(Section, "Color", clGreen);
  Size = IniFile.Read(Section, "Size", 1);
  std::vector<std::string> Args;
  Args.push_back("x");
  Args.push_back("y");
  Func.SetFunc(Text, Args, GetData().CustomFunctions.SymbolList);
  if(!ConstraintsText.empty())
    Constraints.SetFunc(ConstraintsText, Args, GetData().CustomFunctions.SymbolList);
  RelationType = Func.GetFunctionType() == Func32::ftInequality ? rtInequality : rtEquation;
  if(Func.GetFunctionType() == Func32::ftEquation)
    Func.RemoveRelation();

  TGraphElem::ReadFromIni(IniFile, Section);
}
//---------------------------------------------------------------------------
std::wstring TRelation::MakeText() const
{
  return ConstraintsText.empty() ? ToWString(Text) : ToWString(Text + "; " + ConstraintsText);
}
//---------------------------------------------------------------------------
void TRelation::SetConstraints(const std::string &AConstraintsText, const Func32::TSymbolList &SymbolList)
{
  std::vector<std::string> Args;
  Args.push_back("x");
  Args.push_back("y");
  Constraints.SetFunc(AConstraintsText, Args, SymbolList);
  ConstraintsText = AConstraintsText;
}
//---------------------------------------------------------------------------
long double TRelation::Eval(const std::vector<long double> &Args, Func32::ECalcError &E)
{
  if(!Constraints.IsEmpty())
  {
    long double Valid = Constraints.Calc(Args, E);
    if(E.ErrorCode != Func32::ecNoError || !Valid)
      return NAN;
  }

  long double Result = Func.Calc(Args, E);
  if(E.ErrorCode != Func32::ecNoError)
    return NAN;
  return Result;
}
//---------------------------------------------------------------------------
void TRelation::ClearCache()
{
  Region.reset();
  BoundingRegion.reset();
}
//---------------------------------------------------------------------------
void TRelation::Update()
{
  Func.SetTrigonometry(GetData().Axes.Trigonometry);
  Constraints.SetTrigonometry(GetData().Axes.Trigonometry);
  Func.Update(GetData().CustomFunctions.SymbolList);
  Constraints.Update(GetData().CustomFunctions.SymbolList);
}
//---------------------------------------------------------------------------
///////////////
// TAxesView //
///////////////
void TAxesView::WriteToIni(TConfigFile &IniFile, const std::string &Section) const
{
  GetData().Axes.WriteToIni(IniFile);
}
//---------------------------------------------------------------------------
int TAxesView::GetVisible() const
{
  switch(GetData().Axes.AxesStyle)
  {
    case asCrossed:
      return 1;
    case asBoxed:
      return -1;
    default:
      return 0;
  }
}
//---------------------------------------------------------------------------
void TAxesView::ChangeVisible()
{
  TData &Data = const_cast<TData&>(GetData());
  switch(Data.Axes.AxesStyle)
  {
    case asCrossed:
      Data.Axes.AxesStyle = asBoxed;
      Data.ClearCache();
      break;

    case asBoxed:
      Data.Axes.AxesStyle = ::asNone;
      Data.ClearCache();
      break;

    default:
      Data.Axes.AxesStyle = asCrossed;
      break;
  }
}
//---------------------------------------------------------------------------

