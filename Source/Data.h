/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
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

struct TUserModel
{
  std::wstring Name;
  std::string Model;
  std::vector<std::pair<std::string, double> > Defaults;
};

typedef bool (__closure *TAbortUpdateEvent)();

class TData
{
  bool Modified;

  void SaveData(TConfigFile &IniFile);
  const TData& operator=(const TData&);             //Not implemented
  std::string GrfName;
  TAbortUpdateEvent OnAbortUpdate;
  std::vector<boost::shared_ptr<TGraphElem> > ElemList;
  void SaveImage(TConfigFile &IniFile);

public:
  TAxes Axes;
  TProperty Property;
  std::vector<TUserModel> UserModels;
  TCustomFunctions CustomFunctions;

  TData(){}
  TData(const TData &OldData);
  void ClearCache();
  void Clear();
  void LoadDefault();
  bool LoadFromFile(const std::string &FileName, bool ShowErrorMessages = true);
  bool LoadFromString(const std::string &Str);
  bool Save(const std::string &FileName, bool Remember);
  std::string SaveToString(bool ResetModified);
  void SaveDefault();
  bool CheckIniInfo(const TConfigFile &IniFile, bool ShowErrorMessages = true);
  static void WriteInfoToIni(TConfigFile &IniFile);
  void LoadData(const TConfigFile &IniFile);
  bool Import(const std::string &FileName);
  bool ImportData(const std::string &FileName);
  std::wstring CreatePointSeriesDescription();
  boost::shared_ptr<TTextLabel> FindLabel(int X, int Y); //NULL indicates not label found
  void DeleteLabel(int Index);
  void LoadUserModels();
  void SaveUserModels() const;
  void SetModified();
  bool IsModified() const {return Modified;}
  double FindInterception(const TBaseFuncType *Func, int X, int Y, long double Tol) const;
  void Delete(const boost::shared_ptr<TGraphElem> &Elem);
  void Insert(const boost::shared_ptr<TGraphElem> &Elem, unsigned Index);
  void Add(const boost::shared_ptr<TGraphElem> &Elem);
  int GetIndex(const boost::shared_ptr<const TGraphElem> &Elem);
  boost::shared_ptr<TGraphElem> Replace(unsigned Index, const boost::shared_ptr<TGraphElem> &Elem);
  void Replace(const boost::shared_ptr<TGraphElem> &OldElem, const boost::shared_ptr<TGraphElem> &NewElem);
  const std::string& GetFileName() const {return GrfName;}
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

  double Calc(const std::string &Str) const
  {
    return Eval(Str, CustomFunctions.SymbolList, Axes.Trigonometry);
  }

  Func32::TComplex CalcComplex(const std::string &Str) const
  {
    return EvalComplex(Str, CustomFunctions.SymbolList, Axes.Trigonometry);
  }
};
//---------------------------------------------------------------------------
//extern TData Data;

enum TTraceType {ttTrace, ttIntersection, ttXAxis, ttYAxis, ttExtremeX, ttExtremeY};
double TraceFunction(TBaseFuncType *Func, TTraceType TraceType, int X, int Y, const TData &Data, const class TDraw &Draw);
bool ExportPointSeries(const TPointSeries *Series, const char *FileName, char Delimiter);
double FindNearestPoint(const TBaseFuncType *Func, int X, int Y);

//---------------------------------------------------------------------------
#endif
