/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "PyGraph.h"
#include "Unit1.h"
#include "Unit22.h"
#include "PythonBind.h"
#include "VersionInfo.h"
#include "Unit18.h"
#include <fstream>
#include "IThread.h"
#include "Images.h"
#include <cfloat>
#include <boost/variant/get.hpp>
#undef _DEBUG
#include <python.h>
#include "PyVcl.h"
#include "ConfigRegistry.h"
#include "ExtColorBox.h"
#include "PyVclObject.h"
//---------------------------------------------------------------------------
PyObject* DownCastSharedPtr(const boost::shared_ptr<TGraphElem> &Elem);
namespace Python
{
PyObject *PyEFuncError = NULL;
PyObject *PyEGraphError = NULL;
//---------------------------------------------------------------------------
bool ExecutePythonCommand(const String &Command)
{
  TLockGIL Dummy;

  PyObject *Module = PyImport_ImportModule("code");
  bool Result = true;
  if(Module)
  {
    PyObject *Function = PyObject_GetAttrString(Module, "compile_command");
    if(Function)
    {
      PyObject *Code = PyObject_CallFunction(Function, "us", Command.c_str(), "<console>");
      if(Code == NULL)
      {
        PyObject *Type, *Value, *Traceback;
	      PyErr_Fetch(&Type, &Value, &Traceback);
        PyErr_Restore(Type, Value, NULL);
        Py_XDECREF(Traceback);
        PyErr_Print();
      }
      else if(Code == Py_None)
        Result = false;
      else
      {
	      PyObject *MainModule = PyImport_AddModule("__main__");
	      if(MainModule)
        {
      	  PyObject *Dict = PyModule_GetDict(MainModule);
          PyObject *Temp = PyEval_EvalCode((PyCodeObject*)Code, Dict, Dict);
          if(Temp == NULL)
            PyErr_Print();
          Py_XDECREF(Temp);
        }
      }
      Py_XDECREF(Code);
      Py_DECREF(Function);
    }
    Py_DECREF(Module);
  }

  return Result;
}
//---------------------------------------------------------------------------
static int WriteToConsole(int Arg)
{
  std::pair<String, TColor> *Pair = (std::pair<String, TColor>*)Arg;
  if(Form22)
    Form22->WriteText(Pair->first, Pair->second);
  delete Pair;
  return 0;
}
//---------------------------------------------------------------------------
static PyObject* PluginWriteToConsole(PyObject *Self, PyObject *Args)
{
  const wchar_t *Str;
  TColor Color = clBlack;
  if(!PyArg_ParseTuple(Args, "u|i", &Str, &Color))
    return NULL;

  Py_BEGIN_ALLOW_THREADS

  if(GetCurrentThreadId() != MainThreadID)
  {
    std::pair<String,TColor> *Pair = new std::pair<String, TColor>(Str, Color);
		PostMessage(Form1->Handle, WM_USER+1, (DWORD)WriteToConsole, (DWORD)Pair);
  }
  else if(Form22)
    Form22->WriteText(Str, Color);

  static std::ofstream Log((ExtractFileDir(Application->ExeName) + "\\PyGraph.log").c_str());
  if(Log)
    Log << Str;

  Py_END_ALLOW_THREADS
  //This creates problems when used from OLE
//  Application->ProcessMessages();
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginInputQuery(PyObject *Self, PyObject *Args)
{
  const wchar_t *Caption = L"Python input";
  const wchar_t *Prompt = L"";
  if(!PyArg_ParseTuple(Args, "|uu", &Caption, &Prompt))
    return NULL;
  String Value;
  if(InputQuery(Caption, Prompt, Value))
    return PyUnicode_FromWideChar(Value.c_str(), Value.Length());

  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
class TPluginFunc : public Func32::TBaseCustomFunc
{
  PyObject *Func;
  unsigned Arguments;
public:
  TPluginFunc(PyObject *AFunc, unsigned AArguments) : Func(AFunc), Arguments(AArguments) {Py_INCREF(Func);}
  ~TPluginFunc() {Py_XDECREF(Func);}
  unsigned ArgumentCount() const {return Arguments;}
  long double Call(const long double *Args, Func32::TTrigonometry Trig, Func32::TErrorCode &ErrorCode, std::wstring &ErrorStr) const
  {
    return CallCustomFunction(Args, Trig, ErrorCode, ErrorStr);
  }
  Func32::TComplex Call(const Func32::TComplex *Args, Func32::TTrigonometry Trig, Func32::TErrorCode &ErrorCode, std::wstring &ErrorStr) const
  {
    return CallCustomFunction(Args, Trig, ErrorCode, ErrorStr);
  }
  template<typename T>
  T TPluginFunc::CallCustomFunction(const T *Args, Func32::TTrigonometry Trig, Func32::TErrorCode &ErrorCode, std::wstring &ErrorStr) const;
  PyObject* GetFunc() {Py_INCREF(Func); return Func;}
};
//---------------------------------------------------------------------------
template<typename T>
T TPluginFunc::CallCustomFunction(const T *Args, Func32::TTrigonometry Trig, Func32::TErrorCode &ErrorCode, std::wstring &ErrorStr) const
{
  TLockGIL Dummy;
  PyObject *Tuple = PyTuple_New(Arguments);
  for(unsigned I = 0; I < Arguments; I++)
    PyTuple_SET_ITEM(Tuple, I, ToPyObject(Args[I]));
  PyObject *CallResult = PyObject_CallObject(Func, Tuple);
  T Result = T();
  if(CallResult == Py_None)
    ErrorCode = Func32::ecExtFuncError;
  else
  {
    if(CallResult != NULL)
      Result = FromPyObject<T>(CallResult);
    if(PyErr_Occurred())
    {
      PyObject *Type, *Value, *Traceback;
      PyErr_Fetch(&Type, &Value, &Traceback);
      if(PyUnicode_Check(Value))
        ErrorStr = PyUnicode_AsUnicode(Value);
      ErrorCode = Func32::ecExtFuncError;

      Py_XDECREF(Type);
      Py_XDECREF(Value);
      Py_XDECREF(Traceback);
 //     PyErr_Print();
 //     PyErr_Clear();
    }
  }
  Py_XDECREF(CallResult);
  Py_XDECREF(Tuple);
  return Result;
}
//---------------------------------------------------------------------------
static PyObject* PluginSetCustomFunction(PyObject *Self, PyObject *Args)
{
  //The number of arguments are in func_code.co_argcount
  const wchar_t *Name;
  PyObject *Function;
  if(!PyArg_ParseTuple(Args, "uO", &Name, &Function))
    return NULL;

  PyObject *FuncCode = PyObject_GetAttrString(Function, "__code__");
  if(FuncCode)
  {
    PyObject *ArgCount = PyObject_GetAttrString(FuncCode, "co_argcount");
    if(ArgCount)
    {
      long Arguments = PyLong_AsLong(ArgCount);
      boost::shared_ptr<TPluginFunc> Func(new TPluginFunc(Function, Arguments));
      Form1->Data.CustomFunctions.SymbolList.Add(Name, Func);
      Form1->Data.CustomFunctions.GlobalSymbolList.Add(Name, Func);
      Py_XDECREF(ArgCount);
    }
    Py_XDECREF(FuncCode);
  }
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginGetCustomFunction(PyObject *Self, PyObject *Args)
{
  const wchar_t *Name = PyUnicode_AsUnicode(Args);
  if(Name == NULL)
    return NULL;

  if(!Form1->Data.CustomFunctions.GlobalSymbolList.Exists(Name))
  {
    PyErr_SetString(PyExc_KeyError, AnsiString(Name).c_str());
    return NULL;
  }

  boost::shared_ptr<TPluginFunc> Func = boost::dynamic_pointer_cast<TPluginFunc>(Form1->Data.CustomFunctions.GlobalSymbolList.Get(Name));
  if(Func)
  {
    PyObject *Obj = Func->GetFunc();
    return Obj;
  }
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginDelCustomFunction(PyObject *Self, PyObject *Args)
{
  const wchar_t *Name = PyUnicode_AsUnicode(Args);
  if(Name == NULL)
    return NULL;

  Form1->Data.CustomFunctions.SymbolList.Erase(Name);
  Form1->Data.CustomFunctions.GlobalSymbolList.Erase(Name);
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginGetCustomFunctionNames(PyObject *Self, PyObject *Args)
{
  const Func32::TSymbolList &SymbolList = Form1->Data.CustomFunctions.GlobalSymbolList;
  Func32::TSymbolList::TConstIterator Begin = SymbolList.Begin();
  Func32::TSymbolList::TConstIterator End = SymbolList.End();
  PyObject *Names = PyList_New(SymbolList.Size());

  int Index = 0;
  for(Func32::TSymbolList::TConstIterator Iter = Begin; Iter != End; ++Iter, ++Index)
    PyList_SetItem(Names, Index, PyUnicode_FromWideChar(Iter->first.c_str(), Iter->first.size()));
  return Names;
}
//---------------------------------------------------------------------------
static PyObject* PluginEval(PyObject *Self, PyObject *Args)
{
  try
  {
    int Degrees = Form1->Data.Axes.Trigonometry == Func32::Degree;
    const wchar_t *Expression;
    if(!PyArg_ParseTuple(Args, "u|i", &Expression, &Degrees))
      return NULL;

    return PyFloat_FromDouble(Func32::Eval(Expression, Form1->Data.CustomFunctions.SymbolList, Degrees ? Func32::Degree : Func32::Radian));
  }
  catch(Func32::EFuncError &E)
  {
    PyErr_SetString(PyEFuncError, ToString(GetErrorMsg(E)).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginEvalComplex(PyObject *Self, PyObject *Args)
{
  try
  {
    int Degrees = Form1->Data.Axes.Trigonometry == Func32::Degree;
    const wchar_t *Expression;
    if(!PyArg_ParseTuple(Args, "u|i", &Expression, &Degrees))
      return NULL;

    Func32::TComplex Result = Func32::EvalComplex(Expression, Form1->Data.CustomFunctions.SymbolList, Degrees ? Func32::Degree : Func32::Radian);
    return PyComplex_FromDoubles(real(Result), imag(Result));
  }
  catch(Func32::EFuncError &E)
  {
    PyErr_SetString(PyEFuncError, AnsiString(GetErrorMsg(E)).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginUpdate(PyObject *Self, PyObject *Args)
{
  TUnlockGIL Dummy;
  Form1->Draw.AbortUpdate();
  Form1->Data.ClearCache();
  Form1->Data.Update();
  Form1->UpdateTreeView();
  Form1->Data.SetModified();
  Form1->Redraw(); //Activates thread; must be done after OLE update
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginGetConstantNames(PyObject *Self, PyObject *Args)
{
  const TCustomFunctions &Functions = Form1->Data.CustomFunctions;
  TCustomFunctions::TConstIterator Begin = Functions.Begin();
  TCustomFunctions::TConstIterator End = Functions.End();
  PyObject *ConstantNames = PyList_New(End - Begin);

  int Index = 0;
  for(TCustomFunctions::TConstIterator Iter = Begin; Iter != End; ++Iter, ++Index)
    PyList_SetItem(ConstantNames, Index, PyUnicode_FromWideChar(Iter->Name.c_str(), Iter->Name.size()));
  return ConstantNames;
}
//---------------------------------------------------------------------------
static PyObject* PluginGetConstant(PyObject *Self, PyObject *Args)
{
  const wchar_t *Name = PyUnicode_AsUnicode(Args);
  if(Name == NULL)
    return NULL;

  try
  {
    const TCustomFunction &Function = Form1->Data.CustomFunctions.GetValue(Name);
    PyObject *Args = PyTuple_New(Function.Arguments.size() + 1);
    PyTuple_SetItem(Args, 0, PyUnicode_FromWideChar(Function.Text.c_str(), Function.Text.size()));
    for(unsigned I = 0; I < Function.Arguments.size(); I++)
      PyTuple_SetItem(Args, I+1, PyUnicode_FromWideChar(Function.Arguments[I].c_str(), Function.Arguments[I].size()));
    return Args;
  }
  catch(ECustomFunctionError &E)
  {
    PyErr_SetString(PyExc_KeyError, AnsiString(Name).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginSetConstant(PyObject *Self, PyObject *Args)
{
  try
  {
    PyObject *Arguments;
    const wchar_t *Text, *Name;
    if(!PyArg_ParseTuple(Args, "uOu", &Name, &Arguments, &Text))
      return NULL;
    TCustomFunctions &Functions = Form1->Data.CustomFunctions;
    std::vector<std::wstring> ArgList;
    if(Arguments != Py_None)
    {
      if(!PyTuple_Check(Arguments))
      {
        PyErr_SetString(PyExc_TypeError, "Function arguments must be a tuple or None");
        return NULL;
      }

      int Size = PyTuple_Size(Arguments);
      if(Size == -1)
        return NULL;
      for(int I = 0; I < Size; I++)
      {
        const wchar_t *Str = PyUnicode_AsUnicode(PyTuple_GetItem(Arguments, I));
        if(Str == NULL)
          return NULL;
        ArgList.push_back(Str);
      }
    }
    Functions.Add(Name, ArgList, Text);
    Py_RETURN_NONE;
  }
  catch(Func32::EFuncError &E)
  {
    PyErr_SetString(PyEFuncError, AnsiString(GetErrorMsg(E)).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginDelConstant(PyObject *Self, PyObject *Args)
{
  const wchar_t *Name = PyUnicode_AsUnicode(Args);
  if(Name == NULL)
    return NULL;

  try
  {
    TCustomFunctions &Functions = Form1->Data.CustomFunctions;
    Functions.Delete(Name);
    Py_RETURN_NONE;
  }
  catch(ECustomFunctionError &E)
  {
    PyErr_SetString(PyExc_KeyError, AnsiString(Name).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginSaveAsImage(PyObject *Self, PyObject *Args, PyObject *Kwds)
{
  try
  {
    TImageOptions Options(Form1->Image1->Width, Form1->Image1->Height);
    Options.LoadSettings();
    static char *kwlist[] = {"FileType", "Width", "Height", NULL};

    const wchar_t *FileName = NULL;
    int FileType = -1, Width = -1, Height = -1;
    if(!PyArg_ParseTupleAndKeywords(Args, Kwds, "u|iii", kwlist, &FileType, &Width, &Height))
      return NULL;

    if(Width == 0 || Height == 0)
      Options.UseCustomSize = false;
    else if(Width != -1 || Height != -1)
    {
      Options.UseCustomSize = true;
      Options.CustomWidth = Width;
      Options.CustomHeight = Height;
    }

    if(FileType == -1)
      SaveAsImage(FileName, FileType, Options);
    else
      SaveAsImage(FileName, Options);
    Py_RETURN_NONE;
  }
  catch(...)
  {
    ConvertException();
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyMethodDef GraphMethods[] = {
	{"SetCustomFunction",         PluginSetCustomFunction, METH_VARARGS, ""},
	{"GetCustomFunction",         PluginGetCustomFunction, METH_O, ""},
	{"DelCustomFunction",         PluginDelCustomFunction, METH_O, ""},
	{"GetCustomFunctionNames",    PluginGetCustomFunctionNames, METH_NOARGS, ""},
	{"WriteToConsole",            PluginWriteToConsole, METH_VARARGS, ""},
	{"InputQuery",                PluginInputQuery, METH_VARARGS, ""},
	{"Eval",                      PluginEval, METH_VARARGS, ""},
	{"EvalComplex",               PluginEvalComplex, METH_VARARGS, ""},
	{"Update",                    PluginUpdate, METH_NOARGS, ""},
	{"SetConstant",               PluginSetConstant, METH_VARARGS, ""},
	{"GetConstant",               PluginGetConstant, METH_O, ""},
	{"DelConstant",               PluginDelConstant, METH_O, ""},
	{"GetConstantNames",          PluginGetConstantNames, METH_NOARGS, ""},
	{"SaveAsImage",               (PyCFunction)PluginSaveAsImage, METH_VARARGS | METH_KEYWORDS, ""},
	{NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
static PyModuleDef GraphModuleDef =
{
  PyModuleDef_HEAD_INIT,
  "GraphImpl",
  NULL,
  -1,
  GraphMethods,
  NULL,
  NULL,
  NULL,
	NULL,
};
//---------------------------------------------------------------------------
void ShowPythonConsole(bool Visible)
{
  if(Form22)
    Form22->Visible = Visible;
}
//---------------------------------------------------------------------------
PyObject* InitGraphImpl()
{
	PyObject *GraphImpl = PyModule_Create(&GraphModuleDef);
	PyModule_AddObject(GraphImpl, "Form1", VclObject_Create(Form1, false));
	PyModule_AddObject(GraphImpl, "Form22", VclObject_Create(Form22, false));
	return GraphImpl;
}
//---------------------------------------------------------------------------
extern "C" PyObject* PyInit__Settings();
extern "C" PyObject* PyInit__Data();
extern "C" PyObject* PyInit__Utility();
_inittab Modules[] =
{
	{"GraphImpl", InitGraphImpl},
	{"PyVcl", InitPyVcl},
	{"_Settings", PyInit__Settings},
	{"_Data", PyInit__Data},
  {"_Utility", PyInit__Utility},
  {NULL, NULL}
};
//---------------------------------------------------------------------------
static bool PythonInitialized = false;
void InitPlugins()
{
  _control87(DEFAULT_FPU_CONTROL, FPU_MASK);
  if(IsPythonInstalled())
	{
  	RegisterClass(__classid(TExtColorBox));

    Form22 = new TForm22(Application);
    Form1->ScriptDocAction->Visible = true;
    _control87(PYTHON_FPU_CONTROL, FPU_MASK); //Set the FPU Control Word to what Python expects
    PyImport_ExtendInittab(Modules);
    static String ExeName = Application->ExeName; //Py_SetProgramName() requires variable to be static
		Py_SetProgramName(ExeName.c_str());
    PyEval_InitThreads();
    Py_Initialize();
    int argc;
    wchar_t **argv = CommandLineToArgvW(GetCommandLine(), &argc);
    PySys_SetArgv(argc, argv);
    LocalFree(argv);

    PyEFuncError = PyErr_NewException("Graph.EFuncError", NULL, NULL);
    PyEGraphError = PyErr_NewException("Graph.EGraphError", NULL, NULL);

    TVersionInfo Info;
    TVersion Version = Info.FileVersion();
    const char *BetaFinal = Info.FileFlags() & ffDebug ? "beta" : "final";
		AnsiString BaseDir = GetRegValue(REGISTRY_KEY, L"BaseDir", HKEY_CURRENT_USER, ExtractFileDir(Application->ExeName).c_str()).c_str();
		AnsiString PythonCommands = AnsiString().sprintf(
			"import sys\n"
			"import GraphImpl\n"
			"class ConsoleWriter:\n"
			"  def __init__(self, color):\n"
			"    self._color = color\n"
			"  def write(self, str):\n"
			"    GraphImpl.WriteToConsole(str, self._color)\n"
			"  def readline(self):\n"
			"    value = GraphImpl.InputQuery()\n"
			"    if value == None: raise KeyboardInterrupt('operation cancelled')\n"
			"    return value + '\\n'\n"

			"sys.stdout = ConsoleWriter(0)\n"
			"sys.stderr = ConsoleWriter(0xFF)\n"
			"sys.stdin = sys.stdout\n"

			"GraphImpl.version_info = (%d,%d,%d,'%s',%d)\n"

			"sys.path.append('%s/Lib')\n"
			"import PyVcl\n"
			"import vcl\n"
			"import Graph\n"
			"Graph.InitPlugins('%s')\n"

			"sys.stdin = sys.stdout\n"
			, Version.Major, Version.Minor, Version.Release, BetaFinal, Version.Build
			, BaseDir.c_str()
			, BaseDir.c_str()
		);

		int Result = PyRun_SimpleString(PythonCommands.c_str());
		PyEval_SaveThread();
		_clear87();
		_control87(DEFAULT_FPU_CONTROL, FPU_MASK);
		PythonInitialized = true;
	}
	else
	{
		Form1->Panel6->Height = 0;
		Form1->Splitter2->Visible = false;
	}
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const TPyVariant &Variant)
{
  if(const int *Value = boost::get<int>(&Variant))
    return ToPyObject(*Value);
  if(const double *Value = boost::get<double>(&Variant))
    return ToPyObject(*Value);
  if(const std::wstring *Value = boost::get<std::wstring>(&Variant))
    return ToPyObject(*Value);
  if(PyObject *const*Value = boost::get<PyObject*>(&Variant))
		return *Value;
  return NULL;
}
//---------------------------------------------------------------------------
bool ExecutePluginEvent(TPluginEvent PluginEvent, const TGraphElemPtr &Elem)
{
  if(!PythonInitialized)
    return false;
  if(IsPythonInstalled())
  {
    TLockGIL Dummy;
    return ExecutePluginEvent(PluginEvent, Py_BuildValue("(N)", DownCastSharedPtr(Elem)));
  }
  return false;
}
//---------------------------------------------------------------------------
bool ExecutePluginEvent(TPluginEvent PluginEvent, PyObject *Param)
{
  if(IsPythonInstalled() && PythonInitialized)
  {
    TLockGIL Dummy;
    PyObject *Module = PyImport_AddModule("Graph");
    char *MethodName = "ExecuteEvent";
    char *Format = Param ? (char*)"(iN)" : (char*)"(i())";
    PyObject *ResultObj = PyObject_CallMethod(Module, MethodName, Format, PluginEvent, Param, NULL);
    bool Result = ResultObj && PyObject_IsTrue(ResultObj);
		Py_XDECREF(ResultObj);
    return Result;
	}
  return false;
}
//---------------------------------------------------------------------------
bool ExecutePluginEvent(TPluginEvent PluginEvent, TPyVariant V1)
{
  if(IsPythonInstalled())
  {
    TLockGIL Dummy;
    return ExecutePluginEvent(PluginEvent, Py_BuildValue("(N)", ToPyObject(V1)));
  }
  return false;
}
//---------------------------------------------------------------------------
bool ExecutePluginEvent(TPluginEvent PluginEvent, TPyVariant V1, TPyVariant V2)
{
  if(IsPythonInstalled())
  {
    TLockGIL Dummy;
    return ExecutePluginEvent(PluginEvent, Py_BuildValue("(NN)", ToPyObject(V1), ToPyObject(V2)));
  }
  return false;
}
//---------------------------------------------------------------------------
bool ExecutePluginEvent(TPluginEvent PluginEvent, TPyVariant V1, TPyVariant V2, TPyVariant V3)
{
  if(IsPythonInstalled())
	{
		TLockGIL Dummy;
    return ExecutePluginEvent(PluginEvent, Py_BuildValue("(NNN)", ToPyObject(V1), ToPyObject(V2), ToPyObject(V2)));
  }
  return false;
}
//---------------------------------------------------------------------------
void ConvertException()
{
  try
  {
    throw;
  }
  catch(Func32::EFuncError &E)
  {
    PyErr_SetString(PyEFuncError, AnsiString(GetErrorMsg(E)).c_str());
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
  }
  catch(...)
  {
    PyErr_SetString(PyExc_RuntimeError, "Unknown exception");
	}
}
//---------------------------------------------------------------------------
} //namespace Python


