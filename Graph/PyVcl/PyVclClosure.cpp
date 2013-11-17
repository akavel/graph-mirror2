/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Python.hpp"
#include "PyVclClosure.h"
#include "PyVclConvert.h"
#include "PyVclObject.h"

namespace Python
{
//---------------------------------------------------------------------------
struct TVclClosure
{
	PyObject_HEAD
  TValue Callable;
};
//---------------------------------------------------------------------------
/** Return a string reprecentation of the VCL closure.
 *  \return New reference
 */
static PyObject *VclClosure_Repr(TVclClosure* self)
{
  TRttiType *Type = Context.GetType(self->Callable.TypeInfo);
	TMethod Method;
	self->Callable.ExtractRawDataNoCopy(&Method);
	String Str = L"<" + Type->ToString();
	TObject *Object = static_cast<TObject*>(Method.Data);
	TComponent *Component = dynamic_cast<TComponent*>(Object);
	if(Component != NULL)
		Str += " '" + Component->Name + "' of type '" + Component->ClassName() + "'>";
	else
		Str += " of type '" + Object->ClassName() + "'>";
	return ToPyObject(Str);
}
//---------------------------------------------------------------------------
/** Call the closure refferenced by the VclClosure object.
 *  Keyword arguments are not allowed as they cannot be mapped to arguemnts to a
 *  VCL method.
 */
static PyObject *VclClosure_Call(TVclClosure* self, PyObject *args, PyObject *keywds)
{
#if __BORLANDC_ >= 0x0640
	try
	{
    TRttiType *Type = Context.GetType(self->Callable.TypeInfo);
		if(keywds != NULL)
			return SetErrorString(PyExc_TypeError, Type->Name + "() does not accept keyword arguments");

    TRttiMethodType *MethodType = static_cast<TRttiMethodType*>(Type);
		DynamicArray<TRttiParameter*> ParameterTypes = MethodType->GetParameters();
		std::vector<TValue> Parameters;
  	TupleToValues(args, Parameters, ParameterTypes);
		return ToPyObject(MethodType->Invoke(self->Callable,
      Parameters.size() ? &Parameters[0] : NULL, Parameters.size() - 1));
	}
	catch(...)
	{
		return PyVclHandleException();
	}
#else
  return SetErrorString(PyExc_NotImplementedError, "Not supported by this compiler version.");
#endif
}
//---------------------------------------------------------------------------
/** Destructor for VclClosure.
 */
static void VclClosure_Dealloc(TVclClosure* self)
{
	delete self;
}
//---------------------------------------------------------------------------
static PyObject* VclClosure_GetSelf(TVclClosure *self, void *closure)
{
	TMethod Method;
  self->Callable.ExtractRawDataNoCopy(&Method);
	return VclObject_Create(static_cast<TObject*>(Method.Data), false);
}
//---------------------------------------------------------------------------
static PyGetSetDef VclClosure_GetSeters[] =
{
	{"__self__", (getter)VclClosure_GetSelf, NULL, "Object part of closure.", NULL},
	{NULL}  /* Sentinel */
};
//---------------------------------------------------------------------------
/** VclClosure is a proxy for a Delphi closure, i.e. a method pointer and an instance.
 */
PyTypeObject VclClosure_Type =
{
	PyObject_HEAD_INIT(NULL)
	"vcl.VclClosure",        	 /* tp_name */
	sizeof(TVclClosure),       /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)VclClosure_Dealloc, /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_compare */
	(reprfunc)VclClosure_Repr,  /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash */
	(ternaryfunc)VclClosure_Call, /* tp_call */
	0,                         /* tp_str */
	0,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 			 /* tp_flags */
	"VCL closure object",      /* tp_doc */
	0,		                     /* tp_traverse */
	0,		                     /* tp_clear */
	0,		                     /* tp_richcompare */
	0,		                     /* tp_weaklistoffset */
	0,		                     /* tp_iter */
	0,		                     /* tp_iternext */
	0, 								         /* tp_methods */
	0,                         /* tp_members */
	VclClosure_GetSeters,	  	 /* tp_getset */
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
/** Create new VclClosure object, which is a proxy object to an event function.
 *  \param Callable: A TValue containing a TMethod pointing to a closure.
 *  \return New reference to a VclMethod
 */
PyObject* VclClosure_Create(const TValue &Callable)
{
  //Allocate with new should be valid as VclClosure is not subclassable and
  //cannot be allocated directly from Python
	TVclClosure *VclClosure = new TVclClosure();
  PyObject_Init(reinterpret_cast<PyObject*>(VclClosure), &VclClosure_Type);
  VclClosure->Callable = Callable;
	return reinterpret_cast<PyObject*>(VclClosure);
}
//---------------------------------------------------------------------------
} //namespace Python
