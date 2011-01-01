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
#include "PyVclMethod.h"
#include "PyVcl.h"
#include "PyVclConvert.h"
namespace Python
{
//---------------------------------------------------------------------------
struct TVclMethod
{
	PyObject_HEAD
	TObject *Instance;
	DynamicArray<TRttiMethod*> Methods;
};
//---------------------------------------------------------------------------
static PyObject *VclMethod_Repr(TVclMethod* self)
{
	String Str = L'<';
	for(int I = 0; I < self->Methods.get_length(); I++)
	{
		Str += self->Methods[I]->ToString();
		if(I < self->Methods.get_high())
			Str += ", ";
	}
	TComponent *Component = dynamic_cast<TComponent*>(self->Instance);
	if(Component != NULL)
		Str += " of <object '" + Component->Name + "' of type '" + Component->ClassName() + "'>>";
	else
		Str += " of <object of type '" + self->Instance->ClassName() + "'>>";
	return PyUnicode_FromUnicode(Str.c_str(), Str.Length());
}
//---------------------------------------------------------------------------
TValue CallMethod(TRttiType *Type, TObject *Instance, DynamicArray<TRttiMethod*> &Methods, PyObject *Args)
{
	unsigned MethodCount = Methods.get_length();
	for(unsigned I = 0; I < MethodCount; I++)
	{
		TRttiMethod *Method = Methods[I];
		DynamicArray<TRttiParameter*> ParameterTypes = Method->GetParameters();
		std::vector<TValue> Parameters;
		int ParamCount = PyTuple_Size(Args);
		if(ParamCount != ParameterTypes.get_length())
			if(MethodCount == 1)
				throw EPyVclError(Method->Name + "() takes exactly " + ParameterTypes.get_length() + " arguments (" + ParamCount + " given)");
			else
				continue;
		try
		{
			TupleToValues(Args, Parameters, ParameterTypes);
		}
		catch(Exception &E)
		{
			if(MethodCount == 1)
				throw;
			continue;
		}

		if(Method->IsConstructor)
		{
			if(Type == NULL)
				throw EPyVclError("Cannot call constructor");
			return Method->Invoke(Type->AsInstance->MetaclassType, Parameters.size() == 0 ? NULL : &Parameters[0], Parameters.size()-1);

		}
		else if(Method->IsClassMethod || Method->IsStatic)
			return Method->Invoke(Instance->ClassType(), Parameters.size() == 0 ? NULL : &Parameters[0], Parameters.size()-1);
		else
			return Method->Invoke(Instance, Parameters.size() == 0 ? NULL : &Parameters[0], Parameters.size()-1);
	}

	throw EPyVclError("No suitable overload found for the function " + Methods[0]->Name);
}
//---------------------------------------------------------------------------
static PyObject *VclMethod_Call(TVclMethod* self, PyObject *args, PyObject *keywds)
{
	try
	{
		if(keywds != NULL)
			return SetErrorString(PyExc_TypeError, self->Methods[0]->Name + "() does not accept keyword arguments");
		return ToPyObject(CallMethod(NULL, self->Instance, self->Methods, args));
	}
	catch(Exception &E)
	{
		PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
		return NULL;
	}
}
//---------------------------------------------------------------------------
static void VclMethod_Dealloc(TVclMethod* self)
{
	self->Methods.~DynamicArray();
	Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
}
//---------------------------------------------------------------------------
PyTypeObject VclMethodType =
{
	PyObject_HEAD_INIT(NULL)
	"vcl.VclMethod",        	 /* tp_name */
	sizeof(TVclMethod),        /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)VclMethod_Dealloc, /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_compare */
	(reprfunc)VclMethod_Repr,  /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash */
	(ternaryfunc)VclMethod_Call, /* tp_call */
	0,                         /* tp_str */
	0,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 			 /* tp_flags */
	"VCL method object",       /* tp_doc */
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
PyObject* VclMethod_Create(TObject *Instance, const DynamicArray<TRttiMethod*> &Methods)
{
	TVclMethod *VclMethod = PyObject_New(TVclMethod, &VclMethodType);
	new(&VclMethod->Methods) DynamicArray<TRttiMethod*>(Methods);
	VclMethod->Instance = Instance;
	return reinterpret_cast<PyObject*>(VclMethod);
}
//---------------------------------------------------------------------------
} //namespace Python
