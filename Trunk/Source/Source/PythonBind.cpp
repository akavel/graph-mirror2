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
#include "PythonBind.h"
#undef _DEBUG
#include <python.h>
#include "ConfigRegistry.h"
#pragma link "python25.lib"
//---------------------------------------------------------------------------
namespace Python
{
PyTypeObject& GetPythonType(const char *Name);
PyObject* GetPythonAddress(const char *Name);
PyThreadState *ThreadState = NULL;
int GILUseCount = 1;

HINSTANCE PythonInstance = NULL;

PyTypeObject &PyTuple_Type = GetPythonType("PyTuple_Type");
PyTypeObject &PyInt_Type = GetPythonType("PyInt_Type");
PyTypeObject &PyString_Type = GetPythonType("PyString_Type");
PyTypeObject &PyUnicode_Type = GetPythonType("PyUnicode_Type");
PyObject &_Py_NoneStruct = (PyObject&)GetPythonType("_Py_NoneStruct");
PyObject *PyExc_TypeError = GetPythonAddress("PyExc_TypeError");
PyObject *PyExc_KeyError = GetPythonAddress("PyExc_KeyError");
//---------------------------------------------------------------------------
bool IsPythonInstalled()
{
  static int Result = -1;
  if(Result == -1)
  {
    PythonInstance = LoadLibrary(GetRegValue(L"Software\\Ivan\\Graph", L"PythonDll", HKEY_CURRENT_USER, L"Python25.dll").c_str());
    Result = PythonInstance != NULL;
  }
  return Result;
}
//---------------------------------------------------------------------------
PyTypeObject& GetPythonType(const char *Name)
{
  static PyTypeObject Dummy;
  if(IsPythonInstalled())
    return *(PyTypeObject*)GetProcAddress(PythonInstance, Name);
  return Dummy;
}
//---------------------------------------------------------------------------
PyObject* GetPythonAddress(const char *Name)
{
  static PyObject *Dummy = NULL;
  if(IsPythonInstalled())
    return *(PyObject**)GetProcAddress(PythonInstance, Name);
  return Dummy;
}
//---------------------------------------------------------------------------
PyObject* PyReturnNone()
{
  Py_INCREF(Py_None);
  return Py_None;
}
//---------------------------------------------------------------------------
void AllocGIL()
{
  if(GILUseCount++ == 0)
    PyEval_RestoreThread(ThreadState);
  ThreadState = NULL;
}
//---------------------------------------------------------------------------
void FreeGIL()
{
  if(--GILUseCount == 0)
    ThreadState = PyEval_SaveThread();
}
//---------------------------------------------------------------------------
}










