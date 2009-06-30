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
#pragma link "python31.lib"
//---------------------------------------------------------------------------
namespace Python
{
const WORD PythonFpuControl = MCW_EM | IC_PROJECTIVE | RC_NEAR;
const WORD DefaultFpuControl = EM_DENORMAL | EM_UNDERFLOW | EM_INEXACT | IC_AFFINE | RC_NEAR;
const WORD FpuMask = MCW_EM | MCW_IC | MCW_RC;

PyTypeObject& GetPythonType(const char *Name);
PyObject* GetPythonAddress(const char *Name);
PyThreadState *ThreadState = NULL;
int GILUseCount = 1;

HINSTANCE PythonInstance = NULL;

PyTypeObject &PyTuple_Type = GetPythonType("PyTuple_Type");
PyTypeObject &PyLong_Type = GetPythonType("PyLong_Type");
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
    PythonInstance = LoadLibrary(L"Python31.dll");
    Result = PythonInstance != NULL;
  }
  return Result;
}
//---------------------------------------------------------------------------
PyTypeObject& GetPythonType(const char *Name)
{
  static PyTypeObject Dummy;
  if(IsPythonInstalled())
  {
    void *Address = GetProcAddress(PythonInstance, Name);
    return *reinterpret_cast<PyTypeObject*>(Address);
  }
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
  {
    _control87(PythonFpuControl, FpuMask); //Set the FPU Control Word to what Python expects
    PyEval_RestoreThread(ThreadState);
  }
  ThreadState = NULL;
}
//---------------------------------------------------------------------------
void FreeGIL()
{
  if(--GILUseCount == 0)
  {
    ThreadState = PyEval_SaveThread();
    _clear87(); //Clear FPU status flags
    _control87(DefaultFpuControl, FpuMask);   //Reset FPU exception state to the previous
  }
}
//---------------------------------------------------------------------------
}










