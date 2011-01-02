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
#undef _DEBUG
#include "Python.h"
#include <Rtti.hpp>
#include "PythonBind.h"
#include "PyVclRef.h"
#include "PyVclConvert.h"
//---------------------------------------------------------------------------
namespace Python
{
//---------------------------------------------------------------------------
struct TVclRef
{
	PyObject_HEAD
	TValue *Value;
};
//---------------------------------------------------------------------------
static PyObject *VclRef_Repr(TVclRef* self)
{
	return ToPyObject(L"<Reference to '" + self->Value->ToString() + L"'>");
}
//---------------------------------------------------------------------------
static PyObject* VclRef_GetAttro(TVclRef *self, PyObject *attr_name)
{
	try
	{
		if(PyUnicode_AsUnicode(attr_name) == String("Value"))
			return ToPyObject(*self->Value);
		return PyObject_GenericGetAttr(reinterpret_cast<PyObject*>(self), attr_name);
	}
	catch(Exception &E)
	{
		SetErrorString(PyVclException, E.Message);
		return NULL;
	}
}
//---------------------------------------------------------------------------
static int VclRef_SetAttro(TVclRef *self, PyObject *attr_name, PyObject *v)
{
	try
	{
		if(PyUnicode_AsUnicode(attr_name) == String("Value"))
		{
			*self->Value = ToValue(v, self->Value->TypeInfo);
			return 0;
		}
		return PyObject_GenericSetAttr(reinterpret_cast<PyObject*>(self), attr_name, v);
	}
	catch(Exception &E)
	{
		SetErrorString(PyVclException, E.Message);
		return -1;
	}
}
//---------------------------------------------------------------------------
PyTypeObject VclRefType =
{
	PyObject_HEAD_INIT(NULL)
	"vcl.VclRef",        	 		 /* tp_name */
	sizeof(TVclRef),        	 /* tp_basicsize */
	0,                         /* tp_itemsize */
	0,												 /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_compare */
	(reprfunc)VclRef_Repr,  	 /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash */
	0, 												 /* tp_call */
	0,                         /* tp_str */
	(getattrofunc)VclRef_GetAttro, /* tp_getattro */
	(setattrofunc)VclRef_SetAttro, /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 			 /* tp_flags */
	"Reference object",			 	 /* tp_doc */
	0,		                     /* tp_traverse */
	0,		                     /* tp_clear */
	0,		                     /* tp_richcompare */
	0,		                     /* tp_weaklistoffset */
	0,		                     /* tp_iter */
	0,		                     /* tp_iternext */
	0,         								 /* tp_methods */
	0,         								 /* tp_members */
	0,       									 /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	0,                         /* tp_init */
	0,                         /* tp_alloc */
	0,						             /* tp_new */
};
//---------------------------------------------------------------------------
PyObject* VclRef_Create(TValue *Value)
{
	TVclRef *VclRef = PyObject_New(TVclRef, &VclRefType);
	VclRef->Value = Value;
	return reinterpret_cast<PyObject*>(VclRef);
}
//---------------------------------------------------------------------------
} //namespace Python
