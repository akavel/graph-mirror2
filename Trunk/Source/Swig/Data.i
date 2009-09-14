%module Data

%include "std_wstring.i"
%include "std_vector.i"
%include "boost_shared_ptr.i"
%include "attribute.i"
%include "Types.i"

%template(PointVector) std::vector<TPoint>;
%template(IntVector) std::vector<int>;

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

%inline %{
static const boost::shared_ptr<TGraphElem>& Selected() {return Form1->GetGraphElem(Form1->TreeView->Selected);}
static void Redraw() {Form1->Redraw();}
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

%nodefault TGraphElem;
%attribute(TGraphElem, int, Visible, GetVisible, SetVisible);
%attribute(TGraphElem, bool, ShowInLegend, GetShowInLegend, SetShowInLegend);
%attributestring(TGraphElem, std::wstring, LegendText, GetLegendText, SetLegendText);
class TGraphElem
{
public:
  virtual std::wstring MakeLegendText() const;
  virtual std::wstring MakeText() const = 0;
};

enum TDrawType {dtAuto, dtDots, dtLines};

%nodefault TBaseFuncType;
%attributestring(TBaseFuncType, %arg(std::pair<double,double>), CurrentRange, GetCurrentRange);
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

  virtual boost::shared_ptr<TBaseFuncType> MakeDifFunc() =0;
  virtual std::pair<double,double> GetCurrentRange() const;
  virtual const TTextValue& GetSteps() const;
  void SetSteps(const TTextValue &Value) {Steps = Value;}
  virtual std::string GetVariable() const;
  Func32::TBaseFunc& GetFunc();
  Func32::TCoord<long double> Eval(long double t);
  virtual long double CalcArea(long double From, long double To) const;
};

class TStdFunc : public TBaseFuncType
{
};

class TParFunc : public TBaseFuncType
{
};

class TPolFunc : public TBaseFuncType
{
};

class TTan : public TBaseFuncType
{
};

class TPointSeries : public TGraphElem
{
};

class TTextLabel : public TGraphElem
{
};

class TShade : public TGraphElem
{
};

class TRelation : public TGraphElem
{
};

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
