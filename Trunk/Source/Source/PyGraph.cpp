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
#include "Unit22.h"
#include "PythonBind.h"
#include "VersionInfo.h"
#include "Unit18.h"
#undef _DEBUG
#include <python.h>
#include "PyVcl.h"
#include <fstream>
#include "IThread.h"

//---------------------------------------------------------------------------
namespace Python
{
PyObject *PyEFuncError = NULL;
PyObject *PyEGraphError = NULL;
//---------------------------------------------------------------------------
bool ExecutePythonCommand(const AnsiString &Command)
{
  AllocGIL();

  PyObject *Module = PyImport_ImportModule("code");
  bool Result = true;
  if(Module)
  {
    PyObject *Function = PyObject_GetAttrString(Module, "compile_command");
    if(Function)
    {
      PyObject *Code = PyObject_CallFunction(Function, "ss", Command.c_str(), "<console>");
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

  FreeGIL();
  return Result;
}
//---------------------------------------------------------------------------
static int WriteToConsole(int Arg)
{
  std::pair<AnsiString, TColor> *Pair = (std::pair<AnsiString, TColor>*)Arg;
  if(Form22)
    Form22->WriteText(Pair->first, Pair->second);
  delete Pair;  
  return 0;
}
//---------------------------------------------------------------------------
static PyObject* PluginWriteToConsole(PyObject *Self, PyObject *Args)
{
  const char *Str;
  TColor Color = clBlack;
  if(!PyArg_ParseTuple(Args, "s|i", &Str, &Color))
    return NULL;

  Py_BEGIN_ALLOW_THREADS

  if(GetCurrentThreadId() != MainThreadID)
  {
    std::pair<AnsiString,TColor> *Pair = new std::pair<AnsiString, TColor>(Str, Color);
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
  const char *Caption = "Python input";
  const char *Prompt = "";
  if(!PyArg_ParseTuple(Args, "|ss", &Caption, &Prompt))
    return NULL;
  String Value;
  if(InputQuery(Caption, Prompt, Value))
    return PyUnicode_FromWideChar(Value.c_str(), Value.Length());

  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginCreateAction(PyObject *Self, PyObject *Args)
{
  TAction *Action = new TAction(Application);
  Action->Category = _("Plugins");
  Action->ActionList = Form1->ActionManager;

  TMenuItem *MenuItem = new TMenuItem(Form1->MainMenu);
  MenuItem->Action = Action;
  Form1->Plugins_->Add(MenuItem);
  Form1->Plugins_->Visible = true;

  return PyInt_FromLong(reinterpret_cast<long>(Action));
}
//---------------------------------------------------------------------------
static PyObject* PluginCreateParametricFunction(PyObject *Self, PyObject *Args)
{
  const char *xName;
  const char *yName;
  if(!PyArg_ParseTuple(Args, "ss", &xName, &yName))
    return NULL;

  TData &Data = Form1->Data;
  boost::shared_ptr<TBaseFuncType> Func(new TParFunc(xName, yName, Data.CustomFunctions.SymbolList, Data.Axes.Trigonometry));
  Func->From.Value = -10;
  Func->To.Value = 10;
  Func->From.Text = "-10";
  Func->To.Text = "10";
  Func->SetSteps(TTextValue(1000));

  Data.AbortUpdate();
  UndoList.Push(TUndoAdd(Func));
  Data.Add(Func);
  Form1->UpdateTreeView();
  Data.SetModified();
  Form1->Redraw();

  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static long double CallCustomFunction(void *Custom, const long double *Args, unsigned ArgsCount, Func32::TTrigonometry Trigonemtry)
{
  PyRun_SimpleString("math.sqrt(-10.0)\n");
  PyObject *Tuple = PyTuple_New(ArgsCount);
  for(unsigned I = 0; I < ArgsCount; I++)
    PyTuple_SET_ITEM(Tuple, I, PyFloat_FromDouble(Args[I]));
  PyObject *CallResult = PyObject_CallObject(reinterpret_cast<PyObject*>(Custom), Tuple);
  if(CallResult == NULL)
    PyErr_Print();
  double Result = PyFloat_AsDouble(CallResult);
  Py_XDECREF(CallResult);
  Py_XDECREF(Tuple);
  return Result;
}
//---------------------------------------------------------------------------
static Func32::TComplex CallCustomFunction(void *Custom, const Func32::TComplex *Args, unsigned ArgsCount, Func32::TTrigonometry Trigonemtry)
{
  PyObject *Tuple = PyTuple_New(ArgsCount);
  for(unsigned I = 0; I < ArgsCount; I++)
    PyTuple_SET_ITEM(Tuple, I, PyComplex_FromDoubles(Args[I].real(), Args[I].imag()));
  PyObject *CallResult = PyObject_CallObject(reinterpret_cast<PyObject*>(Custom), Tuple);
  if(CallResult == NULL)
    PyErr_Print();
  Py_complex Result = PyComplex_AsCComplex(CallResult);
  Py_XDECREF(CallResult);
  Py_XDECREF(Tuple);
  return Func32::TComplex(Result.real, Result.imag);
}
//---------------------------------------------------------------------------
static PyObject* PluginCreateCustomFunction(PyObject *Self, PyObject *Args)
{
  //The number of arguments are in func_code.co_argcount
  const char *Name;
  PyObject *Function;
  if(!PyArg_ParseTuple(Args, "sO", &Name, &Function))
    return NULL;

  PyObject *FuncCode = PyObject_GetAttrString(Function, "func_code");
  PyObject *ArgCount = PyObject_GetAttrString(FuncCode, "co_argcount");
  long Arguments = PyInt_AsLong(ArgCount);

  Form1->Data.CustomFunctions.GlobalSymbolList.Add(Name, Func32::TCustomFunc(CallCustomFunction, CallCustomFunction, Arguments, Function));

  Py_XDECREF(FuncCode);
  Py_XDECREF(ArgCount);
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginEval(PyObject *Self, PyObject *Args)
{
  try
  {
    int Degrees = Form1->Data.Axes.Trigonometry == Func32::Degree;
    const char *Expression;
    if(!PyArg_ParseTuple(Args, "s|i", &Expression, &Degrees))
      return NULL;

    return PyFloat_FromDouble(Func32::Eval(Expression, Form1->Data.CustomFunctions.SymbolList, Degrees ? Func32::Degree : Func32::Radian));
  }
  catch(Func32::EFuncError &E)
  {
    PyErr_SetString(PyEFuncError, AnsiString(GetErrorMsg(E)).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginEvalComplex(PyObject *Self, PyObject *Args)
{
  try
  {
    int Degrees = Form1->Data.Axes.Trigonometry == Func32::Degree;
    const char *Expression;
    if(!PyArg_ParseTuple(Args, "s|i", &Expression, &Degrees))
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
static PyObject* PluginSaveAsImage(PyObject *Self, PyObject *Args)
{
  const char *FileName = PyString_AsString(Args);
  TImageOptions ImageOptions(Form1->Image1->Width, Form1->Image1->Height);
  switch(Form1->SaveAsImage(FileName, ImageOptions))
  {
    case seFileAccess:
      PyErr_SetString(PyEGraphError, AnsiString(LoadRes(RES_FILE_ACCESS, FileName)).c_str());
      return NULL;

    case seOutOfResources:
      PyErr_SetString(PyEGraphError, AnsiString(LoadRes(RES_OUT_OF_RESOURCES)).c_str());
      return NULL;
  }
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginUpdate(PyObject *Self, PyObject *Args)
{
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
    PyList_SetItem(ConstantNames, Index, PyString_FromString(Iter->Name.c_str()));
  return ConstantNames;  
}
//---------------------------------------------------------------------------
static PyObject* PluginGetConstant(PyObject *Self, PyObject *Args)
{
  const char *Name = PyString_AsString(Args);
  if(Name == NULL)
    return NULL;
    
  try
  {
    const TCustomFunction &Function = Form1->Data.CustomFunctions.GetValue(Name);
    PyObject *Args = PyTuple_New(Function.Arguments.size() + 1);
    PyTuple_SetItem(Args, 0, PyString_FromString(Function.Text.c_str()));
    for(unsigned I = 0; I < Function.Arguments.size(); I++)
      PyTuple_SetItem(Args, I+1, PyString_FromString(Function.Arguments[I].c_str()));
    return Args;
  }
  catch(ECustomFunctionError &E)
  {
    PyErr_SetString(PyExc_KeyError, Name);
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginSetConstant(PyObject *Self, PyObject *Args)
{
  try
  {
    const char *Name;
    PyObject *Arguments;
    const char *Text;
    if(!PyArg_ParseTuple(Args, "sOs", &Name, &Arguments, &Text))
      return NULL;
    TCustomFunctions &Functions = Form1->Data.CustomFunctions;
    std::vector<std::string> ArgList;
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
        const char *Str = PyString_AsString(PyTuple_GetItem(Arguments, I));
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
  const char *Name = PyString_AsString(Args);
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
    PyErr_SetString(PyExc_KeyError, Name);
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyMethodDef GraphMethods[] = {
  {"CreateAction",              PluginCreateAction, METH_NOARGS, ""},
  {"CreateParametricFunction",  PluginCreateParametricFunction, METH_VARARGS, ""},
  {"CreateCustomFunction",      PluginCreateCustomFunction, METH_VARARGS, ""},
  {"WriteToConsole",            PluginWriteToConsole, METH_VARARGS, ""},
  {"InputQuery",                PluginInputQuery, METH_VARARGS, ""},
  {"Eval",                      PluginEval, METH_VARARGS, ""},
  {"EvalComplex",               PluginEvalComplex, METH_VARARGS, ""},
  {"SaveAsImage",               PluginSaveAsImage, METH_O, ""},
  {"Update",                    PluginUpdate, METH_NOARGS, ""},
  {"GetConstantNames",          PluginGetConstantNames, METH_NOARGS, ""},
  {"GetConstant",               PluginGetConstant, METH_O, ""},
  {"SetConstant",               PluginSetConstant, METH_VARARGS, ""},
  {"DelConstant",               PluginDelConstant, METH_O, ""},
  {NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
void ShowPythonConsole(bool Visible)
{
  if(Form22)
  {
    Form22->Visible = Visible;
    Form1->Panel5->Height = Form1->Panel6->VisibleDockClientCount ? 150 : Form1->StatusBar1->Height;
    Form1->Splitter2->Visible = Visible;
  }
}
//---------------------------------------------------------------------------
void InitPlugins()
{
  if(IsPythonInstalled())
  {
    Form22 = new TForm22(Application);
    Py_Initialize();
    AnsiString ExeName = Application->ExeName;
    char *argv[] = {ExeName.c_str(), NULL};
    PySys_SetArgv(1, argv);

    Py_InitModule("GraphImpl", GraphMethods);
    InitPyVcl();

    PyEFuncError = PyErr_NewException("GraphImpl.EFuncError", NULL, NULL);
    PyEGraphError = PyErr_NewException("GraphImpl.EGraphError", NULL, NULL);

    TVersionInfo Info;
    TVersion Version = Info.FileVersion();
    const char *BetaFinal = Info.FileFlags() & ffDebug ? "beta" : "final";
    PyRun_SimpleString(AnsiString().sprintf(
      "import sys\n"
      "class ConsoleWriter:\n"
      "  def __init__(self, color):\n"
      "    self._color = color\n"
      "  def write(self, str):\n"
      "    GraphImpl.WriteToConsole(str, self._color)\n"
      "  def readline(self):\n"
      "    value = GraphImpl.InputQuery()\n"
      "    if value == None: raise KeyboardInterrupt, 'operation cancelled'\n"
      "    return value + '\\n'\n"

      "sys.stdout = ConsoleWriter(0)\n"
      "sys.stderr = ConsoleWriter(0xFF)\n"
      "sys.stdin = sys.stdout\n"

      "import GraphImpl\n"
      "GraphImpl.version_info = (%d,%d,%d,'%s',%d)\n"
      "GraphImpl.handle = %d\n"
      "GraphImpl.form1 = %d\n"

      "import imp\n"
			"File, PathName, Desc = imp.find_module('Graph', ['%s\\Plugins'])\n"
      "Module = imp.load_module('Graph', File, PathName, Desc)\n"
      "File.close()\n"
      "Module.InitPlugins()\n"

      "import Graph\n"
      "import vcl\n"
      "import PyVcl\n"
      "sys.stdin = sys.stdout\n"
      , Version.Major, Version.Minor, Version.Release, BetaFinal, Version.Build
      , Application->Handle
      , Form1
      , AnsiString(ExtractFileDir(Application->ExeName)).c_str()
    ).c_str());

    FreeGIL();
  }
  else
  {
    Form1->Panel5->Height = Form1->StatusBar1->Height;
    Form1->Splitter2->Visible = false;
  }
}
//---------------------------------------------------------------------------
void ExecutePluginEvent(TPluginEvent PluginEvent)
{
  static const char*const EventList[] =
  {
    "OnNew",
    "OnLoad",
    "OnSelect",
    "OnClose",
  };
  if(IsPythonInstalled())
  {
    std::string Command = "Graph.ExecuteEvent(Graph. " + std::string(EventList[PluginEvent]) + ")";
    AllocGIL();
    PyRun_SimpleString(Command.c_str());
    FreeGIL();
  }
}
//---------------------------------------------------------------------------
} //namespace Python


