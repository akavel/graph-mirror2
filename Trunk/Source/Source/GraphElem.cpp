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
#include "GuiUtil.h"
#include <boost/lexical_cast.hpp>
#include <boost/mem_fn.hpp>
//---------------------------------------------------------------------------
std::wistream& operator >>(std::wistream &Stream, const Func32::TDblPoint&)
{
  return Stream;
}
////////////////
// TTextValue //
////////////////
TTextValue::TTextValue(double AValue) : Value(AValue)
{
  if(_finite(Value))
    Text = ToWString(AValue);
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
void TTextValue::Set(const std::wstring AText, const TData &Data, bool IgnoreErrors)
{
  Text = AText;
  if(Text == L"INF" || Text == L"+INF")
  {
    Text = L"";
    Value = INF;
  }
  else if(Text == L"-INF")
  {
    Text = L"";
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
      Text = L"";
      Value = NAN;
    }
}
//---------------------------------------------------------------------------
std::wostream& operator<<(std::wostream &Stream, const TTextValue &TextValue)
{
  if(TextValue.Value == INF)
    return Stream << L"+INF";
  else if(TextValue.Value == -INF)
    return Stream << L"-INF";
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
void TGraphElem::WriteToIni(TConfigFileSection &Section) const
{
  Section.Write(L"Visible", Visible, true);
  Section.Write(L"ShowInLegend", ShowInLegend, true);
  Section.Write(L"LegendText", LegendText, std::wstring());
}
//---------------------------------------------------------------------------
void TGraphElem::ReadFromIni(const TConfigFileSection &Section)
{
  Visible = Section.Read(L"Visible", true);
  ShowInLegend = Section.Read(L"ShowInLegend", true);
  LegendText = Section.Read(L"LegendText", L"");
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
void TBaseFuncType::WriteToIni(TConfigFileSection &Section) const
{
  TGraphElem::WriteToIni(Section);

  Section.Write(L"From", From.Text, std::wstring());
  Section.Write(L"To", To.Text, std::wstring());
  Section.Write(L"Steps", Steps.Text, std::wstring());
  Section.Write(L"Style", Style, psSolid);
  Section.Write(L"Color", Color);
  Section.Write(L"Size", Size, 1U);
  Section.Write(L"StartPoint", StartPointStyle, 0U);
  Section.Write(L"EndPoint", EndPointStyle, 0U);
  Section.Write(L"DrawType", DrawType, dtAuto);
}
//---------------------------------------------------------------------------
void TBaseFuncType::ReadFromIni(const TConfigFileSection &Section)
{
  GetFunc().SetTrigonometry(GetData().Axes.Trigonometry);
  Style = Section.Read(L"Style", psSolid);
  Color = Section.Read(L"Color", clRed);
  Size = Section.Read(L"Size", 1);
  if(Section.KeyExists(L">From"))
    From.Set(Section.Read(L">From", L"-INF"), GetData()); //Caused by bug in MS Outlook Express
  else
    From.Set(Section.Read(L"From", L"-INF"), GetData());
  To.Set(Section.Read(L"To", L"+INF"), GetData());
  Steps.Set(Section.Read(L"Steps", L""), GetData(), true);
  StartPointStyle = Section.Read(L"StartPoint", 0);
  EndPointStyle = Section.Read(L"EndPoint", 0);
  DrawType = Section.Read(L"DrawType", dtAuto);

  TGraphElem::ReadFromIni(Section);
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
TStdFunc::TStdFunc(const std::wstring &AText, const Func32::TSymbolList &SymbolList, Func32::TTrigonometry Trig)
  : Text(AText), Func(ToString(AText), "x", SymbolList, Trig)
{
}
//---------------------------------------------------------------------------
TStdFunc::TStdFunc(const Func32::TFunc &AFunc)
  : Text(ToWString(AFunc.MakeText())), Func(AFunc)
{
}
//---------------------------------------------------------------------------
void TStdFunc::WriteToIni(TConfigFileSection &Section) const
{
  Section.Write(L"FuncType", ftStdFunc);
  Section.Write(L"y", Text);

  TBaseFuncType::WriteToIni(Section);
}
//---------------------------------------------------------------------------
void TStdFunc::ReadFromIni(const TConfigFileSection &Section)
{
  TBaseFuncType::ReadFromIni(Section);
  Text = Section.Read(L"y", L"");

  try
  {
    Func.SetFunc(ToString(Text), "x", GetData().CustomFunctions.SymbolList);
  }
  catch(Func32::EParseError &E)
  {
    ShowStatusError(GetErrorMsg(E));
  }
}
//---------------------------------------------------------------------------
std::wstring TStdFunc::MakeText() const
{
  return L"f(x)=" + Text;
}
//---------------------------------------------------------------------------
boost::shared_ptr<TBaseFuncType> TStdFunc::MakeDifFunc()
{
  boost::shared_ptr<TStdFunc> DifFunc(new TStdFunc);
  DifFunc->Func = Func.MakeDif();
  DifFunc->Text = ToWString(DifFunc->Func.MakeText());
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
TParFunc::TParFunc(const std::wstring &AxText, const std::wstring &AyText, const Func32::TSymbolList &SymbolList, Func32::TTrigonometry Trig)
  : xText(AxText), yText(AyText), Func(ToString(AxText), ToString(AyText), "t", SymbolList, Trig)
{
}
//---------------------------------------------------------------------------
TParFunc::TParFunc(const Func32::TParamFunc &AFunc)
  : xText(ToWString(AFunc.MakeXText())), yText(ToWString(AFunc.MakeYText())), Func(AFunc)
{
}
//---------------------------------------------------------------------------
void TParFunc::WriteToIni(TConfigFileSection &Section) const
{
  Section.Write(L"FuncType", ftParFunc);
  Section.Write(L"x", xText);
  Section.Write(L"y", yText);

  TBaseFuncType::WriteToIni(Section);
}
//---------------------------------------------------------------------------
void TParFunc::ReadFromIni(const TConfigFileSection &Section)
{
  try
  {
    xText = Section.Read(L"x", L"");
    yText = Section.Read(L"y", L"");
    Func.SetFunc(ToString(xText), ToString(yText), "t", GetData().CustomFunctions.SymbolList);
  }
  catch(Func32::EParseError &E)
  {
    ShowStatusError(GetErrorMsg(E));
  }
  TBaseFuncType::ReadFromIni(Section);
}
//---------------------------------------------------------------------------
std::wstring TParFunc::MakeText() const
{
  return L"x(t)=" + xText + L" , y(t)=" + yText;
}
//---------------------------------------------------------------------------
boost::shared_ptr<TBaseFuncType> TParFunc::MakeDifFunc()
{
  boost::shared_ptr<TParFunc> DifFunc(new TParFunc);
  DifFunc->Func = Func.MakeDif();
  DifFunc->xText = ToWString(DifFunc->Func.MakeXText());
  DifFunc->yText = ToWString(DifFunc->Func.MakeYText());
  return DifFunc;
}
//---------------------------------------------------------------------------
//////////////
// TPolFunc //
//////////////
TPolFunc::TPolFunc(const std::wstring &AText, const Func32::TSymbolList &SymbolList, Func32::TTrigonometry Trig)
  : Text(AText), Func(ToString(AText), "t", SymbolList, Trig)
{
}
//---------------------------------------------------------------------------
void TPolFunc::WriteToIni(TConfigFileSection &Section) const
{
  Section.Write(L"FuncType", ftPolFunc);
  Section.Write(L"r", Text);

  TBaseFuncType::WriteToIni(Section);
}
//---------------------------------------------------------------------------
void TPolFunc::ReadFromIni(const TConfigFileSection &Section)
{
  try
  {
    Text = Section.Read(L"r", L"");
    Func.SetFunc(ToString(Text), "t", GetData().CustomFunctions.SymbolList);
  }
  catch(Func32::EParseError &E)
  {
    ShowStatusError(GetErrorMsg(E));
  }

  TBaseFuncType::ReadFromIni(Section);
}
//---------------------------------------------------------------------------
std::wstring TPolFunc::MakeText() const
{
  return L"r(t)=" + Text;
}
//---------------------------------------------------------------------------
boost::shared_ptr<TBaseFuncType> TPolFunc::MakeDifFunc()
{
  boost::shared_ptr<TPolFunc> DifFunc(new TPolFunc);
  DifFunc->Func = Func.MakeDif();
  DifFunc->Text = ToWString(DifFunc->Func.MakeText());
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
void TTan::WriteToIni(TConfigFileSection &Section) const
{
  Section.Write(L"t", t.Text);
  Section.Write(L"From", From, TTextValue(-INF));
  Section.Write(L"To", To, TTextValue(+INF));
  Section.Write(L"Style", Style, psSolid);
  Section.Write(L"Color", Color);
  Section.Write(L"Size", Size, 1U);
  Section.Write(L"TangentType", TangentType, ttTangent);

  TBaseFuncType::WriteToIni(Section);
}
//---------------------------------------------------------------------------
void TTan::ReadFromIni(const TConfigFileSection &Section)
{
  t.Text = Section.Read(L"t", L"0");
  t.Value = GetData().Calc(t.Text);
  TangentType = Section.Read(L"TangentType", ttTangent);

  TBaseFuncType::ReadFromIni(Section);
  DrawType = dtLines; //Overwrite the default from TBaseFuncType::ReadFromIni()
}
//---------------------------------------------------------------------------
std::wstring TTan::MakeText() const
{
  return ToWString(Func.lock()->GetVariable()) + L"=" + t.Text;
}
//---------------------------------------------------------------------------
std::wstring TTan::MakeLegendText() const
{
  if(!GetLegendText().empty())
    return GetLegendText();

  if(_isnan(a))
    return L"";
  if(!_finite(a))
    return L"x=" + RoundToString(q, GetData());
  return L"y=" + RoundToString(a, GetData()) + L"x" + (q < 0 ? L'-' : L'+') + RoundToString(std::abs(q), GetData());
}
//---------------------------------------------------------------------------
const TTextValue& TTan::GetSteps() const
{
  return _finite(a) ? TTextValue(0, L"") : TTextValue(2, L"");
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
void TShade::WriteToIni(TConfigFileSection &Section) const
{
  TGraphElem::WriteToIni(Section);

  Section.Write(L"ShadeStyle", ShadeStyle);
  Section.Write(L"BrushStyle", BrushStyle);
  Section.Write(L"Color", Color);

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

  Section.Write(L"FuncNo", FuncNo);
  Section.Write(L"Func2No", Func2No, 0U);
  Section.Write(L"sMin", sMin, TTextValue(-INF));
  Section.Write(L"sMax", sMax, TTextValue(+INF));
  Section.Write(L"sMin2", sMin2, TTextValue(-INF));
  Section.Write(L"sMax2", sMax2, TTextValue(+INF));
  Section.Write(L"ExtendMinToIntercept", ExtendMinToIntercept, false);
  Section.Write(L"ExtendMaxToIntercept", ExtendMaxToIntercept, false);
  Section.Write(L"ExtendMin2ToIntercept", ExtendMin2ToIntercept, false);
  Section.Write(L"ExtendMax2ToIntercept", ExtendMax2ToIntercept, false);
  Section.Write(L"MarkStart", MarkStart, true);
  Section.Write(L"MarkEnd", MarkEnd, true);
}
//---------------------------------------------------------------------------
void TShade::ReadFromIni(const TConfigFileSection &Section)
{
  TGraphElem::ReadFromIni(Section);

  //For backward compatibility
  if(GetLegendText().empty())
    SetLegendText(LoadString(RES_SHADE));

  ShadeStyle = Section.Read(L"ShadeStyle", ssXAxis);
  BrushStyle = Section.Read(L"BrushStyle", bsBDiagonal);
  Color = Section.Read(L"Color", clGreen);

  Func = GetData().GetFuncFromIndex(Section.Read(L"FuncNo", 0) - 1);

  int Func2No = Section.Read(L"Func2No", 0) - 1;
  Func2.reset();
  if(Func2No != -1)
    Func2 = GetData().GetFuncFromIndex(Func2No);

  sMin.Set(Section.Read(L"sMin", L"-INF"), GetData(), true);
  sMax.Set(Section.Read(L"sMax", L"+INF"), GetData(), true);
  sMin2.Set(Section.Read(L"sMin2", L"-INF"), GetData(), true);
  sMax2.Set(Section.Read(L"sMax2", L"+INF"), GetData(), true);
  ExtendMinToIntercept = Section.Read(L"ExtendMinToIntercept", false);
  ExtendMaxToIntercept = Section.Read(L"ExtendMaxToIntercept", false);
  ExtendMin2ToIntercept = Section.Read(L"ExtendMin2ToIntercept", false);
  ExtendMax2ToIntercept = Section.Read(L"ExtendMax2ToIntercept", false);
  MarkStart = Section.Read(L"MarkStart", true);
  MarkEnd = Section.Read(L"MarkEnd", true);

  //For backweards compatibility
  if(_isnan(sMin.Value)) sMin.Set(L"-INF", GetData());
  if(_isnan(sMax.Value)) sMax.Set(L"+INF", GetData());
  if(_isnan(sMin2.Value)) sMin2.Set(L"-INF", GetData());
  if(_isnan(sMax2.Value)) sMax2.Set(L"+INF", GetData());

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
TPointSeriesPoint::TPointSeriesPoint(const TData &Data, const std::wstring &X, const std::wstring &Y, const std::wstring &XError, const std::wstring &YError, bool IgnoreErrors)
  : xError(XError.empty() ? 0 : Data.Calc(XError), XError), yError(YError.empty() ? 0 : Data.Calc(YError), YError)
{
  x.Text = X;
  y.Text = Y;

  // Optimize for the case of numbers. If the text is not a valid number, fall back to calling Calc() to parse an expression
  try
  {
    //Make sure there is no 'e' (Euler's constant) as it will be interpretted as 'E'
    if(X.find(L"e") == std::wstring::npos && Y.find(L"e") == std::wstring::npos)
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
void TPointSeries::WriteToIni(TConfigFileSection &Section) const
{
  Section.Write(L"FrameColor", FrameColor, clBlack);
  Section.Write(L"FillColor", FillColor);
  Section.Write(L"LineColor", LineColor);
  Section.Write(L"Size", Size);
  Section.Write(L"Style", Style);
  Section.Write(L"LineSize", LineSize, 1U);
  Section.Write(L"LineStyle", LineStyle, psClear);
  Section.Write(L"Interpolation", Interpolation, iaLinear);
  Section.Write(L"ShowLabels", ShowLabels, false);
  Section.Write(L"Font", FontToStr(Font), DEFAULT_POINT_FONT);
  Section.Write(L"LabelPosition", LabelPosition);

  std::wostringstream Str;
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
  Section.Write(L"Points", Str.str());

  Section.Write(L"xErrorBarType", xErrorBarType, ebtNone);
  Section.Write(L"xErrorBarValue", xErrorValue, 0.0);
  if(xErrorBarType == ebtCustom)
  {
    Str.str(L"");
    for(unsigned N = 0; N < PointList.size(); N++)
      Str << PointList[N].xError << ';';
    Section.Write(L"xErrorBarData", Str.str(), std::wstring());
  }

  Section.Write(L"yErrorBarType", yErrorBarType, ebtNone);
  Section.Write(L"yErrorBarValue", yErrorValue, 0.0);
  if(yErrorBarType == ebtCustom)
  {
    Str.str(L"");
    for(unsigned N = 0; N < PointList.size(); N++)
        Str << PointList[N].yError << ';';
    Section.Write(L"yErrorBarData", Str.str(), std::wstring());
  }

  TGraphElem::WriteToIni(Section);
}
//---------------------------------------------------------------------------
void TPointSeries::ReadFromIni(const TConfigFileSection &Section)
{
  TGraphElem::ReadFromIni(Section);
  //For backward compatibility
  if(GetLegendText().empty())
    SetLegendText(Section.Read(L"Text", Section.GetName()));

  FrameColor = Section.Read(L"FrameColor", clBlack);
  FillColor = Section.Read(L"FillColor", clRed);
  LineColor = Section.Read(L"LineColor", clBlue);
  Size = Section.Read(L"Size", 5);
  Style = Section.Read(L"Style", 0);
  LineSize = Section.Read(L"LineSize", 1);
  LineStyle = Section.Read(L"LineStyle", psClear);
  if(Section.KeyExists(L"SmoothLine"))
    Interpolation = Section.Read(L"SmoothLine", iaLinear);
  else
    Interpolation = Section.Read(L"Interpolation", iaLinear);
  ShowLabels = Section.Read(L"ShowLabels", false);
  StrToFont(Section.Read(L"Font", DEFAULT_POINT_FONT), Font);
  LabelPosition = Section.Read(L"LabelPosition", lpBelow);

  TTokenizer Tokens(Section.Read(L"Points", L""), L';');
  std::wistringstream xStream(Section.Read(L"xErrorBarData", L""));
  std::wistringstream yStream(Section.Read(L"yErrorBarData", L""));
  TTokenizer Token(L"", L',', L'"');

//  std::string SavedByVersion = Section.Read("Graph", "Version", "NA");
//  bool BackwardCompatibility = SavedByVersion < TVersion("4.0"); //Before 4.0 the number 3,000,000 would be saved as "3e6"

  while(Token = Tokens.Next(), Tokens)
  {
    std::wstring x, y, xError, yError;
    Token >> x >> y;

    getline(xStream, xError, L';');
    getline(yStream, yError, L';');

    try
    {
//      using boost::lexical_cast;
//      if(BackwardCompatibility)
//        PointList.push_back(TPointSeriesPoint(lexical_cast<double>(x), lexical_cast<double>(y), xError.empty() ? 0.0 : lexical_cast<double>(yError), xError.empty() ? 0.0 : lexical_cast<double>(yError)));
//      else
        PointList.push_back(TPointSeriesPoint(GetData(), x, y, xError, yError));
    }
    catch(Func32::EParseError &E)
    {
      ShowStatusError(GetErrorMsg(E));
      PointList.push_back(TPointSeriesPoint(GetData(), x, y, xError, yError, true));
    }
  }

  xErrorBarType = Section.Read(L"xErrorBarType", ebtNone);
  xErrorValue = Section.Read(L"xErrorBarValue", 0.0);

  yErrorBarType = Section.Read(L"yErrorBarType", ebtNone);
  yErrorValue = Section.Read(L"yErrorBarValue", 0.0);
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
void TTextLabel::WriteToIni(TConfigFileSection &Section) const
{
  Section.Write(L"Placement", LabelPlacement);
  if(LabelPlacement == lpUserTopLeft || LabelPlacement >= lpUserTopRight)
    Section.Write(L"Pos", xPos.Text + L";" + yPos.Text);
  Section.Write(L"Rotation", Rotation, 0U);
  Section.Write(L"Text", ToWString(EncodeEscapeSequence(Text)));
  Section.Write(L"BackgroundColor", BackgroundColor);

  TGraphElem::WriteToIni(Section);
}
//---------------------------------------------------------------------------
void TTextLabel::ReadFromIni(const TConfigFileSection &Section)
{
  LabelPlacement = Section.Read(L"Placement", lpUserTopLeft);
  std::wstring Temp = Section.Read(L"Pos", L"0;0");
  unsigned n = Temp.find(L";");
  if(n == std::string::npos)
  {
    //For backwards compatibility
    Func32::TDblPoint Pos = Section.Read(L"Pos", Func32::TDblPoint(0,0));
    xPos = TTextValue(Pos.x);
    yPos = TTextValue(Pos.y);
  }
  else
  {
    xPos.Set(Temp.substr(0, n), GetData(), true);
    yPos.Set(Temp.substr(n+1), GetData(), true);
  }

  Rotation = Section.Read(L"Rotation", 0U);
  Text = DecodeEscapeSequence(ToString(Section.Read(L"Text", L"ERROR")));
  BackgroundColor = Section.Read(L"BackgroundColor", clNone);

  Update();
  StatusText = ToWString(RtfToPlainText(Text));

  TGraphElem::ReadFromIni(Section);
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
TRelation::TRelation(const std::wstring &AText, const Func32::TSymbolList &SymbolList, TColor AColor, TBrushStyle Style, unsigned ASize, Func32::TTrigonometry Trig)
  : Text(AText), Color(AColor), BrushStyle(Style), Size(ASize)
{
  std::vector<std::string> Args;
  Args.push_back("x");
  Args.push_back("y");
  Func.SetTrigonometry(Trig);
  Constraints.SetTrigonometry(Trig);
  Func.SetFunc(ToString(Text), Args, SymbolList);
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
void TRelation::WriteToIni(TConfigFileSection &Section) const
{
  Section.Write(L"Relation", Text);
  Section.Write(L"Constraints", ConstraintsText, std::wstring());
  Section.Write(L"Style", BrushStyle);
  Section.Write(L"Color", Color);
  Section.Write(L"Size", Size, 1U);

  TGraphElem::WriteToIni(Section);
}
//---------------------------------------------------------------------------
void TRelation::ReadFromIni(const TConfigFileSection &Section)
{
  Func.SetTrigonometry(GetData().Axes.Trigonometry);
  Constraints.SetTrigonometry(GetData().Axes.Trigonometry);
  Text = Section.Read(L"Relation", L"");
  ConstraintsText = Section.Read(L"Constraints", L"");
  BrushStyle = Section.Read(L"Style", bsBDiagonal);
  Color = Section.Read(L"Color", clGreen);
  Size = Section.Read(L"Size", 1);
  std::vector<std::string> Args;
  Args.push_back("x");
  Args.push_back("y");
  Func.SetFunc(ToString(Text), Args, GetData().CustomFunctions.SymbolList );
  if(!ConstraintsText.empty())
    Constraints.SetFunc(ToString(ConstraintsText), Args, GetData().CustomFunctions.SymbolList);
  RelationType = Func.GetFunctionType() == Func32::ftInequality ? rtInequality : rtEquation;
  if(Func.GetFunctionType() == Func32::ftEquation)
    Func.RemoveRelation();

  TGraphElem::ReadFromIni(Section);
}
//---------------------------------------------------------------------------
std::wstring TRelation::MakeText() const
{
  return ConstraintsText.empty() ? Text : Text + L"; " + ConstraintsText;
}
//---------------------------------------------------------------------------
void TRelation::SetConstraints(const std::wstring &AConstraintsText, const Func32::TSymbolList &SymbolList)
{
  std::vector<std::string> Args;
  Args.push_back("x");
  Args.push_back("y");
  Constraints.SetFunc(ToString(AConstraintsText), Args, SymbolList);
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
void TAxesView::ReadFromIni(const TConfigFileSection &Section)
{
  SetShowInLegend(false);
}
//---------------------------------------------------------------------------
void TAxesView::WriteToIni(TConfigFileSection &Section) const
{
  GetData().Axes.WriteToIni(Section);
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

