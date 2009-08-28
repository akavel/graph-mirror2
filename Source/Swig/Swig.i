%module Swig
%import "std_wstring.i"

%begin %{
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

namespace Graph
{
enum TAxesStyle {asNone, asCrossed, asBoxed};
enum TLegendPlacement {lpCustom, lpTopRight, lpBottomRight, lpTopLeft, lpBottomLeft};

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
  TVclObject<TFont> NumberFont;
  TVclObject<TFont> LabelFont;
  TVclObject<TFont> LegendFont;
  std::wstring Title;
  TVclObject<TFont> TitleFont;
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
