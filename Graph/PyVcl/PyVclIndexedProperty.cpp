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
#include "Python.hpp"
#include "PyVclConvert.h"
#include "PyVclObject.h"
#include "PyVcl.h"

namespace Python
{
PyTypeObject *VclIndexedProperty_Type = NULL;
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
 *  \param v: The value to set. This is checked for corret type. v is NULL if it should be deleted.
 *  \return 0 on success and -1 on failure.
 */
static int VclIndexedProperty_SetSubscript(TVclIndexedProperty *self, PyObject *key, PyObject *v)
{
	try
	{
    if(v == NULL)
      throw EPyVclError("Cannot delete indexed value.");
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
/** VclIndexedProperty is a wrapper around an array property in a Delphi object.
 */
static PyType_Slot VclIndexedProperty_Slots[] =
{
  {Py_tp_doc,	(void*) PROJECT_NAME " indexed property"},
  {Py_tp_repr, VclIndexedProperty_Repr},
  {Py_mp_subscript, VclIndexedProperty_Subscript},
  {Py_mp_ass_subscript, VclIndexedProperty_SetSubscript},
  {0, NULL}
};

static PyType_Spec VclIndexedProperty_Spec =
{
  GUI_TYPE "IndexedProperty",   //name
  sizeof(TVclIndexedProperty),  //basicsize
  0,                    //itemsize
  Py_TPFLAGS_DEFAULT,   //flags
  VclIndexedProperty_Slots,     //slots
};
//---------------------------------------------------------------------------
/** Initialize the VclIndexedProperty_Type type object.
 *  \return true on success
 */
bool VclIndexedProperty_InitType()
{
  VclIndexedProperty_Type = reinterpret_cast<PyTypeObject*>(PyType_FromSpec(&VclIndexedProperty_Spec));
  return VclIndexedProperty_Type != NULL;
}
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
	TVclIndexedProperty *VclProperty = PyObject_New(TVclIndexedProperty, VclIndexedProperty_Type);
  //We need to increment refcnt for the type as it is decremented by the default tp_dealloc when the object is destroyed
  Py_INCREF(VclIndexedProperty_Type);
	VclProperty->Instance = Instance;
	VclProperty->IndexedProperty = Property;
	return reinterpret_cast<PyObject*>(VclProperty);
}
//---------------------------------------------------------------------------
} //namespace Python
