# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_Settings', [dirname(__file__)])
        except ImportError:
            import _Settings
            return _Settings
        if fp is not None:
            try:
                _mod = imp.load_module('_Settings', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _Settings = swig_import_helper()
    del swig_import_helper
else:
    import _Settings
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0



def GetAxes() -> "Graph::TAxes *" :
  return _Settings.GetAxes()
GetAxes = _Settings.GetAxes

def SetThreadCount(*args) -> "void" :
  return _Settings.SetThreadCount(*args)
SetThreadCount = _Settings.SetThreadCount
Radian = _Settings.Radian
Degree = _Settings.Degree
asNone = _Settings.asNone
asCrossed = _Settings.asCrossed
asBoxed = _Settings.asBoxed
lpCustom = _Settings.lpCustom
lpTopRight = _Settings.lpTopRight
lpBottomRight = _Settings.lpBottomRight
lpTopLeft = _Settings.lpTopLeft
lpBottomLeft = _Settings.lpBottomLeft
npCenter = _Settings.npCenter
npBefore = _Settings.npBefore
class TAxis(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TAxis, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TAxis, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    __swig_setmethods__["Min"] = _Settings.TAxis_Min_set
    __swig_getmethods__["Min"] = _Settings.TAxis_Min_get
    if _newclass:Min = _swig_property(_Settings.TAxis_Min_get, _Settings.TAxis_Min_set)
    __swig_setmethods__["Max"] = _Settings.TAxis_Max_set
    __swig_getmethods__["Max"] = _Settings.TAxis_Max_get
    if _newclass:Max = _swig_property(_Settings.TAxis_Max_get, _Settings.TAxis_Max_set)
    __swig_setmethods__["LogScl"] = _Settings.TAxis_LogScl_set
    __swig_getmethods__["LogScl"] = _Settings.TAxis_LogScl_get
    if _newclass:LogScl = _swig_property(_Settings.TAxis_LogScl_get, _Settings.TAxis_LogScl_set)
    __swig_setmethods__["MultipleOfPi"] = _Settings.TAxis_MultipleOfPi_set
    __swig_getmethods__["MultipleOfPi"] = _Settings.TAxis_MultipleOfPi_get
    if _newclass:MultipleOfPi = _swig_property(_Settings.TAxis_MultipleOfPi_get, _Settings.TAxis_MultipleOfPi_set)
    __swig_setmethods__["ShowLabel"] = _Settings.TAxis_ShowLabel_set
    __swig_getmethods__["ShowLabel"] = _Settings.TAxis_ShowLabel_get
    if _newclass:ShowLabel = _swig_property(_Settings.TAxis_ShowLabel_get, _Settings.TAxis_ShowLabel_set)
    __swig_setmethods__["ShowNumbers"] = _Settings.TAxis_ShowNumbers_set
    __swig_getmethods__["ShowNumbers"] = _Settings.TAxis_ShowNumbers_get
    if _newclass:ShowNumbers = _swig_property(_Settings.TAxis_ShowNumbers_get, _Settings.TAxis_ShowNumbers_set)
    __swig_setmethods__["ShowTicks"] = _Settings.TAxis_ShowTicks_set
    __swig_getmethods__["ShowTicks"] = _Settings.TAxis_ShowTicks_get
    if _newclass:ShowTicks = _swig_property(_Settings.TAxis_ShowTicks_get, _Settings.TAxis_ShowTicks_set)
    __swig_setmethods__["ShowGrid"] = _Settings.TAxis_ShowGrid_set
    __swig_getmethods__["ShowGrid"] = _Settings.TAxis_ShowGrid_get
    if _newclass:ShowGrid = _swig_property(_Settings.TAxis_ShowGrid_get, _Settings.TAxis_ShowGrid_set)
    __swig_setmethods__["AutoTick"] = _Settings.TAxis_AutoTick_set
    __swig_getmethods__["AutoTick"] = _Settings.TAxis_AutoTick_get
    if _newclass:AutoTick = _swig_property(_Settings.TAxis_AutoTick_get, _Settings.TAxis_AutoTick_set)
    __swig_setmethods__["AutoGrid"] = _Settings.TAxis_AutoGrid_set
    __swig_getmethods__["AutoGrid"] = _Settings.TAxis_AutoGrid_get
    if _newclass:AutoGrid = _swig_property(_Settings.TAxis_AutoGrid_get, _Settings.TAxis_AutoGrid_set)
    __swig_setmethods__["Label"] = _Settings.TAxis_Label_set
    __swig_getmethods__["Label"] = _Settings.TAxis_Label_get
    if _newclass:Label = _swig_property(_Settings.TAxis_Label_get, _Settings.TAxis_Label_set)
    __swig_setmethods__["AxisCross"] = _Settings.TAxis_AxisCross_set
    __swig_getmethods__["AxisCross"] = _Settings.TAxis_AxisCross_get
    if _newclass:AxisCross = _swig_property(_Settings.TAxis_AxisCross_get, _Settings.TAxis_AxisCross_set)
    __swig_setmethods__["TickUnit"] = _Settings.TAxis_TickUnit_set
    __swig_getmethods__["TickUnit"] = _Settings.TAxis_TickUnit_get
    if _newclass:TickUnit = _swig_property(_Settings.TAxis_TickUnit_get, _Settings.TAxis_TickUnit_set)
    __swig_setmethods__["GridUnit"] = _Settings.TAxis_GridUnit_set
    __swig_getmethods__["GridUnit"] = _Settings.TAxis_GridUnit_get
    if _newclass:GridUnit = _swig_property(_Settings.TAxis_GridUnit_get, _Settings.TAxis_GridUnit_set)
    __swig_setmethods__["Visible"] = _Settings.TAxis_Visible_set
    __swig_getmethods__["Visible"] = _Settings.TAxis_Visible_get
    if _newclass:Visible = _swig_property(_Settings.TAxis_Visible_get, _Settings.TAxis_Visible_set)
    __swig_setmethods__["ShowPositiveArrow"] = _Settings.TAxis_ShowPositiveArrow_set
    __swig_getmethods__["ShowPositiveArrow"] = _Settings.TAxis_ShowPositiveArrow_get
    if _newclass:ShowPositiveArrow = _swig_property(_Settings.TAxis_ShowPositiveArrow_get, _Settings.TAxis_ShowPositiveArrow_set)
    __swig_setmethods__["ShowNegativeArrow"] = _Settings.TAxis_ShowNegativeArrow_set
    __swig_getmethods__["ShowNegativeArrow"] = _Settings.TAxis_ShowNegativeArrow_get
    if _newclass:ShowNegativeArrow = _swig_property(_Settings.TAxis_ShowNegativeArrow_get, _Settings.TAxis_ShowNegativeArrow_set)
    __swig_setmethods__["NumberPlacement"] = _Settings.TAxis_NumberPlacement_set
    __swig_getmethods__["NumberPlacement"] = _Settings.TAxis_NumberPlacement_get
    if _newclass:NumberPlacement = _swig_property(_Settings.TAxis_NumberPlacement_get, _Settings.TAxis_NumberPlacement_set)
TAxis_swigregister = _Settings.TAxis_swigregister
TAxis_swigregister(TAxis)

gsLines = _Settings.gsLines
gsDots = _Settings.gsDots
class TAxes(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TAxes, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TAxes, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    __swig_setmethods__["xAxis"] = _Settings.TAxes_xAxis_set
    __swig_getmethods__["xAxis"] = _Settings.TAxes_xAxis_get
    if _newclass:xAxis = _swig_property(_Settings.TAxes_xAxis_get, _Settings.TAxes_xAxis_set)
    __swig_setmethods__["yAxis"] = _Settings.TAxes_yAxis_set
    __swig_getmethods__["yAxis"] = _Settings.TAxes_yAxis_get
    if _newclass:yAxis = _swig_property(_Settings.TAxes_yAxis_get, _Settings.TAxes_yAxis_set)
    __swig_setmethods__["AxesColor"] = _Settings.TAxes_AxesColor_set
    __swig_getmethods__["AxesColor"] = _Settings.TAxes_AxesColor_get
    if _newclass:AxesColor = _swig_property(_Settings.TAxes_AxesColor_get, _Settings.TAxes_AxesColor_set)
    __swig_setmethods__["GridColor"] = _Settings.TAxes_GridColor_set
    __swig_getmethods__["GridColor"] = _Settings.TAxes_GridColor_get
    if _newclass:GridColor = _swig_property(_Settings.TAxes_GridColor_get, _Settings.TAxes_GridColor_set)
    __swig_setmethods__["BackgroundColor"] = _Settings.TAxes_BackgroundColor_set
    __swig_getmethods__["BackgroundColor"] = _Settings.TAxes_BackgroundColor_get
    if _newclass:BackgroundColor = _swig_property(_Settings.TAxes_BackgroundColor_get, _Settings.TAxes_BackgroundColor_set)
    __swig_getmethods__["NumberFont"] = _Settings.TAxes_NumberFont_get
    if _newclass:NumberFont = _swig_property(_Settings.TAxes_NumberFont_get)
    __swig_getmethods__["LabelFont"] = _Settings.TAxes_LabelFont_get
    if _newclass:LabelFont = _swig_property(_Settings.TAxes_LabelFont_get)
    __swig_getmethods__["LegendFont"] = _Settings.TAxes_LegendFont_get
    if _newclass:LegendFont = _swig_property(_Settings.TAxes_LegendFont_get)
    __swig_setmethods__["Title"] = _Settings.TAxes_Title_set
    __swig_getmethods__["Title"] = _Settings.TAxes_Title_get
    if _newclass:Title = _swig_property(_Settings.TAxes_Title_get, _Settings.TAxes_Title_set)
    __swig_getmethods__["TitleFont"] = _Settings.TAxes_TitleFont_get
    if _newclass:TitleFont = _swig_property(_Settings.TAxes_TitleFont_get)
    __swig_setmethods__["ShowLegend"] = _Settings.TAxes_ShowLegend_set
    __swig_getmethods__["ShowLegend"] = _Settings.TAxes_ShowLegend_get
    if _newclass:ShowLegend = _swig_property(_Settings.TAxes_ShowLegend_get, _Settings.TAxes_ShowLegend_set)
    __swig_setmethods__["Trigonometry"] = _Settings.TAxes_Trigonometry_set
    __swig_getmethods__["Trigonometry"] = _Settings.TAxes_Trigonometry_get
    if _newclass:Trigonometry = _swig_property(_Settings.TAxes_Trigonometry_get, _Settings.TAxes_Trigonometry_set)
    __swig_setmethods__["AxesStyle"] = _Settings.TAxes_AxesStyle_set
    __swig_getmethods__["AxesStyle"] = _Settings.TAxes_AxesStyle_get
    if _newclass:AxesStyle = _swig_property(_Settings.TAxes_AxesStyle_get, _Settings.TAxes_AxesStyle_set)
    __swig_setmethods__["LegendPlacement"] = _Settings.TAxes_LegendPlacement_set
    __swig_getmethods__["LegendPlacement"] = _Settings.TAxes_LegendPlacement_get
    if _newclass:LegendPlacement = _swig_property(_Settings.TAxes_LegendPlacement_get, _Settings.TAxes_LegendPlacement_set)
    __swig_setmethods__["LegendPos"] = _Settings.TAxes_LegendPos_set
    __swig_getmethods__["LegendPos"] = _Settings.TAxes_LegendPos_get
    if _newclass:LegendPos = _swig_property(_Settings.TAxes_LegendPos_get, _Settings.TAxes_LegendPos_set)
    __swig_setmethods__["CalcComplex"] = _Settings.TAxes_CalcComplex_set
    __swig_getmethods__["CalcComplex"] = _Settings.TAxes_CalcComplex_get
    if _newclass:CalcComplex = _swig_property(_Settings.TAxes_CalcComplex_get, _Settings.TAxes_CalcComplex_set)
    __swig_setmethods__["GridStyle"] = _Settings.TAxes_GridStyle_set
    __swig_getmethods__["GridStyle"] = _Settings.TAxes_GridStyle_get
    if _newclass:GridStyle = _swig_property(_Settings.TAxes_GridStyle_get, _Settings.TAxes_GridStyle_set)
TAxes_swigregister = _Settings.TAxes_swigregister
TAxes_swigregister(TAxes)

cfReal = _Settings.cfReal
cfRectangular = _Settings.cfRectangular
cfPolar = _Settings.cfPolar
class TProperty(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TProperty, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TProperty, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    __swig_setmethods__["RoundTo"] = _Settings.TProperty_RoundTo_set
    __swig_getmethods__["RoundTo"] = _Settings.TProperty_RoundTo_get
    if _newclass:RoundTo = _swig_property(_Settings.TProperty_RoundTo_get, _Settings.TProperty_RoundTo_set)
    __swig_setmethods__["SavePos"] = _Settings.TProperty_SavePos_set
    __swig_getmethods__["SavePos"] = _Settings.TProperty_SavePos_get
    if _newclass:SavePos = _swig_property(_Settings.TProperty_SavePos_get, _Settings.TProperty_SavePos_set)
    __swig_setmethods__["ComplexFormat"] = _Settings.TProperty_ComplexFormat_set
    __swig_getmethods__["ComplexFormat"] = _Settings.TProperty_ComplexFormat_get
    if _newclass:ComplexFormat = _swig_property(_Settings.TProperty_ComplexFormat_get, _Settings.TProperty_ComplexFormat_set)
    __swig_setmethods__["CheckForUpdate"] = _Settings.TProperty_CheckForUpdate_set
    __swig_getmethods__["CheckForUpdate"] = _Settings.TProperty_CheckForUpdate_get
    if _newclass:CheckForUpdate = _swig_property(_Settings.TProperty_CheckForUpdate_get, _Settings.TProperty_CheckForUpdate_set)
    __swig_setmethods__["DefaultFunction"] = _Settings.TProperty_DefaultFunction_set
    __swig_getmethods__["DefaultFunction"] = _Settings.TProperty_DefaultFunction_get
    if _newclass:DefaultFunction = _swig_property(_Settings.TProperty_DefaultFunction_get, _Settings.TProperty_DefaultFunction_set)
    __swig_setmethods__["DefaultPoint"] = _Settings.TProperty_DefaultPoint_set
    __swig_getmethods__["DefaultPoint"] = _Settings.TProperty_DefaultPoint_get
    if _newclass:DefaultPoint = _swig_property(_Settings.TProperty_DefaultPoint_get, _Settings.TProperty_DefaultPoint_set)
    __swig_setmethods__["DefaultPointLine"] = _Settings.TProperty_DefaultPointLine_set
    __swig_getmethods__["DefaultPointLine"] = _Settings.TProperty_DefaultPointLine_get
    if _newclass:DefaultPointLine = _swig_property(_Settings.TProperty_DefaultPointLine_get, _Settings.TProperty_DefaultPointLine_set)
    __swig_setmethods__["DefaultShade"] = _Settings.TProperty_DefaultShade_set
    __swig_getmethods__["DefaultShade"] = _Settings.TProperty_DefaultShade_get
    if _newclass:DefaultShade = _swig_property(_Settings.TProperty_DefaultShade_get, _Settings.TProperty_DefaultShade_set)
    __swig_setmethods__["DefaultTrendline"] = _Settings.TProperty_DefaultTrendline_set
    __swig_getmethods__["DefaultTrendline"] = _Settings.TProperty_DefaultTrendline_get
    if _newclass:DefaultTrendline = _swig_property(_Settings.TProperty_DefaultTrendline_get, _Settings.TProperty_DefaultTrendline_set)
    __swig_setmethods__["DefaultRelation"] = _Settings.TProperty_DefaultRelation_set
    __swig_getmethods__["DefaultRelation"] = _Settings.TProperty_DefaultRelation_get
    if _newclass:DefaultRelation = _swig_property(_Settings.TProperty_DefaultRelation_get, _Settings.TProperty_DefaultRelation_set)
    __swig_setmethods__["DefaultTangent"] = _Settings.TProperty_DefaultTangent_set
    __swig_getmethods__["DefaultTangent"] = _Settings.TProperty_DefaultTangent_get
    if _newclass:DefaultTangent = _swig_property(_Settings.TProperty_DefaultTangent_get, _Settings.TProperty_DefaultTangent_set)
    __swig_setmethods__["DefaultDif"] = _Settings.TProperty_DefaultDif_set
    __swig_getmethods__["DefaultDif"] = _Settings.TProperty_DefaultDif_get
    if _newclass:DefaultDif = _swig_property(_Settings.TProperty_DefaultDif_get, _Settings.TProperty_DefaultDif_set)
    __swig_setmethods__["DefaultPointLabelFont"] = _Settings.TProperty_DefaultPointLabelFont_set
    __swig_getmethods__["DefaultPointLabelFont"] = _Settings.TProperty_DefaultPointLabelFont_get
    if _newclass:DefaultPointLabelFont = _swig_property(_Settings.TProperty_DefaultPointLabelFont_get, _Settings.TProperty_DefaultPointLabelFont_set)
    __swig_setmethods__["DefaultLabelFont"] = _Settings.TProperty_DefaultLabelFont_set
    __swig_getmethods__["DefaultLabelFont"] = _Settings.TProperty_DefaultLabelFont_get
    if _newclass:DefaultLabelFont = _swig_property(_Settings.TProperty_DefaultLabelFont_get, _Settings.TProperty_DefaultLabelFont_set)
    __swig_setmethods__["ShowTipsAtStartup"] = _Settings.TProperty_ShowTipsAtStartup_set
    __swig_getmethods__["ShowTipsAtStartup"] = _Settings.TProperty_ShowTipsAtStartup_get
    if _newclass:ShowTipsAtStartup = _swig_property(_Settings.TProperty_ShowTipsAtStartup_get, _Settings.TProperty_ShowTipsAtStartup_set)
    __swig_setmethods__["Language"] = _Settings.TProperty_Language_set
    __swig_getmethods__["Language"] = _Settings.TProperty_Language_get
    if _newclass:Language = _swig_property(_Settings.TProperty_Language_get, _Settings.TProperty_Language_set)
    __swig_setmethods__["FontScale"] = _Settings.TProperty_FontScale_set
    __swig_getmethods__["FontScale"] = _Settings.TProperty_FontScale_get
    if _newclass:FontScale = _swig_property(_Settings.TProperty_FontScale_get, _Settings.TProperty_FontScale_set)
    __swig_setmethods__["CustomDecimalSeparator"] = _Settings.TProperty_CustomDecimalSeparator_set
    __swig_getmethods__["CustomDecimalSeparator"] = _Settings.TProperty_CustomDecimalSeparator_get
    if _newclass:CustomDecimalSeparator = _swig_property(_Settings.TProperty_CustomDecimalSeparator_get, _Settings.TProperty_CustomDecimalSeparator_set)
    __swig_setmethods__["DecimalSeparator"] = _Settings.TProperty_DecimalSeparator_set
    __swig_getmethods__["DecimalSeparator"] = _Settings.TProperty_DecimalSeparator_get
    if _newclass:DecimalSeparator = _swig_property(_Settings.TProperty_DecimalSeparator_get, _Settings.TProperty_DecimalSeparator_set)
TProperty_swigregister = _Settings.TProperty_swigregister
TProperty_swigregister(TProperty)

class TGuiFormatSettings(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TGuiFormatSettings, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TGuiFormatSettings, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    __swig_setmethods__["CartesianPointFormat"] = _Settings.TGuiFormatSettings_CartesianPointFormat_set
    __swig_getmethods__["CartesianPointFormat"] = _Settings.TGuiFormatSettings_CartesianPointFormat_get
    if _newclass:CartesianPointFormat = _swig_property(_Settings.TGuiFormatSettings_CartesianPointFormat_get, _Settings.TGuiFormatSettings_CartesianPointFormat_set)
    __swig_setmethods__["DegreePointFormat"] = _Settings.TGuiFormatSettings_DegreePointFormat_set
    __swig_getmethods__["DegreePointFormat"] = _Settings.TGuiFormatSettings_DegreePointFormat_get
    if _newclass:DegreePointFormat = _swig_property(_Settings.TGuiFormatSettings_DegreePointFormat_get, _Settings.TGuiFormatSettings_DegreePointFormat_set)
    __swig_setmethods__["RadianPointFormat"] = _Settings.TGuiFormatSettings_RadianPointFormat_set
    __swig_getmethods__["RadianPointFormat"] = _Settings.TGuiFormatSettings_RadianPointFormat_get
    if _newclass:RadianPointFormat = _swig_property(_Settings.TGuiFormatSettings_RadianPointFormat_get, _Settings.TGuiFormatSettings_RadianPointFormat_set)
TGuiFormatSettings_swigregister = _Settings.TGuiFormatSettings_swigregister
TGuiFormatSettings_swigregister(TGuiFormatSettings)

class TPlotSettings(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TPlotSettings, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TPlotSettings, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    __swig_setmethods__["AxisWidth"] = _Settings.TPlotSettings_AxisWidth_set
    __swig_getmethods__["AxisWidth"] = _Settings.TPlotSettings_AxisWidth_get
    if _newclass:AxisWidth = _swig_property(_Settings.TPlotSettings_AxisWidth_get, _Settings.TPlotSettings_AxisWidth_set)
    __swig_setmethods__["GridWidth"] = _Settings.TPlotSettings_GridWidth_set
    __swig_getmethods__["GridWidth"] = _Settings.TPlotSettings_GridWidth_get
    if _newclass:GridWidth = _swig_property(_Settings.TPlotSettings_GridWidth_get, _Settings.TPlotSettings_GridWidth_set)
    __swig_setmethods__["xNumberDist"] = _Settings.TPlotSettings_xNumberDist_set
    __swig_getmethods__["xNumberDist"] = _Settings.TPlotSettings_xNumberDist_get
    if _newclass:xNumberDist = _swig_property(_Settings.TPlotSettings_xNumberDist_get, _Settings.TPlotSettings_xNumberDist_set)
    __swig_setmethods__["yNumberDist"] = _Settings.TPlotSettings_yNumberDist_set
    __swig_getmethods__["yNumberDist"] = _Settings.TPlotSettings_yNumberDist_get
    if _newclass:yNumberDist = _swig_property(_Settings.TPlotSettings_yNumberDist_get, _Settings.TPlotSettings_yNumberDist_set)
    __swig_setmethods__["TickWidth"] = _Settings.TPlotSettings_TickWidth_set
    __swig_getmethods__["TickWidth"] = _Settings.TPlotSettings_TickWidth_get
    if _newclass:TickWidth = _swig_property(_Settings.TPlotSettings_TickWidth_get, _Settings.TPlotSettings_TickWidth_set)
    __swig_setmethods__["TickLength"] = _Settings.TPlotSettings_TickLength_set
    __swig_getmethods__["TickLength"] = _Settings.TPlotSettings_TickLength_get
    if _newclass:TickLength = _swig_property(_Settings.TPlotSettings_TickLength_get, _Settings.TPlotSettings_TickLength_set)
    __swig_setmethods__["ThreadCount"] = _Settings.TPlotSettings_ThreadCount_set
    __swig_getmethods__["ThreadCount"] = _Settings.TPlotSettings_ThreadCount_get
    if _newclass:ThreadCount = _swig_property(_Settings.TPlotSettings_ThreadCount_get, _Settings.TPlotSettings_ThreadCount_set)
    __swig_setmethods__["SmoothingMode"] = _Settings.TPlotSettings_SmoothingMode_set
    __swig_getmethods__["SmoothingMode"] = _Settings.TPlotSettings_SmoothingMode_get
    if _newclass:SmoothingMode = _swig_property(_Settings.TPlotSettings_SmoothingMode_get, _Settings.TPlotSettings_SmoothingMode_set)
TPlotSettings_swigregister = _Settings.TPlotSettings_swigregister
TPlotSettings_swigregister(TPlotSettings)

class TGuiSettings(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TGuiSettings, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TGuiSettings, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    __swig_setmethods__["MajorZoomIn"] = _Settings.TGuiSettings_MajorZoomIn_set
    __swig_getmethods__["MajorZoomIn"] = _Settings.TGuiSettings_MajorZoomIn_get
    if _newclass:MajorZoomIn = _swig_property(_Settings.TGuiSettings_MajorZoomIn_get, _Settings.TGuiSettings_MajorZoomIn_set)
    __swig_setmethods__["MinorZoomIn"] = _Settings.TGuiSettings_MinorZoomIn_set
    __swig_getmethods__["MinorZoomIn"] = _Settings.TGuiSettings_MinorZoomIn_get
    if _newclass:MinorZoomIn = _swig_property(_Settings.TGuiSettings_MinorZoomIn_get, _Settings.TGuiSettings_MinorZoomIn_set)
    __swig_setmethods__["MajorZoomOut"] = _Settings.TGuiSettings_MajorZoomOut_set
    __swig_getmethods__["MajorZoomOut"] = _Settings.TGuiSettings_MajorZoomOut_get
    if _newclass:MajorZoomOut = _swig_property(_Settings.TGuiSettings_MajorZoomOut_get, _Settings.TGuiSettings_MajorZoomOut_set)
    __swig_setmethods__["MinorZoomOut"] = _Settings.TGuiSettings_MinorZoomOut_set
    __swig_getmethods__["MinorZoomOut"] = _Settings.TGuiSettings_MinorZoomOut_get
    if _newclass:MinorZoomOut = _swig_property(_Settings.TGuiSettings_MinorZoomOut_get, _Settings.TGuiSettings_MinorZoomOut_set)
    __swig_setmethods__["MajorStepSize"] = _Settings.TGuiSettings_MajorStepSize_set
    __swig_getmethods__["MajorStepSize"] = _Settings.TGuiSettings_MajorStepSize_get
    if _newclass:MajorStepSize = _swig_property(_Settings.TGuiSettings_MajorStepSize_get, _Settings.TGuiSettings_MajorStepSize_set)
    __swig_setmethods__["MinorStepSize"] = _Settings.TGuiSettings_MinorStepSize_set
    __swig_getmethods__["MinorStepSize"] = _Settings.TGuiSettings_MinorStepSize_get
    if _newclass:MinorStepSize = _swig_property(_Settings.TGuiSettings_MinorStepSize_get, _Settings.TGuiSettings_MinorStepSize_set)
    __swig_setmethods__["MouseZoomIn"] = _Settings.TGuiSettings_MouseZoomIn_set
    __swig_getmethods__["MouseZoomIn"] = _Settings.TGuiSettings_MouseZoomIn_get
    if _newclass:MouseZoomIn = _swig_property(_Settings.TGuiSettings_MouseZoomIn_get, _Settings.TGuiSettings_MouseZoomIn_set)
    __swig_setmethods__["MouseZoomOut"] = _Settings.TGuiSettings_MouseZoomOut_set
    __swig_getmethods__["MouseZoomOut"] = _Settings.TGuiSettings_MouseZoomOut_get
    if _newclass:MouseZoomOut = _swig_property(_Settings.TGuiSettings_MouseZoomOut_get, _Settings.TGuiSettings_MouseZoomOut_set)
TGuiSettings_swigregister = _Settings.TGuiSettings_swigregister
TGuiSettings_swigregister(TGuiSettings)

import vcl
TAxes.NumberFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_NumberFont_get(self), owned=False))
TAxes.LabelFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_LabelFont_get(self), owned=False))
TAxes.LegendFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_LegenFont_get(self), owned=False))
TAxes.TitleFont = property(lambda self: vcl.TObject(handle=_Settings.TAxes_TitleFont_get(self), owned=False))
TPlotSettings.__swig_setmethods__["ThreadCount"] = lambda self, count: SetThreadCount(self, count)

# This file is compatible with both classic and new-style classes.

cvar = _Settings.cvar
Property = cvar.Property
GuiFormatSettings = cvar.GuiFormatSettings
PlotSettings = cvar.PlotSettings
GuiSettings = cvar.GuiSettings

