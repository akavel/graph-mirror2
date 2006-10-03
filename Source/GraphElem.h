/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef GraphElemH
#define GraphElemH
#include "../Func32/Func32.h"
#include <boost/weak_ptr.hpp>
#include <deque>

enum TGraphError
{
  geInvalidRelation
};

struct EGraphError
{
  TGraphError ErrorCode;
  EGraphError(TGraphError AErrorCode) : ErrorCode(AErrorCode) {}
};

typedef boost::shared_ptr<class TGraphElem> TGraphElemPtr;

struct TGraphElemVisitor
{
  virtual void Visit(class TBaseFuncType &Func) =0;
  virtual void Visit(class TTan &Tan) = 0;
  virtual void Visit(class TPointSeries &Series) =0;
  virtual void Visit(class TShade &Shade) =0;
  virtual void Visit(class TTextLabel &Label) =0;
  virtual void Visit(class TRelation &Relation) =0;
  virtual void Visit(class TAxesView &AxesView) =0;
};

struct TTextValue
{
  std::string Text;
  double Value;
  TTextValue() : Value(0) {}
  explicit TTextValue(double AValue);
  TTextValue(double AValue, const std::string &AText) : Value(AValue), Text(AText) {}
  bool operator==(const TTextValue &TextValue) const {return Value == TextValue.Value;}
  bool operator!=(const TTextValue &TextValue) const {return !(*this == TextValue);}
  bool operator<(double a) const {return Value < a;}
  bool operator>(double a) const {return Value > a;}
  bool operator<=(double a) const {return Value <= a;}
  bool operator>=(double a) const {return Value >= a;}
  void Update(const class TData &Data);
  void Set(const std::string AText, const TData &Data);
};

std::ostream& operator<<(std::ostream &Stream, const TTextValue &TextValue);

class TData;
class TBaseFuncType;

class TGraphElem
{
  const TGraphElem& operator=(const TGraphElem&); //Not implemented
  const TData *Data;

public:
  bool Visible;
  bool ShowInLegend;
  std::wstring LegendText;
  std::vector<boost::shared_ptr<TGraphElem> > ChildList;

  TGraphElem() : Visible(true), ShowInLegend(true), Data(NULL) {}
  TGraphElem(const TGraphElem &Elem);
  virtual ~TGraphElem() {}
  virtual std::wstring GetLegendText() const {return LegendText.empty() ? MakeText() : LegendText;}
  virtual void SetParentFunc(const boost::shared_ptr<TBaseFuncType> &AFunc) {}
  virtual boost::shared_ptr<TBaseFuncType> ParentFunc() const {return boost::shared_ptr<TBaseFuncType>();}
  virtual std::wstring MakeText() const = 0;
  virtual void WriteToIni(class TConfigFile &IniFile, const std::string &Section) const=0;
  virtual void ReadFromIni(const TConfigFile &IniFile, const std::string &Section) =0;
  virtual void Accept(TGraphElemVisitor&) =0;
  virtual boost::shared_ptr<TGraphElem> Clone() const = 0;
  virtual void ClearCache() {};
  virtual void Update() {};
  void SetData(const TData *AData); //For internal use
  const TData& GetData() const {BOOST_ASSERT(Data); return *Data;}
  void AddChild(const TGraphElemPtr &Elem);
  void ReplaceChild(unsigned Index, const TGraphElemPtr &Elem);
  unsigned GetChildIndex(const TGraphElemPtr &Elem) const;
};

enum TLabelPlacement {lpUserPos, lpAboveX, lpBelowX, lpLeftOfY, lpRightOfY};

