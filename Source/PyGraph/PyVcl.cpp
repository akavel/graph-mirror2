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
#define private public
#include <Rtti.hpp>
#undef private
#include "PyVcl.h"
#include <ActnMan.hpp>
#undef _DEBUG
#include <python.h>
#include <structmember.h>
#include "PythonBind.h"
#include "ExtColorBox.h"
#include "FindClass.hpp"
#include <OleCtrls.hpp>
#include <clipbrd.hpp>
#include "PyVClMethod.h"
#include "PyVclObject.h"
#include "PyVClType.h"
#include "PyVclFunction.h"
#include "PyVclArrayProperty.h"
#include <RTLConsts.hpp>

namespace Python
{
//---------------------------------------------------------------------------
TRttiContext Context;
PyObject *PyVclException = NULL;
TValue ToValue(PyObject *O, TTypeInfo *TypeInfo);
//---------------------------------------------------------------------------
struct TGlobalObjectEntry
{
	const char *Name;
	TObject *Object;
};
TGlobalObjectEntry GlobalObjectList[] =
{
	"Application", Application,
	"Mouse", Mouse,
	"Clipboard", Clipboard(),
	"Screen", Screen,
};
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
TMethodImplementation::TInvokeInfo* CreateInvokeInfo(TTypeInfo *TypeInfo)
{
	TTypeData *TypeData = reinterpret_cast<TTypeData*>(&TypeInfo->Name[TypeInfo->Name[0]+1]);
	int Index = 0;
	for(int I = 0; I < TypeData->ParamCount; I++)
	{
		Index++;
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
		InvokeInfo->AddParameter(ParamType, false);
		Index += 4;
	}
	InvokeInfo->Seal();
	return InvokeInfo;
}
//---------------------------------------------------------------------------
TValue ToValue(PyObject *O, TTypeInfo *TypeInfo)
{
	TValue Result;
	switch(TypeInfo->Kind)
	{
		case tkClass:
			if(VclObject_Check(O))
				Result = TValue::From(VclObject_AsObject(O));
			else if(O == Py_None || PyLong_Check(O))
				Result = TValue::From((TObject*)(O == Py_None ? NULL : PyLong_AsLong(O)));
			else
				throw EPyVclError("Cannot convert Python object of type '" + String(O->ob_type->tp_name) + "' to '" + AnsiString(TypeInfo->Name) + "'");
			break;

		case tkEnumeration:
			if(PyUnicode_Check(O))
				TValue::Make(GetEnumValue(TypeInfo, PyUnicode_AsUnicode(O)), TypeInfo, Result);
			if(PyLong_Check(O))
				TValue::Make(PyLong_AsLong(O), TypeInfo, Result);
			break;

		case tkSet:
			TValue::Make(StringToSet(TypeInfo, PyUnicode_AsUnicode(O)), TypeInfo, Result);
			break;

		case tkInteger:
			Result = TValue::From(PyLong_AsLong(O));
			break;

		case tkUString:
		case tkString:
		case tkLString:
		case tkWString:
			Result = TValue::From(String(PyUnicode_AsUnicode(O)));
			break;

		case tkMethod:
		{
			if(O == Py_None)
				return TValue::From<TObject*>(NULL);
			Py_INCREF(O);
			TMethodImplementation *Implementation = new TMethodImplementation(O, CreateInvokeInfo(TypeInfo), PythonCallback);
			TMethod Method = {Implementation->CodeAddress, NULL};
			TValue::Make(&Method, TypeInfo, Result);
			break;
		}
		case tkChar:
		case tkWChar:
			if(PyUnicode_GetSize(O) != 1)
				throw EPyVclError("Expected string with one character");
			Result = TValue::From(PyUnicode_AsUnicode(O)[0]);
			break;

		case tkFloat:
			Result = TValue::From(PyFloat_AsDouble(O));
			break;

		case tkVariant:
		case tkArray:
		case tkRecord:
		case tkInterface:
		case tkInt64:
		case tkDynArray:
		case tkClassRef:
		case tkPointer:
		case tkProcedure:
		case tkUnknown:
		default:
			throw EPyVclError("Cannot convert Python object of type '" + String(O->ob_type->tp_name) + "' to '" + AnsiString(TypeInfo->Name) + "'");
	}
	if(PyErr_Occurred())
		throw EPyVclError("Cannot convert Python object of type '" + String(O->ob_type->tp_name) + "' to '" + AnsiString(TypeInfo->Name) + "'");
	return Result;
}
//---------------------------------------------------------------------------
void TupleToValues(PyObject *O, std::vector<TValue> &Values, const DynamicArray<TRttiParameter*> &Parameters)
{
	Values.clear();
	unsigned Size = PyTuple_Size(O);
	for(unsigned I = 0; I < Size; I++)
		Values.push_back(ToValue(PyTuple_GetItem(O, I), Parameters[I]->ParamType->Handle));
}
//---------------------------------------------------------------------------
static PyObject* GlobalVcl_Dir(TVclObject *self, PyObject *arg)
{
	unsigned GlobalCount = sizeof(GlobalObjectList)/sizeof(GlobalObjectList[0]);
	TStrings *TypeList = GetTypeList();
	PyObject *List = PyList_New(TypeList->Count);
	for(int I = 0; I < TypeList->Count; I++)
		PyList_SET_ITEM(List, I, ToPyObject(TypeList->Strings[I]));
	return List;
}
//---------------------------------------------------------------------------
static PyObject* GlobalVcl_GetAttro(PyObject *self, PyObject *attr_name)
{
	try
	{
		PyObject *Result = PyObject_GenericGetAttr(self, attr_name);
		if(Result != NULL)
			return Result;

		PyErr_Clear();
		String Name = PyUnicode_AsUnicode(attr_name);
		TRttiType *Type = Context.GetType(LookUpClass(Name));
		if(Type != NULL)
		{
//		if(Class->InheritsFrom(__classid(Exception)))
			Result = VclType_Create(Type);
		}
		else
			Result = VclFunction_Create(Name);
		if(Result == NULL)
			throw EPyVclError("VCL has no global attribute '" + Name + "'");

		PyObject_GenericSetAttr(self, attr_name, Result);
		return Result;
	}
	catch(Exception &E)
	{
		SetErrorString(PyVclException, E.Message);
		return NULL;
	}
}
//---------------------------------------------------------------------------
struct TGlobalVcl
{
	PyObject_HEAD
	PyObject *Dict;
};
//---------------------------------------------------------------------------
static PyMemberDef GlobalVcl_Members[] =
{
	{"__dict__", T_OBJECT, offsetof(TGlobalVcl, Dict), READONLY},
	{NULL, 0, 0, 0}
};
//---------------------------------------------------------------------------
static PyMethodDef GlobalVcl_Methods[] =
{
	{"__dir__", (PyCFunction)GlobalVcl_Dir, METH_NOARGS, ""},
	{NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
static PyTypeObject GlobalVclType =
{
	PyObject_HEAD_INIT(NULL)
	"vcl.Vcl",        	 			 /* tp_name */
	sizeof(TGlobalVcl), 			 /* tp_basicsize */
	0,                         /* tp_itemsize */
	0, 												 /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_compare */
	0,  											 /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash */
	0, 												 /* tp_call */
	0,                         /* tp_str */
	GlobalVcl_GetAttro,   		 /* tp_getattro */
	PyObject_GenericSetAttr,	 /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 			 /* tp_flags */
	"Global VCL types and methods", /* tp_doc */
	0,		                     /* tp_traverse */
	0,		                     /* tp_clear */
	0,		                     /* tp_richcompare */
	0,		                     /* tp_weaklistoffset */
	0,		                     /* tp_iter */
	0,		                     /* tp_iternext */
	GlobalVcl_Methods,         /* tp_methods */
	GlobalVcl_Members,         /* tp_members */
	0,       									 /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	offsetof(TGlobalVcl, Dict),/* tp_dictoffset */
	0,						             /* tp_init */
	0,                         /* tp_alloc */
	0,						             /* tp_new */
};
//---------------------------------------------------------------------------
static PyModuleDef PyVclModuleDef =
{
	PyModuleDef_HEAD_INIT,
	"PyVcl",           //m_name
	"Module wrapping the Visual Component Library (VCL)", //m_doc
	-1, //m_size
	NULL, //m_methods
	NULL, //m_reload
	NULL, //m_traverse
	NULL, //m_clear
	NULL, //m_free
};
//---------------------------------------------------------------------------
PyObject* GlobalVcl_Create()
{
	TGlobalVcl *vcl = PyObject_New(TGlobalVcl, &GlobalVclType);
	vcl->Dict = NULL;
	PyObject *self = reinterpret_cast<PyObject*>(vcl);
	for(unsigned I = 0; I < sizeof(GlobalObjectList)/sizeof(GlobalObjectList[0]); I++)
	{
		PyObject *NewObject = VclObject_Create(GlobalObjectList[I].Object, false);
		PyObject_SetAttrString(self, GlobalObjectList[I].Name, NewObject);
		Py_DECREF(NewObject);
	}
	return self;
}
//---------------------------------------------------------------------------
PyObject* InitPyVcl()
{
	PyObject *PyVclModule = PyModule_Create(&PyVclModuleDef);
	if(PyType_Ready(&GlobalVclType) < 0 || PyType_Ready(&VclMethodType) < 0 ||
		PyType_Ready(&VclObjectType) < 0 || PyType_Ready(&VclFunctionType) < 0 ||
		PyType_Ready(&VclArrayPropertyType) < 0)
		return NULL;
	PyVclException = PyErr_NewException("vcl.VclError", NULL, NULL);
	PyModule_AddObject(PyVclModule, "VclError", PyVclException);

	PyObject *Modules = PyImport_GetModuleDict();
	PyObject *vcl = GlobalVcl_Create();
	PyDict_SetItemString(Modules, "vcl", vcl);
	Py_DECREF(vcl);
	return PyVclModule;
}
//---------------------------------------------------------------------------
} //Namespace Python
/*namespace Classes
{
//Instead of registering all classes with RegisterClass() we change GetClass()
//to use our own LookUpClass() which is much faster
PACKAGE TPersistentClass __fastcall FindClass(const System::UnicodeString ClassName)
{
	TTypeInfo *TypeInfo = LookUpClass(ClassName);
	if(TypeInfo && TypeInfo->Kind == tkClass)
	{
		TMetaClass *MetaClass = GetTypeData(TypeInfo)->ClassType;
		if(MetaClass->InheritsFrom(__classid(TPersistent)))
			return MetaClass;
	}
	throw EClassNotFound(Rtlconsts_SClassNotFound, ARRAYOFCONST((ClassName)));
}
}*/ //namespace Classes
//---------------------------------------------------------------------------

