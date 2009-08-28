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
#define PYTHON_WRAP(type,name) type& name = GetPythonAddress<type>(#name);
#include "PythonBind.h"
#undef _DEBUG
#include <python.h>
#include "ConfigRegistry.h"
#pragma link "python31.lib"
//---------------------------------------------------------------------------
namespace Python
{
const WORD PythonFpuControl = MCW_EM | IC_PROJECTIVE | RC_NEAR;
const WORD DefaultFpuControl = EM_DENORMAL | EM_UNDERFLOW | EM_INEXACT | IC_AFFINE | RC_NEAR | PC_64;
const WORD FpuMask = MCW_EM | MCW_IC | MCW_RC;

//PyTypeObject& GetPythonType(const char *Name);
//PyObject* GetPythonAddress(const char *Name);
PyThreadState *ThreadState = NULL;
int GILUseCount = 1;

HINSTANCE PythonInstance = NULL;

template<typename T>
T& GetPythonAddress(const char *Name)
{
  static T Dummy;
  if(IsPythonInstalled())
    return *reinterpret_cast<T*>(GetProcAddress(PythonInstance, Name));
  return Dummy;
}
//---------------------------------------------------------------------------
/*
PyTypeObject &PyTuple_Type = GetPythonAddress<PyTypeObject>("PyTuple_Type");
PyTypeObject &PyLong_Type = GetPythonAddress<PyTypeObject>("PyLong_Type");
PyTypeObject &PyUnicode_Type = GetPythonAddress<PyTypeObject>("PyUnicode_Type");
PyTypeObject &PyBaseObject_Type = GetPythonAddress<PyTypeObject>("PyBaseObjecy_Type");
PyTypeObject &PyCFunction_Type = GetPythonAddress<PyTypeObject>("PyCFunction_Type");
PyTypeObject &PyFloat_Type = GetPythonAddress<PyTypeObject>("PyUnicode_Type");
PyTypeObject &PyType_Type = GetPythonAddress<PyTypeObject>("PyUnicode_Type");
PyObject *PyExc_TypeError = GetPythonAddress<PyObject*>("PyExc_TypeError");
PyObject *PyExc_KeyError = GetPythonAddress<PyObject*>("PyExc_KeyError");
PyObject *PyExc_KeyError = GetPythonAddress<PyObject*>("PyExc_KeyError");
PyObject &_Py_NoneStruct = GetPythonAddress<PyObject>("_Py_NoneStruct");
PyObject &_Py_TrueStruct = GetPythonAddress<PyObject>("_Py_TrueStruct");
PyObject &_Py_FalseStruct = GetPythonAddress<PyObject>("_Py_FalseStruct");
PyObject &_Py_NotImplementedStruct = GetPythonAddress<PyObject>("_Py_NotImplementedStruct");
*/
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
/*PyTypeObject& GetPythonType(const char *Name)
{
  static PyTypeObject Dummy;
  if(IsPythonInstalled())
  {
    void *Address = GetProcAddress(PythonInstance, Name);
    return *reinterpret_cast<PyTypeObject*>(Address);
  }
  return Dummy;
}*/
//---------------------------------------------------------------------------
PyObject* PyReturnNone()
{
  Py_INCREF(Py_None);
  return Py_None;
}
//---------------------------------------------------------------------------
unsigned OldValue;
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










