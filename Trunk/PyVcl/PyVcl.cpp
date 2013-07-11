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
#include "PyVcl.h"
#include <ActnMan.hpp>
#include "python.hpp"
#include <structmember.h>
#include "PyVclConvert.h"
#include "FindClass.hpp"
#include <OleCtrls.hpp>
#include <clipbrd.hpp>
#include "PyVClMethod.h"
#include "PyVclObject.h"
#include "PyVClType.h"
#include "PyVclFunction.h"
#include "PyVclArrayProperty.h"
#include <RTLConsts.hpp>
#include "PyVclRef.h"

namespace Python
{
//---------------------------------------------------------------------------
struct TGlobalObjectEntry
{
	const char *Name;
	TObject *Object;
};

/** List of global VCL objects that should be permanently mapped in PyVcl.vcl
 */
static TGlobalObjectEntry GlobalObjectList[] =
{
	"Application", Application,
	"Mouse", Mouse,
	"Clipboard", Clipboard(),
	"Screen", Screen,
};
//---------------------------------------------------------------------------
/** Retrieve a list of available variables, types and functions in PyVcl.vcl.
 *  \return New reference
 */
static PyObject* VclModule_Dir(PyObject *self, PyObject *arg)
{
	unsigned GlobalCount = sizeof(GlobalObjectList)/sizeof(GlobalObjectList[0]);
	unsigned GlobalFunctionCount = GetVclFunctionCount();
	TStrings *TypeList = GetTypeList();
	PyObject *List = PyList_New(TypeList->Count + GlobalCount + GlobalFunctionCount);
	int Index;
	for(Index = 0; Index < TypeList->Count; Index++)
		PyList_SetItem(List, Index, ToPyObject(TypeList->Strings[Index]));
	for(unsigned I = 0; I < GlobalCount; I++, Index++)
		PyList_SetItem(List, Index, ToPyObject(GlobalObjectList[I].Name));
	for(unsigned I = 0; I < GlobalFunctionCount; I++, Index++)
		PyList_SetItem(List, Index, ToPyObject(GetVclFunctionName(I)));
	return List;
}
//---------------------------------------------------------------------------
/** Retrieve a type, function or global variable from the global vcl object.
 *  The name is searched in this ordier:
 *  1. Check the cache if the object already exists and used it if it does.
 *     Global variables as Application always exist here.
 *  2. Search for type in custom list of available types.
 *  3. Try to retrieve it as a registered class in Delphi.
 *  4. Try to create it as a global function.
 *  At last the object is stored in the cache for fast retrievel next time.
 *  A Python exception is thrown if no variable, type or function with the given name is found.
 *  \return New reference
 */
static PyObject* VclModule_GetAttro(PyObject *self, PyObject *attr_name)
{
	try
	{
		PyObject *Result = PyObject_GenericGetAttr(self, attr_name);
		if(Result != NULL)
			return Result;

		PyErr_Clear();
		String Name = FromPyObject<String>(attr_name);
		TRttiType *Type = Context.GetType(LookUpClass(Name));
		if(Type == NULL)
		{
			TClass Class = GetClass(Name);
			if(Class)
				Type = Context.GetType(Class);
    }
		if(Type != NULL)
			Result = VclType_Create(Type);
		else
			Result = VclFunction_Create(Name);
		if(Result == NULL)
    {
  		SetErrorString(PyExc_AttributeError, "VCL has no global attribute '" + Name + "'");
      return NULL;
    }

		PyObject_GenericSetAttr(self, attr_name, Result);
		return Result;
	}
	catch(...)
	{
		return PyVclHandleException();
	}
}
//---------------------------------------------------------------------------
/** Replacement for TApplication::CreateForm() that doesn't fit into the system
 *  \return New reference to VclObject with a weak reference to a newly created TForm.
 */
static PyObject* VclModule_CreateForm(PyObject *self, PyObject *arg)
{
  TForm *Form;
  Application->CreateForm(__classid(TForm), &Form);
  return VclObject_Create(Form, false);
}
//---------------------------------------------------------------------------
static PyMethodDef VclModule_Methods[] =
{
	{"CreateForm", VclModule_CreateForm, METH_NOARGS, "Replacement for TApplication::CreateForm()"},
	{"__dir__", VclModule_Dir, METH_NOARGS, ""},
	{NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
/** This type derives from PyModule_Type (module in Python) and is used as the class
 *  of the vcl module to make it possible to set tp_getattro.
 */
static PyTypeObject VclModuleType =
{
	PyObject_HEAD_INIT(NULL)
	"vclmodule",        	 		 /* tp_name */
	0, /* Initialized later */ /* tp_basicsize */
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
	VclModule_GetAttro,   		 /* tp_getattro */
	PyObject_GenericSetAttr,	 /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 			 /* tp_flags */
	"Module wrapping the Visual Component Library (VCL)", /* tp_doc */
	0,		                     /* tp_traverse */
	0,		                     /* tp_clear */
	0,		                     /* tp_richcompare */
	0,		                     /* tp_weaklistoffset */
	0,		                     /* tp_iter */
	0,		                     /* tp_iternext */
	VclModule_Methods,         /* tp_methods */
	0,                         /* tp_members */
	0,       									 /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	0,						             /* tp_init */
	0,                         /* tp_alloc */
	0,						             /* tp_new */
};
//---------------------------------------------------------------------------
static PyModuleDef PyVclModuleDef =
{
	PyModuleDef_HEAD_INIT,
	"vcl",           //m_name
	"Module wrapping the Visual Component Library (VCL)", //m_doc
	-1, //m_size
	NULL, //m_methods
	NULL, //m_reload
	NULL, //m_traverse
	NULL, //m_clear
	NULL, //m_free
};
//---------------------------------------------------------------------------
/** Initialize PyVcl. This creates the vcl module, which is an instance of VclModuleType
 *  that is derived from the module class.
 *  \return New reference to the vcl module.
 */
PyObject* InitPyVcl()
{
  //VclModuleType derives from PyModule_Type and VclModuleType must therefore have
  //the same size.
  VclModuleType.tp_basicsize = PyModule_Type.tp_basicsize;
  VclModuleType.tp_base = &PyModule_Type;
	if(PyType_Ready(&VclModuleType) < 0 || PyType_Ready(&VclMethodType) < 0 ||
		PyType_Ready(&VclObjectType) < 0 || PyType_Ready(&VclFunctionType) < 0 ||
		PyType_Ready(&VclArrayPropertyType) < 0 || PyType_Ready(&VclRefType) < 0)
		return NULL;

  //Module must be created by PyModule_Create().
  //See http://bugs.python.org/issue18426
	PyObject *PyVclModule = PyModule_Create(&PyVclModuleDef);
	for(unsigned I = 0; I < sizeof(GlobalObjectList)/sizeof(GlobalObjectList[0]); I++)
	{
		PyObject *NewObject = VclObject_Create(GlobalObjectList[I].Object, false);
		PyObject_SetAttrString(PyVclModule, GlobalObjectList[I].Name, NewObject);
		Py_DECREF(NewObject);
	}

	PyVclException = PyErr_NewException("vcl.VclError", NULL, NULL);
  Py_INCREF(PyVclException);
	PyModule_AddObject(PyVclModule, "VclError", PyVclException); //This steals a reference

  //Nasty hack: Change vcl module to be an instance of VclModuleType instead of
  //PyModule_Type (module).
  PyVclModule->ob_type = &VclModuleType;
	return PyErr_Occurred() ? NULL : PyVclModule;
}
//---------------------------------------------------------------------------
} //Namespace Python

//Workaround: Instead of registering all classes with RegisterClass() we change
//TStream::ReadComponent() to call TComponentFinder::FindComponentClass() to
//retrieve the meta class.
class TComponentFinder
{
public:
	void __fastcall FindComponentClass(TReader *Reader, String ClassName, TComponentClass &ComponentClass)
	{
		if(ComponentClass == NULL)
		{
			TTypeInfo *TypeInfo = LookUpClass(ClassName);
			if(TypeInfo != NULL && TypeInfo->Kind == tkClass)
				ComponentClass = GetTypeData(TypeInfo)->ClassType;
		}
	}
};
TComponentFinder *ComponentFinder = new TComponentFinder;
#if __BORLANDC__ > 0x0640
namespace System {
#endif
namespace Classes
{
TComponent* __fastcall TStream::ReadComponent(TComponent *Instance)
{
	std::auto_ptr<TReader> Reader(new TReader(this, 4096));
	Reader->OnFindComponentClass = ComponentFinder->FindComponentClass;
	return Reader->ReadRootComponent(Instance);
}
}
#if __BORLANDC__ > 0x0640
}
#endif
//---------------------------------------------------------------------------

