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
#include "PyVcl.h"
#include <ActnMan.hpp>
#undef _DEBUG
#include <python.h>
#include "PythonBind.h"
#include "ExtColorBox.h"
#include <Rtti.hpp>
#include "ClassList.h"
#include <OleCtrls.hpp>

#define NAME_VALUE_ENTRY(x) {_TEXT(#x), (TFastcallFunction)x}

TRttiContext Context;
namespace Python
{
//---------------------------------------------------------------------------
PyObject *PyPropertyException = NULL;
PyObject *PyVclException = NULL;
//---------------------------------------------------------------------------
template<class T> T VclCast(TObject *Object)
{
  if(T O = dynamic_cast<T>(Object))
    return O;
  throw EConvertError(AnsiString().sprintf("Cannot convert object of type '%s' to '%s'",
    AnsiString(Object->ClassName()).c_str(), typeid(T).name()));  
}
//---------------------------------------------------------------------------
struct TPythonCallback :
	public TComObject,
	public TMethodImplementationCallback
{
	HRESULT __stdcall QueryInterface(const GUID &IID, void **Obj)
	{
		return TComObject::QueryInterface(IID, Obj);
	}

	unsigned long __stdcall AddRef()
	{
		return TComObject::_AddRef();
	}

	unsigned long __stdcall Release()
	{
		return TComObject::_Release();
	}

	void __fastcall Invoke(void * UserData, const System::DynamicArray<TValue> Args, TValue &Result)
	{

	}
};
//---------------------------------------------------------------------------
TValue ToValue(PyObject *O)
{
	if(PyBool_Check(O))
		return TValue::_op_Implicit(O != Py_False);
	if(PyLong_Check(O))
		return TValue::From(PyLong_AsLong(O));
	if(PyUnicode_Check(O))
		return TValue::From(String(PyUnicode_AsUnicode(O)));
	if(PyFunction_Check(O))
		return TValue::From(new TPythonCallback);
	throw Exception("Invalid type " + String(O->ob_type->tp_name));
}
//---------------------------------------------------------------------------
void TuppleToValues(PyObject *O, std::vector<TValue> &Values)
{
	Values.clear();
	unsigned Size = PyTuple_Size(O);
	for(unsigned I = 0; I < Size; I++)
		Values.push_back(ToValue(PyTuple_GetItem(O, I)));
}
//---------------------------------------------------------------------------
static PyObject* VclFindClass(PyObject *Self, PyObject *Args)
{
	try
	{
		const wchar_t *Name;
		if(!PyArg_ParseTuple(Args, "u", &Name))
			return NULL;

		TMetaClass *MetaClass = LookUpClass(Name);
		TRttiType *Type = Context.GetType(MetaClass);
		return Py_BuildValue("i", Type); //Return pointer to meta class, NULL on failure
	}
	catch(Exception &E)
	{
		PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
		return NULL;
	}
}
//---------------------------------------------------------------------------
static PyObject* VclDeleteObject(PyObject *Self, PyObject *Args)
{
	try
	{
    TObject *Object;
    if(!PyArg_ParseTuple(Args, "i", &Object))
      return NULL;

		delete Object;
		Py_RETURN_NONE;
	}
	catch(Exception &E)
	{
		PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
		return NULL;
	}

}
//---------------------------------------------------------------------------
static PyObject* VclSetProperty(PyObject *Self, PyObject *Args)
{
	try
	{
		TObject *Object;
		const wchar_t *Name;
		PyObject *Value = NULL;
		if(!PyArg_ParseTuple(Args, "iuO", &Object, &Name, &Value))
			return NULL;
		TRttiType *Type = Context.GetType(Object->ClassType());
		TRttiProperty *Property = Type->GetProperty(Name);
		if(Property == NULL)
			throw Exception("Property " + String(Name) + " not found");
		Property->SetValue(Object, ToValue(Value));
		Py_RETURN_NONE;
	}
	catch(Exception &E)
	{
		PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
		return NULL;
	}
}
//---------------------------------------------------------------------------
static PyObject* VclGetProperty(PyObject *Self, PyObject *Args)
{
	try
	{
		TObject *Object;
		const wchar_t *Name;
		if(!PyArg_ParseTuple(Args, "iu", &Object, &Name))
			return NULL;

		TRttiType *Type = Context.GetType(Object->ClassType());
		TRttiProperty *Property = Type->GetProperty(Name);
		if(Property == NULL)
		{
			TRttiMethod *Method = Type->GetMethod(Name);
			if(Method == NULL)
				throw Exception("Property " + String(Name) + " not found");
			return Py_BuildValue("ii", Method, 10);
		}
		TValue Result = Property->GetValue(Object);
		return Py_BuildValue("Ni", ToPyObject(Result), 0);
	}
	catch(Exception &E)
	{
		PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
		return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* VclGetPropertyList(PyObject *Self, PyObject *Args)
{
  TObject *Object = reinterpret_cast<TObject*>(PyLong_AsLong(Args));
  if(PyErr_Occurred())
		return NULL;

	PPropList PropList;
	int Count = GetPropList(Object, PropList);
	PyObject *List = PyList_New(Count);
	for(int I = 0; I < Count; I++)
		PyList_SetItem(List, I, PyUnicode_FromString(AnsiString((*PropList)[I]->Name).c_str()));
	return List;
}
//---------------------------------------------------------------------------
static PyObject* VclCallMethod(PyObject *Self, PyObject *Args)
{
	try
	{
		TObject *Object;
		const wchar_t *Name;
		if(!PyArg_ParseTuple(Args, "iu", &Object, &Name))
			return NULL;

		TRttiType *Type = dynamic_cast<TRttiType*>(Object);
		if(Type == NULL)
			Type = Context.GetType(Object->ClassType());
		TRttiMethod *Method = Type->GetMethod(Name);
		std::vector<TValue> Parameters;
		TuppleToValues(Args, Parameters);
		if(Method->IsConstructor)
		{
			TValue Param[] = {TValue::From(Application)};
			TValue Result = Method->Invoke(Type->AsInstance->MetaclassType, Param, 0);
			return Py_BuildValue("i", Result.AsObject());
		}
		else
		{
			TValue Result;
			if(Method->IsClassMethod)
				Result = Method->Invoke(Object->ClassType(), Parameters.size() == 2 ? NULL : &Parameters[2], Parameters.size()-3);
			else
				Result = Method->Invoke(Object, Parameters.size() == 2 ? NULL : &Parameters[2], Parameters.size()-3);
			return ToPyObject(Result);
		}

		Py_RETURN_NONE;
	}
	catch(Exception &E)
	{
		PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
		return NULL;
	}
}
//---------------------------------------------------------------------------
typedef int __fastcall (*TFastcallFunction)(int,int);
struct TFunctionEntry
{
	const wchar_t *Name;
	TFastcallFunction Function;
};

TFunctionEntry FunctionList[] =
{
  NAME_VALUE_ENTRY(ShortCutToText),
  NAME_VALUE_ENTRY(TextToShortCut),
};
//---------------------------------------------------------------------------
static PyObject* VclCallFunction(PyObject *Self, PyObject *Args)
{
  try
  {
    const wchar_t *Name;
    PyObject *Arg1;
    PyObject *ResultType;
    if(!PyArg_ParseTuple(Args, "uOO", &Name, &ResultType, &Arg1))
      return NULL;
    TFastcallFunction Function = NULL;
    String FunctionName = Name;
		for(unsigned I = 0; I < sizeof(FunctionList) / sizeof(FunctionList[0]); I++)
      if(FunctionName == FunctionList[I].Name)
      {
        Function = FunctionList[I].Function;
        break;
      }

    if(Function == NULL)
      Py_RETURN_NONE;

    int pArg1 = 0, pArg2 = 0, pResult = 0;
    String Str1, Str2;
    if(PyUnicode_Check(Arg1))
    {
      Str1 = PyUnicode_AsUnicode(Arg1);
      pArg1 = (int)Str1.data();
    }
    else if(PyLong_Check(Arg1))
      pArg1 = PyLong_AsLong(Arg1);
    else
      Py_RETURN_NONE;

    if(ResultType == (PyObject*)&PyUnicode_Type)
      pArg2 = (int)&Str2;
    else if(ResultType == (PyObject*)&PyLong_Type)
      ;//pResult = (int)&Int;
    else
      Py_RETURN_NONE;

//    Int = TextToShortCut(Str1);
//    Str1 = ShortCutToText(Int);
    pResult = Function(pArg1, pArg2);

    if(ResultType == (PyObject*)&PyUnicode_Type)
			return PyUnicode_FromWideChar(Str2.c_str(), Str2.Length());
		else if(ResultType == (PyObject*)&PyLong_Type)
			return PyLong_FromLong(pResult);
		else
			Py_RETURN_NONE;
	}
	catch(Exception &E)
	{
		PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
		return NULL;
	}
}
//---------------------------------------------------------------------------
static PyMethodDef PyVclMethods[] = {
	{"FindClass", VclFindClass, METH_VARARGS, ""},
	{"DeleteObject", VclDeleteObject, METH_VARARGS, ""},
	{"SetProperty", VclSetProperty, METH_VARARGS, ""},
	{"GetProperty", VclGetProperty, METH_VARARGS, ""},
	{"CallMethod", VclCallMethod, METH_VARARGS, ""},
	{"GetPropertyList", VclGetPropertyList, METH_O, ""},
	{"CallFunction", VclCallFunction, METH_VARARGS, ""},
	{NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
static PyModuleDef PyVclModuleDef =
{
	PyModuleDef_HEAD_INIT,
	"PyVcl",
	NULL,
	-1,
  PyVclMethods,
  NULL,
  NULL,
  NULL,
  NULL,
};
//---------------------------------------------------------------------------
PyObject* InitPyVcl()
{
	PyObject *PyVclModule = PyModule_Create(&PyVclModuleDef);

  PyPropertyException = PyErr_NewException("PyVcl.PropertyError", NULL, NULL);
  Py_INCREF(PyPropertyException);
  PyModule_AddObject(PyVclModule, "PropertyError", PyPropertyException);

  PyVclException = PyErr_NewException("PyVcl.VclException", NULL, NULL);
  Py_INCREF(PyVclException);
  PyModule_AddObject(PyVclModule, "VclError", PyVclException);
  return PyVclModule;
}
//---------------------------------------------------------------------------
}
















































