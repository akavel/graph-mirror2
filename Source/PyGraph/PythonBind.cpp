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
#include <Rtti.hpp>
#pragma link "python31.lib"
//---------------------------------------------------------------------------
namespace Python
{
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
PyObject* PyReturnNone()
{
  Py_INCREF(Py_None);
	return Py_None;
}
//---------------------------------------------------------------------------
TLockGIL::TLockGIL()
{
  _control87(PYTHON_FPU_CONTROL, FPU_MASK);
  State = PyGILState_Ensure();
}
//---------------------------------------------------------------------------
TLockGIL::~TLockGIL()
{
  PyGILState_Release(static_cast<PyGILState_STATE>(State));
  _clear87();
  _control87(DEFAULT_FPU_CONTROL, FPU_MASK);
}
//---------------------------------------------------------------------------
TUnlockGIL::TUnlockGIL()
{
  State = PyEval_SaveThread();
  _clear87();
  _control87(DEFAULT_FPU_CONTROL, FPU_MASK);
}
//---------------------------------------------------------------------------
TUnlockGIL::~TUnlockGIL()
{
  _control87(PYTHON_FPU_CONTROL, FPU_MASK);
  PyEval_RestoreThread(State);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(bool Value)
{
	return PyBool_FromLong(Value);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(int Value)
{
	return PyLong_FromLong(Value);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(double Value)
{
	return PyFloat_FromDouble(Value);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const std::wstring &Str)
{
  return PyUnicode_FromUnicode(Str.c_str(), Str.size());
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const String &Str)
{
	return PyUnicode_FromUnicode(Str.c_str(), Str.Length());
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const Func32::TComplex &Value)
{
	return Value.imag() ? PyComplex_FromDoubles(Value.real(), Value.imag()) : PyFloat_FromDouble(Value.real());
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const Rtti::TValue &V)
{
	Rtti::TValue &Value = const_cast<Rtti::TValue&>(V);
	if(Value.IsEmpty)
		Py_RETURN_NONE;
	switch(Value.Kind)
	{
		case tkInteger:
			return ToPyObject(Value.AsInteger());

		case tkUString:
			return ToPyObject(Value.AsString());

		case tkEnumeration:
		{
			if(AnsiString(Value.TypeInfo->Name) == "Boolean")
				return ToPyObject((bool)Value.AsOrdinal());
			return ToPyObject(GetEnumName(Value.TypeInfo, Value.AsOrdinal()));
		}

		case tkClass:
			return ToPyObject((int)Value.AsObject());

		case tkSet:
			return ToPyObject(SetToString(Value.TypeInfo, *static_cast<int*>(Value.GetReferenceToRawData()), false));

		case tkUnknown:
		case tkChar:
		case tkFloat:
		case tkString:
		case tkMethod:
		case tkWChar:
		case tkLString:
		case tkWString:
		case tkVariant:
		case tkArray:
		case tkRecord:
		case tkInterface:
		case tkInt64:
		case tkDynArray:
		case tkClassRef:
		case tkPointer:
		case tkProcedure:
		default:
			throw Exception("Unknown type");
	}
}
//---------------------------------------------------------------------------
template<> double FromPyObject<double>(PyObject *O)
{
  if(PyComplex_Check(O))
    if(PyComplex_ImagAsDouble(O) != 0)
      PyErr_SetString(PyExc_TypeError, "complex number has an imaginary part");
		else
      return PyComplex_RealAsDouble(O);
  return PyFloat_AsDouble(O);
}
//---------------------------------------------------------------------------
template<> Func32::TComplex FromPyObject<Func32::TComplex>(PyObject *O)
{
  Py_complex V = PyComplex_AsCComplex(O);
	return Func32::TComplex(V.real, V.imag);
}
//---------------------------------------------------------------------------
template<> std::wstring FromPyObject<std::wstring>(PyObject *O)
{
	return PyUnicode_AsUnicode(O);
}
//---------------------------------------------------------------------------
template<> int FromPyObject<int>(PyObject *O)
{
	return PyLong_AsLong(O);
}
//---------------------------------------------------------------------------
} //namespace Python









