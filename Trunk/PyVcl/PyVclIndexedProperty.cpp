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
#include "PyVclConvert.h"
#include "PyVclObject.h"
#include "PyVcl.h"

namespace Python
{
//---------------------------------------------------------------------------
struct TVclIndexedProperty
{
	PyObject_HEAD
	TObject *Instance;
	TRttiIndexedProperty *IndexedProperty;
};
//---------------------------------------------------------------------------
/** Get an element of the array property.
 *  \param self: The array property wrapper, indicating instance and property RTTI.
 *  \param key: The subscription index. May be a tuple
 *  \return New reference to the retrieved object.
 */
static PyObject* VclIndexedProperty_Subscript(TVclIndexedProperty *self, PyObject *key)
{
	try
	{
    if(!self->IndexedProperty->IsReadable)
  		throw EPyVclError("Property is not readable.");
		DynamicArray<TRttiParameter*> ParameterTypes = self->IndexedProperty->ReadMethod->GetParameters();
    std::vector<TValue> Parameters;
    if(PyTuple_Check(key))
      TupleToValues(key, Parameters, ParameterTypes);
    else
      Parameters.push_back(ToValue(key, ParameterTypes[0]->ParamType->Handle));
    return ToPyObject(self->IndexedProperty->GetValue(self->Instance, &Parameters[0], Parameters.size()-1));
	}
	catch(...)
	{
		return PyVclHandleException();
	}
}
//---------------------------------------------------------------------------
/** Return a string representation of the content of the array property.
 *  \param self: The array property wrapper, indicating instance and property RTTI.
 */
static PyObject *VclIndexedProperty_Repr(TVclIndexedProperty* self)
{
  return ToPyObject("<Indexed property '" + self->IndexedProperty->Name + "'>");
}
//---------------------------------------------------------------------------
/** Set an element of the array property.
 *  \param self: The array property wrapper, indicating instance and property RTTI.
 *  \param key: The subscription index. May be a tuple.
 *  \param v: The value to set. This is checked for corret type.
 *  \return 0 on success and -1 on failure.
 */
static int VclIndexedProperty_SetSubscript(TVclIndexedProperty *self, PyObject *key, PyObject *v)
{
	try
	{
    if(!self->IndexedProperty->IsWritable)
  		throw EPyVclError("Property is not writable.");
		DynamicArray<TRttiParameter*> ParameterTypes = self->IndexedProperty->WriteMethod->GetParameters();
    std::vector<TValue> Parameters;
    if(PyTuple_Check(key))
      TupleToValues(key, Parameters, ParameterTypes);
    else
      Parameters.push_back(ToValue(key, ParameterTypes[0]->ParamType->Handle));
    self->IndexedProperty->SetValue(
      self->Instance,
      &Parameters[0], Parameters.size()-1,
      ToValue(v, self->IndexedProperty->PropertyType->Handle));
		return  0;
	}
	catch(...)
	{
		PyVclHandleException();
		return -1;
	}
}
//---------------------------------------------------------------------------
PyMappingMethods VclIndexedProperty_Mapping =
{
  0, /* mp_length */
  (binaryfunc)VclIndexedProperty_Subscript, /* mp_subscript */
  (objobjargproc)VclIndexedProperty_SetSubscript, /* mp_ass_subscript */
};
//---------------------------------------------------------------------------
/** VclArrayProperty is a wrapper around an array property in a Delphi object.
 *  This is a wrokaround as the Delphi RTTI does not support access to read/write array
 *  properties in a generic way.
 */
PyTypeObject VclIndexedProperty_Type =
{
	PyObject_HEAD_INIT(NULL)
	"vcl.VclArrayProperty",  	 /* tp_name */
	sizeof(TVclIndexedProperty), /* tp_basicsize */
	0,                         /* tp_itemsize */
	0, 												 /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_compare */
	(reprfunc)VclIndexedProperty_Repr,  /*tp_repr */
	0, 												 /* tp_as_number */
	0,                         /* tp_as_sequence */
	&VclIndexedProperty_Mapping,  /* tp_as_mapping */
	0,                         /* tp_hash */
	0, 												 /* tp_call */
	0,                         /* tp_str */
	0, 												 /* tp_getattro */
	0, 												 /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 			 /* tp_flags */
	"VCL array property",			 /* tp_doc */
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
/** Create a new array property wrapper object.
 *  \param Instance: This is the VCL object instance that has the array property.
 *  \param Name: This is the name of the array property.
 *  \return New reference to an array property wrapper.
 */
PyObject* VclIndexedProperty_Create(TObject *Instance, const String &Name)
{
  TRttiType *Type = Context.GetType(Instance->ClassType());
  if(Type == NULL)
	throw EPyVclError("Type not found.");
  TRttiIndexedProperty *Property = Type->GetIndexedProperty(Name);
  if(Property == NULL || Property->Visibility == mvPrivate || Property->Visibility == mvProtected)
		return NULL;
	TVclIndexedProperty *VclProperty = PyObject_New(TVclIndexedProperty, &VclIndexedProperty_Type);
	VclProperty->Instance = Instance;
	VclProperty->IndexedProperty = Property;
	return reinterpret_cast<PyObject*>(VclProperty);
}
//---------------------------------------------------------------------------
} //namespace Python
