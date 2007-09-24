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
#include "Unit22.h"
#include "PythonBind.h"
#include "IThread.h"
#include "VersionInfo.h"
#undef _DEBUG
#include <python.h>
#include "PyVcl.h"
#pragma link "python25.lib"
//---------------------------------------------------------------------------
static bool IsPythonInstalled()
{
  static int Result = -1;
  if(Result == -1)
  {
    HINSTANCE Instance = LoadLibrary(GetRegValue("Software\\Ivan\\Graph", "PythonDll", HKEY_CURRENT_USER, "Python25.dll").c_str());
    Result = Instance != NULL;
    if(Instance)
      FreeLibrary(Instance);
  }
  return Result;
}
//---------------------------------------------------------------------------
void ExecutePythonCommand(const AnsiString &Command)
{
  if(!Command.IsEmpty())
    PyRun_SimpleString(AnsiString().sprintf("eval(compile('%s', '', 'single'))", Command.c_str()).c_str());
  if(Form22)
    Form22->WriteText(">>> ");
}
//---------------------------------------------------------------------------
struct TExecutePythonAction
{
  void __fastcall Execute(TObject *Sender)
  {
//    Form1->Enabled = false;
    if(TTntAction *Action = dynamic_cast<TTntAction*>(Sender))
    {
      PyObject *Result = PyObject_CallObject(reinterpret_cast<PyObject*>(Action->Tag), NULL);
      if(Result == NULL)
        PyErr_Print();
      Py_XDECREF(Result);
    }
//    Form1->Enabled = true;
//    Form1->SetFocus();
  }
};
//---------------------------------------------------------------------------
static PyObject* PluginWriteToConsole(PyObject *Self, PyObject *Args, PyObject *Keywds)
{
  const char *Str;
  TColor Color = clBlack;
  if(!PyArg_ParseTuple(Args, "s|i", &Str, &Color))
    return NULL;
  if(Form22)
    Form22->WriteText(Str, Color);
  return PyNone();
}
//---------------------------------------------------------------------------
static PyObject* PluginCreateAction(PyObject *Self, PyObject *Args, PyObject *Keywds)
{
  TTntAction *Action = new TTntAction(Application);
  Action->Category = _("Plugins");
  Action->ActionList = Form1->ActionManager;

  TTntMenuItem *MenuItem = new TTntMenuItem(Form1->MainMenu);
  MenuItem->Action = Action;
  Form1->Plugins_->Add(MenuItem);
  Form1->Plugins_->Visible = true;

  return PyInt_FromLong(reinterpret_cast<long>(Action));
}
//---------------------------------------------------------------------------
static PyObject* PluginSetActionAttr(PyObject *Self, PyObject *Args, PyObject *Keywds)
{
  try
  {
    TTntAction *Action = NULL;
    const char *Caption = NULL;
    PyObject *Event = NULL;
    const char *IconName = NULL;
    const char *Hint = NULL;
    const char *ShortCut = NULL;
    int Enabled = -1;
    int Visible = -1;

    static char* Kwlist[] = {"action", "caption", "event", "icon", "hint", "shortcut", "enabled", "visible", NULL};
    if(!PyArg_ParseTupleAndKeywords(Args, Keywds, "i|sOsssii", Kwlist, &Action, &Caption, &Event, &IconName, &Hint, &ShortCut, &Enabled, &Visible))
      return NULL;

    if(Event)
    {
      Py_INCREF(Event);
      AnsiString ActionName = PyString_AsString(PyObject_GetAttrString(Event, "__module__"));
      ActionName += "_";
      ActionName += PyString_AsString(PyObject_GetAttrString(Event, "__name__"));

      static TExecutePythonAction ExecutePythonAction;
      Action->Name = ActionName;
      Action->OnExecute = ExecutePythonAction.Execute;
      Action->Tag = reinterpret_cast<int>(Event);
    }

    if(Caption)
      Action->Caption = Caption;
    if(Hint)
      Action->Hint = Hint;
    if(ShortCut)
      Action->ShortCut = TextToShortCut(ShortCut);
    if(IconName)
    {
//      ChDir(ExtractFileDir(Application->ExeName) + "\\Plugins");   //Don't change current dir when called from command line
//      std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
//      Bitmap->LoadFromFile(IconName);
//      Action->ImageIndex = Form1->ImageList2->AddMasked(Bitmap.get(), Bitmap->TransparentColor);
    }
    if(Enabled != -1)
      Action->Enabled = Enabled;
    if(Visible != -1)
      Action->Visible = Visible;

    return PyInt_FromLong(reinterpret_cast<long>(Action));
  }
  catch(Exception &E)
  {
    Application->ShowException(&E);
  }
  return PyNone();
}
//---------------------------------------------------------------------------
static PyObject* PluginGetActionAttr(PyObject *Self, PyObject *Args, PyObject *Keywds)
{
  TAction *Action = reinterpret_cast<TAction*>(PyInt_AsLong(Args));
  if(PyErr_Occurred())
    return NULL;

  return Py_BuildValue("{s:s,s:s,s:s,s:i,s:i}",
    "hint",     Action->Hint.c_str(),
    "shortcut", ShortCutToText(Action->ShortCut).c_str(),
    "caption",  Action->Caption.c_str(),
    "enabled",  Action->Enabled,
    "visible",  Action->Visible
  );
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

  return PyNone();
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
  return PyNone();
}
//---------------------------------------------------------------------------
static PyMethodDef GraphMethods[] = {
  {"CreateAction", reinterpret_cast<PyCFunction>(PluginCreateAction), METH_NOARGS, ""},
  {"SetActionAttr", reinterpret_cast<PyCFunction>(PluginSetActionAttr), METH_VARARGS | METH_KEYWORDS, "Test"},
  {"GetActionAttr", reinterpret_cast<PyCFunction>(PluginGetActionAttr), METH_O, ""},
  {"CreateParametricFunction", PluginCreateParametricFunction, METH_VARARGS, ""},
  {"CreateCustomFunction", PluginCreateCustomFunction, METH_VARARGS, ""},
  {"WriteToConsole", reinterpret_cast<PyCFunction>(PluginWriteToConsole), METH_VARARGS, ""},
  {NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
void ShowPythonConsole()
{
  if(Form22)
  {
    Form22->ManualDock(Form1->Panel6);
    Form22->Show();
    if(Form1->Panel6->VisibleDockClientCount)
      Form1->Panel5->Height = 150;
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
      "sys.stdout = ConsoleWriter(0)\n"
      "sys.stderr = ConsoleWriter(0xFF)\n"

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
      , Version.Major, Version.Minor, Version.Release, BetaFinal, Version.Build
      , Application->Handle
      , Form1
      , ExtractFileDir(Application->ExeName).c_str()
    ).c_str());
  }

  if(Form22)
    Form22->WriteText(">>> ");
}
//---------------------------------------------------------------------------









