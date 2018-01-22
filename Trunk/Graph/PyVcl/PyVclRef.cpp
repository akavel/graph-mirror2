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
PyTypeObject *VclRef_Type = NULL;
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
static PyGetSetDef VclRef_GetSeters[] =
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
static PyType_Slot VclRef_Slots[] =
{
  {Py_tp_doc,	(void*) PROJECT_NAME " reference object"},
  {Py_tp_repr, VclRef_Repr},
  {Py_tp_getset, VclRef_GetSeters},
  {0, NULL}
};

static PyType_Spec VclRef_Spec =
{
  GUI_TYPE "Ref",   //name
  sizeof(TVclRef),  //basicsize
  0,                    //itemsize
  Py_TPFLAGS_DEFAULT,   //flags
  VclRef_Slots,     //slots
};
//---------------------------------------------------------------------------
/** Initialize the VclRef_Type type object.
 *  \return true on success
 */
bool VclRef_InitType()
{
  VclRef_Type = reinterpret_cast<PyTypeObject*>(PyType_FromSpec(&VclRef_Spec));
  return VclRef_Type != NULL;
}
//---------------------------------------------------------------------------
/** Create a new reference object.
 *  \param Value: Pointer to a value referenced by the reference object. The value
 *  must exist at least as long as the object refering to it.
 *  \return New reference
 */
PyObject* VclRef_Create(TValue *Value)
{
	TVclRef *VclRef = PyObject_New(TVclRef, VclRef_Type);
	VclRef->Value = Value;
  //We need to increment refcnt for the type as it is decremented by the default tp_dealloc when the object is destroyed
  Py_INCREF(VclRef_Type);
	return reinterpret_cast<PyObject*>(VclRef);
}
//---------------------------------------------------------------------------
/** Returns true if O is a VclRef object.
 */
bool VclRef_Check(PyObject *O)
{
  return O->ob_type == VclRef_Type;
}
//---------------------------------------------------------------------------
/** Return the value which the given VclRef object is a reference for.
 *  \param O: Pointer to a VclRef object.
 *  \return Value refered to by O.
 *  \throw EPyVclError if O is not a VclRef object.
 */
const TValue& VclRef_AsValue(PyObject *O)
{
  if(O->ob_type == VclRef_Type)
  {
    if(reinterpret_cast<TVclRef*>(O)->Value == NULL)
      throw EPyVclError("Value referenced by VclRef has been freed.");
    return *reinterpret_cast<TVclRef*>(O)->Value;
  }
  throw EPyVclError("Object is not a " GUI_TYPE "Ref object.");
}
//---------------------------------------------------------------------------
/** Invalidate the reference inside the VclRef object.
 *  \param O: Pointer to a VclRef object.
 *  \throw EPyVclError if O is not a VclRef object.
 */
void VclRef_Invalidate(PyObject *O)
{
  if(O->ob_type != VclRef_Type)
    throw EPyVclError("Object is not a " GUI_TYPE "Ref object.");
  reinterpret_cast<TVclRef*>(O)->Value = NULL;
}
//---------------------------------------------------------------------------
} //namespace Python
