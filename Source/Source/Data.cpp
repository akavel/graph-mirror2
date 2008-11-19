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
#include <memory>
#include <algorithm>
#include <complex>
#include <Registry.hpp>
#include "Clipboard.h"
#include "VersionInfo.h"
#include "IGraphic.h"
#include <fstream>
#include "ConfigFile.h"
#include <float.h>
#include <functional>
#include <boost/lexical_cast.hpp>
#include "OleServer.h"
#include "OleObjectElem.h"
#include <iterator>

TUndoList UndoList(50);
//---------------------------------------------------------------------------
TData::TData(const TData &OldData) : Axes(OldData.Axes), CustomFunctions(OldData.CustomFunctions),
  Modified(false), OnAbortUpdate(NULL)
{
  for(unsigned int I = 0; I < OldData.ElemList.size(); I++)
  {
    ElemList.push_back(boost::static_pointer_cast<TBaseFuncType>(OldData.ElemList[I]->Clone()));
    ElemList.back()->SetData(this);
  }

  for(unsigned int I = 0; I < ElemList.size(); I++)
    for(unsigned J = 0; J < ElemList[I]->ChildList.size(); J++)
      if(TShade *Shade = dynamic_cast<TShade*>(ElemList[I]->ChildList[J].get()))
        if(Shade->Func2)
        {
          //Update pointer cross references
          int Index = IndexOf(OldData.ElemList, Shade->Func2);
          Shade->Func2 = boost::dynamic_pointer_cast<TBaseFuncType>(ElemList[Index]);
        }

//  Resize(1);
}
//---------------------------------------------------------------------------
void TData::WriteInfoToIni(TConfigFile &IniFile)
{
  TVersionInfo VerInfo;
  TConfigFileSection &Section = IniFile.Section(L"Graph");
  Section.Write(L"Version", VerInfo.FileVersion().Text());
  Section.Write(L"MinVersion", MINVERSION);
  Section.Write(L"OS", GetWindowsVersion());
}
//---------------------------------------------------------------------------
bool TData::CheckIniInfo(const TConfigFile &IniFile, bool ShowErrorMessages)
{
  std::wstring MinVersion = IniFile.Section(L"Graph").Read(L"MinVersion", L"1.0");
  if(TVersion(MinVersion) > TVersionInfo().ProductVersion())
  {
    if(ShowErrorMessages)
      MessageBox(LoadRes(548, MinVersion), LoadString(RES_FILE_ERROR), MB_ICONSTOP);
    return false;
  }

  TVersion SavedByVersion = IniFile.Section(L"Graph").Read(L"Version", TVersion(100));
  if(SavedByVersion < MIN_SAVED_VERSION)
  {
    if(ShowErrorMessages)
      MessageBox(LoadRes(549), LoadRes(RES_FILE_ERROR), MB_ICONSTOP);
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
void TData::Clear()
{
  ElemList.clear();
  CustomFunctions.Clear();
  AnimationInfo.Clear();
}
//---------------------------------------------------------------------------
void TData::ClearCache()
{
  for(unsigned I = 0; I < ElemList.size(); I++)
    ElemList[I]->ClearCache();
}
//---------------------------------------------------------------------------
void TData::SaveData(TConfigFile &IniFile)
{
  unsigned FuncCount = 0;
  unsigned PointSeriesCount = 0;
  unsigned ShadeCount = 0;
  unsigned LabelCount = 0;
  unsigned RelationCount = 0;
  unsigned OleObjectCount = 0;

  for(unsigned I = 0; I < ElemList.size(); I++)
  {
    if(TBaseFuncType *Func = dynamic_cast<TBaseFuncType*>(ElemList[I].get()))
    {
      std::wstring SectionName = L"Func" + ToWString(++FuncCount);
      Func->WriteToIni(IniFile.Section(SectionName));

      //Write tangents
      unsigned TanCount = 0;
      for(unsigned N = 0; N < Func->ChildList.size(); N++)
        if(dynamic_cast<TTan*>(Func->ChildList[N].get()))
          Func->ChildList[N]->WriteToIni(IniFile.Section(SectionName + L"Tan" + ToWString(++TanCount)));
      IniFile.Section(SectionName).Write(L"TanCount", TanCount, 0U);
    }
    else if(TPointSeries *Series = dynamic_cast<TPointSeries*>(ElemList[I].get()))
      Series->WriteToIni(IniFile.Section(L"PointSeries" + ToWString(++PointSeriesCount)));
    else if(TTextLabel *Label = dynamic_cast<TTextLabel*>(ElemList[I].get()))
      Label->WriteToIni(IniFile.Section(L"Label" + ToWString(++LabelCount)));
    else if(TRelation *Relation = dynamic_cast<TRelation*>(ElemList[I].get()))
      Relation->WriteToIni(IniFile.Section(L"Relation" + ToWString(++RelationCount)));
    else if(TAxesView *AxesView = dynamic_cast<TAxesView*>(ElemList[I].get()))
      AxesView->WriteToIni(IniFile.Section(L"Axes"));
    else if(TOleObjectElem *OleObjectElem = dynamic_cast<TOleObjectElem*>(ElemList[I].get()))
      OleObjectElem->WriteToIni(IniFile.Section(L"OleObject" + ToWString(++OleObjectCount)));

    for(unsigned J = 0; J < ElemList[I]->ChildList.size(); J++)
      if(dynamic_cast<TShade*>(ElemList[I]->ChildList[J].get()))
        ElemList[I]->ChildList[J]->WriteToIni(IniFile.Section(L"Shade" + ToWString(++ShadeCount)));
  }

  TConfigFileSection &DataSection = IniFile.Section(L"Data");
  //Don't use "LabelCount" because it gives problems in version 2.7.1
  DataSection.Write(L"TextLabelCount", LabelCount);
  DataSection.Write(L"FuncCount", FuncCount);
  DataSection.Write(L"PointSeriesCount", PointSeriesCount);
  DataSection.Write(L"ShadeCount", ShadeCount);
  DataSection.Write(L"RelationCount", RelationCount);
  DataSection.Write(L"OleObjectCount", OleObjectCount);
}
//---------------------------------------------------------------------------
void TData::LoadData(const TConfigFile &IniFile)
{
  for(unsigned I = 0; I < IniFile.SectionCount(); I++)
  {
    boost::shared_ptr<TGraphElem> Elem;
    const TConfigFileSection& Section = IniFile.Section(I);
    const std::wstring SectionName = Section.GetName();
    if(SectionName.substr(0, 4) == L"Func" && SectionName.find(L"Tan") == std::wstring::npos)
      switch(Section.Read(L"FuncType", ftStdFunc))
      {
        case ftStdFunc: Elem.reset(new TStdFunc); break;
        case ftParFunc: Elem.reset(new TParFunc); break;
        case ftPolFunc: Elem.reset(new TPolFunc); break;
        default: continue; //Ignore unknown function types
      }
    else if(SectionName.substr(0, 11) == L"PointSeries")
      Elem.reset(new TPointSeries);
    else if(SectionName.substr(0, 5) == L"Label")
      Elem.reset(new TTextLabel);
    else if(SectionName.substr(0, 8) == L"Relation")
      Elem.reset(new TRelation);
    else if(SectionName.substr(0, 10) == L"Axes")
      Elem.reset(new TAxesView);
    else if(SectionName.substr(0, 9) == L"OleObject")
      Elem.reset(new TOleObjectElem);
    else
      continue; //No known elem type

    //Stream data from inifile
    Elem->SetData(this);
    Elem->ReadFromIni(Section);

    //Create list of tangents
    int TanCount = Section.Read(L"TanCount", 0);
    for(int I = 0; I < TanCount; I++)
    {
      boost::shared_ptr<TTan> Tan(new TTan);
      Tan->SetData(this);
      Tan->ReadFromIni(IniFile.Section(L"Tan" + ToWString(I+1)));
      Elem->ChildList.push_back(Tan);
      Tan->SetParentFunc(boost::dynamic_pointer_cast<TBaseFuncType>(Elem));
    }

    ElemList.push_back(Elem);
    Elem->Update(); //Needed to update a and q for tangents. Must be called after ParentFunc is set
  }

  //We need to load shades after all functions are loaded
  unsigned ShadeCount = IniFile.Section(L"Data").Read(L"ShadeCount", 0U);
  for(unsigned I = 0; I < ShadeCount; I++)
  {
    boost::shared_ptr<TGraphElem> Shade(new TShade);
    Shade->SetData(this);
    Shade->ReadFromIni(IniFile.Section(L"Shade" + ToWString(I+1)));
    if(Shade->ParentFunc())
      Shade->ParentFunc()->ChildList.push_back(Shade);
  }
}
//---------------------------------------------------------------------------
//Find pixel coordinate on Func nearest to (X,Y)
//Returns -1 if there is none
int FindNearestPointN(const TBaseFuncType *Func, int X, int Y)
{
  unsigned BestN = 0;
  double BestDist = INF;

  //If no part of function visible
  if(Func->Points.size() == 0)
    return -1;

  for(unsigned N = 0; N < Func->Points.size(); N++)
  {
    double dX = X - Func->Points[N].x;
    double dY = Y - Func->Points[N].y;
    double Dist = dX*dX + dY*dY;
    if(Dist < BestDist)
    {
      BestDist = Dist;
      BestN = N;
    }
  }

  return BestN;
}
//---------------------------------------------------------------------------
//Returns NAN if there is none
double FindNearestPoint(const TBaseFuncType *Func, int X, int Y)
{
  int N = FindNearestPointN(Func, X, Y);
  return N == -1 ? NAN : Func->sList[N].t;
}
//---------------------------------------------------------------------------
//Find interception between Func and other functions
//Returns NAN if no interception is found
double TData::FindInterception(const TBaseFuncType *Func, int X, int Y) const
{
  if(Func->Points.size() < 3)
    return NAN;
  unsigned N = FindNearestPointN(Func, X, Y);
  if(N == 0)
    N = 1;
  else if(N == Func->Points.size() - 1)
    N--;

  typedef std::vector<Func32::TCoordSet>::const_iterator TCoordSetIter;
  TCoordSetIter Begin = Func->sList.begin();
  TCoordSetIter End = Func->sList.end();
  TCoordSetIter p1 = Begin + N; //p1 used to search backwards
  TCoordSetIter p2 = p1;        //p2 is always p1+1
  TCoordSetIter p3 = p2;        //p3 used to search forward
  TCoordSetIter p4 = p3;        //p4 is always p3+1
  --p1;
  ++p4;

  while(p1 != Begin || p4 != End)
  {
    for(unsigned I = 0; I < ElemList.size(); I++)
      if(const TBaseFuncType *Func2 = dynamic_cast<TBaseFuncType*>(ElemList[I].get()))
        if(Func2->GetVisible())
        {
          const Func32::TFunc *StdFunc2 = dynamic_cast<const Func32::TFunc*>(&Func2->GetFunc());

          if(StdFunc2 && Func == Func2)
            continue; //If it is the same function, and the function is a TFunc there is no crossing

          if(p1 != Begin)
          {
            //Standard function is a special case optimized for speed
            if(StdFunc2)
              try
              {
                if(Sign(p1->y - Func32::GetReal(StdFunc2->CalcY(Func32::TComplex(p1->x)))) != Sign(p2->y - Func32::GetReal(StdFunc2->CalcY(Func32::TComplex(p2->x)))))
                  return Func32::FindCrossing(Func->GetFunc(), p1->t, p2->t, *StdFunc2, p1->x, p2->x);
              }
              catch(Func32::EFuncError&) //Errors are ignored; Just continue
              {
              }
            else
            {
              TCoordSetIter Iter;
              if(Func == Func2)
              { //Special handling for crossing with self: Split the search in two
                Iter = FindCrossing(p1, Func->sList.begin(), p1-2);
                if(Iter == p1-2)
                  Iter = FindCrossing(p1, p1+2, Func->sList.end());
              }
              else
                Iter = FindCrossing(p1, Func2->sList.begin(), Func2->sList.end());
              if(Iter != Func2->sList.end())
              {
                unsigned N = Range<unsigned>(1, p1 - Begin, Func->sList.size() - 3);
                unsigned M = Range<unsigned>(1, Iter - Func2->sList.begin(), Func2->sList.size() - 3);
                //Use binary search to increase acuracy of found position
                return Func32::FindCrossing(Func->GetFunc(), Func->sList[N].t, Func->sList[N+1].t, Func2->GetFunc(), Func2->sList[M].t, Func2->sList[M+1].t);
              }
            }
          }

          if(p4 != End)
          {
            //Standard function is a special case optimized for speed
            if(StdFunc2)
              try
              {
                if(Sign(p3->y - Func32::GetReal(StdFunc2->CalcY(Func32::TComplex(p3->x)))) != Sign(p4->y - Func32::GetReal(StdFunc2->CalcY(Func32::TComplex(p4->x)))))
                  return Func32::FindCrossing(Func->GetFunc(), p3->t, p4->t, *StdFunc2, p3->x, p4->x);
              }
              catch(Func32::EFuncError&) //Errors are ignored; Just continue
              {
              }
            else
            {
              TCoordSetIter Iter;
              if(Func == Func2)
              { //Special handling for crossing with self: Split the search in two
                Iter = FindCrossing(p3, Func->sList.begin(), p3-2);
                if(Iter == p3-2)
                  Iter = FindCrossing(p3, p3+2, Func->sList.end());
              }
              else
                Iter = FindCrossing(p3, Func2->sList.begin(), Func2->sList.end());
              if(Iter != Func2->sList.end())
              {
                unsigned N = Range<unsigned>(1, p3 - Begin, Func->Points.size() - 3);
                unsigned M = Range<unsigned>(1, Iter - Func2->sList.begin(), Func2->sList.size() - 3);
                return Func32::FindCrossing(Func->GetFunc(), Func->sList[N].t, Func->sList[N+1].t, Func2->GetFunc(), Func2->sList[M].t, Func2->sList[M+1].t);
              }
            }
          }
        }

    if(p1 != Begin)
      --p1, --p2;
    if(p4 != End)
      ++p3, ++p4;
  }

  return NAN; //Signal no crossing found
}
//---------------------------------------------------------------------------
std::wstring TData::CreatePointSeriesDescription()
{
  std::wstring Str = LoadString(RES_SERIES) + L" ";
  int I = 1;
  while(true)
  {
    std::wstring CmpStr = Str + ToWString(I++);
    unsigned N;
    for(N = 0; N < ElemList.size(); N++)
      if(ElemList[N]->GetLegendText() == CmpStr)
        break;
    if(N == ElemList.size())
      return CmpStr;
  }
}
//---------------------------------------------------------------------------
boost::shared_ptr<TTextLabel> TData::FindLabel(int X, int Y)
{
  for(unsigned I = 0; I < ElemList.size(); I++)
    if(boost::shared_ptr<TTextLabel> Label = boost::dynamic_pointer_cast<TTextLabel>(ElemList[I]))
      if(Label->GetVisible() && Label->IsInsideRect(X, Y))
        return Label;
  return boost::shared_ptr<TTextLabel>();
}
//---------------------------------------------------------------------------
void TData::ImportUserModels(const std::wstring &Str)
{
  TConfigFile IniFile;
  IniFile.LoadFromString(Str);

  for(unsigned I = 0; I < IniFile.SectionCount(); I++)
  {
    const TConfigFileSection &Section = IniFile.Section(I);
    TUserModel UserModel;
    UserModel.Model = Section.Read(L"Model", L"$a*x+$b");

    for(TConfigFileSection::TIterator Iter = Section.Begin(); Iter != Section.End(); ++Iter)
    {
      if(Iter->first[0] == '$')
        UserModel.Defaults.push_back(std::make_pair(Iter->first, Calc(Iter->second)));
    }

    UserModels[Section.GetName()] = UserModel;
  }
}
//---------------------------------------------------------------------------
std::wstring TData::ExportUserModels() const
{
  TConfigFile IniFile;

  for(TUserModels::const_iterator Iter = UserModels.begin(); Iter != UserModels.end(); ++Iter)
  {
    TConfigFileSection &Section = IniFile.Section(Iter->first);
    Section.Write(L"Model", Iter->second.Model);

    for(unsigned J = 0; J < Iter->second.Defaults.size(); J++)
      Section.Write(Iter->second.Defaults[J].first, Iter->second.Defaults[J].second);
  }

  return IniFile.GetAsString();
}
//---------------------------------------------------------------------------
void TData::SetModified()
{
  Modified = true;
  SendOleAdvise(acDataChanged);
}
//---------------------------------------------------------------------------
double FindNearestValue(const std::vector<Func32::TCoordSet> &Values, int X, int Y, const TDraw &Draw)
{
  if(Values.empty())
    throw Exception("No data in vector");

  double MinDist = INF;
  double Result;

  for(std::vector<Func32::TCoordSet>::const_iterator Iter = Values.begin(); Iter != Values.end(); ++Iter)
  {
    double dx = X - Draw.xPoint(Iter->x);
    double dy = Y - Draw.yPoint(Iter->y);
    double Dist = dx*dx + dy*dy;
    if(Dist < MinDist)
      MinDist = Dist, Result = Iter->t;
  }

  return Result;
}
//---------------------------------------------------------------------------
double TraceFunction(const TBaseFuncType *Func, TTraceType TraceType, int X, int Y, const TData &Data, const TDraw &Draw)
{
  switch(TraceType)
  {
    case ttTrace:
      return FindNearestPoint(Func, X, Y); //Returns NAN if no point found

    case ttIntersection:
      return Data.FindInterception(Func, X, Y);

    case ttXAxis:
    case ttYAxis:
    {
      std::pair<double,double> Range = Func->GetCurrentRange();
      std::vector<Func32::TCoordSet> List = AnalyseFunction(Func->GetFunc(), Range.first, Range.second,
        Draw.GetAxesRect().Width(), 1E-16, TraceType == ttXAxis ? Func32::atXAxisCross : Func32::atYAxisCross);
      if(!List.empty())
        return FindNearestValue(List, X, Y, Draw);
      return NAN;
    }

    case ttExtremeY:
    case ttExtremeX:
    {
      std::pair<double,double> Range = Func->GetCurrentRange();
      //WARNING: bcc codegen bug when operator ?: is used and AnalyseFunction() throws an exception
      Func32::TFunc F;
      if(TraceType == ttExtremeY)
        F = Func->GetFunc().ConvYToFunc().MakeDif();
      else
        F = Func->GetFunc().ConvXToFunc().MakeDif();
      std::vector<Func32::TCoordSet> List = AnalyseFunction(F, Range.first, Range.second, Draw.GetAxesRect().Width(), 1E-16, Func32::atXAxisCross);

      //Convert the list of f'(x) coordinates to f(x) coordinates. Notice that List2 may have less
      //elements than List, because we may have found some extremums that don't have valid coordinates.
      std::vector<Func32::TCoordSet> List2;
      Transform(List.begin(), List.end(), std::back_inserter(List2), Func32::TEvalCoordSet(Func->GetFunc())); //Fill list with coordinates for f(x) instead of f'(x)
      if(!List2.empty())
        return FindNearestValue(List2, X, Y, Draw);
      return NAN;
    }

    default:
      return NAN;
  }
}
//---------------------------------------------------------------------------
bool ExportPointSeries(const TPointSeries *Series, const wchar_t *FileName, char Delimiter)
{
  std::wofstream File(FileName);
  if(!File)
    return false;

  File << "X" << Delimiter << "Y" << std::endl;
  for(unsigned I = 0; I < Series->PointList.size(); I++)
    File << Series->PointList[I].x.Text << Delimiter << Series->PointList[I].y.Text << std::endl;
  return true;
}
//---------------------------------------------------------------------------
void TData::Delete(const boost::shared_ptr<TGraphElem> &Elem)
{
  boost::shared_ptr<TBaseFuncType> Parent = Elem->ParentFunc();
  if(Parent)
    Parent->ChildList.erase(Parent->ChildList.begin() + IndexOf(Parent->ChildList, Elem));
  else
    ElemList.erase(ElemList.begin() + IndexOf(ElemList, Elem));
  Elem->SetData(NULL);
}
//---------------------------------------------------------------------------
void TData::Insert(const boost::shared_ptr<TGraphElem> &Elem, unsigned Index)
{
  boost::shared_ptr<TBaseFuncType> Parent = Elem->ParentFunc();
  if(Parent)
    Parent->ChildList.insert(Parent->ChildList.begin() + Index, Elem);
  else
  { //{} Needed as workaround to code generation bug i bcc 5.6.4
    ElemList.insert(ElemList.begin() + Index, Elem);
  }
  Elem->SetData(this);
}
//---------------------------------------------------------------------------
int TData::GetIndex(const boost::shared_ptr<const TGraphElem> &Elem)
{
  boost::shared_ptr<TBaseFuncType> Parent = Elem->ParentFunc();
  if(Parent)
    return IndexOf(Parent->ChildList, Elem);
  return IndexOf(ElemList, Elem);
}
//---------------------------------------------------------------------------
boost::shared_ptr<TGraphElem> TData::Replace(unsigned Index, const boost::shared_ptr<TGraphElem> &Elem)
{
  //Warning: Bcc 5.6.4 bug; Do not put Result outside brackets
  boost::shared_ptr<TBaseFuncType> Parent = Elem->ParentFunc();
  if(Parent)
  {
    boost::shared_ptr<TGraphElem> Result = Parent->ChildList[Index];
    Result->SetData(NULL);
    Parent->ChildList[Index] = Elem;
    Elem->SetData(this);
    return Result;
  }
  else
  {
    boost::shared_ptr<TGraphElem> Result = ElemList[Index];

    boost::shared_ptr<TBaseFuncType> OldFunc = boost::dynamic_pointer_cast<TBaseFuncType>(ElemList[Index]);
    boost::shared_ptr<TBaseFuncType> NewFunc = boost::dynamic_pointer_cast<TBaseFuncType>(Elem);
    if(!!OldFunc && !!NewFunc)
    {
      OldFunc->ChildList.swap(NewFunc->ChildList);
      for(unsigned I = 0; I < NewFunc->ChildList.size(); I++)
        NewFunc->ChildList[I]->SetParentFunc(NewFunc);

      for(unsigned I = 0; I < ElemList.size(); I++)
        for(unsigned J = 0; J < ElemList[I]->ChildList.size(); J++)
          if(TShade *Shade = dynamic_cast<TShade*>(ElemList[I]->ChildList[J].get()))
            if(Shade->Func2 == OldFunc)
              Shade->Func2 = NewFunc;
    }

    ElemList[Index] = Elem;
    Elem->SetData(this);
    Elem->Update(); //Update tangents
    Result->SetData(NULL);
    return Result;
  }
}
//---------------------------------------------------------------------------
void TData::Replace(const boost::shared_ptr<TGraphElem> &OldElem, const boost::shared_ptr<TGraphElem> &NewElem)
{
  Replace(GetIndex(OldElem), NewElem);
}
//---------------------------------------------------------------------------
void TData::Add(const boost::shared_ptr<TGraphElem> &Elem)
{
  boost::shared_ptr<TBaseFuncType> Parent = Elem->ParentFunc();
  if(Parent)
    Parent->ChildList.push_back(Elem);
  else
    ElemList.push_back(Elem);
  Elem->SetData(this);
}
//---------------------------------------------------------------------------
boost::shared_ptr<TBaseFuncType> TData::GetFuncFromIndex(unsigned Index) const
{
  unsigned Count = 0;
  for(unsigned I = 0; I < ElemList.size(); I++)
    if(boost::shared_ptr<TBaseFuncType> Func = boost::dynamic_pointer_cast<TBaseFuncType>(ElemList[I]))
      if(Count++ == Index)
        return Func;
  return boost::shared_ptr<TBaseFuncType>();
}
//---------------------------------------------------------------------------
boost::shared_ptr<TGraphElem> TData::Back() const
{
  if(ElemList.empty())
    return boost::shared_ptr<TGraphElem>();
  return ElemList.back();
}
//---------------------------------------------------------------------------
const boost::shared_ptr<TGraphElem>& TData::GetElem(unsigned Index) const
{
  BOOST_ASSERT(Index < ElemList.size());
  return ElemList[Index];
}
//---------------------------------------------------------------------------
void TData::Update()
{
  CustomFunctions.Update();
  for(unsigned I = 0; I < ElemList.size(); ++I)
    ElemList[I]->Update();
}
//---------------------------------------------------------------------------


