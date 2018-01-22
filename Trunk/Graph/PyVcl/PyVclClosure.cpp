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
#include "PyVclClosure.h"
#include "PyVclConvert.h"
#include "PyVclObject.h"

namespace Python
{
PyTypeObject *VclClosure_Type = NULL;
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
	{(char*)"__self__", (getter)VclClosure_GetSelf, NULL, (char*)"Object part of closure.", NULL},
	{NULL}  /* Sentinel */
};
//---------------------------------------------------------------------------
/** VclClosure is a proxy for a Delphi closure, i.e. a method pointer and an instance.
 *  Returned when you access an event assigned to a C++ function, eg. C = Form1.OnShow
 */
static PyType_Slot VclClosure_Slots[] =
{
  {Py_tp_doc,	(void*) PROJECT_NAME " closure object"},
  {Py_tp_dealloc, VclClosure_Dealloc},
  {Py_tp_repr, VclClosure_Repr},
  {Py_tp_call, VclClosure_Call},
  {Py_tp_getset, VclClosure_GetSeters},
  {0, NULL}
};

static PyType_Spec VclClosure_Spec =
{
  GUI_TYPE "Closure",   //name
  sizeof(TVclClosure),  //basicsize
  0,                    //itemsize
  Py_TPFLAGS_DEFAULT,   //flags
  VclClosure_Slots,     //slots
};
//---------------------------------------------------------------------------
/** Initialize the VclClosure_Type type object.
 *  \return true on success
 */
bool VclClosure_InitType()
{
  VclClosure_Type = reinterpret_cast<PyTypeObject*>(PyType_FromSpec(&VclClosure_Spec));
  return VclClosure_Type != NULL;
}
//---------------------------------------------------------------------------
/** Create new VclClosure object, which is a proxy object to an event function.
 *  \param Callable: A TValue containing a TMethod pointing to a closure.
 *  \return New reference to a VclMethod
 */
PyObject* VclClosure_Create(const TValue &Callable)
{
  //Allocate with new should be valid as VclClosure is not subclassable and cannot be allocated directly
  //from Python. And TVclClosure contains a variable with constructor that must be created with new.
	TVclClosure *VclClosure = new TVclClosure();
  PyObject_Init(reinterpret_cast<PyObject*>(VclClosure), VclClosure_Type);
  VclClosure->Callable = Callable;
	return reinterpret_cast<PyObject*>(VclClosure);
}
//---------------------------------------------------------------------------
} //namespace Python
