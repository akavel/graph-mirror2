/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Platform.h"
#pragma hdrstop
//#define PYTHON_WRAP(type,name) type& name = GetPythonAddress<type>(#name);
#include "PythonBind.h"
#include "python.hpp"
#include "PyVclObject.h"
#include <float.h>
#ifdef _WIN64
#pragma link "python36.a"
#elif defined(__WIN32__)
#pragma link "python36.lib"
#else
#pragma link "python35.dylib"
#endif
//---------------------------------------------------------------------------
namespace Python
{
#ifdef PYTHON_EMBEDDED
bool IsPythonInstalled();
#else
#define IsPythonInstalled() true
#endif
//---------------------------------------------------------------------------
extern HINSTANCE PythonInstance;
template<typename T>
T& GetPythonAddress(const char *Name)
{
  static T Dummy;
  if(IsPythonInstalled())
    return *reinterpret_cast<T*>(GetProcAddress(PythonInstance, Name));
  return Dummy;
}
//---------------------------------------------------------------------------

PyObject* PyReturnNone()
{
  Py_INCREF(Py_None);
	return Py_None;
}
//---------------------------------------------------------------------------
TLockGIL::TLockGIL()
{
  if(IsPythonInstalled())
  {
    SET_PYTHON_FPU_MASK();
    State = PyGILState_Ensure();
  }
  else
    State = NULL;
}
//---------------------------------------------------------------------------
TLockGIL::~TLockGIL()
{
  if(State != NULL)
  {
    PyGILState_Release(static_cast<PyGILState_STATE>(State));
    SET_DEFAULT_FPU_MASK();
  }
}
//---------------------------------------------------------------------------
TUnlockGIL::TUnlockGIL()
{
  if(IsPythonInstalled())
  {
    State = PyEval_SaveThread();
    SET_DEFAULT_FPU_MASK();
  }
}
//---------------------------------------------------------------------------
TUnlockGIL::~TUnlockGIL()
{
  if(IsPythonInstalled())
  {
    SET_PYTHON_FPU_MASK();
    PyEval_RestoreThread(State);
  }
}
//---------------------------------------------------------------------------
PyObject* SetErrorString(PyObject *Type, const String &Str)
{
	PyErr_SetString(Type, AnsiString(Str).c_str());
	return NULL;
}
//---------------------------------------------------------------------------
} //namespace Python
//Helper functions needed for use of boost::intrusive_ptr<PyObject> (TPyObjectPtr)
//Warning: Using TPyObjectPtr requires that you have the GIL
void boost::intrusive_ptr_add_ref(PyObject *O)
{
  Py_INCREF(O);
}
//---------------------------------------------------------------------------
void boost::intrusive_ptr_release(PyObject *O)
{
  Py_DECREF(O);
}
//---------------------------------------------------------------------------







