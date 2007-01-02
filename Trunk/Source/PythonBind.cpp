/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "PythonBind.h"
#include "IThread.h"
#undef _DEBUG
#include <python.h>
#pragma link "python25.lib"
//---------------------------------------------------------------------------
static HHOOK KeyboardHookHandle = NULL;

static bool IsPythonInstalled()
{
  static int Result = -1;
  if(Result == -1)
  {
    HINSTANCE Instance = LoadLibrary("Python25.dll");
    Result = Instance != NULL;
    if(Instance)
      FreeLibrary(Instance);
  }
  return Result;
}
//---------------------------------------------------------------------------
class TConsoleReader : public TIThread
{
  char Buffer[100];

  void ExecutePython()
  {
    PyRun_SimpleString(Buffer);
  }

  void __fastcall Execute()
  {
    HANDLE InHandle = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE OutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD Bytes;
    while(1)
    {
      WriteConsole(OutHandle, ">>> ", 4, &Bytes, NULL);
      if(ReadConsole(InHandle, Buffer, sizeof(Buffer)-1, &Bytes, 0))
      {
        Buffer[Bytes - 2] = 0;
        Synchronize(&ExecutePython);
      }
    }
  }
};
//---------------------------------------------------------------------------
static void ShowConsole()
{
  AllocConsole();
  SetConsoleTitle("Graph plugin console");
  PyRun_SimpleString(
    "import sys\n"
    "sys.stdout = open('CONOUT$', 'w', 0)\n"
    "sys.stderr = sys.stdout\n"
  );
  new TConsoleReader;
}
//---------------------------------------------------------------------------
static LRESULT CALLBACK KeyboardProc(int Code, WPARAM wParam, LPARAM lParam)
{
  //Check for F11 pressed down without ALT
  if(wParam == VK_F11 && (lParam & 0xE0000000))
    ShowConsole();
  return CallNextHookEx(KeyboardHookHandle, Code, wParam, lParam);
}
//---------------------------------------------------------------------------
struct TExecutePythonAction 
{
  void __fastcall Execute(TObject *Sender)
  {
    Form1->Enabled = false;
    if(TTntAction *Action = dynamic_cast<TTntAction*>(Sender))
      PyObject_CallObject(reinterpret_cast<PyObject*>(Action->Tag), NULL);
    Form1->Enabled = true;
  }
};
//---------------------------------------------------------------------------
static PyObject* PluginCreateAction(PyObject *Self, PyObject *Args, PyObject *Keywds)
{
  try
  {
    const char *ActionName;
    const char *Caption;
    PyObject *Event;
    const char *IconName = NULL;
    const char *Hint = NULL;
    const char *ShortCut = NULL;

    static char* Kwlist[] = {"action", "caption", "event", "icon", "hint", "shortcut", NULL};
    if(!PyArg_ParseTupleAndKeywords(Args, Keywds, "ssO|sss", Kwlist, &ActionName, &Caption, &Event, &IconName, &Hint, &ShortCut))
      return NULL;

    Py_INCREF(Event);
    TTntAction *Action = new TTntAction(Application);
    static TExecutePythonAction ExecutePythonAction;
    Action->Name = ActionName;
    Action->Caption = Caption;
    Action->Category = _("Plugins");
    Action->Hint = Hint;
    if(ShortCut)
      Action->ShortCut = TextToShortCut(ShortCut);
    Action->ActionList = Form1->ActionManager;
    Action->Tag = reinterpret_cast<int>(Event);
    Action->OnExecute = ExecutePythonAction.Execute;
    if(IconName)
    {
      ChDir(ExtractFileDir(Application->ExeName) + "\\Plugins");
      std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
      Bitmap->LoadFromFile(IconName);
      Action->ImageIndex = Form1->ImageList2->AddMasked(Bitmap.get(), Bitmap->TransparentColor);
    }

    TMenuItem *MenuItem = new TMenuItem(Form1->MainMenu);
    MenuItem->Action = Action;
    Form1->Plugins_->Add(MenuItem);
    Form1->Plugins_->Visible = true;
  }
  catch(Exception &E)
  {
    Application->ShowException(&E);
  }
  return Py_BuildValue("s", NULL);
}
//---------------------------------------------------------------------------
static PyObject* PluginCreateParametricFunction(PyObject *Self, PyObject *Args)
{
  const char *xName;
  const char *yName;
  if(!PyArg_ParseTuple(Args, "ss", &xName, &yName))
    return NULL;

  TData &Data = Form1->Data;  
  boost::shared_ptr<TBaseFuncType> Func(new TParFunc(xName, yName, Data.CustomFunctions.SymbolList));
  Func->SetTrigonometry(Data.Axes.Trigonometry);
  Func->From.Value = -10;
  Func->To.Value = 10;
  Func->From.Text = "-10";
  Func->To.Text = "10";
  Func->Steps = 1000;

  Data.AbortUpdate();
  UndoList.Push(TUndoAdd(Func));
  Data.Add(Func);
  Form1->UpdateTreeView();
  Data.SetModified();
  Form1->Redraw();

  return Py_BuildValue("s", NULL);
}
//---------------------------------------------------------------------------
static PyMethodDef GraphMethods[] = {
  {"CreateAction", reinterpret_cast<PyCFunction>(PluginCreateAction), METH_VARARGS | METH_KEYWORDS, "Test"},
  {"CreateParametricFunction", PluginCreateParametricFunction, METH_VARARGS, ""},
  {NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
void InitPlugins()
{
  if(IsPythonInstalled())
  {
    KeyboardHookHandle = SetWindowsHookEx(WH_KEYBOARD, reinterpret_cast<HOOKPROC>(KeyboardProc), HInstance, GetCurrentThreadId());
    Py_Initialize();
    AnsiString ExeName = Application->ExeName;
    char *argv[] = {ExeName.c_str(), NULL};
    PySys_SetArgv(1, argv);
    if(FindCmdLineSwitch("C"))
      ShowConsole();

    Py_InitModule("Graph", GraphMethods);
    PyRun_SimpleString(AnsiString().sprintf(
      "import imp\n"
			"File, PathName, Desc = imp.find_module('GraphUtil', ['%s\\Plugins'])\n"
      "Module = imp.load_module('GraphUtil', File, PathName, Desc)\n"
      "File.close()\n"
      "Module.InitPlugins()\n"
      , ExtractFileDir(Application->ExeName).c_str()).c_str());
  }
}
//---------------------------------------------------------------------------






