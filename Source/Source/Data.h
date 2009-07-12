/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef DataH
#define DataH
#include "VclObject.h"
#include "GraphElem.h"
#include "Settings.h"
#include "Convert.h"

class TConfigFile;
namespace Graph
{
struct TUserModel
{
  std::wstring Model;
  std::vector<std::pair<std::wstring, double> > Defaults;
};

typedef std::map<std::wstring, TUserModel> TUserModels;

typedef bool (__closure *TAbortUpdateEvent)();


class TData
{
  bool Modified;

  void SaveData(TConfigFile &IniFile);
  const TData& operator=(const TData&);             //Not implemented
  std::wstring GrfName;
  TAbortUpdateEvent OnAbortUpdate;
  std::vector<boost::shared_ptr<TGraphElem> > ElemList;
  void SaveImage(TConfigFile &IniFile, TCanvas *Canvas, int Width, int Height);

public:
  TAxes Axes;
  TUserModels UserModels;
  TCustomFunctions CustomFunctions;
  TAnimationInfo AnimationInfo;

  TData() : CustomFunctions(*this) {}
  TData(const TData &OldData);
  void ClearCache();
  void Clear();
  void LoadDefault();
  bool LoadFromFile(const std::wstring &FileName, bool ShowErrorMessages = true);
  bool Load(TConfigFile &IniFile);
  bool Save(const std::wstring &FileName, bool Remember);
  std::wstring SaveToString(bool ResetModified);
  void SaveDefault();
  bool CheckIniInfo(const TConfigFile &IniFile, bool ShowErrorMessages = true);
  static void WriteInfoToIni(TConfigFile &IniFile);
  void LoadData(const TConfigFile &IniFile);
  void PreprocessGrfFile(TConfigFile &IniFile);
  bool Import(const std::wstring &FileName);
  bool ImportData(const std::wstring &FileName);
  std::wstring CreatePointSeriesDescription();
  boost::shared_ptr<TTextLabel> FindLabel(int X, int Y); //NULL indicates not label found
  void DeleteLabel(int Index);
  void ImportUserModels(const std::wstring &Str);
  std::wstring ExportUserModels() const;
  void SetModified();
  bool IsModified() const {return Modified;}
  double FindInterception(const TBaseFuncType *Func, int X, int Y) const;
  void Delete(const boost::shared_ptr<TGraphElem> &Elem);
  void Insert(const boost::shared_ptr<TGraphElem> &Elem, unsigned Index);
  void Add(const boost::shared_ptr<TGraphElem> &Elem);
  int GetIndex(const boost::shared_ptr<const TGraphElem> &Elem);
  boost::shared_ptr<TGraphElem> Replace(unsigned Index, const boost::shared_ptr<TGraphElem> &Elem);
  void Replace(const boost::shared_ptr<TGraphElem> &OldElem, const boost::shared_ptr<TGraphElem> &NewElem);
  const std::wstring& GetFileName() const {return GrfName;}
  boost::shared_ptr<TBaseFuncType> GetFuncFromIndex(unsigned Index) const;
  TGraphElemPtr Back() const;
  const TGraphElemPtr& GetElem(unsigned Index) const;
  unsigned ElemCount() const {return ElemList.size();}
  std::vector<TGraphElemPtr>::const_iterator Begin() const {return ElemList.begin();}
  std::vector<TGraphElemPtr>::const_iterator End() const {return ElemList.end();}
  void Update();
  void SetAbortUpdateEvent(TAbortUpdateEvent AOnAbortUpdate) {OnAbortUpdate = AOnAbortUpdate;}
  void AbortUpdate() const {if(OnAbortUpdate) OnAbortUpdate();}
  void Swap(unsigned Index1, unsigned Index2) {boost::swap(ElemList[Index1], ElemList[Index2]);}

  long double Calc(const std::wstring &Str) const
  {
    return Eval(Str, CustomFunctions.SymbolList, Axes.Trigonometry);
  }

  Func32::TComplex CalcComplex(const std::wstring &Str) const
  {
    return EvalComplex(Str, CustomFunctions.SymbolList, Axes.Trigonometry);
  }
};
//---------------------------------------------------------------------------
//extern TData Data;

enum TTraceType {ttTrace, ttIntersection, ttXAxis, ttYAxis, ttExtremeX, ttExtremeY};
double TraceFunction(const TBaseFuncType *Func, TTraceType TraceType, int X, int Y, const TData &Data, const class TDraw &Draw);
bool ExportPointSeries(const TPointSeries *Series, const wchar_t *FileName, char Delimiter, char DecimalSeparator);
double FindNearestPoint(const TBaseFuncType *Func, int X, int Y);
} //namespace Graph
using namespace Graph;
//---------------------------------------------------------------------------
#endif