class TTextLabel : public TGraphElem
{
  std::string Text;
  TLabelPlacement LabelPlacement;
  Func32::TCoord<double> Pos; //Only used when TextLabelPos is tlpCustom
  TRect Rect;
  TColor BackgroundColor;
  TVclObject<TMetafile> Metafile;
  unsigned Rotation; //Rotation in degrees
  std::wstring StatusText; //The text shown in the TreeView

public:
  TTextLabel() : LabelPlacement(lpUserPos), Pos(0,0), Rect(0,0,0,0), Rotation(0) {}
  TTextLabel(const std::string &Str, TLabelPlacement Placement, const Func32::TCoord<double> &Coord, TColor Color, unsigned ARotation);
  void WriteToIni(TConfigFile &IniFile, const std::string &Section) const;
  void ReadFromIni(const TConfigFile &IniFile, const std::string &Section);
  std::wstring MakeText() const;
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  int UpdateRect(int X, int Y) {int Width = Rect.Width(); int Height = Rect.Height(); Rect = TRect(X, Y, X + Width, Y + Height); return Width;}
  bool IsInsideRect(int X, int Y) const {return InsideRect(Rect, TPoint(X, Y));}
  const Func32::TCoord<double>& GetPos() const {return Pos;}
  const std::string& GetText() const {return Text;}
  void Scale(double xSizeMul, double ySizeMul);
  const TRect& GetRect() const {return Rect;}
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TGraphElem>(new TTextLabel(*this));}
  TColor GetBackgroundColor() const {return BackgroundColor;}
  TLabelPlacement GetPlacement() const {return LabelPlacement;}
  unsigned GetRotation() const {return Rotation;}
  TMetafile* GetImage() const {return Metafile;}
  void Update();
};

struct TAxes;
enum TDrawType {dtAuto, dtDots, dtLines};

class TBaseFuncType : public TGraphElem
{
protected:
  static const boost::shared_ptr<TBaseFuncType> CloneWithTangents(TBaseFuncType *Dest, const TBaseFuncType *Src);
  TBaseFuncType(const TBaseFuncType &F);

public:
  std::vector<Func32::TCoordSet> sList;
  std::vector<TPoint> Points;
  std::vector<unsigned> PointNum;
  TColor Color;
  unsigned Size;
  TPenStyle Style;
  TTextValue From, To;
  unsigned Steps;
  unsigned StartPointStyle, EndPointStyle;
  TDrawType DrawType;

  TBaseFuncType();
  virtual void SetTrigonometry(Func32::TTrigonometry Trig) {}; //Does nothing as default
  void WriteToIni(TConfigFile &IniFile, const std::string &Section) const;
  void ReadFromIni(const TConfigFile &IniFile, const std::string &Section);
  virtual boost::shared_ptr<TBaseFuncType> MakeDifFunc() =0;
  virtual std::pair<double,double> GetCurrentRange() const {return std::make_pair(From.Value, To.Value);}
  virtual unsigned GetSteps() const {return Steps;}
  virtual std::string GetVariable() const {return "";}
  virtual const Func32::TBaseFunc& GetFunc() const =0;
  Func32::TBaseFunc& GetFunc() {return const_cast<Func32::TBaseFunc&>(const_cast<const TBaseFuncType*>(this)->GetFunc());}
  void ClearCache();
  void Update();
};

enum TTangentType {ttTangent, ttNormal};
class TTan : public TBaseFuncType
{
  boost::weak_ptr<TBaseFuncType> Func;
  mutable double a, q;  //Calculated at last redraw; a!=INF: y=ax+q, a==INF: x=q
  mutable Func32::TParamFunc TanFunc;

public:
  TTextValue t;
  TTangentType TangentType;

  std::wstring MakeText() const;
  void WriteToIni(TConfigFile &IniFile, const std::string &Section) const;
  void ReadFromIni(const TConfigFile &IniFile, const std::string &Section);
  std::wstring GetLegendText() const;
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TBaseFuncType>(new TTan(*this));}
  boost::shared_ptr<TBaseFuncType> MakeDifFunc() {throw Exception("Tangent cannot be differentiated");}
  void UpdateTan(double a1, double q1);
  bool IsValid() const; //Indicates the parent function is valid at t
  std::pair<double,double> GetCurrentRange() const;
  unsigned GetSteps() const;
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  const Func32::TParamFunc& GetFunc() const {return TanFunc;}
  boost::shared_ptr<TBaseFuncType> ParentFunc() const {return Func.lock();}
  void SetParentFunc(const boost::shared_ptr<TBaseFuncType> &AFunc) {Func = AFunc;}
  void Update();
};

class TStdFunc : public TBaseFuncType
{
  friend class TStdFuncImpl;
  std::string Text;
  Func32::TFunc Func;

public:
  TStdFunc() {}
  TStdFunc(const std::string &AText, const Func32::TSymbolList &SymbolList);
  TStdFunc(const Func32::TFunc &AFunc) : Text(AFunc.MakeText()), Func(AFunc) {}

