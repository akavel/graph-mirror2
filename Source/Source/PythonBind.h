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
extern _typeobject &PyTuple_Type;
extern _typeobject &PyLong_Type;
extern _typeobject &PyString_Type;
extern _typeobject &PyUnicode_Type;
extern _object *PyExc_TypeError;
extern _object *PyExc_KeyError;
extern _object &_Py_NoneStruct;

bool IsPythonInstalled();
void AllocGIL();
void FreeGIL();
}
#endif
