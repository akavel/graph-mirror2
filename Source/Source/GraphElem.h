/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
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

class TConfigFileSection;
namespace Graph
{
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
  virtual void Visit(class TOleObjectElem &OleObjectElem) =0;
};

struct TTextValue
{
  std::wstring Text;
  double Value;
  TTextValue() : Value(0) {}
  explicit TTextValue(double AValue);
  TTextValue(double AValue, const std::wstring &AText) : Value(AValue), Text(AText) {}
  bool operator==(const TTextValue &TextValue) const {return Value == TextValue.Value;}
  bool operator!=(const TTextValue &TextValue) const {return !(*this == TextValue);}
  bool operator==(double a) const {return Value == a;}
  bool operator<(double a) const {return Value < a;}
  bool operator>(double a) const {return Value > a;}
  bool operator<=(double a) const {return Value <= a;}
  bool operator>=(double a) const {return Value >= a;}
  void Update(const class TData &Data);
  void Set(const std::wstring AText, const TData &Data, bool IgnoreErrors = false);
  bool IsFinite() const {return std::_finite(Value);}
};

std::wostream& operator<<(std::wostream &Stream, const TTextValue &TextValue);

class TData;
class TBaseFuncType;

class TGraphElem
{
  const TGraphElem& operator=(const TGraphElem&); //Not implemented
  const TData *Data;
  bool Visible;
  bool ShowInLegend;
  std::wstring LegendText;

public:
  std::vector<boost::shared_ptr<TGraphElem> > ChildList;

  TGraphElem() : Visible(true), ShowInLegend(true), Data(NULL) {}
  TGraphElem(const TGraphElem &Elem);
  virtual ~TGraphElem() {}
  virtual std::wstring MakeLegendText() const {return LegendText.empty() ? MakeText() : LegendText;}
  virtual void SetParentFunc(const boost::shared_ptr<TBaseFuncType> &AFunc) {}
  virtual boost::shared_ptr<TBaseFuncType> ParentFunc() const {return boost::shared_ptr<TBaseFuncType>();}
  virtual std::wstring MakeText() const = 0;
  virtual void WriteToIni(TConfigFileSection &Section) const=0;
  virtual void ReadFromIni(const TConfigFileSection &Section) =0;
  virtual void Accept(TGraphElemVisitor&) =0;
  virtual boost::shared_ptr<TGraphElem> Clone() const = 0;
  virtual void ClearCache() {};
  virtual void Update() {};
  void SetData(const TData *AData); //For internal use
  const TData& GetData() const {BOOST_ASSERT(Data); return *Data;}
  void AddChild(const TGraphElemPtr &Elem);
  void ReplaceChild(unsigned Index, const TGraphElemPtr &Elem);
  unsigned GetChildIndex(const TGraphElemPtr &Elem) const;

  virtual int GetVisible() const {return Visible;}
  virtual void ChangeVisible() {Visible = !Visible;}
  void SetVisible(int AVisible) {Visible = AVisible;}
  bool GetShowInLegend() const {return ShowInLegend;}
  void SetShowInLegend(bool Value) {ShowInLegend = Value;}
  std::wstring GetLegendText() const {return LegendText;}
  void SetLegendText(const std::wstring &Str) {LegendText = Str;}
};

enum TLabelPlacement
{
  lpUserTopLeft,
  lpAboveX,
  lpBelowX,
  lpLeftOfY,
  lpRightOfY,
  lpUserTopRight,
  lpUserBottomLeft,
  lpUserBottomRight
};

class TTextLabel : public TGraphElem
{
  std::string Text;
  TLabelPlacement LabelPlacement;
  TTextValue xPos; //Only used when TextLabelPos is tlpCustom
  TTextValue yPos; //Only used when TextLabelPos is tlpCustom
  TRect Rect;
  TColor BackgroundColor;
  TVclObject<TMetafile> Metafile;
  unsigned Rotation; //Rotation in degrees
  std::wstring StatusText; //The text shown in the TreeView

public:
  TTextLabel() : LabelPlacement(lpUserTopLeft), Rect(0,0,0,0), Rotation(0) {}
  TTextLabel(const std::string &Str, TLabelPlacement Placement, const TTextValue &AxPos, const TTextValue &AyPos, TColor Color, unsigned ARotation);
  void WriteToIni(TConfigFileSection &Section) const;
  void ReadFromIni(const TConfigFileSection &Section);
  std::wstring MakeText() const;
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  int UpdateRect(int X, int Y) {int Width = Rect.Width(); int Height = Rect.Height(); Rect = TRect(X, Y, X + Width, Y + Height); return Width;}
  bool IsInsideRect(int X, int Y) const {return InsideRect(Rect, TPoint(X, Y));}
  const TTextValue& GetXPos() const {return xPos;}
  const TTextValue& GetYPos() const {return yPos;}
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
  TTextValue Steps; //Number of steps/evaluations. Rounded to an integer

public:
  std::vector<Func32::TCoordSet> sList;
  std::vector<TPoint> Points;
  std::vector<unsigned> PointNum;
  TColor Color;
  unsigned Size;
  TPenStyle Style;
  TTextValue From, To;
  unsigned StartPointStyle, EndPointStyle;
  TDrawType DrawType;