  boost::shared_ptr<TGraphElem> Clone() const {return CloneWithTangents(new TStdFunc(*this), this);}
  std::wstring MakeText() const;
  void SetTrigonometry(Func32::TTrigonometry Trig);
  void WriteToIni(TConfigFile &IniFile, const std::string &Section) const;
  void ReadFromIni(const TConfigFile &IniFile, const std::string &Section);
  boost::shared_ptr<TBaseFuncType> MakeDifFunc();
  std::pair<double,double> GetCurrentRange() const;
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  std::string GetVariable() const {return "x";}
  const Func32::TFunc& GetFunc() const {return Func;}
  const std::string& GetText() const {return Text;}
};

class TParFunc : public TBaseFuncType
{
  std::string xText,yText;
  Func32::TParamFunc Func;

public:
  TParFunc() {}
  TParFunc(const std::string &AxText, const std::string &AyText, const Func32::TSymbolList &SymbolList);
  TParFunc(const Func32::TParamFunc &AFunc) : xText(AFunc.MakeXText()), yText(AFunc.MakeYText()), Func(AFunc) {}

  boost::shared_ptr<TGraphElem> Clone() const {return CloneWithTangents(new TParFunc(*this), this);}
  std::wstring MakeText() const;
  void SetTrigonometry(Func32::TTrigonometry Trig);
  void WriteToIni(TConfigFile &IniFile, const std::string &Section) const;
  void ReadFromIni(const TConfigFile &IniFile, const std::string &Section);
  boost::shared_ptr<TBaseFuncType> MakeDifFunc();
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  std::string GetVariable() const {return "t";}
  const Func32::TParamFunc& GetFunc() const {return Func;}
  const std::string& GetxText() const {return xText;}
  const std::string& GetyText() const {return yText;}
};

class TPolFunc : public TBaseFuncType
{
  std::string Text;
  Func32::TPolarFunc Func;

public:
  TPolFunc() {}
  TPolFunc(const std::string &AText, const Func32::TSymbolList &SymbolList);

  boost::shared_ptr<TGraphElem> Clone() const {return CloneWithTangents(new TPolFunc(*this), this);}
  std::wstring MakeText() const;
  void SetTrigonometry(Func32::TTrigonometry Trig);
  void WriteToIni(TConfigFile &IniFile, const std::string &Section) const;
  void ReadFromIni(const TConfigFile &IniFile, const std::string &Section);
  boost::shared_ptr<TBaseFuncType> MakeDifFunc();
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  std::string GetVariable() const {return "t";}
  const Func32::TPolarFunc& GetFunc() const {return Func;}
  const std::string& GetText() const {return Text;}
};

enum TErrorBarType {ebtNone, ebtFixed, ebtRelative, ebtCustom};
struct TPointSeriesPoint
{
  TTextValue x, y;
  TTextValue xError, yError; //Data for error bars; only used if Uncertainty=utCustom

  TPointSeriesPoint() {}
  TPointSeriesPoint(double X, double Y, double XError = 0, double YError = 0)
    : x(X), y(Y), xError(XError), yError(YError) {}
  TPointSeriesPoint(const TData &Data, const std::string &X, const std::string &Y, const std::string &XError = "", const std::string &YError = "");
};

enum TInterpolationAlgorithm {iaLinear, iaCubicSpline, iaHalfCosine};
struct TPointSeries : public TGraphElem
{
  TColor FrameColor;
  TColor FillColor;
  TColor LineColor;
  unsigned Size;
  unsigned Style; //Marker type
  unsigned LineSize;
  TPenStyle LineStyle;
  TInterpolationAlgorithm Interpolation;
  bool ShowLabels;
  TLabelPosition LabelPosition;
  TVclObject<TFont> Font;
  std::vector<TPointSeriesPoint> PointList;
  TErrorBarType xErrorBarType, yErrorBarType;
  double xErrorValue, yErrorValue; //Data for error bars; only used if Uncertainty!=utCustom
  void Update();

