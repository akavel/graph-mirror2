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
/** Create string representation of reference object.
 */
static PyObject *VclRef_Repr(TVclRef* self)
{
  if(self->Value != NULL)
  	return ToPyObject(L"<Reference to '" + self->Value->ToString() + L"'>");
  return ToPyObject(L"<Reference to freed value>");
}
//---------------------------------------------------------------------------
/** Retrieve the value refered to by the reference object.
 */
static PyObject* VclRef_GetValue(TVclRef *self, void *closure)
{
	try
	{
    if(self->Value == NULL)
      Py_RETURN_NONE;
		return ToPyObject(*self->Value);
	}
	catch(...)
	{
		return PyVclHandleException();
	}
}
//---------------------------------------------------------------------------
/** Set the value refered to by the reference object. This will also validate the type.
 */
static int VclRef_SetValue(TVclRef *self, PyObject *value, void *closure)
{
	try
	{
    if(self->Value == NULL)
      throw EPyVclError("Trying to set value for broken reference.");
		*self->Value = ToValue(value, self->Value->TypeInfo);
		return 0;
	}
	catch(...)
	{
		PyVclHandleException();
		return -1;
	}
}
//---------------------------------------------------------------------------
static PyGetSetDef VclRefr_GetSeters[] =
{
	{(char*)"Value", (getter)VclRef_GetValue, (setter)VclRef_SetValue, (char*)"Referenced value", NULL},
	{NULL}  /* Sentinel */
};
//---------------------------------------------------------------------------
/** A reference object is used when callback functions like events are created 
 *  in Python but called from Delphi with a parameter that is a reference.
 *  For example if Delphi expects a function like this:
 *  procedure Foo(var Key : Word);
 *  This may be implemented in Python like this:
 *  def Foo(Key)
 *  In this case Key is a reference object, where Key.Value is used to read and write 
 *  the actual value. This is necessary because it is not possible to change the 
 *  value of a built-in type in Python.
 */
PyTypeObject VclRef_Type =
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
	0, 												 /* tp_getattro */
	0, 												 /* tp_setattro */
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
	VclRefr_GetSeters,				 /* tp_getset */
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
/** Create a new reference object.
 *  \param Value: Pointer to a value referenced by the reference object. The value
 *  must exist at least as long as the object refering to it.
 *  \return New reference
 */
PyObject* VclRef_Create(TValue *Value)
{
	TVclRef *VclRef = PyObject_New(TVclRef, &VclRef_Type);
	VclRef->Value = Value;
	return reinterpret_cast<PyObject*>(VclRef);
}
//---------------------------------------------------------------------------
/** Returns true if O is a VclRef object.
 */
bool VclRef_Check(PyObject *O)
{
  return O->ob_type == &VclRef_Type;
}
//---------------------------------------------------------------------------
/** Return the value which the given VclRef object is a reference for.
 *  \param O: Pointer to a VclRef object.
 *  \return Value refered to by O.
 *  \throw EPyVclError if O is not a VclRef object.
 */
const TValue& VclRef_AsValue(PyObject *O)
{
  if(O->ob_type == &VclRef_Type)
  {
    if(reinterpret_cast<TVclRef*>(O)->Value == NULL)
      throw EPyVclError("Value referenced by VclRef has been freed.");
    return *reinterpret_cast<TVclRef*>(O)->Value;
  }
  throw EPyVclError("Object is not a vcl.VclRef object.");
}
//---------------------------------------------------------------------------
/** Invalidate the reference inside the VclRef object.
 *  \param O: Pointer to a VclRef object.
 *  \throw EPyVclError if O is not a VclRef object.
 */
void VclRef_Invalidate(PyObject *O)
{
  if(O->ob_type != &VclRef_Type)
    throw EPyVclError("Object is not a vcl.VclRef object.");
  reinterpret_cast<TVclRef*>(O)->Value = NULL;
}
//---------------------------------------------------------------------------
} //namespace Python
