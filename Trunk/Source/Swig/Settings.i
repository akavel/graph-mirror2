%module Settings
%import "std_wstring.i"
%import "Types.i"

%begin %{
#include "Graph.h"
#include "Unit1.h"
#undef _DEBUG
#include <Python.h>
#define WRAP_PYOBJECTS
#include "PythonBind.h"
#pragma warn -8060
%}

%inline %{
static Graph::TAxes* GetAxes() {return &Form1->Data.Axes;}
%}

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

%nodefault TDefaultData;
struct TDefaultData
{
  unsigned Style;
  TColor Color;
  unsigned Size;
};

enum TComplexFormat {cfReal,cfRectangular,cfPolar};

%nodefault TProperty;
struct TProperty
{
  int RoundTo; //decimals to round to
  bool SavePos;//True is the windows should have the same size at start as at last exit
  TComplexFormat ComplexFormat;//Format then complex numbers are shown
  bool CheckForUpdate;
  TDefaultData DefaultFunction, DefaultPoint, DefaultPointLine, DefaultShade, DefaultTrendline, DefaultRelation, DefaultTangent, DefaultDif;
  TFont* DefaultPointLabelFont, *DefaultLabelFont;
  bool ShowTipsAtStartup;
  unsigned NextTip;
  std::wstring Language;
  int FontScale; //Window scale in percent
  bool CustomDecimalSeparator; //Default system decimal separator is used when false
  wchar_t DecimalSeparator;    //Decimal separator used when exporting files
};
const TProperty Property;
} //namespace Graph

%pythoncode
{
  import vcl
  TAxes.NumberFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_NumberFont_get(self), owned=False))
  TAxes.LabelFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_LabelFont_get(self), owned=False))
  TAxes.LegendFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_LegenFont_get(self), owned=False))
  TAxes.TitleFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_TitleFont_get(self), owned=False))
}
