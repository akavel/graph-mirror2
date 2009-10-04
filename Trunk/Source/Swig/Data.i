%module Data

%include "stl.i"
%include "std_string.i"
%include "std_wstring.i"
%include "boost_shared_ptr.i"
%include "attribute.i"
%include "Types.i"

%template(PointVector) std::vector<TPoint>;
%template(CoordSetVector) std::vector<Func32::TCoordSet>;
%template(UnsignedVector) std::vector<unsigned>;
%template(StringMap) std::map<std::wstring,std::wstring>;

%begin %{
#include "Graph.h"
#include "Unit1.h"
#undef _DEBUG
#include <Python.h>
#define WRAP_PYOBJECTS
#include "PythonBind.h"
#pragma warn -8060
%}

%{
PyObject* DownCastSharedPtr(const boost::shared_ptr<TGraphElem> &Elem)
{
  if(!Elem)
    return SWIG_Py_Void();
  if(boost::shared_ptr<TStdFunc> StdFunc = boost::dynamic_pointer_cast<TStdFunc>(Elem))
    return SWIG_NewPointerObj(new boost::shared_ptr<TStdFunc>(StdFunc), SWIGTYPE_p_boost__shared_ptrT_TStdFunc_t, SWIG_POINTER_OWN |  0 );
  if(boost::shared_ptr<TParFunc> ParFunc = boost::dynamic_pointer_cast<TParFunc>(Elem))
    return SWIG_NewPointerObj(new boost::shared_ptr<TParFunc>(ParFunc), SWIGTYPE_p_boost__shared_ptrT_TParFunc_t, SWIG_POINTER_OWN |  0 );
  if(boost::shared_ptr<TPolFunc> PolFunc = boost::dynamic_pointer_cast<TPolFunc>(Elem))
    return SWIG_NewPointerObj(new boost::shared_ptr<TPolFunc>(PolFunc), SWIGTYPE_p_boost__shared_ptrT_TPolFunc_t, SWIG_POINTER_OWN |  0 );
  if(boost::shared_ptr<TTan> Tan = boost::dynamic_pointer_cast<TTan>(Elem))
    return SWIG_NewPointerObj(new boost::shared_ptr<TTan>(Tan), SWIGTYPE_p_boost__shared_ptrT_TTan_t, SWIG_POINTER_OWN |  0 );
  if(boost::shared_ptr<TPointSeries> PointSeries = boost::dynamic_pointer_cast<TPointSeries>(Elem))
    return SWIG_NewPointerObj(new boost::shared_ptr<TPointSeries>(PointSeries), SWIGTYPE_p_boost__shared_ptrT_TPointSeries_t, SWIG_POINTER_OWN |  0 );
  if(boost::shared_ptr<TTextLabel> Label = boost::dynamic_pointer_cast<TTextLabel>(Elem))
    return SWIG_NewPointerObj(new boost::shared_ptr<TTextLabel>(Label), SWIGTYPE_p_boost__shared_ptrT_TTextLabel_t, SWIG_POINTER_OWN |  0 );
  if(boost::shared_ptr<TShade> Shade = boost::dynamic_pointer_cast<TShade>(Elem))
    return SWIG_NewPointerObj(new boost::shared_ptr<TShade>(Shade), SWIGTYPE_p_boost__shared_ptrT_TShade_t, SWIG_POINTER_OWN |  0 );
  if(boost::shared_ptr<TRelation> Relation = boost::dynamic_pointer_cast<TRelation>(Elem))
    return SWIG_NewPointerObj(new boost::shared_ptr<TRelation>(Relation), SWIGTYPE_p_boost__shared_ptrT_TRelation_t, SWIG_POINTER_OWN |  0 );
  if(boost::shared_ptr<TAxesView> AxesView = boost::dynamic_pointer_cast<TAxesView>(Elem))
    return SWIG_NewPointerObj(new boost::shared_ptr<TAxesView>(AxesView), SWIGTYPE_p_boost__shared_ptrT_TAxesView_t, SWIG_POINTER_OWN |  0 );
  return SWIG_NewPointerObj(new boost::shared_ptr< TGraphElem >(Elem), SWIGTYPE_p_boost__shared_ptrT_TGraphElem_t, SWIG_POINTER_OWN |  0 );
}
%}

typedef boost::shared_ptr<class TGraphElem> TGraphElemPtr;

