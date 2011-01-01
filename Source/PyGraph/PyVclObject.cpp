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
#include <structmember.h>
#define private public
#include <Rtti.hpp>
#undef private
#include "PyVclObject.h"
#include "PyVclMethod.h"
#include "PyVcl.h"
#include "PyVclConvert.h"
#include "PyVclArrayProperty.h"

namespace Python
{
//---------------------------------------------------------------------------
struct TPythonCallback : public TCppInterfacedObject<TMethodImplementationCallback>
{
	void __fastcall Invoke(void * UserData, const System::DynamicArray<TValue> Args, TValue &Result);
};
TPythonCallback *PythonCallback = new TPythonCallback;
//---------------------------------------------------------------------------
void __fastcall TPythonCallback::Invoke(void * UserData, const System::DynamicArray<TValue> Args, TValue &Result)
{
	TLockGIL Dummy;
	PyObject *Object = static_cast<PyObject*>(UserData);
	int Count = Args.get_length() - 1;
	PyObject *PyArgs = Count != 0 ? PyTuple_New(Count) : NULL;
	for(int I = 0; I < Count; I++)
		PyTuple_SET_ITEM(PyArgs, I, ToPyObject(Args[I + 1]));
	PyObject *PyResult = PyObject_CallObject(Object, PyArgs);
	Py_XDECREF(PyArgs);
	if(PyResult != NULL && PyResult != Py_None)
		Result = ToValue(PyResult, NULL); //Bug: Type of result missing
	Py_XDECREF(PyResult);
	if(PyResult == NULL)
	  PyErr_Print();
}
//---------------------------------------------------------------------------
class TImplementationOwner : public TComponent
{
	TMethodImplementation *Impl;
public:
	TImplementationOwner(TComponent *AOwner, TMethodImplementation *AImpl)
		: TComponent(AOwner), Impl(AImpl) {}
	__fastcall ~TImplementationOwner()
	{
    Py_DECREF(static_cast<PyObject*>(Impl->FUserData));
		delete Impl->FInvokeInfo;
		delete Impl;
	}
};
//---------------------------------------------------------------------------
TMethodImplementation::TInvokeInfo* CreateInvokeInfo(TTypeInfo *TypeInfo)
{
	TTypeData *TypeData = reinterpret_cast<TTypeData*>(&TypeInfo->Name[TypeInfo->Name[0]+1]);
	int Index = 0;
	std::vector<Typinfo::TParamFlags> ParamFlags;
	for(int I = 0; I < TypeData->ParamCount; I++)
	{
		ParamFlags.push_back(reinterpret_cast<Typinfo::TParamFlags&>(TypeData->ParamList[Index++]));
		Index += TypeData->ParamList[Index] + 1;
		Index += TypeData->ParamList[Index] + 1;
	}
	if(TypeData->MethodKind == mkFunction)
	{
		Index += TypeData->ParamList[Index] + 1;
		Index += 4;
	}
	Typinfo::TCallConv CallConv = static_cast<Typinfo::TCallConv>(TypeData->ParamList[Index]);
	Index++;
	TMethodImplementation::TInvokeInfo *InvokeInfo = new TMethodImplementation::TInvokeInfo(CallConv, true);
	InvokeInfo->AddParameter(__delphirtti(TObject), false);
	for(int I = 0; I < TypeData->ParamCount; I++)
	{
		TTypeInfo *ParamType = **(TTypeInfo***)&TypeData->ParamList[Index];
		InvokeInfo->AddParameter(ParamType, ParamFlags[I].Contains(Typinfo::pfVar));
		Index += 4;
	}
	InvokeInfo->Seal();
	return InvokeInfo;
}
//---------------------------------------------------------------------------
static PyObject *VclObject_Repr(TVclObject* self)
{
	String Str;
	TComponent *Component = dynamic_cast<TComponent*>(self->Instance);
	if(Component != NULL)
		Str = "<object '" + Component->Name + "' of type '" + Component->ClassName() + "'>";
	else
		Str = "<object of type '" + self->Instance->ClassName() + "'>";
	return PyUnicode_FromUnicode(Str.c_str(), Str.Length());
}
//---------------------------------------------------------------------------
static PyObject* VclObject_Dir(TVclObject *self, PyObject *arg)
{
	TRttiType *Type = Context.GetType(self->Instance->ClassType());
	DynamicArray<TRttiProperty*> Properties = Type->GetProperties();
	DynamicArray<TRttiMethod*> Methods = Type->GetMethods();
	int PropertyCount = Properties.Length;
	int MethodCount = Methods.Length;
	PyObject *List = PyList_New(Properties.Length + Methods.Length);
	for(int I = 0; I < PropertyCount; I++)
		PyList_SET_ITEM(List, I, ToPyObject(Properties[I]->Name));
	for(int I = 0; I < MethodCount; I++)
		PyList_SET_ITEM(List, I + PropertyCount, ToPyObject(Methods[I]->Name));
	return List;
}
//---------------------------------------------------------------------------
static void VclObject_Dealloc(TVclObject* self)
{
	if(self->Owned)
	{
		if(TWinControl *Control = dynamic_cast<TWinControl*>(self->Instance))
			while(Control->ControlCount)
				Control->Controls[Control->ControlCount-1]->Parent = NULL;
		delete self->Instance;
	}
	Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
}
//---------------------------------------------------------------------------
PyObject* VclObject_GetAttro(TVclObject *self, PyObject *attr_name)
{
	try
	{
		TObject *Object = self->Instance;
		String Name = PyUnicode_AsUnicode(attr_name);
		TRttiType *Type = Context.GetType(Object->ClassType());
		if(Type == NULL)
			throw EPyVclError("Type not found.");
		TRttiProperty *Property = Type->GetProperty(Name);
		if(Property == NULL)
		{
			DynamicArray<TRttiMethod*> Methods = Type->GetMethods(Name);
			if(Methods.get_length() != 0)
				return VclMethod_Create(Object, Methods);

			TRttiField *Field = Type->GetField(Name);
			if(Field == NULL || Field->Visibility != mvPublic)
			{
				PyObject *ArrayProperty = VclArrayProperty_Create(Object, Name);
				if(ArrayProperty != NULL)
					return ArrayProperty;
				PyObject *Result = PyObject_GenericGetAttr(reinterpret_cast<PyObject*>(self), attr_name);
				if(Result == NULL)
					SetErrorString(PyExc_AttributeError, "'" + Type->Name + "' object has no attribute '" + Name + "'");
				return Result;
			}
			TValue Result = Field->GetValue(Object);
			return ToPyObject(Result);
		}
		TValue Result = Property->GetValue(Object);
		return ToPyObject(Result);
	}
	catch(EListError &E)
	{
		PyErr_SetString(PyExc_IndexError, AnsiString(E.Message).c_str());
		return NULL;
	}
	catch(Exception &E)
	{
		SetErrorString(PyVclException, E.Message);
		return NULL;
	}
}
//---------------------------------------------------------------------------
int VclObject_SetAttro(TVclObject *self, PyObject *attr_name, PyObject *v)
{
	try
	{
		String Name = PyUnicode_AsUnicode(attr_name);
		TRttiType *Type = Context.GetType(self->Instance->ClassType());
		TRttiProperty *Property = Type->GetProperty(Name);
		TTypeInfo *TypeInfo = Property->PropertyType->Handle;
		if(Property == NULL)
		{
			int Result = PyObject_GenericSetAttr(reinterpret_cast<PyObject*>(self), attr_name, v);
			if(Result == -1)
				SetErrorString(PyExc_AttributeError, "'" + Type->Name + "' object has no attribute '" + Name + "'");
			return Result;
		}

		TValue Value;
		if(TypeInfo->Kind == tkMethod)
		{
			if(v == Py_None)
				Value = TValue::From<TObject*>(NULL);
			else
			{
				Py_INCREF(v);
				TMethodImplementation *Implementation = new TMethodImplementation(v, CreateInvokeInfo(TypeInfo), PythonCallback);
				TMethod Method = {Implementation->CodeAddress, NULL};
				TValue::Make(&Method, TypeInfo, Value);
				if(TComponent *Component = dynamic_cast<TComponent*>(self->Instance))
					new TImplementationOwner(Component, Implementation);
			}
		}
		else
			Value = ToValue(v, TypeInfo);
		Property->SetValue(self->Instance, Value);
		return 0;
	}
	catch(Exception &E)
	{
		SetErrorString(PyVclException, E.Message);
		return -1;
	}
}
//---------------------------------------------------------------------------
static PyMemberDef VclObject_Members[] =
{
	{"_owned", T_BOOL, offsetof(TVclObject, Owned), 0, "Indicates if the VCL object is freed when the proxy is destroyed"},
	{NULL, 0, 0, 0, NULL}
};
//---------------------------------------------------------------------------
static PyMethodDef VclObject_Methods[] =
{
	{"__dir__", (PyCFunction)VclObject_Dir, METH_NOARGS, ""},
	{NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
PyTypeObject VclObjectType =
{
	PyObject_HEAD_INIT(NULL)
	"vcl.VclObject",        	 /* tp_name */
	sizeof(TVclObject),        /* tp_basicsize */
	0,                         /* tp_itemsize */
	(destructor)VclObject_Dealloc, /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_compare */
	(reprfunc)VclObject_Repr,  /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash */
	0, 												 /* tp_call */
	0,                         /* tp_str */
	(getattrofunc)VclObject_GetAttro, /* tp_getattro */
	(setattrofunc)VclObject_SetAttro, /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 			 /* tp_flags */
	"VCL object",       			 /* tp_doc */
	0,		                     /* tp_traverse */
	0,		                     /* tp_clear */
	0,		                     /* tp_richcompare */
	0,		                     /* tp_weaklistoffset */
	0,		                     /* tp_iter */
	0,		                     /* tp_iternext */
	VclObject_Methods,         /* tp_methods */
	VclObject_Members,         /* tp_members */
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
PyObject* VclObject_Create(TObject *Instance, bool Owned)
{
	TVclObject *VclObject = PyObject_New(TVclObject, &VclObjectType);
	VclObject->Instance = Instance;
	VclObject->Owned = Owned;
	return reinterpret_cast<PyObject*>(VclObject);
}
//---------------------------------------------------------------------------
bool VclObject_Check(PyObject *O)
{
	return O->ob_type->tp_repr == VclObjectType.tp_repr;
}
//---------------------------------------------------------------------------
TObject* VclObject_AsObject(PyObject *O)
{
	if(O->ob_type->tp_repr != VclObjectType.tp_repr)
		throw EPyVclError("Object is not a VclObject type");
	return reinterpret_cast<TVclObject*>(O)->Instance;
}
//---------------------------------------------------------------------------
} //namespace Python
