/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef PythonBindH
#define PythonBindH
//---------------------------------------------------------------------------
struct _object;
struct _typeobject;

namespace Python
{
//typedef _object PyObject;
template<typename T> T& GetPythonAddress(const char *Name);

#ifndef PYTHON_WRAP
#define PYTHON_WRAP(type,name) extern type& name;
#endif
PYTHON_WRAP(_typeobject, PyTuple_Type)
PYTHON_WRAP(_typeobject, PyLong_Type)
PYTHON_WRAP(_typeobject, PyString_Type)
PYTHON_WRAP(_typeobject, PyUnicode_Type)
PYTHON_WRAP(_typeobject, PyBaseObject_Type)
PYTHON_WRAP(_typeobject, PyCFunction_Type)
PYTHON_WRAP(_typeobject, PyFloat_Type)
PYTHON_WRAP(_typeobject, PyType_Type)
PYTHON_WRAP(_typeobject, PySlice_Type)
PYTHON_WRAP(_object*, PyExc_TypeError)
PYTHON_WRAP(_object*, PyExc_KeyError)
PYTHON_WRAP(_object*, PyExc_AttributeError)
PYTHON_WRAP(_object*, PyExc_IOError)
PYTHON_WRAP(_object*, PyExc_IndexError)
PYTHON_WRAP(_object*, PyExc_MemoryError)
PYTHON_WRAP(_object*, PyExc_NameError)
PYTHON_WRAP(_object*, PyExc_OverflowError)
PYTHON_WRAP(_object*, PyExc_RuntimeError)
PYTHON_WRAP(_object*, PyExc_SyntaxError)
PYTHON_WRAP(_object*, PyExc_SystemError)
PYTHON_WRAP(_object*, PyExc_ValueError)
PYTHON_WRAP(_object*, PyExc_ZeroDivisionError)
PYTHON_WRAP(_object*, PyExc_NotImplementedError)
PYTHON_WRAP(_object*, PyExc_StopIteration)
PYTHON_WRAP(_object, _Py_NoneStruct)
PYTHON_WRAP(_object, _Py_TrueStruct)
PYTHON_WRAP(_object, _Py_FalseStruct)
PYTHON_WRAP(_object, _Py_NotImplementedStruct)

bool IsPythonInstalled();
void AllocGIL();
void FreeGIL();

class TLockGIL
{
public:
  TLockGIL(){AllocGIL();}
  ~TLockGIL(){FreeGIL();}
};
}
#ifdef WRAP_PYOBJECTS
#define PyTuple_Type Python::PyTuple_Type
#define PyLong_Type Python::PyLong_Type
#define PyString_Type Python::PyString_Type
#define PyUnicode_Type Python::PyUnicode_Type
#define PyBaseObject_Type Python::PyBaseObject_Type
#define PyCFunction_Type Python::PyCFunction_Type
#define PyFloat_Type Python::PyFloat_Type
#define PyType_Type Python::PyType_Type
#define PySlice_Type Python::PySlice_Type
#define PyExc_TypeError Python::PyExc_TypeError
#define PyExc_KeyError Python::PyExc_KeyError
#define PyExc_AttributeError Python::PyExc_AttributeError
#define PyExc_IOError Python::PyExc_IOError
#define PyExc_IndexError Python::PyExc_IndexError
#define PyExc_MemoryError Python::PyExc_MemoryError
#define PyExc_NameError Python::PyExc_NameError
#define PyExc_OverflowError Python::PyExc_OverflowError
#define PyExc_RuntimeError Python::PyExc_RuntimeError
#define PyExc_SyntaxError Python::PyExc_SyntaxError
#define PyExc_SystemError Python::PyExc_SystemError
#define PyExc_ValueError Python::PyExc_ValueError
#define PyExc_ZeroDivisionError Python::PyExc_ZeroDivisionError
#define PyExc_NotImplementedError Python::PyExc_NotImplementedError
#define PyExc_StopIteration Python::PyExc_StopIteration
#define _Py_NoneStruct Python::_Py_NoneStruct
#define _Py_TrueStruct Python::_Py_TrueStruct
#define _Py_FalseStruct Python::_Py_FalseStruct
#define _Py_NotImplementedStruct Python::_Py_NotImplementedStruct
#endif

#endif
