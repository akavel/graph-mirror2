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
#include "PyVcl.h"
#include "python.hpp"
#include "PyVclConvert.h"
#include "FindClass.hpp"
#include "PyVClMethod.h"
#include "PyVClClosure.h"
#include "PyVclObject.h"
#include "PyVClType.h"
#include "PyVclFunction.h"
#include "PyVclIndexedProperty.h"
#include <RTLConsts.hpp>
#include "PyVclRef.h"

namespace Python
{
/** Replacement for Forms::TForm to allow creation using TForm::Create(TComponent*)
 *  instead of TForm::CreateNew(TComponent*, int).
 */
class TForm : public Forms::TForm
{
public:
  __fastcall TForm(TComponent *C) : Forms::TForm(C, 0) {}
};
//---------------------------------------------------------------------------
TObject* GetPlatform()
{
  _di_IInterface Interface = TPlatformServices::Current->GetPlatformService(__uuidof(IFMXScreenService));
  TObject *obj = NULL;
  //We should use interface_cast<>() but it fails to compile for Win64
  Interface->QueryInterface(__uuidof(__IObjCastGUIDHolder), reinterpret_cast<void**>(&obj));
  return obj;
}
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
	"Screen", Screen,
#if !FIREMONKEY
	"Mouse", Mouse,
	"Clipboard", Clipboard(),
#else
  "PlatformServices", TPlatformServices::Current,
  //Platform is a hack to access the object implementing all the interfaces
  //normally accessed through TPlatformServices
  "Platform", GetPlatform(),
#endif
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
		  SetErrorString(PyExc_AttributeError, GUI_NAME " has no global attribute '" + Name + "'");
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
  Forms::TForm *Form = new Forms::TForm(NULL, 0);
  return VclObject_Create(Form, true);
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
static PyTypeObject VclModule_Type =
{
	PyObject_HEAD_INIT(NULL)
	GUI_NAME "module",         /* tp_name */
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
#if FIREMONKEY
	"Module wrapping the FireMonkey (FMX) library", /* tp_doc */
#else
	"Module wrapping the Visual Component Library (VCL)", /* tp_doc */
#endif
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
	GUI_NAME,           //m_name
#if FIREMONKEY
  "Module wrapping the FireMonkey (FMX) library", //m_doc
#else
	"Module wrapping the Visual Component Library (VCL)", //m_doc
#endif
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
#if FIREMONKEY
  RegisterType(__delphirtti(Python::TForm)); //Register our replacement TForm
#endif
  //VclModuleType derives from PyModule_Type and VclModuleType must therefore have
  //the same size.
  VclModule_Type.tp_basicsize = PyModule_Type.tp_basicsize;
  VclModule_Type.tp_base = &PyModule_Type;
	if(PyType_Ready(&VclModule_Type) < 0 || PyType_Ready(&VclMethod_Type) < 0 ||
		PyType_Ready(&VclObject_Type) < 0 || PyType_Ready(&VclFunction_Type) < 0 ||
		PyType_Ready(&VclIndexedProperty_Type) < 0 || PyType_Ready(&VclRef_Type) < 0 ||
    PyType_Ready(&VclClosure_Type) < 0)
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

	PyVclException = PyErr_NewException(FIREMONKEY ? "fmx.FmxError" : "vcl.VclError", NULL, NULL);
  Py_INCREF(PyVclException);
	PyModule_AddObject(PyVclModule, FIREMONKEY ? "FmxError" : "VclError", PyVclException); //This steals a reference

  //Nasty hack: Change vcl module to be an instance of VclModuleType instead of
  //PyModule_Type (module).
  PyVclModule->ob_type = &VclModule_Type;
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

