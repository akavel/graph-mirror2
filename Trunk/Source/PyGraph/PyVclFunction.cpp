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
#include "PyVcl.h"

namespace Python
{
//---------------------------------------------------------------------------
const int MaxArgCount = 2;
struct TFunctionEntry
{
	const wchar_t *Name;
	void *Address;
	TTypeInfo *Result;
	TTypeInfo *Args[MaxArgCount];
};
//---------------------------------------------------------------------------
const TFunctionEntry FunctionList[] =
{
	{L"ShortCutToText", ShortCutToText, __delphirtti(String), __delphirtti(TShortCut)},
	{L"TextToShortCut", TextToShortCut, __delphirtti(TShortCut), __delphirtti(String)},
	{L"ReadComponentResFile", ReadComponentResFile, __delphirtti(TComponent), __delphirtti(String), __delphirtti(TComponent)},
};
//---------------------------------------------------------------------------
struct TVclFunction
{
	PyObject_HEAD
	const TFunctionEntry *Function;
};
//---------------------------------------------------------------------------
static PyObject *VclFunction_Repr(TVclFunction* self)
{
	String Str = String("<Function ") + self->Function->Name + ">";
	return PyUnicode_FromUnicode(Str.c_str(), Str.Length());
}
//---------------------------------------------------------------------------
static PyObject *VclFunction_Call(TVclFunction* self, PyObject *args, PyObject *keywds)
{
	try
	{
		if(keywds != NULL)
		  throw EPyVclError("Function does not accept keyword arguments");
		DynamicArray<TValue> Arguments;

		int Count;
		for(Count=0; Count < MaxArgCount && self->Function->Args[Count] != NULL; Count++);
		Arguments.Length = Count;
		if(Count != PyTuple_Size(args))
			throw EPyVclError("Function argument count mismatch!");

		for(int I = 0; I < Count; I++)
			 Arguments[I] = ToValue(PyTuple_GET_ITEM(args, I), self->Function->Args[I]);

		TValue Result = Invoke(self->Function->Address, Arguments, ccReg, self->Function->Result);
		return ToPyObject(Result);
	}
	catch(Exception &E)
	{
		SetErrorString(PyVclException, E.Message);
		return NULL;
	}
}
//---------------------------------------------------------------------------
PyTypeObject VclFunctionType =
{
	PyObject_HEAD_INIT(NULL)
	"vcl.VclFunction",       	 /* tp_name */
	sizeof(TVclFunction),      /* tp_basicsize */
	0,                         /* tp_itemsize */
	0, 												 /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_compare */
	(reprfunc)VclFunction_Repr, /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash */
	(ternaryfunc)VclFunction_Call, /* tp_call */
	0,                         /* tp_str */
	0,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 			 /* tp_flags */
	"VCL global function object", /* tp_doc */
	0,		                     /* tp_traverse */
	0,		                     /* tp_clear */
	0,		                     /* tp_richcompare */
	0,		                     /* tp_weaklistoffset */
	0,		                     /* tp_iter */
	0,		                     /* tp_iternext */
	0, 								         /* tp_methods */
	0,                         /* tp_members */
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
PyObject* VclFunction_Create(const String &Name)
{
	TFunctionEntry *Function = NULL;
	for(unsigned I = 0; I < sizeof(FunctionList) / sizeof(FunctionList[0]); I++)
		if(Name == FunctionList[I].Name)
		{
			TVclFunction *VclFunction = PyObject_New(TVclFunction, &VclFunctionType);
			VclFunction->Function = &FunctionList[I];
			return reinterpret_cast<PyObject*>(VclFunction);
		}

	return NULL;
}
//---------------------------------------------------------------------------
} //namespace Python