%inline %{
static TGraphElemPtr Selected() {return Form1->GetGraphElem(Form1->TreeView->Selected);}
static void AbortUpdate() {Form1->Data.AbortUpdate();}
static void Redraw() {Form1->Redraw();}
static boost::shared_ptr<TStdFunc> CreateStdFunc(const std::wstring &Text) {return boost::shared_ptr<TStdFunc>(new TStdFunc(Text, Form1->Data.CustomFunctions.SymbolList, Form1->Data.Axes.Trigonometry));}
static boost::shared_ptr<TParFunc> CreateParFunc(const std::wstring &xText, const std::wstring &yText) {return boost::shared_ptr<TParFunc>(new TParFunc(xText, yText, Form1->Data.CustomFunctions.SymbolList, Form1->Data.Axes.Trigonometry));}
static boost::shared_ptr<TPolFunc> CreatePolFunc(const std::wstring &Text) {return boost::shared_ptr<TPolFunc>(new TPolFunc(Text, Form1->Data.CustomFunctions.SymbolList, Form1->Data.Axes.Trigonometry));}

static unsigned ChildCount(const TGraphElemPtr &Elem) {return Elem->ChildCount();}
static TGraphElemPtr GetChild(const TGraphElemPtr &Elem, unsigned Index) {return Elem->GetChild(Index);}
static void RemoveChild(const TGraphElemPtr &Elem, unsigned Index) {Elem->RemoveChild(Index); Form1->UpdateTreeView();}
static void InsertChild(const TGraphElemPtr &Elem, const TGraphElemPtr &Child, int Index) {Elem->InsertChild(Child, Index); Form1->UpdateTreeView();}
static void ReplaceChild(const TGraphElemPtr &Elem, unsigned Index, const TGraphElemPtr &Child) {Elem->ReplaceChild(Index, Child); Form1->UpdateTreeView();}
static bool CompareElem(const TGraphElemPtr &E1, const TGraphElemPtr &E2) {return E1.get() == E2.get();}
static std::map<std::wstring,std::wstring>& GetPluginData() {return Form1->Data.PluginData;}
static const TGraphElemPtr& GetTopElem() {return Form1->Data.GetTopElem();}
%}

SWIG_SHARED_PTR(TGraphElem, TGraphElem)
SWIG_SHARED_PTR_DERIVED(TBaseFuncType, TGraphElem, TBaseFuncType)
SWIG_SHARED_PTR_DERIVED(TStdFunc, TBaseFuncType, TStdFunc)
SWIG_SHARED_PTR_DERIVED(TParFunc, TBaseFuncType, TParFunc)
SWIG_SHARED_PTR_DERIVED(TPolFunc, TBaseFuncType, TPolFunc)
SWIG_SHARED_PTR_DERIVED(TTan, TBaseFuncType, TTan)
SWIG_SHARED_PTR_DERIVED(TPointSeries, TGraphElem, TPointSeries)
SWIG_SHARED_PTR_DERIVED(TTextLabel, TGraphElem, TTextLabel)
SWIG_SHARED_PTR_DERIVED(TShade, TGraphElem, TShade)
SWIG_SHARED_PTR_DERIVED(TRelation, TGraphElem, TRelation)
SWIG_SHARED_PTR_DERIVED(TAxesView, TGraphElem, TAxesView)

%nodefaultctor TGraphElem;
%attribute(TGraphElem, int, Visible, GetVisible, SetVisible);
%attribute(TGraphElem, bool, ShowInLegend, GetShowInLegend, SetShowInLegend);
%attributestring(TGraphElem, std::wstring, LegendText, GetLegendText, SetLegendText);
%attributestring(TGraphElem, TGraphElemPtr, Parent, GetParent);
%rename(_PluginData) PluginData;
class TGraphElem
{
public:
  std::map<std::wstring,std::wstring> PluginData;
  std::wstring MakeLegendText() const;
  std::wstring MakeText() const;
};

%extend TGraphElem
{
  int ThisPtr() const {return reinterpret_cast<int>(self);}
  %pythoncode %{
    def __eq__(self, rhs):
      if not isinstance(rhs, TGraphElem): return False
      return self.ThisPtr() == rhs.ThisPtr()
  %}
}


enum TDrawType {dtAuto, dtDots, dtLines};
typedef unsigned TPenStyle;