  TPointSeries();
  std::wstring MakeText() const {return LegendText;}
  void WriteToIni(TConfigFile &IniFile, const std::string &Section) const;
  void ReadFromIni(const TConfigFile &IniFile, const std::string &Section);
  Func32::TDblPoint FindCoord(double x) const;
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  double GetXError(unsigned Index) const;
  double GetYError(unsigned Index) const;
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TGraphElem>(new TPointSeries(*this));}
  Func32::TDblPoint GetPoint(unsigned Index) const {return Func32::TDblPoint(PointList[Index].x.Value, PointList[Index].y.Value);}
  void AddPoint(const Func32::TDblPoint &Point) {PointList.push_back(TPointSeriesPoint(Point.x, Point.y));}
};

enum TShadeStyle {ssAbove, ssBelow, ssXAxis, ssYAxis, ssBetween, ssInside};

struct TShade : public TGraphElem
{
  TShadeStyle ShadeStyle;
  TBrushStyle BrushStyle;
  TColor Color;
  boost::weak_ptr<TBaseFuncType> Func;
  boost::shared_ptr<TBaseFuncType> Func2;
  TTextValue sMin;
  TTextValue sMax;
  TTextValue sMin2;
  TTextValue sMax2;
  bool ExtendMinToIntercept;
  bool ExtendMaxToIntercept;
  bool ExtendMin2ToIntercept;
  bool ExtendMax2ToIntercept;
  bool MarkStart;
  bool MarkEnd;

  TShade(){}
  TShade(TShadeStyle AShadeStyle, TBrushStyle ABrushStyle, TColor AColor,
    const boost::shared_ptr<TBaseFuncType> &AFunc, const boost::shared_ptr<TBaseFuncType> &AFunc2,
    double AsMin, double AsMax, double AsMin2, double AsMax2, bool AExtendMinToIntercept, bool AExtendMaxToIntercept,
    bool AExtendMin2ToIntercept, bool AExtendMax2ToIntercept);
  std::wstring MakeText() const;
  void WriteToIni(TConfigFile &IniFile, const std::string &Section) const;
  void ReadFromIni(const TConfigFile &IniFile, const std::string &Section);
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TGraphElem>(new TShade(*this));}
  boost::shared_ptr<TBaseFuncType> ParentFunc() const {return Func.lock();}
  void SetParentFunc(const boost::shared_ptr<TBaseFuncType> &AFunc) {Func = AFunc;}
  void Update();
};

enum TRelationType {rtEquation, rtInequality};
class TRelation : public TGraphElem
{
  std::string Text;
  std::string ConstraintsText;
  Func32::TCustomFunc Func;
  Func32::TCustomFunc Constraints;
  TColor Color;
  unsigned Size;
  TBrushStyle BrushStyle;
  TRelationType RelationType;

public:
  boost::shared_ptr<class TRegion> Region;
  boost::shared_ptr<class TRegion> BoundingRegion; //Used to draw the frame around Region for inequalities

  TRelation();
  TRelation(const std::string &AText, const Func32::TSymbolList &SymbolList, TColor AColor, TBrushStyle Style, unsigned ASize);
  TRelation(const TRelation &Relation);
  std::wstring MakeText() const;
  void WriteToIni(class TConfigFile &IniFile, const std::string &Section) const;
  void ReadFromIni(const TConfigFile &IniFile, const std::string &Section);
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TGraphElem>(new TRelation(*this));}
  void SetConstraints(const std::string &AConstraintsText, const Func32::TSymbolList &SymbolList);
  void Update();

  TColor GetColor() const {return Color;}
  TBrushStyle GetBrushStyle() const {return BrushStyle;}
  const std::string& GetText() const {return Text;}
  const std::string& GetConstraints() const {return ConstraintsText;}
  long double Eval(const std::vector<long double> &Args, Func32::ECalcError &E);
  void ClearCache();
  TRelationType GetRelationType() const {return RelationType;}
  unsigned GetSize() const {return Size;}
};

class TAxesView : public TGraphElem
{
public:
  TAxesView() {ShowInLegend = false;}
  std::wstring MakeText() const {return L"";}
  void WriteToIni(class TConfigFile &IniFile, const std::string &Section) const;
  void ReadFromIni(const TConfigFile &IniFile, const std::string &Section) {ShowInLegend = false;}
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TGraphElem>(new TAxesView(*this));}
};
//---------------------------------------------------------------------------
#endif