  TBaseFuncType();
  void WriteToIni(TConfigFileSection &Section) const;
  void ReadFromIni(const TConfigFileSection &Section);
  virtual boost::shared_ptr<TBaseFuncType> MakeDifFunc() =0;
  virtual std::pair<double,double> GetCurrentRange() const {return std::make_pair(From.Value, To.Value);}
  virtual const TTextValue& GetSteps() const {return Steps;}
  void SetSteps(const TTextValue &Value) {Steps = Value;}
  virtual std::string GetVariable() const {return "";}
  virtual const Func32::TBaseFunc& GetFunc() const =0;
  Func32::TBaseFunc& GetFunc() {return const_cast<Func32::TBaseFunc&>(const_cast<const TBaseFuncType*>(this)->GetFunc());}
  void ClearCache();
  void Update();
  Func32::TCoord<long double> Eval(long double t) const;
  virtual long double CalcArea(long double From, long double To) const;
};

enum TTangentType {ttTangent, ttNormal};
class TTan : public TBaseFuncType
{
  boost::weak_ptr<TBaseFuncType> Func;
  mutable double a, q;  //Calculated at last redraw; a!=INF: y=ax+q, a==INF: x=q
  mutable Func32::TParamFunc TanFunc;
  void UpdateTan(double a1, double q1);

public:
  TTextValue t;
  TTangentType TangentType;

  TTan::TTan();
  std::wstring MakeText() const;
  void WriteToIni(TConfigFileSection &Section) const;
  void ReadFromIni(const TConfigFileSection &Section);
  std::wstring MakeLegendText() const;
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TBaseFuncType>(new TTan(*this));}
  boost::shared_ptr<TBaseFuncType> MakeDifFunc() {throw Exception("Tangent cannot be differentiated");}
  bool IsValid() const; //Indicates the parent function is valid at t
  std::pair<double,double> GetCurrentRange() const;
  const TTextValue& GetSteps() const;
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  const Func32::TParamFunc& GetFunc() const {return TanFunc;}
  boost::shared_ptr<TBaseFuncType> ParentFunc() const {return Func.lock();}
  void SetParentFunc(const boost::shared_ptr<TBaseFuncType> &AFunc) {Func = AFunc;}
  void Update();
  long double CalcArea(long double From, long double To) const;
  bool CalcTan();
};

class TStdFunc : public TBaseFuncType
{
  friend class TStdFuncImpl;
  std::wstring Text;
  Func32::TFunc Func;

public:
  TStdFunc() {}
  TStdFunc(const std::wstring &AText, const Func32::TSymbolList &SymbolList, Func32::TTrigonometry Trig);
  TStdFunc(const Func32::TFunc &AFunc);

  boost::shared_ptr<TGraphElem> Clone() const {return CloneWithTangents(new TStdFunc(*this), this);}
  std::wstring MakeText() const;
  void WriteToIni(TConfigFileSection &Section) const;
  void ReadFromIni(const TConfigFileSection &Section);
  boost::shared_ptr<TBaseFuncType> MakeDifFunc();
  std::pair<double,double> GetCurrentRange() const;
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  std::string GetVariable() const {return "x";}
  const Func32::TFunc& GetFunc() const {return Func;}
  const std::wstring& GetText() const {return Text;}
};

class TParFunc : public TBaseFuncType
{
  std::wstring xText,yText;
  Func32::TParamFunc Func;

public:
  TParFunc() {}
  TParFunc(const std::wstring &AxText, const std::wstring &AyText, const Func32::TSymbolList &SymbolList, Func32::TTrigonometry Trig);
  TParFunc(const Func32::TParamFunc &AFunc);

  boost::shared_ptr<TGraphElem> Clone() const {return CloneWithTangents(new TParFunc(*this), this);}
  std::wstring MakeText() const;
  void WriteToIni(TConfigFileSection &Section) const;
  void ReadFromIni(const TConfigFileSection &Section);
  boost::shared_ptr<TBaseFuncType> MakeDifFunc();
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  std::string GetVariable() const {return "t";}
  const Func32::TParamFunc& GetFunc() const {return Func;}
  const std::wstring& GetxText() const {return xText;}
  const std::wstring& GetyText() const {return yText;}
};

