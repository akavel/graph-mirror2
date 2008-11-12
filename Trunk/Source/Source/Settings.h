/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef SettingsH
#define SettingsH
//---------------------------------------------------------------------------
//Forward declare
namespace Registry
{
  class TRegistry;
}

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

  TAxis();
  void WriteToIni(TConfigFileSection &IniFile, const std::wstring &Prefix) const;
  void ReadFromIni(const TConfigFileSection &IniFile, const std::wstring &Prefix);
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
  bool ZoomSquare;
  TAxesArrows AxesArrows;
  TNumberPlacement NumberPlacement;
  TGridStyle GridStyle;

  TAxes();
  void WriteToIni(TConfigFileSection &IniFile) const;
  void ReadFromIni(const TConfigFileSection &IniFile);
  void HandleZoomSquare(double xyScale);
};

enum TComplexFormat {cfReal,cfRectangular,cfPolar};
enum TFunctionType {ftStdFunc, ftParFunc, ftPolFunc};

struct TDefaultData
{
  unsigned Style;
  TColor Color;
  unsigned Size;
  std::wstring ToString() const;
  void FromString(const std::wstring &Str, unsigned AStyle, TColor AColor, unsigned ASize);
  void Set(unsigned AStyle, TColor AColor, unsigned ASize);
};

struct TProperty
{
  int RoundTo; //decimals to round to
  bool SavePos;//True is the windows should have the same size at start as at last exit
  TComplexFormat ComplexFormat;//Format then complex numbers are shown
  bool CheckForUpdate;
  TDefaultData DefaultFunction, DefaultPoint, DefaultPointLine, DefaultShade, DefaultTrendline, DefaultRelation, DefaultTangent, DefaultDif;
  TVclObject<TFont> DefaultPointLabelFont, DefaultLabelFont;
  bool ShowTipsAtStartup;
  unsigned NextTip;
  AnsiString Language;
  int FontScale; //Window scale in percent

  TProperty();
  void Read(const class TConfigRegistry &Registry);
  void Write(class TConfigRegistry &Registry);
};

enum TCustomFunctionErrorCode
{
  cfeEmptyName,           //!< Empty name string
  cfeInvalidName,         //!< Invalid character in name: ErrorPos is the index in the string
  cfeEmptyArg,            //!< Empty argument name
  cfeInvalidArg,          //!< Invalid character in argument: ErrorPos is the index in the string
  cfeEndParMissing,       //!< No end parenthesis found: ErrorPos is the index in the name string
  cfeCharAfterEndPar,     //!< Character found after end parenthesis: ErrorPos is the index in the name string
  cfeDoubleDefinedSymbol, //!< Symbol has been defined more than once
  cfeParseError,          //!< Error parsing String: ErrorPos is the index; ParseError contains actual error information
  cfeSymbolUndefined      //!< The requested symbol was not found
};

struct ECustomFunctionError
{
  TCustomFunctionErrorCode ErrorCode;
  int ErrorPos;
  std::string Text;
  Func32::EParseError ParseError;

  ECustomFunctionError(TCustomFunctionErrorCode AErrorCode, int AErrorPos = 0)
    : ErrorCode(AErrorCode), ErrorPos(AErrorPos) {}
  ECustomFunctionError(TCustomFunctionErrorCode AErrorCode, int AErrorPos, const std::string &AText)
    : ErrorCode(AErrorCode), ErrorPos(AErrorPos), Text(AText) {}
  ECustomFunctionError(TCustomFunctionErrorCode AErrorCode, int AErrorPos, const Func32::EParseError &AParseError)
    : ErrorCode(AErrorCode), ErrorPos(AErrorPos), ParseError(AParseError) {}
};

struct TCustomFunction
{
  std::string Name;
  std::string Text;
  std::vector<std::string> Arguments;
  TCustomFunction(const std::string &Str, const std::string &AText);
  TCustomFunction(const std::string &AName, const std::vector<std::string> &AArguments, const std::string &AText)
    : Name(AName), Arguments(AArguments), Text(AText) {}
  std::string GetName() const;
  static std::string CheckAndTrimName(const std::string &Str, unsigned Offset);
};

class TCustomFunctions
{
  std::vector<TCustomFunction> Functions;
  typedef std::vector<TCustomFunction>::iterator TIterator;
  const TData &Data;

public:
  typedef std::vector<TCustomFunction>::const_iterator TConstIterator;
  Func32::TSymbolList SymbolList;
  Func32::TSymbolList GlobalSymbolList;

  TCustomFunctions(const TData &AData) : Data(AData) {}
  void Add(const std::string &Str, const std::string &Value);
  void Add(const std::string &Name, const Func32::TArgType &Args, const std::string &Text);
  void Replace(const std::string &Name, const std::string &Value);
  void Replace(const std::string &Name, long double Value);
  void Delete(const std::string &Name);
  const TCustomFunction& GetValue(const std::string &Name) const;
  void Update();
  void Clear();
  void Swap(TCustomFunctions &Other) {Functions.swap(Other.Functions); SymbolList.Swap(Other.SymbolList);}
  void WriteToIni(TConfigFileSection &IniFile) const;
  void ReadFromIni(const TConfigFileSection &IniFile);
  TConstIterator Begin() const {return Functions.begin();}
  TConstIterator End() const {return Functions.end();}
};

struct TAnimationConstant
{
  std::string Min;
  std::string Max;
  std::string Step;
};

struct TAnimationInfo
{
  std::string Constant;
  std::map<std::string, TAnimationConstant> ConstantList;
  unsigned Width;
  unsigned Height;
  double FramesPerSecond;

  TAnimationInfo() : Width(0), Height(0), FramesPerSecond(1) {}
  void WriteToIni(TConfigFileSection &IniFile) const;
  void ReadFromIni(const TConfigFileSection &IniFile);
  void Clear();
};

extern TProperty Property;

#endif