%nodefaultctor TBaseFuncType;
%attributeval(TBaseFuncType, %arg(std::pair<double,double>), CurrentRange, GetCurrentRange);
%attributestring(TBaseFuncType, TTextValue, Steps, GetSteps, SetSteps);
%attributestring(TBaseFuncType, std::wstring, Variable, GetVariable);
class TBaseFuncType : public TGraphElem
{
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

  boost::shared_ptr<TBaseFuncType> MakeDifFunc();
  Func32::TCoord<long double> Eval(long double t);
  long double CalcArea(long double From, long double To) const;
};

%nodefaultctor TStdhElem;
%attributestring(TStdFunc, std::wstring, Text, GetText);
class TStdFunc : public TBaseFuncType
{
};

%nodefaultctor TParFunc;
%attributestring(TParFunc, std::wstring, xText, GetxText);
%attributestring(TParFunc, std::wstring, yText, GetyText);
class TParFunc : public TBaseFuncType
{
};

%nodefaultctor TPolFunc;
%attributestring(TPolFunc, std::wstring, Text, GetText);
class TPolFunc : public TBaseFuncType
{
};

enum TTangentType {ttTangent, ttNormal};
%attribute(TTan, bool, Valid, IsValid);
class TTan : public TBaseFuncType
{
public:
  TTextValue t;
  TTangentType TangentType;
};

%nodefaultctor TPointSeries;
class TPointSeries : public TGraphElem
{
};

%nodefaultctor TTextLabel;
%attribute(TTextLabel, TRect, Rect, GetRect);
%attribute(TTextLabel, TTextValue, xPos, GetXPos);
%attribute(TTextLabel, TTextValue, yPos, GetYPos);
%attributestring(TTextLabel, std::string, Text, GetText);
%attribute(TTextLabel, TColor, BackgroundColor, GetBackgroundColor);
%attribute(TTextLabel, TLabelPlacement, Placement, GetPlacement);
%attribute(TTextLabel, unsigned, Rotation, GetRotation);
class TTextLabel : public TGraphElem
{
public:
  TTextLabel(const std::string &Str, TLabelPlacement Placement, const TTextValue &AxPos, const TTextValue &AyPos, TColor Color, unsigned ARotation);
  void Scale(double xSizeMul, double ySizeMul);
  TMetafile* GetImage() const {return Metafile;}
};

%nodefaultctor TShade;
class TShade : public TGraphElem
{
};

enum TRelationType {rtEquation, rtInequality};

%nodefaultctor TRelation;
%attribute(TRelation, TColor, Color, GetColor);
%attribute(TRelation, TBrushStyle, BrushStyle, GetBrushStyle);
%attribute(TRelation, TRelationType, RelationType, GetRelationType);
%attributestring(TRelation, std::wstring, Text, GetText);
%attributestring(TRelation, std::wstring, Constraints, GetConstraints);
%attribute(TRelation, unsigned, Size, GetSize);
class TRelation : public TGraphElem
{
public:
  TRelation(const std::wstring &AText, const Func32::TSymbolList &SymbolList, TColor AColor, TBrushStyle Style, unsigned ASize, Func32::TTrigonometry Trig);
  void SetConstraints(const std::wstring &AConstraintsText, const Func32::TSymbolList &SymbolList);

  long double Eval(const std::vector<long double> &Args, Func32::ECalcError &E);
};

%nodefaultctor TAxesView;
class TAxesView : public TGraphElem
{
};

%{
  template <>  struct swig::traits_from<TPoint > {
    static PyObject *from(const TPoint& val) {
      return Py_BuildValue("ii", val.x, val.y);
    }
  };
%}

%pythoncode
{
  TStdFunc.__repr__ = lambda self: '%s("%s")' % (self.__class__.__name__, self.MakeText())
  TParFunc.__repr__ = lambda self: '%s("%s")' % (self.__class__.__name__, self.MakeText())
  TPolFunc.__repr__ = lambda self: '%s("%s")' % (self.__class__.__name__, self.MakeText())
  TTan.__repr__ = lambda self: '%s("%s")' % (self.__class__.__name__, self.MakeText())
  TPointSeries.__repr__ = lambda self: '%s("%s")' % (self.__class__.__name__, self.MakeText())
  TRelation.__repr__ = lambda self: '%s("%s")' % (self.__class__.__name__, self.MakeText())
  TTextLabel.__repr__ = lambda self: '%s("%s")' % (self.__class__.__name__, self.MakeText())
  TAxesView.__repr__ = lambda self: '%s("%s")' % (self.__class__.__name__, self.MakeText())
}