class TPolFunc : public TBaseFuncType
{
  std::wstring Text;
  Func32::TPolarFunc Func;

public:
  TPolFunc() {}
  TPolFunc(const std::wstring &AText, const Func32::TSymbolList &SymbolList, Func32::TTrigonometry Trig);

  boost::shared_ptr<TGraphElem> Clone() const {return CloneWithTangents(new TPolFunc(*this), this);}
  std::wstring MakeText() const;
  void WriteToIni(TConfigFileSection &Section) const;
  void ReadFromIni(const TConfigFileSection &Section);
  boost::shared_ptr<TBaseFuncType> MakeDifFunc();
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  std::string GetVariable() const {return "t";}
  const Func32::TPolarFunc& GetFunc() const {return Func;}
  const std::wstring& GetText() const {return Text;}
};

enum TErrorBarType {ebtNone, ebtFixed, ebtRelative, ebtCustom};
struct TPointSeriesPoint
{
  TTextValue x, y;
  TTextValue xError, yError; //Data for error bars; only used if Uncertainty=utCustom

  TPointSeriesPoint() {}
  TPointSeriesPoint(double X, double Y, double XError = 0, double YError = 0)
    : x(X), y(Y), xError(XError), yError(YError) {}
  TPointSeriesPoint(const TData &Data, const std::wstring &X, const std::wstring &Y, const std::wstring &XError = L"", const std::wstring &YError = L"", bool IgnoreErrors=false);
};

enum TInterpolationAlgorithm {iaLinear, iaCubicSpline, iaHalfCosine};
enum TLabelPosition {lpAbove, lpBelow, lpLeft, lpRight, lpAboveLeft, lpAboveRight, lpBelowLeft, lpBelowRight};

struct TPointSeries : public TGraphElem
{
  typedef std::vector<TPointSeriesPoint> TPointList;

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
  TPointList PointList;
  TErrorBarType xErrorBarType, yErrorBarType;
  double xErrorValue, yErrorValue; //Data for error bars; only used if Uncertainty!=utCustom
  void Update();

  TPointSeries();
  std::wstring MakeText() const {return L"";}
  void WriteToIni(TConfigFileSection &Section) const;
  void ReadFromIni(const TConfigFileSection &Section);
  Func32::TDblPoint FindCoord(TPointList::const_iterator Iter, double x) const;
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  double GetXError(unsigned Index) const;
  double GetYError(unsigned Index) const;
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TGraphElem>(new TPointSeries(*this));}
  Func32::TDblPoint GetPoint(unsigned Index) const {return Func32::TDblPoint(PointList[Index].x.Value, PointList[Index].y.Value);}
  unsigned PointCount() const {return PointList.size();}
  TPointList::const_iterator FindPoint(double x) const;
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
  void WriteToIni(TConfigFileSection &Section) const;
  void ReadFromIni(const TConfigFileSection &Section);
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TGraphElem>(new TShade(*this));}
  boost::shared_ptr<TBaseFuncType> ParentFunc() const {return Func.lock();}
  void SetParentFunc(const boost::shared_ptr<TBaseFuncType> &AFunc) {Func = AFunc;}
  void Update();
};

enum TRelationType {rtEquation, rtInequality};
class TRelation : public TGraphElem
{
  std::wstring Text;
  std::wstring ConstraintsText;
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
  TRelation(const std::wstring &AText, const Func32::TSymbolList &SymbolList, TColor AColor, TBrushStyle Style, unsigned ASize, Func32::TTrigonometry Trig);
  TRelation(const TRelation &Relation);
  std::wstring MakeText() const;
  void WriteToIni(TConfigFileSection &Section) const;
  void ReadFromIni(const TConfigFileSection &Section);
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TGraphElem>(new TRelation(*this));}
  void SetConstraints(const std::wstring &AConstraintsText, const Func32::TSymbolList &SymbolList);
  void Update();

  TColor GetColor() const {return Color;}
  TBrushStyle GetBrushStyle() const {return BrushStyle;}
  const std::wstring& GetText() const {return Text;}
  const std::wstring& GetConstraints() const {return ConstraintsText;}
  long double Eval(const std::vector<long double> &Args, Func32::ECalcError &E);
  void ClearCache();
  TRelationType GetRelationType() const {return RelationType;}
  unsigned GetSize() const {return Size;}
};

class TAxesView : public TGraphElem
{
public:
  TAxesView() {SetShowInLegend(false);}
  std::wstring MakeText() const {return L"";}
  void WriteToIni(TConfigFileSection &Section) const;
  void ReadFromIni(const TConfigFileSection &Section);
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TGraphElem>(new TAxesView(*this));}
  int GetVisible() const;
  void ChangeVisible();
};
} //namespace Graph
using namespace Graph;
//---------------------------------------------------------------------------
#endif
