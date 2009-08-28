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
PYTHON_WRAP(_object, _Py_NoneStruct)
PYTHON_WRAP(_object, _Py_TrueStruct)
PYTHON_WRAP(_object, _Py_FalseStruct)
PYTHON_WRAP(_object, _Py_NotImplementedStruct)

bool IsPythonInstalled();
void AllocGIL();
void FreeGIL();
}
#endif
