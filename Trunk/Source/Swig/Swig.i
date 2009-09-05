%module Swig
%import "std_wstring.i"

%typemap(in) Func32::TDblPoint  {
  if(!PyArg_ParseTuple($input, "dd", &($1.x), &($1.y)))
    SWIG_fail;
}

%typemap(out) Func32::TDblPoint {
  $result = Py_BuildValue("dd", $1.x, $1.y);
}

%typemap(out) TFont* {
  $result = PyInt_FromLong((long)$1);
}

%begin %{
//Begin
#include "Graph.h"
#include "Unit1.h"
#include "PythonBind.h"
#undef _DEBUG
#include "Python.h"
#pragma warn -8060
PyObject* SWIG_init2(PyObject*, PyObject*);
namespace Python
{
%}

%runtime %{
  //Runtime
%}

%header %{
  //Header
%}

%wrapper %{
  //Wrapper
%}

%init %{
  //Init
  SWIG_init2(d, m);
  return m;
}
} //namespace Python
using namespace Python;

static PyObject* SWIG_init2(PyObject *d, PyObject *m)
{
%}

%inline %{
static Graph::TAxes* GetAxes() {return &Form1->Data.Axes;}
static void Redraw() {Form1->Redraw();}
%}

typedef unsigned TColor;

namespace Func32
{
enum TTrigonometry {Radian, Degree};
}

namespace Graph
{
enum TAxesStyle {asNone, asCrossed, asBoxed};
enum TLegendPlacement {lpCustom, lpTopRight, lpBottomRight, lpTopLeft, lpBottomLeft};

%nodefault TAxis;
%nodefault TAxes;
struct TAxis
{
  double Min;
  double Max;
  bool LogScl, MultiplyOfPi;
  bool ShowLabel, ShowNumbers, ShowTicks, ShowGrid;
  bool AutoTick, AutoGrid;
  std::wstring Label;
  double AxisCross;
  double TickUnit;
  double GridUnit;
};

enum TAxesArrows
{
  aaNone,
  aaPositiveEnd,
  aaBothEnds
};

enum TNumberPlacement
{
  npCenter,
  npBefore
};

enum TGridStyle
{
  gsLines, gsDots
};

struct TAxes
{
  TAxis xAxis;
  TAxis yAxis;
  TColor AxesColor;
  TColor GridColor;
  TColor BackgroundColor;
  TFont*const NumberFont;
  TFont*const LabelFont;
  TFont*const LegendFont;
  std::wstring Title;
  TFont*const TitleFont;
  bool ShowLegend;
  Func32::TTrigonometry Trigonometry;
  TAxesStyle AxesStyle;
  TLegendPlacement LegendPlacement;
  Func32::TDblPoint LegendPos; //Only valid when LegendPlacement is lpCustom
  unsigned GridSize;
  bool CalcComplex;//When true: Complex numbers are used for graphing
  TAxesArrows AxesArrows;
  TNumberPlacement NumberPlacement;
  TGridStyle GridStyle;
};
}

