%module Settings
%include "std_wstring.i"
%include "attribute.i"
%import "Types.i"

%begin %{
#include "Graph.h"
#include "Unit1.h"
#include "Python.hpp"
#define WRAP_PYOBJECTS
#include "PythonBind.h"
#include "PyGraph.h"
#include "PyVclObject.h"
#pragma warn -8060
%}

%inline %{
static Graph::TAxes* GetAxes() {return &Form1->Data.Axes;}
static void SetThreadCount(TPlotSettings &Settings, int Count) {if(Count >= 1) Form1->Draw.SetThreadCount(Count), Settings.ThreadCount = Count;}
%}

namespace Func32
{
enum TTrigonometry {Radian, Degree};
}

namespace Graph
{
enum TAxesStyle {asNone, asCrossed, asBoxed};
enum TLegendPlacement {lpCustom, lpTopRight, lpBottomRight, lpTopLeft, lpBottomLeft};
enum TNumberPlacement {npCenter, npBefore};
}

%nodefault TAxis;
%nodefault TAxes;
struct TAxis
{
  double Min;
  double Max;
  bool LogScl, MultipleOfPi;
  bool ShowLabel, ShowNumbers, ShowTicks, ShowGrid;
  bool AutoTick, AutoGrid;
  std::wstring Label;
  double AxisCross;
  double TickUnit;
  double GridUnit;
  bool Visible;
  bool ShowPositiveArrow;
  bool ShowNegativeArrow;
  TNumberPlacement NumberPlacement;
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
  bool CalcComplex;//When true: Complex numbers are used for graphing
  TGridStyle GridStyle;
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
  std::wstring Language;
  int FontScale; //Window scale in percent
  bool CustomDecimalSeparator; //Default system decimal separator is used when false
  wchar_t DecimalSeparator;    //Decimal separator used when exporting files
};

%nodefault TGuiFormatSettings;
struct TGuiFormatSettings
{
  std::wstring CartesianPointFormat;
  std::wstring DegreePointFormat;
  std::wstring RadianPointFormat;
};

%nodefault TPlotSettings;
struct TPlotSettings
{
  int AxisWidth;
  int GridWidth;
  int xNumberDist;
  int yNumberDist;
  int TickWidth;
  int TickLength;
  int ThreadCount;
  int SmoothingMode;
};

%nodefault TGuiSettings;
struct TGuiSettings
{
  double MajorZoomIn;
  double MinorZoomIn;
  double MajorZoomOut;
  double MinorZoomOut;
  double MajorStepSize;
  double MinorStepSize;
  double MouseZoomIn;
  double MouseZoomOut;
  TFont*const Font;
};

const TProperty Property;
const TGuiFormatSettings GuiFormatSettings;
const TPlotSettings PlotSettings;
const TGuiSettings GuiSettings;

%pythoncode
{
  import vcl
  TAxes.NumberFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_NumberFont_get(self), owned=False))
  TAxes.LabelFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_LabelFont_get(self), owned=False))
  TAxes.LegendFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_LegenFont_get(self), owned=False))
  TAxes.TitleFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_TitleFont_get(self), owned=False))
  TGuiSettings.Font = property(lambda self: vcl.TObject(handle=_Settings.TGuiSettinga_Font_get(self), owned=False))
  TPlotSettings.__swig_setmethods__["ThreadCount"] = lambda self, count: SetThreadCount(self, count)
}
